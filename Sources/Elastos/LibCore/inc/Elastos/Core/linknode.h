//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include <elatypes.h>
#include <assert.h>

#ifndef __LINKNODE_H__
#define __LINKNODE_H__

#define INVALID_ADDRVALUE           ((virtaddr_t)(0xcccccccc))

#ifndef ForEachDLinkNode

#define ForEachDLinkNode(t, p, h) \
        for (p = (t)((h)->mNext); p != (t)h; p = (t)(p->mNext))

#define FOR_EACH_DLINKNODE(t, p, h) \
        ForEachDLinkNode(t, p, h)

#define ForEachDLinkNodeReversely(t, p, h) \
        for (p = (t)((h)->mPrev); p != (t)h; p = (t)(p->mPrev))

#define FOR_EACH_DLINKNODE_REVERSELY(t, p, h) \
        ForEachDLinkNodeReversely(t, p, h)

#endif // ForEachDLinkNode

#ifndef ForEachSLinkNode

#define ForEachSLinkNode(t, p, h) \
        for (p = (t)((h)->mNext); p != (t)h; p = (t)(p->mNext))

#define FOR_EACH_SLINKNODE(t, p, h) \
        ForEachSLinkNode(t, p, h)
        
#endif // ForEachSLinkNode        

/*!
 * @brief Double Link Node -- For double link circular list with head-node.
 * @since
 * @sa
 */
class DoubleLinkNode
{
public:
/*!
 * @brief the constructor of the class @a DoubleLinkNode.
 */
    DoubleLinkNode()
    {
        mPrev = mNext = NULL;
    }
/*!
 * @brief Initializes the current instance 's previous node equals to the next node.
 * @since
 * @sa
 */   
    void Initialize()
    {
        mPrev = mNext = this;
    }

/*!
 * @brief  Return the previous node.
 * @return
 * @since
 * @sa
 */
    DoubleLinkNode* Prev() const;
/*!
 * @brief  Return the next node.
 * @return
 * @since
 * @sa
 */
    DoubleLinkNode* Next() const;
/*!
 * @brief  Return the first node of list.
 * @return
 * @since
 * @sa
 */    
    DoubleLinkNode* First() const;
/*!
 * @brief  Return the last node of list.
 * @return
 * @since
 * @sa
 */
    DoubleLinkNode* Last() const;
/*!
 * @brief Insert a new node before current node. Return the new previous node.
 * @param prevNode <b>[out]</b> points to the node to be inserted as a previous node of the
 * current node.
 * @return
 * @since
 * @sa
 */
    DoubleLinkNode* InsertPrev(DoubleLinkNode *prevNode);
/*!
 * @brief Insert a new node after current node.  Return the new next node.
 * @param nextNode <b>[out]</b> points to the node to be inserted as a next node of the current
 * node.
 * @return
 * @since
 * @sa
 */
    DoubleLinkNode* InsertNext(DoubleLinkNode *nextNode);
/*!
 * @brief  Insert a new node as the first of list. Return the new first node.
 * @param firstNode <b>[out]</b> points to the node to be inserted as the first node of the list.
 * @return
 * @since
 * @sa
 */   
    DoubleLinkNode* InsertFirst(DoubleLinkNode *firstNode);
/*!
 * @brief  Insert a new node as the last of list.  Return the new last node.
 * @param lastNode <b>[out]</b> points to the node to be inserted as the last node of the list.
 * @return
 * @since
 * @sa
 */        
    DoubleLinkNode* InsertLast(DoubleLinkNode *lastNode);
/*!
 * @brief  Remove current node from list. Return current node.
 * @return
 * @since
 * @sa
 */     
    DoubleLinkNode* Detach();
/*!
 * @brief  If current node is NULL then returns true;false otherwise.
 * @return
 * @since
 * @sa
 */       
    _ELASTOS Boolean IsEmpty() const;

public:
/*!
 *@brief the node just before the current node.
 */
    DoubleLinkNode      *mPrev;
/*!
 *@brief the node just after the current node.
 */
    DoubleLinkNode      *mNext;
};

