
#ifndef __CCONCURRENTLINKEDQUEUE_H__
#define __CCONCURRENTLINKEDQUEUE_H__

#include "_CConcurrentLinkedQueue.h"
#include "AbstractQueue.h"

using Elastos::IO::IObjectOutputStream;
using Elastos::IO::IObjectInputStream;

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CConcurrentLinkedQueue) , public AbstractQueue
{
private:
    class Node
        : public IInterface
        , public ElRefBase
    {
    public:
        Node(
            /* [in] */ IInterface* item);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* object,
            /* [out] */ InterfaceID* iid);

        Boolean CasItem(
            /* [in] */ IInterface* cmp,
            /* [in] */ IInterface* val);

        void LazySetNext(
            /* [in] */ Node* val);

        Boolean CasNext(
            /* [in] */ Node* cmp,
            /* [in] */ Node* val);

    public:
        AutoPtr<IInterface> mItem;
        volatile AutoPtr<Node> mNext;

    private:
//        static sun.misc.Unsafe UNSAFE;
        static Int64 sItemOffset;
        static Int64 sNextOffset;
    };

    class Itr
        : public IIterator
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL();

        Itr(
            /* [in] */ CConcurrentLinkedQueue* owner);

        CARAPI HasNext(
            /* [out] */ Boolean* result);

        CARAPI Next(
            /* [out] */ IInterface** object);

        CARAPI Remove();

    private:
        /**
         * Moves to next valid node and returns item to return for
         * next(), or null if no such.
         */
        AutoPtr<IInterface> Advance();

    public:
        CConcurrentLinkedQueue* mOwner;

    private:
        /**
         * Next node to return item for.
         */
        AutoPtr<Node> mNextNode;

        /**
         * nextItem holds on to item fields because once we claim
         * that an element exists in hasNext(), we must return it in
         * the following next() call even if it was in the process of
         * being removed when hasNext() was called.
         */
        AutoPtr<IInterface> mNextItem;

        /**
         * Node of the last returned item, to support remove.
         */
        AutoPtr<Node> mLastRet;
    };

public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ICollection* c);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Add(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    CARAPI Offer(
        /* [in] */ IInterface* e,
        /* [out] */ Boolean* result);

    CARAPI Poll(
        /* [out] */ IInterface** e);

    CARAPI Peek(
        /* [out] */ IInterface** e);

    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI Contains(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI Remove(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    CARAPI AddAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI ToArray(
        /* [out, callee] */ ArrayOf<IInterface*>** array);

    CARAPI ToArray(
        /* [in] */ ArrayOf<IInterface*>* inArray,
        /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    CARAPI GetIterator(
        /* [out] */ IIterator** iterator);

    CARAPI ContainsAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI RemoveAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI RetainAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI Clear();

    CARAPI Remove(
        /* [out] */ IInterface** e);

    CARAPI Element(
        /* [out] */ IInterface** e);

protected:
    CARAPI_(void) UpdateHead(
        /* [in] */ Node* h,
        /* [in] */ Node* p);

    CARAPI_(AutoPtr<Node>) Succ(
        /* [in] */ Node* p);

    CARAPI_(AutoPtr<Node>) First();

private:
    CARAPI_(void) WriteObject(
        /* [in] */ IObjectOutputStream* s);

    CARAPI_(void) ReadObject(
        /* [in] */ IObjectInputStream* s);

    static CARAPI CheckNotNull(
        /* [in] */ IInterface* v);

    CARAPI_(Boolean) CasTail(
        /* [in] */ Node* cmp,
        /* [in] */ Node* val);

    CARAPI_(Boolean) CasHead(
        /* [in] */ Node* cmp,
        /* [in] */ Node* val);

private:
//    static long serialVersionUID = 196745693267521676L;
    volatile AutoPtr<Node> mHead;
    volatile AutoPtr<Node> mTail;
//    static sun.misc.Unsafe UNSAFE;
    static Int64 sHeadOffset;
    static Int64 sTailOffset;
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__CCONCURRENTLINKEDQUEUE_H__