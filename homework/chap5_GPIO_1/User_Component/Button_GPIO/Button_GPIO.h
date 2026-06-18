/*
 * Button_GPIO.h
 *
 *  Created on: 2020Äê2ÔÂ20ÈÕ
 *      Author: Administrator
 */

#ifndef USER_COMPONENT_BUTTON_GPIO_BUTTON_GPIO_H_
#define USER_COMPONENT_BUTTON_GPIO_BUTTON_GPIO_H_

#include <stdint.h>

// driver
#include "F2802x_Component/F2802x_Device.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifndef TARGET_GLOBAL
   #define TARGET_EXT   extern
#else
   #define TARGET_EXT
#endif

#define   Button1   GPIO_Number_12  //pin


TARGET_EXT uint16_t Button_State_Read(void);
TARGET_EXT void Button_GPIO_pinConfigure(void);


#ifdef __cplusplus
}
#endif // extern "C"


#endif /* USER_COMPONENT_BUTTON_GPIO_BUTTON_GPIO_H_ */
