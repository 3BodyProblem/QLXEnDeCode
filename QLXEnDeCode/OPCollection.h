#ifndef __QLX_OPERATION_COLLECTION_H__
#define __QLX_OPERATION_COLLECTION_H__


#pragma warning(disable : 4786)
#pragma warning(disable : 4244)
#pragma warning(disable : 4996)
#include <istream>
#include "EnDePolicy.h"


#define		MAX_MESSAGE_NUM				2048				///< 最多可支持处理的message类型数量


/**
 * @class		OPNode
 * @brief		节点以象
 */
class OPNode
{
public:
	OPNode();

	/**
	 * @brief		下一个节点的指针
	 */
	OPNode*	Next();

	/**
	 * @brief		重载
	 */
	operator IPbOperation*();

	/**
	 * @brief		将新的节点接到本节点后面
	 */
	bool		PushBack( OPNode* pNode );

	/**
	 * @brief		赋值策略对象
	 */
	OPNode&		operator=( IPbOperation* pOp );

protected:
	OPNode*				m_pNextNode;			///< 下一个同hashkey的节点
	IPbOperation*		m_pOperation;			///< 策略指针
};


/**
 * @class		OperationCollection
 * @brief		序列化策略实例存取hash
 */
class OperationCollection
{
public:
	OperationCollection();

	/**
	 * @brief		增加新的序列化策略对象到hash bucket
	 * @param[in]	pOp			新策略对象
	 * @return		0			成功
	 */
	int		AddOperation( IPbOperation* pOp );

	/**
	 * @brief		根据messageid取出对应的策略对象
	 * @param[in]	nMsgID		Message ID
	 * @return		NULL		不存在该策略实例
					!=NULL		策略实例指针
	 */
	IPbOperation*	operator[] ( unsigned int nMsgID );

protected:
	OPNode*			m_hashNode[MAX_MESSAGE_NUM];		///< hash table
	OPNode			m_vctNode[MAX_MESSAGE_NUM];			///< 指针节点分配列表,最多只能分配MAX_MESSAGE_NUM个实例
	unsigned int	m_nVadlidNodeNum;					///< 有效节点数量
};



#endif






