#include "UnitTest.h"
#ifdef	EXPORT_PYTHON_PYD_MODULE			///< 只有在定义pyd导出的条件下才支持单元测试(gtest)
#include "Encode.h"
#include "Decode.h"
#include <string>
#include <iostream>
#include <algorithm>
#include "SHL2Test/DataReader/StepClient.h"
#include "SHL2Test/TemplateParser/templateReader.h"


extern	TemplateReader	Global_TempalteReader;
PushDataClient			g_SHL2FastDataLoader;
char					*g_pGlobalBuffer = NULL;								///< 全局缓存地址
InterfaceEncode			*g_pEncoder = NULL;
InterfaceDecode			*g_pDecoder = NULL;


///< -------------------- DEMO部分的测试类： PROTOBUF序列化/反序列化类定义 ----------------------------------

static std::string	s_Param1 = "";											///< 以下是测试参数( 1 ~ 6 )
static std::string	s_Param2 = "";
static std::string	s_Param3 = "";
static std::string	s_Param4 = "";
static std::string	s_Param5 = "";
static std::string	s_Param6 = "";


OPDemoMarketStatus::OPDemoMarketStatus( int nMsgID )
 : m_nMessageID( nMsgID )
{
}

int OPDemoMarketStatus::MessageID() const
{
	return m_nMessageID;
}

int OPDemoMarketStatus::Serialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen )
{
	tagDemoMarketStatus*	pData = (tagDemoMarketStatus*)pszBuf;

	m_oDemoMarketStatus.set_status( pData->Status );
	m_oDemoMarketStatus.set_date( pData->Date );
	m_oDemoMarketStatus.set_time( pData->Time );

	int	nByteSize = m_oDemoMarketStatus.ByteSize();
	if( false == m_oDemoMarketStatus.SerializePartialToArray( pszOutput, nByteSize ) )
	{
		return -1;
	}

	return nByteSize;
}

int OPDemoMarketStatus::Unserialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen )
{
	m_oDemoMarketStatus.ParsePartialFromArray( pszBuf, nBufLen );

	if( false == m_oDemoMarketStatus.IsInitialized() )
	{
		return -1;
	}

	tagDemoMarketStatus*	pData = (tagDemoMarketStatus*)pszOutput;

	pData->Status = m_oDemoMarketStatus.status();
	pData->Date =  m_oDemoMarketStatus.date();
	pData->Time =  m_oDemoMarketStatus.time();

	return m_oDemoMarketStatus.ByteSize();
}

OPDemoStatic::OPDemoStatic( int nMsgID )
 : m_nMessageID( nMsgID )
{
}

int OPDemoStatic::MessageID() const
{
	return m_nMessageID;
}

int OPDemoStatic::Serialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen )
{
	tagDemoStaticType*	pData = (tagDemoStaticType*)pszBuf;

	m_oDemoStatic.set_code( pData->Code, sizeof(pData->Code) );
	m_oDemoStatic.set_name( pData->Name, sizeof(pData->Name) );
	m_oDemoStatic.set_type( pData->Type );

	int	nByteSize = m_oDemoStatic.ByteSize();
	if( false == m_oDemoStatic.SerializePartialToArray( pszOutput, nByteSize ) )
	{
		return -1;
	}

	return nByteSize;
}

int OPDemoStatic::Unserialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen )
{
	m_oDemoStatic.ParsePartialFromArray( pszBuf, nBufLen );

	if( false == m_oDemoStatic.IsInitialized() )
	{
		return -1;
	}

	tagDemoStaticType*	pData = (tagDemoStaticType*)pszOutput;

	::strncpy( pData->Code, m_oDemoStatic.code().c_str(), min(sizeof(pData->Code),m_oDemoStatic.code().length()) );
	::strncpy( pData->Name, m_oDemoStatic.name().c_str(), min(sizeof(pData->Name),m_oDemoStatic.name().length()) );
	pData->Type =  m_oDemoStatic.type();

	return m_oDemoStatic.ByteSize();
}

