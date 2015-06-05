
#ifndef __ELASTOS_UTILITY_CCONCURRENTHASHMAP_H__
#define __ELASTOS_UTILITY_CCONCURRENTHASHMAP_H__

#include "_Elastos_Utility_Concurrent_CConcurrentHashMap.h"
#include "AbstractMap.h"
#include "ReentrantLock.h"

using Elastos::IO::ISerializable;
using Elastos::IO::IObjectOutputStream;
using Elastos::IO::IObjectInputStream;
using Elastos::Utility::IIterator;
using Elastos::Utility::IEnumeration;
using Elastos::Utility::Concurrent::Locks::ReentrantLock;
using Elastos::Utility::Concurrent::Locks::IReentrantLock;
using Elastos::Utility::Concurrent::Locks::ICondition;

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CConcurrentHashMap)
    , public AbstractMap
    , public IConcurrentHashMap
    , public IConcurrentMap
{
public:
    /**
     * ConcurrentHashMap list entry. Note that this is never exported
     * out as a user-visible Map.Entry.
     */
    class HashEntry
        : public Object
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

        HashEntry(
            /* [in] */ Int32 hash,
            /* [in] */ IInterface* key,
            /* [in] */ IInterface* value,
            /* [in] */ HashEntry* next)
        {
            mHash = hash;
            mKey = key;
            mValue = value;
            mNext = next;
        }

        /**
         * Sets next field with volatile write semantics.  (See above
         * about use of putOrderedObject.)
         */
        CARAPI_(void) SetNext(
            /* [in] */ HashEntry* n)
        {
            // UNSAFE.putOrderedObject(this, nextOffset, n);
        }

        // Unsafe mechanics
        // static final sun.misc.Unsafe UNSAFE;
        static const Int64 mNextOffset;
        // static {
        //     try {
        //         UNSAFE = sun.misc.Unsafe.getUnsafe();
        //         Class<?> k = HashEntry.class;
        //         nextOffset = UNSAFE.objectFieldOffset
        //             (k.getDeclaredField("next"));
        //     } catch (Exception e) {
        //         throw new Error(e);
        //     }
        // }
    public:
        Int32 mHash;
        AutoPtr<IInterface> mKey;
        AutoPtr<IInterface> mValue;
        AutoPtr<HashEntry> mNext;
    };

    /**
     * Segments are specialized versions of hash tables.  This
     * subclasses from ReentrantLock opportunistically, just to
     * simplify some locking and avoid separate construction.
     */
    class Segment
        : public ReentrantLock
    {
    public:
        Segment(
            /* [in] */ Float lf,
            /* [in] */ Int32 threshold,
            /* [in] */ ArrayOf<HashEntry*>* tab);

        CARAPI_(AutoPtr<IInterface>) Put(
            /* [in] */ IInterface* key,
            /* [in] */ Int32 hash,
            /* [in] */ IInterface* value,
            /* [in] */ Boolean onlyIfAbsent);

        /**
         * Remove; match on key only if value null, else match both.
         */
        CARAPI_(AutoPtr<IInterface>) Remove(
            /* [in] */ IInterface* key,
            /* [in] */ Int32 hash,
            /* [in] */ IInterface* value);

        CARAPI_(Boolean) Replace(
            /* [in] */ IInterface* key,
            /* [in] */ Int32 hash,
            /* [in] */ IInterface* oldValue,
            /* [in] */ IInterface* newValue);

        CARAPI_(AutoPtr<IInterface>) Replace(
            /* [in] */ IInterface* key,
            /* [in] */ Int32 hash,
            /* [in] */ IInterface* value);

        CARAPI_(void) Clear();

    private:
        /**
         * Doubles size of table and repacks entries, also adding the
         * given node to new table
         */
        // @SuppressWarnings("unchecked")
        CARAPI_(void) Rehash(
            /* [in] */ HashEntry* node);

        /**
         * Scans for a node containing given key while trying to
         * acquire lock, creating and returning one if not found. Upon
         * return, guarantees that lock is held. Unlike in most
         * methods, calls to method equals are not screened: Since
         * traversal speed doesn't matter, we might as well help warm
         * up the associated code and accesses as well.
         *
         * @return a new node if key not found, else null
         */
        CARAPI_(AutoPtr<HashEntry>) ScanAndLockForPut(
            /* [in] */ IInterface* key,
            /* [in] */ Int32 hash,
            /* [in] */ IInterface* value);

        /**
         * Scans for a node containing the given key while trying to
         * acquire lock for a remove or replace operation. Upon
         * return, guarantees that lock is held.  Note that we must
         * lock even if the key is not found, to ensure sequential
         * consistency of updates.
         */
        CARAPI_(void) ScanAndLock(
            /* [in] */ IInterface* key,
            /* [in] */ Int32 hash);

    public:
        /**
         * The maximum number of times to tryLock in a prescan before
         * possibly blocking on acquire in preparation for a locked
         * segment operation. On multiprocessors, using a bounded
         * number of retries maintains cache acquired while locating
         * nodes.
         */
        static const Int32 MAX_SCAN_RETRIES; // = Runtime.getRuntime().availableProcessors() > 1 ? 64 : 1;

        /**
         * The per-segment table. Elements are accessed via
         * entryAt/setEntryAt providing volatile semantics.
         */
        /* transient*/ volatile AutoPtr< ArrayOf<HashEntry*> > mTable;

        /**
         * The number of elements. Accessed only either within locks
         * or among other volatile reads that maintain visibility.
         */
        Int32 mCount;

        /**
         * The total number of mutative operations in this segment.
         * Even though this may overflows 32 bits, it provides
         * sufficient accuracy for stability checks in CHM isEmpty()
         * and size() methods.  Accessed only either within locks or
         * among other volatile reads that maintain visibility.
         */
        Int32 mModCount;

        /**
         * The table is rehashed when its size exceeds this threshold.
         * (The value of this field is always <tt>(int)(capacity *
         * loadFactor)</tt>.)
         */
        Int32 mThreshold;

        /**
         * The load factor for the hash table.  Even though this value
         * is same for all segments, it is replicated to avoid needing
         * links to outer object.
         * @serial
         */
        Float mLoadFactor;

    private:
        /*
         * Segments maintain a table of entry lists that are always
         * kept in a consistent state, so can be read (via volatile
         * reads of segments and tables) without locking.  This
         * requires replicating nodes when necessary during table
         * resizing, so the old lists can be traversed by readers
         * still using old version of table.
         *
         * This class defines only mutative methods requiring locking.
         * Except as noted, the methods of this class perform the
         * per-segment versions of ConcurrentHashMap methods.  (Other
         * methods are integrated directly into ConcurrentHashMap
         * methods.) These mutative methods use a form of controlled
         * spinning on contention via methods scanAndLock and
         * scanAndLockForPut. These intersperse tryLocks with
         * traversals to locate nodes.  The main benefit is to absorb
         * cache misses (which are very common for hash tables) while
         * obtaining locks so that traversal is faster once
         * acquired. We do not actually use the found nodes since they
         * must be re-acquired under lock anyway to ensure sequential
         * consistency of updates (and in any case may be undetectably
         * stale), but they will normally be much faster to re-locate.
         * Also, scanAndLockForPut speculatively creates a fresh node
         * to use in put if no node is found.
         */
        static const Int64 sSerialVersionUID = 2249069246763182397L;
    };

    /* ---------------- Iterator Support -------------- */

    class HashIterator
        : public Object
        , public IIterator
    {
    public:
        CAR_INTERFACE_DECL()

        HashIterator(
            /* [in] */ CConcurrentHashMap* host);

        /**
         * Sets nextEntry to first node of next non-empty table
         * (in backwards order, to simplify checks).
         */
        CARAPI_(void) Advance();

        CARAPI_(AutoPtr<HashEntry>) NextEntry();

        CARAPI HasNext(
            /* [out] */ Boolean* value);
        CARAPI HasMoreElements(
            /* [out] */ Boolean* value);

        CARAPI Remove();

    public:
        Int32 mNextSegmentIndex;
        Int32 mNextTableIndex;
        AutoPtr< ArrayOf<HashEntry*> > mCurrentTable;
        AutoPtr<HashEntry> mNextEntry;
        AutoPtr<HashEntry> mLastReturned;
        AutoPtr<CConcurrentHashMap> mHost;
    };

    class KeyIterator
        : public HashIterator
        , public IEnumeration
    {
    public:
        CAR_INTERFACE_DECL()

        KeyIterator(
            /* [in] */ CConcurrentHashMap* host);

        CARAPI Next(
            /* [out] */ IInterface** object);

        CARAPI NextElement(
            /* [out] */ IInterface** object);
    };

    class ValueIterator
        : public HashIterator
        , public IEnumeration
    {
    public:
        CAR_INTERFACE_DECL()

        ValueIterator(
            /* [in] */ CConcurrentHashMap* host);

        CARAPI Next(
            /* [out] */ IInterface** object);

        CARAPI NextElement(
            /* [out] */ IInterface** object);
    };

    /**
     * Custom Entry class used by EntryIterator.next(), that relays
     * setValue changes to the underlying map.
     */
    class WriteThroughEntry
        : public AbstractMap::SimpleEntry
    {
    public:
        WriteThroughEntry(
            /* [in] */ IInterface* k,
            /* [in] */ IInterface* v,
            /* [in] */ CConcurrentHashMap* host);

        /**
         * Sets our entry's value and writes through to the map. The
         * value to return is somewhat arbitrary here. Since a
         * WriteThroughEntry does not necessarily track asynchronous
         * changes, the most recent "previous" value could be
         * different from what we return (or could even have been
         * removed in which case the put will re-establish). We do not
         * and cannot guarantee more.
         */
        CARAPI SetValue(
            /* [in] */ IInterface* value,
            /* [out] */ IInterface** outface);

    private:
        AutoPtr<CConcurrentHashMap> mHost;
    };

    class EntryIterator
        : public HashIterator
    {
    public:
        EntryIterator(
            /* [in] */ CConcurrentHashMap* host);

        CARAPI Next(
            /* [out] */ IInterface** object);
    };

    class _KeySet
        : public AbstractSet
    {
    public:
        _KeySet(
            /* [in] */ CConcurrentHashMap* host);

        CARAPI GetIterator(
            /* [out] */ IIterator** result);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Contains(
            /* [in] */ IInterface* o,
            /* [out] */ Boolean* result);

        CARAPI Remove(
            /* [in] */ IInterface* o,
            /* [out] */ Boolean* result);

        CARAPI Clear();

    private:
        AutoPtr<CConcurrentHashMap> mHost;
    };

    class _Values
        : public AbstractCollection
    {
    public:
        _Values(
            /* [in] */ CConcurrentHashMap* host);

        CARAPI GetIterator(
            /* [out] */ IIterator** result);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Contains(
            /* [in] */ IInterface* o,
            /* [out] */ Boolean* result);

        CARAPI Clear();

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hashCode);
    public:
        AutoPtr<CConcurrentHashMap> mHost;
    };

    class _EntrySet
        : public AbstractSet
    {
    public:
        _EntrySet(
            /* [in] */ CConcurrentHashMap* host);

        CARAPI GetIterator(
            /* [out] */ IIterator** result);

        CARAPI Contains(
            /* [in] */ IInterface* o,
            /* [out] */ Boolean* result);

        CARAPI Remove(
            /* [in] */ IInterface* o,
            /* [out] */ Boolean* result);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Clear();

    private:
        AutoPtr<CConcurrentHashMap> mHost;
    };

