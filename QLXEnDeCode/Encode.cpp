#include <new>
#include <math.h>
#include "Encode.h"


Encode::Encode()
 : m_pszBuffer( NULL )
{
}

int Encode::Initialize( const char* pszTemplateFile, IPbOperation** pListOperation, int nListSize )
{
	::printf( "Encode::Initialize() : begin \n" );
	int	nRet = m_oMsgPolicy.Initialize( pszTemplateFile );

	if( 0 == nRet )
	{
		for( int n = 0; n < nListSize; n++ )
		{
			if( 0 != (nRet=m_oSerializer.BindOperation( *(pListOperation+n) )) )
			{
				return nRet;
			}
		}
	}

	return nRet;
}

void Encode::Release()
{
}

int Encode::Attach2Buffer( char* pBuffer, unsigned int nBufferLen )
{
	m_oSerializer.Attach( (unsigned char*)pBuffer, nBufferLen );

	m_oMsgPolicy.GetRefCounter().ResetCounter();

	return 0;
}

int Encode::OnEncodeSequenceField( MsgField& rField, const char *pData, unsigned int nLen )
{	// { INT8, INT16, INT32, INT64, UINT8, UINT16, UINT32, UINT64, STRING, DOUBLE, SEQUENCE }
	int			nError = -1;
	int			nOffset = 0;

	for( unsigned int i = 0; i < rField.FieldDefinition().SizeOfType(); i++ )
	{
		for( int n = 0; n < rField.Size(); n++ )
		{
			const MsgField	*pSubField = rField[n];
			if( NULL == pSubField )
			{
				::printf( "Encode::OnEncodeSequenceField() : invalid subfield pointer \n" );
				return 0;
			}

			if( i == 0 )	///< 对第一个进来的值,将前值变量清空
			{
				if( EncodePolicyDefinition::CORRELATE == pSubField->EncodeRuleDefinition().Relation() )
				{
					((MsgField*)pSubField)->GetConvertUnignedMark() = true;
				}
			}

			if( 0 > (nError = OnEncodeField( *((MsgField*)pSubField), pData + nOffset, nLen - nOffset )) )
			{
				return nError;
			}

			nOffset += nError;
		}
	}

	return nOffset;
}

int Encode::OnEncodeField( MsgField& rField, const char *pData, unsigned int nLen )
{
	if( FdDefinition::RAWT_SEQUENCE == rField.FieldDefinition().Type() )
	{
		int	nOffset = 0;
		if( 0 > (nOffset=OnEncodeSequenceField( rField, pData, nLen ))  )
		{
			return -100;
		}

		return nOffset;
	}
	else
	{
		switch( rField.FieldDefinition().Type() )
		{
		case FdDefinition::RAWT_INT8:
		case FdDefinition::RAWT_UINT8:
		case FdDefinition::RAWT_INT16:
		case FdDefinition::RAWT_UINT16:
		case FdDefinition::RAWT_INT32:
		case FdDefinition::RAWT_UINT32:
		case FdDefinition::RAWT_INT64:
		case FdDefinition::RAWT_UINT64:
			{
				m_oSerializer << rField.X_CompressNumber( pData );
				return rField.FieldDefinition().SizeOfType();
			}
		case FdDefinition::RAWT_DATE:
			{
				int	nDate = *((unsigned int*)pData);
				nDate -= m_oMsgPolicy.GetBaseDate();
				m_oSerializer << SerialNumber( nDate, FdDefinition::RAWT_DATE );
				return rField.FieldDefinition().SizeOfType();
			}
		case FdDefinition::RAWT_TIME:
			{
				int	nTime = *((unsigned int*)pData);
				nTime -= m_oMsgPolicy.GetBaseTime();
				m_oSerializer << SerialNumber( nTime, FdDefinition::RAWT_TIME );
				return rField.FieldDefinition().SizeOfType();
			}
		case FdDefinition::RAWT_DOUBLE:
			{
				__int64 val = *((double*)pData) * pow( 10., rField.FieldDefinition().Precision() );
				m_oSerializer << SerialNumber( val, FdDefinition::RAWT_DOUBLE );
				return rField.FieldDefinition().SizeOfType();
			}
		case FdDefinition::RAWT_STRING:
			{
				m_oSerializer << rField.X_CompressString( (char*)pData, rField.FieldDefinition().SizeOfType() );
				return rField.FieldDefinition().SizeOfType();
			}
		case FdDefinition::RAWT_VECTOR:
			{
				m_oSerializer << SerialBytes( rField.FieldDefinition().SizeOfType(), (char*)pData, rField.FieldDefinition().SizeOfType() );
				return rField.FieldDefinition().SizeOfType();
			}
		default:
			return -20;
		}
	}

	return 0;
}

int Encode::EncodeMessage( unsigned short nMsgID, const char *pData, unsigned int nLen )
{	// { INT8, INT16, INT32, INT64, UINT8, UINT16, UINT32, UINT64, STRING, DOUBLE, SEQUENCE }
	int				nError = -1;
	int				nOffset = 0;
	Message			*pMsg = m_oMsgPolicy[nMsgID];

	if( NULL != pMsg )
	{
		m_oSerializer.NewMessage( nMsgID );

		for( unsigned int n = 0; n < pMsg->GetFieldNum(); n++ )
		{
			MsgField	*pField = (*pMsg)[n];
			if( NULL == pField )
			{
				::printf( "Encode::EncodeMessage() : invalid subfield pointer. \n" );
				return 0;
			}

			if( 0 > (nError = OnEncodeField( *pField, pData + nOffset, nLen - nOffset )) )
			{
				return nError;
			}

			nOffset += nError;
		}

		m_oSerializer.Serialize();
	}

	return m_oSerializer.GetDataSize();
}





