#ifndef __UTILITY_CLINKEDLIST_H__
#define __UTILITY_CLINKEDLIST_H__

#include "_CLinkedList.h"
#include "AbstractSequentialList.h"

using Elastos::IO::IObjectInputStream;
using Elastos::IO::IObjectOutputStream;

namespace Elastos {
namespace Utility {

CarClass(CLinkedList) , public AbstractSequentialList
{
private:
    class Link : public ElLightRefBase
    {
    public:
        Link(
            /* [in] */ IInterface* o,
            /* [in] */ Link* p,
            /* [in] */ Link* n);

    public:
        AutoPtr<IInterface> mData;
        AutoPtr<Link> mPrevious;
        AutoPtr<Link> mNext;
    };

    class LinkIterator
        : public ElLightRefBase
        , public IListIterator
    {
    public:
        LinkIterator(
            /* [in] */ CLinkedList* object,
            /* [in] */ Int32 location);

        CAR_INTERFACE_DECL();

        CARAPI Add(
            /* [in] */ IInterface* object);

        CARAPI HasNext(
            /* [out] */ Boolean* result);

        CARAPI HasPrevious(
            /* [out] */ Boolean* result);

        CARAPI Next(
            /* [out] */ IInterface** object);

        CARAPI NextIndex(
            /* [out] */ Int32* index);

        CARAPI Previous(
            /* [out] */ IInterface** object);

        CARAPI PreviousIndex(
            /* [out] */ Int32* index);

        CARAPI Remove();

        CARAPI Set(
            /* [in] */ IInterface* object);

    public:
        Int32 mPos;
        Int32 mExpectedModCount;
        const AutoPtr<CLinkedList> mList;
        AutoPtr<Link> mLink;
        AutoPtr<Link> mLastLink;
    };

    class ReverseLinkIterator
        : public ElLightRefBase
        , public IIterator
    {
    public:
        ReverseLinkIterator(
            /* [in] */ CLinkedList* linkedList);

        CAR_INTERFACE_DECL();

        CARAPI HasNext(
            /* [out] */ Boolean* result);

        CARAPI Next(
            /* [out] */ IInterface** object);

        CARAPI Remove();

    private:
        Int32 mExpectedModCount;
        const AutoPtr<CLinkedList> mList;
        AutoPtr<Link> mLink;
        Boolean mCanRemove;
    };

public:
    CLinkedList();

    ~CLinkedList();

    /**
     * Constructs a new empty instance of {@code CLinkedList}.
     */
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ICollection* collection);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Add(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object);

    CARAPI Add(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI AddAll(
        /* [in] */ Int32 location,
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI AddAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI AddFirst(
        /* [in] */ IInterface* object);

    CARAPI AddLast(
        /* [in] */ IInterface* object);

    CARAPI Clear();

    /**
     * Returns a new {@code CLinkedList} with the same elements and size as this
     * {@code CLinkedList}.
     *
     * @return a shallow copy of this {@code CLinkedList}.
     * @see java.lang.Cloneable
     */
    virtual CARAPI Clone(
        /* [out] */ IInterface** object);

    CARAPI Contains(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI Get(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    CARAPI GetFirst(
        /* [out] */ IInterface** object);

    CARAPI GetLast(
        /* [out] */ IInterface** object);

    CARAPI IndexOf(
        /* [in] */ IInterface* object,
        /* [out] */ Int32* index);

    CARAPI LastIndexOf(
        /* [in] */ IInterface* object,
        /* [out] */ Int32* index);

    CARAPI GetListIterator(
        /* [in] */ Int32 location,
        /* [out] */ IListIterator** it);

    CARAPI Remove(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    CARAPI Remove(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI RemoveFirst(
        /* [out] */ IInterface** object);

    CARAPI RemoveLast(
        /* [out] */ IInterface** object);

    CARAPI GetDescendingIterator(
        /* [out] */ IIterator** iterator);

    CARAPI OfferFirst(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI OfferLast(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI PeekFirst(
        /* [out] */ IInterface** object);

    CARAPI PeekLast(
        /* [out] */ IInterface** object);

    CARAPI PollFirst(
        /* [out] */ IInterface** object);

    CARAPI PollLast(
        /* [out] */ IInterface** object);

    CARAPI Pop(
        /* [out] */ IInterface** object);

    CARAPI Push(
        /* [in] */ IInterface* object);

    CARAPI RemoveFirstOccurrence(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI RemoveLastOccurrence(
       /* [in] */ IInterface* object,
       /* [out] */ Boolean* result);

    CARAPI Set(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object,
        /* [out] */ IInterface** prevObject);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI Offer(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI Poll(
        /* [out] */ IInterface** object);

    CARAPI Remove(
        /* [out] */ IInterface** object);

    CARAPI Peek(
        /* [out] */ IInterface** object);

    CARAPI GetElement(
        /* [out] */ IInterface** object);

    CARAPI ToArray(
        /* [out, callee] */ ArrayOf<IInterface*>** array);

    CARAPI ToArray(
        /* [in] */ ArrayOf<IInterface*>* contents,
        /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    CARAPI GetIterator(
        /* [out] */ IIterator** iterator);

    CARAPI ContainsAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    CARAPI RemoveAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI RetainAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI GetListIterator(
        /* [out] */ IListIterator** it);

    CARAPI SubList(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IList** subList);

private:
    CARAPI AddLastImpl(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI AddFirstImpl(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI GetFirstImpl(
        /* [out] */ IInterface** object);

    CARAPI RemoveFirstImpl(
        /* [out] */ IInterface** object);

    CARAPI RemoveLastImpl(
        /* [out] */ IInterface** object);

    CARAPI RemoveFirstOccurrenceImpl(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI RemoveOneOccurrence(
        /* [in] */ IInterface* object,
        /* [in] */ IIterator* iter,
        /* [out] */ Boolean* result);

    CARAPI PeekFirstImpl(
        /* [out] */ IInterface** object);

    CARAPI WriteObject(
        /* [in] */ IObjectOutputStream* stream);

    CARAPI ReadObject(
        /* [in] */ IObjectInputStream* stream);

public:
    Int32 mSize;
    AutoPtr<Link> mVoidLink;
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_CLINKEDLIST_H__
