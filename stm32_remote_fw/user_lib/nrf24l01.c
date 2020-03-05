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

#include "nrf24l01.h"

NRF24L01_HandleTypeDef hnrf24l01;

/*************************************
 * private func ：spi interface optor
 *************************************/
uint8_t gobal=0;
uint8_t gobal1=0;
static uint8_t SPI_ReadWriteByte(NRF24L01_HandleTypeDef *hnrf,uint8_t byte){
	uint8_t read=0,send=byte;
	if(HAL_SPI_TransmitReceive(hnrf->hspix,&send,&read,1,0xff)!=HAL_OK)
		read=0;
	return read;
}

static uint8_t nrf24l01_write_reg(NRF24L01_HandleTypeDef *hnrf,uint8_t nrf24l01_reg,uint8_t value){
	uint8_t status=0;
	NRF_SPI_START_SIGNAL(hnrf);

	status =SPI_ReadWriteByte(hnrf,nrf24l01_reg);
	SPI_ReadWriteByte(hnrf,value);

  NRF_SPI_END_SIGNAL(hnrf);
	return status;
}

static uint8_t nrf24l01_read_reg(NRF24L01_HandleTypeDef *hnrf,uint8_t nrf24l01_reg,uint8_t *value){
	uint8_t	status=0;
	NRF_SPI_START_SIGNAL(hnrf);

	status = SPI_ReadWriteByte(hnrf,nrf24l01_reg);
	*value=SPI_ReadWriteByte(hnrf,0XFF);

  NRF_SPI_END_SIGNAL(hnrf);
	return status;
}


static uint8_t nrf24l01_read_buf(NRF24L01_HandleTypeDef *hnrf,uint8_t nrf24l01_reg,uint8_t *pbuf,uint8_t size){
	uint8_t status=0;
	NRF_SPI_START_SIGNAL(hnrf);

  status=SPI_ReadWriteByte(hnrf,nrf24l01_reg);
  for(int i=0; i<size;++i)
    pbuf[i]=SPI_ReadWriteByte(hnrf,0xff);

	NRF_SPI_END_SIGNAL(hnrf);
	return status;
}


static uint8_t nrf24l01_write_buf(NRF24L01_HandleTypeDef *hnrf,uint8_t nrf24l01_reg,uint8_t *pbuf,uint8_t size){
	uint8_t status=0;
	NRF_SPI_START_SIGNAL(hnrf);

  status=SPI_ReadWriteByte(hnrf,nrf24l01_reg);
  for(int i=0;i<size;++i)
    SPI_ReadWriteByte(hnrf,pbuf[i]);

	NRF_SPI_END_SIGNAL(hnrf);
	return status;
}



/*************************************
 * public func ：support function
 *************************************/
HAL_StatusTypeDef NRF24L01_Check(NRF24L01_HandleTypeDef *hnrf){
	uint8_t i;
  uint8_t checkvalue[hnrf->Init.RxTxAddrWidth];
  NRF_ENTER_STANDBY(hnrf);
	nrf24l01_write_buf(hnrf,cmd_WRITE_REG | reg_TX_ADDR, hnrf->Init.TxMsgAddr,hnrf->Init.RxTxAddrWidth);
	nrf24l01_read_buf(hnrf,cmd_READ_REG + reg_TX_ADDR,checkvalue,hnrf->Init.RxTxAddrWidth);
	for(i=0;i<hnrf->Init.RxTxAddrWidth;++i)
	{
		if(checkvalue[i]!=hnrf->Init.TxMsgAddr[i])
			return HAL_ERROR;
	}
	return HAL_OK;
}

