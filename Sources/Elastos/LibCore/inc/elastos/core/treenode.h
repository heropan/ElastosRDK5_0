//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#if _MSC_VER > 1000
#pragma once
#endif

#if !defined(__TREENODE_H__)
#define __TREENODE_H__

#ifdef __cplusplus
/*!
 @brief 
 @since
 @sa
 */
class TreeNode {
public:
/*!
 @brief Add a right child @a child for the current tree node.
 @param child <b>[in]</b> the child node to be added.
 @since
 @sa
 */
    void AddRight(TreeNode* child);
/*!
 @brief Add a left child @a child for the current tree node.
 @param child <b>[in]</b> the child node to be added.
 @since
 @sa
 */
    void AddLeft(TreeNode* child); 
/*!
 @brief remove the current tree node from the tree.returns null if
 tree become empty,or a replacement node.if the current node is the 
 root,return the new root.
 @return
 @since
 @sa
 */
    TreeNode* Detach();     
/*!
 @brief remove the closer tree node from the tree to the root.returns 
 true if the current node is turning into root.
 @return
 @since
 @sa
 */
    _ELASTOS Boolean MoveCloserToRoot();                        
/*!
 @brief return the parent node of the current tree node.
 @return
 @since
 @sa
 */
    TreeNode * Parent()     { return (TreeNode *) m_parent; }
/*!
 @brief return the left child node of the current tree node.
 @return
 @since
 @sa
 */	
    TreeNode * Left()       { return (TreeNode *) m_left; }
/*!
 @brief return the right child node of the current tree node.
 @return
 @since
 @sa
 */	
    TreeNode * Right()      { return (TreeNode *) m_right; }
/*!
 @brief Initialise this tree,make its parent node ,left child node and right child
 are null.
 @since
 @sa
 */	
    void Init() { m_parent = m_left = m_right = NULL; }
/*!
 @brief creates a null TreeNode instance. its parent node ,left child node and right 
 child are null.
 */	
    TreeNode() : m_parent(NULL), m_left(NULL), m_right(NULL) { }
/*!
 @brief the destructor of TreeNode.
 */
    ~TreeNode() { }

public:
/*!
 @brief parent node.
 */
    TreeNode* m_parent;    
/*!
 @brief children that are less than the current node.
 */
    TreeNode* m_left;       
/*!
 @brief children that are bigger than the current node.
 */	
    TreeNode* m_right;      

public://protected:
/*!
 @brief Replace the current node with the replacement @a replacement.
 
 if the current node has no left child,the the replacement is its right
 child;if the current node has no right child,the the replacement is 
 its left child;or its replacement is the node @a replaceNode;if the node
 @a replaceNode is null,then use the right child node of the current node
 as its replacement.
 @param replaceNode <b>[in]</b> pointer pointed to the replacement
 @return
 @since
 @sa
 */
    TreeNode* RemoveWithReplacement(TreeNode *replacement);
                           
};

#else //!__cplusplus

typedef struct TreeNode TreeNode;

struct TreeNode {
    TreeNode* m_parent;     /*!< parent node, if any */
    TreeNode* m_left;       /*!< children that are less than the current tree node. */
    TreeNode* m_right;      /*!< children that are bigger than the current tree node. */
};

#endif //__cplusplus


/*!
 @brief Add tree node @a child as the right child of the tree node @a pThis.
 @param pThis pointer pointed to the current node.
 @param child pointer pointed to the node to be added as right child.
 @return
 @since
 @sa
 @relates
 */
ELAPI_(void) TreeNodeAddRight(TreeNode*pThis,TreeNode* child);

/*!
 @brief Add tree node @a child as the left child of the tree node @a pThis.
 @param pThis pointer pointed to the current node.
 @param child pointer pointed to the node to be added as left child.
 @return
 @since
 @sa
 @relates
 */
ELAPI_(void) TreeNodeAddLeft(TreeNode*pThis,TreeNode * child);
/*!
 @brief Replace the current node @a pThis with the replacement @a replaceNode.
 
 if the current node @a pThis has no left child,the the replacement is its right
 child;if the current node @a pThis has no right child,the the replacement is 
 its left child;or its replacement is the node @a replaceNode;if the node @a replaceNode
 is null,then use the right child node of the current node @a pThis as its replacement.
 @param pThis pointer pointed to the current node.
 @param replaceNode pointer pointed to the replacement.
 @return
 @since
 @sa
 @relates
 */
ELAPI_(TreeNode*) TreeNodeRemoveWithReplacement(
                                        TreeNode*pThis,TreeNode* replaceNode);
/*!
 @brief move a tree node to make it be more closer to the root tree node @a pThis.
 @param pThis pointer pointed to the current node.
 @return
 @since
 @sa
 @relates
 */
ELAPI_(_ELASTOS Boolean) TreeNodeMoveCloserToRoot(TreeNode*pThis);


#ifdef __cplusplus

CAR_INLINE void TreeNode::AddRight(TreeNode* child)
{
	TreeNodeAddRight(this, child);
}
CAR_INLINE void TreeNode::AddLeft(TreeNode * child)
{
    TreeNodeAddLeft(this,child);
}
CAR_INLINE TreeNode* TreeNode::RemoveWithReplacement(TreeNode* replaceNode)
{
    return TreeNodeRemoveWithReplacement(this,replaceNode);
}
CAR_INLINE _ELASTOS Boolean TreeNode::MoveCloserToRoot(void)
{
    return TreeNodeMoveCloserToRoot(this);
}

CAR_INLINE TreeNode* TreeNode::Detach()
{
    return RemoveWithReplacement(NULL);
}

#endif //__cplusplus

#endif // __TREENODE_H__
