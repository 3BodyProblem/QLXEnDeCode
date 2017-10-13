#ifndef __MEMSTRUCTURE_H__
#define __MEMSTRUCTURE_H__


#include <assert.h>
#include "../../UnitTestStructDefine.h"


#pragma pack( push, 1 )
//市场信息头部
typedef struct												//市场信息
{
	unsigned char					marketid;				//市场编号[唯一标示一个市场，见数据字典]
	unsigned char					marketstatus;			//市场状态[见数据字典]
	unsigned short					warecount;				//本市场商品总数
	unsigned char					kindcount;				//本市场类别的总数
	unsigned char					timezoom;				//本市场所在时区[-12---+12]
	unsigned long					marketdate;				//本市场当前日期[YYYYMMDD格式]
	unsigned long					markettime;				//本市场当前时间[HHMMSS格式]
	unsigned long					checkcode;				//码表的校验码
} tagSHL2Mem_MarketHead;
//..........................................................................................................................
//市场信息类别
typedef struct
{
	char							kindname[8];			//类别名称
	unsigned char					kindattr;				//类别属性[见数据字典]
	unsigned short					pricerate;				//价格的放大倍数
} tagSHL2Mem_MarketDetail;
//..........................................................................................................................
//名称代码表
typedef struct
{
	unsigned char					type;					//该商品的类别[参见本市场信息]
	char							code[6];				//商品代码
	char							name[8];				//商品名称
	unsigned long					close;					//昨收盘价[*放大倍数]
} tagSHL2Mem_NameTable;
//..........................................................................................................................
//债券信息
typedef struct
{
	unsigned short					serial;					//商品序号[从指数开始，从0开始]
//	char							code[6];				//商品代码
	unsigned long					jxrq;					//上海（计息日期）深圳（起息日期）[格式YYYYMMDD]
	unsigned long					lxdq;					//到期日期
	unsigned long					yjlx;					//应计利息[放大10000倍]
	unsigned long					pmlx;					//票面利息[放大10000倍]
} tagSHL2Mem_Gzlx;
//..........................................................................................................................
//权证信息
typedef struct
{
	unsigned short					serial;					//商品序号[从指数开始，从0开始]
//	char							code[6];				//商品代码
	char  							xqcode[6];				//行权代码
	char							bdname[8];				//行权商品名称
	char  							bdcode[6];				//标的证券代码
	char							kind;					//权证类型['C'认购 'P'认沽]
	double  						blance;					//权证流通股数[股]
	double 							total;					//权证发行总量[股]
	char							jsmode;					//结算方式["C"表示现金结算方式，"S"表示证券给付结算方式]
	char							xqmode;					//行权方式['A'美式权证行权 'E'欧式权证行权 'B'百慕大式权证行权]
	unsigned long					jsprice;				//结算价格[*放大倍数]
	unsigned long					xqprice;				//行权价格[*放大倍数]
	unsigned long					xqscale;				//行权比例[放大10000倍]
	unsigned long					ondate;					//权证上市日期[YYYYMMDD格式]
	unsigned long					dedate;					//权证到期日期[YYYYMMDD格式]
	unsigned long					qondate;				//行权起始日期[YYYYMMDD格式]
	unsigned long					qdedate;				//行权截止日期(上海表示结算价使用日期)[YYYYMMDD格式]
} tagSHL2Mem_Qzxx;
//..........................................................................................................................
//指数数据
typedef struct
{
	unsigned short					serial;					//商品序号[从指数开始，从0开始]
	unsigned long					low;					//最低指数[*放大倍数]
	unsigned long					open;					//今开指数[*放大倍数]
	unsigned long					high;					//最高指数[*放大倍数]
	unsigned long					now;					//最新指数[*放大倍数]
	float							amount;					//总成交金额[元]
	double							volume;					//总成交量[股]
} tagSHL2Mem_IndexTable;
//..........................................................................................................................
//买卖盘信息
typedef struct
{
	unsigned long					price;					//委托价格[*放大倍数]
	double							volume;					//委托量[股]
} tagHqMem_Shl2BuySell;
//..........................................................................................................................

