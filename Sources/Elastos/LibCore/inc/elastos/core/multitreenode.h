//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#if _MSC_VER > 1000
#pragma once
#endif

#if !defined(__MULTITREENODE_H__)
#define __MULTITREENODE_H__

#ifdef __cplusplus
/*!
 * @brief A MultiTree controls multiple children of the same level, instead of at
 * most two kids, e.g., in a binary tree.  There are two kinds of  MultiTreeNode,
 * subtree or leaf.  Roughly speaking, a directory in a file system corresponds to 
 * a subtree whileas a file corresponds to a leaf.
 * @since
 * @sa
 */
class MultiTreeNode {
public:
/*!
 * @brief Add a child node @a pChild for the current tree node.
 * @param pChild <b>[in]</b> the multiTree node to be added.
 * @since
 * @sa
 */
    void AddChild(MultiTreeNode* pChild);
/*!
 * @brief Remove a child node @a pChild from the current tree node.
 * @param pChild <b>[in]</b> the multiTree node to be deleted.
 * @since
 * @sa
 */	
    void DetachChild(MultiTreeNode* pChild); // no recursion
/*!
 * @brief Judge the current node is a leaf node or not.return a boolean value,
 * if @a true it is a leaf node;false otherwise.
 * @return
 * @since
 * @sa
 */
    _ELASTOS Boolean IsLeafNode()           { return m_pSubtree == NULL; }
/*!
 * @brief Construct a MultiTreeNode instance with a null parent node and a null subtree.
 */
    MultiTreeNode() : m_pParent(NULL), m_pSubtree(NULL) { }
/*!
 * @brief return the parent node for the current tree node.
 * @return
 * @since
 * @sa
 */
    MultiTreeNode * Parent()    { return m_pParent ; }
/*!
 * @brief return the child node for the current tree node.
 * @return
 * @since
 * @sa
 */
    MultiTreeNode * Child()     { return m_pSubtree ; }
/*!
 * @brief return the next node for the current tree node.
 * @return
 * @since
 * @sa
 */
    MultiTreeNode * Next()      { return (MultiTreeNode *) m_siblings.Next(); }
/*!
 * @brief return the previous node for the current tree node.
 * @return
 * @since
 * @sa
 */
    MultiTreeNode * Prev()      { return (MultiTreeNode *) m_siblings.Prev(); }
/*!
 * @brief return the first child node for the current tree node.
 * @return
 * @since
 * @sa
 */
    MultiTreeNode * FirstChild();
/*!
 * @brief add the MultiTreeNode @a pNode as the next node of the current tree node.
 * @param pNode <b>[in]</b> The multiTree node to be added.
 * @since
 * @sa
 */
    void AddNext(MultiTreeNode *pNode);
/*!
 * @brief add the MultiTreeNode @a pNode as the previous node of the current tree node.
 * @param pNode <b>[in]</b> The multiTree node to be added.
 * @since
 * @sa
 */
    void AddPrev(MultiTreeNode *pNode);
/*!
 * @brief Add a multiTree node @a pChild and all of its younger siblings as the children 
 * of the current multiTree node.
 * @param pChild <b>[in]</b> the multiTree node to be added.
 * @since
 * @sa
 */
    void AddChildren(MultiTreeNode *pChild);
 /*!
 * @brief Add a multiTree node @a pNode and all of its elder siblings as the siblings 
 * of the current multiTree node.
 * @param pNode <b>[in]</b> pointer pointed to the multiTree node to be added.
 * @since
 * @sa
 */
    void AddSiblingsAfter(MultiTreeNode *pNode);
/*!
 * @brief Add a multiTree node @a pNode and all of its younger siblings as the siblings 
 * of the current multiTree node.
 * @param pNode <b>[in]</b> pointer pointed to the multiTree node to be added.
 * @since
 * @sa
 */
    void AddSiblingsBefore(MultiTreeNode *pNode);
 /*!
 * @brief remove the child node of the current node and all of the siblings of this child
 * @a pChild ,the nodes number to remove must less than @a numToDetach.save the nodes number 
 * that has been removed in @a numDetached.
 * @param pChild <b>[in]</b> the child node of the current node.
 * @param numToDetach <b>[in]</b> the largest nodes number to remove.
 * @param numDetached <b>[out]</b> the nodes number that has been removed .
 * @return
 * @since
 * @sa
 */
    MultiTreeNode * DetachChildAndSiblings(
                MultiTreeNode* pChild, int numToDetach, int* numDetached);
/*!
 * @brief remove the current node itself and all of its siblings,the nodes number to remove 
 * must less than @a numToDetach.save the nodes number that has been removed in @a numDetached.
 * @param numToDetach <b>[in]</b> the largest nodes number to remove.
 * @param numDetached <b>[out]</b> the nodes number that has been removed .
 * @return
 * @since
 * @sa
 */
    MultiTreeNode * DetachSelfAndSiblings(int numToDetach, int* numDetached);
/*!
 * @brief the destructor of the multitreenode. 
 */
    ~MultiTreeNode()            { assert(m_pSubtree == NULL); }

public:
/*!
 * @brief the siblings nodes list.it is a list to link all siblings. 
 */
    ListNode m_siblings;           
/*!
 * @brief  the parent node.everyone has a parent except root. 
 */
    MultiTreeNode* m_pParent;      
/*!
 * @brief the subtree node.the value of @a m_pSubtree is NULL, if this is a leaf node. 
 */
    MultiTreeNode* m_pSubtree;      
};

