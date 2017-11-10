#ifndef __STEPCLIENT_H__
#define __STEPCLIENT_H__

#ifdef _DEBUG
#pragma warning(disable : 4786)
#pragma warning(disable : 4018)
#pragma warning(disable : 4996)
#include <string>
#include "../DataCenter/MemStructure.h"


#define	  MAXPARSERBUFLEN	(1024*1024*3)


#pragma pack(1)
typedef struct
{
	unsigned short					sMsgSize;					///< [9]消息体长度
	unsigned long					ulMsgType;					///< [35]消息类型
	unsigned long					ulSeqNo;					///< [34]消息序号
	unsigned long					nSendDate;					///< [52]发送时间
	unsigned long					nSendTime;					///< [52]发送时间
	unsigned long					nEncodeType;				///< 0:其他编码;	1:UTF8编码;
} tagStepComm_Head;
#pragma pack()


/**
 * @class		StepClient
 * @brief		静态/实时数据解析基类
 */
class StepClient
{
public:
	StepClient();
	virtual ~StepClient();

	virtual int			Instance();
	void				Release();

protected:
	/**
	 * @brief			从队列取数据解析(m_parserbuf)
	 * @param[in]		Inbuf			新达到数据包
	 * @param[in]		nLen			数据包长度
	 * @return			< 0, 出错
	 */
	int					ParseData( const char * Inbuf, int nLen );

	/**
	 * @brief			再向下遍历一个/解析step包的节点名/值
						，如果有tab96，则将内容存入m_rawdata
	 * @param[in]		lpInBuf			数据起始地址
	 * @param[in]		sInSize			有效的数据长度
	 * @param[in]		sOffset			数据偏移记录(始起解析位置索引)
	 * @param[out]		strTagName		tag名
	 * @param[out]		strTagValue		tag值
	 * @return			返回解析过数据的数据长度
	 */
	int					inner_readsteptag( const char *lpInBuf, unsigned long sInSize, unsigned long sOffset, char* strTagName, char* strTagValue );

	/**
	 * @brief			从缓存队列取一个数据包
	 * @param[out]		lpHead			数据包基本数据项
	 * @param[out]		lpOutBuf		数据包内容
	 * @param[in]		sInSize			lpOutBuf长度
	 */
	int					inner_bulidmsg( tagStepComm_Head * lpHead, char * lpOutBuf, int sInSize );

	/**
	 * @brief			各派生类实现解析其所关心的数据类型
	 */
	virtual int			DispatchTransint( const tagStepComm_Head * MsgHead, const char * InBuf, unsigned long InSize ) = 0;

	/**
	 * @brief			解析tag96下的快照数据
	 * @param[in]		pRawData		数据地址
	 * @param[in]		nSize			数据长度
	 * @param[in]		id				消息类型(tag10142)
	 */
	virtual void		ParserBit( const void * pRawData, int nSize, unsigned long id ) { return; }

protected:
	int					 m_nUA1201Status;				///< 静态数据初始化状态 ( 0:未有, 1:收据正确，其他:数据异常 )
	unsigned long		 m_category[33];				///< 以tab10142值(商品类型)为下标的，发送序列记录
	unsigned long		 m_95len;						///< tag96(静态数据所带的数据文件内容)所包含的文件长度
	const char *		 m_rawdata;						///< tag96的内容
	char				 m_parserbuf[MAXPARSERBUFLEN];	///< 数据接收缓存
	unsigned long		 m_parserlen;					///< 缓存的有效数据长度
	std::string			 m_filename;					///< 各类数据类型对应的文件保存名
};


/**
 * @class		FullImaeDataClient
 * @brief		快照数据初始化
 */
class FullImaeDataClient : public StepClient
{
public:
	FullImaeDataClient();
	~FullImaeDataClient();

	void				Release();
public:
	int					DispatchTransint( const tagStepComm_Head * MsgHead, const char * InBuf, unsigned long InSize );
	virtual int			DeCodeUA1201( const tagStepComm_Head * MsgHead, const char * InBuf, unsigned long InSize );
protected:
	void				ParserBit( const void * pRawData, int nSize, unsigned long id );
};


/**
 * @class		PushDataClient
 * @brief		推送数据获取
 */
class PushDataClient : public FullImaeDataClient
{
public:
	PushDataClient();
	~PushDataClient();

	bool		LoadFastData( const char* pszDataFile );
};

#endif
#endif



