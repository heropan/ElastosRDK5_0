//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CONTAINER_H__
#define __CONTAINER_H__
#include <elautoptr.h>
#include <eltypes.h>
#include <linknode.h>

using Elastos::AutoPtr;

/*!
 @brief In this file, there are two simple container implementations, they
 are ObjectContainer and CallbackContextContainer, both of them inherited
 from base class SimpleContainer. They provide six methods: Current,MoveNext,
 Reset, Add, Remove and Dispose.
 In fact, Add, Remove and Dispose are right methods of container, but the
 Current, MoveNext and Reset are more like methods of enumerator,we take
 them together for terse reason.
 @since
 @sa
 */
class SimpleContainer
{
public:
/*!
 @brief constructs a new SimpleContainer.
 */
    SimpleContainer();

/*!
 @brief Get the current node of this SimpleContainer @a container,save the current
 node into @a node.
 @param node <b>[out]</b> pointer pointed to the pointer of the current node.
 @return
 @since
 @sa
 */
    _ELASTOS ECode Current(
        /* [in] */ DLinkNode **);

/*!
 @brief Get the node next to the current node in this SimpleContainer,and move it to
 be the current node.
 @return
 @since
 @sa
 */
    _ELASTOS ECode MoveNext();

/*!
 @brief Move the current node's pointer to point to the head node in this SimpleContainer.
 @return
 @since
 @sa
 */
    _ELASTOS ECode Reset();

/*!
 @brief insert a DLinkNode @a node into this SimpleContainer before the head node @a mHead.
 @param node <b>[in]</b> pointer pointed to the node to be inserted.
 @return
 @since
 @sa
 */
    _ELASTOS ECode Add(
        /* [in] */ DLinkNode *);

/*!
 @brief remove a DLinkNode @a node from this SimpleContainer.

 if the node points to the current node,then remove the current node,and move
 the pointer of current node to its previous node before the removing.
 @param node <b>[in]</b> pointer pointed to the node to be removed.
 @return
 @since
 @sa
 */
    _ELASTOS ECode Remove(
        /* [in] */ DLinkNode *);

public:
/*!
 @brief the head node.
 */
    DLinkNode  mHead;
/*!
 @brief the current node.
 */
    DLinkNode  *mCurrent;
};
/*@} */

CAR_INLINE SimpleContainer::SimpleContainer()
{
    mHead.Initialize();
    mCurrent = &mHead;
}

/*!
 @brief Get the current node of this SimpleContainer @a container,save the current
 node into @a node.
 @param container pointer pointed to the SimpleContainer.
 @param node <b>[out]</b> pointer pointed to the pointer of the current node.
 @return
 @since
 @sa
 */
ELAPI SimpleContainerCurrent(
    /* [in] */ SimpleContainer *container,
    /* [in] */ DLinkNode **node);

/*!
 @brief Get the node next to the current node in this SimpleContainer @a container,
 and move it to be the current node.
 @param container pointer pointed to the SimpleContainer.
 @return
 @since
 @sa
 @relates
 */
ELAPI SimpleContainerMoveNext(
    /* [in] */ SimpleContainer *container);

/*!
 @brief Move the current node's pointer to point to the head node in this
 SimpleContainer @a container.
 @param container pointer pointed to the SimpleContainer.
 @return
 @since
 @sa
 @relates
 */
ELAPI SimpleContainerReset(
    /* [in] */ SimpleContainer *container);

/*!
 @brief insert a DLinkNode @a node into this SimpleContainer @a container
 before the head node @a mHead.
 @param container pointer pointed to the SimpleContainer.
 @param node  pointer pointed to the node to be inserted.
 @return
 @since
 @sa
 @relates
 */
ELAPI SimpleContainerAdd(
    /* [in] */ SimpleContainer *container,
    /* [in] */ DLinkNode *node);

/*!
 @brief remove a DLinkNode @a node from this SimpleContainer @a container.
 if the node points to the current node,then remove the current node,and move
 the pointer of current node to its previous node before the removing.
 @param container pointer pointed to the SimpleContainer.
 @param node pointer pointed to the node to be removed.
 @return
 @since
 @sa
 @relates
 */
ELAPI SimpleContainerRemove(
    /* [in] */ SimpleContainer *container,
    /* [in] */ DLinkNode *node);

CAR_INLINE _ELASTOS ECode SimpleContainer::Current(
    /* [in] */ DLinkNode **node)
{
    return SimpleContainerCurrent(this, node);
}

