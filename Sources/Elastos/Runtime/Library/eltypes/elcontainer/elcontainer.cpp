
#include <elcontainer.h>

_ELASTOS_NAMESPACE_BEGIN

//-----------------------------------------------------
// DoubleLinkNode
//-----------------------------------------------------
DoubleLinkNode *DoubleLinkNode::InsertPrev(DoubleLinkNode *prevNode)
{
    assert(prevNode);

    prevNode->mPrev = mPrev;
    prevNode->mNext = this;
    mPrev->mNext = prevNode;
    mPrev = prevNode;

    return prevNode;
}

DoubleLinkNode *DoubleLinkNode::InsertNext(DoubleLinkNode *nextNode)
{
    assert(nextNode);

    nextNode->mPrev = this;
    nextNode->mNext = mNext;
    mNext->mPrev = nextNode;
    mNext = nextNode;

    return nextNode;
}

DoubleLinkNode *DoubleLinkNode::Detach()
{
    mPrev->mNext = mNext;
    mNext->mPrev = mPrev;
    mPrev = mNext = NULL;

    return this;
}

//-----------------------------------------------------
// SingleLinkNode
//-----------------------------------------------------
SingleLinkNode *SingleLinkNode::InsertNext(SingleLinkNode *nextNode)
{
    assert(nextNode);

    nextNode->mNext = mNext;
    mNext = nextNode;

    return nextNode;
}


SingleLinkNode *SingleLinkNode::Detach(SingleLinkNode *prevNode)
{
    prevNode->mNext = mNext;
    mNext = NULL;

    return this;
}

//-----------------------------------------------------
// SimpleContainer
//-----------------------------------------------------
SimpleContainer::SimpleContainer()
{
    mHead.Initialize();
    mCurrent = &mHead;
}

ECode SimpleContainer::Current(DLinkNode **node)
{
    assert(NULL != node);

    if (NULL == mCurrent || &mHead == mCurrent) {
        return E_INVALID_OPERATION;
    }
    *node = mCurrent;
    return NOERROR;
}

ECode SimpleContainer::MoveNext()
{
    if (NULL == mCurrent) {
        return E_INVALID_OPERATION;
    }
    if (mCurrent == mHead.Prev()) {
        mCurrent = NULL;
        return S_FALSE;
    }
    mCurrent = mCurrent->Next();
    return NOERROR;
}

ECode SimpleContainer::Reset()
{
    mCurrent = &mHead;
    return NOERROR;
}

ECode SimpleContainer::Add(DLinkNode *node)
{
    mHead.InsertPrev(node);
    return NOERROR;
}

ECode SimpleContainer::Remove(DLinkNode *node)
{
    if (mCurrent == node) {
        mCurrent = mCurrent->Prev();
    }
    node->Detach();
    return NOERROR;
}

//-----------------------------------------------------
// ObjectContainer
//-----------------------------------------------------
ECode ObjectContainer::Dispose()
{
    ObjectNode *node;

    node = (ObjectNode *)mHead.Next();
    while (&mHead != (DLinkNode *)node) {
        node->Detach();
        delete node;
        node = (ObjectNode *)mHead.Next();
    }
    mCurrent = NULL;
    return NOERROR;
}

ECode ObjectContainer::Current(PInterface *obj)
{
    ECode ec;
    ObjectNode *node;

    ec = SimpleContainer::Current((DLinkNode **)&node);
    if (NOERROR == ec) *obj = node->mObject;
    return ec;
}

ECode ObjectContainer::Add(PInterface pObj)
{
    ObjectNode *node;

    node = new ObjectNode(pObj);
    if (NULL == node) return E_OUT_OF_MEMORY;

    return SimpleContainer::Add(node);
}

ECode ObjectContainer::Remove(PInterface pObj)
{
    ObjectNode *node;

    ForEachDLinkNode(ObjectNode *, node, &mHead) {
        if (node->mObject == pObj) {
            SimpleContainer::Remove(node);
            delete node;
            return NOERROR;
        }
    }
    return S_FALSE;
}

_ELASTOS_NAMESPACE_END