OPDemoSnap::OPDemoSnap( int nMsgID )
 : m_nMessageID( nMsgID )
{
}

int OPDemoSnap::MessageID() const
{
	return m_nMessageID;
}

int OPDemoSnap::Serialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen )
{
	tagDemoSnapType*	pData = (tagDemoSnapType*)pszBuf;

	m_oDemoSnap.set_code( pData->Code, sizeof(pData->Code) );
	m_oDemoSnap.set_now( pData->Now );
	m_oDemoSnap.set_high( pData->High );
	m_oDemoSnap.set_low( pData->Low );
	m_oDemoSnap.set_open( pData->Open );
	m_oDemoSnap.set_close( pData->Close );
	m_oDemoSnap.set_voip( pData->Voip );
	m_oDemoSnap.set_amount( pData->Amount );
	m_oDemoSnap.set_volume( pData->Volume );
	m_oDemoSnap.set_records( pData->Records );

	m_oDemoSnap.clear_buy();
	int nEmptyNum = 0;
	for( int ii = 0; ii < 10; ii++ )
	{
		if( pData->Buy[9-ii].Price == 0 && pData->Buy[9-ii].Volume == 0 )
			nEmptyNum++;
		else
			break;
	}

	for( int i = 0; i < 10-nEmptyNum; i++ )
	{
		MyUnitTest::DemoSnap_InnerBuySell*	pNewBuy = m_oDemoSnap.add_buy();
		pNewBuy->set_price( pData->Buy[i].Price );
		pNewBuy->set_volume( pData->Buy[i].Volume );
	}

	m_oDemoSnap.clear_sell();
	nEmptyNum = 0;
	for( int jj = 0; jj < 10; jj++ )
	{
		if( pData->Sell[9-jj].Price == 0 && pData->Sell[9-jj].Volume == 0 )
			nEmptyNum++;
		else
			break;
	}
	for( int j = 0; j < 10-nEmptyNum; j++ )
	{
		MyUnitTest::DemoSnap_InnerBuySell*	pNewSell = m_oDemoSnap.add_sell();
		pNewSell->set_price( pData->Sell[j].Price );
		pNewSell->set_volume( pData->Sell[j].Volume );
	}

	int	nByteSize = m_oDemoSnap.ByteSize();
	if( false == m_oDemoSnap.SerializePartialToArray( pszOutput, nByteSize ) )
	{
		return -1;
	}

	return nByteSize;
}

int OPDemoSnap::Unserialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen )
{
	m_oDemoSnap.ParsePartialFromArray( pszBuf, nBufLen );

	if( false == m_oDemoSnap.IsInitialized() )
	{
		return -1;
	}

	tagDemoSnapType*	pData = (tagDemoSnapType*)pszOutput;

	::strncpy( pData->Code, m_oDemoSnap.code().c_str(), 6 );
	pData->Now = m_oDemoSnap.now();
	pData->High = m_oDemoSnap.high();
	pData->Low = m_oDemoSnap.low();
	pData->Open = m_oDemoSnap.open();
	pData->Close = m_oDemoSnap.close();
	pData->Voip  = m_oDemoSnap.voip();
	pData->Amount = m_oDemoSnap.amount();
	pData->Volume = m_oDemoSnap.volume();
	pData->Records = m_oDemoSnap.records();

	int	nListSize = m_oDemoSnap.buy_size();
	for( int i = 0; i < 10; i++ )
	{
		if( i < nListSize )
		{
			pData->Buy[i].Price = m_oDemoSnap.buy(i).price();
			pData->Buy[i].Volume = m_oDemoSnap.buy(i).volume();
		}
		else
		{
			pData->Buy[i].Price = 0;
			pData->Buy[i].Volume = 0;
		}
	}

	nListSize = m_oDemoSnap.sell_size();
	for( int j = 0; j < 10; j++ )
	{
		if( j < nListSize )
		{
			pData->Sell[j].Price = m_oDemoSnap.sell(j).price();
			pData->Sell[j].Volume = m_oDemoSnap.sell(j).volume();
		}
		else
		{
			pData->Sell[j].Price = 0;
			pData->Sell[j].Volume = 0;
		}
	}

	return m_oDemoSnap.ByteSize();
}