// 参见交易所NGTS文档的InstrumentStatus一节
enum {
    IS_NOTCHANGE = 0, // 和上次一样
    IS_TRADE,         // 正常交易
    IS_SUSP,          // 停牌
    IS_BETW,          // 禁止交易
    IS_UNKNOWN        // 未知，编程没考虑的状态 
};

//快照数据
typedef struct
{
	unsigned short					serial;					//商品序号[从指数开始，从0开始]
	unsigned long					open;					//开盘价格[*放大倍数]
	unsigned long					high;					//最高价格[*放大倍数]
	unsigned long					low;					//最低价格[*放大倍数]
	unsigned long					now;					//最新价格[*放大倍数]
	float							amount;					//总成交金额[元]
	double							volume;					//总成交量[股]
	unsigned long					records;				//总成交比数[比]
	tagHqMem_Shl2BuySell			buy[10];				//5档委买盘
	tagHqMem_Shl2BuySell			sell[10];				//5档委卖盘
	unsigned long					vbuyprice;				//委买加权平均价格[*放大倍数]
	double							vbuyvolume;				//委买总量[股]
	unsigned long					vsellprice;				//委卖加权平均价格[*放大倍数]
	double							vsellvolume;			//委卖总量[股]
	union
	{
		unsigned long				voip;					//基金模净[*1000倍，仅基金]
		char						reserved[4];			//保留
	};
} tagSHL2Mem_StockData;
//..........................................................................................................................
//逐比成交
typedef struct
{
	unsigned short					serial;					//商品序号[从指数开始，从0开始]
	unsigned long					tradeno;				//交易序号
	unsigned long					tradetime;				//交易时间[HHMMSS格式]
	unsigned long					price;					//成交价格[*放大倍数]
	double							volume;					//成交量
} tagSHL2Mem_TranData;
//..........................................................................................................................
//委托排名信息
typedef struct
{
	unsigned short					serial;					//商品序号
	double							vbuysellvol;			//委托总量[股]
	double							tradevolume;			//交易总量[股]
	float							tradeamount;			//交易总金额[元]
} tagHqMem_OrderRankDetail;
//..........................................................................................................................
typedef struct
{
	unsigned char					kindtype;				//商品类别
	unsigned char					side;					//买卖方向
	tagHqMem_OrderRankDetail		record[10];				//排名信息
} tagSHL2Mem_OrderRaData;
//..........................................................................................................................
//即时撤单排名
typedef struct
{
	unsigned short					serial;					//商品序号
	unsigned long					ordertime;				//委托时间[HHMMSS格式]
	unsigned long					orderprice;				//委托价格[*放大倍数]
	double							ordervolume;			//委托数量[股]
} tagHqMem_CancelRankDetail;
//..........................................................................................................................
typedef struct
{
	unsigned char					kindtype;				//商品类别
	unsigned char					side;					//买卖方向[0、买 1、卖]
	tagHqMem_CancelRankDetail		record[10];				//10档撤单信息
} tagSHL2Mem_CancelRaData;
//..........................................................................................................................
//累计撤单排名
typedef struct
{
	unsigned short					serial;					//商品序号
	double							totalvolume;			//撤单总数量[股]
} tagHqMem_TotalCancelRankDetail;
//..........................................................................................................................
typedef struct
{
	unsigned char					kindtype;				//商品类别
	unsigned char					side;					//买卖方向[0、买 1、卖]
	tagHqMem_TotalCancelRankDetail	record[10];				//10档撤单信息
} tagSHL2Mem_CancelRank;
//..........................................................................................................................
//委托队列
typedef struct
{
	unsigned short					serial;					//商品序号
	unsigned char					side;					//买卖方向
	unsigned long					price;					//价格[*放大倍数]
	unsigned short					volcount;				//委托比数
	double							volume[50];				//量
} tagHqMem_OrderQueue;
//..........................................................................................................................
typedef struct
{
	unsigned char					status;				//市场状态
	unsigned long					marketdate;			//市场日期
	unsigned long					markettime;			//市场时间
} tagHqMem_MarketOverview;
//..........................................................................................................................
typedef struct
{
	unsigned short					serial;				//商品序号
	unsigned long					virtualprice;		//虚拟价格
	double							virtualvolume;		//虚拟匹配量
	double							buyvirtualvol;		//买方虚拟匹配量
	double							sellvirtualvol;		//卖方虚拟匹配量
} tagSHL2Mem_VirtualPrice;