void NRF24L01_Get_Instance(NRF24L01_HandleTypeDef *hnrf){
 // NRF_EXIT_STANDBY(hnrf);
  nrf24l01_read_reg(hnrf,cmd_READ_REG | reg_CONFIG,       &(hnrf->Instance.CONFIG));
  nrf24l01_read_reg(hnrf,cmd_READ_REG | reg_EN_AA ,       &(hnrf->Instance.EN_AA ));
  nrf24l01_read_reg(hnrf,cmd_READ_REG | reg_EN_RXADDR,    &(hnrf->Instance.EN_RXADDR));
  nrf24l01_read_reg(hnrf,cmd_READ_REG | reg_SETUP_AW ,    &(hnrf->Instance.SETUP_AW));
  nrf24l01_read_reg(hnrf,cmd_READ_REG | reg_SETUP_RETR,   &(hnrf->Instance.SETUP_RETR));
  nrf24l01_read_reg(hnrf,cmd_READ_REG | reg_RF_CH,&(hnrf->Instance.RF_CH));
  nrf24l01_read_reg(hnrf,cmd_READ_REG | reg_RF_SETUP,&(hnrf->Instance.RF_SETUP));
  nrf24l01_read_reg(hnrf,cmd_READ_REG | reg_STATUS,&(hnrf->Instance.STATUS));
  nrf24l01_read_reg(hnrf,cmd_READ_REG | reg_OBSERVE_TX,&(hnrf->Instance.OBSERVE_TX));
  nrf24l01_read_reg(hnrf,cmd_READ_REG | reg_CD,&(hnrf->Instance.CD));
  for(int i=0;i<6;++i)
  {
    nrf24l01_read_buf(hnrf,cmd_READ_REG | reg_RX_ADDR_P0+i,(uint8_t *)&(hnrf->Instance.RX_ADDR_P[i]),hnrf->Init.RxTxAddrWidth);
    nrf24l01_read_reg(hnrf,cmd_READ_REG | reg_RX_PW_P0+i,&(hnrf->Instance.RX_PW_P[i]));
  }
  nrf24l01_read_buf(hnrf,cmd_READ_REG | reg_TX_ADDR,(uint8_t *)&(hnrf->Instance.TX_ADDR),hnrf->Init.RxTxAddrWidth);
  
  nrf24l01_read_reg(hnrf,cmd_READ_REG | reg_FIFO_STATUS,&(hnrf->Instance.FIFO_STATUS));
  nrf24l01_read_reg(hnrf,cmd_READ_REG | reg_DYNPD,&(hnrf->Instance.DYNPD));
  nrf24l01_read_reg(hnrf,cmd_READ_REG | reg_FEATURE,&(hnrf->Instance.FEATURE));
  
//  nrf24l01_write_reg(hnrf,cmd_FLUSH_TX,hnrf->State);
//  nrf24l01_write_reg(hnrf,cmd_FLUSH_RX,hnrf->State);
}
/*************************************
 * public func ：user function 
 *************************************/
/* ------- for user to call -------- */
void NRF24L01_Switch2_Rx(NRF24L01_HandleTypeDef *hnrf){
	NRF_ENTER_STANDBY(hnrf);
  hnrf->Init.Mode = NRF_MODE_RX;
  
  
  uint8_t bitmask=0x01;
  
  for(int i=0;i<6;++i,bitmask<<=1)
  {
    if(hnrf->Init.EnablePipe & bitmask)
    {  
      if(bitmask>0x02)
      {
        nrf24l01_write_reg(hnrf,cmd_WRITE_REG | (reg_RX_PW_P0+i)    ,hnrf->Init.RxPipePayLoadWidth[i]);
        nrf24l01_write_buf(hnrf,cmd_WRITE_REG | (reg_RX_ADDR_P0+i)  ,hnrf->Init.RxPipeAddr[i],1);
      }
      else{
        nrf24l01_write_reg(hnrf,cmd_WRITE_REG | (reg_RX_PW_P0+i)    ,hnrf->Init.RxPipePayLoadWidth[i]);
        nrf24l01_write_buf(hnrf,cmd_WRITE_REG | (reg_RX_ADDR_P0+i)  ,hnrf->Init.RxPipeAddr[i],hnrf->Init.RxTxAddrWidth);          
      }
    }
  }
  nrf24l01_write_reg(hnrf,cmd_WRITE_REG | reg_CONFIG    , hnrf->Init.Mode | NRF_POWER_UP | reg_CONFIG__CRCO | reg_CONFIG__EN_CRC );
}



