# CCS 工程代码片段解读（复习用）

> 适用对象：福州大学《单片机原理与应用》课程期末复习
> 目标芯片：TI TMS320F2802x（F28027）
> 工程基座：TI f2802x Common Support Library（结构体指针 + 句柄式 API）
> 用途：考试会从这些工程中抽代码片段让你解释「这段代码是什么意思 / 起什么作用」

---

## 0. 阅读前必须先建立的"底层心智模型"

这套工程不是裸写寄存器（如 `GpioCtrlRegs.GPAMUX1 = ...`），而是用 TI 提供的 **句柄(Handle) + 结构体指针** 封装。理解任何片段前必须先懂这三件事：

### 0.1 句柄 = 指向寄存器结构体的指针
`F2802x_Component/F2802x_Device.c` 里把每个外设的基址强转成结构体指针：

```c
// chap4~chap11 各工程 F2802x_Component/F2802x_Device.c:23-38（以 chap10 为最全）
void System_authorize(void)
{
    myCpu  = CPU_init((void *)NULL, sizeof(CPU_Obj));
    myWDog = WDOG_init((void *)WDOG_BASE_ADDR, sizeof(WDOG_Obj));   // 0x7022
    myPll  = PLL_init((void *)PLL_BASE_ADDR, sizeof(PLL_Obj));
    myClk  = CLK_init((void *)CLK_BASE_ADDR, sizeof(CLK_Obj));       // 0x7010
    myGpio = GPIO_init((void *)GPIO_BASE_ADDR, sizeof(GPIO_Obj));   // 0x6F80
    myPie  = PIE_init((void *)PIE_BASE_ADDR, sizeof(PIE_Obj));      // 0x0CE0
    myTimer0 = TIMER_init((void *)TIMER0_BASE_ADDR, sizeof(TIMER_Obj)); // 0x0C00
    myPwm1 = PWM_init((void *)PWM_ePWM1_BASE_ADDR, sizeof(PWM_Obj));    // 0x6800
    myPwm2 = PWM_init((void *)PWM_ePWM2_BASE_ADDR, sizeof(PWM_Obj));
    myCap  = CAP_init((void *)CAPA_BASE_ADDR, sizeof(CAP_Obj));
    myAdc  = ADC_init((void *)ADC_BASE_ADDR, sizeof(ADC_Obj));      // 0x0B00
    mySci  = SCI_init((void *)SCIA_BASE_ADDR, sizeof(SCI_Obj));
}
```
**解读**：`myGpio`、`myPie`、`myTimer0`、`myPwm1`、`myAdc`、`mySci` 等都是全局指针变量（声明在 `F2802x_Device.h`，`TARGET_EXT` 控制 extern/定义），指向各外设寄存器块。所以 `myGpio->GPADIR`、`myPie->PIEACK`、`EPwm1Regs->TBPRD`（这里写作 `myPwm1->TBPRD`）本质就是访问物理寄存器。

### 0.2 EALLOW / EDIS 保护写
`F2802x_Component/include/cpu.h:72-84`：
```c
#define EALLOW asm(" EALLOW")
#define EDIS  asm(" EDIS")
#define ENABLE_PROTECTED_REGISTER_WRITE_MODE  asm(" EALLOW")
#define DISABLE_PROTECTED_REGISTER_WRITE_MODE asm(" EDIS")
```
所有受保护寄存器（GPIO 复用/方向、PIE 配置、时钟 PCLKCR、看门狗等）在底层 API 内部都被 `ENABLE_PROTECTED_REGISTER_WRITE_MODE ... DISABLE_PROTECTED_REGISTER_WRITE_MODE` 包裹。例如 `gpio.c:207` 的 `GPIO_setMode` 进函数就 EALLOW，出函数就 EDIS。

### 0.3 工程统一骨架（每个工程都一样）
`main.c` 永远是：
```
1. User_System_pinConfigure/functionConfigure/eventConfigure/initial   // 系统层
2. 各外设模块 _pinConfigure/_functionConfigure/_eventConfigure/_initial // 模块层
3. User_Pie_...              // 用到中断才有
4. User_Pie_start()          // 开总中断 EINT
5. for(;;){ ... }            // 主循环
```
每个模块又被拆成 **pinConfigure（引脚复用/方向）、functionConfigure（功能寄存器）、eventConfigure（中断/事件）、initial（变量初值）** 四段式。考试抽任何一段都要能说出它属于哪一层。

---

## 工程1：chap4_GPIO_1 —— GPIO 控制 LED 与按键（轮询+软件消抖）

### 工程概览
- **外设**：GPIO（无中断、无定时器）
- **功能**：4 个 LED（GPIO0~3）输出；1 个按键（GPIO12）输入；主循环轮询按键电平，按下点亮全部 LED，松开熄灭。
- **关键文件**：`main.c`、`User_Component/User_System/User_System.c`、`User_Component/LED_Gpio/LED_Gpio.c/.h`、`User_Component/KEY_Gpio/KEY_Gpio.c/.h`、`Application/app.c`、`F2802x_Component/F2802x_Device.c`、`F2802x_Component/source/gpio.c`

### 关键代码片段

#### 片段1：系统初始化四段式骨架（main.c:31-49）
```c
void main(void)
{
    //1. System runtime environment
    User_System_pinConfigure();
    User_System_functionConfigure();
    User_System_eventConfigure();
    User_System_initial();

    //2. Module
    //2.1 LED_Gpio
    LED_GPIO_pinConfigure();
    LED_GPIO_functionConfigure();
    LED_GPIO_eventConfigure();
    LED_GPIO_initial();

    Button_GPIO_pinConfigure();
    Button_GPIO_functionConfigure();
    Button_GPIO_eventConfigure();
    Button_GPIO_initial();
    ...
```
- **作用**：体现全工程的"系统层 + 模块层"两段、每段四步（pin/function/event/initial）的统一编写范式。
- **关键宏/函数**：`User_System_*` 配系统（时钟/PLL/PIE/WDG），`LED_GPIO_*`/`Button_GPIO_*` 配具体外设。
- **原理**：先配系统再配模块；先配引脚（复用、方向）再配功能寄存器，最后配事件（中断）。`eventConfigure/initial` 在本工程里是空函数，但骨架保留以便扩展。
- **改一下**：把 `User_System_functionConfigure()` 注释掉 → 没关看门狗、没配 PLL、没开 PIE，程序大概率跑飞或 LED 不亮。

#### 片段2：系统功能配置（User_System.c:57-96）
```c
void User_System_functionConfigure(void)
{
    // 0. system authorize
    System_authorize();
    // 1. disable watch DOG
    WDOG_disable(myWDog);
    // 2. disable interrupt
    CPU_disableGlobalInts(myCpu);
    // 3. Select the internal oscillator 1(10MHz) as the clock source
    CLK_setOscSrc(myClk, CLK_OscSrc_Internal);
    // 4. Setup the PLL for x12 /2 which will yield 60Mhz = 10Mhz * 12 / 2
    PLL_setup(myPll, PLL_Multiplier_12, PLL_DivideSelect_ClkIn_by_2);
    // 5. Module's Clock configure
    CLK_enableGpioInputClock(myClk); //Gpio Input clock
    // CLK_enableAdcClock / CpuTimerClock / PwmClock / SciaClock ...（本工程只开 GPIO）

    // 6. PIE configure
    PIE_disable(myPie);                       //禁止PIE
    PIE_disableAllInts(myPie);                //禁止PIE中断
    CPU_disableGlobalInts(myCpu);             //CPU全局中断禁止
    CPU_clearIntFlags(myCpu);                 //CPU中断标志位清零
    PIE_setDefaultIntVectorTable(myPie);      //中断入口地址设为默认值
    PIE_enable(myPie);                        //使能PIE
}
```
- **作用**：系统层的"标准开场白"——关 WDG、关中断、选时钟源、配 PLL 得 60MHz、开外设时钟、初始化 PIE。
- **关键寄存器/宏**：
  - `WDOG_disable` → 写 `WDCR` 的 `WDDIS` 位（`WDOG_WDCR_WDDIS_BITS = 1<<6`），需 EALLOW。
  - `CPU_disableGlobalInts` → `asm(" setc INTM")`（置位 INTM，关全局中断）。
  - `CLK_setOscSrc(.., CLK_OscSrc_Internal)` → 写 `CLKCTL.OSCCLKSRCSEL`，选内部振荡器 1（10MHz）。
  - `PLL_setup(.., PLL_Multiplier_12, PLL_DivideSelect_ClkIn_by_2)` → `SYSCLKOUT = 10MHz × 12 / 2 = 60MHz`，F2802x 最高 60MHz。
  - `CLK_enableGpioInputClock` → 置 `PCLKCR3.GPIOINENCLK`（bit13），给 GPIO 输入时钟，否则读引脚无效。
  - `PIE_disable`/`PIE_enable` → 写 `PIECTRL.ENPIE`（bit0）；`PIE_setDefaultIntVectorTable` 把 256 个向量填成 illegalIsr，防止未注册中断乱跳。
- **原理**：配 PIE 前必须先 `PIE_disable` + `CPU_disableGlobalInts`，避免配置过程中被未初始化向量中断；配完再 `PIE_enable`。本工程不用中断，但 PIE 仍按模板初始化，主循环前不开总中断（没有 `User_Pie_start`）。
- **改一下**：把 `WDOG_disable` 删掉 → 看门狗约 13ms 后复位芯片，LED 会反复闪烁复位；把 `PLL_setup` 的 `PLL_Multiplier_12` 改成 `_6` → SYSCLKOUT 变 30MHz，所有 `Delay()` 时间翻倍。

#### 片段3：LED 引脚配置（LED_Gpio.c:59-82）
```c
void LED_GPIO_pinConfigure(void)
{
    // 1. set mode
    GPIO_setMode(LED_Gpio_obj, LED1, GPIO_0_Mode_GeneralPurpose);
    GPIO_setMode(LED_Gpio_obj, LED2, GPIO_1_Mode_GeneralPurpose);
    GPIO_setMode(LED_Gpio_obj, LED3, GPIO_2_Mode_GeneralPurpose);
    GPIO_setMode(LED_Gpio_obj, LED4, GPIO_3_Mode_GeneralPurpose);
    // 2. set pullup
    GPIO_setPullUp(LED_Gpio_obj, LED1, GPIO_PullUp_Disable);
    ...
    // 3. set direction
    GPIO_setDirection(LED_Gpio_obj, LED1, GPIO_Direction_Output);
    ...
}
```
- **作用**：把 GPIO0~3 配成普通 GPIO、不上拉、输出方向，驱动 4 个 LED。
- **关键宏**：`LED_Gpio_obj = myGpio`、`LED1 = GPIO_Number_0`…（见 `LED_Gpio.h:53-57`）；`GPIO_0_Mode_GeneralPurpose = 0`（复用值 0 = 普通 IO），`GPIO_0_Mode_EPWM1A = 1`（复用值 1 = PWM）。
- **背后原理**（看 `gpio.c:207-256` 的 `GPIO_setMode`）：
  ```c
  uint16_t lShift = gpioNumber << 1;            // 每个引脚占 2 bit
  uint32_t clearBits = (uint32_t)GPIO_GPMUX_CONFIG_BITS << lShift; // 0x3 << lShift
  gpio->GPAMUX1 &= (~clearBits);                // 先清 2 bit
  gpio->GPAMUX1 |= setBits;                     // 再写 mode
  ```
  即 GPIO0~15 复用位在 `GPAMUX1`，每个引脚 2 bit，`00`=GPIO、`01`=外设1、`10`=外设2、`11`=外设3。`GPIO_setDirection` 操作 `GPADIR`，1=输出。
