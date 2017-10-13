#ifndef __QLX_UNIT_TEST_H__
#define __QLX_UNIT_TEST_H__
#pragma warning(disable : 4996)
#pragma warning(disable : 4204)


#include "PYDExport.h"
#ifdef	EXPORT_PYTHON_PYD_MODULE			///< ֻ���ڶ���pyd�����������²�֧�ֵ�Ԫ����(gtest)
#include "IEnDeCode.h"
#include "gtest/gtest.h"
#include "demo.pb.h"
#include "MarketDataStruct.h"
#include "UnitTestStructDefine.h"


#define							GLOBAL_BUFFER_SIZE			1024*1024*10				///< ȫ�ֻ��泤��
extern InterfaceEncode			*g_pEncoder;
extern InterfaceDecode			*g_pDecoder;
extern char						*g_pGlobalBuffer;


///< -------------------- DEMO���ֵĲ����ࣺ PROTOBUF���л�/�����л��ඨ�� ----------------------------------

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


///< --------------------- DEMO���ֵĲ����ࣺ ��Ԫ�����ඨ�� --------------------------------

/**
 * @class		TestDemoMarketInfo
 * @brief		�򵥲����г���Ϣ
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
 * @brief		�򵥲���������̬������ļӽ���
				&
				���Ե��������ӽ���
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
 * @brief		�򵥲���������̬������ļӽ���
				&
				���Ե��������ӽ���
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
 * @brief		���ز������Ϻ�Level2���г��������ݵ���������
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


///< ------------ ��Ԫ���Գ�ʼ���ඨ�� ------------------------------------


/**
 * @class		QLXEnDeCodeTestEnv
 * @brief		ȫ���¼�(��ʼ������)
 */
class QLXEnDeCodeTestEnv : public testing::Environment
{
public:
	void	SetUp();
	void	TearDown();
};


///< ------------------ ��Ԫ���Ե����������� -------------------------------

/**
 * @brief		����demo���͵ĵ�Ԫ����
 * @param[in]	pszXmlEnDeTemplatePath		�������������ļ�xml
 * @param[in]	sParam1						�г���ʶ�ַ���
 * @param[in]	sParam2						����2
 * @param[in]	sParam3						����3
 * @param[in]	sParam4						����4
 * @param[in]	sParam5						����5
 * @param[in]	sParam6						����6
 */
int	RunAllDemoTest_Encode( char* pszXmlEnDeTemplate, std::string sParam1 = "", std::string sParam2 = "", std::string sParam3 = ""
													, std::string sParam4 = "", std::string sParam5 = "", std::string sParam6 = "" );



#endif
#endif





