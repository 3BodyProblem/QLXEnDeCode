#include <new>
#include <math.h>
#include <algorithm>
#include "EnDePolicy.h"


static	int		s_nDebugFlag = 0;		///< 0:不打印xml策略调试信息 1:打印


char* GetModuleVersion()
{
	static	char	pszVersion[32] = "1.0.0";

	return pszVersion;
}


EncodePolicyDefinition::EncodePolicyDefinition()
 : m_nOperator( NONE ), m_nRelation( INDEPENDENCE ), m_pRelateField( NULL )
{
}

EncodePolicyDefinition::EncodePolicyDefinition( EncodePolicyDefinition::OPERATOR nOP, EncodePolicyDefinition::RELATION nRelation, MsgField* pRelateField, MsgField* pSelfField )
 : m_nOperator( nOP ), m_nRelation( nRelation ), m_pRelateField( NULL )
{
	if( COUNTERPART == m_nRelation )			///< 前后两个消息的同一字段，做差值
	{
		m_pRelateField = pSelfField;
	}
	else										///< 在一个消息的内部的两个字段间做差值
	{
		m_pRelateField = (NULL==pRelateField) ? pSelfField:(MsgField*)pRelateField;
	}
}

EncodePolicyDefinition::EncodePolicyDefinition( EncodePolicyDefinition& FromObj )
{
	m_nOperator = FromObj.m_nOperator;
	m_nRelation = FromObj.m_nRelation;
	m_pRelateField = FromObj.m_pRelateField;
}

EncodePolicyDefinition::OPERATOR EncodePolicyDefinition::Operation() const
{
	return m_nOperator;
}

EncodePolicyDefinition::RELATION EncodePolicyDefinition::Relation() const
{
	return m_nRelation;
}

MsgField* EncodePolicyDefinition::RelatedField() const
{
	return m_pRelateField;
}


///< -----------------------------------------------------


///< 字符串映射到策略操作符( + / - )
EncodePolicyDefinition::OPERATOR Str2OP( std::string& sOP )
{
	if( sOP == "" ) {
		return EncodePolicyDefinition::NONE;
	} else if( sOP == "-" ) {
		return EncodePolicyDefinition::DEDUCT;
	} else if( sOP == "+" ) {
		return EncodePolicyDefinition::ADD;
	}

	return EncodePolicyDefinition::NONE;
}

///< 字符串映射到关系符 (counterpart/correlate)
EncodePolicyDefinition::RELATION Str2Relation( std::string& sCounterpart, std::string& sCorrelate )
{
	if( sCounterpart != "" ) {
		return EncodePolicyDefinition::COUNTERPART;
	}

	if( sCorrelate != "" ) {
		return EncodePolicyDefinition::CORRELATE;
	}

	return EncodePolicyDefinition::INDEPENDENCE;
}

///< 字符串映射到字段类型
FdDefinition::FIELD_TYPE Str2FieldType( std::string& sFieldType )
{
	if( sFieldType == "string" ) {
		return FdDefinition::RAWT_STRING;
	} else if( sFieldType == "int8" ) {
		return FdDefinition::RAWT_INT8;
	} else if( sFieldType == "int16" ) {
		return FdDefinition::RAWT_INT16;
	} else if( sFieldType == "int32" ) {
		return FdDefinition::RAWT_INT32;
	} else if( sFieldType == "int64" ) {
		return FdDefinition::RAWT_INT64;
	} else if( sFieldType == "uint8" ) {
		return FdDefinition::RAWT_UINT8;
	} else if( sFieldType == "uint16" ) {
		return FdDefinition::RAWT_UINT16;
	} else if( sFieldType == "uint32" ) {
		return FdDefinition::RAWT_UINT32;
	} else if( sFieldType == "uint64" ) {
		return FdDefinition::RAWT_UINT64;
	} else if( sFieldType == "double" ) {
		return FdDefinition::RAWT_DOUBLE;
	} else if( sFieldType == "vector" ) {
		return FdDefinition::RAWT_VECTOR;
	} else if( sFieldType == "sequence" ) {
		return FdDefinition::RAWT_SEQUENCE;
	} else if( sFieldType == "date" ) {
		return FdDefinition::RAWT_DATE;
	} else if( sFieldType == "time" ) {
		return FdDefinition::RAWT_TIME;
	}

	return FdDefinition::RAWT_ISNONE;
}