- **改一下**：把 `GPIO_0_Mode_GeneralPurpose` 改成 `GPIO_0_Mode_EPWM1A` → 引脚变 PWM 输出，写 GPADAT 不再控制 LED。

#### 片段4：按键引脚配置 + 输入滤波（KEY_Gpio.c:47-57）
```c
void Button_GPIO_pinConfigure(void)
{
    Button_Num = Button1;
    GPIO_setMode(Button_Gpio_obj, Button1, GPIO_12_Mode_GeneralPurpose);
    GPIO_setPullUp(Button_Gpio_obj, Button1, GPIO_PullUp_Disable);
    GPIO_setDirection(Button_Gpio_obj, Button1, GPIO_Direction_Input);

    GPIO_setQualification(Button_Gpio_obj, Button1, GPIO_Qual_Sample_6);
    GPIO_setQualificationPeriod(Button_Gpio_obj, Button1, 16U);
}
```
- **作用**：GPIO12 配成普通输入、不上拉，并启用"6 次采样"输入限定（消抖）。
- **关键宏**：`Button1 = GPIO_Number_12`；`GPIO_Qual_Sample_6`（`GPAQSEL1` 里每 pin 2 bit，`10`=6 采样窗口）；`GPIO_setQualificationPeriod(.., 16U)` 写 `GPACTRL` 里的 qualifier 周期。
- **原理**：按键机械抖动会在几 us 内产生多个边沿，硬件采样限定要求连续 6 个采样窗内电平一致才更新锁存值，等效硬件低通滤波。采样窗周期 = `qualification_period × SYSCLKOUT`，这里 16 个 SYSCLK 周期为一个采样窗。
- **改一下**：改成 `GPIO_Qual_ASync`（异步）→ 没有硬件滤波，按键值会抖动。

#### 片段5：按键读取 + 软件消抖状态机（KEY_Gpio.c:77-109）
```c
uint16_t Button_GPIO_readRaw(void)
{
    return GPIO_getData(Button_Gpio_obj, Button1);
}

uint16_t Button_GPIO_isPressed(void)
{
    static uint16_t stableLevel = Button_Active_Low;
    static uint16_t debounceCount = 0U;
    uint16_t rawLevel;

    rawLevel = Button_GPIO_readRaw();

    if(rawLevel != stableLevel)
    {
        if(debounceCount < 20U)  debounceCount++;
        if(debounceCount >= 20U) { stableLevel = rawLevel; debounceCount = 0U; }
    }
    else
    {
        debounceCount = 0U;
    }

    return (stableLevel == Button_Active_Low) ? 1U : 0U;
}
```
- **作用**：读 GPIO12 原始电平，再用"连续 20 次读到同一新电平才确认"的软件消抖，返回 1=按下（低电平有效）、0=松开。
- **关键 API**：`GPIO_getData`（`gpio.c:33-50`）：
  ```c
  if(gpioNumber < GPIO_Number_32)
      return ((gpio->GPADAT >> gpioNumber) & 0x0001);
  ```
  即读 `GPADAT` 对应 bit。`Button_Active_Low = 0`。
- **原理**：`static` 变量在函数调用间保留状态，构成"电平变化—计数—确认"三态消抖机。即使硬件限定关掉，软件仍能去抖。
- **改一下**：把 `20U` 改成 `1` → 几乎无消抖，按键一抖就翻转判定。

#### 片段6：LED 的 on/off/toggle 内联 API（LED_Gpio.h:103-121）
```c
TARGET_EXT void inline LED_on(GPIO_Number_e led)
{
    GPIO_setLow(LED_Gpio_obj, led);
}
TARGET_EXT void inline LED_off(GPIO_Number_e led)
{
    GPIO_setHigh(LED_Gpio_obj, led);
}
TARGET_EXT void inline LED_toggle(GPIO_Number_e led)
{
    GPIO_toggle(LED_Gpio_obj, led);
}
```
- **作用**：把"LED 低电平点亮"的硬件细节封装成 `LED_on/off/toggle`。
- **背后原理**（`gpio.c:162-204`）：`GPIO_setLow` 写 `GPACLEAR = 1<<n`（写 1 的位被清零，写 0 无影响）；`GPIO_setHigh` 写 `GPASET = 1<<n`（写 1 的位被置 1）；`GPIO_toggle` 写 `GPATOGGLE = 1<<n`（写 1 的位翻转）。这是"set/clear/toggle 置位寄存器"标准用法，比 `GPADAT = GPADAT | (1<<n)` 原子且不影响其它位。
- **改一下**：把 `LED_on` 里换成 `GPIO_setHigh` → 逻辑反相，"按下点亮"变成"按下熄灭"。

#### 片段7：主循环按键控制 LED（main.c:62-86）
```c
p=(uint16_t *) 0x8000;

for( ; ; )
{
    if(Button_GPIO_isPressed())
    {
        LED_on(LED1); LED_on(LED2); LED_on(LED3); LED_on(LED4);
    }
    else
    {
        LED_off(LED1); LED_off(LED2); LED_off(LED3); LED_off(LED4);
    }
}
```
- **作用**：死循环轮询按键，按下点亮 4 个 LED、松开熄灭。`p=(uint16_t*)0x8000` 是把指针指向 RAM 区 0x8000（实验板往往在此映射数码管/扩展 IO），本工程未使用，仅作演示。
- **原理**：纯轮询无中断，CPU 100% 占用；按键响应延迟≈一次消抖确认时间。

#### 片段8：LED 花样数组显示（app.c:109-124）
```c
void insql(unsigned int p) {
    int sta[4],i;
    for (i = 0;i < 4;++i) {
        sta[i] = (p & (1 << i));
    }
    for (i = 0;i < 4;++i) {
        if (sta[i]) LED_on(LED[i]);
        else LED_off(LED[i]);
    }
    Delay(500000L);
}

void LED_Control4(void) {
    insql(1);insql(3);insql(7);insql(15);
}
```
- **作用**：把一个 4 bit 整数按位拆开点亮 LED，依次传入 1(0001)、3(0011)、7(0111)、15(1111)，实现"逐个点亮"渐显效果。
- **关键点**：`p & (1<<i)` 位测试；`LED[i]` 数组里存的是 `GPIO_Number_0~3`（`LED_Gpio.c:48-51` 初始化），用数组下标代替宏名。
- **改一下**：`insql(1);insql(3);insql(7);insql(15);` 改成 `insql(15);insql(7);insql(3);insql(1);` → 反向渐灭。

### 该工程常考点小结
- GPIO 三件套：`setMode`（复用 GPAMUX）、`setPullUp`（GPAPUD）、`setDirection`（GPADIR）的位操作原理
- 输入限定 `GPIO_setQualification` + `setQualificationPeriod` 的硬件消抖
- `GPASET/GPACLEAR/GPATOGGLE` 置位/清零/翻转寄存器的"写 1 有效"用法
- 系统初始化六步：授权→关 WDG→关中断→选时钟源→配 PLL→开外设时钟+PIE 模板
- 软件消抖状态机（static 计数）

---

## 工程2：chap5_GPIO_1 —— GPIO 按键长短按/双击检测（轮询扫描）

### 工程概览
- **外设**：GPIO（LED 输出 GPIO0~3，按键输入 GPIO12），仍无中断
- **功能**：通过轮询计数实现 **短按 / 长按 / 双击** 三种按键事件，分别控制不同 LED 翻转；同时演示数组式 LED 花样显示。
- **关键文件**：`User_Component/KEY/KEY.c/.h`、`User_Component/LED_GPIO/LED_Gpio.c`、`Application/app.c`、`User_Component/Button_GPIO/Button_GPIO.c`
- **与 chap4 区别**：消抖改成纯软件计数（不用 `GPIO_Qual_Sample_6`，用 `GPIO_Qual_ASync`），并加入了 XINT2 的外部中断模板（`KEY_eventConfigure`，但主循环用轮询版本 `KEY_Control_LED`，中断版本被注释）。

### 关键代码片段

#### 片段1：按键引脚配置——异步输入（KEY.c:51-70）
```c
void KEY_pinConfigure(void)
{
    GPIO_setMode(KEY_obj, KEY1, GPIO_12_Mode_GeneralPurpose);
    GPIO_setPullUp(KEY_obj, KEY1, GPIO_PullUp_Disable);
    GPIO_setDirection(KEY_obj, KEY1, GPIO_Direction_Input);
    GPIO_setQualification(KEY_obj, KEY1, GPIO_Qual_ASync);
}
```
- **作用**：GPIO12 普通输入、不上拉、**异步采样**（不做硬件滤波）。
- **关键宏**：`GPIO_Qual_ASync`（`GPAQSEL` 里 `11`=异步，采样窗被旁路，引脚电平直通）。
- **对比 chap4**：chap4 用 `GPIO_Qual_Sample_6` 硬件滤波，chap5 改异步、靠后面 `KEY_STATUS` 软件计数去抖。
- **改一下**：异步下若没有软件消抖，按键值会抖。

#### 片段2：按键事件配置——外部中断模板（KEY.c:85-95）
```c
void KEY_eventConfigure(void)
{
    PIE_setExtIntPolarity(myPie, CPU_ExtIntNumber_2, PIE_ExtIntPolarity_FallingEdge);
    GPIO_setExtInt(KEY_obj, KEY1, CPU_ExtIntNumber_2);
}
```
- **作用**：把 GPIO12 映射到外部中断 2（XINT2），并设为下降沿触发。
- **关键函数**：
  - `PIE_setExtIntPolarity(.., CPU_ExtIntNumber_2, PIE_ExtIntPolarity_FallingEdge)` → 写 `XINTnCR[1]` 的 POLARITY 位（`0`=下降沿、`1`=上升沿、`3`=双边沿，见 `pie.h:389-394`）。
  - `GPIO_setExtInt(.., KEY1, CPU_ExtIntNumber_2)` → 写 `GPIOXINTnSEL[1] = GPIO_Number_12`，告诉芯片"XINT2 的源是 GPIO12"（`gpio.c:146-159`）。
- **注意**：这里只配了源和极性，并没有 `PIE_enableExtInt` 也没有注册 ISR，所以中断实际未启用；主循环走 `KEY_Control_LED()` 轮询版本。
- **改一下**：把 `FallingEdge` 改成 `RisingEdge` → 按键松开瞬间才触发中断。

#### 片段3：短按/长按/双击识别（KEY.c:99-135）
```c
void KEY_STATUS(void)
{
    if(KEYPRESSED == GetKeyStatus(KEY1))
        key_counter1++;

    if((KEYPRESSED != GetKeyStatus(KEY1)) )
    {
        if( key_counter1 >= 600 && key_counter1 <= 8000)      //短按识别
            key_status = KEY_PRESS_SHORT;
        if( key_counter1 >= 10000)                            //长按识别
            key_status = KEY_PRESS_LONG;
        key_counter1=0;
    }

    if((KEYPRESSED == GetKeyStatus(KEY1)) &&( 0 == Flag) )
        Flag = 1;
    if((KEYPRESSED != GetKeyStatus(KEY1)) &&( 1 == Flag) )
    {
        Flag = 0;
        KeyCount ++;
        if(KeyCount>=2) { KeyCount = 0; key_status = KEY_PRESS_TWICE; }
    }
}
```
- **作用**：在主循环里高频调用，按住期间累加 `key_counter1`，松开后根据累计值判定短按（600~8000 次）或长按（≥10000 次）；同时用 `Flag` 边沿检测 + `KeyCount` 累计松开次数，2 次松开即判双击。
- **关键宏/变量**：`KEYPRESSED = 1`（`KEY.h:134`，按键按下时 `GPIO_getData` 返回 1 ——注意本工程按键高电平有效，与 chap4 低电平有效相反，取决于硬件电路）；`key_status` 取值 `KEY_PRESS_SHORT/LONG/TWICE/NO`（0/1/2/3）。
- **API**：`GetKeyStatus`（`KEY.h:111-115`）内联 `return GPIO_getData(KEY_obj, key);`，即读 `GPADAT` bit12。
- **原理**：轮询周期近似恒定，"按下持续时间"正比于 `key_counter1`，所以阈值代表时间。`Flag` 是"按下→松开"的一次边沿锁存，构成单击计数。
- **改一下**：把 `10000` 改小 → 更易被判成长按。

