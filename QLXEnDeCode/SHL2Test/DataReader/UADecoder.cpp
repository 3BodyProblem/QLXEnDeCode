#if _DEBUG
#include <math.h>
#include <iostream>
#include "UADecoder.h"
#include "FastDecoder.h"


DataHolder::DataHolder( size_t	nSize )
 : m_pCache( NULL ), m_nReadPos( 0 ), m_nWritePos( 0 ), m_nMaxCacheSize( nSize )
{
	m_pCache = new char[m_nMaxCacheSize];

	if( NULL == m_pCache )
	{
		::printf( "\n DataHolder::DataHolder() : allocate memory failed \n" );
	}
}

void DataHolder::Reset()
{
	m_nReadPos = 0;
	m_nWritePos = 0;
}

unsigned int DataHolder::GetSize()
{
	return m_nWritePos;
}

int DataHolder::Write( unsigned short nMsgID, char* pszData, unsigned int nDataLen )
{
	if( NULL == m_pCache || nDataLen <= 0 )
	{
		return -1;
	}

	if( (m_nWritePos+nDataLen+sizeof(nMsgID)) > m_nMaxCacheSize )
	{
		return -2;
	}

	///< write msgid
	*((unsigned short*)(m_pCache + m_nWritePos)) = nMsgID;
	m_nWritePos += sizeof(nMsgID);

	///< write data
	::memcpy( m_pCache + m_nWritePos, pszData, nDataLen );
	m_nWritePos += nDataLen;

	return 0;
}

int DataHolder::Read( unsigned short& nMsgID, char* pszData, unsigned int& nDataLen )
{
	if( NULL == m_pCache || NULL == pszData || m_nReadPos >= m_nWritePos )
	{
		return -1;
	}

	///< write msgid
	nMsgID = *((unsigned short*)(m_pCache + m_nReadPos));
	m_nReadPos += sizeof(nMsgID);

	///< write data
	switch( nMsgID )
	{
	case 1:
		::memcpy( pszData, m_pCache + m_nReadPos, sizeof(tagDemoMarketStatus) );
		nDataLen = sizeof(tagDemoMarketStatus);
		m_nReadPos += sizeof(tagDemoMarketStatus);
		break;
	case 10:
		::memcpy( pszData, m_pCache + m_nReadPos, sizeof(tagDemoStaticType) );
		nDataLen = sizeof(tagDemoStaticType);
		m_nReadPos += sizeof(tagDemoStaticType);
		break;
	case 13:
		::memcpy( pszData, m_pCache + m_nReadPos, sizeof(tagDemoSnapType) );
		nDataLen = sizeof(tagDemoSnapType);
		m_nReadPos += sizeof(tagDemoSnapType);
		break;
	case 14:
		::memcpy( pszData, m_pCache + m_nReadPos, sizeof(tagDemoTranData) );
		nDataLen = sizeof(tagDemoTranData);
		m_nReadPos += sizeof(tagDemoTranData);
		break;
	case 15:
		::memcpy( pszData, m_pCache + m_nReadPos, sizeof(tagDemoIndexType) );
		nDataLen = sizeof(tagDemoIndexType);
		m_nReadPos += sizeof(tagDemoIndexType);
		break;
	case 16:
		::memcpy( pszData, m_pCache + m_nReadPos, sizeof(tagDemoVirtualPriceType) );
		nDataLen = sizeof(tagDemoVirtualPriceType);
		m_nReadPos += sizeof(tagDemoVirtualPriceType);
		break;
	default:
		return -2;
	}

	return 0;
}

///< --------------------------------------------------------------------------


static unsigned int s_time = 0;
tagDemoMarketStatus	g_Status = { 1, 0, 0 };
unsigned int		g_nEncodeBufferSize = 0;
DataHolder			g_DataHolder( 1024*8*100 );


#define GETRARECODE \
    double scale = 1.f; \
    if(rate_ != 0) { \
		scale = pow(10., rate_ - precision); \
	}

#define CODEEQUAL(a,b) (*((unsigned long*)(a))==*((unsigned long*)(b))&&*((unsigned short*)((a)+4))==*((unsigned short*)((b)+4)))


