#ifndef __UA_DECODER_H__
#define __UA_DECODER_H__


#if _DEBUG
#pragma warning(disable:4786)
#include <list>
#include <map>
#include <vector>
#include "../../UnitTest.h"
#include "../TemplateParser/FastBase.h"
#include "../DataCenter/MemStructure.h"
#include "../../UnitTestStructDefine.h"


/**
 * @class		DataHolder
 * @brief		将原始数据保留缓存
 */
class DataHolder
{
public:
	DataHolder( size_t nSize );

	void Reset();
	int Write( unsigned short nMsgID, char* pszData, unsigned int nDataLen );
	int Read( unsigned short& nMsgID, char* pszData, unsigned int& nDataLen );

	unsigned int GetSize();

protected:
	char*				m_pCache;
	unsigned int		m_nReadPos;
	unsigned int		m_nWritePos;
	size_t				m_nMaxCacheSize;
};


extern unsigned int			g_nEncodeBufferSize;
extern DataHolder			g_DataHolder;


///< --------------------------------------------------------------------------


typedef struct
{
	void*	next;
	void*	object;
} tagNode;

template< class TYPE, int MAXSIZE = 10000 >
class DataHash
{
public:
	DataHash();
	~DataHash();

	bool		IsValid();

	TYPE*		operator[]( char (&pszCode)[6] );

protected:
	TYPE*						m_pDataBuffer;					///< 分配MAXSIZE大行情数据块
	unsigned int				m_nDataAllocPos;
	tagNode						m_NodeBuffer[MAXSIZE];			///< 分配MAXSIZE大节点数据块
	unsigned int				m_nNodeAllocPos;
	tagNode*					m_pLstData[MAXSIZE];			///< 节点存取MAXSIZE列表
};


template< class TYPE, int MAXSIZE >
DataHash< TYPE, MAXSIZE >::DataHash()
 : m_nDataAllocPos( 0 ), m_nNodeAllocPos( 0 )
{
	m_pDataBuffer = new TYPE[MAXSIZE];
	::memset( m_pLstData, 0, sizeof(m_pLstData) );
	::memset( m_NodeBuffer, 0, sizeof(m_NodeBuffer) );
	::memset( m_pDataBuffer, 0, sizeof(TYPE)*MAXSIZE );

	if( NULL == m_pDataBuffer )
	{
		::printf( "\nDataHash::DataHash() : allocate market data memory failed. \n" );
	}
}

template< class TYPE, int MAXSIZE >
DataHash< TYPE, MAXSIZE >::~DataHash()
{
	if( NULL != m_pDataBuffer )
	{
		delete [] m_pDataBuffer;
		m_pDataBuffer = NULL;
		m_nDataAllocPos = 0;
		m_nNodeAllocPos = 0;
	}
}

template< class TYPE, int MAXSIZE >
bool DataHash< TYPE, MAXSIZE >::IsValid()
{
	if( NULL == m_pDataBuffer )
	{
		return false;
	}

	return true;
}

template< class TYPE, int MAXSIZE >
TYPE* DataHash< TYPE, MAXSIZE >::operator[]( char (&pszCode)[6] )
{
	unsigned int	nCode=( (int)(pszCode[0] - '0') * 100000 + 
							(int)(pszCode[1] - '0') * 10000 + 
							(int)(pszCode[2] - '0') * 1000 + 
							(int)(pszCode[3] - '0') * 100 + 
							(int)(pszCode[4] - '0') * 10 + 
							(int)(pszCode[5] - '0') ) % MAXSIZE;

	tagNode*		pNode = m_pLstData[nCode];

	do
	{
		if( NULL == pNode )
		{
			if( (m_nNodeAllocPos+1) >= MAXSIZE ) {
				break;
			}

			tagNode&	newNode = m_NodeBuffer[m_nNodeAllocPos++];

			pNode = &newNode;
			m_pLstData[nCode] = pNode;
			TYPE*	pNewData = m_pDataBuffer + m_nDataAllocPos++;
			::strncpy( pNewData->Code, pszCode, sizeof(pszCode) );
			newNode.object = pNewData;
		}

		TYPE*	pData = (TYPE*)pNode->object;
		if( 0 == strncmp( pData->Code, pszCode, sizeof(pszCode) ) )
		{
			return pData;
		}
		else
		{
			pNode = (tagNode*)pNode->next;
		}
	} while( pNode );

	return NULL;
}


///< --------------------------------------------------------------------------


//Cb_DeCodeUA3115 市场总览
class Cb_DeCodeUA3115 : public Cb_FastDecoder {
public:
	void OnBegin(int id);
	int OnData(int tag, L2VAL *var, int precision);
	void OnEnd();

private:
    int date_;
    int time_;
};

//Cb_DeCodeUA3202 个股快照
class Cb_DeCodeUA3202 : public Cb_FastDecoder {
public:
	void OnBegin(int id);
	int OnData(int tag, L2VAL *var, int precision);
	void OnEnd();
    PriceLevelItem* PushOrderQueue(PriceLevel *level, bool buy, unsigned long nSellPrice, int nSVolcount, unsigned long nBuyPrice, int nBVolcount);

protected:
    int								rate_;
    int								type_;
    bool							skip_;
    char							code_[6];
	tagDemoSnapType*				m_pStock;
	DataHash<tagDemoSnapType>		m_StockTable;
	DataHash<StockPriceLevel>		m_PriceLevel;
    PriceLevel*						price_level_;
    tagOrderPriceLevel*				order_queue_;

private:
    int serial_;
    int status_10147_;
    int status_10148_;
    tagHqMem_Shl2BuySell last_buysell_;

    int last_10204_;
    int last_10149_;
    bool isbuy_;
};

//Cb_DeCodeUA3201 逐笔
class  Cb_DeCodeUA3201 : public Cb_FastDecoder 
{
public:	
	void	OnBegin(int id);
	int		OnData(int tag, L2VAL *var, int precision);
	void	OnEnd();

protected:
	bool							skip_;
	int								rate_;
	tagDemoTranData*				m_pTrade;
	int								nStockType_;
	DataHash<tagDemoTranData>		m_TradeTable;

protected:
	bool							b10121;
};

//Cb_DeCodeUA3113 指数快照
class  Cb_DeCodeUA3113 : public Cb_FastDecoder 
{
public:
	void	OnBegin(int id);
	int		OnData(int tag, L2VAL *var, int precision);
	void	OnEnd();

protected:
	bool							skip_;
	tagDemoIndexType*				m_pIndex;
	DataHash<tagDemoIndexType>		m_IndexTable;

protected:
	int								nVolFactor;
	int								rate_;
	bool							b10121;
};

//Cb_DeCodeUA3107 集合竞价
class  Cb_DeCodeUA3107 : public Cb_FastDecoder 
{
public:
	void	OnBegin(int id);
	int		OnData(int tag, L2VAL *var, int precision);
	void	OnEnd();

protected:
	bool								skip_;
	tagDemoVirtualPriceType*			m_pVirtualPrice;
	DataHash<tagDemoVirtualPriceType>	m_VirtualPriceTable;

protected:
	int								rate_;
	int								nSerialType_;
	__int64							nVol_;
	char							nSide_;
	bool							b10121;
};


#endif
#endif//__UA_DECODER_H__









