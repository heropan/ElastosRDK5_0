
#ifndef __CCONCURRENTSKIPLISTSET_H__
#define __CCONCURRENTSKIPLISTSET_H__

#include "_CConcurrentSkipListSet.h"
#include "AbstractSet.h"

using Elastos::Core::IComparator;

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CConcurrentSkipListSet) , public AbstractSet
{
public:
    /**
     * Constructs a new, empty set that orders its elements according to
     * their {@linkplain Comparable natural ordering}.
     */
    CARAPI constructor();

    /**
     * Constructs a new, empty set that orders its elements according to
     * the specified comparator.
     *
     * @param comparator the comparator that will be used to order this set.
     *        If <tt>null</tt>, the {@linkplain Comparable natural
     *        ordering} of the elements will be used.
     */
    CARAPI constructor(
        /* [in] */ IComparator* comparator);

    /**
     * Constructs a new set containing the elements in the specified
     * collection, that orders its elements according to their
     * {@linkplain Comparable natural ordering}.
     *
     * @param c The elements that will comprise the new set
     * @throws ClassCastException if the elements in <tt>c</tt> are
     *         not {@link Comparable}, or are not mutually comparable
     * @throws NullPointerException if the specified collection or any
     *         of its elements are null
     */
    CARAPI constructor(
        /* [in] */ ICollection* c);

    /**
     * Constructs a new set containing the same elements and using the
     * same ordering as the specified sorted set.
     *
     * @param s sorted set whose elements will comprise the new set
     * @throws NullPointerException if the specified sorted set or any
     *         of its elements are null
     */
    CARAPI constructor(
        /* [in] */ ISortedSet* s);

    /**
     * For use by submaps
     */
    CARAPI constructor(
        /* [in] */ INavigableMap* m);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Clone(
        /* [out] */ IInterface** res);

    /* ---------------- Set operations -------------- */

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    CARAPI Contains(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI Add(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    CARAPI Remove(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    CARAPI Clear();

    CARAPI GetIterator(
        /* [out] */ IIterator** it);

    CARAPI DescendingIterator(
        /* [out] */ IIterator** outiter);

    /* ---------------- AbstractSet Overrides -------------- */

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI RemoveAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* res);

    /* ---------------- Relational operations -------------- */

    CARAPI Lower(
        /* [in] */ IInterface* e,
        /* [out] */ IInterface** outface);

    CARAPI Floor(
        /* [in] */ IInterface* e,
        /* [out] */ IInterface** outface);

    CARAPI Ceiling(
        /* [in] */ IInterface* e,
        /* [out] */ IInterface** outface);

    CARAPI Higher(
        /* [in] */ IInterface* e,
        /* [out] */ IInterface** outface);

    CARAPI PollFirst(
        /* [out] */ IInterface** outface);

    CARAPI PollLast(
        /* [out] */ IInterface** outface);

    /* ---------------- SortedSet operations -------------- */

    CARAPI Comparator(
        /* [out] */ IComparator** outcom);

    CARAPI First(
        /* [out] */ IInterface** outface);

    CARAPI Last(
        /* [out] */ IInterface** outface);

    CARAPI SubSetEx(
        /* [in] */ IInterface* fromElement,
        /* [in] */ Boolean fromInclusive,
        /* [in] */ IInterface* toElement,
        /* [in] */ Boolean toInclusive,
        /* [out] */ INavigableSet** outnav);

    CARAPI HeadSetEx(
        /* [in] */ IInterface* toElement,
        /* [in] */ Boolean inclusive,
        /* [out] */ INavigableSet** outnav);

    CARAPI TailSetEx(
        /* [in] */ IInterface* fromElement,
        /* [in] */ Boolean inclusive,
        /* [out] */ INavigableSet** outnav);

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

    CARAPI DescendingSet(
        /* [out] */ INavigableSet** outnav);

    CARAPI AddAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI ContainsAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI RetainAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI ToArray(
        /* [out, callee] */ ArrayOf<IInterface*>** array);

    CARAPI ToArrayEx(
        /* [in] */ ArrayOf<IInterface*>* inArray,
        /* [out, callee] */ ArrayOf<IInterface*>** outArray);

private:
    // Support for resetting map in clone
    CARAPI_(void) SetMap(
        /* [in] */ INavigableMap* map);

private:
//    private static final long serialVersionUID = -2479143111061671589L;

    /**
     * The underlying map. Uses Boolean.TRUE as value for each
     * element.  This field is declared final for the sake of thread
     * safety, which entails some ugliness in clone()
     */
    AutoPtr<INavigableMap> mM;
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__CCONCURRENTSKIPLISTSET_H__