#ifndef WS2812_H_OUYJY
#define WS2812_H_OUYJY


#include "dma.h"
#include "tim.h"
/*
*使用说明：
*init参数：定时器句柄，定时器通道
*亮度:0-100;
*/
#define LED_NUM 3

typedef enum
{
	WS2812_STATE_OK=0x00,
	WS2812_STATE_IDX_ERROR,
}WS2812_STATE;

typedef struct 
{	
	TIM_HandleTypeDef *htim;
	uint32_t tim_channel;
	int led_num;
	uint8_t errorcode;
	
	void (*init)(TIM_HandleTypeDef *htimx,uint32_t tim_channel);
	void (*start)(void);
	void (*setone)(int led_idx,uint32_t color,uint8_t brightness);
	void (*all_off)(void);
	void (*all_set)(uint32_t color,uint8_t brightness);
}WS2812_HandleTypeDef;

extern WS2812_HandleTypeDef hws2812;


static void WS2812_INIT(TIM_HandleTypeDef *htimx,uint32_t tim_channel);
uint32_t RGB(uint32_t r,uint32_t g,uint32_t b);
#endif

