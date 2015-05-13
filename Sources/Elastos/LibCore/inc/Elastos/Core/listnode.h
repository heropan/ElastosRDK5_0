//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#if _MSC_VER > 1000
#pragma once
#endif

#if !defined(__LISTNODE_H__)
#define __LISTNODE_H__

#ifdef __cplusplus
/*!
 * @brief
 * @since
 * @sa
 */
class ListNode {
public:
/*!
 * @brief insert a list node @a node just after the current list node.
 * @param node <b>[in]</b>  pointer pointed to the node to be inserted into the list.
 * @return
 * @since
 * @sa
 */
    ListNode* AddNext(ListNode* node);
/*!
 * @brief insert a list node @a node just brfore the current list node.
 * @param node <b>[in]</b> pointer pointed to the node to be inserted into the list.
 * @return
 * @since
 * @sa
 */
    ListNode* AddPrev(ListNode* node);
/*!
 * @brief delete the current list node from the list.returns NULL if list
 * becomes empty,or a replacement node.
 * @return
 * @since
 * @sa
 */
    ListNode* Detach();
/*!
 * @brief return the previous list node of the current list node in the list.
 * @return
 * @since
 * @sa
 */
    ListNode* Prev();
/*!
 * @brief return the next list node of the current list node in the list.
 * @return
 * @since
 * @sa
 */
    ListNode* Next();
/*!
 * @brief delete all the list nodes after the current list node.
 * @since
 * @sa
 */
    void ReclaimNext()  { m_next = NULL; }
/*!
 * @brief Initilize a list with one list node only.
 * @since
 * @sa
 */
    void Init() { m_prev = m_next = this; }
/*!
 * @brief insert another list starting by list node @a node after the current
 * list node.return @a node after inserted.
 * @param node <b>[in]</b> pointer pointed to the first node of the list to be inserted.
 * @return
 * @since
 * @sa
 */
    ListNode* AddListAfter(ListNode* node);
/*!
 * @brief insert another list starting by list node @a node before the current
 * list node.return @a node after inserted.
 * @param node <b>[in]</b> pointer pointed to the first node of the list to be inserted.
 * @return
 * @since
 * @sa
 */
    ListNode* AddListBefore(ListNode* node);
/*!
 * @brief Remove a sublist whose list node is between the current node and @a node.
 * if the current list node is equal to @a node,then remove the current list node
 * only;if @a node 's next node is the current list node,then remove nothing.
 * @param node <b>[in]</b> pointer pointed to the terminal node of the sublist to be remove.
 * @return
 * @since
 * @sa
 */
    ListNode* DetachSublist(ListNode* node);
/*!
 * @brief the constructor of listNode.
 */
    ListNode()          { m_prev = m_next = this; }
/*!
 * @brief the destructor of listNode.
 */
    ~ListNode()         { }

public:
/*!
 * @brief previous node in a sorted link list.
 */
    ListNode* m_prev;
/*!
 * @brief next node in a sorted link list.
 */
    ListNode* m_next;
};

#else //!__cplusplus

typedef struct ListNode ListNode;

struct ListNode {
    ListNode *m_prev; /*!<previous node in a sorted link list. */
    ListNode *m_next; /*!<next node in a sorted link list. */
};
#endif //__cplusplus

/*!
 * @brief Insert a list node @a node into the list after the current list node @a pThis.
 * return @a node.
 * @param pThis pointer pointed to the current list node.
 * @param node pointer pointed to the list node to be inserted into the list.
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(ListNode*) ListNodeAddNext(ListNode* pThis, ListNode* node);
/*!
 * @brief Insert a list node @a node into the list before the current list node @a pThis.
 * return @a node.
 * @param pThis pointer pointed to the current list node.
 * @param node pointer pointed to the list node to be inserted into the list.
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(ListNode*) ListNodeAddPrev(ListNode* pThis, ListNode* node);
/*!
 * @brief remove the current list node @a pThis from the list.return NULL if the list
 * turns empty;otherwise bypass the removed node in list and return a replacement node .
 * @param pThis pointer pointed to the current list node.
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(ListNode*) ListNodeDetach(ListNode* pThis);
/*!
 * @brief insert another list starting by list node @a node after the current list node
 * @a pThis in the list.return @a node after inserted.
 * @param pThis pointer pointed to the current list node.
 * @param node pointer pointed to the first node of the list to be inserted.
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(ListNode*) ListNodeAddListAfter(
                                        ListNode* pThis, ListNode* node);
/*!
 * @brief insert another list starting by list node @a node before the current list node
 * @a pThis in the list.return @a node after inserted.
 * @param pThis pointer pointed to the current list node.
 * @param node pointer pointed to the first node of the list to be inserted.
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(ListNode*) ListNodeAddListBefore(
                                      ListNode* pThis, ListNode* node);
/*!
 * @brief Remove a sublist whose list node is between the current node @a pThis and
 * @a node.if the current list node @a pThis is equal to @a node,then remove the node
 * @a pThis only;if @a node 's next node is @a pThis,then remove nothing.
 * @param pThis pointer pointed to the current list node.
 * @param node pointer pointed to the terminal node of the sublist to be removed.
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(ListNode*) ListNodeDetachSublist(
                                        ListNode* pThis, ListNode *node);

CAR_INLINE void __stdcall ListNodeInit(ListNode* pThis)
{
    pThis->m_prev = pThis->m_next = pThis;
}

#ifdef __cplusplus

CAR_INLINE ListNode* ListNode::AddNext(ListNode* node)
{
    return ListNodeAddNext(this, node);
}

CAR_INLINE ListNode* ListNode::AddPrev(ListNode* node)
{
    return ListNodeAddPrev(this, node);
}

CAR_INLINE ListNode* ListNode::Detach(void)
{
    return ListNodeDetach(this);
}

CAR_INLINE ListNode * ListNode::AddListAfter(ListNode* node)
{
    return ListNodeAddListAfter(this, node);
}

CAR_INLINE ListNode * ListNode::AddListBefore(ListNode* node)
{
    return ListNodeAddListBefore(this, node);
}

CAR_INLINE ListNode* ListNode::DetachSublist(ListNode *node)
{
    return ListNodeDetachSublist(this, node);
}

CAR_INLINE ListNode* ListNode::Prev()
{
    return m_prev;
}

CAR_INLINE ListNode* ListNode::Next()
{
    return m_next;
}

#endif //__cplusplus


#endif // __LISTNODE_H__
