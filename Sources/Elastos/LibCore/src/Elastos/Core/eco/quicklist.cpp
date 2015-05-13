//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <ElUtilities.h>

_ELASTOS_NAMESPACE_USING

// a big hack to fool C++ comipler; we hate friends :-)
//
class _TreeNode : public TreeNode {
public:
    TreeNode* RemoveWithReplacement(TreeNode *replacement) {
        return TreeNode::RemoveWithReplacement(replacement);
    }
};

ELAPI_(QuickListNode*) QuickListNodeDetach(QuickListNode*pThis)
{
    // first, remove this node from double link list
    //
    ListNode* aListNode = ListNodeDetach(&pThis->m_list);
    if (aListNode == NULL) {
        return NULL;    // tree is now empty
    }

    // based on a list node pointer, locate the base address of the ListTreeNode
    //
    QuickListNode* aListTreeNode;
    aListTreeNode = (QuickListNode *)((Address)aListNode - LISTNODE_OFFSET);

    // the whole if() clause below is not necessary, logically speaking.
    // it's reason for being here is such that this function takes const time,
    // which is useful when implementing real time OS algorithms.
    //
    if (pThis->m_tree.m_left != NULL && pThis->m_tree.m_right != NULL) {
        // aListNode must be the smallest node in the right sub-tree,
        // which does not have left child because it's the smallest.
        //
        assert(aListTreeNode->m_tree.m_left == NULL);

        aListTreeNode = (QuickListNode *)TreeNodeRemoveWithReplacement(
                                    &pThis->m_tree,(TreeNode*)aListTreeNode);
    }
    else {
        aListTreeNode = (QuickListNode *)pThis->m_tree.Detach();
    }

    return aListTreeNode;
}
