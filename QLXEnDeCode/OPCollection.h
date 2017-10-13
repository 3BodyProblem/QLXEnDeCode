#ifndef __QLX_OPERATION_COLLECTION_H__
#define __QLX_OPERATION_COLLECTION_H__


#pragma warning(disable : 4786)
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)
#include <istream>
#include "EnDePolicy.h"


#define		MAX_MESSAGE_NUM				2048				///< ����֧�ִ����message��������


/**
 * @class		OPNode
 * @brief		�ڵ�����
 */
class OPNode
{
public:
	OPNode();

	/**
	 * @brief		��һ���ڵ��ָ��
	 */
	OPNode*	Next();

	/**
	 * @brief		����
	 */
	operator IPbOperation*();

	/**
	 * @brief		���µĽڵ�ӵ����ڵ����
	 */
	bool		PushBack( OPNode* pNode );

	/**
	 * @brief		��ֵ���Զ���
	 */
	OPNode&		operator=( IPbOperation* pOp );

protected:
	OPNode*				m_pNextNode;			///< ��һ��ͬhashkey�Ľڵ�
	IPbOperation*		m_pOperation;			///< ����ָ��
};


/**
 * @class		OperationCollection
 * @brief		���л�����ʵ����ȡhash
 */
class OperationCollection
{
public:
	OperationCollection();

	/**
	 * @brief		�����µ����л����Զ���hash bucket
	 * @param[in]	pOp			�²��Զ���
	 * @return		0			�ɹ�
	 */
	int		AddOperation( IPbOperation* pOp );

	/**
	 * @brief		����messageidȡ����Ӧ�Ĳ��Զ���
	 * @param[in]	nMsgID		Message ID
	 * @return		NULL		�����ڸò���ʵ��
					!=NULL		����ʵ��ָ��
	 */
	IPbOperation*	operator[] ( unsigned int nMsgID );

protected:
	OPNode*			m_hashNode[MAX_MESSAGE_NUM];		///< hash table
	OPNode			m_vctNode[MAX_MESSAGE_NUM];			///< ָ��ڵ�����б�,���ֻ�ܷ���MAX_MESSAGE_NUM��ʵ��
	unsigned int	m_nVadlidNodeNum;					///< ��Ч�ڵ�����
};



#endif