// 3115
void Cb_DeCodeUA3115::OnBegin(int id) {
    date_ = 0;
    time_ = 0;
}

int Cb_DeCodeUA3115::OnData(int tag, L2VAL *var, int precision) {
    switch( tag )
	{
        case 42:
            time_ = var->I4 / 100;
            break;
        case 10003:
            date_ = var->I4;
            break;
    }

	return 1;
}

void Cb_DeCodeUA3115::OnEnd()
{
	g_Status.Date = date_;
	g_Status.Time = time_;

	if( (g_Status.Time - s_time) >= 180 )
	{
		s_time = g_Status.Time;
		::printf( "[%u]\n", s_time );
	}

	g_DataHolder.Write( 1, (char*)&g_Status, sizeof(tagDemoMarketStatus) );
	g_nEncodeBufferSize = g_pEncoder->EncodeMessage( 1, (char*)&g_Status, sizeof(tagDemoMarketStatus) );
}

////////////////////////////////////////////////////////////////////////////////
// 3202
static bool IsStockClosed(const char *status_10135) {
    // SUSP是用于解决交易所的数据错误
    if ((strncmp(status_10135, "CLOSE", 5) == 0) || (strncmp(status_10135, "ENDTR", 5) == 0) || (strncmp(status_10135, "SUSP", 4) == 0)) {
        return true;
    } else {
        return false;
    }
}

void Cb_DeCodeUA3202::OnBegin(int id) {
	m_pStock = NULL;
    type_ = 1;
    rate_ = 3;
    skip_ = false;
    last_10204_ = 0;

    memset(&price_level_, 0, sizeof(price_level_));
}