///< 字符串映射到字段类型
size_t CalFieldLength( FdDefinition::FIELD_TYPE eFieldType, unsigned short nLen = 1 )
{
	size_t	nTypeLen = 0;

	switch( eFieldType )
	{
	case FdDefinition::RAWT_INT8:
	case FdDefinition::RAWT_UINT8:
	case FdDefinition::RAWT_VECTOR:
	case FdDefinition::RAWT_STRING:
		nTypeLen = 1;
		break;
	case FdDefinition::RAWT_INT16:
	case FdDefinition::RAWT_UINT16:
		nTypeLen = 2;
		break;
	case FdDefinition::RAWT_DATE:
	case FdDefinition::RAWT_TIME:
	case FdDefinition::RAWT_INT32:
	case FdDefinition::RAWT_UINT32:
		nTypeLen = 4;
		break;
	case FdDefinition::RAWT_INT64:
	case FdDefinition::RAWT_UINT64:
	case FdDefinition::RAWT_DOUBLE:
		nTypeLen = 8;
		break;
	case FdDefinition::RAWT_SEQUENCE:
		nTypeLen = 0;
		break;
	default:
		nTypeLen = 0;
		break;
	}

	return nTypeLen * nLen;
}

///< 字符串映射到字段长度(普通字段，字符串，字节串，序列)
int	Str2FieldTypeNum( FdDefinition::FIELD_TYPE eDataType, std::string& sFieldLen )
{
	switch( eDataType )
	{
	case FdDefinition::RAWT_SEQUENCE:
	case FdDefinition::RAWT_STRING:
	case FdDefinition::RAWT_VECTOR:
		return ::atoi( sFieldLen.c_str() );
	}

	return 1;		///< default, return 1
}

///< 字符串映射到精度值
int Str2Precision( std::string& sPrecision )
{
	if( sPrecision == "" ) {
		return 0;
	}

	return ::atoi( sPrecision.c_str() );
}

///< 字符串转小写
std::string& Str2Lower( std::string& sStr )
{
	std::transform( sStr.begin(), sStr.end(), sStr.begin(), ::tolower );

	return sStr;
}


///< -----------------------------------------------------


MsgField::MsgField( unsigned int nFieldID, MsgPolicy* pMsgPolicy, std::string sFieldName, FdDefinition::FIELD_TYPE nType, size_t nFieldLen
					, EncodePolicyDefinition::OPERATOR nOperator, EncodePolicyDefinition::RELATION nRelation, const MsgField* pRelateField, unsigned short nPrecision )
: m_nFieldNum( 0 ), m_pMsgPolicy( pMsgPolicy ), m_bConvertUnsigned( false ), m_oLastNumber( (__int64)0, nType )
, m_fdDefinition( nFieldID, sFieldName.c_str(), sFieldName.size(), nType, nFieldLen, nPrecision )
, m_policyDefinition( nOperator, nRelation, (MsgField*)pRelateField, this )
{
	::memset( m_lstField, 0, sizeof(m_lstField) );
}

MsgField::MsgField( const MsgField& fd )
: m_oLastNumber( (__int64)0, fd.m_fdDefinition.Type() )
{
	m_fdDefinition  = fd.m_fdDefinition;			///< 字段属性定义
	m_pMsgPolicy = fd.m_pMsgPolicy;					///< 本字段对应的压缩策略引用
	m_policyDefinition = fd.m_policyDefinition;		///< 本字段对应的压缩策略引用

	///< 本字段是一个序列字段的情况
	if( FdDefinition::RAWT_SEQUENCE == FieldDefinition().Type() && fd.m_nFieldNum > 0 )
	{
		m_nFieldNum = fd.m_nFieldNum;

		for( unsigned int n = 0; n < fd.m_nFieldNum; n++ )
		{
			m_lstField[n] = (MsgField*)fd[n];
		}
	}
}

