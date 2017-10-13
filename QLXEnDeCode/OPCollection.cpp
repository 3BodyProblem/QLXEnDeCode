#include <new>
#include <math.h>
#include "OPCollection.h"


OPNode::OPNode()
 : m_pNextNode( NULL ), m_pOperation( NULL )
{
}

OPNode* OPNode::Next()
{
	return m_pNextNode;
}

OPNode::operator IPbOperation*()
{
	return m_pOperation;
}

bool OPNode::PushBack( OPNode* pNode )
{
	if( NULL == pNode )
	{
		return false;
	}

	m_pNextNode = pNode;

	return true;
}

OPNode& OPNode::operator=( IPbOperation* pOp )
{
	if( false == pOp )
	{
		return *this;
	}

	m_pOperation = pOp;

	return *this;
}


OperationCollection::OperationCollection()
 : m_nVadlidNodeNum( 0 )
{
	::memset( m_hashNode, 0, sizeof(m_hashNode) );
}

int OperationCollection::AddOperation( IPbOperation* pOp )
{
	if( NULL == pOp )
	{
		return -1;
	}

	int			nMsgID = pOp->MessageID();
	int			nHashKey = nMsgID % MAX_MESSAGE_NUM;

	if( nMsgID < 0 || nMsgID > 0xFFFF )
	{
		return -2;
	}

	OPNode*	pLastNode = NULL;
	OPNode*	pNode = m_hashNode[nHashKey];

	if( NULL != pNode )
	{
		do
		{
			pLastNode = pNode;

			if( ((IPbOperation*)(*pNode))->MessageID() == nMsgID )
			{
				return 0;
			}
		} while( NULL != (pNode = pNode->Next()) );

		OPNode&	refNewNode = m_vctNode[m_nVadlidNodeNum++];

		refNewNode = pOp;
		pLastNode->PushBack( &refNewNode );

		return 0;
	}
	else
	{
		OPNode&	refNewNode = m_vctNode[m_nVadlidNodeNum++];

		refNewNode = pOp;
		m_hashNode[nHashKey] = &refNewNode;

		return 0;
	}
}

IPbOperation* OperationCollection::operator[] ( unsigned int nMsgID )
{
	if( nMsgID < 0 || nMsgID > 0xFFFF )
	{
		return NULL;
	}

	int			nHashKey = nMsgID % MAX_MESSAGE_NUM;
	OPNode*	pNode = m_hashNode[nHashKey];

	while( NULL != pNode )
	{
		if( ((IPbOperation*)(*pNode))->MessageID() == nMsgID )
		{
			return (IPbOperation*)*pNode;
		}

		pNode = pNode->Next();
	}

	return NULL;
}