typedef struct
{
	unsigned __int64				Volume;					//分量数量
} tagCcComm_OrderVolDetail;

#define ORDER_QUEUE_COUNT 150
typedef struct
{
	unsigned long					Price;	
	unsigned short					Count;			//该档价格的委托笔数,应该等于Volcount			
	unsigned short					Volcount;		//委托笔数
	tagCcComm_OrderVolDetail		Record[ORDER_QUEUE_COUNT];	//最多200，add可能会暂时超过
} tagOrderPriceLevel;

typedef struct
{
	unsigned char					Count;
	tagOrderPriceLevel				Price[10];		//最多5，add可能会暂时超过
} tagOrderPrice;

typedef struct
{
	unsigned short					Serial;
	tagOrderPrice					BuyPrice;
	tagOrderPrice					SellPrice;
} tagShL2FastMem_OrderData;

typedef struct
{
	unsigned short					Serial;
	unsigned long					Close;
} tagShL2FastMem_CloseChg;

typedef struct
{
	unsigned short					serial;
	char								phaseCode[8];      ///< 产品实时阶段及标志(说明见附件表一)
} tagShL2FastMem_TdPhase;

#pragma pack( pop )

typedef struct
{
	unsigned char				type;
	unsigned long			    ulcode;
	unsigned short				aserial;
	unsigned short				serial;						//序号
	unsigned long				uplimit;
	unsigned long				lowlimit;
	unsigned long				reftime;
	unsigned long				tradeno;
    bool                        after_auction;              // 集合竞价是否已经完成
} tagQuickNameTableInfo;

struct PriceLevelItem {
    bool used_;
    tagHqMem_Shl2BuySell price_;
    tagOrderPriceLevel order_queue_;
};

class PriceLevel {
public:
    enum { COUNT = 20 };
    PriceLevel() {
        Reset();
        last_order_queue_.Count = 0;
    }
    void Reset() {
        for (int i = 0; i < COUNT; ++i) {
            index_[i] = -1;
            item_[i].used_ = false;
        }
    }
    int Erase(tagHqMem_Shl2BuySell &data) {
        for (int i = 0; i < COUNT; ++i) {
            PriceLevelItem *item = Get(i);
            if (item == NULL) {
                return -1;
            } else if (item->price_.price == data.price) {
                item->used_ = false;
                index_[i] = -1;
                for (int j = i; j < COUNT - 1; ++j) {
                    index_[j] = index_[j + 1];
                }
                index_[COUNT - 1] = -1;
                return i;
            }
        }
        return -1;
    }
    int Update(tagHqMem_Shl2BuySell &data) {
        for (int i = 0; i < COUNT; ++i) {
            PriceLevelItem *item = Get(i);
            if (item == NULL) {
                return -1;
            } else if (item->price_.price == data.price) {
                item->price_.volume = data.volume;
                return i;
            }
        }
        return -1;
    }
    int Add(tagHqMem_Shl2BuySell &data, bool buy) {
        for (int i = 0; i < COUNT; ++i) {
            PriceLevelItem *item = Get(i);
            if (item == NULL) {
                int free_index = GetFreeIndex();
                assert(free_index != -1);
                item_[free_index].used_ = true;
                item_[free_index].price_.price = data.price;
                item_[free_index].price_.volume = data.volume;
                index_[i] = free_index;
                return i;
            } else {
                bool found;
                if (buy) {
                    found = item->price_.price < data.price;
                } else {
                    found = item->price_.price > data.price;
                }
                if (found) {
                    if (index_[COUNT - 1] != -1) {
                        item_[index_[COUNT - 1]].used_ = false;
                        index_[COUNT - 1] = -1;
                    }
                    for (int j = COUNT - 2; j >= i; --j) {
                        index_[j + 1] = index_[j];
                    }
                    int free_index = GetFreeIndex();
                    assert(free_index != -1);
                    item_[free_index].used_ = true;
                    item_[free_index].price_.price = data.price;
                    item_[free_index].price_.volume = data.volume;
                    index_[i] = free_index;
                    return i;
                }
            }
        }
        return -1;
    }
    PriceLevelItem * Get(int index) {
        int i = index_[index];
        if (i == -1) {
            return NULL;
        }
        return &item_[i];
    }
    int GetFreeIndex() {
        for (int i = 0; i < COUNT; ++i) {
            if (item_[i].used_ == false) {
                memset(&item_[i].price_, 0, sizeof(item_[i].price_));
                memset(&item_[i].order_queue_, 0, sizeof(item_[i].order_queue_));
                return i;
            }
        }
        return -1;
    }

