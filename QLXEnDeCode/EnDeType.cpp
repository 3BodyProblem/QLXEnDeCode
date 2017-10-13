#include "EnDeType.h"
#include "EnDePolicy.h"


FdDefinition::FdDefinition()
 : m_nFieldID( 0 ), m_nFieldType( RAWT_ISNONE ), m_nBytesNum( 0 ), m_nPrecision( 0 )
{
	::memset( m_sFieldName, 0, sizeof(m_sFieldName) );
}

FdDefinition::FdDefinition( unsigned int nID, const char* pszName, short nNameLen, FdDefinition::FIELD_TYPE nType, size_t nBytesNum, unsigned short nPrecision )
 : m_nFieldID( nID ), m_nFieldType( nType ), m_nBytesNum( nBytesNum ), m_nPrecision( nPrecision )
{
	::memset( m_sFieldName, 0, sizeof(m_sFieldName) );
	::memcpy( m_sFieldName, pszName, nNameLen<sizeof(m_sFieldName)?nNameLen:sizeof(m_sFieldName) );

	switch( m_nFieldType )
	{
	case FdDefinition::RAWT_UINT8:
	case FdDefinition::RAWT_INT8:
		m_nBytesNum = 1;
		break;
	case FdDefinition::RAWT_UINT16:
	case FdDefinition::RAWT_INT16:
		m_nBytesNum = 2;
		break;
	case FdDefinition::RAWT_DATE:
	case FdDefinition::RAWT_TIME:
	case FdDefinition::RAWT_UINT32:
	case FdDefinition::RAWT_INT32:
		m_nBytesNum = 4;
		break;
	case FdDefinition::RAWT_DOUBLE:
	case FdDefinition::RAWT_INT64:
	case FdDefinition::RAWT_UINT64:
		m_nBytesNum = 8;
		break;
	}
}

FdDefinition::FdDefinition( FdDefinition& FromObj )
{
	m_nFieldID = FromObj.ID();
	m_nFieldType = FromObj.Type();
	m_nBytesNum = FromObj.SizeOfType();
	m_nPrecision = FromObj.Precision();
	FromObj.GetFieldName( m_sFieldName );
}

unsigned int FdDefinition::ID() const
{
	return m_nFieldID;
}

FdDefinition::FIELD_TYPE FdDefinition::Type() const
{
	return m_nFieldType;
}

size_t FdDefinition::SizeOfType() const
{
	return m_nBytesNum;
}

unsigned short	FdDefinition::Precision() const
{
	return m_nPrecision;
}

void FdDefinition::GetFieldName( char (&pszName)[64] )
{
	::memcpy( pszName, m_sFieldName, sizeof(m_sFieldName) );
}

bool FdDefinition::Compare( std::string& sFieldName ) const
{
	if( ::strncmp( sFieldName.c_str(), m_sFieldName, sFieldName.length() ) == 0 )
	{
		return true;
	}

	return false;
}


SerialString::SerialString( short nRawLen, char *pStr, short nLen )
: pszStr( pStr ), nStrLen( nLen ), nDefineLen( nRawLen )
{
}


SerialBytes::SerialBytes( short nRawLen, char *pStr, short nLen )
: pszStr( pStr ), nStrLen( nLen ), nDefineLen( nRawLen )
{
}


SerialNumber::SerialNumber( __int64 nVal, enum FdDefinition::FIELD_TYPE nType )
: nDataType( nType ), nInt64( nVal )
{
}

SerialNumber::SerialNumber( const char* pBuf, enum FdDefinition::FIELD_TYPE nType )
: nDataType( nType )
{
	switch( nDataType )
	{
	case FdDefinition::RAWT_UINT64:
	case FdDefinition::RAWT_INT64:
	case FdDefinition::RAWT_DOUBLE:
		nUInt64 = *((unsigned __int64*)pBuf);
	case FdDefinition::RAWT_DATE:
	case FdDefinition::RAWT_TIME:
	case FdDefinition::RAWT_INT32:
	case FdDefinition::RAWT_UINT32:
		nUInt32 = *((unsigned int*)pBuf);
		break;
	case FdDefinition::RAWT_UINT16:
	case FdDefinition::RAWT_INT16:
		nUInt16 = *((unsigned short*)pBuf);
		break;
	case FdDefinition::RAWT_UINT8:
	case FdDefinition::RAWT_INT8:
		nUInt8 = *((unsigned char*)pBuf);
		break;
	default:
		nUInt64 = 0;
	}
}

