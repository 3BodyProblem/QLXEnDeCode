/*
		文件：Process.h
		功能：控制
		时间：2008.5.14
		作者：luozn
		版权: 上海乾隆高科技版权所有

		修改纪录
		修改日期		修改人员	修改内容和原因	
*/
#ifndef __TRANSRCSHLFAST_PROCESS_H__
#define __TRANSRCSHLFAST_PROCESS_H__

#include "../../../MEngine/EngineLib/UnitCode/MEngine.hpp"
#include "../../../DataCenter/SrvUnit/Interface/MInterface.hpp"
#include "../DataCenter/MemStructure.h"
#include "../DataCenter/SectionType.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	STATUS_INITIAL = 0,
	STATUS_TRANSPT,
	STATUS_CLOSING,
	STATUS_CLOSED,
	STATUS_ERR
} EWorkStatusType;

class MProcessIO
{
protected:
	tagDataCenterInput		 m_DataCenterFuns;
	SType< unsigned char >   m_srvstatus;
	SType< EWorkStatusType > m_workstatus;
	MThread						m_Thread;
public:
	MProcessIO();
	virtual ~MProcessIO();
public:
	int		 Instance();
	void	 Release();
public:
	void	SetDataCenterFunc( tagDataCenterInput * );
	tagDataCenterInput * GetDataCenterFunc();
	void	WriteLog( int type, unsigned short logLevel, const char * source, const char * tempbuf );
	void	WriteView( unsigned short logLevel, const char * Source, const char * Format, ... );
	void	WriteInfo( unsigned short logLevel, const char * Source, const char * Format, ... );
	void	WriteWarning( unsigned short logLevel, const char * Source, const char * Format, ... );
	void	WriteError( unsigned short logLevel, const char * Source, const char * Format, ... );
public:
	bool    GetSvcStatus();
	void    SetSvcStatus( unsigned char );
	void	SetWorkStatus( EWorkStatusType );
	unsigned char GetWordStatus();
	int		GetDataFrameCount( unsigned char MainType,unsigned char ChildType,char * InBuf,unsigned short InBufSize );
	int		GetOneFrameData( unsigned char MainType,unsigned char ChildType,char * InBuf,unsigned short InBufSize,unsigned short PageNo,char * OutBuf,unsigned short InSize );
	int		GetMarketInfo( tagSHL2Mem_MarketHead * pHead, tagSHL2Mem_MarketDetail * pInfo, unsigned short nCount );
	void	GetSenseWare( tagSHL2Mem_IndexTable * );
	void    GetStatusFresh( tagDrvStatusInfo * );
    bool Is_STATUS_TRANSPT_valid();

private:
    bool STATUS_TRANSPT_set_in_connect_time;
protected:
	static void * __stdcall		pushthreadfunc( void *pParam );
};










#endif//__TRANSRCSHLFAST_PROCESS_H__