OPDemoTrade::OPDemoTrade( int nMsgID )
 : m_nMessageID( nMsgID )
{
}

int OPDemoTrade::MessageID() const
{
	return m_nMessageID;
}

int OPDemoTrade::Serialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen )
{
	tagDemoTranData*	pData = (tagDemoTranData*)pszBuf;

	m_oDemoTrade.set_code( pData->Code, sizeof(pData->Code) );
	m_oDemoTrade.set_tradeno( pData->TradeNo );
	m_oDemoTrade.set_tradetime( pData->TradeTime );
	m_oDemoTrade.set_price( pData->Price );
	m_oDemoTrade.set_volume( pData->Volume );

	int	nByteSize = m_oDemoTrade.ByteSize();
	if( false == m_oDemoTrade.SerializePartialToArray( pszOutput, nByteSize ) )
	{
		return -1;
	}

	return nByteSize;
}

int OPDemoTrade::Unserialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen )
{
	m_oDemoTrade.ParsePartialFromArray( pszBuf, nBufLen );

	if( false == m_oDemoTrade.IsInitialized() )
	{
		return -1;
	}

	tagDemoTranData*	pData = (tagDemoTranData*)pszOutput;

	::strncpy( pData->Code, m_oDemoTrade.code().c_str(), min(sizeof(pData->Code),m_oDemoTrade.code().length()) );
	pData->TradeNo = m_oDemoTrade.tradeno();
	pData->TradeTime = m_oDemoTrade.tradetime();
	pData->Price = m_oDemoTrade.price();
	pData->Volume = m_oDemoTrade.volume();

	return m_oDemoTrade.ByteSize();
}

OPDemoIndex::OPDemoIndex( int nMsgID )
 : m_nMessageID( nMsgID )
{
}

int OPDemoIndex::MessageID() const
{
	return m_nMessageID;
}

int OPDemoIndex::Serialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen )
{
	tagDemoIndexType*	pData = (tagDemoIndexType*)pszBuf;

	m_oDemoIndex.set_code( pData->Code, sizeof(pData->Code) );
	m_oDemoIndex.set_low( pData->Low );
	m_oDemoIndex.set_open( pData->Open );
	m_oDemoIndex.set_high( pData->High );
	m_oDemoIndex.set_now( pData->Now );
	m_oDemoIndex.set_amount( pData->Amount );
	m_oDemoIndex.set_volume( pData->Volume );

	int	nByteSize = m_oDemoIndex.ByteSize();
	if( false == m_oDemoIndex.SerializePartialToArray( pszOutput, nByteSize ) )
	{
		return -1;
	}

	return nByteSize;
}

int OPDemoIndex::Unserialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen )
{
	m_oDemoIndex.ParsePartialFromArray( pszBuf, nBufLen );

	if( false == m_oDemoIndex.IsInitialized() )
	{
		return -1;
	}

	tagDemoIndexType*	pData = (tagDemoIndexType*)pszOutput;

	::strncpy( pData->Code, m_oDemoIndex.code().c_str(), min(sizeof(pData->Code),m_oDemoIndex.code().length()) );
	pData->Low = m_oDemoIndex.low();
	pData->Open = m_oDemoIndex.open();
	pData->High = m_oDemoIndex.high();
	pData->Now = m_oDemoIndex.now();
	pData->Amount = m_oDemoIndex.amount();
	pData->Volume = m_oDemoIndex.volume();

	return m_oDemoIndex.ByteSize();
}

OPDemoVirtualPrice::OPDemoVirtualPrice( int nMsgID )
 : m_nMessageID( nMsgID )
{
}

