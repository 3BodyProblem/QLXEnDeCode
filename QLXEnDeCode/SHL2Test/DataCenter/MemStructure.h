#ifndef __MEMSTRUCTURE_H__
#define __MEMSTRUCTURE_H__


#include <assert.h>
#include "../../UnitTestStructDefine.h"


#pragma pack( push, 1 )
//�г���Ϣͷ��
typedef struct												//�г���Ϣ
{
	unsigned char					marketid;				//�г����[Ψһ��ʾһ���г����������ֵ�]
	unsigned char					marketstatus;			//�г�״̬[�������ֵ�]
	unsigned short					warecount;				//���г���Ʒ����
	unsigned char					kindcount;				//���г���������
	unsigned char					timezoom;				//���г�����ʱ��[-12---+12]
	unsigned long					marketdate;				//���г���ǰ����[YYYYMMDD��ʽ]
	unsigned long					markettime;				//���г���ǰʱ��[HHMMSS��ʽ]
	unsigned long					checkcode;				//����У����
} tagSHL2Mem_MarketHead;
//..........................................................................................................................
//�г���Ϣ���
typedef struct
{
	char							kindname[8];			//�������
	unsigned char					kindattr;				//�������[�������ֵ�]
	unsigned short					pricerate;				//�۸�ķŴ���
} tagSHL2Mem_MarketDetail;
//..........................................................................................................................
//���ƴ����
typedef struct
{
	unsigned char					type;					//����Ʒ�����[�μ����г���Ϣ]
	char							code[6];				//��Ʒ����
	char							name[8];				//��Ʒ����
	unsigned long					close;					//�����̼�[*�Ŵ���]
} tagSHL2Mem_NameTable;
//..........................................................................................................................
//ծȯ��Ϣ
typedef struct
{
	unsigned short					serial;					//��Ʒ���[��ָ����ʼ����0��ʼ]
//	char							code[6];				//��Ʒ����
	unsigned long					jxrq;					//�Ϻ�����Ϣ���ڣ����ڣ���Ϣ���ڣ�[��ʽYYYYMMDD]
	unsigned long					lxdq;					//��������
	unsigned long					yjlx;					//Ӧ����Ϣ[�Ŵ�10000��]
	unsigned long					pmlx;					//Ʊ����Ϣ[�Ŵ�10000��]
} tagSHL2Mem_Gzlx;
//..........................................................................................................................
//Ȩ֤��Ϣ
typedef struct
{
	unsigned short					serial;					//��Ʒ���[��ָ����ʼ����0��ʼ]
//	char							code[6];				//��Ʒ����
	char  							xqcode[6];				//��Ȩ����
	char							bdname[8];				//��Ȩ��Ʒ����
	char  							bdcode[6];				//���֤ȯ����
	char							kind;					//Ȩ֤����['C'�Ϲ� 'P'�Ϲ�]
	double  						blance;					//Ȩ֤��ͨ����[��]
	double 							total;					//Ȩ֤��������[��]
	char							jsmode;					//���㷽ʽ["C"��ʾ�ֽ���㷽ʽ��"S"��ʾ֤ȯ�������㷽ʽ]
	char							xqmode;					//��Ȩ��ʽ['A'��ʽȨ֤��Ȩ 'E'ŷʽȨ֤��Ȩ 'B'��Ľ��ʽȨ֤��Ȩ]
	unsigned long					jsprice;				//����۸�[*�Ŵ���]
	unsigned long					xqprice;				//��Ȩ�۸�[*�Ŵ���]
	unsigned long					xqscale;				//��Ȩ����[�Ŵ�10000��]
	unsigned long					ondate;					//Ȩ֤��������[YYYYMMDD��ʽ]
	unsigned long					dedate;					//Ȩ֤��������[YYYYMMDD��ʽ]
	unsigned long					qondate;				//��Ȩ��ʼ����[YYYYMMDD��ʽ]
	unsigned long					qdedate;				//��Ȩ��ֹ����(�Ϻ���ʾ�����ʹ������)[YYYYMMDD��ʽ]
} tagSHL2Mem_Qzxx;
//..........................................................................................................................
//ָ������
typedef struct
{
	unsigned short					serial;					//��Ʒ���[��ָ����ʼ����0��ʼ]
	unsigned long					low;					//���ָ��[*�Ŵ���]
	unsigned long					open;					//��ָ��[*�Ŵ���]
	unsigned long					high;					//���ָ��[*�Ŵ���]
	unsigned long					now;					//����ָ��[*�Ŵ���]
	float							amount;					//�ܳɽ����[Ԫ]
	double							volume;					//�ܳɽ���[��]
} tagSHL2Mem_IndexTable;
//..........................................................................................................................
//��������Ϣ
typedef struct
{
	unsigned long					price;					//ί�м۸�[*�Ŵ���]
	double							volume;					//ί����[��]
} tagHqMem_Shl2BuySell;
//..........................................................................................................................

// �μ�������NGTS�ĵ���InstrumentStatusһ��
enum {
    IS_NOTCHANGE = 0, // ���ϴ�һ��
    IS_TRADE,         // ��������
    IS_SUSP,          // ͣ��
    IS_BETW,          // ��ֹ����
    IS_UNKNOWN        // δ֪�����û���ǵ�״̬ 
};

