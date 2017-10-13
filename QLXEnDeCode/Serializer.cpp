#include <new>
#include <math.h>
#include "Serializer.h"
#include "demo.pb.h"


FrameSerializer::FrameSerializer()
: m_buf( NULL ), m_buflen( 0 ), m_curpos( 0 )
, m_nDataBufSize( 0 ), m_nMsgID( 0 )
{
}

int	FrameSerializer::GetDataSize()
{
	return m_curpos;
}

int	FrameSerializer::BindOperation( IPbOperation* pOperation )
{
	if( NULL == pOperation )
	{
		return -1;
	}

	int	nMsgID = pOperation->MessageID();

	if( nMsgID < 0 || nMsgID > 0xFFFF )
	{
		return -2;
	}

	return m_Operations.AddOperation( pOperation );
}

bool FrameSerializer::NewMessage( unsigned short nMsgID )
{
	m_nDataBufSize = 0;
	if( NULL == m_buf )
	{
		m_nMsgID = 0;
		return false;
	}

	m_nMsgID = nMsgID;

	return true;
}

int FrameSerializer::Serialize()
{
	IPbOperation*	pOperation = m_Operations[m_nMsgID];

	if( NULL != pOperation )
	{
		*((unsigned short*)(m_buf+m_curpos)) = m_nMsgID;				///< assign id
		m_curpos += sizeof(unsigned short);
		unsigned short	*pnLen = (unsigned short*)(m_buf+m_curpos);		///< msg size
		m_curpos += sizeof(unsigned short);

		int	nBytesSize = pOperation->Serialize( (char*)m_pszDataBuf, sizeof(m_pszDataBuf)
												, (char*)(m_buf + m_curpos), (int)(m_buflen - m_curpos) );

		*pnLen = nBytesSize;
		m_curpos += nBytesSize;

		return 0;
	}

	return -1;
}

void FrameSerializer::Attach( unsigned char *ptr, size_t size )
{
	m_buf = ptr;
	m_buflen = size;
	m_curpos = 0;
}

FrameSerializer::operator char*()
{
	return (char*)m_buf;
}

int FrameSerializer::operator << ( const SerialString& val )
{
	::memcpy( m_pszDataBuf+m_nDataBufSize, val.pszStr, val.nStrLen );
	m_nDataBufSize += val.nDefineLen;

	return val.nDefineLen;
}

int FrameSerializer::operator << ( const SerialBytes& val )
{
	::memcpy( m_pszDataBuf+m_nDataBufSize, val.pszStr, val.nStrLen );
	m_nDataBufSize += val.nDefineLen;

	return val.nDefineLen;
}

int	FrameSerializer::operator << ( const SerialNumber& val )
{
	switch( val.nDataType )
	{
	case FdDefinition::RAWT_UINT64:
	case FdDefinition::RAWT_INT64:
	case FdDefinition::RAWT_DOUBLE:
		*((unsigned __int64*)(m_pszDataBuf+m_nDataBufSize)) = val.nUInt64;
		m_nDataBufSize += 8;
		return 8;
	case FdDefinition::RAWT_DATE:
	case FdDefinition::RAWT_TIME:
	case FdDefinition::RAWT_INT32:
	case FdDefinition::RAWT_UINT32:
		*((unsigned int*)(m_pszDataBuf+m_nDataBufSize)) = val.nUInt32;
		m_nDataBufSize += 4;
		return 4;
	case FdDefinition::RAWT_UINT16:
	case FdDefinition::RAWT_INT16:
		*((unsigned short*)(m_pszDataBuf+m_nDataBufSize)) = val.nUInt16;
		m_nDataBufSize += 2;
		return 2;
	case FdDefinition::RAWT_UINT8:
	case FdDefinition::RAWT_INT8:
		*((unsigned char*)(m_pszDataBuf+m_nDataBufSize)) = val.nUInt8;
		m_nDataBufSize += 1;
		return 1;
	default:
		return 0;
	}
}


