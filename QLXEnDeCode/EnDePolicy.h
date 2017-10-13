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
 * @brief		ģ��汾��
 */
char* GetModuleVersion();


class MsgField;
class MsgPolicy;


/**
 * @class		EncodePolicyDefinition
 * @brief		������Զ���
 */
class EncodePolicyDefinition
{
public:///////////////////////////////////////////<	��������
	enum RELATION	{ INDEPENDENCE, COUNTERPART, CORRELATE };	///< ����������� ��������      Msg������ֵ  Msg������ֵ
	enum OPERATOR	{ NONE, ADD, DEDUCT };						///< ��������   ��    ��   ��

public:
	EncodePolicyDefinition();

	EncodePolicyDefinition( EncodePolicyDefinition::OPERATOR nOP, EncodePolicyDefinition::RELATION nRelation, MsgField* pRelateField, MsgField* pSelfField );

	EncodePolicyDefinition( EncodePolicyDefinition& FromObj );

	/**
	 * @brief			��ֵ��������
	 */
	EncodePolicyDefinition::OPERATOR			Operation() const;

	/**
	 * @brief			��ֵ��ϵ
	 */
	EncodePolicyDefinition::RELATION			Relation() const;

	/**
	 * @brief			��ֵ�����������ֶ�����
	 */
	MsgField*									RelatedField() const;

protected:
	OPERATOR			m_nOperator;							///< �����
	RELATION			m_nRelation;							///< ��������
	MsgField*			m_pRelateField;							///< �����ֶ�
};


/**
 * @class		MsgField
 * @brief		�ֶ�����
 */
class MsgField
{
friend class			Message;
#define					MAX_ITEMNO			128					///< ����ֶ���
#define					MAX_STRLEN			64					///< �ִ����д�/�ַ�������
private:
	/**
	 * @brief			���캯��
	 * @param[in]		nRelation			��������
	 */
	MsgField( unsigned int nFieldID, MsgPolicy* pMsgPolicy, std::string sFieldName = ""
			, FdDefinition::FIELD_TYPE nType = FdDefinition::RAWT_INT8, size_t nFieldLen = 1
			, EncodePolicyDefinition::OPERATOR nOperator = EncodePolicyDefinition::NONE, EncodePolicyDefinition::RELATION nRelation = EncodePolicyDefinition::INDEPENDENCE
			, const MsgField* pRelateField = NULL, unsigned short nPrecision = 0 );

public://////////////////////////////////////////////< ��������
	MsgField( const MsgField& fd );

	~MsgField();

	/**
	 * @brief			����Sequence�ֶε�item
	 */
	const MsgField*			operator[] ( unsigned short nIndex ) const;

	/**
	 * @brief			����SEQUENCE��item����ǰSEQUENCE������
	 */
	MsgField&				operator+= ( const MsgField &fd );

public:
	SerialNumber			X_CompressNumber( const char* pData );
	SerialString			X_CompressString( const char* pData, size_t nDataLen );
	int						X_UnCompressNumber( SerialNumber& oNumber, char* pData, size_t nDataLen );
	int						X_UnCompressString( SerialString& oString, char* pData, size_t nDataLen );

public:
	/**
	 * @brief			ȡ�ֶ���Ϣ
	 */
	const FdDefinition&		FieldDefinition() const;

	/**
	 * @brief			ȡ�������
	 */
	const EncodePolicyDefinition&	EncodeRuleDefinition() const;

	/**
	 * @brief			����Sequence�ֶεĳ���
	 * @return			-1		��SEQUENCE�ֶη���-1
						>=0		����SEQUENCE�ֶεĳ���
	 */
	int						Size() const;

	/**
	 * @brief			ȡ������ֵ�Ļ�ֵ(��һ�α��ֶε�ֵ)
	 */
	SerialNumber&			GetLastNumber();

	/**
	 * @brief			ȡ������ֵ�Ļ�ֵ(��һ�α��ֶε�ֵ)
	 */
	char*					GetLastString();

	bool&					GetConvertUnignedMark();

protected:
	/**
	 * @brief				�ۼӱ��ֶεĻ�ֵ(ǰֵ)���ü���
							&&
							�����ۼ�ǰ��ԭʼ����ֵ
	 * @return				�����ۼ�ǰ��ԭʼ����ֵ
	 */
	int						AddAndReturnRefCnt();

private://///////////////////////////////////< �ֶ�������Ϣ����
	FdDefinition			m_fdDefinition;				///< ����ԭʼ����
	EncodePolicyDefinition	m_policyDefinition;			///< ��ֵ��ѹ��
	MsgPolicy*				m_pMsgPolicy;				///< ��Ϣѹ������
	MsgField*				m_lstField[MAX_ITEMNO];		///< �����SEQUENCE,�洢��ص�MsgField�б�
	unsigned int			m_nFieldNum;				///< SEQUENCE����Ч���ֶ���

private://///////////////////////////////////< �ӽ�����м�״̬����
	SerialNumber			m_oLastNumber;				///< ��ʵֵ�Ļ�ֵ������������ֵ������
	char					m_oLastString[MAX_STRLEN];	///< ��ʵֵ�Ļ�ֵ������������ֵ������
	bool					m_bConvertUnsigned;			///< Correlate�ֶΣ�Ĭ�϶��Ǵ����ŵ���ֵ����; �Ƿ�Ϊ��һ��ѭ��(�ڶ�����)
};


