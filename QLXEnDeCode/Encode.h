#ifndef __QLX_ENCODE_H__
#define __QLX_ENCODE_H__


#pragma warning(disable : 4786)
#pragma warning(disable : 4244)
#include "IEnDeCode.h"
#include "Serializer.h"


/**
 * @class	Encode
 * @brief	����ѹ�����Խ��в�ֵ��ѹ��
 */
class Encode : public InterfaceEncode
{
public:
	Encode();

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
	 * @param[in]	nBufferLne	���泤��
	 * @return		= 0			�ɹ�
	 */
	int Attach2Buffer( char* pBuffer, unsigned int nBufferLen );

	/**
	 * @brief		��frmae�����msg�ṹ
	 * @param[in]	pData		��Ϣ���ݵ�ַ
	 * @param[in]	nLen		��Ϣ����
	 * @return		> 0			�ɹ����������ε����ۻ������л��ĳ���
	 */
	int EncodeMessage( unsigned short nMsgID, const char *pData, unsigned int nLen );

	/**
	 * @brief		���ֶν��б���
	 * @return		�����ֶδ�����ƫ��ֵ(�ṹ��)
					>=0		����ƫ��ֵ
					< 0		�������
	 */
	inline int OnEncodeField( MsgField& rField, const char *pData, unsigned int nLen );

	/**
	 * @brief		�������ֶν��б���
	 * @return		�����ֶδ�����ƫ��ֵ(�ṹ��)
					>=0		����ƫ��ֵ
					< 0		�������
	 */
	inline int OnEncodeSequenceField( MsgField& rField, const char *pData, unsigned int nLen );

private:
	MsgPolicy				m_oMsgPolicy;			///< ����
	FrameSerializer			m_oSerializer;			///< ���л�
	char					*m_pszBuffer;			///< ����ռ�
};



#endif






