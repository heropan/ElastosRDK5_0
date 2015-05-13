#ifndef __UTILITY_CCOLLECTIONS_H__
#define __UTILITY_CCOLLECTIONS_H__

#include "_CCollections.h"
#include "AbstractList.h"
#include "AbstractSet.h"
#include "AbstractMap.h"
#include <elastos/Mutex.h>
#include "MapEntry.h"
#include "AbstractQueue.h"

using Elastos::IO::IObjectOutputStream;
using Elastos::IO::IObjectInputStream;
using Elastos::Core::IComparator;
using Elastos::Core::IRandom;

namespace Elastos {
namespace Utility {

CarClass(CCollections)
{
public:
    class SynchronizedCollection
        : public ICollection
        , public ISerializable
        , public ElRefBase
    {
    public:
        SynchronizedCollection(
            /* [in] */ ICollection* collection);

        SynchronizedCollection(
            /* [in] */ ICollection* collection,
            /* [in] */ Mutex* mutex);

        CAR_INTERFACE_DECL()

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToString(
            /* [out] */ String* str);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

    private:
        CARAPI WriteObject(
            /* [in] */ IObjectOutputStream* stream);

    public:
        AutoPtr<ICollection> mC;
        Mutex* mMutex;
    };

    class SynchronizedList
        : public SynchronizedCollection
        , public IList
    {
    public:
        SynchronizedList(
            /* [in] */ IList* l);

        SynchronizedList(
            /* [in] */ IList* l,
            /* [in] */ Mutex* mutex);

        CAR_INTERFACE_DECL()

        CARAPI AddEx(
            /* [in] */ Int32 location,
            /* [in] */ IInterface* object);

        CARAPI AddAllEx(
            /* [in] */ Int32 location,
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI Get(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** object);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        CARAPI IndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* index);

        CARAPI LastIndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* index);

        CARAPI GetListIterator(
            /* [out] */ IListIterator** it);

        CARAPI GetListIteratorEx(
            /* [in] */ Int32 location,
            /* [out] */ IListIterator** it);

        CARAPI RemoveEx(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** object);

        CARAPI Set(
            /* [in] */ Int32 location,
            /* [in] */ IInterface* object,
            /* [out] */ IInterface** prevObject);

        CARAPI SubList(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ IList** subList);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    private:
        CARAPI WriteObject(
            /* [in] */ IObjectOutputStream* stream);

        CARAPI_(AutoPtr<IInterface>) ReadResolve();

    public:
        AutoPtr<IList> mList;
    };

    class SynchronizedRandomAccessList
        : public SynchronizedList
        , public IRandomAccess
    {
    public:
        SynchronizedRandomAccessList(
            /* [in] */ IList* l);

        SynchronizedRandomAccessList(
            /* [in] */ IList* l,
            /* [in] */ Mutex* mutex);

        CAR_INTERFACE_DECL()

        CARAPI SubList(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ IList** subList);

    private:
        CARAPI_(AutoPtr<IInterface>) WriteReplace();
    };

    class SynchronizedSet
        : public SynchronizedCollection
        , public ISet
    {
    public:
        SynchronizedSet(
            /* [in] */ ISet* set);

        SynchronizedSet(
            /* [in] */ ISet* set,
            /* [in] */ Mutex* mutex);

        CAR_INTERFACE_DECL()

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    private:
        CARAPI WriteObject(
            /* [in] */ IObjectOutputStream* stream);
    };

