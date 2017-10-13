#include "math.h"
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <algorithm>
#include "UADecoder.h"
#include "StepClient.h"
#include "FastDecoder.h"


unsigned __int64	g_nAccuRawDataSize = 0;
unsigned __int64	g_nAccuEncodeDataSize = 0;

#define MAX_STEPMSG_SIZE	(8192)

#define GETRARECODE if( nRate == 0 )									  \
					{													  \
						dRate = 1.f;									  \
					}													  \
					else{												  \
						dRate = pow( 10, nRate-pTemplateField->Decimal ); \
					}

#define GETRARESEQN if( nRate == 0 )									  \
					{													  \
						dRate = 1.f;									  \
					}													  \
					else{												  \
						dRate = pow( 10, nRate-pSequceField->Decimal );   \
					}

#define CODEEQUAL(a,b) (*((unsigned long*)(a))==*((unsigned long*)(b))&&*((unsigned short*)((a)+4))==*((unsigned short*)((b)+4)))


int GetStockType(const char (&code)[6])
{
	switch( code[0] )
	{
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	default:
		return 10;
	}
}


static	char	*pStepTempBuf = new char[MAXPARSERBUFLEN];		///< temp step cache

StepClient::StepClient() : m_parserlen(0), m_nUA1201Status(0)
{
	Release();
}

StepClient::~StepClient()
{
	Release();
}

int StepClient::Instance()
{
	return 0;
}

void StepClient::Release()
{
	m_nUA1201Status = 0;
	m_parserlen = 0;
	m_95len = 0;
	::memset( m_category, 0, sizeof(m_category) );
}