SerialNumber::SerialNumber( __int64 nVal, MsgField* pMFd )
: nDataType( pMFd->FieldDefinition().Type() )
{
	bool	bMarkSign = false;

	switch( pMFd->EncodeRuleDefinition().Operation() )
	{
	case EncodePolicyDefinition::CORRELATE:
		{
			if( NULL == pMFd->EncodeRuleDefinition().RelatedField() )
			{
				break;
			}

			bMarkSign = true;
			if( true == pMFd->EncodeRuleDefinition().RelatedField()->GetConvertUnignedMark() )
			{
				pMFd->EncodeRuleDefinition().RelatedField()->GetConvertUnignedMark() = false;
				pMFd->EncodeRuleDefinition().RelatedField()->GetLastString()[0] = '\0';
				(__int64&)(pMFd->EncodeRuleDefinition().RelatedField()->GetLastNumber()) = 0;
				bMarkSign = false;
			}
		}
		break;
	}

	///< assign value
	switch( nDataType )
	{
	case FdDefinition::RAWT_UINT64:
		if( true == bMarkSign )
		{
			nInt64 = nVal;
			nDataType = FdDefinition::RAWT_INT64;
		}
		else
		{
			nUInt64 = nVal;
		}
		break;
	case FdDefinition::RAWT_INT64:
		nInt64 = nVal;
		break;
	case FdDefinition::RAWT_UINT32:
		if( true == bMarkSign )
		{
			nInt32 = nVal;
			nDataType = FdDefinition::RAWT_INT32;
		}
		else
		{
			nUInt32 = nVal;
		}
		break;
	case FdDefinition::RAWT_DATE:
	case FdDefinition::RAWT_TIME:
	case FdDefinition::RAWT_INT32:
		nInt32 = nVal;
		break;
	case FdDefinition::RAWT_UINT16:
		if( true == bMarkSign )
		{
			nInt16 = nVal;
			nDataType = FdDefinition::RAWT_INT16;
		}
		else
		{
			nUInt16 = nVal;
		}
		break;
	case FdDefinition::RAWT_INT16:
		nInt16 = nVal;
		break;
	case FdDefinition::RAWT_UINT8:
		if( true == bMarkSign )
		{
			nInt8 = nVal;
			nDataType = FdDefinition::RAWT_INT8;
		}
		else
		{
			nUInt8 = nVal;
		}
		break;
	case FdDefinition::RAWT_INT8:
		nInt8 = nVal;
		break;
	case FdDefinition::RAWT_DOUBLE:
		{
			nDataType = FdDefinition::RAWT_INT64;
			nInt64 = nVal;
		}
		break;
	default:
		nDataType = FdDefinition::RAWT_ISNONE;
		return;
	}
}

bool SerialNumber::IsSigned() const
{
	if( nDataType < FdDefinition::RAWT_UINT8 )
	{
		return true;
	}

	return false;
}

bool SerialNumber::operator << ( const char* pBuf )
{
	switch( nDataType )
	{
	case FdDefinition::RAWT_UINT64:
	case FdDefinition::RAWT_INT64:
	case FdDefinition::RAWT_DOUBLE:
		nUInt64 = *((unsigned __int64*)pBuf);
	case FdDefinition::RAWT_DATE:
	case FdDefinition::RAWT_TIME:
	case FdDefinition::RAWT_INT32:
	case FdDefinition::RAWT_UINT32:
		nUInt32 = *((unsigned int*)pBuf);
		break;
	case FdDefinition::RAWT_UINT16:
	case FdDefinition::RAWT_INT16:
		nUInt16 = *((unsigned short*)pBuf);
		break;
	case FdDefinition::RAWT_UINT8:
	case FdDefinition::RAWT_INT8:
		nUInt8 = *((unsigned char*)pBuf);
		break;
	default:
		return false;
	}

	return true;
}

