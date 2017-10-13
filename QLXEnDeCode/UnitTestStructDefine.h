#ifndef __UNIT_TEST_STRUCT_DEFINE_H__
#define __UNIT_TEST_STRUCT_DEFINE_H__


#pragma pack(1)


///< ---------------- DEMO 测试结构定义 ------------------------------

///< msgid=1
typedef struct
{
	char							Status;					//市场状态
	unsigned int					Date;					//市场日期
	unsigned int					Time;					//市场时间
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
	unsigned int					Price;					//委托价格[* 放大倍数]
	unsigned __int64				Volume;					//委托量[股]
} tagDemoBuySellType;

///< msgid=13
typedef struct
{
	char							Code[6];				//商品代码<2009-1-21 Lumy更改>
	unsigned int					Now;					//最新价格[* 放大倍数]
	unsigned int					High;					//最高价格[* 放大倍数]
	unsigned int					Low;					//最低价格[* 放大倍数]
	unsigned int					Open;					//开盘价格[* 放大倍数]
	unsigned int					Close;					//昨收价格[* 放大倍数]
	unsigned int					Voip;					//基金模拟净值[放大1000倍][ETF、深圳LOF该值有效]
	double							Amount;					//总成交金额[元]
	unsigned __int64				Volume;					//总成交量[股]
	unsigned int					Records;				//成交笔数
	tagDemoBuySellType				Buy[10];				//买盘
	tagDemoBuySellType				Sell[10];				//卖盘
} tagDemoSnapType;

///< msgid=14
typedef struct
{
	char							Code[6];				//商品代码<2009-1-21 Lumy更改>
	unsigned int					TradeNo;				//交易序号
	unsigned int					TradeTime;				//交易时间[HHMMSS格式]
	unsigned int					Price;					//成交价格[*放大倍数]
	double							Volume;					//成交量
} tagDemoTranData;

///< msgid=15
typedef struct
{
	char							Code[6];				//商品代码<2009-1-21 Lumy更改>
	unsigned int					Low;					//最低指数[*放大倍数]
	unsigned int					Open;					//今开指数[*放大倍数]
	unsigned int					High;					//最高指数[*放大倍数]
	unsigned int					Now;					//最新指数[*放大倍数]
	double							Amount;					//总成交金额[元]
	double							Volume;					//总成交量[股]
} tagDemoIndexType;

///< msgid=16
typedef struct
{
	char							Code[6];				//商品代码<2009-1-21 Lumy更改>
	unsigned long					VirtualPrice;			//虚拟价格
	double							VirtualVolume;			//虚拟匹配量
	double							BuyVirtualVol;			//买方虚拟匹配量
	double							SellVirtualVol;			//卖方虚拟匹配量
} tagDemoVirtualPriceType;

#pragma pack()



#endif