int	StepClient::inner_bulidmsg( tagStepComm_Head *lpHead, char *lpOutBuf, int sInSize )
{
	register int					errorcode;
	register int					offset = 0;
	unsigned long					ulTagname = 0;
	static char						tagname[1024];					///< 从数据流中得到的STEP数据的标签名
	static char						tagvalue[1024*4];				///< 从数据流中得到的STEP数据的标签内容
	static char						tempTimeBuf[32];				///< STEP数据的时间戳缓存串
	unsigned long					ulCategoryId = 0;
	int								checksize,msgsize;
	int								year,month,day,hour,min,sec;

	if( m_parserlen < 6 ) {
		return 0;
	}
	if( m_parserbuf[0] != '8' && m_parserbuf[1] != '=' ) {
		return -1;	//数据错误
	}
	if( (errorcode = inner_readsteptag( m_parserbuf, m_parserlen, offset, tagname, tagvalue) ) <= 0 ) {			///< 读取STEP协议版本号
		return errorcode;
	}
	offset += errorcode;
	if ( tagname[0] != '8' )
	{
		return -100;
	}
	if ( (errorcode = inner_readsteptag( m_parserbuf, m_parserlen, offset, tagname, tagvalue ) ) <= 0 ) {		///< 读取消息长度
		return errorcode;
	}

	offset += errorcode;
	if( tagname[0] != '9' ) {
		return -100;
	}
	checksize = offset + ::atol( tagvalue );
	msgsize = checksize + 7;

	if( msgsize > MAXPARSERBUFLEN ) {
		return -101;						//ERR_STEPMSG_MAXSIZE
	} else if( msgsize > m_parserlen ) {
		return 0;							//该STEP消息没有接收完成
	}

	if( (errorcode = inner_readsteptag( m_parserbuf, m_parserlen, msgsize - 7, tagname, tagvalue ) ) < 0 ) {	///< 读取校验码，对消息进行校验
		return(errorcode);
	}
	
	if( ::atol( tagname ) != 10 ) {
		return -100;
	}

	///< 开始解析头部其他字段 ///////// memset(lpHead,0x00,sizeof(tagStepComm_Head));
	while( (errorcode = inner_readsteptag( m_parserbuf, m_parserlen, offset, tagname, tagvalue ) ) >= 0 )
	{
		ulTagname = ::atol( tagname );
		if( ulTagname == 35 )
		{	///< 消息类型
			if( tagvalue[0] >= '0' && tagvalue[0] <= '9' )
			{
				lpHead->ulMsgType = ::atol( tagvalue );
			}
			else
			{
				lpHead->ulMsgType = ::atol( tagvalue + 2 );
			}
		}
		else if( ulTagname == 49 )
		{//发送方ID号
		}
		else if( ulTagname == 56 )
		{//目标ID号
		}
		//消息序号
		else if( ulTagname == 34 )
		{
			lpHead->ulSeqNo = ::atol( tagvalue );
		}
		//消息时间戳
		else if( ulTagname == 52 )
		{
			if ( tagvalue[0] == '\0' )
			{
				time_t			nNowT = ::time(NULL);
				struct	tm*		pTimeInfo = ::localtime( &nNowT );
				lpHead->nSendTime = pTimeInfo->tm_hour * 10000 + pTimeInfo->tm_min * 100 + pTimeInfo->tm_sec;
				lpHead->nSendDate = (pTimeInfo->tm_year + 1900) * 10000 + (pTimeInfo->tm_mon + 1) * 100 + pTimeInfo->tm_mday;
			}
			else
			{
				tempTimeBuf[0] = tagvalue[0];
				tempTimeBuf[1] = tagvalue[1];
				tempTimeBuf[2] = tagvalue[2];
				tempTimeBuf[3] = tagvalue[3];
				tempTimeBuf[4] = 0;
				tempTimeBuf[5] = tagvalue[4];
				tempTimeBuf[6] = tagvalue[5];
				tempTimeBuf[7] = 0;
				tempTimeBuf[8] = tagvalue[6];
				tempTimeBuf[9] = tagvalue[7];
				strcpy( tempTimeBuf+10, tagvalue+8 );
				year = atol(tempTimeBuf), month = atol(tempTimeBuf+5), day = atol(tempTimeBuf+8), hour= atol(tempTimeBuf+11), min = atol(tempTimeBuf+14), sec = atol(tempTimeBuf+17);
				lpHead->nSendTime = hour * 10000 + min * 100 + sec;
				lpHead->nSendDate = year * 10000 + month * 100 + day;
			}
		}
		//编码方式
		else if( ulTagname == 347 )
		{
			if( 0 == ::strcmp( tagvalue, "UTF-8" ) ) {
				lpHead->nEncodeType = 1;
			}
		}
		//最初发送方标识符（可加密），用于经第三方发送。
		else if( ulTagname == 115 )
		{}
		//最终接收方标识符（可加密），用于经第三方发送
		else if( ulTagname == 128 )
		{}
		//密文数据长度
		else if( ulTagname == 90 )
		{}
		//密文数据（紧跟密文数据长度域）
		else if ( ulTagname == 91 )
		{}
		//发送方子标识符（可加密）
		else if( ulTagname == 50 )
		{}
		//发送方方位标识符（可加密）
		else if( ulTagname == 142 )
		{}
		//接收方子标识符（可加密）
		else if( ulTagname == 57 )
		{}
		//接收方方位标识符（可加密）
		else if( ulTagname == 143 )
		{}
		//最初发送方子标识符（可加密）
		else if( ulTagname == 116 )
		{}
		//最初发送方方位标识符（可加密）
		else if( ulTagname == 144 )
		{}
		//最终接收方子标识符（可加密）
		else if( ulTagname == 129 )
		{}
		//最终接收方方位标识符（可加密）
		else if( ulTagname == 145 )
		{}
		//可能重复标志，重复发送时，作此标记。（可加密）
		else if( ulTagname == 43 )
		{}
		//可能重发标志。（可加密）
		else if ( ulTagname == 97 )
		{}
		//原始发送时间（可加密）
		else if ( ulTagname == 122 )
		{}
		//最后处理消息序号（可加密）
		else if ( ulTagname == 369 )
		{}
		//最初发送时间（用UTC 表示时间）
		else if ( ulTagname == 370 )
		{}
		//历史跳跃信息重复组，记录消息经第三方发送的历史， 每次经第三方发送为一个跳跃， 仅当OnBehalfOfCompID 使用时有效，主要用于跟踪消息的路径。
		else if ( ulTagname == 627 )
		{}
		//取值第三方的信息
		else if ( ulTagname == 628 || ulTagname == 629 || ulTagname == 630 )
		{}
		else if( ulTagname == 10142 )
		{
			ulCategoryId = ::atol( tagvalue );
		}
		else if( ulTagname == 10072 )
		{
			unsigned long		msgid = ::atol( tagvalue );
			if( ulCategoryId <= 32 )
			{
				if( msgid > 0 && msgid != -1 && msgid != m_category[ulCategoryId] +1 )
				{
					::printf( "StepClient::inner_bulidmsg() : Category(%d) isn't consecutive，missing packet(%d,%d)", ulCategoryId, m_category[ulCategoryId], msgid );
				}
				m_category[ulCategoryId] = msgid;
			}
			else
			{
				::printf( "StepClient::inner_bulidmsg() : Category(%d) isn't correct.", ulCategoryId );
			}
		}
		else if( ulTagname == 95 )
		{
			m_95len = ::atol( tagvalue );
		}
		else if( ulTagname == 96 )
		{
			ParserBit( m_rawdata, m_95len, lpHead->ulMsgType );
		}
		//正文消息
		else
		{
			if( msgsize - offset - 7 > m_parserlen ) {
				return -101;					//ERR_STEPMSG_MAXSIZE;
			}

			if( lpHead->ulMsgType == 101 || lpHead->ulMsgType == 1201 || lpHead->ulMsgType == 1202 || lpHead->ulMsgType == 2001 ) {
				unsigned int	nTextLen = msgsize - offset - 7;
				if( nTextLen <= sInSize )
				{
					::memcpy( lpOutBuf, m_parserbuf + offset, nTextLen );
				}
				else
				{
					::printf( "TextLength(%u) > CacheSize(%u) \n", nTextLen, sInSize );
				}
			}

			m_parserlen -= msgsize;
			::memmove( m_parserbuf, m_parserbuf+msgsize, m_parserlen );

			return(msgsize - offset - 7);
		}

		offset += errorcode;
	}

	return errorcode;
}

