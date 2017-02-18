/****************************************Copyright (c)***************************************************
 **                      Fuzhou LinkMac Information Technology Co.,Ltd.
 **                               http://www.linkmac.com.cn
 **-------------------------------------File Info--------------------------------------------------------
 ** @Filename:			ParamDefine.h
 ** @brief:				The original version  
 ** @CreatedBy:			sunway 
 ** @CreatedDate:		2016-07-01
 ** @Version:			V1.0      
 *******************************************************************************************************/
#ifndef _PARADEF_H
#define _PARADEF_H
/********************************************************************************************************
** @Define name:       APP Debug
********************************************************************************************************/
//#define  APP_DBG_MDB_BILL
//#define  APP_DBG_MDB_COIN
//#define  APP_DBG_FRAM
/********************************************************************************************************
** @Define name:       Vending Machine Parameter Define
********************************************************************************************************/
#define SOFTWAREVERSION		" V1.00"
#define MAX_MOTOR_ROW		6									//�������
#define MAX_MOTOR_COL		8									//�������
#define MAXARRAY			(MAX_MOTOR_ROW * MAX_MOTOR_COL)		//����������
#define VMC_MODE_VEND		0x01								//����ģʽ
#define VMC_MODE_MAIN		0x02								//ά��ģʽ
#define VMC_FRAM_ADDR		0xA0								//FRAM address
#define VMC_PARA_PAGE_S		0									//VMCParam start page
#define VMC_TDPA_PAGE_S		256									//TradeParam start page
#define VMC_TLOG_PAGE_S		512									//TotalTradeLog start page
#define VMC_SLOG_PAGE_S		1024								//TotalTradeLog start page
#define VMC_SLOG_PAGE_E		4096								//TotalTradeLog End page
/********************************************************************************************************
** @Define name:       VMCParam Parameter Define
********************************************************************************************************/
typedef struct
{
	unsigned char GoodsChannelArray[MAXARRAY];
	unsigned char GoodsMaxCapacity[MAXARRAY];
	unsigned char VMCID[7];
	unsigned char Password[6];
	unsigned char Language;
	unsigned char MdbBillDeviceEAB;
	unsigned char MdbCoinDeviceEAB;
	unsigned char MdbCashlessDeviceEAB;
	unsigned char Rtc[8];
	unsigned char VMCMode;
} VMCPARAM;
extern volatile VMCPARAM VMCParam;
/********************************************************************************************************
** @Define name:       Trade Parameter Define
********************************************************************************************************/
typedef struct
{
	unsigned short GoodsPrice[MAXARRAY];
	unsigned char  RemainGoods[MAXARRAY];
	unsigned short TypeBillEscrow;			//�ݴ�ͨ��
	unsigned short TypeBillEnable;			//ֽ��ͨ��ʹ��
	unsigned short TypeCoinEnable;			//Ӳ��ͨ��ʹ��
	unsigned char  ServiceWhenNoChanger;
	unsigned char  ServiceWhenBillStackerIsFull;
} TRADEPARAM;
extern volatile TRADEPARAM TradeParam;
typedef struct
{
	unsigned int TotalCoinPayIn;
	unsigned int TotalCoinPayout;
	unsigned int TotalBillPayIn;
	unsigned int TotalBillPayOut;
	unsigned int TotalCashlessPayIn;
	unsigned int TotalVendNumber;
	unsigned int TotalSuccesNumber;
	unsigned int TotalErrorNumber;
	unsigned int SingleTradeLogPointer;
} TOTALTRADELOG;
extern volatile TOTALTRADELOG TotalTradeLog;
typedef struct
{
	unsigned char Time[7];
	unsigned char Err;
	unsigned int  CoinPayIn;
	unsigned int  BillPayIn;
	unsigned int  CoinPayout;
	unsigned char Channel;
} SINGLETRADELOG;
extern volatile SINGLETRADELOG SingleTradeLog;
/********************************************************************************************************
** @Define name:       Error Code Parameter Define
********************************************************************************************************/
typedef struct
{
	unsigned char FramErr;
	unsigned char TradeParamErr;
	unsigned char MdbErr;
	unsigned char MotorErr;
} SYSTEMERROR;
extern volatile SYSTEMERROR SysError;
/********************************************************************************************************
** @Define name:       MDB Device Parameter Define
********************************************************************************************************/
typedef struct
{
	unsigned char Oneline;
	unsigned char ManufacturerCode[3];
	unsigned char Level;
	unsigned char CurrencyCode[2];
	unsigned char ScalingFactor;
	unsigned char Decimal;
	unsigned char CoinTypeRouting[2];
	unsigned char CoinTypeCredit[16];
	unsigned char CoinTypeTubeStatus[16];
	unsigned char CoinTypePresentInTube[16];
	unsigned char Feature[4];
} MDBCOINACCEPTORCHANGER;
extern volatile MDBCOINACCEPTORCHANGER MDBCoinDevice;
typedef struct
{
	unsigned char Oneline;
	unsigned char ManufacturerCode[3];
	unsigned char Level;
	unsigned char CurrencyCode[2];
	unsigned int  ScalingFactor;
	unsigned char Decimal;
	unsigned int  StackerCapacity;
	unsigned char BillSecurityLevel[2];
	unsigned char Escrow;
	unsigned char BillTypeEscrowEnable[15];
	unsigned int  BillTypeCredit[15];
	unsigned char BillRecycSupport;
	unsigned char Level2Feature[4];
	unsigned char StackerStatus;
	unsigned int  BillInStacker;
} MDBBILLVALIDATORRECYCLER;
extern volatile MDBBILLVALIDATORRECYCLER MDBBillDevice;


