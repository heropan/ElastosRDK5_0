
#ifndef __CCOPYONWRITEARRAYLIST_H__
#define __CCOPYONWRITEARRAYLIST_H__

#include "_CCopyOnWriteArrayList.h"
#include "CopyOnWriteArrayList.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CCopyOnWriteArrayList), public CopyOnWriteArrayList
{
public:
    /**
     * Creates an empty instance.
     */
    CARAPI constructor();

    /**
     * Creates a new instance containing the elements of {@code collection}.
     */
    CARAPI constructor(
        /* [in] */ ICollection* collection);

    /**
     * Creates a new instance containing the elements of {@code array}.
     */
    CARAPI constructor(
        /* [in] */ ArrayOf<IInterface*>* array);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Attempts to add {@code object} to the contents of this
     * {@code Collection} (optional).
     *
     * After this method finishes successfully it is guaranteed that the object
     * is contained in the collection.
     *
     * If the collection was modified it returns {@code true}, {@code false} if
     * no changes were made.
     *
     * An implementation of {@code Collection} may narrow the set of accepted
     * objects, but it has to specify this in the documentation. If the object
     * to be added does not meet this restriction, then an
     * {@code IllegalArgumentException} is thrown.
     *
     * If a collection does not yet contain an object that is to be added and
     * adding the object fails, this method <i>must</i> throw an appropriate
     * unchecked Exception. Returning false is not permitted in this case
     * because it would violate the postcondition that the element will be part
     * of the collection after this method finishes.
     *
     * @param object
     *            the object to add.
     * @return {@code true} if this {@code Collection} is
     *         modified, {@code false} otherwise.
     *
     * @throws UnsupportedOperationException
     *                if adding to this {@code Collection} is not supported.
     * @throws ClassCastException
     *                if the class of the object is inappropriate for this
     *                collection.
     * @throws IllegalArgumentException
     *                if the object cannot be added to this {@code Collection}.
     * @throws NullPointerException
     *                if null elements cannot be added to the {@code Collection}.
     */
    CARAPI Add(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    /**
     * Attempts to add all of the objects contained in {@code Collection}
     * to the contents of this {@code Collection} (optional). If the passed {@code Collection}
     * is changed during the process of adding elements to this {@code Collection}, the
     * behavior is not defined.
     *
     * @param collection
     *            the {@code Collection} of objects.
     * @return {@code true} if this {@code Collection} is modified, {@code false}
     *         otherwise.
     * @throws UnsupportedOperationException
     *                if adding to this {@code Collection} is not supported.
     * @throws ClassCastException
     *                if the class of an object is inappropriate for this
     *                {@code Collection}.
     * @throws IllegalArgumentException
     *                if an object cannot be added to this {@code Collection}.
     * @throws NullPointerException
     *                if {@code collection} is {@code null}, or if it
     *                contains {@code null} elements and this {@code Collection} does
     *                not support such elements.
     */
    CARAPI AddAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    /**
     * Removes all elements from this {@code Collection}, leaving it empty (optional).
     *
     * @throws UnsupportedOperationException
     *                if removing from this {@code Collection} is not supported.
     *
     * @see #isEmpty
     * @see #size
     */
    CARAPI Clear();

    /**
     * Tests whether this {@code Collection} contains the specified object. Returns
     * {@code true} if and only if at least one element {@code elem} in this
     * {@code Collection} meets following requirement:
     * {@code (object==null ? elem==null : object.equals(elem))}.
     *
     * @param object
     *            the object to search for.
     * @return {@code true} if object is an element of this {@code Collection},
     *         {@code false} otherwise.
     * @throws ClassCastException
     *                if the object to look for isn't of the correct
     *                type.
     * @throws NullPointerException
     *                if the object to look for is {@code null} and this
     *                {@code Collection} doesn't support {@code null} elements.
     */
    CARAPI Contains(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    /**
     * Tests whether this {@code Collection} contains all objects contained in the
     * specified {@code Collection}. If an element {@code elem} is contained several
     * times in the specified {@code Collection}, the method returns {@code true} even
     * if {@code elem} is contained only once in this {@code Collection}.
     *
     * @param collection
     *            the collection of objects.
     * @return {@code true} if all objects in the specified {@code Collection} are
     *         elements of this {@code Collection}, {@code false} otherwise.
     * @throws ClassCastException
     *                if one or more elements of {@code collection} isn't of the
     *                correct type.
     * @throws NullPointerException
     *                if {@code collection} contains at least one {@code null}
     *                element and this {@code Collection} doesn't support {@code null}
     *                elements.
     * @throws NullPointerException
     *                if {@code collection} is {@code null}.
     */
    CARAPI ContainsAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    /**
     * Compares the argument to the receiver, and returns true if they represent
     * the <em>same</em> object using a class specific comparison.
     *
     * @param object
     *            the object to compare with this object.
     * @return {@code true} if the object is the same as this object and
     *         {@code false} if it is different from this object.
     * @see #hashCode
     */
    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    /**
     * Returns an integer hash code for the receiver. Objects which are equal
     * return the same value for this method.
     *
     * @return the receiver's hash.
     *
     * @see #equals
     */
    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    /**
     * Returns if this {@code Collection} contains no elements.
     *
     * @return {@code true} if this {@code Collection} has no elements, {@code false}
     *         otherwise.
     *
     * @see #size
     */
    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    /**
     * Removes one instance of the specified object from this {@code Collection} if one
     * is contained (optional). The element {@code elem} that is removed
     * complies with {@code (object==null ? elem==null : object.equals(elem)}.
     *
     * @param object
     *            the object to remove.
     * @return {@code true} if this {@code Collection} is modified, {@code false}
     *         otherwise.
     * @throws UnsupportedOperationException
     *                if removing from this {@code Collection} is not supported.
     * @throws ClassCastException
     *                if the object passed is not of the correct type.
     * @throws NullPointerException
     *                if {@code object} is {@code null} and this {@code Collection}
     *                doesn't support {@code null} elements.
     */
    CARAPI Remove(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    /**
     * Removes all occurrences in this {@code Collection} of each object in the
     * specified {@code Collection} (optional). After this method returns none of the
     * elements in the passed {@code Collection} can be found in this {@code Collection}
     * anymore.
     *
     * @param collection
     *            the collection of objects to remove.
     * @return {@code true} if this {@code Collection} is modified, {@code false}
     *         otherwise.
     *
     * @throws UnsupportedOperationException
     *                if removing from this {@code Collection} is not supported.
     * @throws ClassCastException
     *                if one or more elements of {@code collection}
     *                isn't of the correct type.
     * @throws NullPointerException
     *                if {@code collection} contains at least one
     *                {@code null} element and this {@code Collection} doesn't support
     *                {@code null} elements.
     * @throws NullPointerException
     *                if {@code collection} is {@code null}.
     */
    CARAPI RemoveAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    /**
     * Removes all objects from this {@code Collection} that are not also found in the
     * {@code Collection} passed (optional). After this method returns this {@code Collection}
     * will only contain elements that also can be found in the {@code Collection}
     * passed to this method.
     *
     * @param collection
     *            the collection of objects to retain.
     * @return {@code true} if this {@code Collection} is modified, {@code false}
     *         otherwise.
     * @throws UnsupportedOperationException
     *                if removing from this {@code Collection} is not supported.
     * @throws ClassCastException
     *                if one or more elements of {@code collection}
     *                isn't of the correct type.
     * @throws NullPointerException
     *                if {@code collection} contains at least one
     *                {@code null} element and this {@code Collection} doesn't support
     *                {@code null} elements.
     * @throws NullPointerException
     *                if {@code collection} is {@code null}.
     */
    CARAPI RetainAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    /**
     * Returns a count of how many objects this {@code Collection} contains.
     *
     * @return how many objects this {@code Collection} contains, or Integer.MAX_VALUE
     *         if there are more than Integer.MAX_VALUE elements in this
     *         {@code Collection}.
     */
    CARAPI GetSize(
        /* [out] */ Int32* size);

    /**
     * Returns a new array containing all elements contained in this {@code Collection}.
     *
     * If the implementation has ordered elements it will return the element
     * array in the same order as an iterator would return them.
     *
     * The array returned does not reflect any changes of the {@code Collection}. A new
     * array is created even if the underlying data structure is already an
     * array.
     *
     * @return an array of the elements from this {@code Collection}.
     */
    CARAPI ToArray(
        /* [out, callee] */ ArrayOf<IInterface*>** array);

    /**
     * Returns an array containing all elements contained in this {@code Collection}. If
     * the specified array is large enough to hold the elements, the specified
     * array is used, otherwise an array of the same type is created. If the
     * specified array is used and is larger than this {@code Collection}, the array
     * element following the {@code Collection} elements is set to null.
     *
     * If the implementation has ordered elements it will return the element
     * array in the same order as an iterator would return them.
     *
     * {@code toArray(new Object[0])} behaves exactly the same way as
     * {@code toArray()} does.
     *
     * @param array
     *            the array.
     * @return an array of the elements from this {@code Collection}.
     *
     * @throws ArrayStoreException
     *                if the type of an element in this {@code Collection} cannot be
     *                stored in the type of the specified array.
     */
    CARAPI ToArrayEx(
        /* [in] */ ArrayOf<IInterface*>* inArray,
        /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    /**
     * Returns an {@link Iterator} for the elements in this object.
     *
     * @return An {@code Iterator} instance.
     */
    CARAPI GetIterator(
        /* [out] */ IIterator** it);

    /**
     * Inserts the specified object into this {@code List} at the specified location.
     * The object is inserted before the current element at the specified
     * location. If the location is equal to the size of this {@code List}, the object
     * is added at the end. If the location is smaller than the size of this
     * {@code List}, then all elements beyond the specified location are moved by one
     * position towards the end of the {@code List}.
     *
     * @param location
     *            the index at which to insert.
     * @param object
     *            the object to add.
     * @throws UnsupportedOperationException
     *                if adding to this {@code List} is not supported.
     * @throws ClassCastException
     *                if the class of the object is inappropriate for this
     *                {@code List}.
     * @throws IllegalArgumentException
     *                if the object cannot be added to this {@code List}.
     * @throws IndexOutOfBoundsException
     *                if {@code location < 0 || location > size()}
     */
    CARAPI AddEx(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object);

    /**
     * Inserts the objects in the specified collection at the specified location
     * in this {@code List}. The objects are added in the order they are returned from
     * the collection's iterator.
     *
     * @param location
     *            the index at which to insert.
     * @param collection
     *            the collection of objects to be inserted.
     * @return true if this {@code List} has been modified through the insertion, false
     *         otherwise (i.e. if the passed collection was empty).
     * @throws UnsupportedOperationException
     *                if adding to this {@code List} is not supported.
     * @throws ClassCastException
     *                if the class of an object is inappropriate for this
     *                {@code List}.
     * @throws IllegalArgumentException
     *                if an object cannot be added to this {@code List}.
     * @throws IndexOutOfBoundsException
     *                if {@code location < 0 || location > size()}
     */
    CARAPI AddAllEx(
        /* [in] */ Int32 location,
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    /**
     * Returns the element at the specified location in this {@code List}.
     *
     * @param location
     *            the index of the element to return.
     * @return the element at the specified location.
     * @throws IndexOutOfBoundsException
     *                if {@code location < 0 || location >= size()}
     */
    CARAPI Get(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    /**
     * Searches this {@code List} for the specified object and returns the index of the
     * first occurrence.
     *
     * @param object
     *            the object to search for.
     * @return the index of the first occurrence of the object or -1 if the
     *         object was not found.
     */
    CARAPI IndexOf(
        /* [in] */ IInterface* object,
        /* [out] */ Int32* index);

    /**
     * Searches this {@code List} for the specified object and returns the index of the
     * last occurrence.
     *
     * @param object
     *            the object to search for.
     * @return the index of the last occurrence of the object, or -1 if the
     *         object was not found.
     */
    CARAPI LastIndexOf(
        /* [in] */ IInterface* object,
        /* [out] */ Int32* index);

    /**
     * Returns a {@code List} iterator on the elements of this {@code List}. The elements are
     * iterated in the same order that they occur in the {@code List}.
     *
     * @return a {@code List} iterator on the elements of this {@code List}
     *
     * @see ListIterator
     */
    CARAPI GetListIterator(
        /* [out] */ IListIterator** it);

    /**
     * Returns a list iterator on the elements of this {@code List}. The elements are
     * iterated in the same order as they occur in the {@code List}. The iteration
     * starts at the specified location.
     *
     * @param location
     *            the index at which to start the iteration.
     * @return a list iterator on the elements of this {@code List}.
     * @throws IndexOutOfBoundsException
     *                if {@code location < 0 || location > size()}
     * @see ListIterator
     */
    CARAPI GetListIteratorEx(
        /* [in] */ Int32 location,
        /* [out] */ IListIterator** it);

    /**
     * Removes the object at the specified location from this {@code List}.
     *
     * @param location
     *            the index of the object to remove.
     * @return the removed object.
     * @throws UnsupportedOperationException
     *                if removing from this {@code List} is not supported.
     * @throws IndexOutOfBoundsException
     *                if {@code location < 0 || location >= size()}
     */
    CARAPI RemoveEx(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    /**
     * Replaces the element at the specified location in this {@code List} with the
     * specified object. This operation does not change the size of the {@code List}.
     *
     * @param location
     *            the index at which to put the specified object.
     * @param object
     *            the object to insert.
     * @return the previous element at the index.
     * @throws UnsupportedOperationException
     *                if replacing elements in this {@code List} is not supported.
     * @throws ClassCastException
     *                if the class of an object is inappropriate for this
     *                {@code List}.
     * @throws IllegalArgumentException
     *                if an object cannot be added to this {@code List}.
     * @throws IndexOutOfBoundsException
     *                if {@code location < 0 || location >= size()}
     */
    CARAPI Set(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object,
        /* [out] */ IInterface** prevObject);

    /**
     * Returns a {@code List} of the specified portion of this {@code List} from the given start
     * index to the end index minus one. The returned {@code List} is backed by this
     * {@code List} so changes to it are reflected by the other.
     *
     * @param start
     *            the index at which to start the sublist.
     * @param end
     *            the index one past the end of the sublist.
     * @return a list of a portion of this {@code List}.
     * @throws IndexOutOfBoundsException
     *                if {@code start < 0, start > end} or {@code end >
     *                size()}
     */
    CARAPI SubList(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IList** subList);

    /**
     * Searches this list for {@code object} and returns the index of the first
     * occurrence that is at or after {@code from}.
     *
     * @return the index or -1 if the object was not found.
     */
    CARAPI IndexOfEx(
        /* [in] */ IInterface* object,
        /* [in] */ Int32 from,
        /* [out] */ Int32* value);

    /**
     * Searches this list for {@code object} and returns the index of the last
     * occurrence that is before {@code to}.
     *
     * @return the index or -1 if the object was not found.
     */
    CARAPI LastIndexOfEx(
        /* [in] */ IInterface* object,
        /* [in] */ Int32 to,
        /* [out] */ Int32* value);

    /**
     * Adds the elements of {@code collection} that are not already present in
     * this list. If {@code collection} includes a repeated value, at most one
     * occurrence of that value will be added to this list. Elements are added
     * at the end of this list.
     *
     * <p>Callers of this method may prefer {@link CopyOnWriteArraySet}, whose
     * API is more appropriate for set operations.
     */
    CARAPI AddAllAbsent(
        /* [in] */ ICollection* collection,
        /* [out] */ Int32* value);

    /**
     * Adds {@code object} to the end of this list if it is not already present.
     *
     * <p>Callers of this method may prefer {@link CopyOnWriteArraySet}, whose
     * API is more appropriate for set operations.
     */
    CARAPI AddIfAbsent(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* value);

    CARAPI GetArray(
        /* [out, callee] */ ArrayOf<IInterface*>** outarr);

    CARAPI Clone(
    /* [out] */ IInterface** object);
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__CCOPYONWRITEARRAYLIST_H__