void NRF24L01_Switch2_Tx(NRF24L01_HandleTypeDef *hnrf){
	NRF_ENTER_STANDBY(hnrf);
  hnrf->Init.Mode = NRF_MODE_TX;

  nrf24l01_write_reg(hnrf,cmd_WRITE_REG | reg_SETUP_RETR  ,hnrf->Init.AutoRetransmitCountMax | hnrf->Init.AutoReTransmitDelayTime);

  nrf24l01_write_buf(hnrf,cmd_WRITE_REG | reg_TX_ADDR     ,hnrf->Init.TxMsgAddr,hnrf->Init.RxTxAddrWidth);
  nrf24l01_write_reg(hnrf,cmd_WRITE_REG | reg_RX_PW_P0    ,hnrf->Init.RxPipePayLoadWidth[0]);
  nrf24l01_write_buf(hnrf,cmd_WRITE_REG | reg_RX_ADDR_P0  ,hnrf->Init.TxMsgAddr,hnrf->Init.RxTxAddrWidth);          
  
  nrf24l01_write_reg(hnrf,cmd_WRITE_REG | reg_CONFIG      , hnrf->Init.Mode | NRF_POWER_UP | reg_CONFIG__CRCO | reg_CONFIG__EN_CRC);
}





HAL_StatusTypeDef NRF24L01_Transmit(NRF24L01_HandleTypeDef *hnrf,uint8_t *tbuf,uint32_t timeout){
	if(hnrf->State == NRF_STATE_READY)
  {
    uint8_t status=0;
    uint32_t tickstart = HAL_GetTick();
    hnrf->State =NRF_STATE_BUSY_TX;
    nrf24l01_write_buf(hnrf,cmd_WRITE_TX_PAYLOAD,tbuf,hnrf->Init.TxPayLoadWidth);
    
    /* --- begin --- */
    NRF_EXIT_STANDBY(hnrf);
    do{
      nrf24l01_read_reg(hnrf,cmd_READ_REG  | reg_STATUS,&status);
      if(  (HAL_GetTick() - tickstart) > timeout)
      { 
        NRF_ENTER_STANDBY(hnrf);
        hnrf->State = NRF_STATE_READY;
        return HAL_TIMEOUT;
      }
    }while( ( !(status & reg_STATUS__TX_DS) )&& ( !(status & reg_STATUS__MAX_RT) ));
    NRF_ENTER_STANDBY(hnrf);
    
    /* --- process status flag --- */
    nrf24l01_read_reg(hnrf ,cmd_READ_REG  | reg_STATUS,&status);  
    nrf24l01_write_reg(hnrf,cmd_WRITE_REG | reg_STATUS, status);  /* -- clear iqrbit -- */
    
    if(status&reg_STATUS__MAX_RT){ 
      /* -- ↓ clear TX-FIFO -- */
      nrf24l01_write_reg(hnrf,cmd_FLUSH_TX,status);
      /* -- ↑ clear TX-FIFO -- */
      hnrf->State = NRF_STATE_READY;
      return HAL_ERROR;
    }
    
    /* --- transmit succeed --- */
    if(status & reg_STATUS__TX_DS )
    {
      /* --- receive ACK payload --- */
      if(status & reg_STATUS__RX_DR){
        if(hnrf->AckBuffsize !=0 && hnrf->pAckBuffPtr!=NULL )
        nrf24l01_read_buf(hnrf,cmd_READ_RX_PAYLOAD,hnrf->pAckBuffPtr,hnrf->AckBuffsize);
        nrf24l01_write_reg(hnrf,cmd_FLUSH_RX,hnrf->State);/* --- maybe --- */     
        NRF_ReceiveACKCallback(hnrf);
      }
      /* -- ↓ get auto ack :do something -- */
      HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
      ++(hnrf->stamp);
      /* -- ↑ get auto ack :do something -- */
    }
    hnrf->State = NRF_STATE_READY;
    return HAL_OK;
  }
  else{
    return HAL_BUSY;
  } 
}