public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    /**
     * Creates a new, empty map with the specified initial
     * capacity, load factor and concurrency level.
     *
     * @param initialCapacity the initial capacity. The implementation
     * performs internal sizing to accommodate this many elements.
     * @param loadFactor  the load factor threshold, used to control resizing.
     * Resizing may be performed when the average number of elements per
     * bin exceeds this threshold.
     * @param concurrencyLevel the estimated number of concurrently
     * updating threads. The implementation performs internal sizing
     * to try to accommodate this many threads.
     * @throws IllegalArgumentException if the initial capacity is
     * negative or the load factor or concurrencyLevel are
     * nonpositive.
     */
    // @SuppressWarnings("unchecked")
    CARAPI constructor(
        /* [in] */ Int32 initialCapacity,
        /* [in] */ Float loadFactor,
        /* [in] */ Int32 concurrencyLevel);

    /**
     * Creates a new, empty map with the specified initial capacity
     * and load factor and with the default concurrencyLevel (16).
     *
     * @param initialCapacity The implementation performs internal
     * sizing to accommodate this many elements.
     * @param loadFactor  the load factor threshold, used to control resizing.
     * Resizing may be performed when the average number of elements per
     * bin exceeds this threshold.
     * @throws IllegalArgumentException if the initial capacity of
     * elements is negative or the load factor is nonpositive
     *
     * @since 1.6
     */
    CARAPI constructor(
        /* [in] */ Int32 initialCapacity,
        /* [in] */ Float loadFactor);

    /**
     * Creates a new, empty map with the specified initial capacity,
     * and with default load factor (0.75) and concurrencyLevel (16).
     *
     * @param initialCapacity the initial capacity. The implementation
     * performs internal sizing to accommodate this many elements.
     * @throws IllegalArgumentException if the initial capacity of
     * elements is negative.
     */
    CARAPI constructor(
        /* [in] */ Int32 initialCapacity);

    /**
     * Creates a new, empty map with a default initial capacity (16),
     * load factor (0.75) and concurrencyLevel (16).
     */
    CARAPI constructor();

    /**
     * Creates a new map with the same mappings as the given map.
     * The map is created with a capacity of 1.5 times the number
     * of mappings in the given map or 16 (whichever is greater),
     * and a default load factor (0.75) and concurrencyLevel (16).
     *
     * @param m the map
     */
    CARAPI constructor(
        /* [in] */ IMap* m);

    /**
     * If the specified key is not already associated
     * with a value, associate it with the given value.
     * This is equivalent to
     *  <pre> {@code
     * if (!map.containsKey(key))
     *   return map.put(key, value);
     * else
     *   return map.get(key);}</pre>
     *
     * except that the action is performed atomically.
     *
     * @param key key with which the specified value is to be associated
     * @param value value to be associated with the specified key
     * @return the previous value associated with the specified key, or
     *         <tt>null</tt> if there was no mapping for the key.
     *         (A <tt>null</tt> return can also indicate that the map
     *         previously associated <tt>null</tt> with the key,
     *         if the implementation supports null values.)
     * @throws UnsupportedOperationException if the <tt>put</tt> operation
     *         is not supported by this map
     * @throws ClassCastException if the class of the specified key or value
     *         prevents it from being stored in this map
     * @throws NullPointerException if the specified key or value is null,
     *         and this map does not permit null keys or values
     * @throws IllegalArgumentException if some property of the specified key
     *         or value prevents it from being stored in this map
     *
     */
    CARAPI PutIfAbsent(
        /* [in] */ IInterface* key,
        /* [in] */ IInterface* value,
        /* [out] */ IInterface** outface);

    /**
     * Removes the entry for a key only if currently mapped to a given value.
     * This is equivalent to
     *  <pre> {@code
     * if (map.containsKey(key) && map.get(key).equals(value)) {
     *   map.remove(key);
     *   return true;
     * } else
     *   return false;}</pre>
     *
     * except that the action is performed atomically.
     *
     * @param key key with which the specified value is associated
     * @param value value expected to be associated with the specified key
     * @return <tt>true</tt> if the value was removed
     * @throws UnsupportedOperationException if the <tt>remove</tt> operation
     *         is not supported by this map
     * @throws ClassCastException if the key or value is of an inappropriate
     *         type for this map
     *         (<a href="../Collection.html#optional-restrictions">optional</a>)
     * @throws NullPointerException if the specified key or value is null,
     *         and this map does not permit null keys or values
     *         (<a href="../Collection.html#optional-restrictions">optional</a>)
     */
    CARAPI Remove(
        /* [in] */ IInterface* key,
        /* [in] */ IInterface* value,
        /* [out] */ Boolean* result);

    /**
     * Replaces the entry for a key only if currently mapped to a given value.
     * This is equivalent to
     *  <pre> {@code
     * if (map.containsKey(key) && map.get(key).equals(oldValue)) {
     *   map.put(key, newValue);
     *   return true;
     * } else
     *   return false;}</pre>
     *
     * except that the action is performed atomically.
     *
     * @param key key with which the specified value is associated
     * @param oldValue value expected to be associated with the specified key
     * @param newValue value to be associated with the specified key
     * @return <tt>true</tt> if the value was replaced
     * @throws UnsupportedOperationException if the <tt>put</tt> operation
     *         is not supported by this map
     * @throws ClassCastException if the class of a specified key or value
     *         prevents it from being stored in this map
     * @throws NullPointerException if a specified key or value is null,
     *         and this map does not permit null keys or values
     * @throws IllegalArgumentException if some property of a specified key
     *         or value prevents it from being stored in this map
     */
    CARAPI Replace(
        /* [in] */ IInterface* key,
        /* [in] */ IInterface* oldValue,
        /* [in] */ IInterface* newValue,
        /* [out] */ Boolean* result);

    /**
     * Replaces the entry for a key only if currently mapped to some value.
     * This is equivalent to
     *  <pre> {@code
     * if (map.containsKey(key)) {
     *   return map.put(key, value);
     * } else
     *   return null;}</pre>
     *
     * except that the action is performed atomically.
     *
     * @param key key with which the specified value is associated
     * @param value value to be associated with the specified key
     * @return the previous value associated with the specified key, or
     *         <tt>null</tt> if there was no mapping for the key.
     *         (A <tt>null</tt> return can also indicate that the map
     *         previously associated <tt>null</tt> with the key,
     *         if the implementation supports null values.)
     * @throws UnsupportedOperationException if the <tt>put</tt> operation
     *         is not supported by this map
     * @throws ClassCastException if the class of the specified key or value
     *         prevents it from being stored in this map
     * @throws NullPointerException if the specified key or value is null,
     *         and this map does not permit null keys or values
     * @throws IllegalArgumentException if some property of the specified key
     *         or value prevents it from being stored in this map
     */
    CARAPI Replace(
        /* [in] */ IInterface* key,
        /* [in] */ IInterface* value,
        /* [out] */ IInterface** result);

    /**
     * Removes all elements from this {@code Map}, leaving it empty.
     *
     * @throws UnsupportedOperationException
     *                if removing elements from this {@code Map} is not supported.
     * @see #isEmpty()
     * @see #size()
     */
    CARAPI Clear();

    /**
     * Returns whether this {@code Map} contains the specified key.
     *
     * @param key
     *            the key to search for.
     * @return {@code true} if this map contains the specified key,
     *         {@code false} otherwise.
     */
    CARAPI ContainsKey(
        /* [in] */ IInterface* key,
        /* [out] */ Boolean* result);

    /**
     * Returns whether this {@code Map} contains the specified value.
     *
     * @param value
     *            the value to search for.
     * @return {@code true} if this map contains the specified value,
     *         {@code false} otherwise.
     */
    CARAPI ContainsValue(
        /* [in] */ IInterface* value,
        /* [out] */ Boolean* result);

    /**
     * Legacy method testing if some key maps into the specified value
     * in this table.  This method is identical in functionality to
     * {@link #containsValue}, and exists solely to ensure
     * full compatibility with class {@link java.util.Hashtable},
     * which supported this method prior to introduction of the
     * Java Collections framework.
     *
     * @param  value a value to search for
     * @return <tt>true</tt> if and only if some key maps to the
     *         <tt>value</tt> argument in this table as
     *         determined by the <tt>equals</tt> method;
     *         <tt>false</tt> otherwise
     * @throws NullPointerException if the specified value is null
     */
    CARAPI Contains(
        /* [in] */ IInterface* value,
        /* [out] */ Boolean* result);

    /**
     * Returns a {@code Set} containing all of the mappings in this {@code Map}. Each mapping is
     * an instance of {@link Map.Entry}. As the {@code Set} is backed by this {@code Map},
     * changes in one will be reflected in the other.
     *
     * @return a set of the mappings
     */
    CARAPI EntrySet(
        /* [out] */ ISet** entries);

    /**
     * Compares the argument to the receiver, and returns {@code true} if the
     * specified object is a {@code Map} and both {@code Map}s contain the same mappings.
     *
     * @param object
     *            the {@code Object} to compare with this {@code Object}.
     * @return boolean {@code true} if the {@code Object} is the same as this {@code Object}
     *         {@code false} if it is different from this {@code Object}.
     * @see #hashCode()
     * @see #entrySet()
     */
    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    /**
     * Returns the value of the mapping with the specified key.
     *
     * @param key
     *            the key.
     * @return the value of the mapping with the specified key, or {@code null}
     *         if no mapping for the specified key is found.
     */
    CARAPI Get(
        /* [in] */ IInterface* key,
        /* [out] */ IInterface** value);

    /**
     * Returns an integer hash code for the receiver. {@code Object}s which are equal
     * return the same value for this method.
     *
     * @return the receiver's hash.
     * @see #equals(Object)
     */
    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    /**
     * Returns whether this map is empty.
     *
     * @return {@code true} if this map has no elements, {@code false}
     *         otherwise.
     * @see #size()
     */
    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    /**
     * Returns a set of the keys contained in this {@code Map}. The {@code Set} is backed by
     * this {@code Map} so changes to one are reflected by the other. The {@code Set} does not
     * support adding.
     *
     * @return a set of the keys.
     */
    CARAPI KeySet(
        /* [out] */ ISet** keySet);

    /**
     * Maps the specified key to the specified value.
     *
     * @param key
     *            the key.
     * @param value
     *            the value.
     * @return the value of any previous mapping with the specified key or
     *         {@code null} if there was no mapping.
     * @throws UnsupportedOperationException
     *                if adding to this {@code Map} is not supported.
     * @throws ClassCastException
     *                if the class of the key or value is inappropriate for
     *                this {@code Map}.
     * @throws IllegalArgumentException
     *                if the key or value cannot be added to this {@code Map}.
     * @throws NullPointerException
     *                if the key or value is {@code null} and this {@code Map} does
     *                not support {@code null} keys or values.
     */
    CARAPI Put(
        /* [in] */ IInterface* key,
        /* [in] */ IInterface* value,
        /* [out] */ IInterface** oldValue);

    /**
     * Copies every mapping in the specified {@code Map} to this {@code Map}.
     *
     * @param map
     *            the {@code Map} to copy mappings from.
     * @throws UnsupportedOperationException
     *                if adding to this {@code Map} is not supported.
     * @throws ClassCastException
     *                if the class of a key or a value of the specified {@code Map} is
     *                inappropriate for this {@code Map}.
     * @throws IllegalArgumentException
     *                if a key or value cannot be added to this {@code Map}.
     * @throws NullPointerException
     *                if a key or value is {@code null} and this {@code Map} does not
     *                support {@code null} keys or values.
     */
    CARAPI PutAll(
        /* [in] */ IMap* map);

    /**
     * Removes a mapping with the specified key from this {@code Map}.
     *
     * @param key
     *            the key of the mapping to remove.
     * @return the value of the removed mapping or {@code null} if no mapping
     *         for the specified key was found.
     * @throws UnsupportedOperationException
     *                if removing from this {@code Map} is not supported.
     */
    CARAPI Remove(
        /* [in] */ IInterface* key,
        /* [out] */ IInterface** value);

    /**
     * Returns the number of mappings in this {@code Map}.
     *
     * @return the number of mappings in this {@code Map}.
     */
    CARAPI GetSize(
        /* [out] */ Int32* outsize);

    /**
     * Returns a {@code Collection} of the values contained in this {@code Map}. The {@code Collection}
     * is backed by this {@code Map} so changes to one are reflected by the other. The
     * {@code Collection} supports {@link Collection#remove}, {@link Collection#removeAll},
     * {@link Collection#retainAll}, and {@link Collection#clear} operations,
     * and it does not support {@link Collection#add} or {@link Collection#addAll} operations.
     * <p>
     * This method returns a {@code Collection} which is the subclass of
     * {@link AbstractCollection}. The {@link AbstractCollection#iterator} method of this subclass returns a
     * "wrapper object" over the iterator of this {@code Map}'s {@link #entrySet()}. The {@link AbstractCollection#size} method
     * wraps this {@code Map}'s {@link #size} method and the {@link AbstractCollection#contains} method wraps this {@code Map}'s
     * {@link #containsValue} method.
     * <p>
     * The collection is created when this method is called at first time and
     * returned in response to all subsequent calls. This method may return
     * different Collection when multiple calls to this method, since it has no
     * synchronization performed.
     *
     * @return a collection of the values contained in this map.
     */
    CARAPI Values(
        /* [out] */ ICollection** value);

    /**
     * Returns an enumeration of the keys in this table.
     *
     * @return an enumeration of the keys in this table
     * @see #keySet()
     */
    CARAPI Keys(
        /* [out] */ IEnumeration** outenum);

    /**
     * Returns an enumeration of the values in this table.
     *
     * @return an enumeration of the values in this table
     * @see #values()
     */
    CARAPI Elements(
        /* [out] */ IEnumeration** outenum);

    /**
     * Gets the ith element of given table (if nonnull) with volatile
     * read semantics. Note: This is manually integrated into a few
     * performance-sensitive methods to reduce call overhead.
     */
    // @SuppressWarnings("unchecked")
    static CARAPI_(AutoPtr<HashEntry>) EntryAt(
        /* [in] */ ArrayOf<HashEntry*>* tab,
        /* [in] */ Int32 i);

    /**
     * Sets the ith element of given table, with volatile write
     * semantics. (See above about use of putOrderedObject.)
     */
    static CARAPI_(void) SetEntryAt(
        /* [in] */ ArrayOf<HashEntry*>* tab,
        /* [in] */ Int32 i,
        /* [in] */ HashEntry* e);

    // Accessing segments

    /**
     * Gets the jth element of given segment array (if nonnull) with
     * volatile element access semantics via Unsafe. (The null check
     * can trigger harmlessly only during deserialization.) Note:
     * because each element of segments array is set only once (using
     * fully ordered writes), some performance-sensitive methods rely
     * on this method only as a recheck upon null reads.
     */
    // @SuppressWarnings("unchecked")
    static CARAPI_(AutoPtr<Segment>) SegmentAt(
        /* [in] */ ArrayOf< AutoPtr<Segment> >* ss,
        /* [in] */ Int32 j);

    /**
     * Gets the table entry for the given segment and hash code.
     */
    // @SuppressWarnings("unchecked")
    static CARAPI_(AutoPtr<HashEntry>) EntryForHash(
        /* [in] */ Segment* seg,
        /* [in] */ Int32 h);