#### 片段4：按键控制 LED 翻转（app.c:59-76）
```c
void KEY_Control_LED_1(void)
{
    if(key_status == KEY_PRESS_SHORT)   LED_toggle(LED1);
    if(key_status == KEY_PRESS_LONG)    LED_toggle(LED2);
    if(key_status == KEY_PRESS_TWICE)   LED_toggle(LED3);
    key_status = KEY_PRESS_NO;          //防止重复触发
}
```
- **作用**：消费 `key_status`，不同事件翻转不同 LED，最后清事件标志避免重复响应。
- **原理**：`key_status` 是 KEY 模块与 app 模块间的"事件邮箱"，生产者 `KEY_STATUS` 写、消费者 `KEY_Control_LED_1` 读后清。

#### 片段5：数组式 LED 花样显示（LED_Gpio.c:114-130）
```c
void LED_Display1(void)
{
    int c;int d;
    int a[4][5]={{0,0,1,0,1},{0,1,0,1,0},{0,1,0,1,0},{0,0,1,0,1}};
    for(c=0;c<=4;c++)
        for(d=0;d<=3;d++)
            if(a[d][c]==1) LED_on(LED[d]);
            else           LED_off(LED[d]);
        Delay(50000L);
}
```
- **作用**：用一个 4×5 的 0/1 表描述"4 个 LED 在 5 个时刻的开关状态"，按列扫描实现花样。`a[d][c]` 中 d=LED 编号、c=时间帧。
- **关键点**：把"显示图案"数据化，便于改表换花样；`LED[d]` 数组下标访问引脚。

### 该工程常考点小结
- XINT 外部中断的"源选择 `GPIO_setExtInt` + 极性 `PIE_setExtIntPolarity`"两步配置
- 长短按/双击的纯软件计数识别思路（计数阈值≈时间）
- 异步输入 `GPIO_Qual_ASync` 与硬件滤波的区别
- 事件邮箱 `key_status` 的生产—消费解耦

---

## 工程3：chap6_PIE_1 —— PIE 三级中断 + CPU Timer0 中断

### 工程概览
- **外设**：GPIO + PIE + CPU Timer0
- **功能**：用 CPU Timer0 产生 1 秒周期中断，中断里翻转 LED2；演示 PIE 三级中断配置完整流程（注册向量→模块使能→PIE 使能→CPU 使能→开总中断）。
- **关键文件**：`User_Component/User_Pie/User_Pie.c/.h`、`User_Component/User_Timer/User_Timer.c`、`Application/isr.c`、`User_Component/key_GPIO/key_Gpio.c`、`F2802x_Component/F2802x_Device.c`（这里新增 `myTimer0`）

### 关键代码片段

#### 片段1：注册 PIE 中断向量（User_Pie.c:58-77）
```c
void User_Pie_functionConfigure(void)
{
    //1.1 TIMER0
    PIE_registerPieIntHandler(myPie, PIE_GroupNumber_1, PIE_SubGroupNumber_7,
                              (intVec_t) &myTimer_CpuTimer0_isr);
    //1.2 XINT1（被注释）
    //1.3 EPWM1INT（被注释）
    //1.4 ADCINT（被注释）
}
```
- **作用**：把 Timer0 中断服务函数 `myTimer_CpuTimer0_isr` 的地址写进 PIE 向量表 Group1 SubGroup7（即 PIE 向量表里的 `TINT0` 槽位）。
- **关键类型**：`intVec_t = typedef interrupt void (*intVec_t)(void)`（`pie.h:384`），即"interrupt void(void)"的函数指针；ISR 必须用 `interrupt` 关键字修饰才能匹配此类型。
- **PIE 向量表布局**（`pie.h:578-582`）：`XINT1、XINT2、ADCINT9、TINT0、WAKEINT` 依次在 Group1 的子组 4/5/6/7/8 槽；所以 Timer0 = Group1 SubGroup7。
- **原理**：F2802x 把 96 个外设中断排成 12 组×8 子组，PIE 向量表是一张函数指针表，CPU 响应 INT1~INT12 时会自动跳到 `PIE_VECT[group][subgroup]` 指向的函数。
- **改一下**：把 `&myTimer_CpuTimer0_isr` 写成不带 `&` 也能（函数名即地址），但漏写 `(intVec_t)` 强转在某些编译选项下会警告。

#### 片段2：三级中断使能（User_Pie.c:83-117）
```c
void User_Pie_eventConfigure(void)
{
    //1. module IE   ——第一级：外设自身中断使能
    TIMER_enableInt(myTimer0);   //已移至 User_Timer.c 的 eventConfigure（本工程在此重复使能）

    //2. PIE PIEIERx.y ——第二级：PIE 组内使能
    PIE_enableInt(myPie, PIE_GroupNumber_1, PIE_InterruptSource_TIMER_0);

    //3. CPU IERx      ——第三级：CPU 中断使能
    CPU_enableInt(myCpu, CPU_IntNumber_1);
}
```
- **作用**：完成"外设级 → PIE 级 → CPU 级"三级中断使能，这是 C2000 PIE 必背套路。
- **关键宏/位**：
  - `TIMER_enableInt` → 置 `TCR.TIE`（`TIMER_TCR_TIE_BITS = 1<<14`），允许定时器溢出时发中断。
  - `PIE_enableInt(.., Group1, PIE_InterruptSource_TIMER_0)` → `PIE_InterruptSource_TIMER_0 = 1<<6`（`pie.h:480`），写 `PIEIER1 |= 0x40`，使能 Group1 的第 7 个子组（Timer0）。
  - `CPU_enableInt(.., CPU_IntNumber_1)` → `CPU_IntNumber_1 = 1<<0`，写 `IER |= 0x0001`，使能 CPU INT1（Timer0 挂在 INT1）。
- **原理**：PIE 把 12 组外设中断复用到 CPU 的 INT1~INT12；Timer0 在 Group1，Group1 又对应 CPU INT1，所以三级都要使能且组号要对应一致。
- **改一下**：漏掉 `CPU_enableInt` → 即使 PIE 使能了 CPU 也不响应，中断永远不进。

#### 片段3：开总中断（User_Pie.h:90-94）
```c
TARGET_EXT void inline User_Pie_start(void)
{
    CPU_enableGlobalInts(myCpu);
}
```
- **背后**：`CPU_enableGlobalInts` → `asm(" clrc INTM")`（清 INTM，开全局中断）。对应 `EINT` 宏。
- **要点**：三级使能只是"允许"，真正响应还需要清 INTM。所以 `main.c` 里顺序是：配 PIE → `User_Pie_start()` → 主循环。

#### 片段4：Timer0 配置（User_Timer.c:25-31）
```c
void USER_TIMER_initial(void ) {
    TIMER_stop(myTimer0);                          // 停止原来的
    TIMER_setDecimationFactor(myTimer0, 0);        // 分频系数
    TIMER_setPeriod(myTimer0, 60000000L - 1);      // 周期值设置
    TIMER_reload(myTimer0);                        // 重装载
    TIMER_start(myTimer0);                         // 启动计时器
}
```
- **作用**：CPU Timer0 周期 = 60000000-1，分频 0（即 +1 = 1），时钟 = SYSCLKOUT = 60MHz，所以中断周期 = 60000000 / 60MHz = 1 秒。
- **关键寄存器**：`TIMER_Obj` 含 `TIM、PRD、TCR、TPR`（`timer.h:104-110`）。`setPeriod` 写 `PRD`；`setDecimationFactor` 写 `TPR` 的低 8 位（预分频）；`reload` 置 `TCR.TRB`（把 PRD 装入 TIM）；`start` 清 `TCR.TSS`；`stop` 置 `TCR.TSS`。
- **原理**：32 位 CPU Timer 减计数，TIM 从 PRD 减到 0 时置 `TCR.TIF`，若 `TCR.TIE=1` 则发中断，再自动重装 PRD。**减法计数器，所以周期值要写 N-1**（0 也要算一拍）。
- **改一下**：把 `60000000L-1` 改成 `30000000L-1` → 中断周期变 0.5 秒；漏写 `-1` → 周期多 1 个时钟周期（60MHz 下误差 16.7ns，可忽略但概念错）。

#### 片段5：Timer0 中断服务函数（isr.c:36-39）
```c
interrupt void myTimer_CpuTimer0_isr(void) {
    LED_toggle(LED2);
    PIE_clearInt(myPie, PIE_GroupNumber_1);
}
```
- **作用**：每秒进一次中断，翻转 LED2，然后向 PIEACK 写 Group1 对应位以清"组响应"锁存，否则 Group1 后续中断再也不会进。
- **关键点**：
  - `interrupt` 关键字：编译器会自动保存上下文并在返回时用 `IRET`。
  - `PIE_clearInt(.., PIE_GroupNumber_1)` → `pie->PIEACK = 1<<0`（`pie.h:703-712`），写 PIEACK 的 Group1 位（写 1 清零），表示"CPU 已处理完 Group1，可以再发"。
  - **CPU Timer0 的 TIF 不用手动清**：因为 PIE 响应时硬件会自动清 TIF。但 PWM/ADC 等外设的 flag 要在 ISR 内手动清（见后续工程）。
- **改一下**：漏写 `PIE_clearInt` → 只能进一次中断，之后 LED2 永远不再翻转。

#### 片段6：按键 XINT1 中断配置（key_Gpio.c:85-92）
```c
void KEY_GPIO_eventConfigure(void)
{
    PIE_setExtIntPolarity(myPie, CPU_ExtIntNumber_1, PIE_ExtIntPolarity_RisingEdge);
    GPIO_setExtInt(KEY_Gpio_obj, KEY1, CPU_ExtIntNumber_1);
}
```
- **作用**：把 GPIO12 映射到 XINT1、上升沿触发（与 chap5 用 XINT2 下降沿对照）。
- **注意**：这里同样只配了源和极性，模块使能 `PIE_enableExtInt(myPie, CPU_ExtIntNumber_1)` 在 `User_Pie.c` 里被注释，所以 XINT1 实际未启用，本工程只演示 Timer0 中断。

### 该工程常考点小结
- **PIE 三级中断配置完整五步**：①`PIE_registerPieIntHandler` 注册向量 ②外设 `TIMER_enableInt`/`PWM_enableInt` ③`PIE_enableInt` ④`CPU_enableInt` ⑤`User_Pie_start`(开 INTM)
- ISR 必须用 `interrupt` 关键字
- ISR 末尾必须 `PIE_clearInt(myPie, PIE_GroupNumber_x)` 清 PIEACK 组锁存
- CPU Timer 周期 = `(PRD) × (TPR+1) / SYSCLKOUT`，减计数所以要写 N-1
- PIE 向量表 Group1 SubGroup7 = TINT0 的位置对应关系