    bool IsOrderQueueChanged() {
        PriceLevelItem *item = Get(0);
        if (item == NULL) {
            return false;
        }
        if (last_order_queue_.Count == item->order_queue_.Count) {
            for (int i = 0; i < item->order_queue_.Count; ++i) {
                if (last_order_queue_.Record[i].Volume != item->order_queue_.Record[i].Volume) {
                    return true;
                }
            }
            return false;
        } else {
            return true;
        }
    }

    void SetOrderQueueNotChanged() {
        PriceLevelItem *item = Get(0);
        if (item == NULL) {
            return;
        }
        last_order_queue_.Count = item->order_queue_.Count;
        int c = item->order_queue_.Count;
        int max = sizeof last_order_queue_.Record / sizeof last_order_queue_.Record[0];
        if (c > max) {
            c = max;
        }
        for (int i = 0; i < c; ++i) {
            last_order_queue_.Record[i].Volume = item->order_queue_.Record[i].Volume;
        }
    }
private:
    tagOrderPriceLevel last_order_queue_;
    int index_[COUNT];
    PriceLevelItem item_[COUNT]; 
};

struct StockPriceLevel {
    StockPriceLevel() {
        memset(&last_stockdata_, 0, sizeof(last_stockdata_));
    }
    char	Code[6];
    tagDemoSnapType last_stockdata_;
    PriceLevel buy_;
    PriceLevel sell_;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator == ( const tagSHL2Mem_MarketDetail & a, const tagSHL2Mem_MarketDetail & b )
{
	return !memcmp( a.kindname, b.kindname, 8 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool  operator == ( const tagSHL2Mem_NameTable & a, const tagSHL2Mem_NameTable & b )
{
	if( !memcmp( a.code, b.code, 6 ) )
	{
		return true;
	}
	return false; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator == ( const tagSHL2Mem_StockData & a, const tagSHL2Mem_StockData & b )
{
	return a.serial == b.serial;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool  operator == ( const tagSHL2Mem_IndexTable & a, const tagSHL2Mem_IndexTable& b )
{
	return a.serial == b.serial;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool  operator == ( const tagSHL2Mem_Gzlx & a, const tagSHL2Mem_Gzlx & b )
{
	return a.serial == b.serial;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool  operator == ( const tagSHL2Mem_Qzxx & a, const tagSHL2Mem_Qzxx & b )
{
	return a.serial == b.serial;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool operator == ( const tagSHL2Mem_TranData & a, const tagSHL2Mem_TranData & b )
{
	return a.serial == b.serial;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool  operator == ( const tagSHL2Mem_VirtualPrice & a, const tagSHL2Mem_VirtualPrice & b )
{
	return a.serial == b.serial;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool  operator == ( const tagHqMem_OrderQueue & a, const tagHqMem_OrderQueue& b )
{
	return a.serial == b.serial;
}

inline bool  operator == ( const StockPriceLevel & a, const StockPriceLevel& b )
{
	if( ( *((unsigned long *)a.Code)==*((unsigned long *)b.Code) && *((unsigned short *)(a.Code+4))==*((unsigned short *)(b.Code+4)) ) )
	{
		return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool  operator == ( const tagShL2FastMem_OrderData & a, const tagShL2FastMem_OrderData & b )
{
	return a.Serial == b.Serial;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool  operator == ( const tagQuickNameTableInfo & a, const tagQuickNameTableInfo & b )
{
	return a.ulcode == b.ulcode;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline bool  operator == ( const tagShL2FastMem_TdPhase & a, const tagShL2FastMem_TdPhase& b )
{
	return a.serial == b.serial;
}



#endif//__MEMSTRUCTURE_H__