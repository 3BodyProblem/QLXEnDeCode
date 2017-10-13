#ifndef __QLX_SERIALIZER_H__
#define __QLX_SERIALIZER_H__


#pragma warning(disable : 4786)
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)
#include <istream>
#include "EnDePolicy.h"
#include "OPCollection.h"


#define		MAX_DATABLOCK_SIZE			2048				///< 最长行情数据结构长度


/**
 * @class		FrameSerializer
 * @brief		帧序列化
 */
class FrameSerializer
{
public:
	FrameSerializer();

	/**
	 * @brief		绑定一个protobuf序列化策略对象
	 * @param[in]	pOperation	序列化策略
	 * @return		=0			成功
	 */
	int	BindOperation( IPbOperation* pOperation );

	/**
	 * @brief		返回已经序列化的长度
	 */
	int	GetDataSize();

	/**
	 * @brief			取得缓存地址
	 */
	operator char*();

	/**
	 * @brief			attach一块可以写入的缓存
	 */
	void Attach( unsigned char *ptr, size_t size );

	/**
	 * @brief			赋值MessageID,且清空buffer的长度计数
	 */
	bool NewMessage( unsigned short nMsgID );

	/**
	 * @brief			序列化一个bytes数组
	 * @return			写入序列化bit数
	 */
	int	operator << ( const SerialBytes& val );

	/**
	 * @breif			序列化一个字符串
	 * @return			写入序列化bit数
	 */
	int	operator << ( const SerialString& val );

	/**
	 * @brief			序列化一个任意长度的数值
	 * @return			写入序列化bit数
	 */
	int	operator << ( const SerialNumber& val );

	/**
	 * @brief			将数据序列化后刷入缓存
	 * @return			<0		失败
	 */
	int Serialize();

private:
	OperationCollection		m_Operations;						///< 编码策略集
	char					m_pszDataBuf[MAX_DATABLOCK_SIZE];	///< 对一次encode，所使用的message的差值缓存
	unsigned int			m_nDataBufSize;						///< m_pszDataBuf的有效数据长度
	unsigned char*			m_buf;								///< 缓存
	size_t					m_buflen;							///< 缓存总长度
	size_t					m_curpos;							///< 待写入位置
	unsigned short			m_nMsgID;
};


/**
 * @class		FrameUnserializer
 * @brief		帧反序列化
 */
class FrameUnserializer
{
public:
	FrameUnserializer();

	/**
	 * @brief		返回未序列化部分的长度
	 */
	int	GetDataSize();

	/**
	 * @brief		绑定一个protobuf序列化策略对象
	 * @param[in]	pOperation	序列化策略
	 * @return		=0			成功
	 */
	int	BindOperation( IPbOperation* pOperation );

	/**
	 * @brief			attach一块可以读取的缓存
	 */
	void Attach( unsigned char *ptr, size_t size );

	/**
	 * @brief			读出一个bytes串
	 * @return			读出序列化的bit数
	 */
	int operator >> ( SerialBytes& val );

	/**
	 * @brief			读出一个字符串
	 * @return			读出序列化的bit数
	 */
	int operator >> ( SerialString& val );

	/**
	 * @brief			读出一个任意长度的数值
	 * @return			读出序列化的bit数
	 */
	int operator >> ( SerialNumber& val );

	/**
	 * @brief			将缓存中的数据反序列化
	 * @return			<0		失败
	 */
	int Unserialize();

private:
	OperationCollection		m_Operations;						///< 编码策略集
	char					m_pszDataBuf[MAX_DATABLOCK_SIZE];	///< 对一次encode，所使用的message的差值缓存
	unsigned int			m_nDataBufSize;						///< m_pszDataBuf的有效数据长度
	unsigned char*			m_buf;								///< 缓存
	size_t					m_buflen;							///< 缓存总长度
	size_t					m_curpos;							///< 待读出位置
};




#endif