/******************************************ֽ����Ӳ����ͨѶ�ṹ��*****************************************************/
//ֽ�����յ�ֽ�ҷ�����ֵ
#define MBOX_BILLMONEY					0x01
//����ֽ��������
#define MBOX_BILLDISABLEDEV				0x02
//ʹ��ֽ��������
#define MBOX_BILLENABLEDEV				0x03
//����ֽ��������
#define MBOX_BILLRESETDEV				0x04
//����ֽ��������
#define MBOX_BILLINITDEV				0x05
//ֽ����ѹ������
#define MBOX_BILLESCROW					0x06
//����ֽ����ѹ���ɹ�����
#define MBOX_BILLESCROWSUCC				0x07
//����ֽ����ѹ��ʧ������
#define MBOX_BILLESCROWFAIL				0x08
//ֽ�����˱�����
#define MBOX_BILLRETURN					0x09
//����ֽ�����˱ҳɹ�����
#define MBOX_BILLRETURNSUCC				0x0A
//����ֽ�����˱�ʧ������
#define MBOX_BILLRETURNFAIL				0x0B
//����ֽ������������
#define MBOX_BILLSETUPDEV				0x0C


//Ӳ�����յ�Ӳ�ҷ�����ֵ
#define MBOX_COINMONEY					0x0D
//����Ӳ��������
#define MBOX_COINDISABLEDEV				0x0E
//ʹ��Ӳ��������
#define MBOX_COINENABLEDEV				0x0F
//����Ӳ��������
#define MBOX_COININITDEV				0x10
//����Ӳ���������˱Ҽ��˱���������
#define MBOX_COINRETURN					0x11
//����Ӳ������������
#define MBOX_COINSETUPDEV				0x12


//����level3��mdb��������������
#define MBOX_MDBCHANGELEVEL3			0x13
//����level2��mdb��������������
#define MBOX_MDBCHANGELEVEL2			0x14
//���ڵõ������������
#define MBOX_MDBCHANGETUBE				0x15
//����hopper����������������
#define MBOX_MDBCHANGEHOPPER			0x16
//���������㷵������
#define MBOX_CHANGERBACKMONEY			0x17

typedef struct	
{
	unsigned char Communicate;			//1����ֽ����ͨѶ����
	unsigned char moto;					//1����������
	unsigned char sensor;				//1������������
	unsigned char romchk;				//1����rom����
	unsigned char jam;					//1����Ͷ�ҿ���
	unsigned char removeCash;			//1�Ƴ�ֽ�ҳ���
	unsigned char cashErr;				//1ֽ�ҳ�����ϣ��Լ�������
	unsigned char disable;				//1ֽ����Ϊ����ԭ�򱻽�����
	unsigned char recyErr;				//����ģ���쳣
}MDBBILLERROR;
extern MDBBILLERROR MdbBillErr;

typedef struct	
{
	unsigned char Communicate;			//1����Ӳ����ͨѶ����
	unsigned char sensor;				//1������������
	unsigned char tubejam;				//1������ҿڿ���
	unsigned char romchk;				//1����rom����
	unsigned char routing;				//1�������ͨ������
	unsigned char jam;					//1����Ͷ�ҿ���	
	unsigned char removeTube;			//1Ӳ�Ҷ��ƿ�
	//ʹ����չָ����
	unsigned char disable;				//1Ӳ����Ϊ����ԭ��vmc������
	unsigned char unknowError;			//��0Ӳ���������ֹ���
	unsigned char unknowErrorLow;		//Ӳ���������ֹ��ϵ���״̬��
	
}MDBCOINERROR;
extern MDBCOINERROR MdbCoinErr;

/********************************************************************************************************
** @Define name:       RTC Parameter Define
********************************************************************************************************/
extern volatile unsigned char Rtc[7];
/********************************************************************************************************
** @Define name:       CrcCheck Fuction
********************************************************************************************************/
extern void InitSysErrCode(void);
extern void InitVmcParam(void);
extern void LoadDefaultVmcParam(void);
extern void LoadNewVmcParam(void);
extern void InitTradeParam(void);
extern void LoadDefaultTradeParam(void);
extern void LoadNewTradeParam(void);
extern void InitTotalLog(void);
extern void LoadNewTotalLog(void);
extern void InitSingleLog(void);
extern void LoadNewSingleLog(void);
extern unsigned short CrcCheck(unsigned char *data,unsigned int len);
char *PrintfMoney(unsigned int dispnum);
#endif
/**************************************End Of File*******************************************************/