HAL_StatusTypeDef NRF24L01_Recieve(NRF24L01_HandleTypeDef *hnrf,uint8_t *rbuf,uint32_t timeout){
	if(hnrf->State == NRF_STATE_READY)
  {
    uint8_t status=0;//必须初始化，好几次因为没初始化状态错了找半天，导致出现0x4e这种不可能出现的状态，不是全局static不会自动初始化为0的。
    uint32_t tickstart = HAL_GetTick();
  
    hnrf->State =NRF_STATE_BUSY_RX; 

    NRF_EXIT_STANDBY(hnrf);/* --- start RX mode --- */
    do{
      nrf24l01_read_reg(hnrf,cmd_READ_REG  | reg_STATUS,&status);
      if(  (HAL_GetTick() - tickstart) > timeout)
      { 
        NRF_ENTER_STANDBY(hnrf);
        hnrf->State = NRF_STATE_READY;
        return HAL_TIMEOUT;
      }
    }while( !(status & reg_STATUS__RX_DR) );
    NRF_ENTER_STANDBY(hnrf);
    
    nrf24l01_write_reg(hnrf,cmd_WRITE_REG | reg_STATUS, status);
    
    uint8_t pipe = ((status & reg_STATUS__RX_P_NO_Msk) >> reg_STATUS__RX_P_NO_Pos )&0x7;
    if(pipe < 0x06 )
      nrf24l01_read_buf(hnrf,cmd_READ_RX_PAYLOAD,rbuf,hnrf->Init.RxPipePayLoadWidth[pipe]);
    
    /* --- process --- */
    
    
    
    hnrf->State = NRF_STATE_READY;
    return HAL_OK; 
  }
  else
  {
    return HAL_BUSY;
  }  
}










HAL_StatusTypeDef NRF24L01_Recieve_IT(NRF24L01_HandleTypeDef *hnrf,uint8_t *tbuf,bool payload){
	if(hnrf->State == NRF_STATE_READY)
  {
    hnrf->State = NRF_STATE_BUSY_RX_IRQ ;
    hnrf->pRxBuffPtr = tbuf;

    /* --- configur config register --- */
    //nrf24l01_write_reg(hnrf,cmd_WRITE_REG | reg_CONFIG, hnrf->Init.Mode | NRF_POWER_UP | reg_CONFIG__CRCO | reg_CONFIG__EN_CRC );
    
    /* --- is enable auto ACK with payload --- */
    if(payload==true && hnrf->Init.EnableAckPayLoad == NRF_ENABLE_ACK_PAYLOAD ){ 
      if(hnrf->pAckBuffPtr !=NULL || hnrf->AckBuffsize !=0 ){  
        //nrf24l01_write_reg(hnrf,cmd_FLUSH_TX,hnrf->State);//maybe need to open:  
        nrf24l01_write_buf(hnrf,cmd_W_ACK_PAYLOAD | 0,hnrf->pAckBuffPtr,hnrf->AckBuffsize);
      }
    }
    /* --- enter RX mode --- */
    NRF_EXIT_STANDBY(hnrf);
    return HAL_OK;
  }
  else{
    return HAL_BUSY;
  }
}
/*************************************
 * public func ：irq function
 *************************************/
