//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <ElUtilities.h>

_ELASTOS_NAMESPACE_USING

#define LIST_OFFSET offsetof(MultiTreeNode, m_siblings)

ELAPI_(void) MultiTreeNodeAddChild(
                            MultiTreeNode * pThis,MultiTreeNode* pChild)
{
    assert(pChild != NULL);

    if (pThis->m_pSubtree == NULL) {           // first child?

        ListNodeInit(&pChild->m_siblings);      // make sure the list is valid
        pThis->m_pSubtree = pChild;
    }
    else {
        // add to last
        ListNodeAddPrev( &pThis->m_pSubtree->m_siblings,&pChild->m_siblings);
    }

    pChild->m_pParent = pThis;
}

ELAPI_(void) MultiTreeNodeDetachChild(
                            MultiTreeNode * pThis,MultiTreeNode* pChild)
{
    assert(pChild != NULL);

    ListNode * pReplaceNode = ListNodeDetach(&pChild->m_siblings);
    pChild->m_pParent = NULL;

    if (pThis->m_pSubtree == pChild) {         // removing the first child?
        if (pReplaceNode == NULL) {     // last child is gone?
            pThis->m_pSubtree = NULL;
        }
        else {
            pThis->m_pSubtree =
                        (MultiTreeNode*)((Address)pReplaceNode - LIST_OFFSET);
        }
    }
}

ELAPI_(MultiTreeNode*) MultiTreeNodeFirstChild(MultiTreeNode * pThis)
{
    return pThis->m_pSubtree;                  // same as Child() for now
}

ELAPI_(void) MultiTreeNodeAddNext(
                            MultiTreeNode * pThis,MultiTreeNode *pNode)
{
    assert(pNode);
    ListNodeAddNext(&pThis->m_siblings,(ListNode *)pNode);
    pNode->m_pParent = pThis->m_pParent;
}

ELAPI_(void) MultiTreeNodeAddPrev(
                            MultiTreeNode* pThis,MultiTreeNode *pNode)
{
    assert(pNode);
    ListNodeAddPrev(&pThis->m_siblings,(ListNode *)pNode);

    if (pThis->m_pParent->m_pSubtree == pThis) {  // am I the eldest son?
        pThis->m_pParent->m_pSubtree = pNode;    // new eldest son
    }
    pNode->m_pParent = pThis->m_pParent;
}


// If the child has no sibling, same as AddChild(), but high cost.
//
ELAPI_(void) MultiTreeNodeAddChildren(
                            MultiTreeNode* pThis,MultiTreeNode* pChild)
{
    MultiTreeNode *pYoungest;

    assert(pChild != NULL);

    if (pThis->m_pSubtree == NULL) {           // first child?
        pThis->m_pSubtree = pChild;
    }
    else {
        // add to last
        ListNodeAddListBefore(&pThis->m_pSubtree->m_siblings,
                              &pChild->m_siblings);
    }

    // set all children's parent
    //
    pChild->m_pParent = pThis;

    // we call ListNode_Prev() instead of _MultiTreeNodePrev;
    //
    pYoungest = (MultiTreeNode *)pChild->m_siblings.m_prev;

    while(pYoungest != pChild) {
        pYoungest->m_pParent = pThis;
        pYoungest = (MultiTreeNode *)pYoungest->m_siblings.m_prev;
    }
}


// Add younger brothers
//
ELAPI_(void) MultiTreeNodeAddSiblingsAfter(
                            MultiTreeNode * pThis,MultiTreeNode *pNode)
{
    MultiTreeNode *pNext;

    assert(pNode);

    pNode->m_pParent = pThis->m_pParent;

    for (pNext = (MultiTreeNode *)pNode->m_siblings.m_next;
         pNext != pNode; pNext = (MultiTreeNode *)pNext->m_siblings.m_next) {

        assert(pNext);
        pNext->m_pParent = pThis->m_pParent;
    }

    ListNodeAddListAfter(&pThis->m_siblings,(ListNode *)pNode);
}


// Add elder brothers
//
ELAPI_(void) MultiTreeNodeAddSiblingsBefore(
                            MultiTreeNode * pThis,MultiTreeNode *pNode)
{
    MultiTreeNode *pNext;

    assert(pNode);

    pNode->m_pParent = pThis->m_pParent;

    for (pNext = (MultiTreeNode *)pNode->m_siblings.m_next;
         pNext != pNode; pNext = (MultiTreeNode *)pNext->m_siblings.m_next) {

        assert(pNext);
        pNext->m_pParent = pThis->m_pParent;
    }

    ListNodeAddListBefore(&pThis->m_siblings,(ListNode *)pNode);

    if (pThis->m_pParent->m_pSubtree == pThis) {  // am I the eldest son?
        pThis->m_pParent->m_pSubtree = pNode;    // new eldest son
    }
}

// returns the replacement node, NULL if the subtree is NULL
//
ELAPI_(MultiTreeNode*) MultiTreeNodeDetachChildAndSiblings(
                                        MultiTreeNode * pThis,
                                        MultiTreeNode* pChild,
                                        int numToDetach,
                                        int* numDetached)
{
    int DetachCount;
    MultiTreeNode *pReplacement, *pNext;

    assert(pChild);

    pNext = (MultiTreeNode *)pChild->m_siblings.m_next;

    for (DetachCount = 0; DetachCount < numToDetach; DetachCount++) {
        if (pNext == pThis->m_pSubtree) break;
        pNext = (MultiTreeNode *)pNext->m_siblings.m_next;
    }

    pReplacement = (MultiTreeNode *)ListNodeDetachSublist(&pChild->m_siblings,
            pNext->m_siblings.m_prev);

    if (pThis->m_pSubtree == pChild ) {   // I'am the eldest son
        pThis->m_pSubtree = pReplacement;
    }

    pChild->m_pParent = NULL;

    for (pNext = (MultiTreeNode *)pChild->m_siblings.m_next;
        pNext != pChild; pNext = (MultiTreeNode *)pNext->m_siblings.m_next) {
        pNext->m_pParent = NULL;
    }

    *numDetached = DetachCount + 1;
    return pReplacement;
}


// returns the replacement node, NULL if the sibling-list is NULL
//
ELAPI_(MultiTreeNode*) MultiTreeNodeDetachSelfAndSiblings(
                                        MultiTreeNode * pThis,
                                        int numToDetach,
                                        int* numDetached)
{
    int DetachCount;
    MultiTreeNode *pReplacement, *pNext, *pEldest;

    if (pThis->m_pParent)
        pEldest = pThis->m_pParent->m_pSubtree;
    else
        pEldest = pThis;

    pNext = (MultiTreeNode *)pThis->m_siblings.m_next;

    for (DetachCount = 0; DetachCount < numToDetach; DetachCount++) {
        if (pNext == pEldest) break;
        pNext = (MultiTreeNode *)pNext->m_siblings.m_next;
    }

    pReplacement = (MultiTreeNode *)ListNodeDetachSublist(&pThis->m_siblings,
            pNext->m_siblings.m_prev);

    if (pThis->m_pParent) {
        if (pThis->m_pParent->m_pSubtree == pThis)  // I'm the eldest son
            pThis->m_pParent->m_pSubtree = pReplacement;
    }

    pThis->m_pParent = NULL;

    for (pNext = (MultiTreeNode *)pThis->m_siblings.m_next;
        pNext != pThis; pNext = (MultiTreeNode *)pNext->m_siblings.m_next) {
        pNext->m_pParent = NULL;
    }

    *numDetached = DetachCount + 1;
    return pReplacement;
}
