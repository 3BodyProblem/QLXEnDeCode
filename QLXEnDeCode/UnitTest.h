#ifndef __QLX_UNIT_TEST_H__
#define __QLX_UNIT_TEST_H__
#pragma warning(disable : 4996)
#pragma warning(disable : 4204)


#include "PYDExport.h"
#ifdef	EXPORT_PYTHON_PYD_MODULE			///< 只有在定义pyd导出的条件下才支持单元测试(gtest)
#include "IEnDeCode.h"
#include "gtest/gtest.h"
#include "demo.pb.h"
#include "MarketDataStruct.h"
#include "UnitTestStructDefine.h"


#define							GLOBAL_BUFFER_SIZE			1024*1024*10				///< 全局缓存长度
extern InterfaceEncode			*g_pEncoder;
extern InterfaceDecode			*g_pDecoder;
extern char						*g_pGlobalBuffer;


///< -------------------- DEMO部分的测试类： PROTOBUF序列化/反序列化类定义 ----------------------------------

class OPDemoMarketStatus : public IPbOperation
{
public:
	OPDemoMarketStatus( int nMsgID );

	int		MessageID() const;

	int		Serialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen );

	int		Unserialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen );
protected:
	MyUnitTest::DemoMarketStatus	m_oDemoMarketStatus;
	int								m_nMessageID;
};

class OPDemoStatic : public IPbOperation
{
public:
	OPDemoStatic( int nMsgID );

	int		MessageID() const;

	int		Serialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen );

	int		Unserialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen );
protected:
	MyUnitTest::DemoStatic		m_oDemoStatic;
	int							m_nMessageID;
};

class OPDemoSnap : public IPbOperation
{
public:
	OPDemoSnap( int nMsgID );

	int		MessageID() const;

	int		Serialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen );

	int		Unserialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen );
protected:
	MyUnitTest::DemoSnap		m_oDemoSnap;
	int							m_nMessageID;
};

class OPDemoTrade : public IPbOperation
{
public:
	OPDemoTrade( int nMsgID );

	int		MessageID() const;

	int		Serialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen );

	int		Unserialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen );
protected:
	MyUnitTest::DemoTrade		m_oDemoTrade;
	int							m_nMessageID;
};

class OPDemoIndex : public IPbOperation
{
public:
	OPDemoIndex( int nMsgID );

	int		MessageID() const;

	int		Serialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen );

	int		Unserialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen );
protected:
	MyUnitTest::DemoIndex		m_oDemoIndex;
	int							m_nMessageID;
};

class OPDemoVirtualPrice : public IPbOperation
{
public:
	OPDemoVirtualPrice( int nMsgID );

	int		MessageID() const;

	int		Serialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen );

	int		Unserialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen );
protected:
	MyUnitTest::DemoVirtualPrice	m_oDemoVirtualPrice;
	int								m_nMessageID;
};


///< --------------------- DEMO部分的测试类： 单元测试类定义 --------------------------------

/**
 * @class		TestDemoMarketInfo
 * @brief		简单测试市场信息
 */
class TestDemoMarketInfo : public testing::Test
{
#define		MAX_MARKET_NUM		10
protected:
	static	void	SetUpTestCase();
	static	void	TearDownTestCase();

	void	SetUp();
	void	TearDown();

protected:
	static tagDemoMarketStatus		m_lstTagStatus[MAX_MARKET_NUM];
};


/**
 * @class		TestDemoStatic
 * @brief		简单测试样例静态数据类的加解码
				&
				测试单个或多个加解码
 */
class TestDemoStatic : public testing::Test
{
#define		MAX_STATIC_NUM		10
protected:
	static	void	SetUpTestCase();
	static	void	TearDownTestCase();

	void	SetUp();
	void	TearDown();

protected:
	static	tagDemoStaticType	m_lstTagName[MAX_STATIC_NUM];
};


/**
 * @class		TestDemoSnap
 * @brief		简单测试样例静态数据类的加解码
				&
				测试单个或多个加解码
 */
class TestDemoSnap : public testing::Test
{
#define		MAX_SNAP_NUM		512
protected:
	static	void	SetUpTestCase();
	static	void	TearDownTestCase();

	void	SetUp();
	void	TearDown();

protected:
	static	tagDemoSnapType		m_lstTagSecurity[MAX_SNAP_NUM];
};

/**
 * @class		TestSHL2MarketData
 * @brief		加载并测试上海Level2的市场行情数据的所有类型
 */
class TestSHL2MarketData : public testing::Test
{
protected:
	static	void	SetUpTestCase();
	static	void	TearDownTestCase();

	void	SetUp();
	void	TearDown();
protected:
};


///< ------------ 单元测试初始化类定义 ------------------------------------


/**
 * @class		QLXEnDeCodeTestEnv
 * @brief		全局事件(初始化引擎)
 */
class QLXEnDeCodeTestEnv : public testing::Environment
{
public:
	void	SetUp();
	void	TearDown();
};


///< ------------------ 单元测试导出函数定义 -------------------------------

/**
 * @brief		测试demo类型的单元测试
 * @param[in]	pszXmlEnDeTemplatePath		编码解码的配置文件xml
 * @param[in]	sParam1						市场标识字符串
 * @param[in]	sParam2						参数2
 * @param[in]	sParam3						参数3
 * @param[in]	sParam4						参数4
 * @param[in]	sParam5						参数5
 * @param[in]	sParam6						参数6
 */
int	RunAllDemoTest_Encode( char* pszXmlEnDeTemplate, std::string sParam1 = "", std::string sParam2 = "", std::string sParam3 = ""
													, std::string sParam4 = "", std::string sParam5 = "", std::string sParam6 = "" );



#endif
#endif





