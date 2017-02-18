/***********************************************Copyright (c)************************************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info------------------------------------------------------------------------------------
 ** @Filename:			vendservice.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *************************************************************************************************************************************/
#include "..\API\API.H"
#include "..\API\OS.H"
#include "ParamDefine.h"
#include "MdbBillDeviceOperation.h"
#include "MdbCashlessDeviceOperation.h"
#include "MdbCoinDeviceOperation.h"
#include "Menu.h"
#include "Log.h"

#define DEBUG_VENDSERVICE							//������Ϣ����0���

#define VMC_FREE		  0                         //����״̬
#define VMC_CHAXUN	      1							//������ѯ״̬
#define VMC_SALE 	      3							//����״̬
#define VMC_XUANHUO    	  5							//ѡ��
#define VMC_READVENDFAIL  9							//�������ۿ�ʧ��
#define VMC_CHUHUO  	  10						//����
#define VMC_QUHUO  	  	  11						//ȡ��
#define VMC_PAYOUT 	      13						//����
#define VMC_END 	      14						//��������
#define VMC_ERROR		  15                        //����״̬

char     	ChannelNum[3] = {0};					//ѡ�����ֵ
uint8_t	 	vmcStatus = VMC_FREE;					//��ǰ״̬
uint8_t	 	vmcChangeLow = 0;						//��Ǯ�Ƿ񲻹���,1������,0����
uint8_t	    IsTuibi = 0;							//�Ƿ����˱Ұ�ť,1��
uint8_t	 	vmcEorr = 0;							//��������,1����

uint32_t 	g_coinAmount = 0;						//��ǰͶ��Ӳ���ܽ��
uint32_t 	g_billAmount = 0;    					//��ǰѹ��ֽ���ܽ��
uint32_t 	g_readerAmount = 0;						//��ǰ�������ܽ��

uint16_t	vmcColumn = 0;							//��ǰ�������Ʒ���
uint32_t 	vmcPrice  = 0;							//��ǰ�������Ʒ����
//uint8_t		LogicChannel[6][8];						//�߼�����,ÿ������������Ҫ���¼��� 

static void VendingService(void);
static void VendingIdle(void);
//static void SetupLogicChannel(void);				//��ӳ�����,������ʱ�ͳ��߼�������ӳ�䵽�������

