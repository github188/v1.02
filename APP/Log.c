/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			log.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "..\API\API.H"
#include "..\API\OS.H"
#include "ParamDefine.h"
#include "Log.h"
/*********************************************************************************************************
** @APP Function name:  LogGetMoneyAPI
** @APP Description:   	�ձ�ʱ������־
** @APP Input para:		InValueͶ�ҵĽ���Ϊ��λ,MoneyType=1Ӳ����,2ֽ����,3������
** @APP retrun para:    None
*********************************************************************************************************/
void LogGetMoneyAPI(uint32_t InValue,uint8_t MoneyType)
{
	uint16_t money=(InValue/10);	
	//������ϸ���׼�¼
	if(MoneyType == 1)
		TotalTradeLog.TotalCoinPayIn += money;
	else if(MoneyType == 2)
		TotalTradeLog.TotalBillPayIn += money;	
}
/*********************************************************************************************************
** @APP Function name:  LogTransactionAPI
** @APP Description:   	���׳���ʱ������־
** @APP Input para:		type=1�����ɹ�,0����ʧ��
** @APP retrun para:    None
*********************************************************************************************************/
void LogTransactionAPI(int type)
{
	if(type==1)
		TotalTradeLog.TotalSuccesNumber++;	
	else
		TotalTradeLog.TotalErrorNumber++;		
}
/*********************************************************************************************************
** @APP Function name:  LogChangeAPI
** @APP Description:   	����ʱ������־
** @APP Input para:		InValue����Ľ��
** @APP retrun para:    None
*********************************************************************************************************/
void LogChangeAPI(uint32_t InValue)
{	
	uint16_t money=(InValue/10);
	//���������׼�¼
	TotalTradeLog.TotalCoinPayout += money;
}
/*********************************************************************************************************
** @APP Function name:  LogChangeAPI
** @APP Description:   	����ʱ������־
** @APP Input para:		InValue����Ľ��
** @APP retrun para:    None
*********************************************************************************************************/
void LogEndAPI()
{	
	LoadNewTotalLog();
}
/*********************************************************************************************************
** @APP Function name:  LogClearAPI
** @APP Description:   	�ձ�ʱ������־
** @APP Input para:		ɾ��������־��¼
** @APP retrun para:    None
*********************************************************************************************************/
void LogClearAPI(void)
{
	TotalTradeLog.TotalCoinPayIn=0;
	TotalTradeLog.TotalBillPayIn=0;
	TotalTradeLog.TotalSuccesNumber=0;
	TotalTradeLog.TotalErrorNumber=0;	
	TotalTradeLog.TotalCoinPayout=0;
	LoadNewTotalLog();	
}
/**************************************End Of File*******************************************************/
