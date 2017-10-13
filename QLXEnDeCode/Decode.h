#ifndef __QLX_DECODE_H__
#define __QLX_DECODE_H__


#pragma warning(disable : 4786)
#pragma warning(disable : 4244)
#include "IEnDeCode.h"
#include "Serializer.h"


/**
 * @class	Decode
 * @brief	根据压缩策略进行差值化解压缩
 */
class Decode : public InterfaceDecode
{
public:
	Decode();

	/**
	 * @brief		初始化
	 * @param[in]	pszTemplateFile	编码策略文件
	 * @param[in]	pListOperation	protobuf编码对象列表地址
	 * @param[in]	nListSize		protobuf编码对象列表长度
	 * @return		0			成功
	 */
	int Initialize( const char* pszTemplateFile, IPbOperation** pListOperation, int nListSize );

	/**
	 * @brief		释放对象
	 */
	void Release();

	/**
	 * @brief		准备填写一个新的frame结构
	 * @param[in]	pBuffer		缓存地址
	 * @param[in]	nBufferLne	缓存有效数据的长度
	 * @return		= 0			成功
	 */
	int Attach2Buffer( char* pBuffer, unsigned int nBufferLen );

	/**
	 * @brief		在frmae中填充msg结构
	 * @param[in]	pData		消息数据地址
	 * @param[in]	nLen		消息长度
	 * @return		>= 0		成功，返回历次调用后,未序列化部分的长度
	 */
	int DecodeMessage( unsigned short& nMsgID, char *pData, unsigned int nLen );

	/**
	 * @brief		对字段进行编码
	 * @return		返回字段处理后的偏移值(结构体)
					>=0		代表偏移值
					< 0		代表出错
	 */
	inline int OnDecodeField( MsgField& rField, char *pData, unsigned int nLen );

	/**
	 * @brief		对序列字段进行编码
	 * @return		返回字段处理后的偏移值(结构体)
					>=0		代表偏移值
					< 0		代表出错
	 */
	inline int OnDecodeSequenceField( MsgField& rField, char *pData, unsigned int nLen );

private:
	MsgPolicy				m_oMsgPolicy;			///< 策略
	FrameUnserializer		m_oUnserializer;		///< 序列化
	char					*m_pszBuffer;			///< 缓存空间
};


#endif









