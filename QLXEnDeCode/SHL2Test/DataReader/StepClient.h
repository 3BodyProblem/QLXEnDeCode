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
	unsigned short					sMsgSize;					///< [9]��Ϣ�峤��
	unsigned long					ulMsgType;					///< [35]��Ϣ����
	unsigned long					ulSeqNo;					///< [34]��Ϣ���
	unsigned long					nSendDate;					///< [52]����ʱ��
	unsigned long					nSendTime;					///< [52]����ʱ��
	unsigned long					nEncodeType;				///< 0:��������;	1:UTF8����;
} tagStepComm_Head;
#pragma pack()


/**
 * @class		StepClient
 * @brief		��̬/ʵʱ���ݽ�������
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
	 * @brief			�Ӷ���ȡ���ݽ���(m_parserbuf)
	 * @param[in]		Inbuf			�´ﵽ���ݰ�
	 * @param[in]		nLen			���ݰ�����
	 * @return			< 0, ����
	 */
	int					ParseData( const char * Inbuf, int nLen );

	/**
	 * @brief			�����±���һ��/����step���Ľڵ���/ֵ
						�������tab96�������ݴ���m_rawdata
	 * @param[in]		lpInBuf			������ʼ��ַ
	 * @param[in]		sInSize			��Ч�����ݳ���
	 * @param[in]		sOffset			����ƫ�Ƽ�¼(ʼ�����λ������)
	 * @param[out]		strTagName		tag��
	 * @param[out]		strTagValue		tagֵ
	 * @return			���ؽ��������ݵ����ݳ���
	 */
	int					inner_readsteptag( const char *lpInBuf, unsigned long sInSize, unsigned long sOffset, char* strTagName, char* strTagValue );

	/**
	 * @brief			�ӻ������ȡһ�����ݰ�
	 * @param[out]		lpHead			���ݰ�����������
	 * @param[out]		lpOutBuf		���ݰ�����
	 * @param[in]		sInSize			lpOutBuf����
	 */
	int					inner_bulidmsg( tagStepComm_Head * lpHead, char * lpOutBuf, int sInSize );

	/**
	 * @brief			��������ʵ�ֽ����������ĵ���������
	 */
	virtual int			DispatchTransint( const tagStepComm_Head * MsgHead, const char * InBuf, unsigned long InSize ) = 0;

	/**
	 * @brief			����tag96�µĿ�������
	 * @param[in]		pRawData		���ݵ�ַ
	 * @param[in]		nSize			���ݳ���
	 * @param[in]		id				��Ϣ����(tag10142)
	 */
	virtual void		ParserBit( const void * pRawData, int nSize, unsigned long id ) { return; }

protected:
	int					 m_nUA1201Status;				///< ��̬���ݳ�ʼ��״̬ ( 0:δ��, 1:�վ���ȷ������:�����쳣 )
	unsigned long		 m_category[33];				///< ��tab10142ֵ(��Ʒ����)Ϊ�±�ģ��������м�¼
	unsigned long		 m_95len;						///< tag96(��̬���������������ļ�����)���������ļ�����
	const char *		 m_rawdata;						///< tag96������
	char				 m_parserbuf[MAXPARSERBUFLEN];	///< ���ݽ��ջ���
	unsigned long		 m_parserlen;					///< �������Ч���ݳ���
	std::string			 m_filename;					///< �����������Ͷ�Ӧ���ļ�������
};


/**
 * @class		FullImaeDataClient
 * @brief		�������ݳ�ʼ��
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
 * @brief		�������ݻ�ȡ
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



