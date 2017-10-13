/*
		�ļ���Process.cpp
		���ܣ�����
		ʱ�䣺2008.5.14
		���ߣ�luozn
		��Ȩ: �Ϻ�Ǭ¡�߿Ƽ���Ȩ����

		�޸ļ�¼
		�޸�����		�޸���Ա	�޸����ݺ�ԭ��	
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
		if( ( nRetVal = m_Thread.StartThread( "�����۶���Ϣ�߳�", pushthreadfunc, this ) ) < 0 )
		{
			WriteError( 0, "�Ϻ�L2fastԴ����", "��ʼ��ʧ��,����ʧ�ܣ���ʼ��Processǰ׺�����̳߳���[Err:%d]", nRetVal );
			return -1;
		}
	}
	else
	{
		WriteError( 0, "�Ϻ�L2fastԴ����", "�������۶϶�ʱ�����̣߳�" );
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
	tagStatusInfo->version = Global_Version;           //�汾��
	tagStatusInfo->marketid = Global_MarketId;          //�г����
	tagStatusInfo->marketstatus = GetSvcStatus();      //�г�״̬  1����ʼ�� 0��δ��ʼ��
	tagStatusInfo->freshtime = 0;						//ˢ��ʱ��(����)����̬ˢ��

	Global_DataIO.GetShl2FastMarket().GetMarketHead( &MarketHead );
	
	tagStatusInfo->marketdate = MarketHead.marketdate;        //�г�����
	tagStatusInfo->markettime = MarketHead.markettime;        //�г�ʱ��
	tagStatusInfo->packprospeed = 0;						  //ÿ���Ӵ���İ��ĸ���

	GetSenseWare( &Index );

	tagStatusInfo->price = Index.now;            //��һ����Ʒ�ļ۸�
	tagStatusInfo->amount = Index.amount;        //��һ����Ʒ�ĳɽ����
	tagStatusInfo->volumn = Index .volume;       //��һ����Ʒ�ĳɽ���

	tagStatusInfo->connecttime = 0;				 //�������ӻػ���ʱ��(��)��������Ͷ˿�
	tagStatusInfo->lastIniliazeTime = 0;		 //�ϴγ�ʼ��ʱ��

	if( MarketHead.marketstatus > 0 )
	{
		tagStatusInfo->prepared = true;         //�Ƿ�׼����ϣ���LEVEL�����������壬ֱ�ӷ��������Ƿ�ɹ�
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

	pSelf->WriteInfo(0,"ǰ׺���г�״̬�����߳�","����");
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
				if (worktime.GetDuration() > (10 *60 *1000) )	//ǰ10���Ӳ�����
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
			pSelf->WriteInfo(0,"ǰ׺�仯����","ǰ׺�仯���Ϳ�ʼ��һ��");
			
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
						//ǰ���Σ��������ʼ��ֵ��û�䣬Ҳû��Ҫ��������
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
			sErr.Format( "ǰ׺���г�״̬�����̷߳����쳣[%s]", e.what() );
			Global_Process.WriteInfo( 0, "ProcessIO����", sErr.c_str() );
		}
		catch(...)
		{
			Global_Process.WriteInfo( 0, "ProcessIO����", "ǰ׺���г�״̬�����̷߳���δ֪�쳣" );
		}

	}

	pSelf->WriteInfo(0,"ǰ׺���г�״̬�����߳�","�˳�");
	return 0;
}