int	StepClient::inner_readsteptag( const char *lpInBuf, unsigned long sInSize, unsigned long sOffset, char* strTagName, char* strTagValue )
{
	register int			k = 0;
	register int			i = sOffset;
	bool					bComplete = false;
	static	char			tempbuf[MAX_STEPMSG_SIZE];

	if( sOffset >= sInSize ) {
		return 0;
	}

	///< 读取tagName的while循环
	while( i < sInSize )
	{
		if( k >= 254 )
		{
			return -200;	//ERR_STEPMSG_TAGMAXSIZE;	//tagName超过最大长度
		}
		else if ( lpInBuf[i] == '=' )
		{
			bComplete = true;
			break;
		}
		else if( lpInBuf[i] == 0x1 )
		{
			i ++;
			continue;
		}
		else
		{
			tempbuf[k] = lpInBuf[i];
			i ++;
			k ++;
		}
	}

	if( !bComplete )
	{
		return 0;
	}

	tempbuf[k] = 0;
	strcpy( strTagName, tempbuf );

	///< 读取tagValue的while循环
	++i;
	k = 0;
	if( ::atol( strTagName ) == 96 )
	{	
		if( m_95len > sInSize - i )
		{
			return 0;
		}

		m_rawdata = lpInBuf + i;
		return i-sOffset + m_95len;
	}

	bComplete = false;
	while( i < sInSize )
	{
		if( k >= MAX_STEPMSG_SIZE - 2 )
		{
			return -200;		//ERR_STEPMSG_TAGMAXSIZE;
		}
		else if ( (unsigned char)lpInBuf[i] == 0x01 )
		{
			bComplete = true;
			i++;
			break;
		}
		else
		{
			tempbuf[k] = lpInBuf[i];
			i++;
			k++;
		}
	}

	if( !bComplete )
	{
		return 0;
	}

	tempbuf[k] = 0;
	::strcpy( strTagValue, tempbuf );

	return (i - sOffset);
}