bool SerialNumber::operator >>( char* pBuf )
{
	switch( nDataType )
	{
	case FdDefinition::RAWT_UINT64:
	case FdDefinition::RAWT_INT64:
	case FdDefinition::RAWT_DOUBLE:
		*((unsigned __int64*)pBuf) = nUInt64;
	case FdDefinition::RAWT_DATE:
	case FdDefinition::RAWT_TIME:
	case FdDefinition::RAWT_INT32:
	case FdDefinition::RAWT_UINT32:
		*((unsigned int*)pBuf) = nUInt32;
		break;
	case FdDefinition::RAWT_UINT16:
	case FdDefinition::RAWT_INT16:
		*((unsigned short*)pBuf) = nUInt16;
		break;
	case FdDefinition::RAWT_UINT8:
	case FdDefinition::RAWT_INT8:
		*((unsigned char*)pBuf) = nUInt8;
		break;
	default:
		return false;
	}

	return true;
}

SerialNumber::operator __int64() const
{
	switch( nDataType )
	{
	case FdDefinition::RAWT_UINT64:
		return nUInt64;
	case FdDefinition::RAWT_INT64:
	case FdDefinition::RAWT_DOUBLE:
	case FdDefinition::RAWT_SEQUENCE:
		return nInt64;
	case FdDefinition::RAWT_UINT32:
		return nUInt32;
	case FdDefinition::RAWT_DATE:
	case FdDefinition::RAWT_TIME:
	case FdDefinition::RAWT_INT32:
		return nInt32;
	case FdDefinition::RAWT_UINT16:
		return nUInt16;
	case FdDefinition::RAWT_INT16:
		return nInt16;
	case FdDefinition::RAWT_UINT8:
		return nUInt8;
	case FdDefinition::RAWT_INT8:
		return nInt8;
	default:
		return 0;
	}
}

SerialNumber::operator unsigned __int64() const
{
	switch( nDataType )
	{
	case FdDefinition::RAWT_UINT64:
	case FdDefinition::RAWT_SEQUENCE:
		return nUInt64;
	case FdDefinition::RAWT_INT64:
	case FdDefinition::RAWT_DOUBLE:
		return nInt64;
	case FdDefinition::RAWT_UINT32:
		return nUInt32;
	case FdDefinition::RAWT_DATE:
	case FdDefinition::RAWT_TIME:
	case FdDefinition::RAWT_INT32:
		return nInt32;
	case FdDefinition::RAWT_UINT16:
		return nUInt16;
	case FdDefinition::RAWT_INT16:
		return nInt16;
	case FdDefinition::RAWT_UINT8:
		return nUInt8;
	case FdDefinition::RAWT_INT8:
		return nInt8;
	default:
		return 0;
	}
}

SerialNumber::operator __int64&()
{
	return nInt64;
}

SerialNumber::operator unsigned __int64&()
{
	return nUInt64;
}

bool SerialNumber::IsPositive() const
{
	switch( nDataType )
	{
	case FdDefinition::RAWT_UINT64:
		return nUInt64 >= 0;
	case FdDefinition::RAWT_INT64:
		return nInt64 >= 0;
	case FdDefinition::RAWT_UINT32:
		return nUInt32 >= 0;
	case FdDefinition::RAWT_DATE:
	case FdDefinition::RAWT_TIME:
	case FdDefinition::RAWT_INT32:
		return nInt32 >= 0;
	case FdDefinition::RAWT_UINT16:
		return nUInt16 >= 0;
	case FdDefinition::RAWT_INT16:
		return nInt16 >= 0;
	default:
		return true;
	}
}




