#ifndef __UTILITY_ABSTRACTLIST_H__
#define __UTILITY_ABSTRACTLIST_H__

#include "AbstractCollection.h"

using Elastos::Utility::IIterator;
using Elastos::Utility::ICollection;
using Elastos::Utility::IList;
using Elastos::Utility::IListIterator;
using Elastos::Utility::IRandomAccess;

namespace Elastos {
namespace Utility {

class AbstractList : public AbstractCollection
{
private:
    class SimpleListIterator
        : public ElRefBase
        , public IIterator
    {
    public:
        SimpleListIterator(
            /* [in] */ AbstractList* owner);

        CAR_INTERFACE_DECL();

        CARAPI HasNext(
            /* [out] */ Boolean* result);

        CARAPI Next(
            /* [out] */ IInterface** object);

        CARAPI Remove();

    public:
        Int32 mPos;
        Int32 mExpectedModeCount;
        Int32 mLastPosition;
        AutoPtr<AbstractList> mOwner;
    };

    class FullListIterator
        : public SimpleListIterator
        , public IListIterator
    {
    public:
        FullListIterator(
            /* [in] */ Int32 start,
            /* [in] */ AbstractList* owner);

        CAR_INTERFACE_DECL();

        CARAPI Add(
            /* [in] */ IInterface* object);

        CARAPI HasPrevious(
            /* [out] */ Boolean* result);

        CARAPI NextIndex(
            /* [out] */ Int32* result);

        CARAPI Previous(
            /* [out] */ IInterface** object);

        CARAPI PreviousIndex(
            /* [out] */ Int32* result);

        CARAPI Set(
            /* [in] */ IInterface* object);

        CARAPI HasNext(
            /* [out] */ Boolean* result);

        CARAPI Next(
            /* [out] */ IInterface** object);

        CARAPI Remove();
    };

public:
    virtual CARAPI_(UInt32) AddRef() = 0;

    virtual CARAPI_(UInt32) Release() = 0;

    /**
     * Inserts the specified object into this List at the specified location.
     * The object is inserted before any previous element at the specified
     * location. If the location is equal to the size of this List, the object
     * is added at the end.
     * <p>
     * Concrete implementations that would like to support the add functionality
     * must override this method.
     *
     * @param location
     *            the index at which to insert.
     * @param object
     *            the object to add.
     *
     * @throws UnsupportedOperationException
     *                if adding to this List is not supported.
     * @throws ClassCastException
     *                if the class of the object is inappropriate for this
     *                List
     * @throws IllegalArgumentException
     *                if the object cannot be added to this List
     * @throws IndexOutOfBoundsException
     *                if {@code location < 0 || location > size()}
     */
    virtual CARAPI Add(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object);