---

## 工程4：chap7_TIMER_1 —— CPU Timer0 定时 + LED 跑马灯

### 工程概览
- **外设**：GPIO + CPU Timer0 + PIE + XINT1（按键）
- **功能**：Timer0 1 秒中断驱动 4 个 LED 跑马灯（每次中断切到下一个 LED）；XINT1 按键中断翻转 LED1。
- **关键文件**：`User_Component/myTimer/myTimer.c`、`User_Component/User_Pie/User_Pie.c`、`Application/isr.c/.h`、`User_Component/User_System/User_System.c`（这里首次打开 `CLK_enableCpuTimerClock`）

### 关键代码片段

#### 片段1：使能 CPU Timer0 时钟（User_System.c:72）
```c
CLK_enableCpuTimerClock(myClk, CLK_CpuTimerNumber_0);//Cpu Timer0 clock
```
- **作用**：置 `PCLKCR3.CPUTIMER0ENCLK`（`CLK_PCLKCR3_CPUTIMER0ENCLK_BITS = 1<<8`），给 CPU Timer0 提供时钟。
- **关键点**：F2802x 所有外设默认时钟是关的，必须按用到的外设逐个开。chap4 只开 GPIO，chap6 也漏开了（注释状态，所以 chap6 实际可能跑不动——这是模板的一个坑，chap7 修正了）。
- **对照**：`CLK_CpuTimerNumber_0 = 1<<8`、`_1 = 1<<9`、`_2 = 1<<10`（`clk.h:256-260`），分别对应 Timer0/1/2。
- **改一下**：注释掉这行 → Timer0 寄存器读写无效，中断不发生。

#### 片段2：Timer0 周期配置（myTimer.c:57-74）
```c
void myTimer_functionConfigure(void)
{
    TIMER_stop(myTimer0);
    TIMER_setDecimationFactor(myTimer0, 0);   //0+1，定时器分频
    TIMER_setPeriod(myTimer0, 60000000L-1);    //60000000*1/60000000=1s
    TIMER_reload(myTimer0);
    TIMER_start(myTimer0);
}
```
- **作用**：与 chap6 `USER_TIMER_initial` 完全等价，注释更清楚说明"分频 0+1=1，周期 60000000，1/60MHz=1s"。
- **关键点**：`setDecimationFactor(0)` 即 TPR=0，预分频 = 0+1 = 1；周期 = `(60000000-1+1) × 1 / 60MHz = 1s`。
- **改一下**：把 `setDecimationFactor` 改成 `9` → 预分频 10，周期变 10 秒。

#### 片段3：Timer0 中断跑马灯（isr.c:57-69）
```c
interrupt void myTimer_CpuTimer0_isr(void)
{
    LED_off(LED[LED_Count]);
    LED_Count++;
    if(LED_Count >=4)  LED_Count=0;
    LED_on(LED[LED_Count]);

    PIE_clearInt(myPie, PIE_GroupNumber_1);
}
```
- **作用**：每秒关掉当前 LED，索引 +1 循环（0→1→2→3→0），点亮下一个，实现跑马灯。
- **关键变量**：`LED_Count`（在 `isr.h` 或 LED 模块定义的全局循环索引），`LED[]` 数组存 `GPIO_Number_0~3`。
- **原理**：中断里只做"状态机推进 + 引脚操作 + 清 PIEACK"，无阻塞，主循环空跑即可。
- **改一下**：把 `LED_off(LED[LED_Count])` 移到 `LED_on` 之后 → 短暂同时点亮两个 LED，视觉上变成"流水+拖影"。

#### 片段4：XINT1 按键中断 ISR（isr.c:46-52）
```c
interrupt void KEY_XINT1_isr(void)
{
    LED_toggle(LED1);
    PIE_clearInt(myPie, PIE_GroupNumber_1);
}
```
- **作用**：XINT1 下降/上升沿（按 `key_Gpio.c` 配置为上升沿）触发，翻转 LED1。
- **关键点**：XINT1 也在 PIE Group1，所以清 `PIE_GroupNumber_1`。XINT 的 IFR 由 PIE 自动清，无需手写。
- **注意**：本工程 `User_Pie.c` 里 XINT1 注册和使能被注释，实际只有 Timer0 中断在跑；这里 ISR 存在是为教学完整性。

### 该工程常考点小结
- 外设时钟使能 `CLK_enableCpuTimerClock` 的位（PCLKCR3 bit8/9/10）
- Timer0 周期公式 `T = (PRD) × (TPR+1) / SYSCLKOUT`，减计数写 N-1
- 跑马灯"先灭当前→索引循环→点亮下一个"的中断状态机写法
- XINT1 与 TINT0 同属 PIE Group1，所以 ISR 末尾都清 `PIE_GroupNumber_1`

---

## 工程5：chap9_PWM_1 —— ePWM 呼吸灯 + eCAP 捕获

### 工程概览
- **外设**：GPIO（EPWM 复用）+ ePWM1/ePWM2 + eCAP1 + CPU Timer0 + PIE
- **功能**：用 ePWM1A/1B/2A/2B 输出 4 路 PWM 驱动 4 个 LED；EPWM1 周期中断里用三角波算法改变 4 路 CMPA/CMPB 实现 4 个 LED 不同速度的呼吸灯；eCAP1 捕获 GPIO5 外部信号周期；Timer0 中断里递增 CMPA 演示手动改占空比。
- **关键文件**：`User_Component/LED_Pwm/LED_Pwm.c/.h`、`User_Component/myCap/myCap.c`、`User_Component/myTimer/myTimer.c`、`User_Component/User_Pie/User_Pie.c`、`Application/isr.c`、`User_Component/User_System/User_System.c`

### 关键代码片段

#### 片段1：ePWM 引脚复用配置（LED_Pwm.c:53-74）
```c
void LED_PWM_pinConfigure(void)
{
    GPIO_setMode(myGpio, GPIO_Number_0, GPIO_0_Mode_EPWM1A);
    GPIO_setMode(myGpio, GPIO_Number_1, GPIO_1_Mode_EPWM1B);
    GPIO_setMode(myGpio, GPIO_Number_2, GPIO_2_Mode_EPWM2A);
    GPIO_setMode(myGpio, GPIO_Number_3, GPIO_3_Mode_EPWM2B);
    GPIO_setPullUp(myGpio, GPIO_Number_0, GPIO_PullUp_Disable);
    ... // 4 路都 Disable
    GPIO_setDirection(myGpio, GPIO_Number_0, GPIO_Direction_Output);
    ... // 4 路都 Output
}
```
- **作用**：把 GPIO0~3 复用成 EPWM1A/1B/2A/2B 功能引脚（复用值 = 1，对照 `gpio.h:63-78` 的枚举）。
- **关键宏**：`GPIO_0_Mode_EPWM1A = 1`、`GPIO_1_Mode_EPWM1B = 1`…（在 `GPAMUX1` 里对应位的 2 bit 写 `01`）。
- **要点**：即使引脚交给外设，方向仍要设成 Output（外设输出需 GPIO 输出方向配合），不上拉避免干扰波形。

#### 片段2：ePWM 时基与计数器配置（LED_Pwm.c:80-104）
```c
void LED_PWM_functionConfigure(void)
{
    //1. TBCLKSYNC = 0    ——先关时基同步，配置期间冻结所有 ePWM 计数
    CLK_disableTbClockSync(myClk);
    //2. TBCLK            ——分频
    PWM_setHighSpeedClkDiv(myPwm1, PWM_HspClkDiv_by_1);
    PWM_setClkDiv(myPwm1, PWM_ClkDiv_by_1);
    PWM_setHighSpeedClkDiv(myPwm2, PWM_HspClkDiv_by_1);
    PWM_setClkDiv(myPwm2, PWM_ClkDiv_by_1);
    //3. TBCTR 计数模式
    PWM_setCounterMode(myPwm1, PWM_CounterMode_Up);
    PWM_setCounterMode(myPwm2, PWM_CounterMode_Up);
    //4. TBPRD 周期
    PWM_setPeriod(myPwm1, 60000);
    PWM_setPeriod(myPwm2, 60000);
    PWM_setPeriodLoad(myPwm1, PWM_PeriodLoad_Shadow);
    PWM_setPeriodLoad(myPwm2, PWM_PeriodLoad_Shadow);
    ...
    //8. TBCLKSYNC = 1    ——配完再开同步
    CLK_enableTbClockSync(myClk);
}
```
- **作用**：两路 ePWM 都配成"TBCLK=SYSCLKOUT、向上计数、周期 60000、影子装载"。
- **关键寄存器/宏**（`pwm.h` 中 `PWM_Obj`，`pwm.h:828-884`）：
  - `TBCTL`（Time-Base Control）：含 `HSPCLKDIV`(bit7-9)、`CLKDIV`(bit10-12)、`CNTMODE`(bit0-1)。
  - `PWM_HspClkDiv_by_1 = 0<<7`、`PWM_ClkDiv_by_1 = 0<<10` → TBCLK = SYSCLKOUT/1 = 60MHz。
  - `PWM_CounterMode_Up = 0<<0` → 单增计数（0→TBPRD→0 重新）。
  - `TBPRD = 60000` → PWM 周期 = 60000/60MHz = 1ms（1kHz）。
  - `PWM_PeriodLoad_Shadow = 0<<3` → TBPRD 写入影子寄存器，在 TBCTR=0 或周期事件时再装载到活动寄存器，避免运行中改周期产生毛刺。
- **核心套路**：**配置 ePWM 必须先 `CLK_disableTbClockSync` 冻结，配完再 `CLK_enableTbClockSync` 同步启动**，否则多路 ePWM 起始相位不一致。`TBCLKSYNC` 在 `PCLKCR0` bit2（`CLK_PCLKCR0_TBCLKSYNC_BITS`）。
- **改一下**：把 `PWM_CounterMode_Up` 改成 `PWM_CounterMode_UpDown` → 变三角波计数，频率减半；把 `60000` 改成 `6000` → 频率 10kHz。

#### 片段3：CMPA 影子装载 + 动作限定 AQ（LED_Pwm.c:110-141）
```c
    //5. CMPA 影子模式
    PWM_setShadowMode_CmpA(myPwm1, PWM_ShadowMode_Shadow);
    PWM_setShadowMode_CmpA(myPwm2, PWM_ShadowMode_Shadow);
    PWM_setLoadMode_CmpA(myPwm1, PWM_LoadMode_Zero);   // TBCTR=0 时装载
    PWM_setLoadMode_CmpA(myPwm2, PWM_LoadMode_Zero);
    PWM_setShadowMode_CmpB(myPwm1, PWM_ShadowMode_Shadow);
    ...
    //7. action qualifier ——动作限定器
    // EPWM1A: CMPA时Set, Period时Clear
    PWM_setActionQual_CntUp_CmpA_PwmA(myPwm1, PWM_ActionQual_Set);
    PWM_setActionQual_Period_PwmA(myPwm1, PWM_ActionQual_Clear);
    // EPWM1B: CMPB时Set, Period时Clear
    PWM_setActionQual_CntUp_CmpB_PwmB(myPwm1, PWM_ActionQual_Set);
    PWM_setActionQual_Period_PwmB(myPwm1, PWM_ActionQual_Clear);
    // EPWM2A / EPWM2B 同理
```
- **作用**：定义"计数值 = CMPA 时把 EPWMxA 置 1，计数值 = 周期 TBPRD 时把 EPWMxA 清 0"，形成占空比可调的 PWM。
- **关键寄存器**：`CMPCTL`（影子控制，`PWM_ShadowMode_Shadow=0` 影子模式、`PWM_LoadMode_Zero=0` 在零点装载）、`CMPA/CMPB`（比较值）、`AQCTLA/AQCTLB`（动作限定，`pwm.h:841-842`）。
- **占空比计算（向上计数模式）**：`占空比 = CMPA / TBPRD`（高电平时间 = TBPRD - CMPA，因为 CMPA 时置 1、周期时清 0）。CMPA=0 → 几乎全高；CMPA=TBPRD → 几乎全低。
- **关键枚举**（`pwm.h:425-428`）：`PWM_ActionQual_Clear`、`PWM_ActionQual_Set`、还有 `Toggle/High/Low/DoNothing`。AQ 每个事件（CAU/CAD/CBU/CBD/PRD/ZRO）占 2 bit。
- **改一下**：把 `PWM_ActionQual_Set` 和 `Clear` 互换 → PWM 反相（低电平时间变成 CMPA）；把 `setActionQual_CntUp_CmpA_PwmA` 改成 `..._PwmB` → 事件改去驱动 EPWMxB。
- **对比 chap10 的 AQ**：chap10 `LED_Pwm.c:120-128` 用了 `CmpA_PwmB ... Clear` + `Period_PwmB ... Set`，让 EPWMxB 与 EPWMxA 互补（一个 set 一个 clear），形成差分驱动——这种"两路互补"的写法也是考点。

