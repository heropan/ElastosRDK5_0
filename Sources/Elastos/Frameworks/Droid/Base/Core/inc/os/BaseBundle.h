
#ifndef __ELASTOS_DROID_OS_BASE_BUNDLE_H__
#define __ELASTOS_DROID_OS_BASE_BUNDLE_H__

#include "ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::IClassLoader;
using Elastos::Core::IArrayOf;
using Elastos::IO::ISerializable;
using Elastos::Utility::IArrayList;
using Elastos::Utility::ISet;
using Elastos::Utility::IMap;

using Elastos::Droid::Utility::IArrayMap;

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * A mapping from String values to various types.
 */
class BaseBundle
    : public Object
    , public IBaseBundle
{
public:
    CAR_INTERFACE_DECL()

    /**
     * Constructs a new, empty Bundle that uses a specific ClassLoader for
     * instantiating Parcelable and Serializable objects.
     *
     * @param loader An explicit ClassLoader to use when instantiating objects
     * inside of the Bundle.
     * @param capacity Initial size of the ArrayMap.
     */
    CARAPI constructor(
        /* [in] */ IClassLoader* loader,
        /* [in] */ Int32 capacity);

    /**
     * Constructs a new, empty Bundle.
     */
    CARAPI constructor();

    /**
     * Constructs a Bundle whose data is stored as a Parcel.  The data
     * will be unparcelled on first contact, using the assigned ClassLoader.
     *
     * @param parcelledData a Parcel containing a Bundle
     */
    CARAPI constructor(
        /* [in] */ IParcel* parcel);

    CARAPI constructor(
        /* [in] */ IParcel* parcel,
        /* [in] */ Int32 length);

    /**
     * Constructs a new, empty Bundle that uses a specific ClassLoader for
     * instantiating Parcelable and Serializable objects.
     *
     * @param loader An explicit ClassLoader to use when instantiating objects
     * inside of the Bundle.
     */
    CARAPI constructor(
        /* [in] */ IClassLoader* loader);

    /**
     * Constructs a new, empty Bundle sized to hold the given number of
     * elements. The Bundle will grow as needed.
     *
     * @param capacity the initial capacity of the Bundle
     */
    CARAPI constructor(
        /* [in] */ Int32 capacity);

    /**
     * Constructs a Bundle containing a copy of the mappings from the given
     * Bundle.
     *
     * @param b a Bundle to be copied.
     */
    CARAPI constructor(
        /* [in] */ IBaseBundle* b);

    /**
     * TODO: optimize this later (getting just the value part of a Bundle
     * with a single pair) once Bundle.forPair() above is implemented
     * with a special single-value Map implementation/serialization.
     *
     * Note: value in single-pair Bundle may be null.
     *
     * @hide
     */
    CARAPI_(String) GetPairValue();

    /**
     * Changes the ClassLoader this Bundle uses when instantiating objects.
     *
     * @param loader An explicit ClassLoader to use when instantiating objects
     * inside of the Bundle.
     */
    CARAPI_(void) SetClassLoader(
        /* [in] */ IClassLoader* loader);

    /**
     * Return the ClassLoader currently associated with this Bundle.
     */
    CARAPI_(AutoPtr<IClassLoader>) GetClassLoader();

    /**
     * If the underlying data are stored as a Parcel, unparcel them
     * using the currently assigned class loader.
     */
    CARAPI_(void) Unparcel();

    CARAPI_(Boolean) IsParceled();

    CARAPI GetSize(
        /* [out] */ Int32 * size);

    CARAPI IsEmpty(
        /* [out] */ Boolean* empty);

    CARAPI Clear();

    CARAPI ContainsKey(
        /* [in] */ String key,
        /* [out] */ Boolean* result);

    CARAPI Get(
        /* [in] */ String key,
        /* [out] */ IInterface** obj);

    CARAPI Remove(
        /* [in] */ String key);

    CARAPI PutAll(
        /* [in] */ IPersistableBundle* bundle);

    CARAPI PutAll(
        /* [in] */ IMap* bundle);

    CARAPI GetKeySet(
        /* [out] */ ISet** set);

    CARAPI PutBoolean(
        /* [in] */ String key,
        /* [in] */ Boolean value);

    CARAPI PutByte(
        /* [in] */ String key,
        /* [in] */ Byte value);

    CARAPI PutChar(
        /* [in] */ String key,
        /* [in] */ Char32 value);

    CARAPI PutInt16(
        /* [in] */ String key,
        /* [in] */ Int16 value);

    CARAPI PutInt32(
        /* [in] */ String key,
        /* [in] */ Int32 value);

    CARAPI PutInt64(
        /* [in] */ String key,
        /* [in] */ Int64 value);

    CARAPI PutFloat(
        /* [in] */ String key,
        /* [in] */ Float value);

    CARAPI PutDouble(
        /* [in] */ String key,
        /* [in] */ Double value);

    CARAPI PutString(
        /* [in] */ String key,
        /* [in] */ String value);

    CARAPI PutCharSequence(
        /* [in] */ String key,
        /* [in] */ ICharSequence* value);

    CARAPI PutIntegerArrayList(
        /* [in] */ String key,
        /* [in] */ IArrayList* value);

    CARAPI PutStringArrayList(
        /* [in] */ String key,
        /* [in] */ IArrayList* value);

    CARAPI PutCharSequenceArrayList(
        /* [in] */ String key,
        /* [in] */ IArrayList* value);

    CARAPI PutSerializable(
        /* [in] */ String key,
        /* [in] */ ISerializable* value);

    CARAPI PutBooleanArray(
        /* [in] */ String key,
        /* [in] */ ArrayOf<Boolean>* value);

    CARAPI PutByteArray(
        /* [in] */ String key,
        /* [in] */ ArrayOf<Byte>* value);

    CARAPI PutCharArray(
        /* [in] */ String key,
        /* [in] */ ArrayOf<Char32>* value);

    CARAPI PutInt16Array(
        /* [in] */ String key,
        /* [in] */ ArrayOf<Int16>* value);

    CARAPI PutInt32Array(
        /* [in] */ String key,
        /* [in] */ ArrayOf<Int32>* value);

    CARAPI PutInt64Array(
        /* [in] */ String key,
        /* [in] */ ArrayOf<Int64>* value);

    CARAPI PutFloatArray(
        /* [in] */ String key,
        /* [in] */ ArrayOf<Float>* value);

    CARAPI PutDoubleArray(
        /* [in] */ String key,
        /* [in] */ ArrayOf<Double>* value);

    CARAPI PutStringArray(
        /* [in] */ String key,
        /* [in] */ ArrayOf<String>* value);

    CARAPI PutCharSequenceArray(
        /* [in] */ String key,
        /* [in] */ ArrayOf<ICharSequence*>* value);

    CARAPI GetBoolean(
        /* [in] */ String key,
        /* [out] */ Boolean* value);

    CARAPI GetBoolean(
        /* [in] */ String key,
        /* [in] */ Boolean defaultValue,
        /* [out] */ Boolean* value);

    CARAPI GetByte(
        /* [in] */ String key,
        /* [out] */ Byte* value);

    CARAPI GetByte(
        /* [in] */ String key,
        /* [in] */ Byte defaultValue,
        /* [out] */ Byte* value);

    CARAPI GetChar(
        /* [in] */ String key,
        /* [out] */ Char32* value);

    CARAPI GetChar(
        /* [in] */ String key,
        /* [in] */ Char32 defaultValue,
        /* [out] */ Char32* value);

    CARAPI GetInt16(
        /* [in] */ String key,
        /* [out] */ Int16* value);

    CARAPI GetInt16(
        /* [in] */ String key,
        /* [in] */ Int16 defaultValue,
        /* [out] */ Int16* value);

    CARAPI GetInt32(
        /* [in] */ String key,
        /* [out] */ Int32* value);

    CARAPI GetInt32(
        /* [in] */ String key,
        /* [in] */ Int32 defaultValue,
        /* [out] */ Int32* value);

    CARAPI GetInt64(
        /* [in] */ String key,
        /* [out] */ Int64* value);

    CARAPI GetInt64(
        /* [in] */ String key,
        /* [in] */ Int64 defaultValue,
        /* [out] */ Int64* value);

    CARAPI GetFloat(
        /* [in] */ String key,
        /* [out] */ Float* value);

    CARAPI GetFloat(
        /* [in] */ String key,
        /* [in] */ Float defaultValue,
        /* [out] */ Float* value);

    CARAPI GetDouble(
        /* [in] */ String key,
        /* [out] */ Double* value);

    CARAPI GetDouble(
        /* [in] */ String key,
        /* [in] */ Double defaultValue,
        /* [out] */ Double* value);

    CARAPI GetString(
        /* [in] */ String key,
        /* [out] */ String* value);

    CARAPI GetString(
        /* [in] */ String key,
        /* [in] */ String defaultValue,
        /* [out] */ String* value);

    CARAPI GetCharSequence(
        /* [in] */ String key,
        /* [out] */ ICharSequence** value);

    CARAPI GetCharSequence(
        /* [in] */ String key,
        /* [in] */ ICharSequence* defaultValue,
        /* [out] */ ICharSequence** value);

    CARAPI GetSerializable(
        /* [in] */ String key,
        /* [out] */ ISerializable** value);

    CARAPI GetIntegerArrayList(
        /* [in] */ String key,
        /* [out] */ IArrayList** value);

    CARAPI GetStringArrayList(
        /* [in] */ String key,
        /* [out] */ IArrayList** value);

    CARAPI GetCharSequenceArrayList(
        /* [in] */ String key,
        /* [out] */ IArrayList** value);

    CARAPI GetBooleanArray(
        /* [in] */ String key,
        /* [out, callee] */ ArrayOf<Boolean>** value);

    CARAPI GetByteArray(
        /* [in] */ String key,
        /* [out, callee] */ ArrayOf<Byte>** value);

    CARAPI GetInt16Array(
        /* [in] */ String key,
        /* [out, callee] */ ArrayOf<Int16>** value);

    CARAPI GetCharArray(
        /* [in] */ String key,
        /* [out, callee] */ ArrayOf<Char32>** value);

    CARAPI GetInt32Array(
        /* [in] */ String key,
        /* [out, callee] */ ArrayOf<Int32>** value);

    CARAPI GetInt64Array(
        /* [in] */ String key,
        /* [out, callee] */ ArrayOf<Int64>** value);

    CARAPI GetFloatArray(
        /* [in] */ String key,
        /* [out, callee] */ ArrayOf<Float>** value);

    CARAPI GetDoubleArray(
        /* [in] */ String key,
        /* [out, callee] */ ArrayOf<Double>** value);

    CARAPI GetStringArray(
        /* [in] */ String key,
        /* [out, callee] */ ArrayOf<String>** value);

    CARAPI GetCharSequenceArray(
        /* [in] */ String key,
        /* [out, callee] */ ArrayOf<ICharSequence*>** value);

    /**
     * Writes the Bundle contents to a Parcel, typically in order for
     * it to be passed through an IBinder connection.
     * @param parcel The parcel to copy this bundle to.
     */
    CARAPI WriteToParcelInner(
        /* [in] */ IParcel* parcel,
        /* [in] */ Int32 flags);

    /**
     * Reads the Parcel contents into this Bundle, typically in order for
     * it to be passed through an IBinder connection.
     * @param parcel The parcel to overwrite this bundle from.
     */
    CARAPI ReadFromParcelInner(
        /* [in] */ IParcel* parcel);

protected:
    BaseBundle();

private:

    CARAPI WriteArrayMapInternal(
        /* [in] */ IParcel* dest,
        /* [in] */ IArrayMap* map);

    CARAPI WriteValue(
    /* [in] */ IParcel* dest,
    /* [in] */ IInterface* obj);

    CARAPI ReadFromParcelInner(
        /* [in] */ IParcel* parcel,
        /* [in] */ Int32 length);

    // Log a message if the value was non-null but not of the expected type
    CARAPI_(void) TypeWarning(
        /* [in] */ String key,
        /* [in] */ String className);

    CARAPI_(AutoPtr<IInterface>) GetValue(
        /* [in] */ String key);

    CARAPI_(AutoPtr<IArrayOf>) GetIArrayOf(
        /* [in] */ String key,
        /* [in] */ String className);

private:
    static const String TAG;// = "Bundle";
    static const Boolean DEBUG;// = false;

    static const Int32 BUNDLE_MAGIC;// = 0x4C444E42; // 'B' 'N' 'D' 'L'
    static const AutoPtr<IParcel> EMPTY_PARCEL;

    // static {
    //     EMPTY_PARCEL = Parcel.obtain();
    // }

    // Invariant - exactly one of mMap / mParcelledData will be null
    // (except inside a call to unparcel)

    AutoPtr<IArrayMap> mMap;//ArrayMap<String, Object> mMap = null;

    /*
     * If mParcelledData is non-null, then mMap will be null and the
     * data are stored as a Parcel containing a Bundle.  When the data
     * are unparcelled, mParcelledData willbe set to null.
     */
    AutoPtr<IParcel> mParcelledData;// = null;

    /**
     * The ClassLoader used when unparcelling data from mParcelledData.
     */
    AutoPtr<IClassLoader> mClassLoader;

    static const Int32 VAL_NULL;// = -1;
    static const Int32 VAL_NOT_NULL;// = 0;
    static const Int32 VAL_STRING;// = 1;
    static const Int32 VAL_INTEGER32;// = 2;
    static const Int32 VAL_MAP;// = 3;
    static const Int32 VAL_BUNDLE;// = 4;
    static const Int32 VAL_PARCELABLE;// = 5;
    static const Int32 VAL_INTEGER16;// = 6;
    static const Int32 VAL_INTEGER64;// = 7;
    static const Int32 VAL_FLOAT;// = 8;
    static const Int32 VAL_DOUBLE;// = 9;
    static const Int32 VAL_BOOLEAN;// = 10;
    static const Int32 VAL_CHARSEQUENCE;// = 11;
    static const Int32 VAL_LIST ;// = 12;
    static const Int32 VAL_SPARSEARRAY;// = 13;
    static const Int32 VAL_BYTEARRAY;// = 14;
    static const Int32 VAL_STRINGARRAY;// = 15;
    static const Int32 VAL_IBINDER;// = 16;
    static const Int32 VAL_PARCELABLEARRAY;// = 17;
    static const Int32 VAL_OBJECTARRAY;// = 18;
    static const Int32 VAL_INTARRAY;// = 19;
    static const Int32 VAL_LONGARRAY;// = 20;
    static const Int32 VAL_BYTE;// = 21;
    static const Int32 VAL_SERIALIZABLE;// = 22;
    static const Int32 VAL_SPARSEBOOLEANARRAY;// = 23;
    static const Int32 VAL_BOOLEANARRAY;// = 24;
    static const Int32 VAL_CHARSEQUENCEARRAY;// = 25;
    static const Int32 VAL_ARRAYOF;// = 26;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_BASE_BUNDLE_H__
