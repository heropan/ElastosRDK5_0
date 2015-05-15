//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#if _MSC_VER > 1000
#pragma once
#endif

#if !defined(__QUICKLIST_H__)
#define __QUICKLIST_H__


#ifdef __cplusplus
/*!
 @brief To speed up the process of searching for a specific node in the 
 list,QuickListNode data structure manages a hidden balanced tree in addition
 to a list.In other words, we pay the price of 3 more pointers to gain average 
 searching speed of O(log2(N)) rather than O(N).
 @since
 @sa
 */
class QuickListNode {
public:
/*!
 @brief  add the right child to the current list node.
 @param child <b>[in]</b> pointer pointed to the child to be added.
 @since
 @sa
 */
    void AddRight(QuickListNode* child); 
/*!
 @brief  add the left child to the current list node.
 @param child <b>[in]</b> pointer pointed to the child to be added.
 @since
 @sa
 */	

    void AddLeft(QuickListNode* child);  
/*!
 @brief  remove the current list node from list.
 @return
 @since
 @sa
 */	

    QuickListNode* Detach();       
/*!
 @brief  move the current list node to be more closer to the root,return true
 if the current node has turned into root.
 @return
 @since
 @sa
 */	

    _ELASTOS Boolean MoveCloserToRoot();  
/*!
 @brief  return parent node.
 @return
 @since
 @sa
 */
    QuickListNode * Parent();      
/*!
 @brief  return left child.
 @return
 @since
 @sa
 */	
    QuickListNode * Left();          
/*!
 @brief  return right child.
 @return
 @since
 @sa
 */		
    QuickListNode * Right();        
/*!
 @brief  return previous node.
 @return
 @since
 @sa
 */
    QuickListNode * Prev();          
/*!
 @brief  return next node.
 @return
 @since
 @sa
 */
    QuickListNode * Next();  
/*!
 @brief Initialise this quick list,make its root node and link list are initialized.
 @since
 @sa
 */
    void Init();  
/*!
 @brief creates a TreeNode instance.
 */	
    QuickListNode() { }
/*!
 @brief the destructor of QuickListNode.
 */	
    ~QuickListNode() { }

public://protected:
/*!
 @brief the binary tree.
 */	
    TreeNode m_tree;  
/*!
 @brief the sorted link list.
 */		
    ListNode m_list;            
};

#else //!__cplusplus

typedef struct QuickListNode QuickListNode;

struct QuickListNode {
    TreeNode m_tree;                /*!< the binary tree */ 
    ListNode m_list;                /*!< the sorted link list */ 
};

#endif //__cplusplus
/*!
 @brief  remove the list node @a pThis from list.
 @param pThis pointer pointed to the current node to be removed.
 @return
 @since
 @sa
 @relates
 */
ELAPI_(QuickListNode*) QuickListNodeDetach(QuickListNode* pThis);

#ifdef __cplusplus

CAR_INLINE void QuickListNode::AddRight(QuickListNode* child)
{
    m_tree.AddRight((TreeNode *) &(child->m_tree));
    m_list.AddNext((ListNode *) &child->m_list);
}

CAR_INLINE void QuickListNode::AddLeft(QuickListNode* child)
{
    m_tree.AddLeft((TreeNode *) &(child->m_tree));
    m_list.AddPrev((ListNode *) &child->m_list);
}

CAR_INLINE _ELASTOS Boolean QuickListNode::MoveCloserToRoot()
{
    return m_tree.MoveCloserToRoot();
}

CAR_INLINE QuickListNode * QuickListNode::Parent()
{
    return (QuickListNode *) m_tree.Parent();
}

CAR_INLINE QuickListNode * QuickListNode::Left()
{
    return (QuickListNode *) m_tree.Left();
}

CAR_INLINE QuickListNode * QuickListNode::Right()
{
    return (QuickListNode *) m_tree.Right();
}

#define LISTNODE_OFFSET offsetof(QuickListNode, m_list)

CAR_INLINE QuickListNode * QuickListNode::Prev()
{
    return (QuickListNode *)((_ELASTOS Address)m_list.Prev() - LISTNODE_OFFSET);
}

CAR_INLINE QuickListNode * QuickListNode::Next()
{
    return (QuickListNode *)((_ELASTOS Address)m_list.Next() - LISTNODE_OFFSET);
}

CAR_INLINE void QuickListNode::Init()
{
    m_tree.Init();
    m_list.Init();
}

CAR_INLINE QuickListNode* QuickListNode::Detach()
{
	return QuickListNodeDetach(this);
}

#endif //__cplusplus

#endif // __QUICKLIST_H__