//////////////////////////////////////////

FrameUnserializer::FrameUnserializer()
: m_buf( NULL ), m_buflen( 0 ), m_curpos( 0 )
{
}

int	FrameUnserializer::BindOperation( IPbOperation* pOperation )
{
	if( NULL == pOperation )
	{
		return -1;
	}

	int	nMsgID = pOperation->MessageID();

	if( nMsgID < 0 || nMsgID > 0xFFFF )
	{
		return -2;
	}

	return m_Operations.AddOperation( pOperation );
}

int	FrameUnserializer::GetDataSize()
{
	return m_buflen - m_curpos;
}

void FrameUnserializer::Attach( unsigned char *ptr, size_t size )
{
	m_buf = ptr;
	m_buflen = size;
	m_curpos = 0;
}

int FrameUnserializer::Unserialize()
{
	if( NULL == m_buf )
	{
		return -1;
	}

	unsigned short	nMsgID = *((unsigned short*)(m_buf+m_curpos));		///< assign id
	m_curpos += sizeof(unsigned short);

	unsigned short	nMsgSize = *((unsigned short*)(m_buf+m_curpos));	///< message size
	m_curpos += sizeof(unsigned short);

	m_nDataBufSize = 0;

	IPbOperation*	pOperation = m_Operations[nMsgID];
	if( NULL != pOperation )
	{
		int	nBytesSize = pOperation->Unserialize( (char*)(m_buf+m_curpos), nMsgSize/*(int)(m_buflen-m_curpos)*/
												, (char*)m_pszDataBuf, sizeof(m_pszDataBuf) );

		if( nMsgSize != nBytesSize )
		{
			return -2;
		}

		m_curpos += nBytesSize;
		m_nDataBufSize = 0;

		return nMsgID;
	}

	return -3;
}

int FrameUnserializer::operator >> ( SerialNumber& val )
{
	switch( val.nDataType )
	{
	case FdDefinition::RAWT_UINT64:
	case FdDefinition::RAWT_INT64:
	case FdDefinition::RAWT_DOUBLE:
		val.nUInt64 = *((unsigned __int64*)(m_pszDataBuf+m_nDataBufSize));
		m_nDataBufSize += 8;
		return 8;
	case FdDefinition::RAWT_DATE:
	case FdDefinition::RAWT_TIME:
	case FdDefinition::RAWT_INT32:
	case FdDefinition::RAWT_UINT32:
		val.nUInt32 = *((unsigned int*)(m_pszDataBuf+m_nDataBufSize));
		m_nDataBufSize += 4;
		return 4;
	case FdDefinition::RAWT_UINT16:
	case FdDefinition::RAWT_INT16:
		val.nUInt16 = *((unsigned short*)(m_pszDataBuf+m_nDataBufSize));
		m_nDataBufSize += 2;
		return 2;
	case FdDefinition::RAWT_UINT8:
	case FdDefinition::RAWT_INT8:
		val.nUInt8 = *((unsigned char*)(m_pszDataBuf+m_nDataBufSize));
		m_nDataBufSize += 1;
		return 1;
	default:
		return 0;
	}
}

int FrameUnserializer::operator >> ( SerialBytes& val )
{
	::memcpy( val.pszStr, m_pszDataBuf+m_nDataBufSize, val.nDefineLen );

	m_nDataBufSize += val.nDefineLen;

	return val.nDefineLen;
}

int FrameUnserializer::operator >> ( SerialString& val )
{
	::memcpy( val.pszStr, m_pszDataBuf+m_nDataBufSize, val.nDefineLen );

	if( *(val.pszStr + (val.nDefineLen-1)) == '\0' )
	{
		val.nStrLen = ::strlen( val.pszStr );
	}

	m_nDataBufSize += val.nDefineLen;

	return val.nDefineLen;
}





