#else //!__cplusplus

typedef struct MultiTreeNode MultiTreeNode;

struct MultiTreeNode {
    ListNode m_siblings;            /*!< a list to link all siblings*/
    MultiTreeNode* m_pParent;       /*!< everyone has a parent except root*/
    MultiTreeNode* m_pSubtree;      /*!< NULL, if this is a leaf node*/
};

#endif //__cplusplus
/*!
 * @brief Add a multiTree node @a pChild as the child of the current multiTree 
 * node @a pThis.
 * @param pThis  the current multiTree node.
 * @param pChild the multiTree node to be added.
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(void) MultiTreeNodeAddChild(
                            MultiTreeNode * pThis,MultiTreeNode* pChild);
/*!
 * @brief Delete a multiTree node @a pChild from the current multiTree node @a pThis.
 * @param pThis  the current multiTree node.
 * @param pChild the multiTree node to be deleted.
 * @return
 * @since
 * @sa
 * @relates
 */							
ELAPI_(void) MultiTreeNodeDetachChild(  // no recursion
                            MultiTreeNode * pThis,MultiTreeNode* pChild);
ELAPI_(MultiTreeNode*) MultiTreeNodeFirstChild(
                            MultiTreeNode * pThis);
/*!
 * @brief Add the multiTree node @a pNode as the sublings just after the current
 * multiTree node @a pThis.
 * @param pThis the current multiTree node.
 * @param pNode The multiTree node to be added.
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(void) MultiTreeNodeAddNext(
                            MultiTreeNode * pThis,MultiTreeNode *pNode);
/*!
 * @brief Add the multiTree node @a pNode as the sublings just before the current
 * multiTree node @a pThis.
 * @param pThis the current multiTree node.
 * @param pNode The multiTree node to be added.
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(void) MultiTreeNodeAddPrev(
                            MultiTreeNode * pThis,MultiTreeNode *pNode);
/*!
 * @brief Add a multiTree node @a pChild and all of its younger siblings as the children 
 * of the current multiTree node @a pThis.
 * @param pThis  the current multiTree node.
 * @param pChild the multiTree node to be added.
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(void) MultiTreeNodeAddChildren(
                            MultiTreeNode * pThis,MultiTreeNode *pChild);
/*!
 * @brief Add a multiTree node @a pNode and all of its elder siblings as the siblings 
 * of the current multiTree node @a pThis.
 * @param pThis  pointer pointed to the current multiTree node.
 * @param pNode pointer pointed to the multiTree node to be added.
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(void) MultiTreeNodeAddSiblingsAfter(
                            MultiTreeNode * pThis,MultiTreeNode *pNode);
/*!
 * @brief Add a multiTree node @a pNode and all of its younger siblings as the siblings 
 * of the current multiTree node @a pThis.
 * @param pThis  pointer pointed to the current multiTree node.
 * @param pNode pointer pointed to the multiTree node to be added.
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(void) MultiTreeNodeAddSiblingsBefore(
                            MultiTreeNode * pThis,MultiTreeNode *pNode);
/*!
 * @brief remove the child node of the current node @a pThis and all of the siblings of this child
 * @a pChild ,the nodes number to remove must less than @a numToDetach.save the nodes number that
 *  has been removed in @a numDetached.
 * @param pThis the current node.
 * @param pChild the child node of the current node.
 * @param numToDetach the largest nodes number to remove.
 * @param numDetached the nodes number that has been removed .
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(MultiTreeNode*) MultiTreeNodeDetachChildAndSiblings(
                            MultiTreeNode * pThis,
                MultiTreeNode* pChild, int numToDetach, int* numDetached);
/*!
 * @brief remove the current node @a pThis and all of its siblings,the nodes number to remove 
 * must less than @a numToDetach.save the nodes number that has been removed in @a numDetached.
 * @param pThis the current node.
 * @param numToDetach the largest nodes number to remove.
 * @param numDetached the nodes number that has been removed .
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(MultiTreeNode*) MultiTreeNodeDetachSelfAndSiblings(
                            MultiTreeNode * pThis,
                            int numToDetach, int* numDetached);


#ifdef __cplusplus

CAR_INLINE void MultiTreeNode::AddChild(MultiTreeNode* pChild)
{
    ::MultiTreeNodeAddChild(this, pChild);
}

CAR_INLINE void MultiTreeNode::DetachChild(MultiTreeNode* pChild) // no recursion
{
    ::MultiTreeNodeDetachChild(this, pChild);
}

CAR_INLINE MultiTreeNode * MultiTreeNode::FirstChild()
{
    return MultiTreeNodeFirstChild(this);
}

CAR_INLINE void MultiTreeNode::AddNext(MultiTreeNode *pNode)
{
    ::MultiTreeNodeAddNext(this, pNode);
}

CAR_INLINE void MultiTreeNode::AddPrev(MultiTreeNode *pNode)
{
    ::MultiTreeNodeAddPrev(this, pNode);
}

CAR_INLINE void MultiTreeNode::AddChildren(MultiTreeNode *pChild)
{
    ::MultiTreeNodeAddChildren(this, pChild);
}

CAR_INLINE void MultiTreeNode::AddSiblingsAfter(MultiTreeNode *pNode)
{
    ::MultiTreeNodeAddSiblingsAfter(this, pNode);
}

CAR_INLINE void MultiTreeNode::AddSiblingsBefore(MultiTreeNode *pNode)
{
    ::MultiTreeNodeAddSiblingsBefore(this, pNode);
}

CAR_INLINE MultiTreeNode * MultiTreeNode::DetachChildAndSiblings(
                            MultiTreeNode* pChild,
                            int numToDetach,
                            int* numDetached)
{
    return MultiTreeNodeDetachChildAndSiblings(
                    this, pChild, numToDetach, numDetached);
}

CAR_INLINE MultiTreeNode * MultiTreeNode::DetachSelfAndSiblings(
                            int numToDetach, int* numDetached)
{
    return MultiTreeNodeDetachSelfAndSiblings(this, numToDetach, numDetached);
}

#endif //__cplusplus

#endif // __MULTITREENODE_H__
