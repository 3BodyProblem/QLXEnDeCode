#include <new>
#include <math.h>
#include "Decode.h"


Decode::Decode()
 : m_pszBuffer( NULL )
{
}

int Decode::Initialize( const char* pszTemplateFile, IPbOperation** pListOperation, int nListSize )
{
	::printf( "Decode::Initialize() : begin \n" );
	int nRet = m_oMsgPolicy.Initialize( pszTemplateFile );

	if( 0 == nRet )
	{
		for( int n = 0; n < nListSize; n++ )
		{
			if( 0 != (nRet=m_oUnserializer.BindOperation( *(pListOperation+n) )) )
			{
				return nRet;
			}
		}
	}

	return nRet;
}

void Decode::Release()
{
}

int Decode::Attach2Buffer( char* pBuffer, unsigned int nBufferLen )
{
	m_oUnserializer.Attach( (unsigned char*)pBuffer, nBufferLen );

	m_oMsgPolicy.GetRefCounter().ResetCounter();

	return 0;
}

int Decode::OnDecodeSequenceField( MsgField& rField, char *pData, unsigned int nLen )
{	// { INT8, INT16, INT32, INT64, UINT8, UINT16, UINT32, UINT64, STRING, DOUBLE, SEQUENCE }
	int			nError = -1;
	int			nOffset = 0;

	for( int i = 0; i < rField.FieldDefinition().SizeOfType(); i++ )
	{
		for( int n = 0; n < rField.Size(); n++ )
		{
			const MsgField	*pSubField = rField[n];
			if( NULL == pSubField )
			{
				::printf( "Decode::OnDecodeSequenceField() : invalid subfield pointer \n" );
				return 0;
			}

			if( i == 0 )	///< 对第一个进来的值,将前值变量清空
			{
				if( EncodePolicyDefinition::CORRELATE == pSubField->EncodeRuleDefinition().Relation() )
				{
					((MsgField*)pSubField)->GetConvertUnignedMark() = true;
				}
			}

			if( 0 > (nError = OnDecodeField( *((MsgField*)pSubField), pData + nOffset, nLen - nOffset )) )
			{
				return nError;
			}

			nOffset += nError;
		}
	}

	return nOffset;
}

int Decode::OnDecodeField( MsgField& rField, char *pData, unsigned int nLen )
{
	if( FdDefinition::RAWT_SEQUENCE == rField.FieldDefinition().Type() )
	{
		int	nOffset = 0;
		if( 0 > (nOffset=OnDecodeSequenceField( rField, pData, nLen ))  )
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
				SerialNumber	nValue( 0, &rField );
				m_oUnserializer >> nValue;
				rField.X_UnCompressNumber( nValue, pData, nLen );
				return rField.FieldDefinition().SizeOfType();
			}
		case FdDefinition::RAWT_DATE:
			{
				SerialNumber	nValue( 0, &rField );
				m_oUnserializer >> nValue;
				*((int*)pData) = nValue.nInt32 + m_oMsgPolicy.GetBaseDate();
				return rField.FieldDefinition().SizeOfType();
			}
		case FdDefinition::RAWT_TIME:
			{
				SerialNumber	nValue( 0, &rField );
				m_oUnserializer >> nValue;
				*((int*)pData) = nValue.nInt32 + m_oMsgPolicy.GetBaseTime();
				return rField.FieldDefinition().SizeOfType();
			}
		case FdDefinition::RAWT_DOUBLE:
			{
				SerialNumber	nValue( 0, &rField );
				m_oUnserializer >> nValue;
				*((double*)pData) = (double)nValue.nInt64/* * 1.0 /*/ * pow( 10., -rField.FieldDefinition().Precision() );
				return rField.FieldDefinition().SizeOfType();
			}
		case FdDefinition::RAWT_STRING:
			{
				SerialString	oStr( rField.FieldDefinition().SizeOfType(), pData, rField.FieldDefinition().SizeOfType() );
				m_oUnserializer >> oStr;
				rField.X_UnCompressString( oStr, pData, rField.FieldDefinition().SizeOfType() );
				return rField.FieldDefinition().SizeOfType();
			}
		case FdDefinition::RAWT_VECTOR:
			{
				m_oUnserializer >> SerialBytes( rField.FieldDefinition().SizeOfType(), pData, rField.FieldDefinition().SizeOfType() );
				return rField.FieldDefinition().SizeOfType();
			}
		default:
			return -20;
		}
	}

	return 0;
}

int Decode::DecodeMessage( unsigned short& nMsgID, char *pData, unsigned int nLen )
{	// { INT8, INT16, INT32, INT64, UINT8, UINT16, UINT32, UINT64, STRING, DOUBLE, SEQUENCE }
	int				nError = -1;
	int				nOffset = 0;

	nMsgID = m_oUnserializer.Unserialize();
	Message			*pMsg = m_oMsgPolicy[nMsgID];

	if( NULL != pMsg )
	{
		for( unsigned int n = 0; n < pMsg->GetFieldNum(); n++ )
		{
			MsgField	*pField = (*pMsg)[n];

			if( 0 > (nError = OnDecodeField( *pField, pData + nOffset, nLen - nOffset )) )
			{
				return nError;
			}

			nOffset += nError;
		}
	}

	return m_oUnserializer.GetDataSize();
}






