/*************************************
 * @file           : nrf24l01.h
 * @brief          : ic driver
 * @author         : Ouyang Junyuan
 *                   at HITsz WTR
 *************************************
 * @attention      :
 *  configuration 
 *  at the file-end in nrf24l01.c
 *************************************/
 
#ifndef __24L01_H
#define __24L01_H 
 
 
#include "spi.h"
#include  <stdbool.h>
/******************************
 * NRF24L01 spi command table 
 ******************************/
#define	cmd_READ_REG	0x00    
#define cmd_WRITE_REG 0x20    
#define cmd_READ_RX_PAYLOAD 0x61	
#define cmd_WRITE_TX_PAYLOAD 0xA0 
#define cmd_FLUSH_TX 0xE1  
#define cmd_FLUSH_RX 0xE2	 
#define cmd_REUSE_TX_PAYLOAD  0xE3
#define cmd_ACTIVATE 0x50
#define cmd_R_RX_PL_WID 0x60
#define cmd_W_TX_PAYLOAD_NOACK 0x58
#define cmd_W_ACK_PAYLOAD 0xA8
#define cmd_NOP 0xFF  //dummy operation to get status register msgs

/******************************
 * NRF24L01 register map table 
 ******************************/
#define reg_CONFIG 0x00 
#define reg_CONFIG__MASK_RX_DR_Pos   (6U)
#define reg_CONFIG__MASK_RX_DR_Msk   (0x1UL << reg_CONFIG__MASK_RX_DR_Pos) 
#define reg_CONFIG__MASK_RX_DR       reg_CONFIG__MASK_RX_DR_Msk
#define reg_CONFIG__MASK_TX_DS_Pos   (5U)
#define reg_CONFIG__MASK_TX_DS_Msk   (0x1UL << reg_CONFIG__MASK_TX_DS_Pos) 
#define reg_CONFIG__MASK_TX_DS       reg_CONFIG__MASK_TX_DS_Msk
#define reg_CONFIG__MASK_MAX_RT_Pos  (4U)
#define reg_CONFIG__MASK_MAX_RT_Msk  (0x1UL << reg_CONFIG__MASK_MAX_RT_Pos) 
#define reg_CONFIG__MASK_MAX_RT     reg_CONFIG__MASK_MAX_RT_Msk
#define reg_CONFIG__MASK_IRQ_Pos     (4U)
#define reg_CONFIG__MASK_IRQ_Msk     (0x7UL << reg_CONFIG__MASK_IRQ_Pos) 
#define reg_CONFIG__MASK_IRQ         reg_CONFIG__MASK_IRQ_Msk
#define reg_CONFIG__EN_CRC_Pos       (3U)        
#define reg_CONFIG__EN_CRC_Msk       (0x1UL << reg_CONFIG__EN_CRC_Pos) 
#define reg_CONFIG__EN_CRC           reg_CONFIG__EN_CRC_Msk
#define reg_CONFIG__CRCO_Pos         (2U)
#define reg_CONFIG__CRCO_Msk         (0x1UL << reg_CONFIG__CRCO_Pos) 
#define reg_CONFIG__CRCO             reg_CONFIG__CRCO_Msk
#define reg_CONFIG__PWR_UP_Pos       (1U)
#define reg_CONFIG__PWR_UP_Msk       (0x1UL << reg_CONFIG__PWR_UP_Pos) 
#define reg_CONFIG__PWR_UP           reg_CONFIG__PWR_UP_Msk
#define reg_CONFIG__PRIM_RX_Pos      (0U)
#define reg_CONFIG__PRIM_RX_Msk      (0x1UL << reg_CONFIG__PRIM_RX_Pos) 
#define reg_CONFIG__PRIM_RX          reg_CONFIG__PRIM_RX_Msk