/**
 * @class		Message
 * @brief		��Ϣ����
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
	 * @brief			����MsgField��������
	 */
	MsgField&			operator += ( const MsgField& fd );

	/**
	 * @brief			����message�е�field��name��ѯĳ��field��ַ
	 * @return			NULL		���ĳ��name��������message��
	 */
	MsgField*			QueryFieldByFName( const std::string& sCorrelate, const std::string& sCounterpart );

	/**
	 * @brief			�����ֶ���
	 */
	unsigned int		GetFieldNum() const;

	/**
	 * @brief			����indexȡ���ֶζ���
	 * @return			NULL		����
	 */
	MsgField*			operator[] ( unsigned int nIndex );

private:
	Message( std::string sMsgName, MsgPolicy *pPolicy );

private:
	std::string				m_sMsgName;					///< Message Name
	bool					m_bIndexByCode;				///< �Ƿ���Code����
	MsgField*				m_lstField[MAX_ITEMNO];		///< �����SEQUENCE,�洢��ص�MsgField�б�
	unsigned int			m_nFieldNum;				///< ��Ч���ֶ���
	MsgPolicy*				m_pMsgPolicy;				///< ��Ϣѹ������
};


/**
 * @class	RefCounter
 * @brief	���ֶε�ǰֵ���ü�����ά����
 */
class RefCounter
{
public:
	RefCounter();

	/**
	 * @brief			�ͷ���������
	 */
	void				Release();

	/**
	 * @brief			�������ǰֵ�����ʶ
	 */
	void				ResetCounter();

	/**
	 * @brief			����FieldIDȡ�ø��ֶε�ǰֵ����
	 */
	int&				operator [] ( unsigned int nFieldID );

protected:
	int					m_lstRefCounter[1024*8];
};


/**
 * @class		MsgFieldObjCache
 * @brief		���ڷ���MsgField����Ļ���
 */
class MsgFieldObjCache
{
public:
	/**
	 * @brief			���캯��
	 * @param[in]		nMaxFieldNum		���ɷ����MsgField���������
	 */
	MsgFieldObjCache( unsigned int nMaxFieldNum = MAX_ITEMNO * 1024 );

	/**
	 * @brief			����,�����Release����
	 */
	~MsgFieldObjCache();

	/**
	 * @brief			��ʼ���ڴ�
	 */
	bool				Instance();

	/**
	 * @brief			�ͷ��ڴ�
	 */
	void				Release();

	/**
	 * @brief			����һ��ռ������µ�MsgField����
	 */
	MsgField*			operator()();

	/**
	 * @brief			ȡ�õ�ǰ�Ѿ������˶��ٶ���
	 */
	unsigned int		GetCurrentNum();

protected:
	MsgField			*m_pMsgFieldObjBuffer;				///< MsgField�ķ��仺��
	unsigned int		m_nMsgFieldsObjNumber;				///< �Ѿ����������
	unsigned int		m_nMaxFieldsObjNumber;				///< ���ɷ��������
};


/**
 * @class		MsgPolicy
 * @brief		���� encode/decode ����(xmlģ��)
 * @author		barry
 * @note		һ��dllģ��ֻ�������һ��xml����ģ�壬��Ȼ���Դ������encode�������decode����
 */
class MsgPolicy
{
public:
	MsgPolicy();
	~MsgPolicy();

	/**
	 * @brief			��ʼ��������xml�ļ�/���ز���ģ��
	 */
	int					Initialize( const char* pszTemplateFile );

	/**
	 * @brief			�ͷ���Դ
	 */
	void				Release();

public:
	/**
	 * @brief			����msgid������Ϣ����ģ��
	 * @return			NULL			msgid��Ч
	 */
	Message*			operator[] ( unsigned int nMsgID );

	/**
	 * @brief			ȡ��Fieldsǰֵά�����������
	 */
	RefCounter&			GetRefCounter();

	/**
	 * @brief			ȡ�û������
	 */
	MsgFieldObjCache&	GetFieldCache();

	unsigned int		GetBaseDate();
	unsigned int		GetBaseTime();

protected:
	/**
	 * @brief			����Message����
	 * @return			�������Ӻ��Message��
						< 0		ʧ��
	 */
	Message&			GrabNewMessage( int nMsgID, std::string sMsgName );

	/**
	 * @brief			��Message�б����xml�е����б���
	 * @return			>0		�ɹ���ÿ��Message����Ӧ�ڴ�ṹ�ĳ���
						<=0		ʧ��
	 */
	int					LoadSubPolicy( CMarkupSTL& xmlparser, Message& msg, MsgField* pSubField );

private:
	MsgFieldObjCache			m_oFieldObjCache;					///< ����MsgField��ַ�Ļ���
	RefCounter					m_oFieldRefCounter;					///< ����Fields��ǰֵ���ü�����ά������
	std::vector<Message>		m_lstMessage;						///< Message Template
	unsigned short				m_lstMsgID2Index[0xFFFF];			///< Get Index of 'm_lstMessage' by MsgID
	unsigned int				m_nBaseDate;						///< ��������
	unsigned int				m_nBaseTime;						///< ����ʱ��
};


#endif