int Cb_DeCodeUA3202::OnData(int tag, L2VAL *var, int precision) {
    if (skip_) {
        return 1;
    }
    switch (tag) {
        case 10178:
			g_Status.Time = var->I4;
			g_DataHolder.Write( 1, (char*)&g_Status, sizeof(tagDemoMarketStatus) );
			g_nEncodeBufferSize = g_pEncoder->EncodeMessage( 1, (char*)&g_Status, sizeof(tagDemoMarketStatus) );
		    break;
        case 10121: {
            if (var->I4 == 1) {
                skip_ = true;
            }
            break;
        }
        case 48: {
			strncpy(code_, var->String, sizeof(code_));
			m_pStock = m_StockTable[code_];
			if( NULL == m_pStock )
			{
				skip_ = true;
			}
            break;
        }
        case 10146: // 是否全量数据
            if (var->I4 == 1) {
                StockPriceLevel *stockpricelevel = m_PriceLevel[code_];
                if (stockpricelevel) {
                    stockpricelevel->buy_.Reset();
                    stockpricelevel->sell_.Reset();
                }
                memset( m_pStock->Buy, 0, sizeof(m_pStock->Buy) );
                memset( m_pStock->Sell, 0, sizeof(m_pStock->Sell) );
                status_10147_ = 1;
                status_10148_ = 1;
            }
            break;
        case 140: {
			GETRARECODE
			unsigned long close = ((double)var->I4 + 0.5 / scale) * scale;
			if (var->I4 > 0) {
				m_pStock->Now = close;
			}
            break;
        }
        case 10018: {
            GETRARECODE
			unsigned long open = (double)var->I4 * scale;	
			if (open != 0) {
				m_pStock->Open = open;
			}
            break;
        }
        case 332: {
            GETRARECODE
			unsigned long high = (double)var->I4 * scale;	
			if (high != 0) {
				m_pStock->High = high;
			}
            break;
        }
        case 333: {
            GETRARECODE
            unsigned long low = (double)var->I4 * scale;
            if (low != 0) {
                m_pStock->Low = low;
            }
            break;
        }
        case 31: {
            GETRARECODE
            unsigned long now = (double)var->I4 * scale;
            if (now != 0) {
                m_pStock->Now = now;
            }
            break;
        }
        case 10204: {
            GETRARECODE
            if (var->vt != dt_null && var->I4 != 0) {
                last_10204_ = var->I4 * scale;
            }
            break;
        }
        case 10135: {
            // 处理收盘价格
            if (IsStockClosed(var->String)) {
                if (last_10204_ > 0) {
                    m_pStock->Now = last_10204_;
                }
            }
            break;
		}
        case 8503:
            if (var->I4 != 0) {
                m_pStock->Records = var->I4;
            }
            break;
        case 387:
			if (var->I8 != 0) {
				//对量进行调整
				m_pStock->Volume = (double)var->I8 * pow(10., -precision);
			}
            break;
        case 8504:
            if (var->I8 != 0) {
				__int64	nAmt = (double)var->I8 * 1000;
                m_pStock->Amount = (double)nAmt * pow(10., -3);	//(double)var->I8 * pow(10., -3);
            }
            break;
        case 10043:
            break;
        case 10039:
            break;
        case 10044:
            break;
        case 10040:
            break;
        case 10057:
			if (var->I4 > 0) {
				m_pStock->Voip = var->I4 * pow(10., 4 - precision);
			}
            break;
        case 10068: {
			isbuy_ = true;
			StockPriceLevel *stockpricelevel = m_PriceLevel[code_];
			if (stockpricelevel) {
                price_level_ = &stockpricelevel->buy_;
            } else {
                price_level_ = NULL;
            }
            break;
        }
        case 10069: {
            isbuy_ = false;
			StockPriceLevel *stockpricelevel = m_PriceLevel[code_];
            if (stockpricelevel) {
                price_level_ = &stockpricelevel->sell_;
            } else {
                price_level_ = NULL;
            }
            break;
        }
        case 10147:
            if (var->vt == dt_int32) {
                status_10147_ = var->I4;
            } else {
                status_10147_ = 1;
            }
            if (status_10147_ == 0) {
                status_10147_ = 1;
            }
            status_10148_ = 1;
            break;
        case 44: {
            memset(&last_buysell_, 0, sizeof(last_buysell_));
            GETRARECODE
            last_buysell_.price = var->I4 * scale;
            break;
        }
        case 39: {
			last_buysell_.volume = var->I8 * pow(10.,  -precision);
            std::list<tagHqMem_Shl2BuySell>::iterator it;
            switch (status_10147_) {
                case 1: {
                    int index = price_level_->Add(last_buysell_, isbuy_);
                    if (index == -1) {
                        order_queue_ = NULL;
                    } else {
                        order_queue_ = &price_level_->Get(index)->order_queue_;
                    }
                    break;
                }
                case 2: {
                    int index = price_level_->Update(last_buysell_);
                    if (index == -1) {
                        order_queue_ = NULL;
                    } else {
                        order_queue_ = &price_level_->Get(index)->order_queue_;
                    }
                    break;
                }
                case 3: {
                    int index = price_level_->Erase(last_buysell_);
                    order_queue_ = NULL;
                    break;
                }
            }
            break;
        }
        case 10067:
            if (order_queue_) {
                order_queue_->Volcount = var->I4;
            }
            break;
        case 73:
            break;
        case 10148:
            if (var->vt == dt_int32) {
                status_10148_ = var->I4;
            } else {
                status_10148_ = 1;
            }
            if (status_10148_ == 0) {
                status_10148_ = 1;
            }
            break;
        case 10149:
            last_10149_ = var->I4;
            if (status_10148_ == 3) {
                if (order_queue_) {
                    order_queue_->Record[last_10149_].Volume = _UI64_MAX;
                }
            }
            break;
        case 38:
            if (order_queue_ == NULL || last_10149_ >= ORDER_QUEUE_COUNT) {
                break;
            }
            if (status_10148_ == 1 || status_10148_ == 2) {
                __int64 vol = var->I8 * pow(10., -precision);
                if (status_10148_ == 1) {
                   if (order_queue_->Count < ORDER_QUEUE_COUNT) {
                       order_queue_->Record[order_queue_->Count++].Volume = vol;
                   }
                } else if (status_10148_ == 2) {
                    order_queue_->Record[last_10149_].Volume = vol;
                }
            }
            break;
    }
    return 1;
}