#define reg_EN_AA 0x01
#define reg_EN_AA__ENAA_P5_Pos   (5U)
#define reg_EN_AA__ENAA_P4_Pos   (4U)
#define reg_EN_AA__ENAA_P3_Pos   (3U)
#define reg_EN_AA__ENAA_P2_Pos   (2U)
#define reg_EN_AA__ENAA_P1_Pos   (1U)
#define reg_EN_AA__ENAA_P0_Pos   (0U)
#define reg_EN_AA__ENAA_P5       (0x1UL << reg_EN_AA__ENAA_P5_Pos) 
#define reg_EN_AA__ENAA_P4       (0x1UL << reg_EN_AA__ENAA_P4_Pos) 
#define reg_EN_AA__ENAA_P3       (0x1UL << reg_EN-AA__ENAA_P3_Pos) 
#define reg_EN_AA__ENAA_P2       (0x1UL << reg_EN_AA__ENAA_P2_Pos) 
#define reg_EN_AA__ENAA_P1       (0x1UL << reg_EN_AA__ENAA_P1_Pos) 
#define reg_EN_AA__ENAA_P0       (0x1UL << reg_EN_AA__ENAA_P0_Pos) 


#define reg_EN_RXADDR 0x02
#define reg_EN_RXADDR__ERX_P5_Pos   (5U)
#define reg_EN_RXADDR__ERX_P4_Pos   (4U)
#define reg_EN_RXADDR__ERX_P3_Pos   (3U)
#define reg_EN_RXADDR__ERX_P2_Pos   (2U)
#define reg_EN_RXADDR__ERX_P1_Pos   (1U)
#define reg_EN_RXADDR__ERX_P0_Pos   (0U)
#define reg_EN_RXADDR__ERX_P5       (0x1UL << reg_EN_RXADDR__ERX_P5_Pos) 
#define reg_EN_RXADDR__ERX_P4       (0x1UL << reg_EN_RXADDR__ERX_P4_Pos) 
#define reg_EN_RXADDR__ERX_P3       (0x1UL << reg_EN_RXADDR__ERX_P3_Pos) 
#define reg_EN_RXADDR__ERX_P2       (0x1UL << reg_EN_RXADDR__ERX_P2_Pos) 
#define reg_EN_RXADDR__ERX_P1       (0x1UL << reg_EN_RXADDR__ERX_P1_Pos) 
#define reg_EN_RXADDR__ERX_P0       (0x1UL << reg_EN_RXADDR__ERX_P0_Pos) 


#define reg_SETUP_AW  0x03
#define reg_SETUP_AW__AW_Pos       (0U)        
#define reg_SETUP_AW__AW_Msk       (0x3UL << reg_SETUP_AW__AW_Pos) 
#define reg_SETUP_AW__AW           reg_SETUP_AW__AW_Msk
#define reg_SETUP_AW__AW_0         (0x1UL << reg_SETUP_AW__AW_Pos) 
#define reg_SETUP_AW__AW_1         (0x2UL << reg_SETUP_AW__AW_Pos) 


#define	reg_SETUP_RETR 0x04
#define reg_SETUP_RETR__ARD_Pos    (4U)        
#define reg_SETUP_RETR__ARD_Msk    (0x1UL << reg_SETUP_RETR__ARD_Pos)
#define reg_SETUP_RETR__ARD(n)     ( (n)   *    reg_SETUP_RETR__ARD_Msk) 

#define reg_SETUP_RETR__ARC_Pos    (0U)        
#define reg_SETUP_RETR__ARC_Msk    (0x1UL << reg_SETUP_RETR__ARC_Pos)
#define reg_SETUP_RETR__ARC(n)     ( (n)   *    reg_SETUP_RETR__ARC_Msk)     


#define reg_RF_CH		0x05
#define reg_RF_CH__RF_CH_Pos       (0U)      
#define reg_RF_CH__RF_CH_Msk       (0x1UL << reg_RF_CH__RF_CH_Pos)
#define reg_RF_CH__RF_CH(n)        ( (n)   *    reg_RF_CH__RF_CH_Msk)  