    class SynchronizedMap
        : public IMap
        , public ISerializable
        , public ElRefBase
    {
    public:
        SynchronizedMap(
            /* [in] */ IMap* map);

        SynchronizedMap(
            /* [in] */ IMap* map,
            /* [in] */ Mutex* mutex);

        CAR_INTERFACE_DECL()

        CARAPI Clear();

        CARAPI ContainsKey(
            /* [in] */ IInterface* key,
            /* [out] */ Boolean* result);

        CARAPI ContainsValue(
            /* [in] */ IInterface* value,
            /* [out] */ Boolean* result);

        CARAPI EntrySet(
            /* [out, callee] */ ISet** entries);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI Get(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI KeySet(
            /* [out, callee] */ ISet** keySet);

        CARAPI Put(
            /* [in] */ PInterface key,
            /* [in] */ PInterface value,
            /* [out] */ PInterface* oldValue);

        CARAPI PutAll(
            /* [in] */ IMap* map);

        CARAPI Remove(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI Values(
            /* [out] */ ICollection** value);

        CARAPI ToString(
            /* [out] */ String* str);

    private:
        CARAPI WriteObject(
            /* [in] */ IObjectOutputStream* stream);

    public:
        Mutex* mMutex;
        AutoPtr<IMap> mM;
    };

    class SynchronizedSortedMap
        : public SynchronizedMap
        , public ISortedMap
    {
    public:
        SynchronizedSortedMap(
            /* [in] */ ISortedMap* map);

        SynchronizedSortedMap(
            /* [in] */ ISortedMap* map,
            /* [in] */ Mutex* mutex);

        CAR_INTERFACE_DECL()

        CARAPI Comparator(
            /* [out] */ IComparator** comp);

        CARAPI FirstKey(
            /* [out] */ IInterface** outface);

        CARAPI HeadMap(
            /* [in] */ IInterface* endKey,
            /* [out] */ ISortedMap** sortmap);

        CARAPI LastKey(
            /* [out] */ IInterface** outface);

        CARAPI SubMap(
            /* [in] */ IInterface* startKey,
            /* [in] */ IInterface* endKey,
            /* [out] */ ISortedMap** sortmap);

        CARAPI TailMap(
            /* [in] */ IInterface* startKey,
            /* [out] */ ISortedMap** sortmap);

        CARAPI Clear();

        CARAPI ContainsKey(
            /* [in] */ IInterface* key,
            /* [out] */ Boolean* result);

        CARAPI ContainsValue(
            /* [in] */ IInterface* value,
            /* [out] */ Boolean* result);

        CARAPI EntrySet(
            /* [out, callee] */ ISet** entries);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI Get(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI KeySet(
            /* [out, callee] */ ISet** keySet);

        CARAPI Values(
            /* [out] */ ICollection** value);

        CARAPI Put(
            /* [in] */ PInterface key,
            /* [in] */ PInterface value,
            /* [out] */ PInterface* oldValue);

        CARAPI PutAll(
            /* [in] */ IMap* map);

        CARAPI Remove(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI GetSize(
            /* [out] */ Int32* size);

    private:
        CARAPI WriteObject(
            /* [in] */ IObjectOutputStream* stream);
        AutoPtr<ISortedMap> mSm;
    };

    class SynchronizedSortedSet
        : public SynchronizedSet
        , public ISortedSet
    {
    public:
        SynchronizedSortedSet(
            /* [in] */ ISortedSet* set);

        SynchronizedSortedSet(
            /* [in] */ ISortedSet* set,
            /* [in] */ Mutex* mutex);

        CAR_INTERFACE_DECL()

        CARAPI Comparator(
            /* [out] */ IComparator** outcom);

        CARAPI First(
            /* [out] */ IInterface** outface);

        CARAPI HeadSet(
            /* [in] */ IInterface* end,
            /* [out] */ ISortedSet** outsort);

        CARAPI Last(
            /* [out] */ IInterface** outface);

        CARAPI SubSet(
            /* [in] */ IInterface* start,
            /* [in] */ IInterface* end,
            /* [out] */ ISortedSet** outsort);

        CARAPI TailSet(
            /* [in] */ IInterface* start,
            /* [out] */ ISortedSet** outsort);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    private:
        CARAPI WriteObject(
            /* [in] */ IObjectOutputStream* stream);
        AutoPtr<ISortedSet> mSs;
    };

private:
    class Iterator
        : public IIterator
        , public ElRefBase
    {
    public:
        Iterator();

        CAR_INTERFACE_DECL()

        CARAPI HasNext(
            /* [out] */ Boolean* result);

        CARAPI Next(
            /* [out] */ IInterface** object);

        CARAPI Remove();
    };

    class MyEnumeration
        : public IEnumeration
        , public ElRefBase
    {
    public:
        MyEnumeration();

        CAR_INTERFACE_DECL()

        CARAPI HasMoreElements(
            /* [out] */ Boolean* value);

        CARAPI NextElement(
            /* [out] */ IInterface** inter);
    };

    class MyEnumeration2
        : public IEnumeration
        , public ElRefBase
    {
    public:
        MyEnumeration2(
            /* [in] */ ICollection* c);

        CAR_INTERFACE_DECL()

        CARAPI HasMoreElements(
            /* [out] */ Boolean* value);

        CARAPI NextElement(
            /* [out] */ IInterface** inter);
    public:
        AutoPtr<IIterator> mIt;
    };

    class CopiesList
        : public IList
        , public AbstractList
        , public ISerializable
        , public ElRefBase
    {
    public:
        CopiesList(
            /* [in] */ Int32 length,
            /* [in] */ IInterface* object);

        CAR_INTERFACE_DECL()

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI Get(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** object);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

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

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

        CARAPI AddEx(
            /* [in] */ Int32 location,
            /* [in] */ IInterface* object);

        CARAPI AddAllEx(
            /* [in] */ Int32 location,
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI IndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* index);

        CARAPI LastIndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* index);

        CARAPI GetListIterator(
            /* [out] */ IListIterator** it);

        CARAPI GetListIteratorEx(
            /* [in] */ Int32 location,
            /* [out] */ IListIterator** it);

        CARAPI RemoveEx(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** object);

        CARAPI Set(
            /* [in] */ Int32 location,
            /* [in] */ IInterface* object,
            /* [out] */ IInterface** prevObject);

        CARAPI SubList(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ IList** subList);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);
        Int32 mN;
        AutoPtr<IInterface> mElement;
    };

public:
    class EmptyList
        : public AbstractList
        , public IList
        , public IRandomAccess
        , public ISerializable
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI Get(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** object);

        CARAPI Set(
            /* [in] */ Int32 location,
            /* [in] */ IInterface* object,
            /* [out] */ IInterface** prevObject);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI Clear();

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

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* contents,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

        CARAPI AddEx(
            /* [in] */ Int32 location,
            /* [in] */ IInterface* object);

        CARAPI AddAllEx(
            /* [in] */ Int32 location,
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI IndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* index);

        CARAPI LastIndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* index);

        CARAPI GetListIterator(
            /* [out] */ IListIterator** it);

        CARAPI SubList(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ IList** subList);

        CARAPI GetListIteratorEx(
            /* [in] */ Int32 location,
            /* [out] */ IListIterator** listiterator);

        CARAPI RemoveEx(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** object);

    private:
        CARAPI_(AutoPtr<IInterface>) ReadResolve();
    };

    class EmptySet
        : public AbstractSet
        , public ISet
        , public ISerializable
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

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

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    private:
        CARAPI_(AutoPtr<IInterface>) ReadResolve();
    };

    class EmptyMap
        : public AbstractMap
        , public IMap
        , public ISerializable
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI ContainsKey(
            /* [in] */ IInterface* key,
            /* [out] */ Boolean* result);

        CARAPI ContainsValue(
            /* [in] */ IInterface* value,
            /* [out] */ Boolean* result);

        CARAPI EntrySet(
            /* [out, callee] */ ISet** entries);

        CARAPI Get(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI KeySet(
            /* [out, callee] */ ISet** keySet);

        CARAPI Values(
            /* [out] */ ICollection** value);

        CARAPI Clear();

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Put(
            /* [in] */ PInterface key,
            /* [in] */ PInterface value,
            /* [out] */ PInterface* oldValue);

        CARAPI PutAll(
            /* [in] */ IMap* map);

        CARAPI Remove(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI GetSize(
            /* [out] */ Int32* size);

    private:
        CARAPI_(AutoPtr<IInterface>) ReadResolve();
    };

    class ReverseComparator
        : public IComparator
        , public ISerializable
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Compare(
            /* [in] */ IInterface* lhs,
            /* [in] */ IInterface* rhs,
            /* [out] */ Int32* result);

    private:
        CARAPI_(AutoPtr<IInterface>) ReadResolve();

    public:
        static const AutoPtr<ReverseComparator> INSTANCE;
    };

