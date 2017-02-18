/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			Menu.c
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#include "Menu.h"
/*****************************************�û����ײ˵�**************************************************/
/********************************************************************************************************
** @Define name:       welcome
********************************************************************************************************/
const UIMEMU UIMenu =
{
	{"��ӭ����","welcome"},
	{"������Ͷ��","Pls insert cash"},
	{"����Ͷ��Ľ��:","You Pay in:"},
	{"��������Ʒ���:%s","Merchandise NO:%s"},
	{"%s ��Ʒ�۸�: %s","%s U.P.: %s"},
	{"���ڳ���...","Vending..."},
	{"��ȡ����Ʒ","Take Merchandise away"},
	{"��Ʒ������","Sold out"},
	{"����:%s","Changer:%s"},
	{"��ȡ����Ǯ","Take coin away:"},
	{"������Ҳ���,Ƿ��: %s","IOU: %s"},
	{"лл�ݹ�","Thank you"},
	{"���������","Unable to Give Change Now"},
	{"��ͣ����","Stop service"}
};
/*********************************************ά���˵�**************************************************/
/*********************************************************************************************************
** @Menu Define name:   LoginUI
*********************************************************************************************************/
const LOGINUI LoginUI =
{
	{"��½ά��","LOGIN"},
	{"��������:","Enter Password:"},
	{"�������","Password ERR"},
	{"��<ȡ��>�˳�","Press 'cancel' to exit"}
};
/*********************************************************************************************************
** @Menu Define name:   MaintenUI
*********************************************************************************************************/
const MAITENMENU MaintenUI =
{
	{"��Ӫά��","Maint"},
	{"A.�豸��Ϣ","A.Dev Info"},
	{"B.�豸����","B.Dev Config"},
	{"C.�豸���","C.Dev Test"},
	{"D.��������","D.Trade Config"},
	{"E.���׼�¼","E.Trade LOG"},
	{"��<ȡ��>�˳�","Press 'cancel' to exit"}
};
/*********************************************************************************************************
** @Menu Define name:   DevInfo
*********************************************************************************************************/
const DEVINFO DevInfo =
{
	{"��Ӫά��->�豸��Ϣ","Maint->Dev Info"},
	{"A.����汾:","A.Version:"},
	{"B.�豸���к�:","B.VMC ID:"},
	{"C.������:","C.Err code:"},
	{"��<ȡ��>����","Press 'cancel' to exit"}
};
/*********************************************************************************************************
** @API Define name:   DevConfig
*********************************************************************************************************/
const DEVCONFIG DevConfig =
{
	{"��Ӫά��->�豸����","Maint->Dev Config"},
	{"A.�Ƿ���ֽ����:%d","A.Bill validator:%d"},
	{"B.�Ƿ���Ӳ����:%d","B.Coin acceptor/changer:%d"},
	{"C.�Ƿ���������:%d","C.Cashless:%d"},
	{"D.��������:%d","D.Language:%d"},
	{"E.ʱ������","E.RTC Set"},
		{"��Ӫά��->�豸����->ʱ��","Maint->Dev Config->RTC"},
		{"A.��:%d","A.Year:%d"},
		{"B.��:%d","B.Mon:%d"},
		{"C.��:%d","C.Day:%d"},
		{"D.ʱ:%d","D.Hour:%d"},
		{"E.��:%d","E.Min:%d"},		
	{"��ʾ:0-Close,1-Open","Note:0-Close,1-Open"},
	{"��<ȡ��>����","Press 'cancel' back"}
};
/*********************************************************************************************************
** @Menu Define name:   DevTest
*********************************************************************************************************/
const DEVTEST DevTest =
{
	{"��Ӫά��->�豸����","Maint->Dev Test"},
	{"A.ֽ��������","A.Bill validator Test"},
	{"B.Ӳ��������","B.Coin acceptor/changer Test"},
	{"C.����������","C.Cashless Test"},
	{"D.��������","D.Goods channel Test"},
	{"��<ȡ��>����","Press 'cancel' back"}
};
/*********************************************************************************************************
** @Menu Define name:   BillTest
*********************************************************************************************************/
const BILLDEVTEST BillTest =
{
	{"��Ӫά��->�豸����->ֽ����","Maint->Dev Test->Bill"},
	{"<--ֽ��������-->","<--Bill validator Test-->"},
	{"ֽ��δ����","Bill validator is not config"},					//msg0
	{"δ����ֽ����,����Ӳ��","Not found,pls check hardware"},		//msg1
	{"Ӳ������,��Ͷ��","Check Ok,Pls insert bill"},					//msg2
	{"������,��ȡ��ֽ��","Stacker is full,Pls remove bill"},		//msg3
	{"ֽ��������,����Ӳ��","validator ERR,pls check hardware"},	//msg4
	{"��������","Bill Type is Disable"},							//msg5
	{"ֽ�ң��ձ�--%s","OK:Type currency--%s"},				//msg6
	{"��<ȡ��>�˳�����","Press 'cancel' to exit test"}
};
/*********************************************************************************************************
** @Menu Define name:   CoinTest
*********************************************************************************************************/
const COINDEVTEST CoinTest =
{
	{"��Ӫά��->�豸����->Ӳ����","Maint->Dev Test->Coin"},
	{"<--Ӳ��������-->","<--Bill validator Test-->"},
	{"ֽ��δ����","Bill validator is not config"},					//msg0
	{"δ����ֽ����,����Ӳ��","Not found,pls check hardware"},		//msg1
	{"Ӳ������,��Ͷ��","Check Ok,Pls insert bill"},					//msg2
	{"������,��ȡ��ֽ��","Stacker is full,Pls remove bill"},		//msg3
	{"ֽ��������,����Ӳ��","validator ERR,pls check hardware"},	//msg4
	{"��������","Bill Type is Disable"},							//msg5
	{"Ӳ�ң��ձ�--%s","OK:Type currency--%s"},				//msg6
	{"Ӳ�ң�����--%s","OK:Payout currency--%s"},				//msg7
	{"��<ȡ��>�˳�����","Press 'cancel' to exit test"}
};
/*********************************************************************************************************
** @Menu Define name:   CashlessTest
*********************************************************************************************************/
const CASHLESSDEVTEST CashlessTest =
{
	{"��Ӫά��->�豸����->������","Maint->Dev Test->Cashless"},
	{"<--����������-->","<--Bill validator Test-->"},
	{"ֽ��δ����","Bill validator is not config"},					//msg0
	{"δ����ֽ����,����Ӳ��","Not found,pls check hardware"},		//msg1
	{"Ӳ������,��Ͷ��","Check Ok,Pls insert bill"},					//msg2
	{"������,��ȡ��ֽ��","Stacker is full,Pls remove bill"},		//msg3
	{"ֽ��������,����Ӳ��","validator ERR,pls check hardware"},	//msg4
	{"��������","Bill Type is Disable"},							//msg5
	{"������ͨ��--%d,���--","OK:Type--%d;currency--"},				//msg6
	{"��<ȡ��>�˳�����","Press 'cancel' to exit test"}
};
/*********************************************************************************************************
** @Menu Define name:   ChannelTest
*********************************************************************************************************/
const CHANNELDEVTEST ChannelTest=
{
	{"��Ӫά��->�豸����->����","Maint->Dev Test->GC"},
	{"<--��������-->","<--Bill validator Test-->"},
	{"ֽ��δ����","Bill validator is not config"},					//msg0
	{"δ����ֽ����,����Ӳ��","Not found,pls check hardware"},		//msg1
	{"Ӳ������,��Ͷ��","Check Ok,Pls insert bill"},					//msg2
	{"������,��ȡ��ֽ��","Stacker is full,Pls remove bill"},		//msg3
	{"ֽ��������,����Ӳ��","validator ERR,pls check hardware"},	//msg4
	{"��������","Bill Type is Disable"},							//msg5
	{"������ͨ��--%d,���--","OK:Type--%d;currency--"},				//msg6
	{"��<ȡ��>�˳�����","Press 'cancel' to exit test"}
};
/*********************************************************************************************************
** @Menu Define name:   TradeConfig
*********************************************************************************************************/
const TRADECONFIG TradeConfig =
{
	{"��Ӫά��->��������","Maint->Trade Config"},
	{"A.��������","A.Channel config"},
	{"B.ֽ���ݴ�����","B.Bill Escrow"},
	{"C.ֽ��ͨ������","C.Bill Type config"},
	{"D.Ӳ��ͨ������","D.Coin Type config"},
	{"E.���","E.Add Goods"},
	{"��<ȡ��>����","Press 'cancel' back"}
};
/*********************************************************************************************************
** @Menu Define name:   TradeConfigGC
*********************************************************************************************************/
const TRADECONFIG_GD TradeConfigGC =
{
	{"��Ӫά��->��������->����","Maint->Trade Config->GC"},
	{"������������:","Pls Enter GD NO:"},
	{"������������:%C","Pls Enter GD NO:%C"},
	{"������������:%C%d","Pls Enter GD NO:%C%d"},
	{"���û���%C%D:","Config GD %C%d:"},
	{"A.�Ƿ�������:%d","A.Enalble GC:%d"},
	{"B.�����������:%d","B.Set MAX Cap:%d"},
	{"C.���û����۸�:%d.%d","C.Set Price:%d.%d"},
	{"��<ȡ��>����","Press 'cancel' back"}
};
/*********************************************************************************************************
** @Menu Define name:   TradeConfigBillEscrow
*********************************************************************************************************/
const TRADECONFIG_BILL_ESCROW TradeConfigBillEscrow =
{
	{"��Ӫά��->��������->ֽ���ݴ�","Maint->Trade Config->Escrow"},
	{"ֽ���ݴ�����:","Bill Escrow list:"},
	{"A.ͨ��0:%d","A.Chl0:%d"},
	{"B.ͨ��1:%d","B.Chl1:%d"},
	{"C.ͨ��2:%d","C.Chl2:%d"},
	{"D.ͨ��3:%d","D.Chl3:%d"},
	{"E.ͨ��4:%d","E.Chl4:%d"},
	{"F.ͨ��5:%d","F.Chl5:%d"},
	{"��<ȡ��>����","Press 'cancel' back"}
};
/*********************************************************************************************************
** @Menu Define name:   TradeConfigBillChannel
*********************************************************************************************************/
const TRADECONFIG_BILL_CHL TradeConfigBillChannel =
{
	{"��Ӫά��->��������->ֽ��ͨ��","Maint->Trade Config->Bill"},
	{"ֽ��ͨ�������б�:","Bill Chl EA list:"},
	{"A.ͨ��0: %d","A.Chl0: %d"},
	{"B.ͨ��1: %d","B.Chl1: %d"},
	{"C.ͨ��2: %d","C.Chl2: %d"},
	{"D.ͨ��3: %d","D.Chl3: %d"},
	{"E.ͨ��4: %d","E.Chl4: %d"},
	{"F.ͨ��5: %d","F.Chl5: %d"},
	{"��<ȡ��>����","Press 'cancel' back"}
};
/*********************************************************************************************************
** @Menu Define name:   TradeConfigCoinChannel
*********************************************************************************************************/
const TRADECONFIG_COIN_CHL TradeConfigCoinChannel =
{
	{"��Ӫά��->��������->Ӳ��ͨ��","Maint->Trade Config->Coin"},
	{"Ӳ��ͨ�������б�:","Coin Chl EA list:"},
	{"A.ͨ��0:%d","A.Chl0:%d"},
	{"B.ͨ��1:%d","B.Chl2:%d"},
	{"C.ͨ��2:%d","C.Chl4:%d"},
	{"��<ȡ��>����","Press 'cancel' back"}
};
/*********************************************************************************************************
** @Menu Define name:   TradeConfigCoinChannel
*********************************************************************************************************/
const TRADECONFIG_ADD_GOODS TradeConfigAddGoods =
{
	{"��Ӫά��->��������->���","Maint->Trade Config->Add"},
	{"A.ȫ������","A.All"},
	{"B.��������","B.Layer"},
	{"C.�����������","C.Channel"},
	{"��<ȡ��>����","Press 'cancel' back"},
	//�Ӳ˵�
	{"ȫ����������?","Add All?"},
	{"��������:","Enter layer:"},
	{"��������:%C","Enter layer:%C"},
	{"ȷ������%C��?","Add layer %C full?"},
	{"�����������:","Enter layer:"},
	{"�����������:%C","Enter layer:%C"},
	{"�����������:%C%d","Enter layer:%C%d"},
	{"���������%C%d����:","Enter<%C%d>Numb:"},
	{"���������%C%d����:%d","Enter<%C%d>Numb:%d"},
	{"�Ѱ�ϵͳ�趨ֵ����","Set as system def."},
	{"����ɹ�","Set Ok"},
	{"A.ȡ��  B.ȷ��","A.Cancel  B.OK"},
	{"��ǰ����%C%d����:%d","current<%C%d>Numb:%d"}
};
/*********************************************************************************************************
** @Menu Define name:   TradeConfig
*********************************************************************************************************/
const TRADELOG TradeLog =
{
	{"��Ӫά��->���׼�¼","Maint->Trade Log"},
	{"1.Ӳ��-����:%d.%d,֧��:%d.%d","1.Coin Payin:%d.%d,out:%d.%d"},
	{"2.ֽ��-����:%d.%d,֧��:%d.%d","2.Bill Payin:%d.%d,out:%d.%d"},
	{"3.���ֽ�����:%d.%d","3.Cashless Payin:%d.%d"},
	{"4.������:%d.%d,��֧��:%d.%d","4.Total IN:%d.%d,OUT:%d.%d"},
	{"5.���״���-�ɹ�:%d,ʧ��:%d","5.TradeNum-Ok:%d,Err:%d"},
	{"��<ȡ��>����    ��<ȷ��>���","Press 'cancel' back      'enter'clean"}
};
/**************************************End Of File*******************************************************/