#define reg_RF_SETUP 0x06
#define reg_RF_SETUP__RF_DR_Pos   (3U)
#define reg_RF_SETUP__RF_DR_Msk   (0x1UL <<  reg_RF_SETUP__RF_DR_Pos)
#define reg_RF_SETUP__RF_DR       reg_RF_SETUP__RF_DR_Msk
#define reg_RF_SETUP__RF_PWR_Pos  (1U)
#define reg_RF_SETUP__RF_PWR_Msk  (0x3UL <<  reg_RF_SETUP__RF_PWR_Pos)
#define reg_RF_SETUP__RF_PWR      reg_RF_SETUP__RF_PWR_Msk
#define reg_RF_SETUP__RF_PWR_0    (0x1UL <<  reg_RF_SETUP__RF_PWR_Pos)
#define reg_RF_SETUP__RF_PWR_1    (0x2UL <<  reg_RF_SETUP__RF_PWR_Pos)
#define reg_RF_SETUP__LNA_Pos     (0U)
#define reg_RF_SETUP__LNA_Msk     (0x1UL <<  reg_RF_SETUP__LNA_Pos)
#define reg_RF_SETUP__LNA         reg_RF_SETUP__LNA_Msk


#define reg_STATUS	0x07            
#define reg_STATUS__RX_DR_Pos     (6U)
#define reg_STATUS__RX_DR_Msk     (0x1UL <<  reg_STATUS__RX_DR_Pos)
#define reg_STATUS__RX_DR         reg_STATUS__RX_DR_Msk
#define reg_STATUS__TX_DS_Pos     (5U)
#define reg_STATUS__TX_DS_Msk     (0x1UL <<  reg_STATUS__TX_DS_Pos)
#define reg_STATUS__TX_DS         reg_STATUS__TX_DS_Msk
#define reg_STATUS__MAX_RT_Pos    (4U)
#define reg_STATUS__MAX_RT_Msk    (0x1UL <<  reg_STATUS__MAX_RT_Pos)
#define reg_STATUS__MAX_RT        reg_STATUS__MAX_RT_Msk
#define reg_STATUS__RX_P_NO_Pos   (1U)
#define reg_STATUS__RX_P_NO_Msk   (0x7UL <<  reg_STATUS__RX_P_NO_Pos)
#define reg_STATUS__TX_FULL_Pos   (0U)
#define reg_STATUS__TX_FULL_Msk   (0x1UL <<  reg_STATUS__TX_FULL_Pos)
#define reg_STATUS__TX_FULL       reg_STATUS__TX_FULL_Msk


#define reg_OBSERVE_TX 0x08
#define reg_OBSERVE_TX__PLOS_CNT_Pos   (4U)
#define reg_OBSERVE_TX__PLOS_CNT_Msk   (0x1UL <<  reg_OBSERVE_TX__PLOS_CNT_Pos)
#define reg_OBSERVE_TX__PLOS_CNT       reg_OBSERVE_TX__PLOS_CNT_Msk       
#define reg_OBSERVE_TX__ARC_CNT_Pos    (0U)
#define reg_OBSERVE_TX__ARC_CNT_Msk    (0x1UL <<  reg_OBSERVE_TX__ARC_CNT_Pos)
#define reg_OBSERVE_TX__ARC_CNT        reg_OBSERVE_TX__ARC_CNT_Msk  


#define	reg_CD 0x09
#define reg_CD__CD_Pos   (0U)
#define reg_CD__CD_Msk   (0x1UL <<  reg_CD__CD_Pos)
#define reg_CD__CD       reg_CD__CD_Msk


#define reg_RX_ADDR_P0 0x0A
#define reg_RX_ADDR_P1 0x0B
#define reg_RX_ADDR_P2 0x0C
#define reg_RX_ADDR_P3 0x0D
#define reg_RX_ADDR_P4 0x0E
#define reg_RX_ADDR_P5 0x0F
#define reg_RX_ADDR_P0_Msk  0xffffffffffUL
#define reg_RX_ADDR_P1_Msk  0xffffffffffUL
#define reg_RX_ADDR_P2_Msk  0xfU
#define reg_RX_ADDR_P3_Msk  0xfU
#define reg_RX_ADDR_P4_Msk  0xfU
#define reg_RX_ADDR_P5_Msk  0xfU