MsgField::~MsgField()
{
}

const FdDefinition&	MsgField::FieldDefinition() const
{
	return m_fdDefinition;
}

const EncodePolicyDefinition& MsgField::EncodeRuleDefinition() const
{
	return m_policyDefinition;
}

int MsgField::Size() const
{
	if( FdDefinition::RAWT_SEQUENCE != FieldDefinition().Type() )
	{
		return -1;
	}

	return m_nFieldNum;
}

bool& MsgField::GetConvertUnignedMark()
{
	return m_bConvertUnsigned;
}

const MsgField* MsgField::operator[] ( unsigned short nIndex ) const
{
	if( FdDefinition::RAWT_SEQUENCE != FieldDefinition().Type() )
	{
		return NULL;
	}

	if( nIndex >= m_nFieldNum )
	{
		return NULL;
	}

	return m_lstField[nIndex];
}

MsgField&	MsgField::operator+= ( const MsgField &fd )
{
	m_lstField[m_nFieldNum] = (MsgField*)&fd;

	return *m_lstField[m_nFieldNum++];
}

SerialNumber& MsgField::GetLastNumber()
{
	return m_oLastNumber;
}

char* MsgField::GetLastString()
{
	return m_oLastString;
}

int MsgField::AddAndReturnRefCnt()
{
	return m_pMsgPolicy->GetRefCounter()[m_fdDefinition.ID()]++;
}

SerialString MsgField::X_CompressString( const char* pData, size_t nDataLen )
{
	switch( m_policyDefinition.Operation() )
	{
	case EncodePolicyDefinition::INDEPENDENCE:
		return SerialString( nDataLen, (char*)pData, nDataLen );
	case EncodePolicyDefinition::COUNTERPART:
		{
			if( 0 == AddAndReturnRefCnt() )///< 基值不存在的情况（即Frame中的第一个包）
			{
				::memcpy( m_oLastString, pData, sizeof(m_oLastString)<nDataLen?sizeof(m_oLastString):nDataLen );
				return SerialString( nDataLen, m_oLastString, nDataLen );
			}
			else								///< 基值存在，有作差值的条件
			{
				for( unsigned int n = 0; n < nDataLen; n++ )
				{
					if( *(pData+n) != m_oLastString[n] )
					{
						::memcpy( m_oLastString, pData, sizeof(m_oLastString)<nDataLen?sizeof(m_oLastString):nDataLen );
						return SerialString( nDataLen, (char*)pData + n, nDataLen - n );
					}
				}
			}
		}
		break;
	case EncodePolicyDefinition::CORRELATE:
		{
			if( NULL != m_policyDefinition.RelatedField() )
			{
				char*	pszLastString = m_policyDefinition.RelatedField()->GetLastString();

				for( unsigned int n = 0; n < nDataLen; n++ )
				{
					if( *(pData+n) != pszLastString[n] )
					{
						return SerialString( nDataLen, (char*)pData + n, nDataLen - n );
					}
				}
			}
		}
		break;
	}

	return SerialString( nDataLen );
}

SerialNumber MsgField::X_CompressNumber( const char* pData )
{
	switch( m_policyDefinition.Operation() )
	{
	case EncodePolicyDefinition::INDEPENDENCE:
		{
			m_oLastNumber << pData;
			return m_oLastNumber;
		}
	case EncodePolicyDefinition::COUNTERPART:
		{
			if( 0 == AddAndReturnRefCnt() )		///< 基值不存在的情况（即Frame中的第一个包）
			{
				m_oLastNumber << pData;
				return m_oLastNumber;
			}
			else								///< 基值存在，有作差值的条件
			{
				SerialNumber	oRetNum( pData, m_fdDefinition.Type() );

				(__int64&)oRetNum = (__int64)oRetNum - (__int64)m_oLastNumber;
				m_oLastNumber << pData;

				return oRetNum;
			}
		}
	case EncodePolicyDefinition::CORRELATE:
		{
			if( NULL != m_policyDefinition.RelatedField() )
			{
				SerialNumber	oRetNum( 0, this );

				oRetNum << pData;
				(__int64&)oRetNum = (__int64)oRetNum - (__int64)(m_policyDefinition.RelatedField()->GetLastNumber());
				m_oLastNumber << pData;

				return oRetNum;
			}
		}
	}

	return SerialNumber();
}

