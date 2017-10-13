#ifndef __MARKET_DATA_STRUCT_H__
#define __MARKET_DATA_STRUCT_H__


/**
	本模块分两个逻辑层：
		a)	数据差值化压缩层		,这一层的编码策略通过xml配置文件的方式，做成通用逻辑
		b)	protobuf数据编码层	,这一层因为protobuf的cpp文件需要对每个message进行生成，所以做成传入策略对象(IPbOperation)的方式
 */


/**
 * @class		tagFrameHeader

 * @brief		大包的包头(不进行压缩)
				&&
				包含的字段信息 = 包文信息 + 市场行情筛选信息
				&&
				Head[tagFrameHeader] + Body[ (msgid + 压缩Message) + (msgid + 压缩Message) + (msgid + 压缩Message) + ... ]

 * @note		msgid的类型定义为unsigned short
 */
typedef struct
{
	unsigned char			cMarketID;			///< 市场ID
	unsigned char			cPackageType;		///< 数据包类型		[0:静态数据	1:行情数据]
	unsigned int			nPackageSize;		///< 大包的body部分长度
	unsigned short			nPackSerialNo;		///< 包自增序号
} tagFrameHeader;


///< 以下为需要进行差值压缩，和protobuf压缩的类型


/**
 * @class		tagMarketInfoHead
 * @brief		市场信息
 * @msgid		1000
 */
typedef struct
{
	unsigned short			MsgID;				///< 消息标识号
	unsigned int			nMarketDate;		///< 市场日期
	unsigned int			nMarketTime;		///< 市场时间
	unsigned char			cMarketStatus;		///< 市场状态[0表示初始化 1表示行情中]
	unsigned short			nWareCount;			///< 商品数量[包括指数和个股等所有的商品]
	unsigned char			nKindCount;			///< 类别数量
	unsigned short			nPeriods[4][2];		///< 交易时间段描述[最多支持4个时段，每个时段纪录开始和结束时间，精确到分钟，HH * 60 + MM格式]
	unsigned char			nPeriodsCount;		///< 交易时间段
} tagMsgShL2MarketInfoHead;


/**
 * @class		tagMarketInfoDetail
 * @brief		市场分类信息
				市场信息 = tagMarketInfoHead | tagMarketInfoDetail | tagMarketInfoDetail | ...
				tagMarketInfoDetail的数量由tagMarketInfoHead::nWareCount指定
 * @msgid		1001
 */
typedef struct
{
	unsigned short			MsgID;				///< 消息标识号
	char					pszKindName[8];		///< 类别的名称
	unsigned char			nPriceRate;			///< 该类别中价格放大倍数[10的多少次方]
	unsigned short			nLotSize;			///< 该类别中"手"比率
	unsigned short			nWareCount;			///< 该类商品的数量
} tagMsgShL2MarketInfoDetail;


/**
 * @class		tagMsgShNameTable
 * @brief		上海码表信息
 * @msgid		1002
 */
typedef struct
{
	unsigned short			MsgID;				///< 消息标识号
	char					pszCode[6];			///< 商品代码
	unsigned char			cType;				///< 该商品类别[对应MarketInfo中的类别]
	char					pszName[8];			///< 商品名称
	char					pszPreName[4];		///< 深交所前缀[仅深圳市场，上海市场保留]
} tagMsgShL2NameTable;


typedef struct
{
	unsigned int			nPrice;				///< 委托价格[* 放大倍数]
	unsigned __int64		nVolume;			///< 委托量[股]
} tagBuySell;


/**
 * @class		tagMsgStockData10
 * @brief		上海快照信息
 * @msgid		1003
 */
typedef struct
{
	unsigned short			MsgID;				///< 消息标识号
	char					pszCode[6];			///< 商品代码
	unsigned int			nHigh;				///< 最高价格[* 放大倍数]
	unsigned int			nOpen;				///< 开盘价格[* 放大倍数]
	unsigned int			nLow;				///< 最低价格[* 放大倍数]
	unsigned int			nNow;				///< 最新价格[* 放大倍数]
	double					dAmount;			///< 总成交金额[元]
	unsigned __int64		nVolume;			///< 总成交量[股]
	unsigned int			nRecords;			///< 成交笔数
	unsigned int			nVBuyPrice;			///< 委买均价[* 放大倍数]
	unsigned __int64		nVBuyVolume;		///< 委买总量[股]
	unsigned int			nVSellPrice;		///< 委卖均价[* 放大倍数]
	unsigned __int64		nVSellVolume;		///< 委卖总量[股]
	tagBuySell				lstBuy[10];			///< 买盘
	tagBuySell				lstSell[10];		///< 卖盘
	unsigned int			nVoip;				///< 基金模拟净值[放大1000倍][ETF、深圳LOF该值有效]
	unsigned int			nClose;				///< 昨收价格[* 放大倍数]
	unsigned int			nHighLimit;			///< 涨停价格[* 放大倍数]
	unsigned int			nLowLimit;			///< 跌停价格[* 放大倍数]
} tagMsgShL2StockData10;




#endif






