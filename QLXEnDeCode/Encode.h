#ifndef __QLX_ENCODE_H__
#define __QLX_ENCODE_H__


#pragma warning(disable : 4786)
#pragma warning(disable : 4244)
#include "IEnDeCode.h"
#include "Serializer.h"


/**
 * @class	Encode
 * @brief	根据压缩策略进行差值化压缩
 */
class Encode : public InterfaceEncode
{
public:
	Encode();

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
	 * @param[in]	nBufferLne	缓存长度
	 * @return		= 0			成功
	 */
	int Attach2Buffer( char* pBuffer, unsigned int nBufferLen );

	/**
	 * @brief		在frmae中填充msg结构
	 * @param[in]	pData		消息数据地址
	 * @param[in]	nLen		消息长度
	 * @return		> 0			成功，返回历次调用累积的序列化的长度
	 */
	int EncodeMessage( unsigned short nMsgID, const char *pData, unsigned int nLen );

	/**
	 * @brief		对字段进行编码
	 * @return		返回字段处理后的偏移值(结构体)
					>=0		代表偏移值
					< 0		代表出错
	 */
	inline int OnEncodeField( MsgField& rField, const char *pData, unsigned int nLen );

	/**
	 * @brief		对序列字段进行编码
	 * @return		返回字段处理后的偏移值(结构体)
					>=0		代表偏移值
					< 0		代表出错
	 */
	inline int OnEncodeSequenceField( MsgField& rField, const char *pData, unsigned int nLen );

private:
	MsgPolicy				m_oMsgPolicy;			///< 策略
	FrameSerializer			m_oSerializer;			///< 序列化
	char					*m_pszBuffer;			///< 缓存空间
};



#endif