int MsgField::X_UnCompressNumber( SerialNumber& oNumber, char* pData, size_t nDataLen )
{
	switch( m_policyDefinition.Operation() )
	{
	case EncodePolicyDefinition::INDEPENDENCE:
		{
			oNumber >> pData;
			m_oLastNumber << pData;
			return 0;
		}
	case EncodePolicyDefinition::COUNTERPART:
		{
			if( 0 == AddAndReturnRefCnt() )		///< 基值不存在的情况（即Frame中的第一个包）
			{
				oNumber >> pData;
				m_oLastNumber << pData;
			}
			else								///< 基值存在，有作差值的条件
			{
				(__int64&)m_oLastNumber = (__int64)m_oLastNumber + (__int64)oNumber;
				m_oLastNumber >> pData;
			}

			return 0;
		}
	case EncodePolicyDefinition::CORRELATE:
		{
			if( NULL != m_policyDefinition.RelatedField() )
			{
				SerialNumber	oRetNum( (__int64)(m_policyDefinition.RelatedField()->GetLastNumber()), m_policyDefinition.RelatedField()->FieldDefinition().Type() );

				(__int64&)oRetNum = (__int64)oRetNum + (__int64)oNumber;
				oRetNum >> pData;
				if( m_policyDefinition.RelatedField() == this )
				{
					m_policyDefinition.RelatedField()->GetLastNumber() << pData;
				}

				return 0;
			}
		}
	}

	return -1;
}

int MsgField::X_UnCompressString( SerialString& oString, char* pData, size_t nDataLen )
{
	switch( m_policyDefinition.Relation() )
	{
	case EncodePolicyDefinition::INDEPENDENCE:
		return 0;
	case EncodePolicyDefinition::COUNTERPART:
		{
			if( 0 == AddAndReturnRefCnt() )		///< 基值不存在的情况（即Frame中的第一个包）
			{
				::memcpy( m_oLastString, oString.pszStr, sizeof(m_oLastString)<nDataLen?sizeof(m_oLastString):nDataLen );
				m_oLastString[nDataLen] = '\0';
				return 0;
			}
			else								///< 基值存在，有作差值的条件
			{
				if( 0 == oString.nStrLen )
				{
					::strcpy( pData, m_oLastString );
				}
				else
				{
					::strncpy( m_oLastString + (nDataLen-oString.nStrLen), oString.pszStr, oString.nStrLen );
					m_oLastString[nDataLen] = '\0';
					::strcpy( pData, m_oLastString );
				}

				return 0;
			}
		}
		break;
	case EncodePolicyDefinition::CORRELATE:
		{
			if( NULL != m_policyDefinition.RelatedField() )
			{
				char*	pszLastString = m_policyDefinition.RelatedField()->GetLastString();

				if( 0 == oString.nStrLen )
				{
					::strcpy( pData, pszLastString );
				}
				else
				{
					::strncpy( pszLastString + (nDataLen-oString.nStrLen), oString.pszStr, oString.nStrLen );
					pszLastString[nDataLen] = '\0';
					::strcpy( pData, pszLastString );
				}

				return 0;
			}
		}
		break;
	}

	return -1;
}


///<---------------------------------------------------------------------------------


MsgField& Message::Grab( std::string sFieldName, FdDefinition::FIELD_TYPE nType, size_t nFieldLen
						, EncodePolicyDefinition::OPERATOR nOperator, EncodePolicyDefinition::RELATION nRelation
						, const MsgField* pRelateField, unsigned short nPrecision )
{
	if( NULL == m_pMsgPolicy )
	{
		throw std::string( "Message::Grab() : invalid policy obj. address. " );
	}

	if( sFieldName == "code" )
	{
		if( s_nDebugFlag > 0 ) {
			::printf( " ---> assign index flag(%d) to 1 \n", m_bIndexByCode );
		}

		m_bIndexByCode = true;
	}

	MsgFieldObjCache&	refCache = m_pMsgPolicy->GetFieldCache();

	return *(::new (refCache()) MsgField( refCache.GetCurrentNum(), m_pMsgPolicy
										, sFieldName, nType, nFieldLen
										, nOperator, nRelation, pRelateField, nPrecision ));
}