private:
    /**
     * Returns the segment for the given index, creating it and
     * recording in segment table (via CAS) if not already present.
     *
     * @param k the index
     * @return the segment
     */
    // @SuppressWarnings("unchecked")
    CARAPI_(AutoPtr<Segment>) EnsureSegment(
        /* [in] */ Int32 k);

    // Hash-based segment and entry accesses

    /**
     * Gets the segment for the given hash code.
     */
    // @SuppressWarnings("unchecked")
    CARAPI_(AutoPtr<Segment>) SegmentForHash(
        /* [in] */ Int32 h);

    /**
     * Applies a supplemental hash function to a given hashCode, which
     * defends against poor quality hash functions.  This is critical
     * because ConcurrentHashMap uses power-of-two length hash tables,
     * that otherwise encounter collisions for hashCodes that do not
     * differ in lower or upper bits.
     */
    static CARAPI_(Int32) Hash(
        /* [in] */ Int32 h);

    /* ---------------- Serialization Support -------------- */

    /**
     * Saves the state of the <tt>ConcurrentHashMap</tt> instance to a
     * stream (i.e., serializes it).
     * @param s the stream
     * @serialData
     * the key (Object) and value (Object)
     * for each key-value mapping, followed by a null pair.
     * The key-value mappings are emitted in no particular order.
     */
    CARAPI WriteObject(
        /* [in] */ IObjectOutputStream* s);

    /**
     * Reconstitutes the <tt>ConcurrentHashMap</tt> instance from a
     * stream (i.e., deserializes it).
     * @param s the stream
     */
    // @SuppressWarnings("unchecked")
    CARAPI ReadObject(
        /* [in] */ IObjectInputStream* s);