void Cb_DeCodeUA3202::OnEnd() {
    if (skip_) {
        return;
    }

    int i;
	StockPriceLevel *stockpricelevel = m_PriceLevel[code_];
    if (stockpricelevel == NULL) {
        return;
    }

	int				nBVolcount = 0;			///< 买一委托笔数
	int				nSVolcount = 0;			///< 卖一委托笔数
	unsigned long	nBuyPrice = 0;			///< 买一价
	unsigned long	nSellPrice = 0;			///< 卖一价
	PriceLevelItem	*sitem = NULL;
	if( (sitem = stockpricelevel->sell_.Get(0)) )
	{
		nSellPrice = sitem->price_.price;
		nSVolcount = sitem->order_queue_.Volcount;
	}

    PriceLevel *level = &stockpricelevel->buy_;
    for (i = 0; i < 10; ++i) {
        PriceLevelItem *item = level->Get(i);
        if (item == NULL) {
            m_pStock->Buy[i].Price = 0;
            m_pStock->Buy[i].Volume = 0;
        } else {
            item->order_queue_.Price = item->price_.price;
            m_pStock->Buy[i].Price = item->price_.price;
            m_pStock->Buy[i].Volume = item->price_.volume;

			if( i == 0 )
			{
				nBuyPrice = item->price_.price;
				nBVolcount = item->order_queue_.Volcount;
			}
        }
    }
    if (level == NULL) {
        return;
    }

    PriceLevelItem *item = PushOrderQueue( level, true, nSellPrice, nSVolcount, nBuyPrice, nBVolcount );

    // sell
    level = &stockpricelevel->sell_;
    for (i = 0; i < 10; ++i) {
        PriceLevelItem *item = level->Get(i);
        if (item == NULL) {
            m_pStock->Sell[i].Price = 0;
            m_pStock->Sell[i].Volume = 0;
        } else {
            item->order_queue_.Price = item->price_.price;
            m_pStock->Sell[i].Price = item->price_.price;
            m_pStock->Sell[i].Volume = item->price_.volume;
        }
    }

    item = PushOrderQueue( level, false, nSellPrice, nSVolcount, nBuyPrice, nBVolcount );

    if (memcmp(&stockpricelevel->last_stockdata_, m_pStock, sizeof(tagDemoSnapType)) != 0) {
		g_DataHolder.Write( 13, (char*)m_pStock, sizeof(tagDemoSnapType) );
		g_nEncodeBufferSize = g_pEncoder->EncodeMessage( 13, (char*)m_pStock, sizeof(tagDemoSnapType) );
        memcpy(&stockpricelevel->last_stockdata_, m_pStock, sizeof(tagDemoSnapType));
    }
}

PriceLevelItem* Cb_DeCodeUA3202::PushOrderQueue(PriceLevel *level, bool buy, unsigned long nSellPrice, int nSVolcount, unsigned long nBuyPrice, int nBVolcount) {
    PriceLevelItem *item = level->Get(0);

	if( true == buy && nBuyPrice > 0 && nSellPrice == 0 && nSVolcount == 0 )
	{
		tagOrderPriceLevel	tagPriceLevel = { 0 };
//		Global_DataIO.UpdateOrderQueue( serial_, &tagPriceLevel, false );
	}

	if( false == buy && nSellPrice > 0 && nBuyPrice == 0 && nBVolcount == 0 )
	{
		tagOrderPriceLevel	tagPriceLevel = { 0 };
//		Global_DataIO.UpdateOrderQueue( serial_, &tagPriceLevel, true );
	}

    if (item) {
        tagCcComm_OrderVolDetail r[ORDER_QUEUE_COUNT];
        int j = 0;
        int i = 0;
        int count = item->order_queue_.Count;
        int delete_count = 0;
        for (;;) {
            if (count == 0) {
                break;
            }
            for (;;) {
                if (i >= ORDER_QUEUE_COUNT) {
                    break;
                }
                if (item->order_queue_.Record[i].Volume != _UI64_MAX) {
                    break;
                } else {
                    delete_count++;
                    i++;
                }
            }
            if (i >= ORDER_QUEUE_COUNT || j >= ORDER_QUEUE_COUNT) {
                break;
            }
            r[j].Volume = item->order_queue_.Record[i].Volume;

            i++;
            j++;
            count--;
        }

        if (delete_count > item->order_queue_.Count) {
            item->order_queue_.Count = 0;
        } else {
            item->order_queue_.Count -= delete_count;
        }

        memcpy(item->order_queue_.Record, r, sizeof(item->order_queue_.Record));

        if (level->IsOrderQueueChanged()) {
//            Global_DataIO.UpdateOrderQueue(serial_, &item->order_queue_, buy);
            level->SetOrderQueueNotChanged();
        }
    }
	else
	{
		tagOrderPriceLevel	tagPriceLevel = { 0 };
//		Global_DataIO.UpdateOrderQueue( serial_, &tagPriceLevel, buy );
	}

    return item;
}