#define reg_TX_ADDR	0x10	 
#define reg_TX_ADDR_Msk  0xffffffffffUL


#define reg_RX_PW_P0	0x11     
#define	reg_RX_PW_P1	0x12
#define	reg_RX_PW_P2	0x13
#define	reg_RX_PW_P3	0x14
#define	reg_RX_PW_P4	0x15
#define	reg_RX_PW_P5	0x16
#define reg_RX_PW_P0_Msk  (0X3F)
#define reg_RX_PW_P1_Msk  (0X3F)
#define reg_RX_PW_P2_Msk  (0X3F)
#define reg_RX_PW_P3_Msk  (0X3F)
#define reg_RX_PW_P4_Msk  (0X3F)
#define reg_RX_PW_P5_Msk  (0X3F)


#define reg_FIFO_STATUS	0x17
#define reg_FIFO_STATUS__TX_REUSE_Pos     (6U)
#define reg_FIFO_STATUS__TX_REUSE_Msk     (0x1UL  <<    reg_FIFO_STATUS__TX_REUSE_Pos)
#define reg_FIFO_STATUS__TX_REUSE         reg_FIFO_STATUS__TX_REUSE_Msk
#define reg_FIFO_STATUS__TX_FULL_Pos      (5U)
#define reg_FIFO_STATUS__TX_FULL_Msk      (0x1UL  <<    reg_FIFO_STATUS__TX_FULL_Pos)
#define reg_FIFO_STATUS__TX_FULL          reg_FIFO_STATUS__TX_FULL_Msk
#define reg_FIFO_STATUS__TX_EMPTY_Pos     (4U)
#define reg_FIFO_STATUS__TX_EMPTY_Msk     (0x1UL  <<    reg_FIFO_STATUS__TX_EMPTY_Pos)
#define reg_FIFO_STATUS__TX_EMPTY         reg_FIFO_STATUS__TX_EMPTY_Msk
#define reg_FIFO_STATUS__RX_FULL_Pos      (1U)
#define reg_FIFO_STATUS__RX_FULL_Msk      (0x1UL  <<    reg_FIFO_STATUS__RX_FULL_Pos)
#define reg_FIFO_STATUS__RX_FULL          reg_FIFO_STATUS__RX_FULL_Msk
#define reg_FIFO_STATUS__RX_EMPTY_Pos     (0U)
#define reg_FIFO_STATUS__RX_EMPTY_Msk     (0x1UL  <<    reg_FIFO_STATUS__RX_EMPTY_Pos)
#define reg_FIFO_STATUS__RX_EMPTY         reg_FIFO_STATUS__RX_EMPTY_Msk


#define reg_DYNPD   0x1C
#define reg_DYNPD__DPL_P5           (0x1UL<< 5)
#define reg_DYNPD__DPL_P4           (0x1UL<< 4)         
#define reg_DYNPD__DPL_P3           (0x1UL<< 3)
#define reg_DYNPD__DPL_P2           (0x1UL<< 2)
#define reg_DYNPD__DPL_P1           (0x1UL<< 1)
#define reg_DYNPD__DPL_P0           (0x1UL<< 0)


#define reg_FEATURE 0x1D
#define reg_FEATURE__EN_DPL_Pos     (2U)
#define reg_FEATURE__EN_DPL_Msk     (0x1UL << reg_FEATURE__EN_DPL_Pos)
#define reg_FEATURE__EN_DPL         reg_FEATURE__EN_DPL_Msk
#define reg_FEATURE__EN_ACK_PAY_Pos (1U)
#define reg_FEATURE__EN_ACK_PAY_Msk (0x1UL << reg_FEATURE__EN_ACK_PAY_Pos)
#define reg_FEATURE__EN_ACK_PAY     reg_FEATURE__EN_ACK_PAY_Msk
#define reg_FEATURE__EN_DYN_ACK_Pos (0U)
#define reg_FEATURE__EN_DYN_ACK_Msk (0x1UL << reg_FEATURE__EN_DYN_ACK_Pos)
#define reg_FEATURE__EN_DYN_ACK     reg_FEATURE__EN_DYN_ACK_Msk