public:
    /*
     * The basic strategy is to subdivide the table among Segments,
     * each of which itself is a concurrently readable hash table.  To
     * reduce footprint, all but one segments are constructed only
     * when first needed (see ensureSegment). To maintain visibility
     * in the presence of lazy construction, accesses to segments as
     * well as elements of segment's table must use volatile access,
     * which is done via Unsafe within methods segmentAt etc
     * below. These provide the functionality of AtomicReferenceArrays
     * but reduce the levels of indirection. Additionally,
     * volatile-writes of table elements and entry "next" fields
     * within locked operations use the cheaper "lazySet" forms of
     * writes (via putOrderedObject) because these writes are always
     * followed by lock releases that maintain sequential consistency
     * of table updates.
     *
     * Historical note: The previous version of this class relied
     * heavily on "final" fields, which avoided some volatile reads at
     * the expense of a large initial footprint.  Some remnants of
     * that design (including forced construction of segment 0) exist
     * to ensure serialization compatibility.
     */

    /* ---------------- Constants -------------- */

    /**
     * The default initial capacity for this table,
     * used when not otherwise specified in a constructor.
     */
    static const Int32 DEFAULT_INITIAL_CAPACITY = 16;

    /**
     * The default load factor for this table, used when not
     * otherwise specified in a constructor.
     */
    static const Float DEFAULT_LOAD_FACTOR = 0.75f;

    /**
     * The default concurrency level for this table, used when not
     * otherwise specified in a constructor.
     */
    static const Int32 DEFAULT_CONCURRENCY_LEVEL = 16;

    /**
     * The maximum capacity, used if a higher value is implicitly
     * specified by either of the constructors with arguments.  MUST
     * be a power of two <= 1<<30 to ensure that entries are indexable
     * using ints.
     */
    static const Int32 MAXIMUM_CAPACITY = 1 << 30;

    /**
     * The minimum capacity for per-segment tables.  Must be a power
     * of two, at least two to avoid immediate resizing on next use
     * after lazy construction.
     */
    static const Int32 MIN_SEGMENT_TABLE_CAPACITY = 2;

    /**
     * The maximum number of segments to allow; used to bound
     * constructor arguments. Must be power of two less than 1 << 24.
     */
    static const Int32 MAX_SEGMENTS = 1 << 16; // slightly conservative

    /**
     * Number of unsynchronized retries in size and containsValue
     * methods before resorting to locking. This is used to avoid
     * unbounded retries if tables undergo continuous modification
     * which would make it impossible to obtain an accurate result.
     */
    static const Int32 RETRIES_BEFORE_LOCK = 2;

    /* ---------------- Fields -------------- */

    /**
     * Mask value for indexing into segments. The upper bits of a
     * key's hash code are used to choose the segment.
     */
    Int32 mSegmentMask;

    /**
     * Shift value for indexing within segments.
     */
    Int32 mSegmentShift;

    /**
     * The segments, each of which is a specialized hash table.
     */
    AutoPtr< ArrayOf< AutoPtr<Segment> > > mSegments;

    AutoPtr<ISet> mKeySet;
    AutoPtr<ISet> mEntrySet;
    AutoPtr<ICollection> mValues;

private:
    // Unsafe mechanics
    // static final sun.misc.Unsafe UNSAFE;
    static Int64 SBASE;
    static Int32 SSHIFT;
    static Int64 TBASE;
    static Int32 TSHIFT;
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CCONCURRENTHASHMAP_H__