    /**
     * Adds the specified object at the end of this List.
     *
     *
     * @param object
     *            the object to add
     * @return true
     *
     * @throws UnsupportedOperationException
     *                if adding to this List is not supported
     * @throws ClassCastException
     *                if the class of the object is inappropriate for this
     *                List
     * @throws IllegalArgumentException
     *                if the object cannot be added to this List
     */
    virtual CARAPI Add(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    /**
     * Inserts the objects in the specified Collection at the specified location
     * in this List. The objects are added in the order they are returned from
     * the collection's iterator.
     *
     * @param location
     *            the index at which to insert.
     * @param collection
     *            the Collection of objects
     * @return {@code true} if this List is modified, {@code false} otherwise.
     * @throws UnsupportedOperationException
     *             if adding to this list is not supported.
     * @throws ClassCastException
     *             if the class of an object is inappropriate for this list.
     * @throws IllegalArgumentException
     *             if an object cannot be added to this list.
     * @throws IndexOutOfBoundsException
     *             if {@code location < 0 || location > size()}
     */
    virtual CARAPI AddAll(
        /* [in] */ Int32 location,
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    /**
     * Removes all elements from this list, leaving it empty.
     *
     * @throws UnsupportedOperationException
     *             if removing from this list is not supported.
     * @see List#isEmpty
     * @see List#size
     */
    virtual CARAPI Clear();

    /**
     * Compares the specified object to this list and return true if they are
     * equal. Two lists are equal when they both contain the same objects in the
     * same order.
     *
     * @param object
     *            the object to compare to this object.
     * @return {@code true} if the specified object is equal to this list,
     *         {@code false} otherwise.
     * @see #hashCode
     */
    virtual CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    /**
     * Returns the element at the specified location in this list.
     *
     * @param location
     *            the index of the element to return.
     * @return the element at the specified index.
     * @throws IndexOutOfBoundsException
     *             if {@code location < 0 || location >= size()}
     */
    virtual CARAPI Get(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object) = 0;

    /**
     * Returns the hash code of this list. The hash code is calculated by taking
     * each element's hashcode into account.
     *
     * @return the hash code.
     * @see #equals
     * @see List#hashCode()
     */
    virtual CARAPI GetHashCode(
        /* [out] */ Int32* result);

    /**
     * Searches this list for the specified object and returns the index of the
     * first occurrence.
     *
     * @param object
     *            the object to search for.
     * @return the index of the first occurrence of the object, or -1 if it was
     *         not found.
     */
    virtual CARAPI IndexOf(
        /* [in] */ IInterface* object,
        /* [out] */ Int32* result);

    /**
     * Returns an iterator on the elements of this list. The elements are
     * iterated in the same order as they occur in the list.
     *
     * @return an iterator on the elements of this list.
     * @see Iterator
     */
    virtual CARAPI GetIterator(
        /* [out] */ IIterator** it);

    /**
     * Searches this list for the specified object and returns the index of the
     * last occurrence.
     *
     * @param object
     *            the object to search for.
     * @return the index of the last occurrence of the object, or -1 if the
     *         object was not found.
     */
    virtual CARAPI LastIndexOf(
        /* [in] */ IInterface* object,
        /* [out] */ Int32* result);

    /**
     * Returns a ListIterator on the elements of this list. The elements are
     * iterated in the same order that they occur in the list.
     *
     * @return a ListIterator on the elements of this list
     * @see ListIterator
     */
    virtual CARAPI GetListIterator(
        /* [out] */ IListIterator** listiterator);

    /**
     * Returns a list iterator on the elements of this list. The elements are
     * iterated in the same order as they occur in the list. The iteration
     * starts at the specified location.
     *
     * @param location
     *            the index at which to start the iteration.
     * @return a ListIterator on the elements of this list.
     * @throws IndexOutOfBoundsException
     *             if {@code location < 0 || location > size()}
     * @see ListIterator
     */
    virtual CARAPI GetListIterator(
        /* [in] */ Int32 location,
        /* [out] */ IListIterator** listiterator);

    /**
     * Removes the object at the specified location from this list.
     *
     * @param location
     *            the index of the object to remove.
     * @return the removed object.
     * @throws UnsupportedOperationException
     *             if removing from this list is not supported.
     * @throws IndexOutOfBoundsException
     *             if {@code location < 0 || location >= size()}
     */
    virtual CARAPI Remove(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    /**
     * Replaces the element at the specified location in this list with the
     * specified object.
     *
     * @param location
     *            the index at which to put the specified object.
     * @param object
     *            the object to add.
     * @return the previous element at the index.
     * @throws UnsupportedOperationException
     *             if replacing elements in this list is not supported.
     * @throws ClassCastException
     *             if the class of an object is inappropriate for this list.
     * @throws IllegalArgumentException
     *             if an object cannot be added to this list.
     * @throws IndexOutOfBoundsException
     *             if {@code location < 0 || location >= size()}
     */
    virtual CARAPI Set(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object);

    /**
     * Returns a part of consecutive elements of this list as a view. The
     * returned view will be of zero length if start equals end. Any change that
     * occurs in the returned subList will be reflected to the original list,
     * and vice-versa. All the supported optional operations by the original
     * list will also be supported by this subList.
     * <p>
     * This method can be used as a handy method to do some operations on a sub
     * range of the original list, for example
     * {@code list.subList(from, to).clear();}
     * <p>
     * If the original list is modified in other ways than through the returned
     * subList, the behavior of the returned subList becomes undefined.
     * <p>
     * The returned subList is a subclass of AbstractList. The subclass stores
     * offset, size of itself, and modCount of the original list. If the
     * original list implements RandomAccess interface, the returned subList
     * also implements RandomAccess interface.
     * <p>
     * The subList's set(int, Object), get(int), add(int, Object), remove(int),
     * addAll(int, Collection) and removeRange(int, int) methods first check the
     * bounds, adjust offsets and then call the corresponding methods of the
     * original AbstractList. addAll(Collection c) method of the returned
     * subList calls the original addAll(offset + size, c).
     * <p>
     * The listIterator(int) method of the subList wraps the original list
     * iterator. The iterator() method of the subList invokes the original
     * listIterator() method, and the size() method merely returns the size of
     * the subList.
     * <p>
     * All methods will throw a ConcurrentModificationException if the modCount
     * of the original list is not equal to the expected value.
     *
     * @param start
     *            start index of the subList (inclusive).
     * @param end
     *            end index of the subList, (exclusive).
     * @return a subList view of this list starting from {@code start}
     *         (inclusive), and ending with {@code end} (exclusive)
     * @throws IndexOutOfBoundsException
     *             if (start < 0 || end > size())
     * @throws IllegalArgumentException
     *             if (start > end)
     */
    virtual CARAPI SubList(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IList** list);

    /**
     * Removes the objects in the specified range from the start to the end
     * index minus one.
     *
     * @param start
     *            the index at which to start removing.
     * @param end
     *            the index after the last element to remove.
     * @throws UnsupportedOperationException
     *             if removing from this list is not supported.
     * @throws IndexOutOfBoundsException
     *             if {@code start < 0} or {@code start >= size()}.
     */
    virtual CARAPI RemoveRange(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    virtual CARAPI Set(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object,
        /* [out] */ IInterface** prevObject) = 0;

protected:
    /**
     * Constructs a new instance of this AbstractList.
     */
    AbstractList()
        : mModCount(0)
    {}

public:
    /**
     * A counter for changes to the list.
     */
    Int32 mModCount;
};

class SubAbstractList
    : public ElRefBase
    , public AbstractList
    , public IList
{
private:
    class SubAbstractListIterator
        : public ElLightRefBase
        , public IListIterator
    {
    public:
        SubAbstractListIterator(
            /* [in] */ IListIterator* it,
            /* [in] */ SubAbstractList* list,
            /* [in] */ Int32 offset,
            /* [in] */ Int32 length);

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
            /* [out] */ Int32* result);

        CARAPI Previous(
            /* [out] */ IInterface** object);

        CARAPI PreviousIndex(
            /* [out] */ Int32* result);

        CARAPI Remove();

        CARAPI Set(
            /* [in] */ IInterface* object);

    private:
        const AutoPtr<SubAbstractList> mSubList;

        const AutoPtr<IListIterator> mIterator;

        Int32 mStart;

        Int32 mEnd;
    };

public:
    SubAbstractList(
        /* [in] */ AbstractList* list,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CAR_INTERFACE_DECL();

    CARAPI Add(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object);

    CARAPI AddAll(
        /* [in] */ Int32 location,
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI AddAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI Get(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    CARAPI GetIterator(
        /* [out] */ IIterator** it);

    CARAPI GetListIterator(
        /* [in] */ Int32 location,
        /* [out] */ IListIterator** listiterator);

    CARAPI Remove(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    CARAPI Set(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* inobject,
        /* [out] */ IInterface** outobject);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI SizeChanged(
        /* [in] */ Boolean increment);

    CARAPI RemoveRange(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

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

    CARAPI  RetainAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI ToArray(
        /* [out, callee] */ ArrayOf<IInterface*>** array);

    CARAPI ToArray(
        /* [in] */ ArrayOf<IInterface*>* contents,
        /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI Add(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    CARAPI Clear();

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

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

    CARAPI SubList(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IList** subList);

private:
    const AutoPtr<AbstractList> mFulllist;

    Int32 mOffset;

    Int32 mSize;
};

class SubAbstractListRandomAccess
    : public SubAbstractList
    , public IRandomAccess
{
public:
    SubAbstractListRandomAccess(
        /* [in] */ AbstractList* list,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CAR_INTERFACE_DECL();
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_ABSTRACTLIST_H__