/******************************
 * Init Value Define
 ******************************/
#define NRF_DISABLE_IRQ_RX  reg_CONFIG__MASK_RX_DR
#define NRF_DISABLE_IRQ_TX  reg_CONFIG__MASK_TX_DS
#define NRF_DISABLE_IRQ_RT  reg_CONFIG__MASK_MAX_RT 
#define NRF_DISABLE_IRQ_ALL reg_CONFIG__MASK_IRQ
 
#define NRF_POWER_UP reg_CONFIG__PWR_UP
#define NRF_POWER_DOWN 0x00
#define NRF_MODE_RX  reg_CONFIG__PRIM_RX
#define NRF_MODE_TX  0x00

#define NRF_DATE_PIPE_ALL  0x3f
#define NRF_DATE_PIPE_NONE 0x00
#define NRF_DATE_PIPE0 reg_EN_RXADDR__ERX_P0
#define NRF_DATE_PIPE1 reg_EN_RXADDR__ERX_P1
#define NRF_DATE_PIPE2 reg_EN_RXADDR__ERX_P2
#define NRF_DATE_PIPE3 reg_EN_RXADDR__ERX_P3
#define NRF_DATE_PIPE4 reg_EN_RXADDR__ERX_P4
#define NRF_DATE_PIPE5 reg_EN_RXADDR__ERX_P5

#define NRF_DATE_ACK_PIPE_ALL  0x3f
#define NRF_DATE_ACK_PIPE_NONE 0x00
#define NRF_DATE_ACK_PIPE0 reg_EN_AA__ENAA_P0
#define NRF_DATE_ACK_PIPE1 reg_EN_AA__ENAA_P1
#define NRF_DATE_ACK_PIPE2 reg_EN_AA__ENAA_P2
#define NRF_DATE_ACK_PIPE3 reg_EN_AA__ENAA_P3
#define NRF_DATE_ACK_PIPE4 reg_EN_AA__ENAA_P4
#define NRF_DATE_ACK_PIPE5 reg_EN_AA__ENAA_P5

#define NRF_ADDRWIDTH_REGBIT_5BYTES   (reg_SETUP_AW__AW_0 | reg_SETUP_AW__AW_1)
#define NRF_ADDRWIDTH_REGBIT_4BYTES   (reg_SETUP_AW__AW_1)
#define NRF_ADDRWIDTH_REGBIT_3BYTES   (reg_SETUP_AW__AW_0)

#define NRF_ADDRWIDTH_5BYTES (NRF_ADDRWIDTH_REGBIT_5BYTES + 2)
#define NRF_ADDRWIDTH_4BYTES (NRF_ADDRWIDTH_REGBIT_4BYTES + 2)
#define NRF_ADDRWIDTH_3BYTES (NRF_ADDRWIDTH_REGBIT_3BYTES + 2)

#define NRF_AutoRetransmit_DelayTime_us(n)  reg_SETUP_RETR__ARD(((uint8_t)(n/250 - 1)))
#define NRF_AutoRetransmit_Max(n)           reg_SETUP_RETR__ARC(((uint8_t)n))

#define NRF_FREQUENCY_CHANNEL(n)            reg_RF_CH__RF_CH(((uint8_t)n))

#define NRF_AIR_DATA_RATE_1MHz  0x00
#define NRF_AIR_DATA_RATE_2MHz  reg_RF_SETUP__RF_DR

#define NRF_RF_PWR_n18dBm    (0x00)
#define NRF_RF_PWR_n12dBm    (reg_RF_SETUP__RF_PWR_0)
#define NRF_RF_PWR_n6dBm     (reg_RF_SETUP__RF_PWR_1) 
#define NRF_RF_PWR_0dBm      (reg_RF_SETUP__RF_PWR_1 | reg_RF_SETUP__RF_PWR_0)