Message::Message( std::string sMsgName, MsgPolicy *pPolicy )
 : m_sMsgName( sMsgName ), m_nFieldNum( 0 ), m_pMsgPolicy( pPolicy )
 , m_bIndexByCode( false )
{
	::memset( m_lstField, 0, sizeof(m_lstField) );
}

MsgField& Message::operator += ( const MsgField& fd )
{
	m_lstField[m_nFieldNum] = (MsgField*)&fd;

	return *m_lstField[m_nFieldNum++];
}

MsgField* Message::operator[] ( unsigned int nIndex )
{
	if( nIndex >= m_nFieldNum )
	{
		return NULL;
	}

	return m_lstField[nIndex];
}

unsigned int Message::GetFieldNum() const
{
	return m_nFieldNum;
}

MsgField* Message::QueryFieldByFName( const std::string& sCorrelate, const std::string& sCounterpart )
{
	std::string	sFieldName = sCorrelate;

	if( sFieldName == "" ) {
		sFieldName = sCounterpart;
	}

	if( sFieldName == "" ) {
		return NULL;
	}

	for( unsigned int n = 0; n < m_nFieldNum; n++ )
	{
		if( true == m_lstField[n]->FieldDefinition().Compare( sFieldName ) )
		{
			if( s_nDebugFlag > 0 ) {
				::printf( "relate: %s \n", sFieldName.c_str() );
			}

			return m_lstField[n];
		}
	}

	return NULL;
}


///<---------------------------------------------------------------------------------

RefCounter::RefCounter()
{
	Release();
}

void RefCounter::Release()
{
	::memset( m_lstRefCounter, 0, sizeof(m_lstRefCounter) );
}

void RefCounter::ResetCounter()
{
	::memset( m_lstRefCounter, 0, sizeof(m_lstRefCounter) );
}

int& RefCounter::operator[]( unsigned int nFieldID )
{
	if( nFieldID >= sizeof(m_lstRefCounter)/sizeof(int) )
	{
		static	int	nTmp = -100;
		nTmp = -100;
		return nTmp;
	}

	return m_lstRefCounter[nFieldID];
}


///< --------------------------------------------------------------------------


MsgFieldObjCache::MsgFieldObjCache( unsigned int nMaxFieldNum )
 : m_nMsgFieldsObjNumber( 0 ), m_pMsgFieldObjBuffer( NULL ), m_nMaxFieldsObjNumber( nMaxFieldNum )
{
}

MsgFieldObjCache::~MsgFieldObjCache()
{
	Release();
}

bool MsgFieldObjCache::Instance()
{
	Release();

	m_pMsgFieldObjBuffer = (MsgField*)::malloc( (m_nMaxFieldsObjNumber + 1)*sizeof(MsgField) );

	if( NULL == m_pMsgFieldObjBuffer )
	{
		m_pMsgFieldObjBuffer = NULL;
		::printf( "MsgFieldObjCache::MsgFieldObjCache() : error in malloc() \n" );
		return false;
	}

	return true;
}

void MsgFieldObjCache::Release()
{
	if( NULL != m_pMsgFieldObjBuffer )
	{
		for( unsigned int n = 0; n < m_nMsgFieldsObjNumber; n++ )
		{
			MsgField*	pFreeField = (MsgField*)(m_pMsgFieldObjBuffer + n);
			pFreeField->~MsgField();
		}

		::free( m_pMsgFieldObjBuffer );
		m_pMsgFieldObjBuffer = NULL;
		m_nMsgFieldsObjNumber = 0;
	}
}