int OPDemoVirtualPrice::MessageID() const
{
	return m_nMessageID;
}

int OPDemoVirtualPrice::Serialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen )
{
	tagDemoVirtualPriceType*	pData = (tagDemoVirtualPriceType*)pszBuf;

	m_oDemoVirtualPrice.set_code( pData->Code, sizeof(pData->Code) );
	m_oDemoVirtualPrice.set_virtualprice( pData->VirtualPrice );
	m_oDemoVirtualPrice.set_virtualvolume( pData->VirtualVolume );
	m_oDemoVirtualPrice.set_buyvirtualvol( pData->BuyVirtualVol );
	m_oDemoVirtualPrice.set_sellvirtualvol( pData->SellVirtualVol );

	int	nByteSize = m_oDemoVirtualPrice.ByteSize();
	if( false == m_oDemoVirtualPrice.SerializePartialToArray( pszOutput, nByteSize ) )
	{
		return -1;
	}

	return nByteSize;
}

int OPDemoVirtualPrice::Unserialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen )
{
	m_oDemoVirtualPrice.ParsePartialFromArray( pszBuf, nBufLen );

	if( false == m_oDemoVirtualPrice.IsInitialized() )
	{
		return -1;
	}

	tagDemoVirtualPriceType*	pData = (tagDemoVirtualPriceType*)pszOutput;

	::strncpy( pData->Code, m_oDemoVirtualPrice.code().c_str(), min(sizeof(pData->Code),m_oDemoVirtualPrice.code().length()) );
	pData->VirtualPrice = m_oDemoVirtualPrice.virtualprice();
	pData->VirtualVolume = m_oDemoVirtualPrice.virtualvolume();
	pData->BuyVirtualVol = m_oDemoVirtualPrice.buyvirtualvol();
	pData->SellVirtualVol = m_oDemoVirtualPrice.sellvirtualvol();

	return m_oDemoVirtualPrice.ByteSize();
}

///< --------------------- DEMO部分的测试类： 单元测试类定义 --------------------------------

tagDemoMarketStatus TestDemoMarketInfo::m_lstTagStatus[MAX_MARKET_NUM] = { 0 };

void TestDemoMarketInfo::SetUpTestCase()
{
	for( int n = 0; n < MAX_MARKET_NUM; n++ )
	{
		m_lstTagStatus[n].Status = n % 2;
		m_lstTagStatus[n].Date = 20181212;
		m_lstTagStatus[n].Time = 150102 + n;
	}
}

void TestDemoMarketInfo::TearDownTestCase()
{
	::memset( m_lstTagStatus, 0, sizeof(m_lstTagStatus) );
}