typedef DoubleLinkNode DLinkNode;
/*!
 * @brief insert a link node @a prevNode into the double link before the current node 
 * @a currNode.
 * @param currNode  pointer pointed to the current node.
 * @param prevNode  pointer pointed to the node to be inserted. 
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(DoubleLinkNode*) DoubleLinkNodeInsertPrev(
    DoubleLinkNode *currNode, DoubleLinkNode *prevNode);
/*!
 * @brief insert a link node @a nextNode into the double link after the current node 
 * @a currNode.
 * @param currNode  pointer pointed to the current node.
 * @param nextNode  pointer pointed to the node to be inserted. 
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(DoubleLinkNode*) DoubleLinkNodeInsertNext(
    DoubleLinkNode *currNode, DoubleLinkNode *nextNode);
/*!
 * @brief remove the current node @a currNode from the double link.
 * @param currNode  pointer pointed to the current node.
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(DoubleLinkNode*) DoubleLinkNodeDetach(
    DoubleLinkNode *currNode); 

CAR_INLINE DoubleLinkNode* DoubleLinkNode::Prev() const
{
    return mPrev;
}

CAR_INLINE DoubleLinkNode* DoubleLinkNode::Next() const
{
    return mNext;
}

CAR_INLINE DoubleLinkNode* DoubleLinkNode::First() const
{ 
    return Next(); 
}

CAR_INLINE DoubleLinkNode* DoubleLinkNode::Last() const
{ 
    return Prev(); 
}

CAR_INLINE DoubleLinkNode* DoubleLinkNode::InsertPrev(DoubleLinkNode *prevNode)
{
    return DoubleLinkNodeInsertPrev(this, prevNode);
}

CAR_INLINE DoubleLinkNode* DoubleLinkNode::InsertNext(DoubleLinkNode *nextNode)
{
    return DoubleLinkNodeInsertNext(this, nextNode);
}

CAR_INLINE DoubleLinkNode* DoubleLinkNode::InsertFirst(DoubleLinkNode *firstNode)
{
    return InsertNext(firstNode);
}

CAR_INLINE DoubleLinkNode* DoubleLinkNode::InsertLast(DoubleLinkNode *lastNode)
{
    return InsertPrev(lastNode);
}

CAR_INLINE DoubleLinkNode *DoubleLinkNode::Detach()
{
    return DoubleLinkNodeDetach(this);
}

CAR_INLINE _ELASTOS Boolean DoubleLinkNode::IsEmpty() const
{ 
    return this == mNext; 
}

/*!
 * @brief Single Link Node -- For single link circular list with head-node.
 */
class SingleLinkNode
{
public:
/*!
 * @brief constructs a SingleLinkNode instance without link node.
 */
    SingleLinkNode()
    {
        mNext = NULL;
    }
/*!
 * @brief initializes a SingleLinkNode instance with one link node only.
 * @since
 * @sa
 */
    void Initialize() 
    { 
        mNext = this; 
    }
/*!
 * @brief Return the next node.
 * @return
 * @since
 * @sa
 */
    SingleLinkNode *Next();
/*!
 * @brief Insert a new node after the current node.  Return the new next node.
 * @param nextNode <b>[out]</b>  the node to be inserted.
 * @return
 * @since
 * @sa
 */
    SingleLinkNode *InsertNext(SingleLinkNode *nextNode);
/*!
 * @brief Remove the current node from list. Return the current node.
 * @param prevNode <b>[out]</b> the node to be removed.
 * @return
 * @since
 * @sa
 */
    SingleLinkNode *Detach(SingleLinkNode *prevNode);
/*!
 * @brief judge the current node is empty or not.if empty returns true;
 * otherwise false.
 * @return
 * @since
 * @sa
 */    
    _ELASTOS Boolean IsEmpty() const;

public:
/*!
 * @brief the node next to the current node.
 */
    SingleLinkNode      *mNext;
};

typedef SingleLinkNode SLinkNode;
/*!
 * @brief Insert a new node @a nextNode after the current node@a currNode.
 * Return the new next node.
 * @param currNode the current node.
 * @param nextNode the node to be inserted.
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(SingleLinkNode*) SingleLinkNodeInsertNext(
    SingleLinkNode *currNode, SingleLinkNode *nextNode);
/*!
 * @brief Insert a new node @a prevNode before the current node@a currNode.
 * Return the new previous node.
 * @param currNode the current node.
 * @param prevNode the node to be inserted.
 * @return
 * @since
 * @sa
 * @relates
 */
ELAPI_(SingleLinkNode*) SingleLinkNodeDetach(
    SingleLinkNode *currNode, SingleLinkNode *prevNode);    

CAR_INLINE SingleLinkNode *SingleLinkNode::Next()
{
    return mNext;
}

CAR_INLINE SingleLinkNode *SingleLinkNode::InsertNext(SingleLinkNode *nextNode)
{
    return SingleLinkNodeInsertNext(this, nextNode);
}

CAR_INLINE SingleLinkNode *SingleLinkNode::Detach(SingleLinkNode *prevNode)
{
   return SingleLinkNodeDetach(this, prevNode);
}

CAR_INLINE _ELASTOS Boolean SingleLinkNode::IsEmpty() const
{   
    return this == mNext; 
}

#endif //__LINKNODE_H__