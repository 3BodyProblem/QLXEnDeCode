#ifndef __QLX_INTERFACE_ENDECODE_H__
#define __QLX_INTERFACE_ENDECODE_H__


/**
 * @class	IPbOperation
 * @brief	protobuf编码策略
			对每一个类型的protobuf的message类型，派生一个对应的编码策略类
			之后再绑定到en/decode实例上
 */
class IPbOperation
{
public:
	/**
	 * @brief		取得某protobuf message id
	 * @return		协议号
	 */
	virtual	int		MessageID() const = 0;

	/**
	 * @brief		序列化protobuf对象到缓存
	 * @param[in]	pszBuf		缓存地址
	 * @param[in]	nBufLen		缓存长度
	 * @param[out]	pszOutput	输出解码数据
	 * @param[in]	nOutputLen	输出缓存的长度
	 * @return		返回序列化写入的缓存长度
	 */
	virtual int		Serialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen ) = 0;

	/**
	 * @brief		从缓存反序列化protobuf对象
	 * @param[in]	pszBuf		缓存地址
	 * @param[in]	nBufLen		缓存长度
	 * @param[out]	pszOutput	输出解码数据,	* 注，这个输出缓存可能留上有次操作的残余数据，需要在sizeof(MessageType)的空间范围内做全字段写入
	 * @param[in]	nOutputLen	输出缓存的长度
	 * @return		返回读出的缓存长度
	 * @note		注意，输出参数pszOutput, 可能留上有次操作的残余数据，需要在sizeof(MessageType)的空间范围内做全字段写入
	 */
	virtual int		Unserialize( char* pszBuf, int nBufLen, char* pszOutput, int nOutputLen ) = 0;
};


/**
 * @class	InterfaceEncode
 * @brief	Encode接口
 */
class InterfaceEncode
{
public:
	/**
	 * @brief		初始化
	 * @param[in]	pszTemplateFile	编码策略文件
	 * @param[in]	pListOperation	protobuf编码对象列表地址
	 * @param[in]	nListSize		protobuf编码对象列表长度
	 * @return		0			成功
	 */
	virtual int Initialize( const char* pszTemplateFile, IPbOperation** pListOperation, int nListSize ) = 0;

	/**
	 * @brief		释放接口
	 */
	virtual void Release() = 0;

	/**
	 * @brief		准备填写一个新的frame结构
	 * @param[in]	pBuffer		缓存地址
	 * @param[in]	nBufferLne	缓存长度
	 * @return		= 0			成功
	 */
	virtual int Attach2Buffer( char* pBuffer, unsigned int nBufferLen ) = 0;

	/**
	 * @brief		在frmae中填充msg结构
	 * @param[in]	nMsgID		Message ID
	 * @param[in]	pData		消息数据地址
	 * @param[in]	nLen		消息长度
	 * @return		> 0			成功，返回历次调用累积的序列化的长度
	 */
	virtual int EncodeMessage( unsigned short nMsgID, const char *pData, unsigned int nLen ) = 0;
};


/**
 * @class	InterfaceDecode
 * @brief	Decode接口
 */
class InterfaceDecode
{
public:
	/**
	 * @brief		初始化
	 * @param[in]	pszTemplateFile	编码策略文件
	 * @param[in]	pListOperation	protobuf编码对象列表地址
	 * @param[in]	nListSize		protobuf编码对象列表长度
	 * @return		0			成功
	 */
	virtual int Initialize( const char* pszTemplateFile, IPbOperation** pListOperation, int nListSize ) = 0;

	/**
	 * @brief		释放接口
	 */
	virtual void Release() = 0;

	/**
	 * @brief		准备填写一个新的frame结构
	 * @param[in]	pBuffer		缓存地址
	 * @param[in]	nBufferLne	缓存有效数据的长度
	 * @return		= 0			成功
	 */
	virtual int Attach2Buffer( char* pBuffer, unsigned int nBufferLen ) = 0;

	/**
	 * @brief		在frmae中填充msg结构
	 * @param[out]	nMsgID		返回Message ID
	 * @param[in]	pData		消息数据地址
	 * @param[in]	nLen		消息长度
	 * @return		>= 0		成功，返回历次调用后,未序列化部分的长度
	 */
	virtual int DecodeMessage( unsigned short& nMsgID, char *pData, unsigned int nLen ) = 0;
};


#endif





