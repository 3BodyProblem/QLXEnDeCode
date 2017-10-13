#ifndef __QLX_ENDE_POLICY_H__
#define __QLX_ENDE_POLICY_H__


#pragma warning(disable : 4786)
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)
#include <map>
#include "math.h"
#include "stdio.h"
#include <vector>
#include <string>
#include "EnDeType.h"
#include "IEnDeCode.h"
#include "MarkupSTL.h"


/**
 * @brief		模块版本号
 */
char* GetModuleVersion();


class MsgField;
class MsgPolicy;


/**
 * @class		EncodePolicyDefinition
 * @brief		编码策略定义
 */
class EncodePolicyDefinition
{
public:///////////////////////////////////////////<	定义类型
	enum RELATION	{ INDEPENDENCE, COUNTERPART, CORRELATE };	///< 编码关联类型 独立属性      Msg间作差值  Msg内作差值
	enum OPERATOR	{ NONE, ADD, DEDUCT };						///< 编码计算符   无    加   减

public:
	EncodePolicyDefinition();

	EncodePolicyDefinition( EncodePolicyDefinition::OPERATOR nOP, EncodePolicyDefinition::RELATION nRelation, MsgField* pRelateField, MsgField* pSelfField );

	EncodePolicyDefinition( EncodePolicyDefinition& FromObj );

	/**
	 * @brief			差值操作类型
	 */
	EncodePolicyDefinition::OPERATOR			Operation() const;

	/**
	 * @brief			差值关系
	 */
	EncodePolicyDefinition::RELATION			Relation() const;

	/**
	 * @brief			差值操作关联的字段引用
	 */
	MsgField*									RelatedField() const;

protected:
	OPERATOR			m_nOperator;							///< 计算符
	RELATION			m_nRelation;							///< 关联类型
	MsgField*			m_pRelateField;							///< 关联字段
};


/**
 * @class		MsgField
 * @brief		字段描述
 */
class MsgField
{
friend class			Message;
#define					MAX_ITEMNO			128					///< 最大字段数
#define					MAX_STRLEN			64					///< 字大序列串/字符串长度
private:
	/**
	 * @brief			构造函数
	 * @param[in]		nRelation			关联类型
	 */
	MsgField( unsigned int nFieldID, MsgPolicy* pMsgPolicy, std::string sFieldName = ""
			, FdDefinition::FIELD_TYPE nType = FdDefinition::RAWT_INT8, size_t nFieldLen = 1
			, EncodePolicyDefinition::OPERATOR nOperator = EncodePolicyDefinition::NONE, EncodePolicyDefinition::RELATION nRelation = EncodePolicyDefinition::INDEPENDENCE
			, const MsgField* pRelateField = NULL, unsigned short nPrecision = 0 );

public://////////////////////////////////////////////< 方法定义
	MsgField( const MsgField& fd );

	~MsgField();

	/**
	 * @brief			返回Sequence字段的item
	 */
	const MsgField*			operator[] ( unsigned short nIndex ) const;

	/**
	 * @brief			增加SEQUENCE的item到当前SEQUENCE对象中
	 */
	MsgField&				operator+= ( const MsgField &fd );

public:
	SerialNumber			X_CompressNumber( const char* pData );
	SerialString			X_CompressString( const char* pData, size_t nDataLen );
	int						X_UnCompressNumber( SerialNumber& oNumber, char* pData, size_t nDataLen );
	int						X_UnCompressString( SerialString& oString, char* pData, size_t nDataLen );

public:
	/**
	 * @brief			取字段信息
	 */
	const FdDefinition&		FieldDefinition() const;

	/**
	 * @brief			取编码策略
	 */
	const EncodePolicyDefinition&	EncodeRuleDefinition() const;

	/**
	 * @brief			返回Sequence字段的长度
	 * @return			-1		非SEQUENCE字段返回-1
						>=0		返回SEQUENCE字段的长度
	 */
	int						Size() const;

	/**
	 * @brief			取得做差值的基值(上一次本字段的值)
	 */
	SerialNumber&			GetLastNumber();

	/**
	 * @brief			取得做差值的基值(上一次本字段的值)
	 */
	char*					GetLastString();

	bool&					GetConvertUnignedMark();

protected:
	/**
	 * @brief				累加本字段的基值(前值)引用计数
							&&
							返回累加前的原始计数值
	 * @return				返回累加前的原始计数值
	 */
	int						AddAndReturnRefCnt();

private://///////////////////////////////////< 字段属性信息定义
	FdDefinition			m_fdDefinition;				///< 数据原始类型
	EncodePolicyDefinition	m_policyDefinition;			///< 差值化压缩
	MsgPolicy*				m_pMsgPolicy;				///< 消息压缩策略
	MsgField*				m_lstField[MAX_ITEMNO];		///< 如果是SEQUENCE,存储相关的MsgField列表
	unsigned int			m_nFieldNum;				///< SEQUENCE的有效子字段数

private://///////////////////////////////////< 加解码的中间状态属性
	SerialNumber			m_oLastNumber;				///< 真实值的基值，用来当做差值的依据
	char					m_oLastString[MAX_STRLEN];	///< 真实值的基值，用来当做差值的依据
	bool					m_bConvertUnsigned;			///< Correlate字段，默认都是带符号的数值类型; 是否为第一个循环(在队列中)
};


/**
 * @class		Message
 * @brief		消息参数
 */