void TestDemoMarketInfo::SetUp()
{
	ASSERT_EQ( 0, g_pEncoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
	ASSERT_EQ( 0, g_pDecoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
}

void TestDemoMarketInfo::TearDown()
{
}


tagDemoStaticType TestDemoStatic::m_lstTagName[MAX_STATIC_NUM] = { 0 };

void TestDemoStatic::SetUpTestCase()
{
	for( int n = 0; n < MAX_STATIC_NUM; n++ )
	{
		char		pszCode[7] = { 0 };

		m_lstTagName[n].Type = (n % 10) + 1;
		::sprintf( pszCode, "%u", 600001 + n );
		::memcpy( m_lstTagName[n].Code, pszCode, 6 );
		::strncpy( m_lstTagName[n].Name, "abcdefg", 8/*"深A发展", sizeof("深A发展")*/ );
	}
}

void TestDemoStatic::TearDownTestCase()
{
	::memset( m_lstTagName, 0, sizeof(m_lstTagName) );
}

void TestDemoStatic::SetUp()
{
	ASSERT_EQ( 0, g_pEncoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
	ASSERT_EQ( 0, g_pDecoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
}

void TestDemoStatic::TearDown()
{
}


tagDemoSnapType TestDemoSnap::m_lstTagSecurity[MAX_SNAP_NUM] = { 0 };

void TestDemoSnap::SetUpTestCase()
{
	for( int n = 0; n < MAX_SNAP_NUM; n++ )
	{
		char				pszCode[7] = { 0 };
		tagDemoSnapType		&snap = m_lstTagSecurity[n];
		unsigned int		nBasePrice = 102400 + n * 500;

		::sprintf( pszCode, "%u", 600001 + n );
		::memcpy( snap.Code, pszCode, 6 );
		snap.Now = nBasePrice;
		snap.High = nBasePrice + 500;
		snap.Low = nBasePrice - 500;
		snap.Open = nBasePrice - 10;
		snap.Close = nBasePrice + 20;
		snap.Voip = nBasePrice - 1000;
		snap.Amount = (nBasePrice * 128) + 0.123;
		snap.Volume = nBasePrice * 300;
		snap.Records = 1000;

		for( int i = 0; i < 10; i++ )
		{
			snap.Buy[i].Price = nBasePrice + i * 5;
			snap.Buy[i].Volume = nBasePrice * 1024 + i;
			snap.Sell[i].Price = nBasePrice - i * 5;
			snap.Sell[i].Volume = nBasePrice * 1024 - i;
		}
	}
}

void TestDemoSnap::TearDownTestCase()
{
	::memset( m_lstTagSecurity, 0, sizeof(m_lstTagSecurity) );
}

void TestDemoSnap::SetUp()
{
	ASSERT_EQ( 0, g_pEncoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
	ASSERT_EQ( 0, g_pDecoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
}

void TestDemoSnap::TearDown()
{
}


///< ------------------------- 各大市场真实行情数据 ----------------------------------

void TestSHL2MarketData::SetUpTestCase()
{
}

void TestSHL2MarketData::TearDownTestCase()
{
}

void TestSHL2MarketData::SetUp()
{
	ASSERT_EQ( 0, g_pEncoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
	ASSERT_EQ( 0, g_pDecoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE ) );
}

void TestSHL2MarketData::TearDown()
{
}



///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------
///< ------------------------ 测试用例定义 ----------------------------------------------------


TEST_F( TestDemoMarketInfo, Bundle )
{
	unsigned short			nMsgID = 0;
	tagDemoMarketStatus		value = { 0 };

	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 1, (char*)&m_lstTagStatus[0], sizeof(tagDemoMarketStatus) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 1, (char*)&m_lstTagStatus[1], sizeof(tagDemoMarketStatus) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 1, (char*)&m_lstTagStatus[2], sizeof(tagDemoMarketStatus) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 1, (char*)&m_lstTagStatus[2], sizeof(tagDemoMarketStatus) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 1, (char*)&m_lstTagStatus[1], sizeof(tagDemoMarketStatus) ) );
	int	nBufSize = g_pEncoder->EncodeMessage( 1, (char*)&m_lstTagStatus[0], sizeof(tagDemoMarketStatus) );
	ASSERT_LT( 0, nBufSize );

	ASSERT_EQ( 0, g_pDecoder->Attach2Buffer( g_pGlobalBuffer, nBufSize ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagStatus[0], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagStatus[1], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagStatus[2], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagStatus[2], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagStatus[1], sizeof(value) ) );
	ASSERT_EQ( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagStatus[0], sizeof(value) ) );
}

TEST_F( TestDemoStatic, Single )
{
	unsigned short			nMsgID = 0;
	tagDemoStaticType		value = { 0 };
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[0], sizeof(tagDemoStaticType) ) );
	g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[0], sizeof(value) ) );
}

