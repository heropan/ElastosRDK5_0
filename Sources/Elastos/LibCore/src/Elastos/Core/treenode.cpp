//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <ElUtilities.h>

_ELASTOS_NAMESPACE_USING

// add child to my right
ELAPI_(void) TreeNodeAddRight(TreeNode* pThis, TreeNode* child)
{
    TreeNode * right = pThis->m_right;
    child->m_parent = pThis;
    child->m_left = NULL;
    child->m_right = right;
    pThis->m_right = child;
    if (right != NULL) right->m_parent = child;
}

// add child to my left
ELAPI_(void) TreeNodeAddLeft(TreeNode* pThis, TreeNode * child)
{
    TreeNode * left = pThis->m_left;
    child->m_parent = pThis;
    child->m_left = left;
    child->m_right = NULL;
    pThis->m_left = child;
    if (left != NULL) left->m_parent = child;
}

ELAPI_(TreeNode*) TreeNodeRemoveWithReplacement(
                                TreeNode* pThis, TreeNode* replaceNode)
{
    TreeNode * replacement;
    if (pThis->m_left == NULL) {               // no left child
        replacement = pThis->m_right;          // or no child at all
    }
    else if (pThis->m_right == NULL) {         // no right child
        replacement = pThis->m_left;
    }
    else {                              // has both children
        // replacement is the smallest node in the right sub-tree.
        //
        if (replaceNode != NULL) { // we trust the caller to do the right thing
            replacement = replaceNode;
        }
        else {
            replacement = pThis->m_right;
            while (replacement->m_left != NULL) {
                replacement = replacement->m_left;
            }
        }

        // replacement(R) must not have left child and it's the smallest
        // node in the right sub-tree of pThis node(N).
        //_______________________________________________
        //                                               |
        //    N          R           N           R       |
        //   / \        / \         / \         / \      |
        //  A   R      A   B       A   B       A   B     |
        //       \  =>                / \  =>     / \    |
        //        B                  R   C       D   C   |
        //                            \                  |
        //                             D                 |
        //_______________________________________________|
        //
        replacement->m_left = pThis->m_left;
        pThis->m_left->m_parent = replacement;

        if (replacement->m_parent != pThis) {
            replacement->m_parent->m_left = replacement->m_right;
            if (replacement->m_right != NULL) {
                replacement->m_right->m_parent = replacement->m_parent;
            }
            replacement->m_right = pThis->m_right;
            pThis->m_right->m_parent = replacement;
        }
    }

    if (pThis->m_parent != NULL) {
        if (pThis->m_parent->m_left == pThis) {
            pThis->m_parent->m_left = replacement;
        }
        else {
            pThis->m_parent->m_right = replacement;
        }
    }

    if (replacement != NULL) {
        replacement->m_parent = pThis->m_parent;
    }
    else {
        replacement = pThis->m_parent;
    }
    return replacement;
}

ELAPI_(Boolean) TreeNodeMoveCloserToRoot(TreeNode* pThis)
{
    if (pThis->m_parent == NULL) {          // the node is the root, do nothing
        return TRUE;
    }

    TreeNode * grandparent = pThis->m_parent->m_parent;
    pThis->m_parent->m_parent = pThis;          // splay asserts pThis operation
    TreeNode * newchild;

    if (grandparent == NULL) {          // pThis is a degenerated case
        //
        // parent(P) is the root, node(N) can be either left or right child
        //_____________________________________________
        //                                             |
        //     P        N          P            N      |
        //    / \      / \        / \          / \     |
        //   N   C => A   P      A   N   =>   P   C    |
        //  / \          / \        / \      / \       |
        // A   B        B   C      B   C    A   B      |
        //_____________________________________________|
        //
        if (pThis->m_parent->m_left == pThis) { // pThis node is left child
            newchild = pThis->m_right;
            pThis->m_right = pThis->m_parent;
            pThis->m_parent->m_left = newchild;
        }
        else {                          // this node is right child
            newchild = pThis->m_left;
            pThis->m_left = pThis->m_parent;
            pThis->m_parent->m_right = newchild;
        }

        if (newchild != NULL) {
            newchild->m_parent = pThis->m_parent;
        }
        pThis->m_parent = NULL;
        return TRUE;                    // done
    }

    // splay operation between grandparent(G) and this node(N)
    //
    TreeNode * greatgrandparent = grandparent->m_parent;
    TreeNode * old_parent = pThis->m_parent;
    pThis->m_parent = greatgrandparent;

    if (greatgrandparent != NULL) {     // node replaces grandparent
        if (greatgrandparent->m_left == grandparent) {
            greatgrandparent->m_left = pThis;
        }
        else {
            greatgrandparent->m_right = pThis;
        }
    }

    TreeNode * newgrandchild;
    if (old_parent->m_left == pThis) {
        //___________________________________________________________
        //                                                           |
        //       G          N                G               N       |
        //      / \        / \              / \            /   \     |
        //     P   D      A   P            A   P          G     P    |
        //    / \     =>     / \              / \   =>   / \   / \   |
        //   N   C          B   G            N   D      A   B C   D  |
        //  / \                / \          / \                      |
        // A   B              C   D        B   C                     |
        //___________________________________________________________|
        //
        newchild = pThis->m_right;
        old_parent->m_left = newchild;
        pThis->m_right = old_parent;

        if (grandparent->m_left == old_parent) {
            newgrandchild = old_parent->m_right;
            grandparent->m_parent = old_parent;
            grandparent->m_left = newgrandchild;
            old_parent->m_right = grandparent;
        }
        else {
            newgrandchild = pThis->m_left;
            grandparent->m_parent = pThis;
            grandparent->m_right = newgrandchild;
            pThis->m_left = grandparent;
        }
    }
    else {
        //__________________________________________________________
        //                                                          |
        //   G                N               G            N        |
        //  / \              / \             / \         /   \      |
        // A   P            P   D           P   D       P     G     |
        //    / \    =>    / \             / \     =>  / \   / \    |
        //   B   N        G   C           A   N       A   B C   D   |
        //      / \      / \                 / \                    |
        //     C   D    A   B               B   C                   |
        //__________________________________________________________|
        //
        newchild = pThis->m_left;
        old_parent->m_right = newchild;
        pThis->m_left = old_parent;

        if (grandparent->m_right == old_parent) {
            newgrandchild = old_parent->m_left;
            grandparent->m_parent = old_parent;
            grandparent->m_right = newgrandchild;
            old_parent->m_left = grandparent;
        }
        else {
            newgrandchild = pThis->m_right;
            grandparent->m_parent = pThis;
            grandparent->m_left = newgrandchild;
            pThis->m_right = grandparent;
        }
    }

    if (newchild != NULL) {
        newchild->m_parent = old_parent;
    }
    if (newgrandchild != NULL) {
        newgrandchild->m_parent = grandparent;
    }

    return (greatgrandparent == NULL);
}