MsgField* MsgFieldObjCache::operator()()
{
	if( NULL == m_pMsgFieldObjBuffer )
	{
		::printf( "MsgFieldObjCache::operator() : invalid pointer \n" );
		return NULL;
	}

	if( m_nMaxFieldsObjNumber <= (m_nMsgFieldsObjNumber+3) )
	{
		::printf( "MsgFieldObjCache::operator() : no more space \n" );
		return NULL;
	}

	return m_pMsgFieldObjBuffer + m_nMsgFieldsObjNumber++;
}

unsigned int MsgFieldObjCache::GetCurrentNum()
{
	return m_nMsgFieldsObjNumber;
}


///< --------------------------------------------------------------------------


MsgPolicy::MsgPolicy()
 : m_nBaseDate( 20161010 ), m_nBaseTime( 120000 )
{
}

MsgPolicy::~MsgPolicy()
{
	Release();
}

void MsgPolicy::Release()
{
	m_nBaseDate = 20161010;
	m_nBaseTime = 120000;
	m_lstMessage.clear();
	m_oFieldRefCounter.Release();
}

unsigned int MsgPolicy::GetBaseDate()
{
	return m_nBaseDate;
}

unsigned int MsgPolicy::GetBaseTime()
{
	return m_nBaseTime;
}

int MsgPolicy::Initialize( const char* pszTemplateFile )
{
	try
	{
		CMarkupSTL	xmlparser;

		Release();

		if( false == m_oFieldObjCache.Instance() )
		{
			::printf( "MsgPolicy::Initialize() : failed 2 allocate msg field cache.\n" );
			return -100;
		}

		if( !xmlparser.Load( pszTemplateFile ) )
		{
			::printf( "MsgPolicy::Initialize() : template file isn't exists. [%s] \n", pszTemplateFile );
			return -1;
		}

		if( !xmlparser.IsWellFormed() )
		{
			::printf( "MsgPolicy::Initialize() : invalid .xml format. \n" );
			return -2;
		}

		::printf( "MsgPolicy::Initialize() : template file loaded [%s] \n", pszTemplateFile );

		if( !xmlparser.FindElem( "policy" ) )
		{
			::printf( "MsgPolicy::Initialize() : miss node [policy].\n" );
			return -100;
		}

		//xmlparser.IntoElem();
		std::string	sVersion = xmlparser.GetAttrib( "version" );
		std::string sBaseDate = xmlparser.GetAttrib( "baseDate" );
		std::string sBaseTime = xmlparser.GetAttrib( "baseTime" );
		if( sBaseDate != "" )			m_nBaseDate = ::atol( sBaseDate.c_str() );
		if( sBaseTime != "" )			m_nBaseTime = ::atol( sBaseTime.c_str() );
		::printf( "MsgPolicy::Initialize() : version = %s. base date = %u, base time = %u\n", sVersion.c_str(), m_nBaseDate, sBaseTime );

		while( xmlparser.FindChildElem( "message" ) )
		{
			xmlparser.IntoElem();

			std::string	sMsgName = xmlparser.GetAttrib( "name" );
			int			nMsgID = ::atoi( xmlparser.GetAttrib( "id" ).c_str() );
			std::string	sDebugFlag = xmlparser.GetAttrib( "dbg" );

			if( nMsgID < 0 ) {
				xmlparser.OutOfElem();
				continue;
			}

			if( sDebugFlag == "" ) {
				s_nDebugFlag = 0;
			} else {
				s_nDebugFlag = ::atol( sDebugFlag.c_str() );
			}

			if( s_nDebugFlag > 0 ) {
				::printf( "\n --------- [Message] Name = %s, ID = %d ---------------- \n", sMsgName.c_str(), nMsgID );
			}

			Message		&msg = GrabNewMessage( nMsgID, sMsgName.c_str() );
			int			nMessageSize = LoadSubPolicy( xmlparser, msg, NULL );
			xmlparser.OutOfElem();

			if( s_nDebugFlag > 0 ) {
				::printf( " --------- [Message] SIZE = %u ---------------- \n", nMessageSize );
			}
		}
	}
	catch( std::exception& err )
	{
		::printf( "%s \n", err.what() );
		return -10;
	}

	return 0;
}