////////////////////////////////////////////////////////////////////////////////
void	Cb_DeCodeUA3201::OnBegin(int id)
{
    rate_ = 1;
	skip_ = false;
	m_pTrade = NULL;
	nStockType_ = 1;

	b10121 = false;
}

int		Cb_DeCodeUA3201::OnData(int tag, L2VAL *var, int precision)
{
    if( skip_ ) {
        return 1;
    }

	if (tag == 10121)
	{
		b10121 = var->I4 ==1;
	}
	if (b10121)
	{
		return 1;
	}

	if( tag == 48 )
	{	//取得代码的序号
		char	Code[6];

		strncpy( Code, var->String, 6 );
		m_pTrade = m_TradeTable[Code];

		if( NULL == m_pTrade )
		{
			skip_ = true;
		}
	}
	else if( tag == 10013 )
	{
		m_pTrade->TradeTime = var->I4/100;
	}
	else if( tag == 10014 )
	{
		GETRARECODE
		m_pTrade->Price = (double)var->I4*scale;
	}
	else if( tag == 10015 )
	{
		m_pTrade->Volume = var->I8 * pow( 10.,  -precision );
    }

	return 1;
}

void	Cb_DeCodeUA3201::OnEnd()
{
	if( true == skip_ )
	{
		return;
	}

	static unsigned short s_nTradeNo = 0;
	static int s_tradetime = 0;

	if (b10121)
	{
		return;
	}

	if( m_pTrade->TradeTime == 0 )
	{
		m_pTrade->TradeTime = s_tradetime;
	}

	if( m_pTrade->Volume != 0 && m_pTrade->Price != 0 )
	{
		m_pTrade->TradeNo = s_nTradeNo++;
		g_DataHolder.Write( 14, (char*)m_pTrade, sizeof(tagDemoTranData) );
		g_nEncodeBufferSize = g_pEncoder->EncodeMessage( 14, (char*)m_pTrade, sizeof(tagDemoTranData) );
	}

	s_tradetime = m_pTrade->TradeTime;
}
////////////////////////////////////////////////////////////////////////////////
void Cb_DeCodeUA3113::OnBegin(int id) {
	skip_ = false;
	m_pIndex = NULL;

	b10121=false;
	nVolFactor=1;
	rate_=1;
}

int Cb_DeCodeUA3113::OnData(int tag, L2VAL *var, int precision) {
    if (skip_) {
        return 1;
    }

	if (tag == 10121)
	{
		b10121 = var->I4 == 1;
	}
	if (b10121)
	{
		return 1;
	}

	if( tag == 48 )
	{	//取得代码的序号
		char			Code[6];

		strncpy( Code, var->String, 6 );
		m_pIndex = m_IndexTable[Code];

		if( NULL == m_pIndex )
		{
			skip_ = true;
		}
	}
	else if( tag == 10006 )
	{
		GETRARECODE

		if( var->I8 != 0 )
		{
			m_pIndex->Open = (double)var->I8 * scale + 0.5;
		}
	}
	else if( tag == 10007 )
	{
		GETRARECODE
		double n64TempValue = (double)var->I8 * scale + 0.5;
		if( n64TempValue > 0 )
		{
			m_pIndex->Now = n64TempValue;
		}
	}
	else if( tag == 10009 )
	{
		GETRARECODE
		if( var->I8 != 0 )
		{
			m_pIndex->High = (double)var->I8 * scale + 0.5;
		}
	}
	else if( tag == 10010 )
	{
		GETRARECODE
		if( var->I8 != 0 )
		{
			m_pIndex->Low = (double)var->I8 * scale + 0.5;
		}
	}
	else if( tag == 10008 )
	{
		GETRARECODE
		if( var->I8 != 0 )
		{
			m_pIndex->Now = (double)var->I8 * scale + 0.5;
		}
	}
	else if( tag == 10118 )
	{
		if( var->I8 != 0 )
		{
			m_pIndex->Amount = var->I8;
		}
	}
	else if( tag == 387 )
	{
        if (var->I8 != 0) {
		    m_pIndex->Volume = var->I8;
        }
	}
	else if( tag == 10178 )
	{
		g_Status.Time = var->I4;
		g_DataHolder.Write( 1, (char*)&g_Status, sizeof(tagDemoMarketStatus) );
		g_nEncodeBufferSize = g_pEncoder->EncodeMessage( 1, (char*)&g_Status, sizeof(tagDemoMarketStatus) );
	}
    else if (tag == 10205) {
        if (var->vt != dt_null) {
            if (var->I8 > 0) {
                GETRARECODE
                m_pIndex->Now = (double)var->I8 * scale + 0.5;
            }
        }
    }

	return 1;
}

