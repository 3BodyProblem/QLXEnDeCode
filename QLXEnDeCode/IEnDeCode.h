#ifndef __QLX_INTERFACE_ENDECODE_H__
#define __QLX_INTERFACE_ENDECODE_H__


/**
 * @class	IPbOperation
 * @brief	protobuf�������
			��ÿһ�����͵�protobuf��message���ͣ�����һ����Ӧ�ı��������
			֮���ٰ󶨵�en/decodeʵ����
 */
class IPbOperation
{
public:
	/**
	 * @brief		ȡ��ĳprotobuf message id
	 * @return		Э���
	 */
	virtual	int		MessageID() const = 0;

	/**
	 * @brief		���л�protobuf���󵽻���
	 * @param[in]	pszBuf		�����ַ
	 * @param[in]	nBufLen		���泤��
	 * @param[out]	pszOutput	�����������
	 * @param[in]	nOutputLen	�������ĳ���
	 * @return		�������л�д��Ļ��泤��
	 */
	virtual int		Serialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen ) = 0;

	/**
	 * @brief		�ӻ��淴���л�protobuf����
	 * @param[in]	pszBuf		�����ַ
	 * @param[in]	nBufLen		���泤��
	 * @param[out]	pszOutput	�����������,	* ע��������������������дβ����Ĳ������ݣ���Ҫ��sizeof(MessageType)�Ŀռ䷶Χ����ȫ�ֶ�д��
	 * @param[in]	nOutputLen	�������ĳ���
	 * @return		���ض����Ļ��泤��
	 * @note		ע�⣬�������pszOutput, ���������дβ����Ĳ������ݣ���Ҫ��sizeof(MessageType)�Ŀռ䷶Χ����ȫ�ֶ�д��
	 */
	virtual int		Unserialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen ) = 0;
};


/**
 * @class	InterfaceEncode
 * @brief	Encode�ӿ�
 */
class InterfaceEncode
{
public:
	/**
	 * @brief		��ʼ��
	 * @param[in]	pszTemplateFile	��������ļ�
	 * @param[in]	pListOperation	protobuf��������б��ַ
	 * @param[in]	nListSize		protobuf��������б���
	 * @return		0			�ɹ�
	 */
	virtual int Initialize( const char* pszTemplateFile, IPbOperation** pListOperation, int nListSize ) = 0;

	/**
	 * @brief		�ͷŽӿ�
	 */
	virtual void Release() = 0;

	/**
	 * @brief		׼����дһ���µ�frame�ṹ
	 * @param[in]	pBuffer		�����ַ
	 * @param[in]	nBufferLne	���泤��
	 * @return		= 0			�ɹ�
	 */
	virtual int Attach2Buffer( char* pBuffer, unsigned int nBufferLen ) = 0;

	/**
	 * @brief		��frmae�����msg�ṹ
	 * @param[in]	nMsgID		Message ID
	 * @param[in]	pData		��Ϣ���ݵ�ַ
	 * @param[in]	nLen		��Ϣ����
	 * @return		> 0			�ɹ����������ε����ۻ������л��ĳ���
	 */
	virtual int EncodeMessage( unsigned short nMsgID, const char *pData, unsigned int nLen ) = 0;
};


/**
 * @class	InterfaceDecode
 * @brief	Decode�ӿ�
 */
class InterfaceDecode
{
public:
	/**
	 * @brief		��ʼ��
	 * @param[in]	pszTemplateFile	��������ļ�
	 * @param[in]	pListOperation	protobuf��������б��ַ
	 * @param[in]	nListSize		protobuf��������б���
	 * @return		0			�ɹ�
	 */
	virtual int Initialize( const char* pszTemplateFile, IPbOperation** pListOperation, int nListSize ) = 0;

	/**
	 * @brief		�ͷŽӿ�
	 */
	virtual void Release() = 0;

	/**
	 * @brief		׼����дһ���µ�frame�ṹ
	 * @param[in]	pBuffer		�����ַ
	 * @param[in]	nBufferLne	������Ч���ݵĳ���
	 * @return		= 0			�ɹ�
	 */
	virtual int Attach2Buffer( char* pBuffer, unsigned int nBufferLen ) = 0;

	/**
	 * @brief		��frmae�����msg�ṹ
	 * @param[out]	nMsgID		����Message ID
	 * @param[in]	pData		��Ϣ���ݵ�ַ
	 * @param[in]	nLen		��Ϣ����
	 * @return		>= 0		�ɹ����������ε��ú�,δ���л����ֵĳ���
	 */
	virtual int DecodeMessage( unsigned short& nMsgID, char *pData, unsigned int nLen ) = 0;
};


#endif