#### 片段4：ePWM 中断配置（LED_Pwm.c:152-158）
```c
void LED_PWM_eventConfigure(void)
{
    PWM_setIntMode(myPwm1, PWM_IntMode_CounterEqualZero);   //TBCTR=0 触发中断
    PWM_setIntPeriod(myPwm1, PWM_IntPeriod_FirstEvent);    //每次事件都触发
}
```
- **作用**：EPWM1 在 TBCTR=0（每个周期起点）发中断。
- **关键寄存器**：`ETSEL.INTSEL`（中断事件选择，`PWM_IntMode_CounterEqualZero=1<<0`，见 `pwm.h:637`）、`ETPS.INTPRD`（中断周期，`FirstEvent` = 每个事件都发）。
- **配套使能**：在 `User_Pie.c:92` 还要 `PWM_enableInt(myPwm1)` 置 `ETSEL.INTEN`，并 `PIE_enableInt(.., Group3, EPWM1)` + `CPU_enableInt(.., INT3)`（EPWM 中断在 PIE Group3）。

#### 片段5：ePWM 呼吸灯中断 ISR（isr.c:82-142）
```c
interrupt void LED_EPWM1_isr(void)
{
    static int dir1 = 0;
    if(dir1 == 0) { myCmpA_e1 += 5;  if(myCmpA_e1 >= 20000) dir1 = 1; }
    else          { myCmpA_e1 -= 5;  if(myCmpA_e1 <= 0)    dir1 = 0; }
    // dir2/dir3/dir4 同理，步进分别为 8/12/3，使 4 个 LED 呼吸不同速

    PWM_setCmpA(myPwm1, myCmpA_e1);   // LED1
    PWM_setCmpB(myPwm1, myCmpB_e1);   // LED2
    PWM_setCmpA(myPwm2, myCmpA_e2);   // LED3
    PWM_setCmpB(myPwm2, myCmpB_e2);   // LED4

    PWM_clearIntFlag(myPwm1);          // 清 EPWM 中断标志（ETFLG.INT）
    PIE_clearInt(myPie, PIE_GroupNumber_3);   // EPWM 在 Group3
}
```
- **作用**：每个 PWM 周期（1ms）进一次中断，4 个比较值各自按不同步进做三角波（0→20000→0），实现 4 个 LED 不同速率的呼吸灯。
- **关键点**：
  - **PWM 中断标志必须手动清**：`PWM_clearIntFlag` 写 `ETCLR.INT`（`pwm.h:901-910`）。这与 CPU Timer0（自动清）不同，是高频考点。
  - `PIE_clearInt(.., PIE_GroupNumber_3)` ——EPWM 中断挂在 PIE Group3，必须清对应组锁存。
  - `static int dir1` 在中断里保留方向状态，构成"增-到顶-减-到底"三角波发生器。
- **改一下**：漏写 `PWM_clearIntFlag` → 中断标志一直挂，会立刻再次进中断，主循环得不到执行（PWM 中断风暴）。

#### 片段6：eCAP 捕获配置（myCap.c:51-106）
```c
void myCap_pinConfigure(void)
{
    GPIO_setMode(myGpio, GPIO_Number_5, GPIO_5_Mode_ECAP1);
    GPIO_setPullUp(myGpio, GPIO_Number_5, GPIO_PullUp_Disable);
    GPIO_setDirection(myGpio, GPIO_Number_5, GPIO_Direction_Input);
    GPIO_setQualification(myGpio, GPIO_Number_5, GPIO_Qual_ASync);
}

void myCap_functionConfigure(void)
{
    CAP_setCapContinuous(myCap);                       // 连续捕获模式
    CAP_setStopWrap(myCap, CAP_Stop_Wrap_CEVT2);       // 捕到 CEVT2 后回绕
    CAP_setCapEvtPolarity(myCap, CAP_Event_1, CAP_Polarity_Rising);  // 事件1：上升沿
    CAP_setCapEvtPolarity(myCap, CAP_Event_2, CAP_Polarity_Falling); // 事件2：下降沿
    CAP_setCapEvtReset(myCap, CAP_Event_1, CAP_Reset_Enable);        // 事件1后复位时基计数器
    CAP_setCapEvtReset(myCap, CAP_Event_2, CAP_Reset_Disable);
    CAP_enableCaptureLoad(myCap);                      // 允许捕获值装入 CAP1/2 FIFO
}

void myCap_eventConfigure(void)
{
    CAP_enableTimestampCounter(myCap);   // 启动捕获时基计数器
}
```
- **作用**：把 GPIO5 复用成 ECAP1 输入，配置成"连续捕获、2 事件回绕、事件1=上升沿且复位计数器、事件2=下降沿不复位"，于是 `CAP1` 寄存器存上升沿时刻、`CAP2` 存下降沿时刻，二者差即为高电平时长。
- **关键寄存器**：`ECAP1` 的 `ECCTL1/2`、`ECEINT`、`CAP1/CAP2`（捕获 FIFO）。
- **原理**：eCAP 内部有一个 32 位时基计数器（TSCTR），每个边沿事件把当前 TSCTR 锁存到 CAPx；事件1 复位 TSCTR，所以 CAP2 直接等于本次高电平时长（TSCTR 时钟 = SYSCLKOUT 时可直接换算成 us）。

#### 片段7：eCAP 中断 ISR（isr.c:147-159）
```c
interrupt void myCap_CAPINT_isr(void)
{
    myCapVal1 = CAP_getCap1(myCap);   // 上升沿时刻
    myCapVal2 = CAP_getCap2(myCap);   // 下降沿时刻

    CAP_clearInt(myCap, CAP_Int_Type_CEVT2);   // 清事件2中断标志
    CAP_clearInt(myCap, CAP_Int_Type_Global);  // 清全局中断标志

    PIE_clearInt(myPie, PIE_GroupNumber_4);    // eCAP 在 PIE Group4
}
```
- **作用**：捕到下降沿（CEVT2）后进中断，读两个捕获值，清两层中断标志，清 PIE Group4 锁存。
- **关键点**：eCAP 也要"外设标志 + PIEACK"双重清理；`CAP_getCap1/2` 读 `CAP1/CAP2` FIFO。

#### 片段8：开 ePWM/eCAP 时钟（User_System.c:75-80）
```c
CLK_enablePwmClock(myClk, PWM_Number_1); //EPWM1 clock
CLK_enablePwmClock(myClk, PWM_Number_2); //EPWM2 clock
CLK_enableEcap1Clock(myClk);  //ECAP clock
```
- **作用**：置 `PCLKCR1.EPWM1ENCLK/EPWM2ENCLK`（bit0/bit1，`clk.h:93-94`）和 `PCLKCR1.ECAP1ENCLK`（bit8，`clk.h:121`）。
- **要点**：`PWM_Number_1 = 0` 对应 EPWM1，依次类推；ePWM 时钟不开则 `myPwm1->TBPRD` 等寄存器写不进去。

### 该工程常考点小结
- ePWM 配置八步：TBCLKSYNC=0 → TBCLK 分频 → 计数模式 → TBPRD → CMPA/CMPB 影子模式 → AQ 动作限定 → 中断配置 → TBCLKSYNC=1
- AQ 动作限定"某事件 Set/Clear"决定波形；占空比 = CMPA/TBPRD（向上计数）
- 影子寄存器 + 装载时刻（Zero/Period）避免改值毛刺
- **PWM/ADC/CAP 中断标志必须 ISR 内手动清**（与 CPU Timer0 自动清对照）
- eCAP 连续捕获 + 事件极性 + 复位时基，测脉宽原理
- 外设时钟使能：`PCLKCR0/1/3` 各 bit 对应不同外设

---

## 工程6：chap10_ADC_1 —— ADC 采样 + ePWM 触发 SOC

### 工程概览
- **外设**：ADC + ePWM1/ePWM2 + eCAP1 + GPIO + PIE
- **功能**：用片内温度传感器（ADC 通道 A5/温度传感器共享）做 ADC 采样；ePWM1 周期事件产生 ADCSOCA 触发 ADC SOC0/SOC2；ADC EOC0 产生 ADCINT1 中断；中断里读结果并换算温度。同时保留 ePWM 呼吸灯、eCAP 捕获（继承 chap9）。
- **关键文件**：`User_Component/myAdc/myAdc.c/.h`、`User_Component/LED_Pwm/LED_Pwm.c`、`User_Component/User_Pie/User_Pie.c`、`Application/isr.c`、`User_Component/User_System/User_System.c`

### 关键代码片段

#### 片段1：使能 ADC/ePWM/eCAP 时钟（User_System.c:71-80）
```c
CLK_enableAdcClock(myClk);  //ADC colck         ——PCLKCR0.ADCENCLK bit3
CLK_enablePwmClock(myClk, PWM_Number_1); //EPWM1 clock
CLK_enablePwmClock(myClk, PWM_Number_2); //EPWM2 clock
CLK_enableEcap1Clock(myClk);  //ECAP clock
```
- **作用**：把要用到的 4 个外设时钟全开（ADC 在 PCLKCR0 bit3，`clk.h:67`）。

#### 片段2：ADC 引脚配置——使能温度传感器（myAdc.c:50-56）
```c
void myADC_pinConfigure(void)
{
    ADC_enableTempSensor(myAdc);
}
```
- **作用**：把片内温度传感器连到 ADC 通道 A5（温度传感器与 A5 复用，使能后 A5 外部引脚断开）。
- **关键寄存器**：写 `ADCCTL1.TEMPCONV` 位。