void	Cb_DeCodeUA3113::OnEnd()
{
    if (skip_) {
        return;
    }

	if (b10121)
	{
		return;
	}

	g_DataHolder.Write( 15, (char*)m_pIndex, sizeof(tagDemoIndexType) );
	g_nEncodeBufferSize = g_pEncoder->EncodeMessage( 15, (char*)m_pIndex, sizeof(tagDemoIndexType) );
}


////////////////////////////////////////////////////////////////////////////////
//集合竞价
void	Cb_DeCodeUA3107::OnBegin(int id)
{
	skip_ = false;
	m_pVirtualPrice = NULL;

	rate_=1;
	nSerialType_=0;
	nVol_=0;
	nSide_=0;
	b10121=false;
}

int		Cb_DeCodeUA3107::OnData(int tag, L2VAL *var, int precision)
{
    if (skip_) {
        return 1;
    }

	if (tag == 10121)
	{
		b10121 = var->I4 ==1;
	}
	if (b10121)
	{
		return 1;
	}
	if (tag == 48)
	{
		//取得代码的序号
		char					Code[6];
		
		strncpy( Code, var->String , 6 );
		m_pVirtualPrice = m_VirtualPriceTable[Code];

		if( NULL == m_pVirtualPrice )
		{
			skip_ = true;
		}
	}
	else if( tag == 44 )
	{
		GETRARECODE
		m_pVirtualPrice->VirtualPrice = var->I4 * scale;
	}
	else if( tag == 10127 )
	{
		if( precision > 0 )
		{
			m_pVirtualPrice->VirtualVolume = (double)var->I8 *pow( 10., -precision);
		}
		else
		{
			m_pVirtualPrice->VirtualVolume = var->I8;
		}
	}
	else if( tag == 151 )
	{
		nVol_ = (double)var->I8 *pow( 10., -precision );
	}
	else if( tag == 54 )
	{
		nSide_ = var->I4;
	}
	else if( tag == 10178 )
	{
		g_Status.Time = var->I4;
		g_DataHolder.Write( 1, (char*)&g_Status, sizeof(tagDemoMarketStatus) );
		g_nEncodeBufferSize = g_pEncoder->EncodeMessage( 1, (char*)&g_Status, sizeof(tagDemoMarketStatus) );
	}
	return 1;
}

void	Cb_DeCodeUA3107::OnEnd()
{
    if (skip_) {
        return;
    }

	if (b10121)
	{
		return;
	}

	if( nSide_ == '1' )
	{
		m_pVirtualPrice->BuyVirtualVol = nVol_;
	}
	else
	{
		m_pVirtualPrice->SellVirtualVol = nVol_;
	}

	g_DataHolder.Write( 16, (char*)m_pVirtualPrice, sizeof(tagDemoVirtualPriceType) );
	g_nEncodeBufferSize = g_pEncoder->EncodeMessage( 16, (char*)m_pVirtualPrice, sizeof(tagDemoVirtualPriceType) );
}


#endif