class Message
{
friend class	MsgPolicy;
public:
	MsgField&			Grab( std::string sFieldName, FdDefinition::FIELD_TYPE nType = FdDefinition::RAWT_INT8, size_t nFieldLen = 1
							, EncodePolicyDefinition::OPERATOR nOperator = EncodePolicyDefinition::NONE
							, EncodePolicyDefinition::RELATION nRelation = EncodePolicyDefinition::INDEPENDENCE
							, const MsgField* pRelateField = NULL, unsigned short nPrecision = 0 );

	/**
	 * @brief			增加MsgField对象描述
	 */
	MsgField&			operator += ( const MsgField& fd );

	/**
	 * @brief			根据message中的field的name查询某个field地址
	 * @return			NULL		如果某个name不存在于message中
	 */
	MsgField*			QueryFieldByFName( const std::string& sCorrelate, const std::string& sCounterpart );

	/**
	 * @brief			返回字段数
	 */
	unsigned int		GetFieldNum() const;

	/**
	 * @brief			根据index取得字段定义
	 * @return			NULL		错误
	 */
	MsgField*			operator[] ( unsigned int nIndex );

private:
	Message( std::string sMsgName, MsgPolicy *pPolicy );

private:
	std::string				m_sMsgName;					///< Message Name
	bool					m_bIndexByCode;				///< 是否有Code索引
	MsgField*				m_lstField[MAX_ITEMNO];		///< 如果是SEQUENCE,存储相关的MsgField列表
	unsigned int			m_nFieldNum;				///< 有效子字段数
	MsgPolicy*				m_pMsgPolicy;				///< 消息压缩策略
};


/**
 * @class	RefCounter
 * @brief	各字段的前值引用计数的维护类
 */
class RefCounter
{
public:
	RefCounter();

	/**
	 * @brief			释放所有数据
	 */
	void				Release();

	/**
	 * @brief			清空所有前值到达标识
	 */
	void				ResetCounter();

	/**
	 * @brief			根据FieldID取得该字段的前值引用
	 */
	int&				operator [] ( unsigned int nFieldID );

protected:
	int					m_lstRefCounter[1024*8];
};


/**
 * @class		MsgFieldObjCache
 * @brief		用于分配MsgField对象的缓存
 */
class MsgFieldObjCache
{
public:
	/**
	 * @brief			构造函数
	 * @param[in]		nMaxFieldNum		最多可分配的MsgField对象的数量
	 */
	MsgFieldObjCache( unsigned int nMaxFieldNum = MAX_ITEMNO * 1024 );

	/**
	 * @brief			析构,会调用Release方法
	 */
	~MsgFieldObjCache();

	/**
	 * @brief			初始化内存
	 */
	bool				Instance();

	/**
	 * @brief			释放内存
	 */
	void				Release();

	/**
	 * @brief			分配一块空间用于新的MsgField对象
	 */
	MsgField*			operator()();

	/**
	 * @brief			取得当前已经分配了多少对象
	 */
	unsigned int		GetCurrentNum();

protected:
	MsgField			*m_pMsgFieldObjBuffer;				///< MsgField的分配缓存
	unsigned int		m_nMsgFieldsObjNumber;				///< 已经分配的数量
	unsigned int		m_nMaxFieldsObjNumber;				///< 最大可分配的数量
};


/**
 * @class		MsgPolicy
 * @brief		加载 encode/decode 策略(xml模板)
 * @author		barry
 * @note		一个dll模块只允许加载一份xml策略模板，虽然可以创建多个encode对象或者decode对象
 */
class MsgPolicy
{
public:
	MsgPolicy();
	~MsgPolicy();

	/**
	 * @brief			初始化，解析xml文件/加载策略模块
	 */
	int					Initialize( const char* pszTemplateFile );

	/**
	 * @brief			释放资源
	 */
	void				Release();

public:
	/**
	 * @brief			根据msgid返回消息定义模板
	 * @return			NULL			msgid无效
	 */
	Message*			operator[] ( unsigned int nMsgID );

	/**
	 * @brief			取得Fields前值维护对象的引用
	 */
	RefCounter&			GetRefCounter();

	/**
	 * @brief			取得缓存对象
	 */
	MsgFieldObjCache&	GetFieldCache();

	unsigned int		GetBaseDate();
	unsigned int		GetBaseTime();

protected:
	/**
	 * @brief			增加Message字义
	 * @return			返回增加后的Message数
						< 0		失败
	 */
	Message&			GrabNewMessage( int nMsgID, std::string sMsgName );

	/**
	 * @brief			按Message列表加载xml中的子列表项
	 * @return			>0		成功，每个Message所对应内存结构的长度
						<=0		失败
	 */
	int					LoadSubPolicy( CMarkupSTL& xmlparser, Message& msg, MsgField* pSubField );

private:
	MsgFieldObjCache			m_oFieldObjCache;					///< 分配MsgField地址的缓存
	RefCounter					m_oFieldRefCounter;					///< 所有Fields的前值引用计数的维护对象
	std::vector<Message>		m_lstMessage;						///< Message Template
	unsigned short				m_lstMsgID2Index[0xFFFF];			///< Get Index of 'm_lstMessage' by MsgID
	unsigned int				m_nBaseDate;						///< 基础日期
	unsigned int				m_nBaseTime;						///< 日期时间
};


#endif