int MsgPolicy::LoadSubPolicy( CMarkupSTL& xmlparser, Message& msg, MsgField* pSubField )
{
	int		nMessageLen = 0;		///< 每个Message长度统计

	xmlparser.IntoElem();
	while( true == xmlparser.FindElem() )
	{
		MsgField*			pNewSubField = NULL;
		std::string			sOperator = Str2Lower( xmlparser.GetData() );
		std::string			sFieldType = Str2Lower( xmlparser.GetTagName()  );
		std::string			sFieldLen = Str2Lower( xmlparser.GetAttrib( "len" ) );
		std::string			sFieldName = Str2Lower( xmlparser.GetAttrib( "name" ) );
		std::string			sPrecision = Str2Lower( xmlparser.GetAttrib( "precision" ) );
		std::string			sCorrelate = Str2Lower( xmlparser.GetAttrib( "correlate" ) );
		std::string			sCounterpart = Str2Lower( xmlparser.GetAttrib( "counterpart" ) );
		FdDefinition::FIELD_TYPE			eDataType = Str2FieldType( sFieldType );
		EncodePolicyDefinition::OPERATOR	eOP = Str2OP( sOperator );
		int					nDataLen = Str2FieldTypeNum( eDataType, sFieldLen );
		EncodePolicyDefinition::RELATION	eRelation = Str2Relation( sCounterpart, sCorrelate );
		int					nPrecision = Str2Precision( sPrecision );
		MsgField*			pRelateField = msg.QueryFieldByFName( sCorrelate, sCounterpart );

		if( s_nDebugFlag > 0 ) {
			printf( "[%s] name=%s, len=%d, correlate=%s, counterpart=%s, operator=%s\n"
					, sFieldType.c_str(), sFieldName.c_str(), nDataLen, sCorrelate.c_str(), sCounterpart.c_str(), sOperator.c_str() );
		}

		if( NULL == pSubField ) {
			pNewSubField = &(msg += msg.Grab( sFieldName, eDataType, nDataLen, eOP, eRelation, pRelateField, nPrecision ));
		} else {
			pNewSubField = &(*pSubField += msg.Grab( sFieldName, eDataType, nDataLen, eOP, eRelation, pRelateField, nPrecision ));
		}

		if( FdDefinition::RAWT_SEQUENCE != eDataType )
		{
			pNewSubField = NULL;
			nMessageLen += CalFieldLength( eDataType, nDataLen );	///< 累计非SEQUENCE字段的长度
		}

		int	nSequenceLen = LoadSubPolicy( xmlparser, msg, pNewSubField );

		if( FdDefinition::RAWT_SEQUENCE == eDataType )
		{
			nMessageLen += (nSequenceLen * nDataLen);				///< 累计SEQUENCE字段的长度 = SEQUENCE_LENGTH * ITEMS_TOTAL_LEN
		}
	}
	xmlparser.OutOfElem();

	return nMessageLen;
}

MsgFieldObjCache& MsgPolicy::GetFieldCache()
{
	return m_oFieldObjCache;
}

RefCounter& MsgPolicy::GetRefCounter()
{
	return m_oFieldRefCounter;
}

Message& MsgPolicy::GrabNewMessage( int nMsgID, std::string sMsgName )
{
	unsigned int	nInsertPosition = m_lstMessage.size();

	m_lstMsgID2Index[nMsgID] = nInsertPosition;
	m_lstMessage.push_back( Message( sMsgName, this ) );

	return m_lstMessage[nInsertPosition];
}

Message* MsgPolicy::operator[] ( unsigned int nMsgID )
{
	if( nMsgID >= 0xFFFF )		///< message id最大不能超过unsigned short即0xFFFF
	{
		return NULL;
	}

	unsigned short	nMsgIndex = m_lstMsgID2Index[nMsgID];

	if( nMsgIndex >= m_lstMessage.size() )
	{
		return NULL;
	}

	return &(m_lstMessage[ m_lstMsgID2Index[nMsgID] ]);
}