#define NRF_CLOSE_LNA         0x00
#define NRF_SETUP_LNA         reg_RF_SETUP__LNA

#define NRF_ENABLE_DYNAMIC_PLYLOAD_WIDTH   reg_FEATURE__EN_DPL
#define NRF_DISABLE_DYNAMIC_PLYLOAD_WIDTH  0x00

#define NRF_DYNAMIC_PAYLOAD_WIDTH_NONE  0x00
#define NRF_DYNAMIC_PAYLOAD_WIDTH_ALL   0X3F
#define NRF_DYNAMIC_PAYLOAD_WIDTH_P0    reg_DYNPD__DPL_P0
#define NRF_DYNAMIC_PAYLOAD_WIDTH_P1    reg_DYNPD__DPL_P1
#define NRF_DYNAMIC_PAYLOAD_WIDTH_P2    reg_DYNPD__DPL_P2
#define NRF_DYNAMIC_PAYLOAD_WIDTH_P3    reg_DYNPD__DPL_P3
#define NRF_DYNAMIC_PAYLOAD_WIDTH_P4    reg_DYNPD__DPL_P4
#define NRF_DYNAMIC_PAYLOAD_WIDTH_P5    reg_DYNPD__DPL_P5

#define NRF_ENABLE_ACK_PAYLOAD  reg_FEATURE__EN_ACK_PAY
#define NRF_DISABLE_ACK_PAYLOAD 0x00


/******************************
 * Application Macro  
 ******************************/
/* --- physical pin mask define --- */
#define NRF_PIN_CSN 0
#define NRF_PIN_CE 1 
#define NRF_PIN_IRQ 2
/* --- bind physical pin macro --- */
#define NRF_SET_PIN(__HANDLE__,__PIN_MASK__,__PORT__,__PIN__)  __HANDLE__.port[__PIN_MASK__]=__PORT__ ; __HANDLE__.pin[__PIN_MASK__]=__PIN__ 
           
/******************************
 * NRF24L01 Lib Typedef Table  
 ******************************/
/* --- nrf typedef --- */
typedef struct{
  uint8_t CONFIG;
  uint8_t EN_AA;
  uint8_t EN_RXADDR;
  uint8_t SETUP_AW;
  uint8_t SETUP_RETR;
  uint8_t RF_CH;
  uint8_t RF_SETUP;
  uint8_t STATUS;
  uint8_t OBSERVE_TX;
  uint8_t CD;
  uint64_t RX_ADDR_P[6]; 
  uint64_t TX_ADDR;
  uint8_t RX_PW_P[6];
  uint8_t FIFO_STATUS;
  uint8_t DYNPD;
  uint8_t FEATURE;
}NRF_TypeDef;

typedef enum{
  NRF_STATE_RESET         =0X00U,
  NRF_STATE_READY         =0X20U,
  NRF_STATE_BUSY_TX       =0X21U,
  NRF_STATE_BUSY_RX       =0X22U,
  NRF_STATE_BUSY_RX_IRQ   =0X23U,
  NRF_STATE_TIMEOUT       =0XA0U, 
  NRF_STATE_ERROR         =0XE0U
}NRF_StateTypeDef;


typedef struct{
  uint8_t                 Mode;
  uint8_t                 EnablePipe;
 	uint8_t                 AutoAckPipe;
  uint8_t                 RxTxAddrWidth2Regbit;
  uint8_t                 RxTxAddrWidth;
  uint8_t                 AutoReTransmitDelayTime;
  uint8_t                 AutoRetransmitCountMax;  
  uint8_t                 FrequncyChannel;  
  uint8_t                 RfAirDataRate;  
  uint8_t                 RfPower;  
  uint8_t                 UseLNA;  
  uint8_t                 (*RxPipeAddr)[5];  
  uint8_t                 *TxMsgAddr;  
  uint8_t                 *RxPipePayLoadWidth; 
  uint8_t                 TxPayLoadWidth;
  
  uint8_t                 EnableDyanmeicPayLoadWidth;
  uint8_t                 EnableDynamicPayLoadPipe;
  uint8_t                 EnableAckPayLoad;

}NRF_InitTypeDef;
 