#### 片段3：ADC 功能配置——上电、参考、SOC（myAdc.c:62-90）
```c
void myADC_functionConfigure(void)
{
    //1. setup power
    ADC_powerUp(myAdc);
    ADC_enableBandGap(myAdc);
    ADC_enableRefBuffers(myAdc);
    //2. enable ADC
    ADC_enable(myAdc);
    //3. Volt ref
    ADC_setVoltRefSrc(myAdc, ADC_VoltageRefSrc_Int);
    //4. SOC ——配置两个 SOC 通道
    ADC_setSocChanNumber(myAdc, ADC_SocNumber_0, ADC_SocChanNumber_A5);
    ADC_setSocSampleWindow(myAdc, ADC_SocNumber_0, ADC_SocSampleWindow_7_cycles);
    ADC_setSocTrigSrc(myAdc, ADC_SocNumber_0, ADC_SocTrigSrc_EPWM1_ADCSOCA);

    ADC_setSocChanNumber(myAdc, ADC_SocNumber_2, ADC_SocChanNumber_A5);
    ADC_setSocSampleWindow(myAdc, ADC_SocNumber_2, ADC_SocSampleWindow_7_cycles);
    ADC_setSocTrigSrc(myAdc, ADC_SocNumber_2, ADC_SocTrigSrc_EPWM1_ADCSOCA);
}
```
- **作用**：ADC 上电（带隙基准、参考缓冲、ADC 使能、内部参考），配置 SOC0 和 SOC2 都采通道 A5、采样窗 7 个 ADC 时钟周期、由 EPWM1 的 ADCSOCA 脉冲触发。
- **关键寄存器/宏**：
  - `ADC_powerUp/enableBandGap/enableRefBuffers` → 写 `ADCPWRCTL`（PWDN/bandgap/refbuf 位）。F2802x ADC 上电顺序：先带隙→参考缓冲→ADC 使能，需要等待稳定。
  - `ADC_setVoltRefSrc(.., ADC_VoltageRefSrc_Int)` → 用内部 3.0V 参考（`adc.h:454`）。
  - `ADC_setSocChanNumber(.., SOC0, ADC_SocChanNumber_A5)` → 写 `ADCSOC0CTL.CHSEL`（`A5 = 5<<6`，`adc.h:314`），选通道 A5。
  - `ADC_setSocSampleWindow(.., SOC0, ADC_SocSampleWindow_7_cycles)` → 写 `ADCSOC0CTL.ACPS`，采样窗 7 周期（`adc.h:363`）。
  - `ADC_setSocTrigSrc(.., SOC0, ADC_SocTrigSrc_EPWM1_ADCSOCA)` → 写 `ADCSOC0CTL.TRIGSEL = 5<<11`（`adc.h:433`），触发源选 EPWM1 的 ADCSOCA 脉冲。
- **原理**：F2802x ADC 是"Round-Robin SOC"架构，最多 16 个 SOC（SOC0~15），每个 SOC 独立配置通道/采样窗/触发源；触发来时该 SOC 自动转换，转换结束 (EOC) 置位对应 EOC 标志。
- **改一下**：把 `ADC_SocTrigSrc_EPWM1_ADCSOCA` 改成 `ADC_SocTrigSrc_CpuTimer_0` → 用 Timer0 触发采样（`myAdc.c:81` 注释里就有这行）；把 `A5` 改成 `A0` → 采外部 A0 引脚（要先关温度传感器）。

#### 片段4：ADC 中断配置（myAdc.c:96-101）
```c
void myADC_eventConfigure(void)
{
    ADC_setIntMode(myAdc, ADC_IntNumber_1, ADC_IntMode_ClearFlag);
    ADC_setIntPulseGenMode(myAdc, ADC_IntPulseGenMode_Prior);
    ADC_setIntSrc(myAdc, ADC_IntNumber_1, ADC_IntSrc_EOC0);
}
```
- **作用**：ADC 中断 1 配成"EOC0 触发、需手动清 flag 才能再发"模式，脉冲先于结果写入产生。
- **关键宏**：
  - `ADC_IntMode_ClearFlag = 0`（`adc.h:222`）→ 必须在 ISR 内 `ADC_clearIntFlag` 才能再触发，避免连续触发。
  - `ADC_IntSrc_EOC0 = 0<<0`（`adc.h:240`）→ 中断 1 的源是 SOC0 转换结束。
- **配套使能**（`User_Pie.c:97`）：`ADC_enableInt(myAdc, ADC_IntNumber_1)` → 置 `ADCINTSEL1N2.INT1CONT/INT1E`；`PIE_enableInt(.., Group10, ADCINT_10_1)` → ADCINT1 在 PIE Group10 SubGroup1；`CPU_enableInt(.., INT10)`。

#### 片段5：ePWM 产生 ADC SOC 触发（LED_Pwm.c:139-154）
```c
void LED_PWM_eventConfigure(void)
{   // 1.1 EPWMxINT（与 chap9 相同）
    PWM_setIntMode(myPwm1, PWM_IntMode_CounterEqualZero);
    PWM_setIntPeriod(myPwm1, PWM_IntPeriod_FirstEvent);

    //1.2 ADC EPWMxSOCA ——用 EPWM 事件触发 ADC SOC
    PWM_setSocAPulseSrc(myPwm1, PWM_SocPulseSrc_CounterEqualPeriod); //TBCTR=TBPRD 时发 SOCA
    PWM_setSocAPeriod(myPwm1, PWM_SocPeriod_FirstEvent);             //每次事件都发
    PWM_enableSocAPulse(myPwm1);                                     //使能 SOCA 输出
}
```
- **作用**：EPWM1 在计数值=周期（TBPRD）时发出 ADCSOCA 脉冲，去触发前面配好的 SOC0/SOC2。
- **关键寄存器**：`ETSEL.SOCASESEL`（SOCA 事件选择，`PWM_SocPulseSrc_CounterEqualPeriod`，`pwm.h:747`）、`ETPS.SOCAPRD`（SOCA 周期，`FirstEvent`）、`ETSEL.SOCAEN`（使能）。
- **原理**：ePWM 的"Event-Trigger"子模块可以把 TBCTR 等于 0/周期/CMPA/CMPB 的事件，转换成 ADCSOC 脉冲或中断脉冲。本工程选"周期事件"发 SOCA，配合 ADC `EPWM1_ADCSOCA` 触发源。
- **改一下**：把 `CounterEqualPeriod` 改成 `CounterEqualCmpA` → 在 CMPA 处触发采样（常用于在 PWM 中点采样电流）。

#### 片段6：ADC 中断 ISR——读结果+算温度（isr.c:105-117）
```c
interrupt void myAdc_ADCINT_isr(void)
{
    Temp  = ADC_readResult(myAdc, ADC_ResultNumber_0);          // 读 ADCRESULT0
    TempC = ADC_getTemperatureC(myAdc, Temp);                   // 换算摄氏度

    ADC_clearIntFlag(myAdc, ADC_IntNumber_1);                   // 清 ADCINT1 标志

    PIE_clearInt(myPie, PIE_GroupNumber_10);                    // ADC 在 PIE Group10
}
```
- **作用**：ADC 转换完成进中断，读 `ADCRESULT0`（对应 SOC0 的结果），用库函数换算温度，清 ADC 中断标志 + PIE Group10 锁存。
- **关键 API**：
  - `ADC_readResult(.., ADC_ResultNumber_0)` → 读 `ADCRESULT0` 寄存器（12 bit，左对齐到 16 bit 高位）。
  - `ADC_clearIntFlag(.., ADC_IntNumber_1)` → 写 `ADCINTFLGCLR.INT1`。
- **要点**：**ADC 中断标志必须手动清**（因为前面配成 `ClearFlag` 模式）；PIE Group10 是 ADCINT1~8 所在组。
- **改一下**：漏写 `ADC_clearIntFlag` → 中断标志不清，`ClearFlag` 模式下再也进不了新中断。

### 该工程常考点小结
- ADC 上电顺序：powerUp → bandgap → refbuf → enable → voltref
- SOC 三要素：`setSocChanNumber`（通道）、`setSocSampleWindow`（采样窗）、`setSocTrigSrc`（触发源）
- ePWM 触发 ADC：`PWM_setSocAPulseSrc` + `PWM_enableSocAPulse` 配合 ADC 的 `EPWMx_ADCSOCA` 触发源
- ADC 中断：`ADC_setIntSrc(.., EOC0)` + `ADC_setIntMode(.., ClearFlag)` + ISR 内 `ADC_clearIntFlag`
- ADC 中断在 PIE Group10，与 EPWM(Group3)/CAP(Group4)/Timer0(Group1) 的组号区分

---

## 工程7：chap11_SCI_1 —— SCI 串口收发（查询+中断+FIFO）

### 工程概览
- **外设**：SCI-A（串口）+ ADC + ePWM + eCAP + CPU Timer0 + GPIO + PIE（继承全部前面外设的中断框架，但实际启用 SCI + ADC）
- **功能**：
  - 查询方式 `SCI_Test1()`：主循环里阻塞收一个字符、原样回发（串口回显器）；
  - 中断+FIFO 方式 `SCI_ExchageData()`：RX 中断把数据存入 `SciReceiveBuf`，收到 `\0` 后 TX 中断把缓冲区逐字节回发；
  - Timer0 中断同时驱动一个"时分秒"软件时钟翻转 LED1。
- **关键文件**：`User_Component/mySci/mySci.c/.h`、`Application/app.c`、`Application/isr.c`、`User_Component/User_Pie/User_Pie.c`、`User_Component/User_System/User_System.c`、`F2802x_Component/include/sci.h`

### 关键代码片段

#### 片段1：LSPCLK 配置 + SCI 时钟（User_System.c:67-84）
```c
    // 4. Configure the system clock output(SYSCLKOUT) SYSCLKOUT = 10Mhz * 12 / 2 = 60Mhz
    PLL_setup(myPll, PLL_Multiplier_12, PLL_DivideSelect_ClkIn_by_2);
    // 5. Configure the low speed peripheral clock(LSPCLK) LSPCLK = SYSCLKOUT/4 =15MHz
    CLK_setLowSpdPreScaler(myClk, CLK_LowSpdPreScaler_SysClkOut_by_4);
    // 6. Module's Clock configure
    CLK_enablePwmClock(myClk, PWM_Number_1);
    CLK_enablePwmClock(myClk, PWM_Number_2);
    CLK_enableSciaClock(myClk);  //SCIA clock  ——PCLKCR0.SCIAENCLK bit10
```
- **作用**：除 SYSCLKOUT=60MHz 外，**专门配 LSPCLK = 60MHz/4 = 15MHz**（SCI/SPI 等低速外设的时钟源），再开 SCI-A 时钟。
- **关键寄存器/宏**：
  - `CLK_setLowSpdPreScaler(.., SysClkOut_by_4)` → 写 `LOSPCP.LSPCLK`（`clk.h:269`，值 2 = /4），默认上电就是 /4，这里显式写出便于理解波特率公式。
  - `CLK_enableSciaClock` → 置 `PCLKCR0.SCIAENCLK`（`CLK_PCLKCR0_SCIAENCLK_BITS = 1<<10`，`clk.h:83`）。
- **为什么 LSPCLK 重要**：SCI 波特率公式 `BRR = LSPCLK/(BAUD×8) - 1`，LSPCLK 变波特率就变。