int StepClient::ParseData( const char * Inbuf, int nLen )
{
	int	nCalLen = sizeof(m_parserbuf)-m_parserlen;
	int nMin = (nLen<nCalLen) ? nLen:nCalLen;
	memcpy(m_parserbuf + m_parserlen, Inbuf, nMin);
	m_parserlen += nMin;

    tagStepComm_Head tagStepHead;
    int nErrCode = -1;
	while( (nErrCode = inner_bulidmsg( &tagStepHead, pStepTempBuf, MAXPARSERBUFLEN )) > 0 )
	{
		DispatchTransint( &tagStepHead, pStepTempBuf, nErrCode );
	}

	if (nErrCode < 0) {
		//目前错误只有ERR_STEPMSG_HEADNOTAG(头部某些标识字段错误), ERR_STEPMSG_MAXSIZE(格式的长度不对)
        switch (nErrCode) {
            case -100://ERR_STEPMSG_HEADNOTAG
                ::printf( "StepClient::ParseData() : 解析到错误的Step,头部格式不对" );
                break;
			case -101://ERR_STEPMSG_MAXSIZE:
                ::printf( "StepClient::ParseData() : 解析到错误的Step,数据长度不对,清空解析缓冲区" );
                break;
            default:
                ::printf( "StepClient::ParseData() : 解析到错误的Step,未知错误,清空解析缓冲区" );
                break;
        }
		m_parserlen = 0;
	}

	if (nMin < nLen) {
		nLen -= nMin;
		int nTemp = std::min<int>(nLen, sizeof(m_parserbuf) - m_parserlen);
		memcpy(m_parserbuf+m_parserlen, Inbuf+nMin, nTemp);
		m_parserlen += nTemp;
		if (nTemp < nLen) {
			::printf( "StepClient::ParseData() : 解析缓冲区满，丢弃%d字节", nLen-nTemp );
		}
	}

	return nErrCode;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

FullImaeDataClient::FullImaeDataClient()
{
	m_filename = "FullImage.";
}

FullImaeDataClient::~FullImaeDataClient()
{
}

void FullImaeDataClient::ParserBit( const void * pRawData, int nSize, unsigned long msgid )
{
	char							decodebuf[32] = {0};
	char							mapbit[8] = {0};
	int								nOffset = 0;
	int								nRetVal = 0;
	long					        lMsgId = 0;

	static Cb_DeCodeUA3115 decodeua3115;
    static Cb_DeCodeUA3202 decodeua3202;
	static Cb_DeCodeUA3113 decodeua3113;

	static Cb_DeCodeUA3201 decodeua3201;
	static Cb_DeCodeUA3107 decodeua3107;

    Cb_FastDecoder *decode = NULL;
    switch (msgid) {
	case 3115: // 市场总览
		decode = &decodeua3115;
		break;
	case 3202: // 个股快照
		decode = &decodeua3202;
		break;
	case 3201:	//逐笔
		decode = &decodeua3201;
		break;
	case 3113:  //指数快照
		decode = &decodeua3113;
		break;
	case 3107:	//集合竞价
		decode = &decodeua3107;
		break;
	case 1115: // 旧市场状态
        break;
	case 5102:
		break;
	default:
		break;
    }

    if (decode) {
		g_nEncodeBufferSize = 0;
		g_pEncoder->Attach2Buffer( g_pGlobalBuffer, GLOBAL_BUFFER_SIZE );

        FastDecoder fastdecoder;
        fastdecoder.Instance(decode);
        fastdecoder.DecodeFastStream((const char *)pRawData, nSize, msgid);
		fastdecoder.Release();

		g_pDecoder->Attach2Buffer( g_pGlobalBuffer, g_nEncodeBufferSize );
		g_nAccuRawDataSize += g_DataHolder.GetSize();
		g_nAccuEncodeDataSize += g_nEncodeBufferSize;
		while( g_nEncodeBufferSize > 0 )
		{
			unsigned short	nDeMsgID = 0;
			char			pszDecodeValue[sizeof tagDemoSnapType];
			int				nRetDecode = g_pDecoder->DecodeMessage( nDeMsgID, pszDecodeValue, sizeof(pszDecodeValue) );

			unsigned short	nRawMsgID = 0;
			unsigned int	nRawLen = sizeof tagDemoSnapType;
			char			pszRawValue[sizeof tagDemoSnapType] = { 0 };
			g_DataHolder.Read( nRawMsgID, pszRawValue, nRawLen );

			ASSERT_EQ( nDeMsgID, nRawMsgID );
			ASSERT_EQ( 0, ::memcmp( pszDecodeValue, pszRawValue, nRawLen ) );

			if( nRetDecode <= 0 )
			{
				break;
			}
		}
    }

	///< to do: 这里需要将数据推送出去
	g_DataHolder.Reset();
}

int FullImaeDataClient::DispatchTransint( const tagStepComm_Head * MsgHead, const char * InBuf, unsigned long InSize )
{
	if( MsgHead->ulMsgType == 1201 )
	{
		return DeCodeUA1201( MsgHead, InBuf, InSize );
	}

	return 1;
}

int FullImaeDataClient::DeCodeUA1201( const tagStepComm_Head * MsgHead, const char * InBuf, unsigned long InSize )
{
	int									nOffset = 0;
	int									nErrorcode;
	char								tagname[64] = { 0 };
	char								tagvalue[64] = { 0 };
	unsigned long						ulRebuildType = 0;
	unsigned long						ulTagName = 0;

	while( nOffset < InSize && (nErrorcode = inner_readsteptag( InBuf, InSize, nOffset, tagname, tagvalue )) > 0 )
	{
		nOffset += nErrorcode;
		ulTagName = ::atol(tagname);
		
		if( ulTagName == 10075 )
		{
			ulRebuildType = ::atol(tagvalue);
		}
		else if( ulTagName == 10076 )		//重发状态
		{
			if( tagvalue[0] == '1' )
			{
				::printf( "FullImaeDataClient::DeCodeUA1201() : Full image初始化,数据Rebuild完成\n" );
				m_nUA1201Status = 1;
			}
			else if( tagvalue[0] == '2' )
			{
				m_nUA1201Status = 2;
				::printf( "FullImaeDataClient::DeCodeUA1201() : Full image初始化,数据Rebuild不全\n" );
			}
			else if( tagvalue[0] == '3' )
			{
				m_nUA1201Status = 3;
				::printf( "FullImaeDataClient::DeCodeUA1201() : Full image初始化,数据Rebuild被拒绝\n" );
			}
			else if( tagvalue[0] == '4' )
			{
				m_nUA1201Status = 4;
				::printf( "FullImaeDataClient::DeCodeUA1201() : Full image初始化, 数据Rebuild失败(数据无效)\n" );
			}
		}
	}

	return 1;
}

void FullImaeDataClient::Release()
{
	StepClient::Release();
	m_parserlen = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PushDataClient::PushDataClient()
{
	m_filename = "fastdata.";
}

PushDataClient::~PushDataClient()
{
	Release();
}

bool PushDataClient::LoadFastData( const char* pszDataFile )
{
	time_t							bt,et;
	std::ifstream					filetr;
	int								nErr = -1;
	char							tempbuf[8192] = { 0 };

	Release();
	filetr.open( pszDataFile, std::ios::binary|std::ios::in );
	if( false == filetr.is_open() )
	{
		::printf( "PushDataClient::Instance() : 推送数据初始化,模拟文件打开失败\n" );
		return false;
	}

	filetr.seekg( 0, std::ios::beg );
	bt = ::time( NULL );
	while( true )
	{
		filetr.read( tempbuf, sizeof(tempbuf) );
		nErr = filetr.gcount();
		if( nErr <= 0 )
		{
			::printf( "PushDataClient::Instance() : [OK] 模拟数据解析完成\n" );
			break;
		}
		else
		{
			if( ( nErr = ParseData( tempbuf, nErr ) ) < 0 )
			{
				::printf( "PushDataClient::Instance() : [ERROR] 模拟数据解析发现错误的推送数据\n" );
				return false;
			}
		}
	}

	et = ::time( NULL );
	::printf( "总共解析耗时: %d \n", et - bt );
	::printf( "压缩前累计: %I64d, 压缩后累计: %I64d \n", g_nAccuRawDataSize, g_nAccuEncodeDataSize );

	return true;
}