typedef struct{
  NRF_TypeDef             Instance;
  NRF_InitTypeDef         Init;
  
	GPIO_TypeDef            *port[3];
	uint16_t                pin[3];
  SPI_HandleTypeDef       *hspix;
  
  uint8_t                 *pRxBuffPtr;
  uint8_t                 *pAckBuffPtr;
  uint8_t                  AckBuffsize;
  
  uint8_t                 pipex;
  uint8_t                 stamp;
  __IO NRF_StateTypeDef   State;
}NRF24L01_HandleTypeDef;








#define NRF_PIN_STATE(__HANDLE__,__PIN_MASK__,__STATE__ ) HAL_GPIO_WritePin((__HANDLE__)->port[__PIN_MASK__],(__HANDLE__)->pin[__PIN_MASK__],__STATE__)
#define NRF_PIN_GET(__HANDLE__,__PIN_MASK__) HAL_GPIO_ReadPin((__HANDLE__)->port[__PIN_MASK__],(__HANDLE__)->pin[__PIN_MASK__])


#define NRF_CE_0(__HANDLE__) NRF_PIN_STATE(__HANDLE__,NRF_PIN_CE,GPIO_PIN_RESET)
#define NRF_ENTER_STANDBY(__HANDLE__)  NRF_CE_0(__HANDLE__)

#define NRF_CE_1(__HANDLE__) NRF_PIN_STATE(__HANDLE__,NRF_PIN_CE,GPIO_PIN_SET)
#define NRF_EXIT_STANDBY(__HANDLE__)   NRF_CE_1(__HANDLE__)

#define NRF_CSN_0(__HANDLE__) NRF_PIN_STATE(__HANDLE__,NRF_PIN_CSN,GPIO_PIN_RESET)
#define NRF_SPI_START_SIGNAL(__HANDLE__)  NRF_CSN_0(__HANDLE__)

#define NRF_CSN_1(__HANDLE__) NRF_PIN_STATE(__HANDLE__,NRF_PIN_CSN,GPIO_PIN_SET)
#define NRF_SPI_END_SIGNAL(__HANDLE__)   NRF_CSN_1(__HANDLE__)

#define NRF_IRQ_READ(__HANDLE__)  NRF_PIN_GET(__HANDLE__,NRF_PIN_IRQ)

 




/******************************
 * declare func here
 ******************************/


void NRF24L01_Init(void);

HAL_StatusTypeDef NRF24L01_Check(NRF24L01_HandleTypeDef *hnrf);
void NRF24L01_Get_Instance(NRF24L01_HandleTypeDef *hnrf);

void NRF24L01_Switch2_Tx(NRF24L01_HandleTypeDef *hnrf);
void NRF24L01_Switch2_Rx(NRF24L01_HandleTypeDef *hnrf);

HAL_StatusTypeDef NRF24L01_Recieve_IT(NRF24L01_HandleTypeDef *hnrf,uint8_t *tbuf,bool payload);
HAL_StatusTypeDef NRF24L01_Recieve(NRF24L01_HandleTypeDef *hnrf,uint8_t *tbuf,uint32_t timeout);
HAL_StatusTypeDef NRF24L01_Transmit(NRF24L01_HandleTypeDef *hnrf,uint8_t *tbuf,uint32_t timeout);

/* --- irq func --- */
HAL_StatusTypeDef NRF_IRQHandler(NRF24L01_HandleTypeDef *hnrf); //called by ext interrupt ISR  

void NRF_ReceiveACKCallback(NRF24L01_HandleTypeDef *hnrf);
void NRF_RxCpltCallback(NRF24L01_HandleTypeDef *hnrf,uint8_t pipex);
void NRF_RxPayloadAckCpltCallback(NRF24L01_HandleTypeDef *hnrf,uint8_t pipex);

/******************************
 * extern gobal varable 
 ******************************/

extern NRF24L01_HandleTypeDef hnrf24l01;
#endif
