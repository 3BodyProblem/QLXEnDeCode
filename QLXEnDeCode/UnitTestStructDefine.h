#ifndef __UNIT_TEST_STRUCT_DEFINE_H__
#define __UNIT_TEST_STRUCT_DEFINE_H__


#pragma pack(1)


///< ---------------- DEMO ���Խṹ���� ------------------------------

///< msgid=1
typedef struct
{
	char							Status;					//�г�״̬
	unsigned int					Date;					//�г�����
	unsigned int					Time;					//�г�ʱ��
} tagDemoMarketStatus;

///< msgid=10
typedef struct
{
	char							Code[6];
	unsigned int					Type;
	char							Name[8];
} tagDemoStaticType;

typedef struct
{
	unsigned int					Price;					//ί�м۸�[* �Ŵ���]
	unsigned __int64				Volume;					//ί����[��]
} tagDemoBuySellType;

///< msgid=13
typedef struct
{
	char							Code[6];				//��Ʒ����<2009-1-21 Lumy����>
	unsigned int					Now;					//���¼۸�[* �Ŵ���]
	unsigned int					High;					//��߼۸�[* �Ŵ���]
	unsigned int					Low;					//��ͼ۸�[* �Ŵ���]
	unsigned int					Open;					//���̼۸�[* �Ŵ���]
	unsigned int					Close;					//���ռ۸�[* �Ŵ���]
	unsigned int					Voip;					//����ģ�⾻ֵ[�Ŵ�1000��][ETF������LOF��ֵ��Ч]
	double							Amount;					//�ܳɽ����[Ԫ]
	unsigned __int64				Volume;					//�ܳɽ���[��]
	unsigned int					Records;				//�ɽ�����
	tagDemoBuySellType				Buy[10];				//����
	tagDemoBuySellType				Sell[10];				//����
} tagDemoSnapType;

///< msgid=14
typedef struct
{
	char							Code[6];				//��Ʒ����<2009-1-21 Lumy����>
	unsigned int					TradeNo;				//�������
	unsigned int					TradeTime;				//����ʱ��[HHMMSS��ʽ]
	unsigned int					Price;					//�ɽ��۸�[*�Ŵ���]
	double							Volume;					//�ɽ���
} tagDemoTranData;

///< msgid=15
typedef struct
{
	char							Code[6];				//��Ʒ����<2009-1-21 Lumy����>
	unsigned int					Low;					//���ָ��[*�Ŵ���]
	unsigned int					Open;					//��ָ��[*�Ŵ���]
	unsigned int					High;					//���ָ��[*�Ŵ���]
	unsigned int					Now;					//����ָ��[*�Ŵ���]
	double							Amount;					//�ܳɽ����[Ԫ]
	double							Volume;					//�ܳɽ���[��]
} tagDemoIndexType;

///< msgid=16
typedef struct
{
	char							Code[6];				//��Ʒ����<2009-1-21 Lumy����>
	unsigned long					VirtualPrice;			//����۸�
	double							VirtualVolume;			//����ƥ����
	double							BuyVirtualVol;			//������ƥ����
	double							SellVirtualVol;			//��������ƥ����
} tagDemoVirtualPriceType;

#pragma pack()



#endif