TEST_F( TestDemoStatic, Loop )
{
	unsigned short			nMsgID = 0;
	tagDemoStaticType		value = { 0 };
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[0], sizeof(tagDemoStaticType) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[1], sizeof(tagDemoStaticType) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[2], sizeof(tagDemoStaticType) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[2], sizeof(tagDemoStaticType) ) );
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[1], sizeof(tagDemoStaticType) ) );
	int	nBufSize = g_pEncoder->EncodeMessage( 10, (char*)&m_lstTagName[0], sizeof(tagDemoStaticType) );
	ASSERT_LT( 0, nBufSize );

	ASSERT_EQ( 0, g_pDecoder->Attach2Buffer( g_pGlobalBuffer, nBufSize ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[0], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[1], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[2], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[2], sizeof(value) ) );
	ASSERT_NE( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[1], sizeof(value) ) );
	ASSERT_EQ( 0, g_pDecoder->DecodeMessage( nMsgID, (char*)&value, sizeof(value) ) );
	ASSERT_EQ( 0, ::memcmp( &value, &m_lstTagName[0], sizeof(value) ) );
}

TEST_F( TestDemoSnap, Single )
{
	unsigned short		nMsgID = 0;
	tagDemoSnapType		snap = { 0 };
	ASSERT_LT( 0, g_pEncoder->EncodeMessage( 13, (char*)&m_lstTagSecurity[0], sizeof(snap) ) );
	g_pDecoder->DecodeMessage( nMsgID, (char*)&snap, sizeof(snap) );
	ASSERT_EQ( 0, ::memcmp( &snap, &m_lstTagSecurity[0], sizeof(snap) ) );
}

TEST_F( TestDemoSnap, Loop )
{
	unsigned __int64	nSizeCnt = 0;

	for( int i = 0; i < 9999; i++ )
	{
		SetUp();

		for( int n = 0; n < MAX_SNAP_NUM; n++ )
		{
			ASSERT_LT( 0, g_pEncoder->EncodeMessage( 13, (char*)&m_lstTagSecurity[n], sizeof(tagDemoSnapType) ) );
			nSizeCnt += sizeof(tagDemoSnapType);
		}

		for( int n = 0; n < MAX_SNAP_NUM; n++ )
		{
			unsigned short		nMsgID = 0;
			tagDemoSnapType		snap = { 0 };
			g_pDecoder->DecodeMessage( nMsgID, (char*)&snap, sizeof(snap) );
			ASSERT_EQ( 0, ::memcmp( &snap, &m_lstTagSecurity[n], sizeof(snap) ) );
		}
	}

	::printf( "压缩前累计: %I64d \n", nSizeCnt );
}


///< ----------------------- 各大市场外部导入数据测试 -------------------------------------------

TEST_F( TestSHL2MarketData, AllData )
{
	::printf( "从本地文件加载上海L2Fast行情数据(%s + %s)，进行单元测试 \n", s_Param2.c_str(), s_Param3.c_str() );
	ASSERT_LT( 0, Global_TempalteReader.Instance( s_Param2.c_str() ) );
	ASSERT_EQ( true, g_SHL2FastDataLoader.LoadFastData( s_Param3.c_str() ) );
	::printf( "[OK] 上海L2Fast行情数据文件加载解析完毕 \n" );
}


///< ------------------------- 单元测试初始化类定义 ----------------------------------------------

static std::string	s_XmlTemplatePath = "";									///< 数据解析模板文件路径
OPDemoMarketStatus	EnMarket( 1 ), DeMarket( 1 );
OPDemoStatic		EnStatic( 10 ), DeStatic( 10 );							///< demo的static类型策略
OPDemoSnap			EnSnap( 13 ), DeSnap( 13 );								///< demo的snap类型策略
OPDemoTrade			EnTrade( 14 ), DeTrade( 14 );
OPDemoIndex			EnIndex( 15 ), DeIndex( 15 );
OPDemoVirtualPrice	EnVP( 16 ), DeVP( 16 );