#### 片段2：SCI 引脚复用（mySci.c:54-67）
```c
void mySCI_pinConfigure(void)
{
    // 1. set mode
    GPIO_setMode(myGpio, GPIO_Number_28, GPIO_28_Mode_SCIRXDA);   // 复用值 1
    GPIO_setMode(myGpio, GPIO_Number_29, GPIO_29_Mode_SCITXDA);   // 复用值 1
    // 2. set pullup
    GPIO_setPullUp(myGpio, GPIO_Number_28, GPIO_PullUp_Enable);   // RX 上拉（空闲高电平）
    GPIO_setPullUp(myGpio, GPIO_Number_29, GPIO_PullUp_Disable);
    //3. set direction
    GPIO_setDirection(myGpio, GPIO_Number_28, GPIO_Direction_Input);
    GPIO_setDirection(myGpio, GPIO_Number_29, GPIO_Direction_Output);
    //4 input qualification
    GPIO_setQualification(myGpio, GPIO_Number_28, GPIO_Qual_ASync); // 异步，避免采样窗影响串口时序
}
```
- **作用**：GPIO28 复用为 SCIRXDA、GPIO29 复用为 SCITXDA；RX 上拉保证串口空闲时是高电平（串口协议要求）；RX 异步采样以匹配串口异步时序。
- **关键宏**：`GPIO_28_Mode_SCIRXDA`、`GPIO_29_Mode_SCITXDA`（`gpio.h:115-120`，复用值 1）。
- **要点**：串口引脚**必须配异步输入**（`GPIO_Qual_ASync`），否则 GPIO 采样窗会破坏串口 bit 时序。

