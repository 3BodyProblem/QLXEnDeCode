#ifndef __MARKET_DATA_STRUCT_H__
#define __MARKET_DATA_STRUCT_H__


/**
	��ģ��������߼��㣺
		a)	���ݲ�ֵ��ѹ����		,��һ��ı������ͨ��xml�����ļ��ķ�ʽ������ͨ���߼�
		b)	protobuf���ݱ����	,��һ����Ϊprotobuf��cpp�ļ���Ҫ��ÿ��message�������ɣ��������ɴ�����Զ���(IPbOperation)�ķ�ʽ
 */


/**
 * @class		tagFrameHeader

 * @brief		����İ�ͷ(������ѹ��)
				&&
				�������ֶ���Ϣ = ������Ϣ + �г�����ɸѡ��Ϣ
				&&
				Head[tagFrameHeader] + Body[ (msgid + ѹ��Message) + (msgid + ѹ��Message) + (msgid + ѹ��Message) + ... ]

 * @note		msgid�����Ͷ���Ϊunsigned short
 */
typedef struct
{
	unsigned char			cMarketID;			///< �г�ID
	unsigned char			cPackageType;		///< ���ݰ�����		[0:��̬����	1:��������]
	unsigned int			nPackageSize;		///< �����body���ֳ���
	unsigned short			nPackSerialNo;		///< ���������
} tagFrameHeader;


///< ����Ϊ��Ҫ���в�ֵѹ������protobufѹ��������


/**
 * @class		tagMarketInfoHead
 * @brief		�г���Ϣ
 * @msgid		1000
 */
typedef struct
{
	unsigned short			MsgID;				///< ��Ϣ��ʶ��
	unsigned int			nMarketDate;		///< �г�����
	unsigned int			nMarketTime;		///< �г�ʱ��
	unsigned char			cMarketStatus;		///< �г�״̬[0��ʾ��ʼ�� 1��ʾ������]
	unsigned short			nWareCount;			///< ��Ʒ����[����ָ���͸��ɵ����е���Ʒ]
	unsigned char			nKindCount;			///< �������
	unsigned short			nPeriods[4][2];		///< ����ʱ�������[���֧��4��ʱ�Σ�ÿ��ʱ�μ�¼��ʼ�ͽ���ʱ�䣬��ȷ�����ӣ�HH * 60 + MM��ʽ]
	unsigned char			nPeriodsCount;		///< ����ʱ���
} tagMsgShL2MarketInfoHead;


/**
 * @class		tagMarketInfoDetail
 * @brief		�г�������Ϣ
				�г���Ϣ = tagMarketInfoHead | tagMarketInfoDetail | tagMarketInfoDetail | ...
				tagMarketInfoDetail��������tagMarketInfoHead::nWareCountָ��
 * @msgid		1001
 */
typedef struct
{
	unsigned short			MsgID;				///< ��Ϣ��ʶ��
	char					pszKindName[8];		///< ��������
	unsigned char			nPriceRate;			///< ������м۸�Ŵ���[10�Ķ��ٴη�]
	unsigned short			nLotSize;			///< �������"��"����
	unsigned short			nWareCount;			///< ������Ʒ������
} tagMsgShL2MarketInfoDetail;


/**
 * @class		tagMsgShNameTable
 * @brief		�Ϻ������Ϣ
 * @msgid		1002
 */
typedef struct
{
	unsigned short			MsgID;				///< ��Ϣ��ʶ��
	char					pszCode[6];			///< ��Ʒ����
	unsigned char			cType;				///< ����Ʒ���[��ӦMarketInfo�е����]
	char					pszName[8];			///< ��Ʒ����
	char					pszPreName[4];		///< ���ǰ׺[�������г����Ϻ��г�����]
} tagMsgShL2NameTable;


typedef struct
{
	unsigned int			nPrice;				///< ί�м۸�[* �Ŵ���]
	unsigned __int64		nVolume;			///< ί����[��]
} tagBuySell;


/**
 * @class		tagMsgStockData10
 * @brief		�Ϻ�������Ϣ
 * @msgid		1003
 */
typedef struct
{
	unsigned short			MsgID;				///< ��Ϣ��ʶ��
	char					pszCode[6];			///< ��Ʒ����
	unsigned int			nHigh;				///< ��߼۸�[* �Ŵ���]
	unsigned int			nOpen;				///< ���̼۸�[* �Ŵ���]
	unsigned int			nLow;				///< ��ͼ۸�[* �Ŵ���]
	unsigned int			nNow;				///< ���¼۸�[* �Ŵ���]
	double					dAmount;			///< �ܳɽ����[Ԫ]
	unsigned __int64		nVolume;			///< �ܳɽ���[��]
	unsigned int			nRecords;			///< �ɽ�����
	unsigned int			nVBuyPrice;			///< ί�����[* �Ŵ���]
	unsigned __int64		nVBuyVolume;		///< ί������[��]
	unsigned int			nVSellPrice;		///< ί������[* �Ŵ���]
	unsigned __int64		nVSellVolume;		///< ί������[��]
	tagBuySell				lstBuy[10];			///< ����
	tagBuySell				lstSell[10];		///< ����
	unsigned int			nVoip;				///< ����ģ�⾻ֵ[�Ŵ�1000��][ETF������LOF��ֵ��Ч]
	unsigned int			nClose;				///< ���ռ۸�[* �Ŵ���]
	unsigned int			nHighLimit;			///< ��ͣ�۸�[* �Ŵ���]
	unsigned int			nLowLimit;			///< ��ͣ�۸�[* �Ŵ���]
} tagMsgShL2StockData10;




#endif






