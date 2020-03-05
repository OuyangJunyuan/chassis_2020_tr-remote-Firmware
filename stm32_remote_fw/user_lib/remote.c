#include "remote.h"



#define M1 523
#define M2 587
#define M3 659
#define M4 698
#define M5 784
#define M6 880
#define M7 988
#define H1 1046
#define H2 1175
#define H3 1318
#define H4 1397
#define H5 1568
#define H6 1760
#define H7 1967
#define H8 2100
#define H9 2300


REMOTE_HandleTypeDef hremote={.Init=Initilaize};

static int spectrum[16]={M1,M2,M3,M4,M5,M6,M7,H1,H2,H3,H4,H5,H6,H7,H8,H9};
static int temp[4][10];
static uint16_t adc_count;
void Initilaize(void)
{
	hremote.fresh=DataFresh;
	
	//tim;
	hremote.buzzer.tim.htim=&htim4;
	hremote.buzzer.tim.channel=TIM_CHANNEL_3;
	hremote.buzzer.start=buzzer_start;
	hremote.buzzer.stop=buzzer_stop;
	hremote.buzzer.cry=buzeer_cry;
	hremote.buzzer.spectrum=spectrum;
	//keyboard;
	hremote.keyboard.gpio[0].PORT=GPIOA;
	hremote.keyboard.gpio[0].PIN=GPIO_PIN_15;
	
	hremote.keyboard.gpio[1].PORT=GPIOB;
	hremote.keyboard.gpio[1].PIN=GPIO_PIN_3;
	
	hremote.keyboard.gpio[2].PORT=GPIOB;
	hremote.keyboard.gpio[2].PIN=GPIO_PIN_4;
	
	hremote.keyboard.gpio[3].PORT=GPIOB;
	hremote.keyboard.gpio[3].PIN=GPIO_PIN_5;
	
	hremote.keyboard.gpio[4].PORT=GPIOA;
	hremote.keyboard.gpio[4].PIN=GPIO_PIN_11;
	//joystick_key
	hremote.joystick.adc=&hadc1;
	
	hremote.joystick.gpio[0].PORT=GPIOA;
	hremote.joystick.gpio[0].PIN=GPIO_PIN_6;
	hremote.joystick.gpio[1].PORT=GPIOC;
	hremote.joystick.gpio[1].PIN=GPIO_PIN_15;
}

void getKeyboard(void)
{
	hremote.keyboard.key[0]=HAL_GPIO_ReadPin(hremote.keyboard.gpio[0].PORT,hremote.keyboard.gpio[0].PIN)^0x01;
	hremote.keyboard.key[1]=(HAL_GPIO_ReadPin(hremote.keyboard.gpio[1].PORT,hremote.keyboard.gpio[1].PIN)^0x01)<<1;
	hremote.keyboard.key[2]=(HAL_GPIO_ReadPin(hremote.keyboard.gpio[2].PORT,hremote.keyboard.gpio[2].PIN)^0x01)<<2;
	hremote.keyboard.key[3]=(HAL_GPIO_ReadPin(hremote.keyboard.gpio[3].PORT,hremote.keyboard.gpio[3].PIN)^0x01)<<3;
	hremote.keyboard.key[4]=HAL_GPIO_ReadPin(hremote.keyboard.gpio[4].PORT,hremote.keyboard.gpio[4].PIN)^0x01;

	hremote.data[KEY_MASK]=(hremote.keyboard.key[0]|
													hremote.keyboard.key[1]|
													hremote.keyboard.key[2]|
													hremote.keyboard.key[3])|
												 (hremote.keyboard.key[4]?0x00:0xff);
}

void getJoyStick(void)
{
	hremote.data[JOYSITCK_L_KEY_MASK]=HAL_GPIO_ReadPin(hremote.joystick.gpio[0].PORT,hremote.joystick.gpio[0].PIN)^0x01;
	hremote.data[JOYSITCK_R_KEY_MASK]=HAL_GPIO_ReadPin(hremote.joystick.gpio[1].PORT,hremote.joystick.gpio[1].PIN)^0x01;

	for(int i=0;i<4;++i)
	{
		HAL_ADC_Start(hremote.joystick.adc);
		HAL_ADC_PollForConversion(hremote.joystick.adc,0xffff);
		hremote.joystick.joystick_adc[i]=HAL_ADC_GetValue(hremote.joystick.adc);
		temp[i][adc_count]=hremote.joystick.joystick_adc[i];
	}
	adc_count++;
	HAL_ADC_Stop(hremote.joystick.adc);
	
	if(adc_count>9){
		for(int i=0;i<4;++i){
			for(int j=1;j<10;++j)
			{
				temp[i][0]+=temp[i][j];
			}	
		}		
		adc_count=0;
		hremote.data[JOYSTICK_LX_MASK] = -(temp[3][0]/10-1024)/100  ;
		hremote.data[JOYSTICK_LY_MASK]= (temp[2][0]/10-1024)/100;
		hremote.data[JOYSTICK_RX_MASK]= -(temp[1][0]/10-1024)/100;	
		hremote.data[JOYSTICK_RY_MASK]= (temp[0][0]/10-1024)/100 ;
		if(abs((signed char)hremote.data[JOYSTICK_LX_MASK])<2)
			hremote.data[JOYSTICK_LX_MASK]=0;
		if(abs((signed char)hremote.data[JOYSTICK_LY_MASK])<2)
			hremote.data[JOYSTICK_LY_MASK]=0;
		if(abs((signed char)hremote.data[JOYSTICK_RX_MASK])<2)
			hremote.data[JOYSTICK_RX_MASK]=0;
		if(abs((signed char)hremote.data[JOYSTICK_RY_MASK])<2)
			hremote.data[JOYSTICK_RY_MASK]=0;
	}
}

void buzzer_start(void)
{
	HAL_TIM_PWM_Start(hremote.buzzer.tim.htim,hremote.buzzer.tim.channel);
	__HAL_TIM_SET_COMPARE(hremote.buzzer.tim.htim,hremote.buzzer.tim.channel,0);
}
void buzzer_stop(void)
{
	HAL_TIM_PWM_Stop(hremote.buzzer.tim.htim,hremote.buzzer.tim.channel);
}

void buzeer_cry(int _freq)
{
 int freq=2 * 1000000 / _freq -1;
		__HAL_TIM_SET_AUTORELOAD(hremote.buzzer.tim.htim,freq);
	  __HAL_TIM_SET_COMPARE(hremote.buzzer.tim.htim,hremote.buzzer.tim.channel,freq/2);
}


void DataFresh(void)
{
	getKeyboard();
	getJoyStick();
}





