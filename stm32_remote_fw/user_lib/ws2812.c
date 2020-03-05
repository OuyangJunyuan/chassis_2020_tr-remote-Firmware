#include "ws2812.h"
#include "math.h"


#define LED_BUFFER_LEN (LED_NUM+1)
#define BITBUFFER_PAD 300
#define BITBUFFER_LEN (24*LED_BUFFER_LEN+BITBUFFER_PAD)


#define BIT24_1 (1 << 23)

WS2812_HandleTypeDef hws2812={.init=WS2812_INIT};

static uint16_t bitbuffer[BITBUFFER_LEN];

static uint32_t RGBbuffer[LED_BUFFER_LEN];
static uint8_t brightnessbuffer[LED_BUFFER_LEN];
static uint8_t gamma_table[256];
static uint16_t WS2812_BIT_ONE=66,WS2812_BIT_ZERO=23;



static uint32_t rgb_to_local(uint32_t color,uint8_t brightness);
void ws2811_set_led_color(int led, uint32_t color,uint8_t brightness);
void ws2811_set_all(uint32_t color,uint8_t brightness);
void ws2811_all_off(void);
void WS2812_START_BY_PWM_DMA(void);


static void WS2812_INIT(TIM_HandleTypeDef *htimx,uint32_t tim_channel){
	hws2812.led_num = LED_NUM;
	hws2812.htim = htimx;
	hws2812.tim_channel = tim_channel;
	hws2812.errorcode = WS2812_STATE_OK;
	
	hws2812.start = WS2812_START_BY_PWM_DMA;
	hws2812.setone = ws2811_set_led_color;
	hws2812.all_set = ws2811_set_all;
	hws2812.all_off = ws2811_all_off;
	
	
	uint32_t arr = htimx->Instance->ARR;
	WS2812_BIT_ONE = roundf(0.75*arr);
	WS2812_BIT_ZERO = roundf(0.25*arr);
	
	int i, bit;
	
	for (i = 0;i < LED_BUFFER_LEN;i++) {
		RGBbuffer[i] = 0;
		brightnessbuffer[i]=0;
	}

	for (i = 0;i < LED_BUFFER_LEN;i++) {
		uint32_t tmp_color = rgb_to_local(RGBbuffer[i],brightnessbuffer[i]);

		for (bit = 0;bit < 24;bit++) {
			if(tmp_color & BIT24_1) {
				bitbuffer[bit + i * 24] = WS2812_BIT_ONE;
			} else {
				bitbuffer[bit + i * 24] = WS2812_BIT_ZERO;
			}
			tmp_color <<= 1;
		}
	}
	
	//填充空白作为芯片复位信号(更新收到的rgb值)
	for (i = 0;i < BITBUFFER_PAD;i++) {
		bitbuffer[BITBUFFER_LEN - BITBUFFER_PAD - 1 + i] = 0;
	}

	//gamma校正表产生
	for (int i = 0;i < 256;i++) {
		gamma_table[i] = (int)roundf(powf((float)i / 255.0f, 1.0f / 0.45f) * 255.0f);
	}
}

static void WS2812_START_BY_PWM_DMA(void){
	HAL_TIM_PWM_Start_DMA(hws2812.htim,hws2812.tim_channel,(uint32_t*)bitbuffer,BITBUFFER_LEN);
}

void ws2811_set_led_color(int led, uint32_t color,uint8_t brightness) {
	if (led >= 0 && led < LED_NUM)
	{
		RGBbuffer[led] = color;
		brightnessbuffer[led] = brightness;
		color = rgb_to_local(color,brightness);

		int bit;
		for (bit = 0;bit < 24;bit++) 
		{
			if(color & BIT24_1) 
			{
				bitbuffer[bit + led * 24] = WS2812_BIT_ONE;
			} 
			else 
			{
				bitbuffer[bit + led * 24] = WS2812_BIT_ZERO;
			}
			//24BIT=GRB顺序且高位先发，故左移取第24bit
			color <<= 1;
		}
		hws2812.errorcode = WS2812_STATE_OK;
	}
	else 
		hws2812.errorcode = WS2812_STATE_IDX_ERROR;
}



void ws2811_get_led_color(int led,uint32_t *color) {
	if (led >= 0 && led < LED_NUM) 
	{
		*color=RGBbuffer[led];
		hws2812.errorcode = WS2812_STATE_OK;
	}
	else 
		hws2812.errorcode = WS2812_STATE_IDX_ERROR;
}

void ws2811_get_brightness(int led,uint8_t *brightness) {
	if (led >= 0 && led < LED_NUM) 
	{
		*brightness=brightnessbuffer[led];
		hws2812.errorcode = WS2812_STATE_OK;
	}
	else 
		hws2812.errorcode = WS2812_STATE_IDX_ERROR;
}

void ws2811_all_off(void) {
	int i;

	for (i = 0;i < LED_NUM;i++) {
		RGBbuffer[i] = 0;
	}

	for (i = 0;i < (LED_NUM * 24);i++) {
		bitbuffer[i] = WS2812_BIT_ZERO;
	}
}

void ws2811_set_all(uint32_t color,uint8_t brightness) {
	int i, bit;

	for (i = 0;i < LED_NUM;i++) {
		RGBbuffer[i] = color;
		brightnessbuffer[i]=brightness;
		
		uint32_t tmp_color = rgb_to_local(color,brightness);

		for (bit = 0;bit < 24;bit++) {
			if(tmp_color & BIT24_1) {
				bitbuffer[bit + i * 24] = WS2812_BIT_ONE;
			} else {
				bitbuffer[bit + i * 24] = WS2812_BIT_ZERO;
			}
			tmp_color <<= 1;
		}
	}
}



uint32_t RGB(uint32_t r,uint32_t g,uint32_t b){
	return (r<<16) | (g<<8) | b;
}
/*
*内部在设置bitbuffer的时候都调用了这个函数来亮度gamma校正后真正的rgb值和调换rgb顺序使得。
*/
static uint32_t rgb_to_local(uint32_t color,uint8_t brightness) {
	uint32_t r = (color >> 16) & 0xFF;
	uint32_t g = (color >> 8) & 0xFF;
	uint32_t b = color & 0xFF;

	r = (r * brightness) / 100;
	g = (g * brightness) / 100;
	b = (b * brightness) / 100;

	r = gamma_table[r];
	g = gamma_table[g];
	b = gamma_table[b];
 //24BIT=GRB顺序且高位先发
	return (g << 16) | (r << 8) | b;
}
