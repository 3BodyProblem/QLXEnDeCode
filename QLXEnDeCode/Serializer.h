#ifndef __QLX_SERIALIZER_H__
#define __QLX_SERIALIZER_H__


#pragma warning(disable : 4786)
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)
#include <istream>
#include "EnDePolicy.h"
#include "OPCollection.h"


#define		MAX_DATABLOCK_SIZE			2048				///< ��������ݽṹ����


/**
 * @class		FrameSerializer
 * @brief		֡���л�
 */
class FrameSerializer
{
public:
	FrameSerializer();

	/**
	 * @brief		��һ��protobuf���л����Զ���
	 * @param[in]	pOperation	���л�����
	 * @return		=0			�ɹ�
	 */
	int	BindOperation( IPbOperation* pOperation );

	/**
	 * @brief		�����Ѿ����л��ĳ���
	 */
	int	GetDataSize();

	/**
	 * @brief			ȡ�û����ַ
	 */
	operator char*();

	/**
	 * @brief			attachһ�����д��Ļ���
	 */
	void Attach( unsigned char *ptr, size_t size );

	/**
	 * @brief			��ֵMessageID,�����buffer�ĳ��ȼ���
	 */
	bool NewMessage( unsigned short nMsgID );

	/**
	 * @brief			���л�һ��bytes����
	 * @return			д�����л�bit��
	 */
	int	operator << ( const SerialBytes& val );

	/**
	 * @breif			���л�һ���ַ���
	 * @return			д�����л�bit��
	 */
	int	operator << ( const SerialString& val );

	/**
	 * @brief			���л�һ�����ⳤ�ȵ���ֵ
	 * @return			д�����л�bit��
	 */
	int	operator << ( const SerialNumber& val );

	/**
	 * @brief			���������л���ˢ�뻺��
	 * @return			<0		ʧ��
	 */
	int Serialize();

private:
	OperationCollection		m_Operations;						///< ������Լ�
	char					m_pszDataBuf[MAX_DATABLOCK_SIZE];	///< ��һ��encode����ʹ�õ�message�Ĳ�ֵ����
	unsigned int			m_nDataBufSize;						///< m_pszDataBuf����Ч���ݳ���
	unsigned char*			m_buf;								///< ����
	size_t					m_buflen;							///< �����ܳ���
	size_t					m_curpos;							///< ��д��λ��
	unsigned short			m_nMsgID;
};


/**
 * @class		FrameUnserializer
 * @brief		֡�����л�
 */
class FrameUnserializer
{
public:
	FrameUnserializer();

	/**
	 * @brief		����δ���л����ֵĳ���
	 */
	int	GetDataSize();

	/**
	 * @brief		��һ��protobuf���л����Զ���
	 * @param[in]	pOperation	���л�����
	 * @return		=0			�ɹ�
	 */
	int	BindOperation( IPbOperation* pOperation );

	/**
	 * @brief			attachһ����Զ�ȡ�Ļ���
	 */
	void Attach( unsigned char *ptr, size_t size );

	/**
	 * @brief			����һ��bytes��
	 * @return			�������л���bit��
	 */
	int operator >> ( SerialBytes& val );

	/**
	 * @brief			����һ���ַ���
	 * @return			�������л���bit��
	 */
	int operator >> ( SerialString& val );

	/**
	 * @brief			����һ�����ⳤ�ȵ���ֵ
	 * @return			�������л���bit��
	 */
	int operator >> ( SerialNumber& val );

	/**
	 * @brief			�������е����ݷ����л�
	 * @return			<0		ʧ��
	 */
	int Unserialize();

private:
	OperationCollection		m_Operations;						///< ������Լ�
	char					m_pszDataBuf[MAX_DATABLOCK_SIZE];	///< ��һ��encode����ʹ�õ�message�Ĳ�ֵ����
	unsigned int			m_nDataBufSize;						///< m_pszDataBuf����Ч���ݳ���
	unsigned char*			m_buf;								///< ����
	size_t					m_buflen;							///< �����ܳ���
	size_t					m_curpos;							///< ������λ��
};




#endif