void QLXEnDeCodeTestEnv::SetUp()
{
	IPbOperation* pListEncodeOperation[] = { &EnMarket, &EnStatic, &EnSnap, &EnTrade, &EnIndex, &EnVP };///< 需要注册的protobuf的encode策略对象
	IPbOperation* pListDecodeOperation[] = { &DeMarket, &DeStatic, &DeSnap, &DeTrade, &DeIndex, &DeVP };///< 需要注册的protobuf的encode策略对象

	g_pGlobalBuffer = new char[GLOBAL_BUFFER_SIZE];							///< 分配一个encode和decode共用的大缓存,用来依次存放多个Messages
	ASSERT_NE( 0, (int)g_pGlobalBuffer );									///< 检测缓存是否分配成功
	g_pEncoder = new Encode();
	ASSERT_NE( 0, (int)g_pEncoder );
	g_pDecoder = new Decode();
	ASSERT_NE( 0, (int)g_pDecoder );
	ASSERT_EQ( 0, g_pEncoder->Initialize( s_XmlTemplatePath.c_str(), pListEncodeOperation, sizeof(pListEncodeOperation)/sizeof(IPbOperation*) ) );
	ASSERT_EQ( 0, g_pDecoder->Initialize( s_XmlTemplatePath.c_str(), pListDecodeOperation, sizeof(pListDecodeOperation)/sizeof(IPbOperation*) ) );
}

void QLXEnDeCodeTestEnv::TearDown()
{
/*	delete [] g_pGlobalBuffer;
	g_pGlobalBuffer = NULL;

	delete g_pEncoder;
	delete g_pDecoder;
	g_pEncoder = NULL;
	g_pDecoder = NULL;*/
}


///< ------------------ 单元测试导出函数定义 -------------------------------
///< ------------------ 单元测试导出函数定义 -------------------------------
///< ------------------ 单元测试导出函数定义 -------------------------------
///< ------------------ 单元测试导出函数定义 -------------------------------
///< ------------------ 单元测试导出函数定义 -------------------------------
///< ------------------ 单元测试导出函数定义 -------------------------------
///< ------------------ 单元测试导出函数定义 -------------------------------
///< ------------------ 单元测试导出函数定义 -------------------------------
///< ------------------ 单元测试导出函数定义 -------------------------------
///< ------------------ 单元测试导出函数定义 -------------------------------
///< ------------------ 单元测试导出函数定义 -------------------------------


/**
 * @brief		初始化gtest工作环境
				&&
				并设置测试Case名称(filter)
 */
void PrepareGTestEnv( char* pszFilter )
{
	static	bool	s_bInit = false;

	if( false == s_bInit )	{
		int			nArgc = 1;
		char*		pszArgv[32] = { "QLXEnDeCode.dll", };

		s_bInit = true;
		testing::AddGlobalTestEnvironment( new QLXEnDeCodeTestEnv() );
		testing::InitGoogleTest( &nArgc, pszArgv );
	}

	::testing::GTEST_FLAG( filter ) = pszFilter;
}

int	RunAllDemoTest_Encode( char* pszXmlEnDeTemplate, std::string sParam1, std::string sParam2, std::string sParam3, std::string sParam4, std::string sParam5, std::string sParam6 )
{
	std::string		sUsageDesc = "\
	本测试单元，用于测试内置的行情类型。\n\
	\ta) 使用内置的测试数据测试 [RunAllDemoTest_Encode( \"./QLXDemoTemplate.xml\", \"*Demo*\" )] \n\
	\tb) 使用外部导出的测试数据测试 [RunAllDemoTest_Encode( \"./QLXDemoTemplate.xml\", \"*SHL2*\", \"./template.2.13.xml\", \"./fastdata\" )] \n";

	::printf( "%s\n", sUsageDesc.c_str() );

	s_Param1 = sParam1;		///< 市场简称标识字符串
	s_Param2 = sParam2;
	s_Param3 = sParam3;
	s_Param4 = sParam4;
	s_Param5 = sParam5;
	s_Param6 = sParam6;
	s_XmlTemplatePath = pszXmlEnDeTemplate;

	PrepareGTestEnv( (char*)s_Param1.c_str() );

	return RUN_ALL_TESTS();
}


#endif






