#ifndef __QLX_ENDE_TYPE_H__
#define __QLX_ENDE_TYPE_H__


#pragma warning(disable : 4786)
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)
#include <string>
#include "stdio.h"


/**
 * @class		FdDefinition
 * @brief		字段原始结构描述
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
	 * @brief		返回字段ID
	 */
	unsigned int	ID() const;

	/**
	 * @brief		返回字段类型
	 */
	FdDefinition::FIELD_TYPE	Type() const;

	/**
	 * @brief		该数据类型定义的占位大小
					注:
						对SEQUENCE类型为该序列的长度;
						其他则为RAWTYPE所占的字节数;
	 */
	size_t			SizeOfType() const;

	/**
	 * @brief		小数位数精度
	 */
	unsigned short	Precision() const;

	/**
	 * @brief		取字段名称
	 */
	void			GetFieldName( char (&pszName)[64] );

	/**
	 * @brief		比较是否有相同的字段名称
	 */
	bool			Compare( std::string& sFieldName ) const;

protected:
	unsigned int			m_nFieldID;						///< Field ID
	char					m_sFieldName[64];				///< 字段名
	FIELD_TYPE				m_nFieldType;					///< 数据类型
	size_t					m_nBytesNum;					///< 数据个数(注:对SEQUENCE类型为该序列的长度;其他则为RAWTYPE所占的字节数)
	unsigned short			m_nPrecision;					///< 小数位数精度
};


/**
 * @class		SerialString
 * @brief		字符串引用类(序列化类型）
 */
class SerialString
{
public:
	/**
	 * @brief			字符串（引用类型）
	 * @param[in]		nRawLen		字段的定义长度
	 * @param[in]		pStr		被引用字符串地址
	 * @param[in]		nLen		字符串有效长度
	 */
	SerialString( short nRawLen, char *pStr = NULL, short nLen = 0 );

public:
	char				*pszStr;			///< 字符串引用地址
	short				nStrLen;			///< 字符串长度
	short				nDefineLen;			///< 原始定义长度
};


/**
 * @class		SerialBytes
 * @brief		字节串引用类(序列化类型）
 */
class SerialBytes
{
public:
	/**
	 * @brief			字节串（引用类型）
	 * @param[in]		nRawLen		字段的定义长度
	 * @param[in]		pStr		被引用字节串地址
	 * @param[in]		nLen		字节串有效长度
	 */
	SerialBytes( short nRawLen, char *pStr = NULL, short nLen = 0 );
public:
	char				*pszStr;			///< 字节串引用地址
	short				nStrLen;			///< 字节串长度
	short				nDefineLen;			///< 原始定义长度
};


class	MsgField;
/**
 * @class		SerialNumber
 * @brief		任意数值（值copy类型）(序列化类型）
 */
class SerialNumber
{
public:
	/**
	 * @brief				构造数值封装
	 * @param[in]			nVal			任意类型的数值
	 * @param[in]			nType			数据值类型, 根据这个类型将值赋给对应的字段
	 */
	SerialNumber( __int64 nVal = 0, enum FdDefinition::FIELD_TYPE nType = FdDefinition::RAWT_ISNONE );

	/**
	 * @brief				构造数值封装
	 * @param[in]			pBuf			任意类型的数据的缓存
	 * @param[in]			nType			数据值类型, 根据这个类型将值赋给对应的字段
	 */
	SerialNumber( const char* pBuf, enum FdDefinition::FIELD_TYPE nType );

	/**
	 * @brief				构造数值封装
	 * @param[in]			nVal			任意类型的数值
	 * @param[in]			pMFd			消息字段模板定义
	 * @note				对于需要做差值原类型为unsigned类型的字段，会自动将类型转换成signed型
	 */
	SerialNumber( __int64 nVal, MsgField* pMFd );

	/**
	 * @brief				(读)根据类型,转出对应的数值
	 * @note				读取时有类型解释上的依赖，需要对不同长度的number作不同的返回
	 */
	operator __int64() const;

	/**
	 * @brief				(读)根据类型,转出对应的数值
	 * @note				读取时有类型解释上的依赖，需要对不同长度的number作不同的返回
	 */
	operator unsigned __int64() const;

	/**
	 * @brief				(写)根据类型,转出对应的数值
	 * @note				写数据时，由写入方式控制写入的什么类型的数据长度
	 */
	operator __int64&();

	/**
	 * @brief				(写)根据类型,转出对应的数值
	 * @note				写数据时，由写入方式控制写入的什么类型的数据长度
	 */
	operator unsigned __int64&();

	/**
	 * @brief				将数值根据各类型的长度copy到内存
	 */
	bool	operator >> ( char* pBuf );

	/**
	 * @brief				将缓存中的内容copy到对象中
	 */
	bool	operator << ( const char* pBuf );

	/**
	 * @brief				根据nDataType判断是否为有符号数字
							判断条件为 nDataType < UINT8 为有符号数
	 * @return				true		有符号
							false		无符号
	 */
	bool	IsSigned()	const;

	/**
	 * @brief				判断是否为正数( >= 0 )
	 */
	bool	IsPositive() const;

public:
	enum FdDefinition::FIELD_TYPE	nDataType;			///< 段字类型
	union										///< 数值区
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