/***************************************************************************************************************************************
** @APP Function name:   VendService
** @APP Descriptions:	 ����������
** @APP Input para:      pvParameters:no used
** @APP retrun para:     None
***************************************************************************************************************************************/
void VendService(void *pvParameters)
{
	pvParameters = pvParameters;
	API_SYSTEM_PinFuctionSelect();
	API_VENDING_Init();
	API_RTC_RtcInit();
	API_FRAM_Init();
	API_SYSTEM_TimerInit(0);
	API_SYSTEM_TimerInit(1);
	API_COM_Init(0,115200);		//Debug���
	API_MDB_UartInit();
	API_LCM_Init();
	API_KEY_KeyboardInit();
	API_BUZZER_Buzzer();
	InitVmcParam();
	InitTradeParam();
	InitTotalLog();
	VMCParam.VMCMode = VMC_MODE_VEND;
	while(1)
	{
		switch(VMCParam.VMCMode)
		{
			case VMC_MODE_VEND:
				VendingService();
				break;
			case VMC_MODE_MAIN:
				VendingIdle();
				break;
		}
		vTaskDelay(2);
	}
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_GetAmountMoney
** @APP Descriptions:	 ��ȡͶ���ܽ��;g_coinAmountӲ��Ͷ�ҽ��,g_billAmountֽ��Ͷ�ҽ��,g_readerAmount��������ý��
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
uint32_t Vend_GetAmountMoney(void)
{	
	return (g_coinAmount + g_billAmount + g_readerAmount);
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_IsTuibiAPI
** @APP Descriptions:	 �Ƿ��а���������
** @APP Input para:      None
** @APP retrun para:     1����,0û����
***************************************************************************************************************************************/
uint8_t Vend_IsTuibiAPI(void)
{
	if(IsTuibi == 1)
	{
		IsTuibi=0;
		return 1;
	}
	else
	{
		return 0;
	}
}
/***************************************************************************************************************************************
** @APP Function name:   BillCoinEnable
** @APP Descriptions:	 ʹ�ܽ���ֽ����Ӳ����
** @APP Input para:      billCtr����ֽ����=1,ʹ��,2����,0������;coinCtr����Ӳ����=1,ʹ��,2����,0������;readerCtr���ƶ�����=1,ʹ��,2����,0������
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_BillCoinCtr(uint8_t billCtr,uint8_t coinCtr,uint8_t readerCtr)
{
	uint32_t InValue=0;
	uint8_t  billOptBack = 0;
	uint8_t Billtype = 0;
	//1.ֽ��������
	if(VMCParam.MdbBillDeviceEAB == 0x01)
	{
		switch(billCtr)
		{
			case 1:
				#ifdef DEBUG_VENDSERVICE
				Trace("\r\n%S,%d:App Enable BillDevice",__FILE__,__LINE__);
				#endif
				BillDevProcess(&InValue,&Billtype,MBOX_BILLENABLEDEV,&billOptBack);
				break;
			case 2:
				#ifdef DEBUG_VENDSERVICE
				Trace("\r\n%S,%d:App Disable BillDevice",__FILE__,__LINE__);
				#endif
				BillDevProcess(&InValue,&Billtype,MBOX_BILLDISABLEDEV,&billOptBack);
				break;
			default:break;	
		}
	}
	//2.Ӳ��������
	if(VMCParam.MdbCoinDeviceEAB == 0x01)
	{
		switch(coinCtr)
		{
			case 1:
				#ifdef DEBUG_VENDSERVICE
				Trace("\r\n%S,%d:App Enable CoinDevice",__FILE__,__LINE__);
				#endif
				MdbCoinTypeEanbleOrDisable(1,1);
				break;
			case 2:
				#ifdef DEBUG_VENDSERVICE
				Trace("\r\n%S,%d:App Disable CoinDevice",__FILE__,__LINE__);
				#endif
				MdbCoinTypeEanbleOrDisable(0,1);
				break;
			default:break;	
		}
	}
	//3.����������
	if(VMCParam.MdbCashlessDeviceEAB == 0x01)
	{
		switch(readerCtr)
		{
			case 1:
				//ReaderDevEnableAPI();
				break;
			case 2:
				//ReaderDevDisableAPI();
				break;
			default:break;	
		}
	}
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_ClearDealPar
** @APP Descriptions:	 �ص�����״̬ǰ����������׵�����
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_ClearDealPar(void)
{
	memset(ChannelNum,0,sizeof(ChannelNum));
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_CLrBusinessText
** @APP Descriptions:	 �彻��ʱ��ʾ����
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_CLrBusinessText(void)
{	
	API_LCM_ClearArea(0,13,239,15);
	vTaskDelay(2);	
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispFreePage
** @Descriptions Func:	 ����ҳ��,vmcEorr����������ҳ����ʾ,vmcChangeLow�������ȱ��ҳ����ʾ
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispFreePage(void)
{
	unsigned char DisplayX;	//����X����	
	API_LCM_ClearArea(0,6,239,7);
	vTaskDelay(2);	
	if(vmcEorr == 1)
	{
		DisplayX = (239 - (strlen(UIMenu.error[VMCParam.Language]) * 8)) / 2;
		API_LCM_Printf(DisplayX,6,0,0,UIMenu.error[VMCParam.Language]);
	}
	else if(vmcChangeLow == 1) 
	{
		DisplayX = (239 - (strlen(UIMenu.changeempty[VMCParam.Language]) * 8)) / 2;
		API_LCM_Printf(DisplayX,6,0,0,UIMenu.changeempty[VMCParam.Language]);
	}
	else
	{
		DisplayX = (239 - (strlen(UIMenu.welcome[VMCParam.Language]) * 8)) / 2;
		API_LCM_Printf(DisplayX,6,0,0,UIMenu.welcome[VMCParam.Language]);
	}
	vTaskDelay(2);
	API_RTC_Read((void *)VMCParam.Rtc);
	API_LCM_Printf(55,8,0,0,"%04D/%02D/%02D %02D:%02D",((((unsigned int)VMCParam.Rtc[5]) << 8) + VMCParam.Rtc[6]),VMCParam.Rtc[4],VMCParam.Rtc[3],VMCParam.Rtc[2],VMCParam.Rtc[1]);
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispChaxunPage
** @APP Descriptions:	 �����ѯ������ҳ��
** @APP Input para:      keyValue������ֵ
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispChaxunPage(uint8_t *keyValue)
{
	unsigned char layer = 0x00,channel = 0x00;
	char    *pstr;
	char	strMoney[10];
	static unsigned char channelInput,LayerOk;
	//ȡ��
	if(*keyValue == '>')
	{
		Vend_ClearDealPar();
		Vend_CLrBusinessText();
		API_LCM_Printf(7,13,0,0,UIMenu.column[VMCParam.Language],"--");
		vmcStatus = VMC_SALE;
		LayerOk = 0x00;
		channelInput = 0x00;
		return;
	}
	if(LayerOk == 0x00)
	{
		if((*keyValue >= 'A') && (*keyValue <= 'F'))
		{
			LayerOk = 0x01;
			channelInput = 0x00;
			ChannelNum[channelInput++] = *keyValue;
			API_LCM_Printf(7,13,0,0,UIMenu.column[VMCParam.Language],ChannelNum);
		}
		else
			API_LCM_Printf(7,13,0,0,UIMenu.column[VMCParam.Language],"--");
	}
	else
	{
		if((*keyValue >= '0') && (*keyValue <= '9'))
		{
			ChannelNum[channelInput++] = *keyValue;
			API_LCM_Printf(7,13,0,0,UIMenu.column[VMCParam.Language],ChannelNum);
			vTaskDelay(100);
		}
		else
		{
			LayerOk = 0x00;
			channelInput = 0x00;
			API_LCM_Printf(7,13,0,0,UIMenu.column[VMCParam.Language],"--");
		}
	}
	if(channelInput >= 2)
	{
		LayerOk = 0x00;
		channelInput = 0x00;
		layer = ChannelNum[0];
		channel = ChannelNum[1] - 0x30;
		vmcColumn = ((layer - 0x41)*8 + (channel - 1));
		#ifdef DEBUG_VENDSERVICE
		Trace("\r\n%S,%d:%s column%ld=%d,%d,%d\r\n",__FILE__,__LINE__,ChannelNum,vmcColumn,VMCParam.GoodsChannelArray[vmcColumn],TradeParam.GoodsPrice[vmcColumn],TradeParam.RemainGoods[vmcColumn]);
		#endif
		if((VMCParam.GoodsChannelArray[vmcColumn] == 0) || (TradeParam.GoodsPrice[vmcColumn] == 0))	//����δ���� �� ��������Ϊ0
		{
			Vend_ClearDealPar();
			Vend_CLrBusinessText();
			API_LCM_Printf(7,13,0,0,UIMenu.column[VMCParam.Language],"--");
			vmcStatus = VMC_SALE;
			#ifdef DEBUG_VENDSERVICE
			Trace("\r\n%S,%d:Channel is disable or Price = 0",__FILE__,__LINE__);
			#endif
		}
		else if(TradeParam.RemainGoods[vmcColumn] == 0)	//������ǰ�������
		{
			Vend_ClearDealPar();
			Vend_CLrBusinessText();
			API_LCM_Printf(7,13,0,0,UIMenu.soldout[VMCParam.Language]);
			vmcStatus = VMC_SALE;
			#ifdef DEBUG_VENDSERVICE
			Trace("\r\n%S,%d:Goods is sold out",__FILE__,__LINE__);
			#endif
		}
		else
		{
			vmcPrice=(uint32_t)TradeParam.GoodsPrice[vmcColumn]*10;//�Է�Ϊ��λ����
			pstr = PrintfMoney(vmcPrice);
			strcpy(strMoney, pstr);
			Vend_CLrBusinessText();
			API_LCM_Printf(7,13,0,0,UIMenu.price[VMCParam.Language],ChannelNum,strMoney);
			if(Vend_GetAmountMoney() >= vmcPrice)
			{
				#ifdef DEBUG_VENDSERVICE
				Trace("\r\n%S,%d:Go to vending",__FILE__,__LINE__);
				#endif
				vmcStatus = VMC_CHUHUO;
			}
			else
			{
				#ifdef DEBUG_VENDSERVICE
				Trace("\r\n%S,%d:User need pay in more",__FILE__,__LINE__);
				#endif				
				vmcStatus = VMC_SALE;
				Vend_ClearDealPar();
			}
		}
	}	
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispSalePage
** @APP Descriptions:	 Ͷ�ҽ��н���ҳ��
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispSalePage(void)
{
	char    *pstr;
	char	strMoney[10];
	pstr = PrintfMoney(Vend_GetAmountMoney());
	strcpy(strMoney,pstr);
	API_LCM_ClearScreen();
	vTaskDelay(2);
	API_LCM_Printf(7,1,0,0,UIMenu.amount[VMCParam.Language]);
	API_LCM_PutRMBSymbol(31,5);//24x32 bmp
	API_LCM_Printf(71,5,1,0,"%S",strMoney);//dot 32x16
	API_LCM_DrawLine(0,12);
	API_LCM_Printf(7,13,0,0,UIMenu.column[VMCParam.Language],"");
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispChuhuoPage
** @APP Descriptions:	 ��������
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispChuhuoPage(void)
{
	Vend_CLrBusinessText();
	API_LCM_Printf(7,13,0,0,UIMenu.dispense[VMCParam.Language]);	
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispQuhuoPage
** @APP Descriptions:	 ȡ������
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispQuhuoPage(void)
{
	Vend_CLrBusinessText();
	API_LCM_Printf(7,13,0,0,UIMenu.takecolumn[VMCParam.Language]);
	vTaskDelay(400*3);
}
/***************************************************************************************************************************************
** @APP Function name:   DispChhuoFailPage
** @APP Descriptions:	 ����ʧ�ܽ���
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void DispChhuoFailPage(void)
{
	Vend_CLrBusinessText();
	API_LCM_Printf(7,13,0,0,UIMenu.soldout[VMCParam.Language]);
	vTaskDelay(400*3);
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispPayoutPage
** @APP Descriptions:	 �����������
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispPayoutPage(void)
{
	char    *pstr;
	char	strMoney[10];
	Vend_CLrBusinessText();
	pstr = PrintfMoney(Vend_GetAmountMoney());
	strcpy(strMoney, pstr);
	API_LCM_Printf(7,13,0,0,UIMenu.payout[VMCParam.Language],strMoney);
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispQuChangePage
** @APP Descriptions:	 ȡ��Ǯ
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispQuChangePage(void)
{
	Vend_CLrBusinessText();
	API_LCM_Printf(7,13,0,0,UIMenu.takemoney[VMCParam.Language]);
	vTaskDelay(400*3);
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispIOUPage
** @APP Descriptions:	 ����ʧ��
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispIOUPage(uint32_t debtMoney)
{
	char    *pstr;
	char	strMoney[10];
	Vend_CLrBusinessText();
	pstr = PrintfMoney(debtMoney);
	strcpy(strMoney, pstr);
	API_LCM_Printf(7,13,0,0,UIMenu.IOU[VMCParam.Language],strMoney);
	vTaskDelay(400*3);
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_DispEndPage
** @APP Descriptions:	 ���׽���
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_DispEndPage(void)
{
	API_LCM_ClearScreen();	
	vTaskDelay(20);
	API_LCM_Printf(((240 - (strlen(UIMenu.end[VMCParam.Language]) * 8)) / 2),7,0,0,UIMenu.end[VMCParam.Language]);
	vTaskDelay(300*3);
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_UpdateTubeMoney
** @APP Descriptions:	 ����Ӳ����ʣ��Ӳ�ҽ��,��С��3Ԫʱ,�ر��ֽ��豸����ȱ��λ��λ��������3Ԫʱ,�����ֽ��豸�����ȱ��λ
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_UpdateTubeMoney(void)
{
	uint32_t coinMoney=0;	
	uint8_t i;
	MdbCoinGetTubeStatus();
	for(i=0;i<16;i++)
	{
		coinMoney+=MDBCoinDevice.CoinTypePresentInTube[i]*MdbGetCoinValue(MDBCoinDevice.CoinTypeCredit[i]);
	}
	#ifdef DEBUG_VENDSERVICE
	Trace("\r\n%S,%d:Update coin in tube:%d",__FILE__,__LINE__,coinMoney);
	#endif
	//��ǮС��3Ԫ
	if(coinMoney < 300)
	{
		if(vmcChangeLow == 0)
		{
			vmcChangeLow = 1;
			Vend_BillCoinCtr(2,2,0);
		}
	}
	else
	{
		if(vmcChangeLow == 1)
		{
			vmcChangeLow=0;
			Vend_BillCoinCtr(1,1,0);
		}
	}
	vTaskDelay(10);	
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_GetMoney
** @APP Descriptions:	 Ӳ������ѯ,ֽ������ѯ����ȡ�û�Ͷ�ҽ��
** @APP Input para:      None
** @APP retrun para:     1�����յ�Ͷ����Ϣ,0û��
***************************************************************************************************************************************/
uint8_t Vend_GetMoney(void)
{
	uint32_t InValue=0,billEscrow=0;
	uint8_t  billOptBack = 0;
	uint8_t Billtype = 0;
	//1.Ӳ����ѯ
	if(VMCParam.MdbCoinDeviceEAB == 0x01)
	{
		vTaskDelay(20);
		CoinDevProcess(&InValue,&Billtype,&billOptBack);
		if(InValue > 0)
		{			
			g_coinAmount += InValue;
			#ifdef DEBUG_VENDSERVICE
			Trace("\r\n%S,%d:Current Coin Pay in:%ld",__FILE__,__LINE__,InValue);
			#endif
			LogGetMoneyAPI(InValue,1);//��¼��־
			return 1;		
		}
		//�ж��Ƿ���Ӳ�����˱Ұ���
		if(billOptBack == 1)
		{
			#ifdef DEBUG_VENDSERVICE
			Trace("\r\n%S,%d:User press down escrow request",__FILE__,__LINE__);
			#endif
			API_BUZZER_Buzzer();
			IsTuibi = 1;
		}
	}
	//2.ֽ����ѯ
	if(VMCParam.MdbBillDeviceEAB == 0x01)
	{
		//�ж��Ƿ���ֽ��Ͷ��
		vTaskDelay(20);
		BillDevProcess(&InValue,&Billtype,0,&billOptBack);
		if(InValue > 0)
		{
			#ifdef DEBUG_VENDSERVICE
			Trace("\r\n%S,%d:Current Bin Pay in:%ld",__FILE__,__LINE__,InValue);
			#endif			
			BillDevProcess(&billEscrow,&Billtype,MBOX_BILLESCROW,&billOptBack);
			if(billOptBack == 2)
			{
				#ifdef DEBUG_VENDSERVICE
				Trace("\r\n%S,%d:EscrowSuccess:%ld",__FILE__,__LINE__,InValue);
				#endif					
				g_billAmount += InValue;
				LogGetMoneyAPI(InValue,2);//��¼��־
				InValue = 0;
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
	return 0;
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_ChangerMoney
** @APP Descriptions:	 ����
** @APP Input para:      None
** @APP retrun para:     ���ؽ�����ʧ�ܷ���Ƿ�������ɹ�����0
***************************************************************************************************************************************/
uint32_t Vend_ChangerMoney(void)
{	
	uint32_t tempmoney=0,backmoney=0;
	#ifdef DEBUG_VENDSERVICE
	Trace("\r\n%S,%d:App Changer:%d",__FILE__,__LINE__,Vend_GetAmountMoney());
	#endif
	tempmoney = Vend_GetAmountMoney();
	//����Ӳ��
	if(Vend_GetAmountMoney())
	{		
		ChangePayoutProcessLevel3(Vend_GetAmountMoney(),&backmoney);	
		#ifdef DEBUG_VENDSERVICE
		Trace("\r\n%S,%d:App Pay back money:%d",__FILE__,__LINE__,backmoney);
		#endif
	}	
	LogChangeAPI(backmoney);//��¼��־
	//����ʧ��
	if(tempmoney > backmoney)
	{		
		#ifdef DEBUG_VENDSERVICE
		Trace("\r\n%S,%d:App change Fail",__FILE__,__LINE__);
		#endif		
		g_coinAmount = 0;
		g_billAmount = 0;
		return  tempmoney-backmoney;
	}
	//����ɹ�
	else
	{
		#ifdef DEBUG_VENDSERVICE
		Trace("\r\n%S,%d:App change Sucess",__FILE__,__LINE__);
		#endif	
		g_coinAmount = 0;
		g_billAmount = 0;
		return 0;
	}
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_SaleCostMoney
** @APP Descriptions:	 ������۳����
** @APP Input para:      PriceSale��Ʒ���
** @APP retrun para:     None
***************************************************************************************************************************************/
void Vend_SaleCostMoney(uint32_t PriceSale)
{	
	//�۳�����
	if(PriceSale)
	{
		/*//����������ͽ��׳ɹ�
		if(g_readerAmount > 0)
		{
			TraceReader("\r\n AppCHuhuoSucc");
			ReaderDevVendoutResultAPI(1);
			g_readerAmount -= PriceSale;
			PriceSale = 0;
		}		
		else 
		*/
		if(PriceSale >= g_billAmount)
		{
			PriceSale -= g_billAmount;
			g_billAmount = 0;
		}
		else
		{
			g_billAmount -= PriceSale;
			PriceSale = 0;
		}
	}
	if(PriceSale)
	{
		g_coinAmount -= PriceSale;					
	}	
}
/***************************************************************************************************************************************
** @APP Function name:   Vend_IsErrorState
** @APP Descriptions:	 �Ƿ�������״̬
** @APP Input para:      None
** @APP retrun para:     1�����豸��0���� 
***************************************************************************************************************************************/
uint8_t Vend_IsErrorState()
{ 
	uint8_t coinError = 0,billError = 0;
	//ֽ����	
	if(VMCParam.MdbBillDeviceEAB == 0x01)
	{
		if((MdbBillErr.Communicate)||(MdbBillErr.moto)||(MdbBillErr.sensor)||(MdbBillErr.romchk)||(MdbBillErr.jam)||(MdbBillErr.removeCash)||(MdbBillErr.cashErr))
		{
			//billError = 1;			
		}	
	}
	#ifdef DEBUG_VENDSERVICE	
	Trace("\r\n%S,%d:BillErr=%d,%d,%d,%d,%d,%d,%d",__FILE__,__LINE__,MdbBillErr.Communicate,MdbBillErr.moto,MdbBillErr.sensor,MdbBillErr.romchk,MdbBillErr.jam,MdbBillErr.removeCash,MdbBillErr.cashErr);
	#endif
	//Ӳ����
	if(VMCParam.MdbCoinDeviceEAB == 0x01)
	{
		if((MdbCoinErr.Communicate)||(MdbCoinErr.sensor)||(MdbCoinErr.tubejam)||(MdbCoinErr.romchk)||(MdbCoinErr.routing)||(MdbCoinErr.jam)||(MdbCoinErr.removeTube))
		{
			coinError = 1;	
		}		
	}
	#ifdef DEBUG_VENDSERVICE	
	Trace("\r\n%S,%d:CoinErr=%d,%d,%d,%d,%d,%d,%d",__FILE__,__LINE__,MdbCoinErr.Communicate,MdbCoinErr.sensor,MdbCoinErr.tubejam,MdbCoinErr.romchk,MdbCoinErr.routing,MdbCoinErr.jam,MdbCoinErr.removeTube);
	#endif
	if(coinError || billError)
	{
		vmcEorr=1;
		return 1;
	}	
	else
	{
		vmcEorr=0;
		return 0;
	}
}
/***************************************************************************************************************************************
** @APP Function name:   VendingService
** @APP Descriptions:	 �û���������
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
static void VendingService(void)
{
	uint8_t  billOptBack = 0;
	uint32_t InValue = 0;
	uint8_t Billtype = 0;

	uint8_t keyValue = 0;//����ֵ
	uint8_t moneyGet = 0;//�Ƿ��Ѿ���Ǯ��
	uint32_t debtMoney;	
	uint8_t ChuhuoRst = 0;
	API_LCM_ClearScreen();
	API_LCM_Printf(0,0,0,0,"Init system,please wait...\r\n");
	//��ʼ��Ӳ����
	if(VMCParam.MdbCoinDeviceEAB == 0x00)
	{
		API_LCM_Printf(0,2,0,0,"CoinDevice is disable...\r\n");
	}
	else
	{
		MdbCoinResetAndSetup();
		if(MDBCoinDevice.Oneline)
			API_LCM_Printf(0,2,0,0,"Init CoinDevice OK...\r\n");
		else
			API_LCM_Printf(0,2,0,0,"Init CoinDevice ERR...\r\n");
		MdbCoinTypeEanbleOrDisable(1,1);
		vTaskDelay(20);
		CoinDevProcess(&InValue,&Billtype,&billOptBack);
		vTaskDelay(20);		
	}
	//��ʼ��ֽ����
	if(VMCParam.MdbBillDeviceEAB == 0x00)
	{
		API_LCM_Printf(0,4,0,0,"BillDevice is disable...\r\n");
	}
	else
	{
		MdbBillResetAndSetup();
		if(MDBBillDevice.Oneline)
			API_LCM_Printf(0,4,0,0,"Init BillDevice OK...\r\n");
		else
			API_LCM_Printf(0,4,0,0,"Init BillDevice ERR...\r\n");
		BillDevProcess(&InValue,&Billtype,MBOX_BILLENABLEDEV,&billOptBack);
		vTaskDelay(20);
		BillDevProcess(&InValue,&Billtype,0,&billOptBack);
		vTaskDelay(20);
	}
	API_LCM_ClearScreen();
	API_SYSTEM_TimerChannelSet(4,5 * 100);		
	while(1)
	{
		switch(vmcStatus)
		{
			case VMC_FREE:
				//1.��ʾ����ҳ��
				if(API_SYSTEM_TimerReadChannelValue(1) == 0)
				{
					API_SYSTEM_TimerChannelSet(1,5 * 100);
					Vend_DispFreePage();
				}
				//2.����豸����״̬
				if(API_SYSTEM_TimerReadChannelValue(4) == 0)
				{
					API_SYSTEM_TimerChannelSet(4,5 * 100);				
					if(Vend_IsErrorState())
					{
						API_SYSTEM_TimerChannelSet(1,0);
						Vend_BillCoinCtr(2,2,0);
						vmcStatus = VMC_ERROR;
					}
				}			
				//3.��ѯͶֽ�Һ�Ӳ�ҽ��	
				moneyGet = Vend_GetMoney();
				if(moneyGet == 1)
				{
					IsTuibi=0;
					Vend_DispSalePage();
					#ifdef DEBUG_VENDSERVICE
					Trace("\r\n%S,%d:App User Pay in,amount = %ld",__FILE__,__LINE__,Vend_GetAmountMoney());
					#endif				
					vmcStatus = VMC_SALE;
				}
				//4.��ѯӲ����������Ӳ��
				if(API_SYSTEM_TimerReadChannelValue(2) == 0)
				{
					API_SYSTEM_TimerChannelSet(2,5 * 100);
					Vend_UpdateTubeMoney();
				}
				//5.�ж��Ƿ����ά��ҳ��
				if(API_KEY_ReadKey() == 'M')
				{
					VMCParam.VMCMode = VMC_MODE_MAIN;
					break;
				}
				break;
			case VMC_CHAXUN:
				//1.��ѯ����
				if(keyValue)
				{
					#ifdef DEBUG_VENDSERVICE
					Trace("\r\n%S,%d:App User press key",__FILE__,__LINE__);
					#endif
					API_SYSTEM_TimerChannelSet(3,5 * 100);
					Vend_DispChaxunPage(&keyValue);
				}
				keyValue = API_KEY_ReadKey();
				if(keyValue)
				{					
					if((keyValue >= '1') && (keyValue <= '8'))
						vTaskDelay(2);
					else if(keyValue == '>')
						vTaskDelay(2);
					else
						keyValue=0;
				}
				//2.��ѯͶֽ�Һ�Ӳ�ҽ��
				moneyGet = Vend_GetMoney();
				if(moneyGet == 1)
				{
					Vend_DispSalePage();
					#ifdef DEBUG_VENDSERVICE
					Trace("\r\n%S,%d:App User Pay in,amount = %ld",__FILE__,__LINE__,Vend_GetAmountMoney());
					#endif				
				}
				//3.��ʱ�˳���ѯҳ��
				if(API_SYSTEM_TimerReadChannelValue(3) == 0)
				{
					keyValue = '>';
					API_SYSTEM_TimerChannelSet(3,5 * 100);
				}	
				//4.�а����˱Ұ���
				if(Vend_IsTuibiAPI())
				{
					#ifdef DEBUG_VENDSERVICE
					Trace("\r\n%S,%d:App User Press Escrow button",__FILE__,__LINE__);
					#endif
					Vend_BillCoinCtr(2,2,0);
					vmcStatus = VMC_PAYOUT;
				}
				break;	
			case VMC_SALE:
				//1.��ѯͶֽ�Һ�Ӳ�ҽ��
				moneyGet = Vend_GetMoney();
				if(moneyGet == 1)
				{
					Vend_DispSalePage();
					#ifdef DEBUG_VENDSERVICE
					Trace("\r\n%S,%d:App User Pay in,amount = %ld",__FILE__,__LINE__,Vend_GetAmountMoney());
					#endif					
				}
				//2.��ѯ����
				keyValue = API_KEY_ReadKey();
				if(keyValue)
				{
					//ȷ��
					if((keyValue >= 'A') && (keyValue <= 'F'))
					{
						#ifdef DEBUG_VENDSERVICE
						Trace("\r\n%S,%d:App User Press key = %d",__FILE__,__LINE__,keyValue);
						#endif
						API_SYSTEM_TimerChannelSet(3,5 * 100);
						vmcStatus = VMC_CHAXUN;	
						Vend_CLrBusinessText();
						break;
					}
				}	
				//3.�а����˱Ұ���
				if(Vend_IsTuibiAPI())
				{
					#ifdef DEBUG_VENDSERVICE
					Trace("\r\n%S,%d:App User Press Escrow button",__FILE__,__LINE__);
					#endif
					Vend_BillCoinCtr(2,2,0);
					vmcStatus = VMC_PAYOUT;
				}
				break;
			case VMC_CHUHUO:
				//����ǰ���ܰ������ֽ��豸��
				Vend_BillCoinCtr(2,2,0);				
				Vend_DispChuhuoPage();
				API_KEY_KeyboardCtrl(0x00);			
				ChuhuoRst = API_VENDING_Vend(ChannelNum[0],ChannelNum[1]);
				API_KEY_KeyboardCtrl(0x01);				
				if(ChuhuoRst==1)
				{	
					//������������ݼ�
					#ifdef DEBUG_VENDSERVICE
					Trace("\r\n%S,%d:App Vending success and go to deduct money",__FILE__,__LINE__);
					#endif
					if(TradeParam.RemainGoods[vmcColumn] > 0)
					{
						TradeParam.RemainGoods[vmcColumn]--;
						LoadNewTradeParam();
					}
					Vend_DispQuhuoPage();	
					Vend_SaleCostMoney(vmcPrice);//�ۿ�				
					#ifdef DEBUG_VENDSERVICE
					Trace("\r\n%S,%d:Deduct money,Remain = %d",__FILE__,__LINE__,Vend_GetAmountMoney());
					#endif							
					LogTransactionAPI(1);//��¼��־
				}
				else
				{
					#ifdef DEBUG_VENDSERVICE
					Trace("\r\n%S,%d:App Vending Fail",__FILE__,__LINE__);
					#endif
					DispChhuoFailPage();			
					LogTransactionAPI(0);//��¼��־
				}
				vmcPrice = 0;
				vmcColumn= 0;
				Vend_ClearDealPar();
				if(Vend_GetAmountMoney())
				{
					//�û����������ֽ��豸����������
					Vend_BillCoinCtr(1,1,0);	
					Vend_DispSalePage();
					vmcStatus = VMC_SALE;
				}
				else
				{
					vmcStatus = VMC_END;
				}	
				break;	
			case VMC_PAYOUT:				
				Vend_DispPayoutPage();
				debtMoney = Vend_ChangerMoney();
				if(debtMoney)
				{
					Vend_DispIOUPage(debtMoney);
				}
				else
				{
					Vend_DispQuChangePage();
				}				
				vmcStatus = VMC_END;
				break;
			case VMC_END:				
				Vend_DispEndPage();
				LogEndAPI();
				Vend_ClearDealPar();
				vmcColumn = 0;	
				Vend_BillCoinCtr(1,1,0);				
				API_LCM_ClearScreen();
				vmcStatus = VMC_FREE;
				#ifdef DEBUG_VENDSERVICE
				Trace("\r\n\r\n%S,%d:App Vending Flow end...\r\n",__FILE__,__LINE__);
				#endif
				break;	
			case VMC_ERROR:					
				//1.��ʾ��ӭ�����ҳ��
				if(API_SYSTEM_TimerReadChannelValue(1) == 0)
				{
					API_SYSTEM_TimerChannelSet(1,5 * 100);
					Vend_DispFreePage();
				}	
				//2.����豸�Ƿ�ָ�����
				if(API_SYSTEM_TimerReadChannelValue(4) == 0)
				{
					API_SYSTEM_TimerChannelSet(4,5 * 100);	
					if(!Vend_IsErrorState())
					{	
						API_SYSTEM_TimerChannelSet(1,0);
						Vend_BillCoinCtr(1,1,1);
						vmcStatus = VMC_FREE;
					}
				}
				//3.ֻ����ѯ
				Vend_GetMoney();
				//4.�ж��Ƿ����ά��ҳ��
				if(API_KEY_ReadKey() == 'M')
					VMCParam.VMCMode = VMC_MODE_MAIN;
				break;
			default:
				break;
		}
		//�ж��Ƿ����ά��ҳ��
		if(VMCParam.VMCMode == VMC_MODE_MAIN)
		{
			vmcStatus = VMC_FREE;
			break;
		}
		vTaskDelay(2);
	}
}
/***************************************************************************************************************************************
** @APP Function name:   VendingIdle
** @APP Descriptions:	 �ص�����״̬ǰ����������׵����ݣ���ʱϵͳ����ά��ģʽ
** @APP Input para:      None
** @APP retrun para:     None
***************************************************************************************************************************************/
static void VendingIdle(void)
{
	LPC_GPIO2->FIODIR |= 1ul <<12;
	LPC_GPIO2->FIOSET |= 1ul <<12;
	vTaskDelay(50);
	LPC_GPIO2->FIODIR |= 1ul <<12;	
	LPC_GPIO2->FIOCLR |= 1ul <<12;
	vTaskDelay(50);
}
/**************************************End Of File*************************************************************************************/