    class ReverseComparator2
        : public IComparator
        , public ISerializable
        , public ElRefBase
    {
    public:
        ReverseComparator2(
            /* [in] */ IComparator* comparator);

        CAR_INTERFACE_DECL()

        CARAPI Compare(
            /* [in] */ IInterface* lhs,
            /* [in] */ IInterface* rhs,
            /* [out] */ Int32* result);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);
    public:
        AutoPtr<IComparator> mCmp;
    };

    class SingletonSet
        : public AbstractSet
        , public ISet
        , public ISerializable
        , public ElRefBase
    {
    private:
        class Iterator
            : public IIterator
            , public ElRefBase
        {
        public:
            Iterator(
                /* [in] */ SingletonSet* owner);

            CAR_INTERFACE_DECL()

            CARAPI HasNext(
                /* [out] */ Boolean* result);

            CARAPI Next(
                /* [out] */ IInterface** object);

            CARAPI Remove();

        public:
            Boolean mHasNext;
            AutoPtr<SingletonSet> mOwner;
        };
    public:
        SingletonSet(
            /* [in] */ IInterface* object);

        CAR_INTERFACE_DECL()

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

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

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    public:
        AutoPtr<IInterface> mElement;
    };

    class SingletonList
        : public AbstractList
        , public IList
        , public ISerializable
        , public ElRefBase
    {
    public:
        SingletonList(
            /* [in] */ IInterface* object);

        CAR_INTERFACE_DECL()

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI Get(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** object);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

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

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

        CARAPI AddEx(
            /* [in] */ Int32 location,
            /* [in] */ IInterface* object);

        CARAPI AddAllEx(
            /* [in] */ Int32 location,
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI IndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* index);

        CARAPI LastIndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* index);

        CARAPI GetListIterator(
            /* [out] */ IListIterator** it);

        CARAPI GetListIteratorEx(
            /* [in] */ Int32 location,
            /* [out] */ IListIterator** it);

        CARAPI RemoveEx(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** object);

        CARAPI Set(
            /* [in] */ Int32 location,
            /* [in] */ IInterface* object,
            /* [out] */ IInterface** prevObject);

        CARAPI SubList(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ IList** subList);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

    public:
        AutoPtr<IInterface> mElement;
    };

    class SingletonMap
        : public AbstractMap
        , public IMap
        , public ISerializable
        , public ElRefBase
    {
    private:
        class MySet
            : public ISet
            , public AbstractSet
            , public ElRefBase
        {
        private:
            class Iterator
                : public IIterator
                , public ElRefBase
            {
            private:
                class MyMapEntry : public MapEntry
                {
                public:
                    MyMapEntry(
                        /* [in] */ IInterface* k,
                        /* [in] */ IInterface* v);

                    CARAPI SetValue(
                        /* [in] */ IInterface* valueReplacer,
                        /* [out] */ IInterface** valueReplacee);
                };

            public:
                Iterator(
                    /* [in] */ MySet* owner);

                CAR_INTERFACE_DECL()

                CARAPI HasNext(
                    /* [out] */ Boolean* result);

                CARAPI Next(
                    /* [out] */ IMapEntry** object);

                CARAPI Next(
                    /* [out] */ IInterface** object);

                CARAPI Remove();

            public:
                Boolean mHasNext;
                AutoPtr<MySet> mOwner;
            };
        public:
            MySet(
                /* [in] */ SingletonMap* owner);

            CAR_INTERFACE_DECL()

            CARAPI Contains(
                /* [in] */ IInterface* object,
                /* [out] */ Boolean* result);

            CARAPI GetSize(
                /* [out] */ Int32* size);

            CARAPI GetIterator(
                /* [out] */ IIterator** it);

            CARAPI Add(
                /* [in] */ IInterface* object,
                /* [out] */ Boolean* modified);

            CARAPI AddAll(
                /* [in] */ ICollection* collection,
                /* [out] */ Boolean* modified);

            CARAPI Clear();

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

            CARAPI Remove(
                /* [in] */ IInterface* object,
                /* [out] */ Boolean* modified);

            CARAPI RemoveAll(
                /* [in] */ ICollection* collection,
                /* [out] */ Boolean* modified);

            CARAPI RetainAll(
                /* [in] */ ICollection* collection,
                /* [out] */ Boolean* modified);

            CARAPI ToArray(
                /* [out, callee] */ ArrayOf<IInterface*>** array);

            CARAPI ToArrayEx(
                /* [in] */ ArrayOf<IInterface*>* inArray,
                /* [out, callee] */ ArrayOf<IInterface*>** outArray);

        private:
            SingletonMap* mOwner;
        };
    public:
        SingletonMap(
            /* [in] */ IInterface* key,
            /* [in] */ IInterface* value);

        CAR_INTERFACE_DECL()

        CARAPI ContainsKey(
            /* [in] */ IInterface* key,
            /* [out] */ Boolean* result);

        CARAPI ContainsValue(
            /* [in] */ IInterface* value,
            /* [out] */ Boolean* result);

        CARAPI Get(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI EntrySet(
            /* [out, callee] */ ISet** entries);

        CARAPI Clear();

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI KeySet(
            /* [out, callee] */ ISet** keySet);

        CARAPI Put(
            /* [in] */ PInterface key,
            /* [in] */ PInterface value,
            /* [out] */ PInterface* oldValue);

        CARAPI PutAll(
            /* [in] */ IMap* map);

        CARAPI Remove(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI Values(
            /* [out] */ ICollection** value);

    protected:
        AutoPtr<IInterface> mK;
        AutoPtr<IInterface> mV;
    };

    class UnmodifiableCollection
        : public ICollection
        , public ISerializable
        , public ElRefBase
    {
    private:
        class Iterator
            : public IIterator
            , public ElRefBase
        {
        public:
            Iterator(
                /* [in] */ UnmodifiableCollection* owner);

            CAR_INTERFACE_DECL()

            CARAPI HasNext(
                /* [out] */ Boolean* result);

            CARAPI Next(
                /* [out] */ IInterface** object);

            CARAPI Remove();
        public:
            AutoPtr<Iterator> mIterator;
        };
    public:
        UnmodifiableCollection(
            /* [in] */ ICollection* collection);

        CAR_INTERFACE_DECL()

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

        CARAPI ToString(
            /* [out] */ String* str);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

    public:
        AutoPtr<ICollection> mC;
    };

    class UnmodifiableList
        : public UnmodifiableCollection
        , public IList
    {
    private:
        class ListIterator
            : public IListIterator
            , public ElRefBase
        {
        public:
            ListIterator(
                /* [in] */ UnmodifiableList* owner,
                /* [in] */ Int32 location);

            CAR_INTERFACE_DECL()

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
            AutoPtr<IListIterator> mIterator;
        };
    public:
        UnmodifiableList(
            /* [in] */ IList* l);

        CAR_INTERFACE_DECL()

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI Get(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** object);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        CARAPI IndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* index);

        CARAPI LastIndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* index);

        CARAPI GetListIterator(
            /* [out] */ IListIterator** it);

        CARAPI GetListIteratorEx(
            /* [in] */ Int32 location,
            /* [out] */ IListIterator** it);

        CARAPI RemoveEx(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** object);

        CARAPI Set(
            /* [in] */ Int32 location,
            /* [in] */ IInterface* object,
            /* [out] */ IInterface** prevObject);

        CARAPI SubList(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ IList** subList);

        CARAPI Clear();

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

        CARAPI AddEx(
            /* [in] */ Int32 location,
            /* [in] */ IInterface* object);

        CARAPI AddAllEx(
            /* [in] */ Int32 location,
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

    private:
        CARAPI_(AutoPtr<IInterface>) ReadResolve();

    public:
        AutoPtr<IList> mList;
    };

    class UnmodifiableRandomAccessList
        : public UnmodifiableList
        , public IRandomAccess
    {
    public:
        UnmodifiableRandomAccessList(
            /* [in] */ IList* l);

        CAR_INTERFACE_DECL()

        CARAPI SubList(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ IList** subList);

    private:
        CARAPI_(AutoPtr<IInterface>) WriteReplace();
    };

    class UnmodifiableSet
        : public UnmodifiableCollection
        , public ISet
    {
    public:
        UnmodifiableSet(
            /* [in] */ ISet* set);

        CAR_INTERFACE_DECL()

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);
    };

    class UnmodifiableMap
        : public IMap
        , public ISerializable
        , public ElRefBase
    {
    private:
        class UnmodifiableEntrySet
            : public UnmodifiableSet
        {
        private:
            class UnmodifiableMapEntry
                : public IMapEntry
                , public ElRefBase
            {
            public:
                UnmodifiableMapEntry(
                    /* [in] */ IMapEntry* entry);

                CAR_INTERFACE_DECL()

                CARAPI Equals(
                    /* [in] */ IInterface* entry,
                    /* [out] */ Boolean* result);

                CARAPI GetKey(
                    /* [out] */ PInterface* key);

                CARAPI GetValue(
                    /* [out] */ PInterface* value);

                CARAPI GetHashCode(
                    /* [out] */ Int32* hashCode);

                CARAPI SetValue(
                    /* [in] */ IInterface* valueReplacer,
                    /* [out] */ IInterface** valueReplacee);

                CARAPI ToString(
                    /* [out] */ String* str);

            public:
                AutoPtr<IMapEntry> mMapEntry;
            };

            class Iterator
                : public IIterator
                , public ElRefBase
            {
            public:
                Iterator(
                    /* [in] */ UnmodifiableEntrySet* owner);

                CAR_INTERFACE_DECL()

                CARAPI HasNext(
                    /* [out] */ Boolean* result);

                CARAPI Next(
                    /* [out] */ IMapEntry** object);

                CARAPI Next(
                    /* [out] */ IInterface** object);

                CARAPI Remove();
            public:
                AutoPtr<Iterator> mIterator;
            };

        public:
            UnmodifiableEntrySet(
                /* [in] */ ISet* set);

            CARAPI GetIterator(
                /* [out] */ IIterator** result);

            CARAPI ToArray(
                /* [out, callee] */ ArrayOf<IInterface*>** array);

            CARAPI ToArrayEx(
                /* [in] */ ArrayOf<IInterface*>* inArray,
                /* [out, callee] */ ArrayOf<IInterface*>** outArray);

        };

    public:
        UnmodifiableMap(
            /* [in] */ IMap* map);

        CAR_INTERFACE_DECL()

        CARAPI Clear();

        CARAPI ContainsKey(
            /* [in] */ IInterface* key,
            /* [out] */ Boolean* result);

        CARAPI ContainsValue(
            /* [in] */ IInterface* value,
            /* [out] */ Boolean* result);

        CARAPI EntrySet(
            /* [out, callee] */ ISet** entries);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI Get(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI KeySet(
            /* [out, callee] */ ISet** keySet);

        CARAPI Put(
            /* [in] */ PInterface key,
            /* [in] */ PInterface value,
            /* [out] */ PInterface* oldValue);

        CARAPI PutAll(
            /* [in] */ IMap* map);

        CARAPI Remove(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI Values(
            /* [out] */ ICollection** value);

        CARAPI ToString(
            /* [out] */ String* str);
        AutoPtr<IMap> mM;
    };

    class UnmodifiableSortedMap
        : public UnmodifiableMap
        , public ISortedMap
    {
    public:
        UnmodifiableSortedMap(
            /* [in] */ ISortedMap* map);

        CAR_INTERFACE_DECL()

        CARAPI Comparator(
            /* [out] */ IComparator** comp);

        CARAPI FirstKey(
            /* [out] */ IInterface** outface);

        CARAPI HeadMap(
            /* [in] */ IInterface* endKey,
            /* [out] */ ISortedMap** sortmap);

        CARAPI LastKey(
            /* [out] */ IInterface** outface);

        CARAPI SubMap(
            /* [in] */ IInterface* startKey,
            /* [in] */ IInterface* endKey,
            /* [out] */ ISortedMap** sortmap);

        CARAPI TailMap(
            /* [in] */ IInterface* startKey,
            /* [out] */ ISortedMap** sortmap);

        CARAPI Clear();

        CARAPI ContainsKey(
            /* [in] */ IInterface* key,
            /* [out] */ Boolean* result);

        CARAPI ContainsValue(
            /* [in] */ IInterface* value,
            /* [out] */ Boolean* result);

        CARAPI EntrySet(
            /* [out, callee] */ ISet** entries);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI Get(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI KeySet(
            /* [out, callee] */ ISet** keySet);

        CARAPI Put(
            /* [in] */ PInterface key,
            /* [in] */ PInterface value,
            /* [out] */ PInterface* oldValue);

        CARAPI PutAll(
            /* [in] */ IMap* map);

        CARAPI Remove(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI Values(
            /* [out] */ ICollection** value);
        AutoPtr<ISortedMap> mSm;
    };

    class UnmodifiableSortedSet
        : public UnmodifiableSet
        , public ISortedSet
    {
    public:
        UnmodifiableSortedSet(
            /* [in] */ ISortedSet* set);

        CAR_INTERFACE_DECL()

        CARAPI Comparator(
            /* [out] */ IComparator** outcom);

        CARAPI First(
            /* [out] */ IInterface** outface);

        CARAPI HeadSet(
            /* [in] */ IInterface* end,
            /* [out] */ ISortedSet** outsort);

        CARAPI Last(
            /* [out] */ IInterface** outface);

        CARAPI SubSet(
            /* [in] */ IInterface* start,
            /* [in] */ IInterface* end,
            /* [out] */ ISortedSet** outsort);

        CARAPI TailSet(
            /* [in] */ IInterface* start,
            /* [out] */ ISortedSet** outsort);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);
        AutoPtr<ISortedSet> mSs;
    };

    class SetFromMap
        : public AbstractSet
        , public ISet
        , public ISerializable
        , public ElRefBase
    {
    public:
        SetFromMap(
            /* [in] */ IMap* map);

        CAR_INTERFACE_DECL()

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* value);

        CARAPI GetHashCode(
            /* [out] */ Int32* value);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

        CARAPI ToString(
            /* [out] */ String* str);

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* contents,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

        CARAPI GetIterator(
            /* [out] */ IIterator** result);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

    private:
        CARAPI ReadObject(
            /* [in] */ IObjectInputStream* stream);
        AutoPtr<IMap> mM;
        AutoPtr<ISet> mBackingSet;
    };

    class AsLIFOQueue
        : public IQueue
        , public AbstractQueue
        , public ISerializable
        , public ElRefBase
    {
    public:
        AsLIFOQueue(
            /* [in] */ IDeque* deque);

        CAR_INTERFACE_DECL()

        CARAPI GetIterator(
            /* [out] */ IIterator** result);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI Offer(
            /* [in] */ IInterface* obj,
            /* [out] */ Boolean* result);

        CARAPI Peek(
            /* [out] */ IInterface** obj);

        CARAPI Poll(
            /* [out] */ IInterface** obj);

        CARAPI Add(
            /* [in] */ IInterface* obj,
            /* [out] */ Boolean* result);

        CARAPI Clear();

        CARAPI Element(
            /* [out] */ IInterface** object);

        CARAPI RemoveEx(
            /* [out] */ IInterface** obj);

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* contents,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

        CARAPI ToString(
            /* [out] */ String* result);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        AutoPtr<IDeque> mQ;
    };

    class CheckedCollection
        : public ICollection
        , public ISerializable
        , public ElRefBase
    {
    public:
       CheckedCollection(
            /* [in] */ ICollection* c,
            /* [in] */ InterfaceID type);

        CAR_INTERFACE_DECL()

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetIterator(
            /* [out] */ IIterator** result);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

        CARAPI ToString(
            /* [out] */ String* result);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

    public:
        AutoPtr<ICollection> mC;
        InterfaceID mType;
    };

    class CheckedListIterator
        : public IListIterator
        , public ElRefBase
    {
    public:
        CheckedListIterator(
            /* [in] */ IListIterator* i,
            /* [in] */ InterfaceID type);

        CAR_INTERFACE_DECL()

        CARAPI HasNext(
            /* [out] */ Boolean* result);

        CARAPI Next(
            /* [out] */ IInterface** object);

        CARAPI Remove();

        CARAPI HasPrevious(
            /* [out] */ Boolean* result);

        CARAPI Previous(
            /* [out] */ IInterface** object);

        CARAPI NextIndex(
            /* [out] */ Int32* index);

        CARAPI PreviousIndex(
            /* [out] */ Int32* index);

        CARAPI Set(
            /* [in] */ IInterface* object);

        CARAPI Add(
            /* [in] */ IInterface* object);

    private:
        AutoPtr<IListIterator> mI;
        InterfaceID mType;
    };

    class CheckedList
        : public CheckedCollection
        , public IList
    {
    public:
       CheckedList(
            /* [in] */ IList* l,
            /* [in] */ InterfaceID type);

        CAR_INTERFACE_DECL()

        CARAPI AddAllEx(
            /* [in] */ Int32 location,
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Get(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** object);

        CARAPI Set(
            /* [in] */ Int32 location,
            /* [in] */ IInterface* object,
            /* [out] */ IInterface** prevObject);

        CARAPI AddEx(
            /* [in] */ Int32 location,
            /* [in] */ IInterface* object);

        CARAPI RemoveEx(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** object);

        CARAPI IndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* index);

        CARAPI LastIndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* index);

        CARAPI GetListIterator(
            /* [out] */ IListIterator** it);

        CARAPI GetListIteratorEx(
            /* [in] */ Int32 location,
            /* [out] */ IListIterator** it);

        CARAPI SubList(
            /* [in] */ Int32 start,
            /* [in] */ Int32 end,
            /* [out] */ IList** subList);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    public:
        AutoPtr<IList> mL;
    };

    /**
     * A dynamically typesafe view of a RandomAccessList.
     */
    class CheckedRandomAccessList
        : public CheckedList
        , public IRandomAccess
    {
    public:
        CheckedRandomAccessList(
            /* [in] */ IList* l,
            /* [in] */ InterfaceID type);

        CAR_INTERFACE_DECL()
    };

    /**
     * A dynamically typesafe view of a Set.
     */
    class CheckedSet
        : public CheckedCollection
        , public ISet
    {
    public:
       CheckedSet(
            /* [in] */ ISet* s,
            /* [in] */ InterfaceID type);

        CAR_INTERFACE_DECL()

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);
    };

    /**
     * A dynamically typesafe view of a Map.
     */
    class CheckedMap
        : public IMap
        , public ISerializable
        , public ElRefBase
    {
    private:
        /**
         * A dynamically typesafe view of a Map.Entry.
         */
        class CheckedEntry
            : public IMapEntry
            , public ElRefBase
        {
        public:
            CheckedEntry(
                /* [in] */ IMapEntry* e,
                /* [in] */ InterfaceID valueType);

            CAR_INTERFACE_DECL()

            CARAPI GetKey(
                /* [out] */ PInterface* key);

            CARAPI GetValue(
                /* [out] */ PInterface* value);

            CARAPI SetValue(
                /* [in] */ IInterface* valueReplacer,
                /* [out] */ IInterface** valueReplacee);

            CARAPI Equals(
                /* [in] */ IInterface* entry,
                /* [out] */ Boolean* result);

            CARAPI GetHashCode(
                /* [out] */ Int32* hashCode);

        public:
            AutoPtr<IMapEntry> mE;
            InterfaceID mValueType;
        };
        /**
         * A dynamically typesafe view of an entry set.
         */
        class CheckedEntrySet
            : public ISet
            , public ElRefBase
        {
        private:
            class CheckedEntryIterator
                : public IIterator
                , public ElRefBase
            {
            public:
                CheckedEntryIterator(
                    /* [in] */ IIterator* i,
                    /* [in] */ InterfaceID valueType);

                CAR_INTERFACE_DECL()

                CARAPI HasNext(
                    /* [out] */ Boolean* result);

                CARAPI Remove();

                CARAPI Next(
                    /* [out] */ IMapEntry** object);

                CARAPI Next(
                    /* [out] */ IInterface** object);

            public:
                AutoPtr<IIterator> mI;
                InterfaceID mValueType;
            };
        public:
            CheckedEntrySet(
                /* [in] */ ISet* s,
                /* [in] */ InterfaceID valueType);

            CAR_INTERFACE_DECL()

            CARAPI GetIterator(
                /* [out] */ IIterator** result);

            CARAPI ToArray(
                /* [out, callee] */ ArrayOf<IInterface*>** array);

            CARAPI ToArrayEx(
                /* [in] */ ArrayOf<IInterface*>* inArray,
                /* [out, callee] */ ArrayOf<IInterface*>** outArray);

            CARAPI RetainAll(
                /* [in] */ ICollection* collection,
                /* [out] */ Boolean* modified);

            CARAPI RemoveAll(
                /* [in] */ ICollection* collection,
                /* [out] */ Boolean* modified);

            CARAPI ContainsAll(
                /* [in] */ ICollection* collection,
                /* [out] */ Boolean* result);

            CARAPI AddAll(
                /* [in] */ ICollection* collection,
                /* [out] */ Boolean* modified);

            CARAPI Remove(
                /* [in] */ IInterface* object,
                /* [out] */ Boolean* modified);

            CARAPI Contains(
                /* [in] */ IInterface* object,
                /* [out] */ Boolean* result);

            CARAPI Add(
                /* [in] */ IInterface* object,
                /* [out] */ Boolean* modified);

            CARAPI IsEmpty(
                /* [out] */ Boolean* result);

            CARAPI Clear();

            CARAPI GetSize(
                /* [out] */ Int32* size);

            CARAPI GetHashCode(
                /* [out] */ Int32* hashCode);

            CARAPI Equals(
                /* [in] */ IInterface* object,
                /* [out] */ Boolean* result);
        public:
            AutoPtr<ISet> mS;
            InterfaceID mValueType;
        };

    public:
        CheckedMap(
            /* [in] */ IMap* m,
            /* [in] */ InterfaceID keyType,
            /* [in] */ InterfaceID valueType);

        CAR_INTERFACE_DECL()

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI ContainsKey(
            /* [in] */ IInterface* key,
            /* [out] */ Boolean* result);

        CARAPI ContainsValue(
            /* [in] */ IInterface* value,
            /* [out] */ Boolean* result);

        CARAPI Get(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI Put(
            /* [in] */ PInterface key,
            /* [in] */ PInterface value,
            /* [out] */ PInterface* oldValue);

        CARAPI Remove(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI PutAll(
            /* [in] */ IMap* map);

        CARAPI Clear();

        CARAPI KeySet(
            /* [out, callee] */ ISet** keySet);

        CARAPI Values(
            /* [out] */ ICollection** value);

        CARAPI EntrySet(
            /* [out, callee] */ ISet** entries);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        CARAPI ToString(
            /* [out] */ String* result);

    public:
        AutoPtr<IMap> mM;
        InterfaceID mKeyType;
        InterfaceID mValueType;
    };

    /**
     * A dynamically typesafe view of a SortedSet.
     */
    class CheckedSortedSet
        : public CheckedSet
        , public ISortedSet
    {
    public:
        CheckedSortedSet(
            /* [in] */ ISortedSet* s,
            /* [in] */ InterfaceID type);

        CAR_INTERFACE_DECL()

        CARAPI Comparator(
            /* [out] */ IComparator** outcom);

        CARAPI SubSet(
            /* [in] */ IInterface* start,
            /* [in] */ IInterface* end,
            /* [out] */ ISortedSet** outsort);

        CARAPI HeadSet(
            /* [in] */ IInterface* end,
            /* [out] */ ISortedSet** outsort);

        CARAPI TailSet(
            /* [in] */ IInterface* start,
            /* [out] */ ISortedSet** outsort);

        CARAPI First(
            /* [out] */ IInterface** outface);

        CARAPI Last(
            /* [out] */ IInterface** outface);

        CARAPI GetIterator(
            /* [out] */ IIterator** result);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArrayEx(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

        CARAPI RetainAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI RemoveAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI ContainsAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* result);

        CARAPI AddAll(
            /* [in] */ ICollection* collection,
            /* [out] */ Boolean* modified);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI Add(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Clear();

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);
        AutoPtr<ISortedSet> mSs;
    };

    /**
     * A dynamically typesafe view of a SortedMap.
     */
    class CheckedSortedMap
        : public CheckedMap
        , public ISortedMap
    {
    public:
       CheckedSortedMap(
            /* [in] */ ISortedMap* m,
            /* [in] */ InterfaceID keyType,
            /* [in] */ InterfaceID valueType);

        CAR_INTERFACE_DECL()

        CARAPI Comparator(
            /* [out] */ IComparator** comp);

        CARAPI SubMap(
            /* [in] */ IInterface* startKey,
            /* [in] */ IInterface* endKey,
            /* [out] */ ISortedMap** sortmap);

        CARAPI HeadMap(
            /* [in] */ IInterface* endKey,
            /* [out] */ ISortedMap** sortmap);

        CARAPI TailMap(
            /* [in] */ IInterface* startKey,
            /* [out] */ ISortedMap** sortmap);

        CARAPI FirstKey(
            /* [out] */ IInterface** outface);

        CARAPI LastKey(
            /* [out] */ IInterface** outface);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI ContainsKey(
            /* [in] */ IInterface* key,
            /* [out] */ Boolean* result);

        CARAPI ContainsValue(
            /* [in] */ IInterface* value,
            /* [out] */ Boolean* result);

        CARAPI Get(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI Put(
            /* [in] */ PInterface key,
            /* [in] */ PInterface value,
            /* [out] */ PInterface* oldValue);

        CARAPI Remove(
            /* [in] */ PInterface key,
            /* [out] */ PInterface* value);

        CARAPI PutAll(
            /* [in] */ IMap* map);

        CARAPI Clear();

        CARAPI KeySet(
            /* [out, callee] */ ISet** keySet);

        CARAPI Values(
            /* [out] */ ICollection** value);

        CARAPI EntrySet(
            /* [out, callee] */ ISet** entries);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);

    public:
        AutoPtr<ISortedMap> mSm;
    };

public:
    CARAPI BinarySearch(
        /* [in] */ IList* list,
        /* [in] */ IInterface* object,
        /* [out] */ Int32* index);

    CARAPI BinarySearchEx(
        /* [in] */ IList* list,
        /* [in] */ IInterface* object,
        /* [in] */ IComparator* comparator,
        /* [out] */ Int32* index);

    CARAPI Copy(
        /* [in] */ IList* destination,
        /* [in] */ IList* source);

    CARAPI NewEnumeration(
        /* [in] */ ICollection* collection,
        /* [out] */ IEnumeration** result);

    CARAPI Fill(
        /* [in] */ IList* list,
        /* [in] */ IInterface* object);

    CARAPI Max(
        /* [in] */ ICollection* collection,
        /* [out] */ IInterface** result);

    CARAPI MaxEx(
        /* [in] */ ICollection* collection,
        /* [in] */ IComparator* comparator,
        /* [out] */ IInterface** result);

    CARAPI Min(
        /* [in] */ ICollection* collection,
        /* [out] */ IInterface** result);

    CARAPI MinEx(
        /* [in] */ ICollection* collection,
        /* [in] */ IComparator* comparator,
        /* [out] */ IInterface** result);

    CARAPI NCopies(
        /* [in] */ Int32 length,
        /* [in] */ IInterface* object,
        /* [out] */ IList** result);

    CARAPI Reverse(
        /* [in] */ IList* list);

    CARAPI ReverseOrder(
        /* [out] */ IComparator** result);

    CARAPI ReverseOrderEx(
        /* [in] */ IComparator* c,
        /* [out] */ IComparator** result);

    CARAPI Shuffle(
        /* [in] */ IList* list);

    CARAPI ShuffleEx(
        /* [in] */ IList* list,
        /* [in] */ IRandom* random);

    CARAPI NewSingleton(
        /* [in] */ IInterface* object,
        /* [out] */ ISet** out);

    CARAPI NewSingletonList(
        /* [in] */ IInterface* object,
        /* [out] */ IList** result);

    CARAPI NewSingletonMap(
        /* [in] */ IInterface* key,
        /* [in] */ IInterface* value,
        /* [out] */ IMap** result);

    CARAPI Sort(
        /* [in] */ IList* list);

    CARAPI SortEx(
        /* [in] */ IList* list,
        /* [in] */ IComparator* comparator);

    CARAPI Swap(
        /* [in] */ IList* list,
        /* [in] */ Int32 index1,
        /* [in] */ Int32 index2);

    CARAPI ReplaceAll(
        /* [in] */ IList* list,
        /* [in] */ IInterface* obj,
        /* [in] */ IInterface* obj2,
        /* [out] */ Boolean* result);

    CARAPI Rotate(
        /* [in] */ IList* lst,
        /* [in] */ Int32 dist);

    CARAPI IndexOfSubList(
        /* [in] */ IList* list,
        /* [in] */ IList* sublist,
        /* [out] */ Int32* index);

    CARAPI LastIndexOfSubList(
        /* [in] */ IList* list,
        /* [in] */ IList* sublist,
        /* [out] */ Int32* index);

    CARAPI  NewList(
        /* [in] */ IEnumeration* enumeration,
        /* [out] */ IArrayList** result);

    CARAPI  NewSynchronizedCollection(
        /* [in] */ ICollection* collection,
        /* [out] */ ICollection** result);

    CARAPI  NewSynchronizedList(
        /* [in] */ IList* list,
        /* [out] */ IList** result);

    CARAPI  NewSynchronizedMap(
        /* [in] */ IMap* map,
        /* [out] */ IMap** result);

    CARAPI  NewSynchronizedSet(
        /* [in] */ ISet* set,
        /* [out] */ ISet** result);

    CARAPI  NewSynchronizedSortedMap(
        /* [in] */ ISortedMap* map,
        /* [out] */ ISortedMap** result);

    CARAPI  NewSynchronizedSortedSet(
        /* [in] */ ISortedSet* set,
        /* [out] */ ISortedSet** result);

    CARAPI  NewUnmodifiableCollection(
        /* [in] */ ICollection* collection,
        /* [out] */ ICollection** result);

    CARAPI  NewUnmodifiableList(
        /* [in] */ IList* list,
        /* [out] */ IList** result);

    CARAPI  NewUnmodifiableMap(
        /* [in] */ IMap* map,
        /* [out] */ IMap** result);

    CARAPI  NewUnmodifiableSet(
        /* [in] */ ISet* set,
        /* [out] */ ISet** result);

    CARAPI  NewUnmodifiableSortedMap(
        /* [in] */ ISortedMap* map,
        /* [out] */ ISortedMap** result);

    CARAPI  NewUnmodifiableSortedSet(
        /* [in] */ ISortedSet* set,
        /* [out] */ ISortedSet** result);

    CARAPI Frequency(
        /* [in] */ ICollection* c,
        /* [in] */ IInterface* o,
        /* [out] */ Int32* result);

    CARAPI GetEmptyList(
        /* [out] */ IList** result);

    CARAPI GetEmptySet(
        /* [out] */ ISet** result);

    CARAPI GetEmptyMap(
        /* [out] */ IMap** result);

    CARAPI GetEmptyEnumeration(
        /* [out] */ IEnumeration** result);

    CARAPI GetEmptyIterator(
        /* [out] */ IIterator** result);

    CARAPI GetEmptyListIterator(
        /* [out] */ IListIterator** result);

    CARAPI NewCheckedCollection(
        /* [in] */ ICollection* c,
        /* [in] */ const InterfaceID& type,
        /* [out] */ ICollection** result);

    CARAPI NewCheckedMap(
        /* [in] */ IMap* m,
        /* [in] */ const InterfaceID& keyType,
        /* [in] */ const InterfaceID& valueType,
        /* [out] */ IMap** result);

    CARAPI NewCheckedList(
        /* [in] */ IList* list,
        /* [in] */ const InterfaceID& type,
        /* [out] */ IList** result);

    CARAPI NewCheckedSet(
        /* [in] */ ISet* s,
        /* [in] */ const InterfaceID& type,
        /* [out] */ ISet** result);

    CARAPI NewCheckedSortedMap(
        /* [in] */ ISortedMap* m,
        /* [in] */ const InterfaceID& keyType,
        /* [in] */ const InterfaceID& valueType,
        /* [out] */ ISortedMap** result);

    CARAPI NewCheckedSortedSet(
        /* [in] */ ISortedSet* s,
        /* [in] */ const InterfaceID& type,
        /* [out] */ ISortedSet** result);

    CARAPI AddAll(
        /* [in] */ ICollection* c,
        /* [in] */ ArrayOf<IInterface*>* a,
        /* [out] */ Boolean* result);

    CARAPI Disjoint(
        /* [in] */ ICollection* c1,
        /* [in] */ ICollection* c2,
        /* [out] */ Boolean* result);

    CARAPI CheckType(
        /* [in] */ IInterface* obj,
        /* [in] */ const InterfaceID& type);

    CARAPI NewSetFromMap(
        /* [in] */ IMap* map,
        /* [out] */ ISet** result);

    CARAPI NewAsLifoQueue(
        /* [in] */ IDeque* deque,
        /* [out] */ IQueue** result);

public:
    static CARAPI _BinarySearch(
        /* [in] */ IList* list,
        /* [in] */ IInterface* object,
        /* [out] */ Int32* index);

    static CARAPI _BinarySearchEx(
        /* [in] */ IList* list,
        /* [in] */ IInterface* object,
        /* [in] */ IComparator* comparator,
        /* [out] */ Int32* index);

    static CARAPI _Copy(
        /* [in] */ IList* destination,
        /* [in] */ IList* source);

    static CARAPI _NewEnumeration(
        /* [in] */ ICollection* collection,
        /* [out] */ IEnumeration** result);

    static CARAPI _Fill(
        /* [in] */ IList* list,
        /* [in] */ IInterface* object);

    static CARAPI _Max(
        /* [in] */ ICollection* collection,
        /* [out] */ IInterface** result);

    static CARAPI _MaxEx(
        /* [in] */ ICollection* collection,
        /* [in] */ IComparator* comparator,
        /* [out] */ IInterface** result);

    static CARAPI _Min(
        /* [in] */ ICollection* collection,
        /* [out] */ IInterface** result);

    static CARAPI _MinEx(
        /* [in] */ ICollection* collection,
        /* [in] */ IComparator* comparator,
        /* [out] */ IInterface** result);

    static CARAPI _NCopies(
        /* [in] */ Int32 length,
        /* [in] */ IInterface* object,
        /* [out] */ IList** result);

    static CARAPI _Reverse(
        /* [in] */ IList* list);

    static CARAPI _ReverseOrder(
        /* [out] */ IComparator** result);

    static CARAPI _ReverseOrderEx(
        /* [in] */ IComparator* c,
        /* [out] */ IComparator** result);

    static CARAPI _Shuffle(
        /* [in] */ IList* list);

    static CARAPI _ShuffleEx(
        /* [in] */ IList* list,
        /* [in] */ IRandom* random);

    static CARAPI _NewSingleton(
        /* [in] */ IInterface* object,
        /* [out] */ ISet** out);

    static CARAPI _NewSingletonList(
        /* [in] */ IInterface* object,
        /* [out] */ IList** result);

    static CARAPI _NewSingletonMap(
        /* [in] */ IInterface* key,
        /* [in] */ IInterface* value,
        /* [out] */ IMap** result);

    static CARAPI _Sort(
        /* [in] */ IList* list);

    static CARAPI _SortEx(
        /* [in] */ IList* list,
        /* [in] */ IComparator* comparator);

    static CARAPI _Swap(
        /* [in] */ IList* list,
        /* [in] */ Int32 index1,
        /* [in] */ Int32 index2);

    static CARAPI _ReplaceAll(
        /* [in] */ IList* list,
        /* [in] */ IInterface* obj,
        /* [in] */ IInterface* obj2,
        /* [out] */ Boolean* result);

    static CARAPI _Rotate(
        /* [in] */ IList* lst,
        /* [in] */ Int32 dist);

    static CARAPI _IndexOfSubList(
        /* [in] */ IList* list,
        /* [in] */ IList* sublist,
        /* [out] */ Int32* index);

    static CARAPI _LastIndexOfSubList(
        /* [in] */ IList* list,
        /* [in] */ IList* sublist,
        /* [out] */ Int32* index);

    static CARAPI _NewList(
        /* [in] */ IEnumeration* enumeration,
        /* [out] */ IArrayList** result);

    static CARAPI _NewSynchronizedCollection(
        /* [in] */ ICollection* collection,
        /* [out] */ ICollection** result);

    static CARAPI _NewSynchronizedList(
        /* [in] */ IList* list,
        /* [out] */ IList** result);

    static CARAPI _NewSynchronizedMap(
        /* [in] */ IMap* map,
        /* [out] */ IMap** result);

    static CARAPI _NewSynchronizedSet(
        /* [in] */ ISet* set,
        /* [out] */ ISet** result);

    static CARAPI _NewSynchronizedSortedMap(
        /* [in] */ ISortedMap* map,
        /* [out] */ ISortedMap** result);

    static CARAPI _NewSynchronizedSortedSet(
        /* [in] */ ISortedSet* set,
        /* [out] */ ISortedSet** result);

    static CARAPI _NewUnmodifiableCollection(
        /* [in] */ ICollection* collection,
        /* [out] */ ICollection** result);

    static CARAPI _NewUnmodifiableList(
        /* [in] */ IList* list,
        /* [out] */ IList** result);

    static CARAPI _NewUnmodifiableMap(
        /* [in] */ IMap* map,
        /* [out] */ IMap** result);

    static CARAPI _NewUnmodifiableSet(
        /* [in] */ ISet* set,
        /* [out] */ ISet** result);

    static CARAPI _NewUnmodifiableSortedMap(
        /* [in] */ ISortedMap* map,
        /* [out] */ ISortedMap** result);

    static CARAPI _NewUnmodifiableSortedSet(
        /* [in] */ ISortedSet* set,
        /* [out] */ ISortedSet** result);

    static CARAPI _Frequency(
        /* [in] */ ICollection* c,
        /* [in] */ IInterface* o,
        /* [out] */ Int32* result);

    static CARAPI _GetEmptyList(
        /* [out] */ IList** result);

    static CARAPI _GetEmptySet(
        /* [out] */ ISet** result);

    static CARAPI _GetEmptyMap(
        /* [out] */ IMap** result);

    static CARAPI _GetEmptyEnumeration(
        /* [out] */ IEnumeration** result);

    static CARAPI _GetEmptyIterator(
        /* [out] */ IIterator** result);

    static CARAPI _GetEmptyListIterator(
        /* [out] */ IListIterator** result);

    static CARAPI _NewCheckedCollection(
        /* [in] */ ICollection* c,
        /* [in] */ const InterfaceID& type,
        /* [out] */ ICollection** result);

    static CARAPI _NewCheckedMap(
        /* [in] */ IMap* m,
        /* [in] */ const InterfaceID& keyType,
        /* [in] */ const InterfaceID& valueType,
        /* [out] */ IMap** result);

    static CARAPI _NewCheckedList(
        /* [in] */ IList* list,
        /* [in] */ const InterfaceID& type,
        /* [out] */ IList** result);

    static CARAPI _NewCheckedSet(
        /* [in] */ ISet* s,
        /* [in] */ const InterfaceID& type,
        /* [out] */ ISet** result);

    static CARAPI _NewCheckedSortedMap(
        /* [in] */ ISortedMap* m,
        /* [in] */ const InterfaceID& keyType,
        /* [in] */ const InterfaceID& valueType,
        /* [out] */ ISortedMap** result);

    static CARAPI _NewCheckedSortedSet(
        /* [in] */ ISortedSet* s,
        /* [in] */ const InterfaceID& type,
        /* [out] */ ISortedSet** result);

    static CARAPI _AddAll(
        /* [in] */ ICollection* c,
        /* [in] */ ArrayOf<IInterface*>* a,
        /* [out] */ Boolean* result);

    static CARAPI _Disjoint(
        /* [in] */ ICollection* c1,
        /* [in] */ ICollection* c2,
        /* [out] */ Boolean* result);

    static CARAPI _CheckType(
        /* [in] */ IInterface* obj,
        /* [in] */ const InterfaceID& type);

    static CARAPI _NewSetFromMap(
        /* [in] */ IMap* map,
        /* [out] */ ISet** result);

    static CARAPI _NewAsLifoQueue(
        /* [in] */ IDeque* deque,
        /* [out] */ IQueue** result);
public:
    static const AutoPtr<IList> EMPTY_LIST;
    static const AutoPtr<ISet> EMPTY_SET;
    static const AutoPtr<IMap> EMPTY_MAP;

private:
    static const AutoPtr<IIterator> EMPTY_ITERATOR;
    static const AutoPtr<IEnumeration> EMPTY_ENUMERATION;
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_CCOLLECTIONS_H__
