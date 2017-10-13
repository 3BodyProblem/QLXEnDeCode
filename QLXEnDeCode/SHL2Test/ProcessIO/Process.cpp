/*
		文件：Process.cpp
		功能：控制
		时间：2008.5.14
		作者：luozn
		版权: 上海乾隆高科技版权所有

		修改纪录
		修改日期		修改人员	修改内容和原因	
*/
#include "./Process.h"
#include "math.h"
#include "../GlobalIO/MGlobalIO.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MProcessIO::MProcessIO()
{
	m_srvstatus.SetValue( (unsigned char)0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MProcessIO::~MProcessIO()
{}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int MProcessIO::Instance()
{
	if (Global_Option.IsBroadCastPreName())
	{
		int						nRetVal = 0;
		if( ( nRetVal = m_Thread.StartThread( "推送熔断信息线程", pushthreadfunc, this ) ) < 0 )
		{
			WriteError( 0, "上海L2fast源驱动", "初始化失败,启动失败，初始化Process前缀推送线程出错[Err:%d]", nRetVal );
			return -1;
		}
	}
	else
	{
		WriteError( 0, "上海L2fast源驱动", "不启动熔断定时推送线程！" );
	}

	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MProcessIO::Release()
{
	if (Global_Option.IsBroadCastPreName())
	{
		m_Thread.StopThread();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MProcessIO::SetDataCenterFunc( tagDataCenterInput * tagInput )
{
	if( tagInput )
	{
		memcpy( &m_DataCenterFuns, tagInput, sizeof( tagDataCenterInput ) );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MProcessIO::WriteLog( int type, unsigned short logLevel, const char * source, const char * tempbuflog )
{
	char							tempbuf[8000];

	_snprintf( tempbuf, sizeof(tempbuf)-1, "<%s>%s", source, tempbuflog );
	m_DataCenterFuns.onwritelog( type, Global_DrvUnitCode, logLevel, tempbuflog );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MProcessIO::WriteView( unsigned short logLevel, const char * Source, const char * Format, ... )
{
	char				tempbuf[4096];

	va_list				marker;
	
	va_start( marker,Format );
	_vsnprintf( tempbuf, 4096, Format, marker );
	va_end( marker );

	WriteLog( 0, logLevel, Source, tempbuf );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MProcessIO::WriteInfo( unsigned short logLevel, const char * Source, const char * Format, ... )
{
	char				tempbuf[4096];

	va_list				marker;
	
	va_start( marker,Format );
	_vsnprintf( tempbuf, 4096, Format, marker );
	va_end( marker );

	WriteLog( 1, logLevel, Source, tempbuf );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MProcessIO::WriteWarning( unsigned short logLevel, const char * Source, const char * Format, ... )
{
	char				tempbuf[4096];

	va_list				marker;
	
	va_start( marker,Format );
	_vsnprintf( tempbuf, 4096, Format, marker );
	va_end( marker );

	WriteLog( 2, logLevel, Source, tempbuf );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MProcessIO::WriteError( unsigned short logLevel, const char * Source, const char * Format, ... )
{
	char				tempbuf[4096];

	va_list				marker;
	
	va_start( marker,Format );
	_vsnprintf( tempbuf, 4096, Format, marker );
	va_end( marker );

	WriteLog( 3, logLevel, Source, tempbuf );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tagDataCenterInput * MProcessIO::GetDataCenterFunc()
{
	return &m_DataCenterFuns;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool MProcessIO::GetSvcStatus()
{
	return m_srvstatus.GetValue() != 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MProcessIO::SetSvcStatus( unsigned char val )
{
    if (val == STATUS_TRANSPT) {
        if (Global_Option.IsConnectTime()) {
            STATUS_TRANSPT_set_in_connect_time = true;
        } else {
            STATUS_TRANSPT_set_in_connect_time = false;
        }
    }
	m_srvstatus.SetValue( val );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MProcessIO::SetWorkStatus( EWorkStatusType type )
{
	m_workstatus.SetValue( &type );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char MProcessIO::GetWordStatus() 
{
	return m_workstatus.GetValue();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int	MProcessIO::GetDataFrameCount( unsigned char MainType,unsigned char ChildType,char * InBuf,unsigned short InBufSize )
{
	EWorkStatusType							workstatus;

	if( m_srvstatus .GetValue()<= 0 )
	{
		return -1;
	}

	m_workstatus.GetValue( &workstatus );
	
	if( workstatus != STATUS_TRANSPT )
	{
		return -2;
	}

	return Global_DataIO.GetDataFrameCount( ChildType, InBuf, InBufSize );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int	MProcessIO::GetOneFrameData( unsigned char MainType,unsigned char ChildType,char * InBuf,unsigned short InBufSize,unsigned short PageNo,char * OutBuf,unsigned short InSize )
{
	EWorkStatusType							workstatus;

	if( m_srvstatus .GetValue()<= 0 )
	{
		return -1;
	}

	m_workstatus.GetValue( &workstatus );
	
	if( workstatus != STATUS_TRANSPT )
	{
		return -2;
	}

	return Global_DataIO.GetOneFrameData(ChildType,InBuf,InBufSize,PageNo,OutBuf,InSize);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int MProcessIO::GetMarketInfo( tagSHL2Mem_MarketHead * pHead, tagSHL2Mem_MarketDetail * pInfo, unsigned short nCount )
{

	EWorkStatusType							workstatus;
	int										nRet;

	if( m_srvstatus .GetValue()<= 0 )
	{
		return -1;
	}

	m_workstatus.GetValue( &workstatus );
	
	if( workstatus != STATUS_TRANSPT )
	{
		return -2;
	}

	nRet = Global_DataIO.GetMarketInfo( pHead, pInfo, nCount );
	for( int i = 0; i < nRet; ++i )
	{
		pInfo[i].pricerate = pow( 10, pInfo[i].pricerate );
	}
	return nRet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MProcessIO::GetSenseWare( tagSHL2Mem_IndexTable * pOut )
{
	if( pOut == NULL )
	{
		return;
	}
	memset( pOut, 0, sizeof(*pOut) );
	if( m_srvstatus .GetValue()<= 0 )
	{
		return;
	}

	Global_DataIO.GetSenseWare( pOut );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MProcessIO::GetStatusFresh( tagDrvStatusInfo * tagStatusInfo )
{
	tagSHL2Mem_IndexTable			Index = {0};
	tagSHL2Mem_MarketHead			MarketHead = {0};

	if( tagStatusInfo == NULL )
	{
		return;
	}
	tagStatusInfo->version = Global_Version;           //版本号
	tagStatusInfo->marketid = Global_MarketId;          //市场编号
	tagStatusInfo->marketstatus = GetSvcStatus();      //市场状态  1，初始化 0，未初始化
	tagStatusInfo->freshtime = 0;						//刷新时间(毫秒)，动态刷新

	Global_DataIO.GetShl2FastMarket().GetMarketHead( &MarketHead );
	
	tagStatusInfo->marketdate = MarketHead.marketdate;        //市场日期
	tagStatusInfo->markettime = MarketHead.markettime;        //市场时间
	tagStatusInfo->packprospeed = 0;						  //每秒钟处理的包的个数

	GetSenseWare( &Index );

	tagStatusInfo->price = Index.now;            //第一个商品的价格
	tagStatusInfo->amount = Index.amount;        //第一个商品的成交金额
	tagStatusInfo->volumn = Index .volume;       //第一个商品的成交量

	tagStatusInfo->connecttime = 0;				 //本次连接回话的时间(秒)，针对推送端口
	tagStatusInfo->lastIniliazeTime = 0;		 //上次初始化时间

	if( MarketHead.marketstatus > 0 )
	{
		tagStatusInfo->prepared = true;         //是否准备完毕，在LEVEL驱动中无意义，直接返回连接是否成功
	}
	else 
	{
		tagStatusInfo->prepared = false;
	}
}

bool MProcessIO::Is_STATUS_TRANSPT_valid() {
    return STATUS_TRANSPT_set_in_connect_time;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void * __stdcall MProcessIO::pushthreadfunc( void * pParam )
{
	MProcessIO *pSelf = reinterpret_cast<MProcessIO *>( pParam );

	pSelf->WriteInfo(0,"前缀及市场状态推送线程","启动");
	unsigned long nPushInterval = Global_Option.GetPushInterval();
	unsigned long nUpdateInterval = Global_Option.GetUpdateInterval();

	MCounter  worktime;
	worktime.SetCurTickCount();

	MDateTime				datetime;
	datetime.SetCurDateTime();
	unsigned long ulOldDate = datetime.DateToLong();
	unsigned long ulNowDate, ulNowTime;

	MCounter  refresh;
	refresh.SetCurTickCount();

	bool bFirst = true;
	int nCount = 0;
	while( !pSelf->m_Thread.GetThreadStopFlag() )
	{
		try
		{
			MThread::Sleep( 1000 );
			
			datetime.SetCurDateTime();
			ulNowTime= datetime.TimeToLong();
			ulNowDate= datetime.DateToLong();

			unsigned long			starttime,endtime;
			unsigned short			st,et;
			Global_Option.GetWorkTime(&st,&et);
			starttime = st*100, endtime = et*100;
			if( ulOldDate != ulNowDate && ulNowTime > starttime )
			{
				bFirst = true;
				nCount =0;
				worktime.SetCurTickCount();
				refresh.SetCurTickCount();
				ulOldDate = ulNowDate; 
			}
			
			//1,
			if (!Global_Option.IsConnectTime())
			{
				continue;
			}
			
			//2,
			if (pSelf->GetWordStatus() != STATUS_TRANSPT || !(pSelf->GetSvcStatus()))
			{
				continue;
			}
			
			//3,
			if (bFirst)
			{
				if (worktime.GetDuration() > (10 *60 *1000) )	//前10分钟不推送
				{
					bFirst =false;
				}
				else
				{
					continue;
				}
			}
			
			//
			if (refresh.GetDuration() <nPushInterval)
			{
				continue;
			}
			refresh.SetCurTickCount();
			pSelf->WriteInfo(0,"前缀变化推送","前缀变化推送开始新一轮");
			
			unsigned short nSerial=0;
			bool bOnlyUp = nCount>=3;
			while(true)
			{
				tagShL2FastMem_TdPhase refObj;
				memset(&refObj, 0, sizeof(tagShL2FastMem_TdPhase));
				if (Global_DataIO.GetShl2FastTdPhase().GetNextOne(&refObj, nSerial) > 0 )
				{
					if (bOnlyUp && refObj.phaseCode[0] == 0 && refObj.phaseCode[1] == 0 && refObj.phaseCode[2] == 0 && 
						refObj.phaseCode[3] == 0 && refObj.phaseCode[4] == 0 && refObj.phaseCode[5] == 0)
					{
						//前三次，如果连初始化值都没变，也没必要往下推送
						nSerial++;
						continue;
					}
					Global_SrvPush.PushData( &refObj );
				}
				else
				{
					break;
				}
				nSerial++;
				MThread::Sleep( nUpdateInterval );
			}
			
			nCount++;
		}
		catch( std::exception &e )
		{
			MString sErr;
			sErr.Format( "前缀及市场状态推送线程发生异常[%s]", e.what() );
			Global_Process.WriteInfo( 0, "ProcessIO推送", sErr.c_str() );
		}
		catch(...)
		{
			Global_Process.WriteInfo( 0, "ProcessIO推送", "前缀及市场状态推送线程发生未知异常" );
		}

	}

	pSelf->WriteInfo(0,"前缀及市场状态推送线程","退出");
	return 0;
}