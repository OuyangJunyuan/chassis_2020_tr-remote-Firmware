#ifndef AT24C256_H_OUYJY

#include "i2c.h"
#include <stdlib.h>
#include "string.h"



#define STORAGE_I2C hi2c1
#define MAX_NUM_STORAGE_ONE 20
#define MEMORY_SIZE (256*1024/8)

typedef struct
{
		__IO  void *data;
    int size;
		char *_name;
    unsigned char *data_c;
    unsigned short int addr;
}STORAGE_ONE;

typedef struct
{
	I2C_HandleTypeDef *hi2c;
	uint16_t ptr;
	STORAGE_ONE set[MAX_NUM_STORAGE_ONE];
	uint16_t num;
}STORAGE_HandleTypeDef;

#define MASTER storage
STORAGE_HandleTypeDef MASTER={.hi2c=&STORAGE_I2C,.ptr =0, .num=0};



#define ADDR_AT24C02_Write 0xA0
#define ADDR_AT24C02_Read 0xA1





#define AT24Cx_REGISTER(name)  do{\
																		MASTER.set[MASTER.num].data=&name;\
																		MASTER.set[MASTER.num].size=sizeof(name);\
																		MASTER.set[MASTER.num]._name=(char *)malloc(sizeof(#name));\
																		strcpy((char *)MASTER.set[MASTER.num]._name,#name);\
																		MASTER.set[MASTER.num].data_c=(unsigned char *)malloc(MASTER.set[MASTER.num].size);\
																		essert_addr(MASTER.ptr);\
																		MASTER.set[MASTER.num].addr=MASTER.ptr;\
																		MASTER.ptr+=MASTER.set[MASTER.num].size;\
																		MASTER.num++;\
																 }while(0) 	


#define AT24Cx_STORAGE(name)	 do{	int idx=-1;\
																		for(int i=0;i<MASTER.num;i++)\
																			if(strcmp(MASTER.set[i]._name,#name)==0) idx=i;\
																		if(idx!=-1){\
																			for(int i=0;i<MASTER.set[idx].size;++i)\
																				MASTER.set[idx].data_c[i]= ((uint8_t *)MASTER.set[idx].data)[i];\
																			HAL_I2C_Mem_Write(MASTER.hi2c, ADDR_AT24C02_Write, MASTER.set[idx].addr, I2C_MEMADD_SIZE_16BIT,MASTER.set[idx].data_c,MASTER.set[idx].size, 1000);\
																			HAL_Delay(5);\
																		}\
																}while(0) 

#define AT24Cx_RECOVER(name)	do{	int idx=-1;\
																		for(int i=0;i<MASTER.num;i++)\
																			if(strcmp(MASTER.set[i]._name,#name)==0) idx=i;\
																		if(idx!=-1){\
																			HAL_I2C_Mem_Read(MASTER.hi2c, ADDR_AT24C02_Read,MASTER.set[idx].addr, I2C_MEMADD_SIZE_16BIT,MASTER.set[idx].data_c,MASTER.set[idx].size, 1000);\
																			name=*(typeof(name)*)MASTER.set[idx].data_c;\
																		}\
																}while(0) 

	
		

#define essert_addr(ptr) {if(ptr>=MEMORY_SIZE)\
													do{;}while(1);}

#endif
							


		//
													
													