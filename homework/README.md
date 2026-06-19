# 关于作业

## 一些提醒

这里是 `main` 分支的作业，有本人按题目要求进行改动的部分，并未放弃跟踪老师源文件中的 `Debug/` 和 `Release/` 等编译产物。

如想查看与源文件哪里不同，或干脆想自己手动改一遍，请查看 `teacher-origin` 分支而不是本分支。

本课程[线上学习资源](https://www.bilibili.com/video/BV16kcBzwEdC)

## 完成思路

以下说明按 `diff teacher-origin..main` 整理，只记录与程序行为有关的源码改动。

公共硬件约定：

- `LED1`、`LED2`、`LED3`、`LED4` 分别是 `GPIO_Number_0`、`GPIO_Number_1`、`GPIO_Number_2`、`GPIO_Number_3`。也就是板上的四个 LED 接在 GPIO0~GPIO3。
- `LED_on()` 内部调用 `GPIO_setLow()`，`LED_off()` 内部调用 `GPIO_setHigh()`，所以这些 LED 是低电平点亮、高电平熄灭。
- `KEY1` 是 `GPIO_Number_12`。`KEYPRESSED` 定义为 `1`，表示读到按键按下。
- `Delay(...)`、`Delay1(...)` 这类函数是软件空循环延时，参数是循环次数，不是严格的毫秒数；实际时间会受主频、编译优化和运行环境影响。

### chap4

目标：在老师 GPIO 实验代码基础上，清理无关调试内容，保留 LED1+LED2 交替闪烁。

`main.c`
- 删除 `uint16_t *p;` 和 `p=(uint16_t *)0x8000` 等裸地址操作。
- 删除 GPIO5 的配置（`GPIO_setMode/setDirection`），非本题所需。
- 删除主循环中 `*(p+1)=...`、`myGpio->GPADAT=...` 等注释掉的调试代码。
- 保留 `for(;;){ LED_Control(); }`。

`Application/app.c`
- 未改动。老师原有代码已实现 LED1+LED2 交替闪烁。

### chap5

目标：修复按键扫描去抖逻辑，实现短按/长按/双击三种按键识别，用于切换 LED 显示模式。

`User_Component/KEY/KEY.c`
- `KEY_initial()` 新增 `key_status = KEY_PRESS_NO` 和 `key_counter/key_counter1/key_counter2 = 0`，确保上电后状态确定。
- `KEY_STATUS()` 重写为完整状态机：
  - 按下时记录 `Flag=1` 并计数 `key_counter1`。
  - 松开时根据 `key_counter1` 判断：>=10000 为长按，>=600 进入短按/双击判断。
  - 双击窗口由 `Flag1` 和 `key_counter2` 管理，超时 8000 个循环确认为单次短按。
- `KEY_STATUS_1()` 改用独立的 `Flag2` 和 `key_counter`，避免与 `KEY_STATUS()` 共享 `Flag`。

`Application/app.c`
- `KEY_Control_LED_1()`：短按设 `KeyCount=0`，长按设 `KeyCount=1`，双击 `KeyCount++`，取代原来的 `LED_toggle`。
- `KEY_Control_LED_2()` 不变，根据 `KeyCount % 2` 显示两种 LED 模式。

`main.c`
- 启用 `KEY_STATUS()` + `KEY_Control_LED_1()` + `KEY_Control_LED_2()` 路径。
- 注释掉简单的 `KEY_Control_LED()`。

### chap6

目标：使用 PIE 外部中断（XINT1）触发按键处理，按键循环切换三种 LED 显示模式。

`User_Component/key_GPIO/key_Gpio.c`
- `KEY_GPIO_initial()` 新增 `key_counter=0` 和 `KeyCount=0` 的初始化。

`Application/app.c`
- 未改动。老师原有 `KEY_ID()` 已将按键次数映射为 `key_status` 的 1/2/3 循环，`KEY_Control_LED()` 和 `KEY_Control_LED1()` 根据 `key_status` 选择对应的 `LED_DISPLAY`。

`main.c`
- 未改动。老师原有代码已正确调用 `KEY_GPIO_*` 初始化、PIE 配置和 `KEY_Control_LED1()`。

### chap7

目标：使用 CPU Timer0 定时中断驱动 LED 流水灯，每个 LED 亮约 1 秒。

`User_Component/myTimer/myTimer.c`
- `TIMER_setDecimationFactor()` 修正为 `TIMER_setPreScaler()`，参数 0 表示 1 分频。

`User_Component/LED_Gpio/LED_Gpio.c`
- `LED_GPIO_initial()` 新增 `LED_Count=0` 和 `LED_on(LED[LED_Count])`，使上电后 LED1 先亮。

`Application/isr.c`
- 未改动。老师原有 `myTimer_CpuTimer0_isr` 已实现流水灯逻辑。

`main.c`
- 未改动。老师原有初始化顺序包含 LED_GPIO、KEY、myTimer、PIE。

### chap9

目标：使用 ePWM1A 输出 PWM 驱动 LED1 呼吸灯，EPWM1 中断周期性改变占空比。

`User_Component/LED_Pwm/LED_Pwm.h`
- 新增 `#define PWM_BREATH_PERIOD 60000U` 和 `#define PWM_BREATH_STEP 100U`，集中管理周期和步长。

`User_Component/LED_Pwm/LED_Pwm.c`
- `LED_PWM_initial()` 新增 `PWM_setCmpA(myPwm1, myCmpA)`，将初始值写入硬件。
- `LED_PWM_functionConfigure()` 中 `PWM_setPeriod` 改用 `PWM_BREATH_PERIOD` 宏。

`Application/isr.c`
- `LED_EPWM1_isr()` 中 `direction` 类型改为 `static uint16_t`。
- 呼吸范围从 0~20000（步长 5）改为 0~`PWM_BREATH_PERIOD`（步长 `PWM_BREATH_STEP`），覆盖全占空比。
- 变亮/变暗分支加边界保护，避免 `uint16_t` 下溢绕回 65535。
- 删除重复的 `PWM_clearIntFlag` 和 `PIE_clearInt` 调用。
- 修正 PIE 组注释：group 1 → group 3。

`main.c`、`User_Component/User_Pie/User_Pie.c`、`User_Component/User_System/User_System.c`
- 未改动。老师原有代码已包含必要的 PWM 模块初始化、PIE 中断注册（Timer0+EPWM1+ECAP）和外设时钟使能。Timer0 和 ECAP 虽在 PIE 中注册，但 Timer0 时钟未使能且 main 中未初始化，不会产生实际中断。

### chap8 / chap10 / chap11

这三个章节（CAP 捕获、ADC 模数转换、SCI 串口通信）未做，代码保持 `teacher-origin` 分支原样。