CAR_INLINE _ELASTOS ECode SimpleContainer::MoveNext()
{
    return SimpleContainerMoveNext(this);
}

CAR_INLINE _ELASTOS ECode SimpleContainer::Reset()
{
    return SimpleContainerReset(this);
}

CAR_INLINE _ELASTOS ECode SimpleContainer::Add(
    /* [in] */ DLinkNode *node)
{
    return SimpleContainerAdd(this, node);
}

CAR_INLINE _ELASTOS ECode SimpleContainer::Remove(
    /* [in] */ DLinkNode *node)
{
    return SimpleContainerRemove(this, node);
}

/** @addtogroup BaseTypesRef
  *   @{
  */
struct ObjectNode : public DLinkNode
{
/*!
 @brief constructs a ObjectNode.
 @param object <b>[in]</b> the content of the object node.
 @since
 @sa
 */
    ObjectNode(
        /* [in] */ IInterface* object):mObject(object) {}

/*!
 @brief a PInterface instance.
 */
    AutoPtr<IInterface> mObject;
};

class ObjectContainer : public SimpleContainer
{
public:
/*!
 @brief Get the current node object from this ObjectContainer,and save it in
 the @a object.
 @param object <b>[out]</b>  pointer pointed to the node object.
 @return
 @since
 @sa
 */
    _ELASTOS ECode Current(
        /* [out] */ PInterface *object);

/*!
 @brief insert @a object into this ObjectContainer before the head object node @a mHead.
 @param object <b>[in]</b> pointer pointed to the node object to be added.
 @return
 @since
 @sa
 */
    _ELASTOS ECode Add(
        /* [in] */ PInterface object);

/*!
 @brief remove @a object from this ObjectContainer.

 if the node points to the current object node,then remove the current object node,and
 move the pointer of current object node to its previous object node before the removing.
 @param object <b>[in]</b> pointer pointed to the object node to be removed.
 @return
 @since
 @sa
 */
    _ELASTOS ECode Remove(
        /* [in] */ PInterface object);

/*!
 @brief remove all the object node of this ObjectContainer.
 @return
 @since
 @sa
 */
    _ELASTOS ECode Dispose();

/*!
 @brief the destructor of ObjectContainer.
*/
    virtual ~ObjectContainer();
};

/*!
 @brief remove all the object node of this ObjectContainer @a container.
 @param container pointer pointed to the ObjectContainer.
 @return
 @since
 @sa
 @relates
 */
ELAPI ObjectContainerDispose(
    /* [in] */ ObjectContainer *container);

/*!
 @brief Get the current object node object from this ObjectContainer @a container,and
 save it in the @a object.
 @param container pointer pointed to the ObjectContainer.
 @param object pointer pointed to the node object.
 @return
 @since
 @sa
 @relates
 */
ELAPI ObjectContainerCurrent(
    /* [in] */ ObjectContainer *container,
    /* [out] */ PInterface *object);

/*!
 @brief insert @a object into this ObjectContainer @a container before the head
 object node @a mHead.
 @param container pointer pointed to the ObjectContainer.
 @param object pointer pointed to the node object to be added.
 @return
 @since
 @sa
 @relates
 */
ELAPI ObjectContainerAdd(
    /* [in] */ ObjectContainer *container,
    /* [in] */ PInterface object);

/*!
 @brief remove @a object into this ObjectContainer @a container before the current
 object node.
 if the node points to the current object node,then remove the current object node,and
 move the pointer of current object node to its previous object node before the removing.
 @param container pointer pointed to the ObjectContainer.
 @param object pointer pointed to the object node to be removed.
 @return
 @since
 @sa
 @relates
 */
ELAPI ObjectContainerRemove(
    /* [in] */ ObjectContainer *container,
    /* [in] */ PInterface object);

CAR_INLINE ObjectContainer::~ObjectContainer()
{
    this->Dispose();
}

CAR_INLINE _ELASTOS ECode ObjectContainer::Dispose()
{
    return ObjectContainerDispose(this);
}

CAR_INLINE _ELASTOS ECode ObjectContainer::Current(
    /* [out] */ PInterface *object)
{
    return ObjectContainerCurrent(this, object);
}

CAR_INLINE _ELASTOS ECode ObjectContainer::Add(
    /* [in] */ PInterface object)
{
    return ObjectContainerAdd(this, object);
}

CAR_INLINE _ELASTOS ECode ObjectContainer::Remove(
    /* [in] */ PInterface object)
{
    return ObjectContainerRemove(this, object);
}

#endif // __CONTAINER_H__
