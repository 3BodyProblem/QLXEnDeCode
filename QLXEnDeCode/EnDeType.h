#ifndef __QLX_ENDE_TYPE_H__
#define __QLX_ENDE_TYPE_H__


#pragma warning(disable : 4786)
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)
#include <string>
#include "stdio.h"


/**
 * @class		FdDefinition
 * @brief		�ֶ�ԭʼ�ṹ����
 */
class FdDefinition
{
public:
	enum FIELD_TYPE
	{
		RAWT_INT8,		RAWT_INT16,		RAWT_INT32,		RAWT_INT64,
		RAWT_UINT8,		RAWT_UINT16,	RAWT_UINT32,	RAWT_UINT64,
		RAWT_DOUBLE,	RAWT_STRING,	RAWT_VECTOR,	RAWT_SEQUENCE,
		RAWT_DATE,		RAWT_TIME,		RAWT_ISNONE,
	};

public:
	FdDefinition();

	FdDefinition( unsigned int nID, const char* pszName, short nNameLen, FdDefinition::FIELD_TYPE nType, size_t nBytesNum, unsigned short nPrecision );

	FdDefinition( FdDefinition& FromObj );

	/**
	 * @brief		�����ֶ�ID
	 */
	unsigned int	ID() const;

	/**
	 * @brief		�����ֶ�����
	 */
	FdDefinition::FIELD_TYPE	Type() const;

	/**
	 * @brief		���������Ͷ����ռλ��С
					ע:
						��SEQUENCE����Ϊ�����еĳ���;
						������ΪRAWTYPE��ռ���ֽ���;
	 */
	size_t			SizeOfType() const;

	/**
	 * @brief		С��λ������
	 */
	unsigned short	Precision() const;

	/**
	 * @brief		ȡ�ֶ�����
	 */
	void			GetFieldName( char (&pszName)[64] );

	/**
	 * @brief		�Ƚ��Ƿ�����ͬ���ֶ�����
	 */
	bool			Compare( std::string& sFieldName ) const;

protected:
	unsigned int			m_nFieldID;						///< Field ID
	char					m_sFieldName[64];				///< �ֶ���
	FIELD_TYPE				m_nFieldType;					///< ��������
	size_t					m_nBytesNum;					///< ���ݸ���(ע:��SEQUENCE����Ϊ�����еĳ���;������ΪRAWTYPE��ռ���ֽ���)
	unsigned short			m_nPrecision;					///< С��λ������
};


/**
 * @class		SerialString
 * @brief		�ַ���������(���л����ͣ�
 */
class SerialString
{
public:
	/**
	 * @brief			�ַ������������ͣ�
	 * @param[in]		nRawLen		�ֶεĶ��峤��
	 * @param[in]		pStr		�������ַ�����ַ
	 * @param[in]		nLen		�ַ�����Ч����
	 */
	SerialString( short nRawLen, char *pStr = NULL, short nLen = 0 );

public:
	char				*pszStr;			///< �ַ������õ�ַ
	short				nStrLen;			///< �ַ�������
	short				nDefineLen;			///< ԭʼ���峤��
};


/**
 * @class		SerialBytes
 * @brief		�ֽڴ�������(���л����ͣ�
 */
class SerialBytes
{
public:
	/**
	 * @brief			�ֽڴ����������ͣ�
	 * @param[in]		nRawLen		�ֶεĶ��峤��
	 * @param[in]		pStr		�������ֽڴ���ַ
	 * @param[in]		nLen		�ֽڴ���Ч����
	 */
	SerialBytes( short nRawLen, char *pStr = NULL, short nLen = 0 );
public:
	char				*pszStr;			///< �ֽڴ����õ�ַ
	short				nStrLen;			///< �ֽڴ�����
	short				nDefineLen;			///< ԭʼ���峤��
};


class	MsgField;
/**
 * @class		SerialNumber
 * @brief		������ֵ��ֵcopy���ͣ�(���л����ͣ�
 */
class SerialNumber
{
public:
	/**
	 * @brief				������ֵ��װ
	 * @param[in]			nVal			�������͵���ֵ
	 * @param[in]			nType			����ֵ����, ����������ͽ�ֵ������Ӧ���ֶ�
	 */
	SerialNumber( __int64 nVal = 0, enum FdDefinition::FIELD_TYPE nType = FdDefinition::RAWT_ISNONE );

	/**
	 * @brief				������ֵ��װ
	 * @param[in]			pBuf			�������͵����ݵĻ���
	 * @param[in]			nType			����ֵ����, ����������ͽ�ֵ������Ӧ���ֶ�
	 */
	SerialNumber( const char* pBuf, enum FdDefinition::FIELD_TYPE nType );

	/**
	 * @brief				������ֵ��װ
	 * @param[in]			nVal			�������͵���ֵ
	 * @param[in]			pMFd			��Ϣ�ֶ�ģ�嶨��
	 * @note				������Ҫ����ֵԭ����Ϊunsigned���͵��ֶΣ����Զ�������ת����signed��
	 */
	SerialNumber( __int64 nVal, MsgField* pMFd );

	/**
	 * @brief				(��)��������,ת����Ӧ����ֵ
	 * @note				��ȡʱ�����ͽ����ϵ���������Ҫ�Բ�ͬ���ȵ�number����ͬ�ķ���
	 */
	operator __int64() const;

	/**
	 * @brief				(��)��������,ת����Ӧ����ֵ
	 * @note				��ȡʱ�����ͽ����ϵ���������Ҫ�Բ�ͬ���ȵ�number����ͬ�ķ���
	 */
	operator unsigned __int64() const;

	/**
	 * @brief				(д)��������,ת����Ӧ����ֵ
	 * @note				д����ʱ����д�뷽ʽ����д���ʲô���͵����ݳ���
	 */
	operator __int64&();

	/**
	 * @brief				(д)��������,ת����Ӧ����ֵ
	 * @note				д����ʱ����д�뷽ʽ����д���ʲô���͵����ݳ���
	 */
	operator unsigned __int64&();

	/**
	 * @brief				����ֵ���ݸ����͵ĳ���copy���ڴ�
	 */
	bool	operator >> ( char* pBuf );

	/**
	 * @brief				�������е�����copy��������
	 */
	bool	operator << ( const char* pBuf );

	/**
	 * @brief				����nDataType�ж��Ƿ�Ϊ�з�������
							�ж�����Ϊ nDataType < UINT8 Ϊ�з�����
	 * @return				true		�з���
							false		�޷���
	 */
	bool	IsSigned()	const;

	/**
	 * @brief				�ж��Ƿ�Ϊ����( >= 0 )
	 */
	bool	IsPositive() const;

public:
	enum FdDefinition::FIELD_TYPE	nDataType;			///< ��������
	union										///< ��ֵ��
	{
		unsigned __int64	nUInt64;
		__int64				nInt64;
		unsigned int		nUInt32;
		int					nInt32;
		unsigned short		nUInt16;
		short				nInt16;
		unsigned char		nUInt8;
		char				nInt8;
	};
};


#endif










