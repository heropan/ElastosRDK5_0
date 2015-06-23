
#ifndef __CCONTENTVALUES_H__
#define __CCONTENTVALUES_H__

#include "_CContentValues.h"
#include "ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::IObjectStringMap;
using Elastos::Core::ICharSequence;
using Elastos::Core::IByte;
using Elastos::Core::IInteger16;
using Elastos::Core::IInteger32;
using Elastos::Core::IInteger64;
using Elastos::Core::IFloat;
using Elastos::Core::IDouble;
using Elastos::Core::IBoolean;
using Elastos::Core::IArrayOf;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CContentValues)
{
public:
    CContentValues();

    ~CContentValues();

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashcode);

    /**
     * Adds a value to the set.
     *
     * @param key the name of the value to put
     * @param value the data for the value to put
     */
    CARAPI PutString(
        /* [in] */ const String& key,
        /* [in] */ ICharSequence* value);

    /**
     * Adds all values from the passed in ContentValues.
     *
     * @param other the ContentValues from which to copy
     */
    CARAPI PutAll(
        /* [in] */ IContentValues* contentvalues);

    /**
     * Adds a value to the set.
     *
     * @param key the name of the value to put
     * @param value the data for the value to put
     */
    CARAPI PutByte(
        /* [in] */ const String& key,
        /* [in] */ IByte* value);

    /**
     * Adds a value to the set.
     *
     * @param key the name of the value to put
     * @param value the data for the value to put
     */
    CARAPI PutInt16(
        /* [in] */ const String& key,
        /* [in] */ IInteger16* value);

    /**
     * Adds a value to the set.
     *
     * @param key the name of the value to put
     * @param value the data for the value to put
     */
    CARAPI PutInt32(
        /* [in] */ const String& key,
        /* [in] */ IInteger32* value);

    /**
     * Adds a value to the set.
     *
     * @param key the name of the value to put
     * @param value the data for the value to put
     */
    CARAPI PutInt64(
        /* [in] */ const String& key,
        /* [in] */ IInteger64* value);

    /**
     * Adds a value to the set.
     *
     * @param key the name of the value to put
     * @param value the data for the value to put
     */
    CARAPI PutFloat(
        /* [in] */ const String& key,
        /* [in] */ IFloat* value);

    /**
     * Adds a value to the set.
     *
     * @param key the name of the value to put
     * @param value the data for the value to put
     */
    CARAPI PutDouble(
        /* [in] */ const String& key,
        /* [in] */ IDouble* value);

    /**
     * Adds a value to the set.
     *
     * @param key the name of the value to put
     * @param value the data for the value to put
     */
    CARAPI PutBoolean(
        /* [in] */ const String& key,
        /* [in] */ IBoolean* value);

    /**
     * Adds a value to the set.
     *
     * @param key the name of the value to put
     * @param value the data for the value to put
     */
    CARAPI PutBytes(
        /* [in] */ const String& key,
        /* [in] */ IArrayOf* value);

    /**
     * Adds a null value to the set.
     *
     * @param key the name of the value to make null
     */
    CARAPI PutNull(
        /* [in] */ const String& key);

    /**
     * Returns the number of values.
     *
     * @return the number of values
     */
    CARAPI GetSize(
        /* [out] */ Int32* size);

    /**
     * Remove a single value.
     *
     * @param key the name of the value to remove
     */
    CARAPI Remove(
        /* [in] */ const String& key);

    /**
     * Removes all values.
     */
    CARAPI Clear();

    /**
     * Returns true if this object has the named value.
     *
     * @param key the value to check for
     * @return {@code true} if the value is present, {@code false} otherwise
     */
    CARAPI ContainsKey(
        /* [in] */ const String& key,
        /* [out] */ Boolean* result);

    /**
     * Gets a value. Valid value types are {@link String}, {@link Boolean}, and
     * {@link Number} implementations.
     *
     * @param key the value to get
     * @return the data for the value
     */
    CARAPI Get(
        /* [in] */ const String& key,
        /* [out] */ IInterface** value);

    /**
     * Gets a value and converts it to a String.
     *
     * @param key the value to get
     * @return the String for the value
     */
    CARAPI GetAsString(
        /* [in] */ const String& key,
        /* [out] */ String* value);

    /**
     * Gets a value and converts it to a Long.
     *
     * @param key the value to get
     * @return the Long value, or null if the value is missing or cannot be converted
     */
    CARAPI GetAsInt64(
        /* [in] */ const String& key,
        /* [out] */ Int64* value);

    /**
     * Gets a value and converts it to an Integer.
     *
     * @param key the value to get
     * @return the Integer value, or null if the value is missing or cannot be converted
     */
    CARAPI GetAsInt32(
        /* [in] */ const String& key,
        /* [out] */ Int32* value);

    /**
     * Gets a value and converts it to a Short.
     *
     * @param key the value to get
     * @return the Short value, or null if the value is missing or cannot be converted
     */
    CARAPI GetAsInt16(
        /* [in] */ const String& key,
        /* [out] */ Int16* value);

    /**
     * Gets a value and converts it to a Byte.
     *
     * @param key the value to get
     * @return the Byte value, or null if the value is missing or cannot be converted
     */
    CARAPI GetAsByte(
        /* [in] */ const String& key,
        /* [out] */ Byte* value);

    /**
     * Gets a value and converts it to a Double.
     *
     * @param key the value to get
     * @return the Double value, or null if the value is missing or cannot be converted
     */
    CARAPI GetAsDouble(
        /* [in] */ const String& key,
        /* [out] */ Double* value);

    /**
     * Gets a value and converts it to a Float.
     *
     * @param key the value to get
     * @return the Float value, or null if the value is missing or cannot be converted
     */
    CARAPI GetAsFloat(
        /* [in] */ const String& key,
        /* [out] */ Float* value);

    /**
     * Gets a value and converts it to a Boolean.
     *
     * @param key the value to get
     * @return the Boolean value, or null if the value is missing or cannot be converted
     */
    CARAPI GetAsBoolean(
        /* [in] */ const String& key,
        /* [out] */ Boolean* value);

    /**
     * Gets a value that is a byte array. Note that this method will not convert
     * any other types to byte arrays.
     *
     * @param key the value to get
     * @return the byte[] value, or null is the value is missing or not a byte[]
     */
    CARAPI GetAsByteArray(
        /* [in] */ const String& key,
        /* [out, callee] */ ArrayOf<Byte>** value);

    /**
     * Returns a set of all of the keys and values
     *
     * @return a set of all of the keys and values
     */
    CARAPI ValueSet(
        /* [out] */ IObjectStringMap** values);

    /**
     * Returns a set of all of the keys
     *
     * @return a set of all of the keys
     */
    CARAPI KeySet(
        /* [out, callee] */ ArrayOf<String>** value);

    /**
     * Returns a string containing a concise, human-readable description of this object.
     * @return a printable representation of this object.
     */
    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    /**
     * Creates an empty set of values using the default initial size
     */
    CARAPI constructor();

    /**
     * Creates an empty set of values using the given initial size
     *
     * @param size the initial size of the set of values
     */
    CARAPI constructor(
        /* [in] */ Int32 size);

    /**
     * Creates a set of values copied from the given set
     *
     * @param from the values to copy
     */
    CARAPI constructor(
        /* [in] */ IContentValues* from);

private:
    CARAPI WriteValue(
        /* [in] */ IParcel* dest,
        /* [in] */ IInterface* obj);

    CARAPI_(AutoPtr<IInterface>) ReadValue(
        /* [in] */ IParcel* source);

private:
    static const Int32 VAL_NULL = -1;
    static const Int32 VAL_STRING = 0;
    static const Int32 VAL_BYTE = 1;
    static const Int32 VAL_INTEGER16 = 2;
    static const Int32 VAL_INTEGER32 = 3;
    static const Int32 VAL_INTEGER64 = 4;
    static const Int32 VAL_FLOAT = 5;
    static const Int32 VAL_DOUBLE = 6;
    static const Int32 VAL_BOOLEAN = 7;
    static const Int32 VAL_ARRAYOF = 8;

public:
    static const String TAG;

private:
    /** Holds the actual values */
    HashMap<String, AutoPtr<IInterface> > mValues;

};

}
}
}

#endif // __CCONTENTVALUES_H__