//��������
typedef struct
{
	unsigned short					serial;					//��Ʒ���[��ָ����ʼ����0��ʼ]
	unsigned long					open;					//���̼۸�[*�Ŵ���]
	unsigned long					high;					//��߼۸�[*�Ŵ���]
	unsigned long					low;					//��ͼ۸�[*�Ŵ���]
	unsigned long					now;					//���¼۸�[*�Ŵ���]
	float							amount;					//�ܳɽ����[Ԫ]
	double							volume;					//�ܳɽ���[��]
	unsigned long					records;				//�ܳɽ�����[��]
	tagHqMem_Shl2BuySell			buy[10];				//5��ί����
	tagHqMem_Shl2BuySell			sell[10];				//5��ί����
	unsigned long					vbuyprice;				//ί���Ȩƽ���۸�[*�Ŵ���]
	double							vbuyvolume;				//ί������[��]
	unsigned long					vsellprice;				//ί����Ȩƽ���۸�[*�Ŵ���]
	double							vsellvolume;			//ί������[��]
	union
	{
		unsigned long				voip;					//����ģ��[*1000����������]
		char						reserved[4];			//����
	};
} tagSHL2Mem_StockData;
//..........................................................................................................................
//��ȳɽ�
typedef struct
{
	unsigned short					serial;					//��Ʒ���[��ָ����ʼ����0��ʼ]
	unsigned long					tradeno;				//�������
	unsigned long					tradetime;				//����ʱ��[HHMMSS��ʽ]
	unsigned long					price;					//�ɽ��۸�[*�Ŵ���]
	double							volume;					//�ɽ���
} tagSHL2Mem_TranData;
//..........................................................................................................................
//ί��������Ϣ
typedef struct
{
	unsigned short					serial;					//��Ʒ���
	double							vbuysellvol;			//ί������[��]
	double							tradevolume;			//��������[��]
	float							tradeamount;			//�����ܽ��[Ԫ]
} tagHqMem_OrderRankDetail;
//..........................................................................................................................
typedef struct
{
	unsigned char					kindtype;				//��Ʒ���
	unsigned char					side;					//��������
	tagHqMem_OrderRankDetail		record[10];				//������Ϣ
} tagSHL2Mem_OrderRaData;
//..........................................................................................................................
//��ʱ��������
typedef struct
{
	unsigned short					serial;					//��Ʒ���
	unsigned long					ordertime;				//ί��ʱ��[HHMMSS��ʽ]
	unsigned long					orderprice;				//ί�м۸�[*�Ŵ���]
	double							ordervolume;			//ί������[��]
} tagHqMem_CancelRankDetail;
//..........................................................................................................................
typedef struct
{
	unsigned char					kindtype;				//��Ʒ���
	unsigned char					side;					//��������[0���� 1����]
	tagHqMem_CancelRankDetail		record[10];				//10��������Ϣ
} tagSHL2Mem_CancelRaData;
//..........................................................................................................................
//�ۼƳ�������
typedef struct
{
	unsigned short					serial;					//��Ʒ���
	double							totalvolume;			//����������[��]
} tagHqMem_TotalCancelRankDetail;
//..........................................................................................................................
typedef struct
{
	unsigned char					kindtype;				//��Ʒ���
	unsigned char					side;					//��������[0���� 1����]
	tagHqMem_TotalCancelRankDetail	record[10];				//10��������Ϣ
} tagSHL2Mem_CancelRank;
//..........................................................................................................................
//ί�ж���
typedef struct
{
	unsigned short					serial;					//��Ʒ���
	unsigned char					side;					//��������
	unsigned long					price;					//�۸�[*�Ŵ���]
	unsigned short					volcount;				//ί�б���
	double							volume[50];				//��
} tagHqMem_OrderQueue;
//..........................................................................................................................
typedef struct
{
	unsigned char					status;				//�г�״̬
	unsigned long					marketdate;			//�г�����
	unsigned long					markettime;			//�г�ʱ��
} tagHqMem_MarketOverview;
//..........................................................................................................................
typedef struct
{
	unsigned short					serial;				//��Ʒ���
	unsigned long					virtualprice;		//����۸�
	double							virtualvolume;		//����ƥ����
	double							buyvirtualvol;		//������ƥ����
	double							sellvirtualvol;		//��������ƥ����
} tagSHL2Mem_VirtualPrice;

typedef struct
{
	unsigned __int64				Volume;					//��������
} tagCcComm_OrderVolDetail;

#define ORDER_QUEUE_COUNT 150
typedef struct
{
	unsigned long					Price;	
	unsigned short					Count;			//�õ��۸��ί�б���,Ӧ�õ���Volcount			
	unsigned short					Volcount;		//ί�б���
	tagCcComm_OrderVolDetail		Record[ORDER_QUEUE_COUNT];	//���200��add���ܻ���ʱ����
} tagOrderPriceLevel;

typedef struct
{
	unsigned char					Count;
	tagOrderPriceLevel				Price[10];		//���5��add���ܻ���ʱ����
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
	char								phaseCode[8];      ///< ��Ʒʵʱ�׶μ���־(˵����������һ)
} tagShL2FastMem_TdPhase;

#pragma pack( pop )

typedef struct
{
	unsigned char				type;
	unsigned long			    ulcode;
	unsigned short				aserial;
	unsigned short				serial;						//���
	unsigned long				uplimit;
	unsigned long				lowlimit;
	unsigned long				reftime;
	unsigned long				tradeno;
    bool                        after_auction;              // ���Ͼ����Ƿ��Ѿ����
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