/* ------- weak ISR function -------- */
__weak void NRF_ReceiveACKCallback(NRF24L01_HandleTypeDef *hnrf){return;}
__weak void NRF_RxCpltCallback(NRF24L01_HandleTypeDef *hnrf,uint8_t pipex){return;}
__weak void NRF_RxPayloadAckCpltCallback(NRF24L01_HandleTypeDef *hnrf,uint8_t pipex){return;}
/* ------- intermediate function -------- */
HAL_StatusTypeDef NRF_IRQHandler(NRF24L01_HandleTypeDef *hnrf){
  if( hnrf->State == NRF_STATE_BUSY_RX_IRQ )
  {
    /* --- exit RX mode --- */
    NRF_ENTER_STANDBY(hnrf);
     
    /* --- read data at edg  mat couse such status==0x4e  --- */
    //maybe need to open: for(int i=0;i<30000;++i);/* --- wait to stable --- */
    uint8_t status=0;
    
    /* --- read status and clear bit immediately --- */
    nrf24l01_read_reg(hnrf,cmd_READ_REG  | reg_STATUS,&status);
    nrf24l01_write_reg(hnrf,cmd_WRITE_REG | reg_STATUS, status);

    uint8_t pipex=((status & reg_STATUS__RX_P_NO_Msk) >> reg_STATUS__RX_P_NO_Pos ) & 0x7 ;
    hnrf->pipex=pipex;
    /* -- unstable data occur(too less time to update data in register)-- */
    if(pipex>5){
      hnrf->State = NRF_STATE_READY;
      return HAL_ERROR;
    }
      
    /* --- these code has moved to receive_it --- */
    /* ****************************************** */
    /* ********* code to ack payload ************ */
    /* ****************************************** */
    
    /* --- if RX_DR IRQ occur  --- */
    if( status & reg_STATUS__RX_DR )
    { 
      /* --- if auto ACK with payload occur  --- */
      if(status  & reg_STATUS__TX_DS)
      {
        NRF_RxPayloadAckCpltCallback(hnrf,hnrf->pipex);
      }
      nrf24l01_read_buf(hnrf,cmd_READ_RX_PAYLOAD,hnrf->pRxBuffPtr,hnrf->Init.RxPipePayLoadWidth[hnrf->pipex]);
      NRF_RxCpltCallback(hnrf,hnrf->pipex);
      hnrf->State = NRF_STATE_READY;
    }
    
    /* --- ready to get next package  --- */
    //be a to do list
    return HAL_OK;
  }
  else
  {
    return HAL_ERROR;
  }
}












/*************************************
 * private func ：configure
 *************************************/
static HAL_StatusTypeDef NRF24L01_Config(NRF24L01_HandleTypeDef *hnrf,NRF_InitTypeDef *NRF_InitStruct){
  /*--- initlize register---*/
  hnrf->State = NRF_STATE_RESET;
  hnrf->Init = *NRF_InitStruct;
  
  if(NRF24L01_Check(hnrf) !=HAL_OK)
  {
    return HAL_ERROR;
  }
  NRF_ENTER_STANDBY(hnrf); /* --- set CEpin low --- */
  
  nrf24l01_write_reg(hnrf,cmd_WRITE_REG | reg_EN_AA       ,NRF_InitStruct->AutoAckPipe);
  nrf24l01_write_reg(hnrf,cmd_WRITE_REG | reg_EN_RXADDR   ,NRF_InitStruct->EnablePipe);
  nrf24l01_write_reg(hnrf,cmd_WRITE_REG | reg_RF_CH       ,NRF_InitStruct->FrequncyChannel);
  nrf24l01_write_reg(hnrf,cmd_WRITE_REG | reg_RF_SETUP    ,NRF_InitStruct->RfPower | NRF_InitStruct->RfAirDataRate | NRF_InitStruct->UseLNA);
  
  NRF_EXIT_STANDBY(hnrf);
  
  nrf24l01_write_reg(hnrf,cmd_WRITE_REG | reg_SETUP_AW    ,NRF_InitStruct->RxTxAddrWidth2Regbit);

  if(NRF_InitStruct->EnableAckPayLoad == NRF_ENABLE_ACK_PAYLOAD)
    nrf24l01_write_reg(hnrf,cmd_ACTIVATE, 0x73); /* --- enable the command --- */
  nrf24l01_write_reg(hnrf,cmd_WRITE_REG | reg_FEATURE    ,NRF_InitStruct->EnableAckPayLoad | NRF_InitStruct->EnableDyanmeicPayLoadWidth);
  nrf24l01_write_reg(hnrf,cmd_WRITE_REG | reg_DYNPD      ,NRF_InitStruct->EnableDynamicPayLoadPipe);
 
  if(NRF_InitStruct->Mode == NRF_MODE_RX)
  {
    NRF24L01_Switch2_Rx(hnrf);
  }else if (NRF_InitStruct->Mode == NRF_MODE_TX)
  {
    NRF24L01_Switch2_Tx(hnrf);
  }
  nrf24l01_write_reg(hnrf,cmd_FLUSH_TX,hnrf->State);
  nrf24l01_write_reg(hnrf,cmd_FLUSH_RX,hnrf->State);
  
  /*--- initlize structs ---*/
  hnrf->pAckBuffPtr = NULL;
  hnrf->pRxBuffPtr  = NULL;
  hnrf->AckBuffsize = 0;
  hnrf->pipex       = 0x07;/* -- no used --- */
  hnrf->State       = NRF_STATE_READY;
    
  /*--- clear status register ---*/
  nrf24l01_write_reg(hnrf,cmd_WRITE_REG|reg_STATUS,0x70);
  
  /*--- exam from register ---*/
  NRF24L01_Get_Instance(hnrf);
  
  return HAL_OK;  
}








