//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <ElUtilities.h>


// add a node after me
ELAPI_(ListNode*) ListNodeAddNext(ListNode* pThis, ListNode* node)
{
    ListNode * next = pThis->m_next;
    node->m_prev = pThis;
    node->m_next = next;
    pThis->m_next = node;
    next->m_prev = node;
    return node;
}

// add a node before me
ELAPI_(ListNode*) ListNodeAddPrev(ListNode* pThis, ListNode* node)
{
    ListNode * prev = pThis->m_prev;
    node->m_prev = prev;
    node->m_next = pThis;
    pThis->m_prev = node;
    prev->m_next = node;
    return node;
}

// remove myself from list.
ELAPI_(ListNode*) ListNodeDetach(ListNode* pThis)
{
    ListNode * next = pThis->m_next;
    ListNode * prev = pThis->m_prev;

    if (next == pThis) {                     // return NULL if list turns empty
        assert(prev == next);
        return NULL;
    }

    // make the removed node a circular single node list before returning.
    // it's safer this way.
    //
    pThis->m_next = pThis->m_prev = pThis;

    prev->m_next = next;                    // bypass the removed node in list
    next->m_prev = prev;
    return next;                            // return a replacement node
}


// add a link list after me.  this function is similar to AddNext()
// where you could only add one node, but at high cost.
//
ELAPI_(ListNode*) ListNodeAddListAfter(
                                    ListNode* pThis, ListNode* node)
{
    ListNode * pNextAfterThis = pThis->m_next;   // next node of this node
    ListNode * pLastOflink = node->m_prev;      // the last node of added link

    // must be a double circle link
    //
    assert(node->m_next->m_prev == node);
    assert(node->m_prev->m_next == node);

    node->m_prev = pThis;
    pLastOflink->m_next = pNextAfterThis;
    pThis->m_next = node;
    pNextAfterThis->m_prev = pLastOflink;
    return node;
}

// add a link list before me.  this function is similar to AddPrev()
// where you could only add one node, but at high cost.
//
ELAPI_(ListNode*) ListNodeAddListBefore(
                                    ListNode* pThis, ListNode* node)
{
    ListNode * pPrevBeforeThis = pThis->m_prev;  // next node of this node
    ListNode * pLastOflink = node->m_prev;      // the last node of added link

    // must be a double circle link
    //
    assert(node->m_next->m_prev == node);
    assert(node->m_prev->m_next == node);

    node->m_prev = pPrevBeforeThis;
    pLastOflink->m_next = pThis;
    pThis->m_prev = pLastOflink;
    pPrevBeforeThis->m_next = node;
    return node;
}


// detach myself to the node pointed by "node".
// returns NULL if list becomes empty, or a replacement node.
// note : myself and replacement node in different list.
//
ELAPI_(ListNode*) ListNodeDetachSublist(
                                    ListNode* pThis, ListNode *node)
{
    ListNode * pPrevBeforeThis = pThis->m_prev;
    ListNode * pReplacement = node->m_next;

    // similar to Detach()
    //
    if(pThis == node) {
        return ListNodeDetach(pThis);
    }

    if(pReplacement == pThis) {
        return NULL;
    }

    // make the removed list a circular list before returning.
    // it's safer this way.
    //
    pPrevBeforeThis->m_next = node->m_next;
    pReplacement->m_prev = pPrevBeforeThis;

    pThis->m_prev = node;
    node->m_next = pThis;

    return pReplacement;        // return a replacement node
}

