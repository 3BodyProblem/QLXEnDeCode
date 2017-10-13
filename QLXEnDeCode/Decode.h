#ifndef __QLX_DECODE_H__
#define __QLX_DECODE_H__


#pragma warning(disable : 4786)
#pragma warning(disable : 4244)
#include "IEnDeCode.h"
#include "Serializer.h"


/**
 * @class	Decode
 * @brief	����ѹ�����Խ��в�ֵ����ѹ��
 */
class Decode : public InterfaceDecode
{
public:
	Decode();

	/**
	 * @brief		��ʼ��
	 * @param[in]	pszTemplateFile	��������ļ�
	 * @param[in]	pListOperation	protobuf��������б��ַ
	 * @param[in]	nListSize		protobuf��������б���
	 * @return		0			�ɹ�
	 */
	int Initialize( const char* pszTemplateFile, IPbOperation** pListOperation, int nListSize );

	/**
	 * @brief		�ͷŶ���
	 */
	void Release();

	/**
	 * @brief		׼����дһ���µ�frame�ṹ
	 * @param[in]	pBuffer		�����ַ
	 * @param[in]	nBufferLne	������Ч���ݵĳ���
	 * @return		= 0			�ɹ�
	 */
	int Attach2Buffer( char* pBuffer, unsigned int nBufferLen );

	/**
	 * @brief		��frmae�����msg�ṹ
	 * @param[in]	pData		��Ϣ���ݵ�ַ
	 * @param[in]	nLen		��Ϣ����
	 * @return		>= 0		�ɹ����������ε��ú�,δ���л����ֵĳ���
	 */
	int DecodeMessage( unsigned short& nMsgID, char *pData, unsigned int nLen );

	/**
	 * @brief		���ֶν��б���
	 * @return		�����ֶδ�����ƫ��ֵ(�ṹ��)
					>=0		����ƫ��ֵ
					< 0		�������
	 */
	inline int OnDecodeField( MsgField& rField, char *pData, unsigned int nLen );

	/**
	 * @brief		�������ֶν��б���
	 * @return		�����ֶδ�����ƫ��ֵ(�ṹ��)
					>=0		����ƫ��ֵ
					< 0		�������
	 */
	inline int OnDecodeSequenceField( MsgField& rField, char *pData, unsigned int nLen );

private:
	MsgPolicy				m_oMsgPolicy;			///< ����
	FrameUnserializer		m_oUnserializer;		///< ���л�
	char					*m_pszBuffer;			///< ����ռ�
};


#endif