/*************************************
 * @configuration : for user
 *************************************
 * please configure there ↓↓↓
 * please configure there ↓↓↓
 * please configure there ↓↓↓
 *************************************/
static uint8_t pipeadress[6][5]={{0x12,0x34,0x56,0x78,0x9a},
/* ↓↓↓↓ */                       {0x1a,0xC2,0xC2,0xC2,0xC2},
                                 {0x1b,0x00,0x00,0x00,0x00},
/* this four should have same */ {0x1c,0x00,0x00,0x00,0x00},
/* four latter one */            {0x1d,0x00,0x00,0x00,0x00},
                                 {0x1e,0x00,0x00,0x00,0x00}};
/* ↑↑↑ */   
static uint8_t txmsgadress[5] = {0x12,0x34,0x56,0x78,0x9a};
static uint8_t pipepayloadwidth[6] = {7,7,7,7,7,7};

void NRF24L01_Init(void){
  hnrf24l01.hspix = &hspi2;
  
  NRF_SET_PIN(hnrf24l01,NRF_PIN_CE ,GPIOB,GPIO_PIN_11);
  NRF_SET_PIN(hnrf24l01,NRF_PIN_CSN,GPIOB,GPIO_PIN_12);
  NRF_SET_PIN(hnrf24l01,NRF_PIN_IRQ,GPIOB,GPIO_PIN_10);
  
  NRF_InitTypeDef NRF_InitStruct={0};
  NRF_InitStruct.AutoReTransmitDelayTime    = NRF_AutoRetransmit_DelayTime_us(250*4);
  NRF_InitStruct.AutoRetransmitCountMax     = NRF_AutoRetransmit_Max(15);
  NRF_InitStruct.FrequncyChannel            = NRF_FREQUENCY_CHANNEL(50);
  NRF_InitStruct.RfAirDataRate              = NRF_AIR_DATA_RATE_2MHz;
  NRF_InitStruct.RfPower                    = NRF_RF_PWR_0dBm;
  NRF_InitStruct.UseLNA                     = NRF_SETUP_LNA;
  
  NRF_InitStruct.Mode                       = NRF_MODE_TX ;
  NRF_InitStruct.EnablePipe                 = NRF_DATE_PIPE_ALL ;
  NRF_InitStruct.AutoAckPipe                = NRF_DATE_ACK_PIPE_ALL;
  NRF_InitStruct.RxTxAddrWidth2Regbit       = NRF_ADDRWIDTH_REGBIT_5BYTES;
  NRF_InitStruct.RxTxAddrWidth              = NRF_ADDRWIDTH_5BYTES;
  NRF_InitStruct.RxPipeAddr                 = pipeadress;
  NRF_InitStruct.TxMsgAddr                  = (uint8_t*)txmsgadress;
  NRF_InitStruct.RxPipePayLoadWidth         = (uint8_t*)pipepayloadwidth;
  NRF_InitStruct.TxPayLoadWidth             = 7; 
  
  /* --- In order to enable Auto Acknowledgement with payload  --- *
   * --- the dynamic payload width should be set               --- */
  NRF_InitStruct.EnableAckPayLoad           = NRF_ENABLE_ACK_PAYLOAD;
  
  NRF_InitStruct.EnableDyanmeicPayLoadWidth = NRF_ENABLE_DYNAMIC_PLYLOAD_WIDTH;
  NRF_InitStruct.EnableDynamicPayLoadPipe   = NRF_DYNAMIC_PAYLOAD_WIDTH_P0;
  
  
  
	NRF24L01_Config(&hnrf24l01,&NRF_InitStruct);                   
}


