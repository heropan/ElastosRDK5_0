#ifndef __ELASTOS_DROID_OS_CBUNDLE_H__
#define __ELASTOS_DROID_OS_CBUNDLE_H__

#include "_Elastos_Droid_Os_CBundle.h"
#include "os/BaseBundle.h"

using Elastos::Core::IClassLoader;
using Elastos::Core::ICloneable;
using Elastos::Core::ICharSequence;
using Elastos::Utility::IArrayList;
using Elastos::Utility::IList;
using Elastos::Droid::Utility::ISize;
using Elastos::Droid::Utility::ISizeF;
using Elastos::Droid::Utility::ISparseArray;

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CBundle)
    , public BaseBundle
    , public IBundle
    , public ICloneable
    , public IParcelable
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CBundle();

    ~CBundle();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IClassLoader* loader);

    CARAPI constructor(
        /* [in] */ Int32 capacity);

    CARAPI constructor(
        /* [in] */ IBundle* b);

    CARAPI constructor(
        /* [in] */ IPersistableBundle* b);

    /**
     * Make a Bundle for a single key/value pair.
     *
     * @hide
     */
    static AutoPtr<IBundle> ForPair(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    /** @hide */
    CARAPI SetAllowFds(
        /* [in] */ Boolean allowFds);

    CARAPI SetAllowFds(
        /* [in] */ Boolean allowFds,
        /* [out] */ Boolean* prev);

    CARAPI Clone(
        /* [out] */ IInterface** obj);

    /**
     * Removes all elements from the mapping of this Bundle.
     */
    CARAPI Clear();

    /**
     * Inserts all mappings from the given Bundle into this Bundle.
     *
     * @param bundle a Bundle
     */
    CARAPI PutAll(
        /* [in] */ IBundle* map);

    /**
     * Reports whether the bundle contains any parcelled file descriptors.
     */
    CARAPI HasFileDescriptors(
        /* [out] */ Boolean* result);

    /**
     * Inserts a Parcelable value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a String, or null
     * @param value a Parcelable object, or null
     */
    CARAPI PutParcelable(
        /* [in] */ const String& key,
        /* [in] */ IParcelable* value);

    /**
     * Inserts a Size value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a String, or null
     * @param value a Size object, or null
     */
    CARAPI PutSize(
        /* [in] */ const String& key,
        /* [in] */ ISize* value);

    /**
     * Inserts a SizeF value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a String, or null
     * @param value a SizeF object, or null
     */
    CARAPI PutSizeF(
        /* [in] */ const String& key,
        /* [in] */ ISizeF* value);

    /**
     * Inserts an array of Parcelable values into the mapping of this Bundle,
     * replacing any existing value for the given key.  Either key or value may
     * be null.
     *
     * @param key a String, or null
     * @param value an array of Parcelable objects, or null
     */
    CARAPI PutParcelableArray(
        /* [in] */ const String& key,
        /* [in] */ ArrayOf<IParcelable*>* value);

    /**
     * Inserts a List of Parcelable values into the mapping of this Bundle,
     * replacing any existing value for the given key.  Either key or value may
     * be null.
     *
     * @param key a String, or null
     * @param value an ArrayList of Parcelable objects, or null
     */
    CARAPI PutParcelableArrayList(
        /* [in] */ const String& key,
        /* [in] */ IArrayList* value);

    /** {@hide} */
    CARAPI PutParcelableList(
        /* [in] */ const String& key,
        /* [in] */ IList* value);

    /**
     * Inserts a SparceArray of Parcelable values into the mapping of this
     * Bundle, replacing any existing value for the given key.  Either key
     * or value may be null.
     *
     * @param key a String, or null
     * @param value a SparseArray of Parcelable objects, or null
     */
    CARAPI PutSparseParcelableArray(
        /* [in] */ const String& key,
        /* [in] */ ISparseArray* value);

   /**
     * Inserts a Bundle value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a String, or null
     * @param value a Bundle object, or null
     */
    CARAPI PutBundle(
        /* [in] */ const String& key,
        /* [in] */ IBundle* value);

    /**
     * Inserts an {@link IBinder} value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * <p class="note">You should be very careful when using this function.  In many
     * places where Bundles are used (such as inside of Intent objects), the Bundle
     * can live longer inside of another process than the process that had originally
     * created it.  In that case, the IBinder you supply here will become invalid
     * when your process goes away, and no longer usable, even if a new process is
     * created for you later on.</p>
     *
     * @param key a String, or null
     * @param value an IBinder object, or null
     */
    CARAPI PutBinder(
        /* [in] */ const String& key,
        /* [in] */ IBinder* value);

    /**
     * Inserts an IBinder value into the mapping of this Bundle, replacing
     * any existing value for the given key.  Either key or value may be null.
     *
     * @param key a String, or null
     * @param value an IBinder object, or null
     *
     * @deprecated
     * @hide This is the old name of the function.
     */
    CARAPI PutIBinder(
        /* [in] */ const String& key,
        /* [in] */ IBinder* value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a String, or null
     * @return a Size value, or null
     */
    CARAPI GetSize(
        /* [in] */ const String& key,
        /* [out] */ ISize** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a String, or null
     * @return a Size value, or null
     */
    CARAPI GetSizeF(
        /* [in] */ const String& key,
        /* [out] */ ISizeF** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a String, or null
     * @return a Bundle value, or null
     */
    CARAPI GetBundle(
        /* [in] */ const String& key,
        /* [out] */ IBundle** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a String, or null
     * @return a Parcelable value, or null
     */
    CARAPI GetParcelable(
        /* [in] */ const String& key,
        /* [out] */ IParcelable** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a String, or null
     * @return a Parcelable[] value, or null
     */
    CARAPI GetParcelableArray(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<IParcelable*>** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a String, or null
     * @return an ArrayList<T> value, or null
     */
    CARAPI GetParcelableArrayList(
        /* [in] */ const String& key,
        /* [out] */ IArrayList** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a String, or null
     *
     * @return a SparseArray of T values, or null
     */
    CARAPI GetSparseParcelableArray(
        /* [in] */ const String& key,
        /* [out] */ ISparseArray** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a String, or null
     * @return an IBinder value, or null
     */
    CARAPI GetBinder(
        /* [in] */ const String& key,
        /* [out] */ IBinder** value);

    /**
     * Returns the value associated with the given key, or null if
     * no mapping of the desired type exists for the given key or a null
     * value is explicitly associated with the key.
     *
     * @param key a String, or null
     * @return an IBinder value, or null
     *
     * @deprecated
     * @hide This is the old name of the function.
     */
    //@Deprecated
    CARAPI GetIBinder(
        /* [in] */ const String& key,
        /* [out] */ IBinder** value);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ToString(
        /* [out] */ String* str);

public:
    static AutoPtr<IBundle> EMPTY;

private:
    Boolean mHasFds;
    Boolean mFdsKnown;
    Boolean mAllowFds;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_CBUNDLE_H__
