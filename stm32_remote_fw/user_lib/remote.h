#ifndef REMOTE_H
#define REMOTE_H
/*
使用MDK编译必须勾选GNU-extensions,再option(魔术棒)-c/c++中 
*/
#include "gpio.h"
#include "adc.h"
#include "tim.h"
#include "i2c.h"
#include <stdlib.h>

//数据帧格式
//data[]={按键，左摇杆按键，右摇杆按键，左摇杆x，左摇杆y，右摇杆x，右摇杆y，}
#define KEY_MASK 0
#define JOYSITCK_L_KEY_MASK 1
#define JOYSITCK_R_KEY_MASK 2
#define JOYSTICK_LX_MASK 3
#define JOYSTICK_LY_MASK 4
#define JOYSTICK_RX_MASK 5
#define JOYSTICK_RY_MASK 6

void Initilaize(void);

void DataFresh(void);
void getKeyboard(void);
void getJoyStick(void);

void buzzer_start(void);
void buzzer_stop(void);
void buzeer_cry(int _freq);





typedef struct
{
	GPIO_TypeDef * PORT;
	uint16_t PIN;
}GPIO_T;

typedef struct
{
	TIM_HandleTypeDef *htim;
	uint32_t channel;
}TIM_T;



typedef struct
{
	void(*cry)(int);
	void(*start)(void);
	void(*stop)(void);
	
	TIM_T tim;
	int *spectrum;
}BUZZER_HandleTypeDef;


typedef struct
{
	uint8_t current;
	
	GPIO_T gpio[5];
	uint8_t key[5];
}KEYBOARD_HandleTypeDef;

typedef struct
{
	uint8_t current[4];
	
	
	ADC_HandleTypeDef *adc;
	GPIO_T gpio[2];
	uint16_t joystick_adc[4];
	
}JOYSTICK_HandleTypeDef;



typedef struct
{
	BUZZER_HandleTypeDef buzzer;
	KEYBOARD_HandleTypeDef keyboard;
	JOYSTICK_HandleTypeDef joystick;
	uint8_t data[20];
	void (*Init)(void);
	void (*fresh)(void);
}REMOTE_HandleTypeDef;

extern REMOTE_HandleTypeDef hremote;




#endif

