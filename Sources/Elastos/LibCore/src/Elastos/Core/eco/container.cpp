#include "cmdef.h"
#include "container.h"

ELAPI SimpleContainerCurrent(
    /* [in] */ SimpleContainer *container,
    /* [in] */ DLinkNode **node)
{
    assert(NULL != node);

    if (NULL == container->mCurrent || &(container->mHead) == container->mCurrent) {
        return E_INVALID_OPERATION;
    }
    *node = container->mCurrent;
    return NOERROR;
}

ELAPI SimpleContainerMoveNext(
    /* [in] */ SimpleContainer *container)
{
    if (NULL == container->mCurrent) {
        return E_INVALID_OPERATION;
    }
    if (container->mCurrent == container->mHead.Prev()) {
        container->mCurrent = NULL;
        return S_FALSE;
    }
    container->mCurrent = container->mCurrent->Next();
    return NOERROR;
}

ELAPI SimpleContainerReset(
    /* [in] */ SimpleContainer *container)
{
    container->mCurrent = &(container->mHead);
    return NOERROR;
}

ELAPI SimpleContainerAdd(
    /* [in] */ SimpleContainer *container,
    /* [in] */ DLinkNode *node)
{
    container->mHead.InsertPrev(node);
    return NOERROR;
}

ELAPI SimpleContainerRemove(
    /* [in] */ SimpleContainer *container,
    /* [in] */ DLinkNode *node)
{
    if (container->mCurrent == node) {
        container->mCurrent = container->mCurrent->Prev();
    }
    node->Detach();
    return NOERROR;
}

ELAPI ObjectContainerDispose(
    /* [in] */ ObjectContainer *container)
{
    ObjectNode *node;

    node = (ObjectNode *)container->mHead.Next();
    while (&(container->mHead) != (DLinkNode *)node) {
        node->Detach();
        delete node;
        node = (ObjectNode *)container->mHead.Next();
    }
    container->mCurrent = NULL;
    return NOERROR;
}

ELAPI ObjectContainerCurrent(
    /* [in] */ ObjectContainer *container,
    /* [out] */ PInterface *object)
{
    _ELASTOS ECode ec;
    ObjectNode *node;

    ec = SimpleContainerCurrent((SimpleContainer*)container, (DLinkNode **)&node);
    if (NOERROR == ec) {
        *object = node->mObject;
        INTERFACE_ADDREF(*object);
    }
    return ec;
}

ELAPI ObjectContainerAdd(
    /* [in] */ ObjectContainer *container,
    /* [in] */ PInterface object)
{
    ObjectNode *node = new ObjectNode(object);
    if (NULL == node) return E_OUT_OF_MEMORY;

    return SimpleContainerAdd((SimpleContainer*)container, node);
}

ELAPI ObjectContainerRemove(
    /* [in] */ ObjectContainer *container,
    /* [in] */ PInterface object)
{
    ObjectNode *node;

    ForEachDLinkNode(ObjectNode *, node, &(container->mHead)) {
        if (node->mObject.Get() == object) {
            SimpleContainerRemove((SimpleContainer*)container, node);
            delete node;
            return NOERROR;
        }
    }
    return S_FALSE;
}