#### 片段3：SCI 波特率与帧格式（mySci.c:72-85）
```c
void mySCI_functionConfigure(void)
{
    //1. SCI BRR = LSPCLK/(SCI BAUDx8) - 1
    SCI_setBaudRate(mySci, SCI_BaudRate_9_6_kBaud);
    //2. configure package(1 stop bit, No loopback, No parity, 8 char bits, async mode, idle-line protocol)
    SCI_disableParity(mySci);
    SCI_setNumStopBits(mySci, SCI_NumStopBits_One);
    SCI_setCharLength(mySci, SCI_CharLength_8_Bits);
    //3. enable SCI TX&RX
    SCI_enableTx(mySci);
    SCI_enableRx(mySci);
    //4. enable SCI module
    SCI_enable(mySci);
}
```
- **作用**：配 9600 波特率、无校验、1 停止位、8 数据位、空闲线协议；使能 TX/RX 和 SCI 模块。
- **关键寄存器/宏**（`sci.h:328-345` 的 `SCI_Obj`：`SCICCR、SCICTL1、SCIHBAUD、SCILBAUD、SCICTL2...`）：
  - `SCI_setBaudRate(.., SCI_BaudRate_9_6_kBaud)` → 写 `SCIHBAUD:SCILBAUD = 194`。**BRR=194 → Baud = LSPCLK/(8×(BRR+1)) = 15MHz/(8×195) ≈ 9615 ≈ 9600**（`sci.h:243`）。其它常用值：`19_2k=97`、`57_6k=33`、`115_2k=15`。
  - `SCI_disableParity` → 清 `SCICCR.PARITYENA`(bit5)。
  - `SCI_setNumStopBits(.., One)` → 清 `SCICCR.STOP`(bit7，0=1 停止位，`sci.h:278`）。
  - `SCI_setCharLength(.., 8_Bits)` → `SCICCR.CHAR = 7`（bit0-2，`sci.h:261`）。
  - `SCI_enableTx/Rx` → 置 `SCICTL1.TXWAKE/RXENA` 等；`SCI_enable` → 置 `SCICTL1.SWRESET`，**SCI 改配置后必须 SWRESET 复位**才能生效。
- **改一下**：把 `9_6_kBaud` 改成 `115_2_kBaud` → 波特率提高 12 倍，对端也要跟着改；漏写 `SCI_enable` → 配置不生效。

#### 片段4：SCI 收发内联 API（mySci.h:117-158）
```c
TARGET_EXT void inline mySCI_sendCharBlocking(uint16_t ch)
{
    SCI_putDataBlocking(mySci, ch);          // 等待 TXBUF 空 → 写 SCITXBUF
}

TARGET_EXT uint16_t inline mySCI_receiveCharBlocking(void)
{
    return SCI_getDataBlocking(mySci);       // 等待 RXBUF 满 → 读 SCIRXBUF
}

// FIFO 版本
TARGET_EXT void inline mySCI_Fifo_sendCharBlocking(uint16_t ch)
{
    while(SCI_getTxFifoStatus(mySci) == SCI_FifoStatus_4_Words) {}  // 等 TX FIFO 不满
    SCI_putData(mySci, ch);                                          // 非阻塞写
}

TARGET_EXT uint16_t inline mySCI_Fifo_receiveCharBlocking(void)
{
    while(SCI_getRxFifoStatus(mySci) == SCI_FifoStatus_Empty) {}     // 等 RX FIFO 非空
    return SCI_getData(mySci);                                       // 非阻塞读
}
```
- **作用**：阻塞版直接等"TX 空/RX 满"单字节寄存器收发；FIFO 版查 FIFO 状态字（`SCIFFTX.TXFFST`/`SCIFFRX.RXFFST`，状态字 `0/1/2/3/4` 表示 FIFO 内字数）。
- **关键寄存器**：`SCITXBUF`（发送缓冲，写即启动发送）、`SCIRXBUF`（接收缓冲）、`SCIFFTX/SCIFFRX`（FIFO 控制/状态）。
- **原理**：阻塞版每次 1 字节都要等完整帧发完，CPU 浪费；FIFO 版可连续写 4 字节再等，效率更高。

#### 片段5：SCI FIFO 配置（mySci.c:112-138）
```c
void mySCIFIFO_functionConfigure(void)
{
    SCI_enableFifoEnh(mySci);    // 使能 FIFO 增强
    SCI_resetChannels(mySci);    // 复位 FIFO 通道
    SCI_resetTxFifo(mySci);      // 复位 TX FIFO
    SCI_resetRxFifo(mySci);      // 复位 RX FIFO
    SCI_setTxDelay(mySci, 10);   // 发送延时
}

void mySCIFIFO_eventConfigure(void)
{
    SCI_setTxFifoIntLevel(mySci, SCI_FifoLevel_Empty);    // TX FIFO 空触发中断
    SCI_setRxFifoIntLevel(mySci, SCI_FifoLevel_1_Word);   // RX FIFO 1 字触发中断
}
```
- **作用**：使能 SCI FIFO（16 级 TX/RX FIFO），复位 FIFO，设 TX 延时；中断触发电平：TX 空、RX 1 字。
- **关键寄存器**：`SCIFFTX.TXFFIL`（TX FIFO 中断触发电平）、`SCIFFRX.RXFFIL`（RX FIFO 中断触发电平）。`SCI_FifoLevel_Empty=0`、`_1_Word=1`（`sci.h:305-310`）。
- **原理**：FIFO 把"每字节一次中断"变成"积攒到 N 字或空时才中断"，大幅降中断频率。

#### 片段6：SCI 查询回显（app.c:77-84）
```c
void SCI_Test1(void)
{
    uint16_t ch;
    while(1)
    {
        ch = mySCI_receiveCharBlocking();  // 阻塞等接收
        mySCI_sendCharBlocking(ch);        // 原样发送回去
    }
}
```
- **作用**：查询方式实现串口回显——收到一个字符就立刻发回去。`main.c` 主循环里调用的就是这个，所以板子表现为"串口助手发什么收什么"。
- **要点**：查询方式**不需要开中断**，但 CPU 100% 卡在 `while(1)` 里等字符；`main.c:82` 调用 `User_Pie_start()` 开了中断也没影响（没注册 SCI ISR 时不进）。

#### 片段7：SCI 中断收发 ISR（isr.c:141-183）
```c
interrupt void mySCI_TXINT_isr(void)
{
    if(true == SciSentOk)
    {
        SciSendPoint +=1;
        if('\0' != SciSendBuf[SciSendPoint] && SciSendPoint <=50)
            SCI_putData(mySci, SciSendBuf[SciSendPoint]);   // 继续发下一个
        else
            SciSentOk = false;                              // 发完停止
    }
    PIE_clearInt(myPie, PIE_GroupNumber_9);   // SCI 在 PIE Group9
}

interrupt void mySCI_RXINT_isr(void)
{
    uint16_t ch;
    if(false == SciReceivedOk)
    {
        ch = SCI_getData(mySci);                            // 读接收数据（清 RX 标志）
        SciReceiveBuf[SciReceivePoint] = ch;
        SciReceivePoint += 1;
        if(SciReceivePoint >=50) SciReceivePoint = 0;
        if('\0' == ch) SciReceivedOk = true;                // 收到串尾置完成
    }
    PIE_clearInt(myPie, PIE_GroupNumber_9);
}
```
- **作用**：TX 中断每发完一个字节进一次，继续发 `SciSendBuf` 下一个直到 `\0`；RX 中断每收一个字节存入 `SciReceiveBuf`，收到 `\0` 置 `SciReceivedOk`。`app.c` 的 `SCI_ExchageData` 检测到 `SciReceivedOk` 后把收到的数据搬到 `SciSendBuf` 并触发首次 TX，启动回发。
- **关键点**：
  - SCI TX/RX 中断在 PIE **Group9 SubGroup2/SubGroup1**（`pie.h:507-508`：`SCIARX = 1<<0`、`SCIATX = 1<<1`）。
  - `SCI_getData` 读 `SCIRXBUF` 同时会自动清 `SCIRXINT` 标志；`SCI_putData` 写 `SCITXBUF` 启动发送。
  - ISR 末尾清 `PIE_GroupNumber_9`。
- **配套使能**（`User_Pie.c:74-78,100-106,130-131`）：注册 TX/RX ISR → `SCI_enableTxInt/enableRxInt` → `CPU_enableInt(.., INT9)`。注意 PIE 使能 SCIATX/SCIARX 那两行在工程里被注释了，所以中断方式实际未完全启用，默认走的是查询 `SCI_Test1`。

#### 片段8：Timer0 中断里的软件时钟（isr.c:57-85）
```c
interrupt void myTimer_CpuTimer0_isr(void)
{
    HalfSecond++;
    if(2 <= HalfSecond)       // 半秒×2 = 1秒
    {
        HalfSecond = 0;
        LED_toggle(LED1);
        Second++;
        if(Second >= 60) { Second = 0; Minute++; ... 时分秒进位 ... }
    }
    PIE_clearInt(myPie, PIE_GroupNumber_1);
}
```
- **作用**：Timer0 周期 0.5 秒（在 `myTimer.c` 配置），中断累计 2 次为 1 秒，维护"时:分:秒"软件时钟并每秒翻转 LED1。
- **要点**：典型的"用定时器中断做软件时钟"模式——硬件只给一个固定节拍，进位用软件累计。`HalfSecond/Second/Minute/Hour` 都是全局变量。

### 该工程常考点小结
- SCI 引脚**必须异步输入 + RX 上拉**
- 波特率公式 `BRR = LSPCLK/(8×Baud) - 1`，LSPCLK=SYSCLKOUT/4=15MHz 时 9600 对应 BRR=194
- 帧格式：`SCICCR` 里 CHAR(bit0-2)、PARITY(bit5)、STOP(bit7)；改配置后需 `SCI_enable`(SWRESET)
- 查询收发 `SCI_putDataBlocking/getDataBlocking` 与 FIFO 收发 `SCI_putData/getData + 状态字` 的区别
- SCI TX/RX 中断在 PIE **Group9**，与 ADC(Group10)/EPWM(Group3)/CAP(Group4)/Timer0(Group1) 区分
- FIFO 中断触发电平 `SCI_setTxFifoIntLevel/setRxFifoIntLevel` 降中断频率

---

# 跨工程高频考点速记表

## 一、系统初始化"开场白"（每个工程 `User_System_functionConfigure` 都有）
| 步骤 | 函数 | 操作寄存器/位 | 必背点 |
|---|---|---|---|
| 授权 | `System_authorize()` | 给所有 myXxx 句柄赋基址指针 | 句柄=结构体指针映射寄存器 |
| 关 WDG | `WDOG_disable(myWDog)` | `WDCR.WDDIS`(bit6) | 必须 EALLOW，否则跑飞 |
| 关中断 | `CPU_disableGlobalInts(myCpu)` | `asm(" setc INTM")` | 配 PIE 前必须关 |
| 时钟源 | `CLK_setOscSrc(.., Internal)` | `CLKCTL.OSCCLKSRCSEL` | 内部 10MHz |
| PLL | `PLL_setup(.., 12, by_2)` | `SYSCLKOUT=10×12/2=60MHz` | F2802x 最高 60MHz |
| LSPCLK | `CLK_setLowSpdPreScaler(.., by_4)` | `LOSPCP.LSPCLK` | 仅 chap11 显式写，LSPCLK=15MHz |
| 外设时钟 | `CLK_enableXxxClock` | `PCLKCR0/1/3` 各 bit | 用谁开谁，不开寄存器写不进 |
| PIE 模板 | `PIE_disable → disableAllInts → CPU_clearIntFlags → setDefaultIntVectorTable → PIE_enable` | `PIECTRL.ENPIE`、`PIEACK` | 配 PIE 前关，配完开 |

## 二、外设时钟使能速查（PCLKCR）
| 外设 | 函数 | 寄存器/位 |
|---|---|---|
| GPIO 输入 | `CLK_enableGpioInputClock` | PCLKCR3 bit13 |
| CPU Timer0/1/2 | `CLK_enableCpuTimerClock(.., 0/1/2)` | PCLKCR3 bit8/9/10 |
| EPWM1~4 | `CLK_enablePwmClock(.., PWM_Number_1~4)` | PCLKCR1 bit0~3 |
| ECAP1 | `CLK_enableEcap1Clock` | PCLKCR1 bit8 |
| ADC | `CLK_enableAdcClock` | PCLKCR0 bit3 |
| SCI-A | `CLK_enableSciaClock` | PCLKCR0 bit10 |
| TBCLKSYNC | `CLK_enable/disableTbClockSync` | PCLKCR0 bit2，配 ePWM 前关后开 |

## 三、GPIO 配置三件套（每个工程都有）
| 函数 | 操作寄存器 | 位操作 |
|---|---|---|
| `GPIO_setMode(.., pin, mode)` | `GPAMUX1/GPAMUX2/GPBMUX1` | 每 pin 2 bit，先清 `0x3<<lShift` 再写 mode |
| `GPIO_setPullUp(.., pin, Disable/Enable)` | `GPAPUD/GPBPUD` | 1 bit，0=上拉使能 |
| `GPIO_setDirection(.., pin, In/Out)` | `GPADIR/GPBDIR` | 1 bit，1=输出 |
| `GPIO_setQualification(.., pin, Qual_xxx)` | `GPAQSEL1/2、GPBQSEL1` | 每 pin 2 bit：Sync/3Sample/6Sample/ASync |
| `GPIO_setQualificationPeriod(.., pin, n)` | `GPACTRL/GPBCTRL` | 采样窗周期，8 pin 共享一组 |

## 四、GPIO 数据寄存器四件套（务必分清）
| 寄存器 | 写 1 含义 | 用法 |
|---|---|---|
| `GPADAT` | 直接写整端口 | 读改写，非原子 |
| `GPASET` | 写 1 的位置 1，写 0 无影响 | 原子置高（`GPIO_setHigh`） |
| `GPACLEAR` | 写 1 的位清 0 | 原子置低（`GPIO_setLow`） |
| `GPATOGGLE` | 写 1 的位翻转 | 原子翻转（`GPIO_toggle`） |

## 五、PIE 三级中断配置（最高频考点，chap6~11 全用）
```
① PIE_registerPieIntHandler(myPie, Group_x, SubGroup_y, (intVec_t)&ISR);  // 注册向量
② 外设级使能：TIMER_enableInt / PWM_enableInt / ADC_enableInt / CAP_enableInt / SCI_enableTxInt...
③ PIE_enableInt(myPie, Group_x, PIE_InterruptSource_xxx);                  // PIEIERx.y
④ CPU_enableInt(myCpu, CPU_IntNumber_x);                                   // IERx
⑤ User_Pie_start() → CPU_enableGlobalInts → asm(" clrc INTM")              // 开总中断
```
**ISR 模板**：
```c
interrupt void XXX_isr(void)
{
    // ... 业务逻辑 ...
    外设清flag（PWM/ADC/CAP 必须手动清；CPU Timer0/XINT 自动清）;
    PIE_clearInt(myPie, PIE_GroupNumber_x);   // 清 PIEACK 组锁存，必写！
}
```

## 六、各外设 PIE 组号 + ISR 末尾清的组（必背）
| 外设 | PIE Group | SubGroup | ISR 末尾清 | 外设 flag 是否手动清 |
|---|---|---|---|---|
| CPU Timer0 | Group1 | Sub7(TINT0) | `PIE_GroupNumber_1` | 否（自动） |
| XINT1/2 | Group1 | Sub4/Sub5 | `PIE_GroupNumber_1` | 否（自动） |
| EPWM1~4 INT | Group3 | Sub1~4 | `PIE_GroupNumber_3` | **是** `PWM_clearIntFlag` |
| ECAP1 | Group4 | Sub1 | `PIE_GroupNumber_4` | **是** `CAP_clearInt`(CEVT+Global) |
| SCI-A RX/TX | Group9 | Sub1/Sub2 | `PIE_GroupNumber_9` | 读 SCIRXBUF 自动清 RX；TX 无需 |
| ADCINT1/2 | Group10 | Sub1/Sub2 | `PIE_GroupNumber_10` | **是** `ADC_clearIntFlag` |

## 七、CPU 中断号 IER 映射
`CPU_IntNumber_1 = 1<<0` … `CPU_IntNumber_12 = 1<<11`，对应 PIE Group1~12 → CPU INT1~12。Timer0/XINT 走 INT1、EPWM 走 INT3、ECAP 走 INT4、SCI 走 INT9、ADC 走 INT10。

## 八、定时器周期公式（CPU Timer）
```
T = (PRD) × (TPR+1) / SYSCLKOUT
SYSCLKOUT = 60MHz
PRD = 60000000-1, TPR = 0  →  T = 1s
```
**减计数器，写周期值要 N-1**。`TIMER_stop → setDecimationFactor → setPeriod → reload → start` 五步。

## 九、ePWM 配置八步（chap9/10 核心）
```
1. CLK_disableTbClockSync(myClk);                  // 冻结
2. PWM_setHighSpeedClkDiv / PWM_setClkDiv          // TBCLK = SYSCLKOUT/(HSPCLKDIV×CLKDIV)
3. PWM_setCounterMode(.., Up/UpDown/Down)          // 计数模式
4. PWM_setPeriod(.., TBPRD) + setPeriodLoad(Shadow)// 周期 + 影子
5. PWM_setShadowMode_CmpA/CmpB + setLoadMode_CmpA/CmpB // CMPA/CMPB 影子装载
6. PWM_setCmpA / setCmpB                           // 比较值
7. PWM_setActionQual_xxx_PwmA/PwmB(Set/Clear)      // 动作限定
8. CLK_enableTbClockSync(myClk);                   // 同步启动
```
**PWM 周期（向上计数）= TBPRD × (HSPCLKDIV×CLKDIV) / SYSCLKOUT**；**占空比 = CMPA/TBPRD**（按 AQ 配置方向）。

## 十、ADC SOC 三要素 + 中断（chap10 核心）
```
ADC_setSocChanNumber(.., SOCn, 通道)       // CHSEL
ADC_setSocSampleWindow(.., SOCn, 窗口)     // ACPS
ADC_setSocTrigSrc(.., SOCn, 触发源)        // TRIGSEL（EPWMx_ADCSOCA / CpuTimer_0 / 软件）
ADC_setIntSrc(.., INT1, EOC0) + setIntMode(.., ClearFlag)
ADC_enableInt(.., INT1) → PIE_enableInt(.., Group10, ADCINT_10_1) → CPU_enableInt(.., INT10)
ISR 内：ADC_readResult + ADC_clearIntFlag + PIE_clearInt(Group10)
```

## 十一、SCI 配置要点（chap11 核心）
- **引脚**：RX 上拉 + 异步输入；TX 输出
- **波特率**：`BRR = LSPCLK/(8×Baud) - 1`，LSPCLK=15MHz 时 9600→194、19200→97、57600→33、115200→15
- **帧格式（SCICCR）**：CHAR(bit0-2)=7 表 8 位、PARITY(bit5)、STOP(bit7)
- **改配置后必须 `SCI_enable`（SWRESET）**
- **中断组**：SCI TX/RX 在 PIE Group9 Sub2/Sub1
- **FIFO**：`SCI_setTxFifoIntLevel(Empty)` / `setRxFifoIntLevel(1_Word)` 降中断频率

## 十二、EALLOW/EDIS 保护写规则
- **需要 EALLOW**：GPIO 复用/方向/上拉、PIE 配置、PCLKCR、WD、ADC 上电、ePWM 部分寄存器
- **底层 API 已自动包裹**（`ENABLE_PROTECTED_REGISTER_WRITE_MODE ... DISABLE_PROTECTED_REGISTER_WRITE_MODE`）
- **裸写寄存器时**：必须 `EALLOW; GpioCtrlRegs.GPAMUX1 = ...; EDIS;` 三明治写法

## 十三、`interrupt` 关键字
- ISR 函数必须 `interrupt void xxx_isr(void)`，编译器自动保存上下文 + `IRET` 返回
- 函数指针类型 `intVec_t = interrupt void (*)(void)`（`pie.h:384`），注册时 `(intVec_t)&ISR` 强转

## 十四、各工程外设对照一览
| 工程 | 新增外设 | 新增时钟使能 | 新增 PIE 组 |
|---|---|---|---|
| chap4 | GPIO | GPIOINENCLK | 无 |
| chap5 | GPIO+XINT2 模板 | 同 chap4 | 无（中断未启用） |
| chap6 | PIE+CPU Timer0 | 同 chap4（**漏开 Timer0 时钟**） | Group1(Timer0) |
| chap7 | CPU Timer0+XINT1 | **+CPUTIMER0ENCLK** | Group1 |
| chap9 | ePWM1/2+eCAP1 | +EPWM1/2ENCLK、ECAP1ENCLK | Group3(EPWM)、Group4(ECAP) |
| chap10 | ADC | +ADCENCLK | +Group10(ADC) |
| chap11 | SCI-A | +SCIAENCLK、LSPCLK 显式 /4 | +Group9(SCI TX/RX) |

## 十五、考试抽片段识别口诀
1. 看到 `myGpio->GPAMUX` / `GPIO_setMode` → GPIO 复用配置
2. 看到 `PIE_registerPieIntHandler` → 注册中断向量
3. 看到 `PIE_enableInt + CPU_enableInt` → 三级中断后两级
4. 看到 `TIMER_setPeriod(60000000L-1)` → 1 秒定时
5. 看到 `PWM_setActionQual_..._Set/Clear` → ePWM 动作限定（决定波形）
6. 看到 `CLK_disableTbClockSync ... enableTbClockSync` → ePWM 配置三明治
7. 看到 `ADC_setSocTrigSrc(.., EPWM1_ADCSOCA)` → PWM 触发 ADC
8. 看到 `SCI_setBaudRate(.., 9_6_kBaud)` → 串口波特率（BRR=194）
9. 看到 `PIE_clearInt(myPie, PIE_GroupNumber_x)` → ISR 末尾清组锁存
10. 看到 `EALLOW ... EDIS` → 受保护寄存器写
