
#ifndef __ORG_JSON_JSONOBJECT_H__
#define __ORG_JSON_JSONOBJECT_H__

#include "Elastos.CoreLibrary.External.h"
#include "Elastos.CoreLibrary.Utility.h"
#include "Object.h"

using Elastos::Core::INumber;
using Elastos::Utility::IIterator;
using Elastos::Utility::IMap;
using Elastos::Utility::ISet;
using Elastos::Utility::ILinkedHashMap;

namespace Org {
namespace Json {

// Note: this class was written without inspecting the non-free org.json sourcecode.

/**
 * A modifiable set of name/value mappings. Names are unique, non-null strings.
 * Values may be any mix of {@link JSONObject JSONObjects}, {@link JSONArray
 * JSONArrays}, Strings, Booleans, Integers, Longs, Doubles or {@link #NULL}.
 * Values may not be {@code null}, {@link Double#isNaN() NaNs}, {@link
 * Double#isInfinite() infinities}, or of any type not listed here.
 *
 * <p>This class can coerce values to another type when requested.
 * <ul>
 *   <li>When the requested type is a boolean, strings will be coerced using a
 *       case-insensitive comparison to "true" and "false".
 *   <li>When the requested type is a double, other {@link Number} types will
 *       be coerced using {@link Number#doubleValue() doubleValue}. Strings
 *       that can be coerced using {@link Double#valueOf(String)} will be.
 *   <li>When the requested type is an int, other {@link Number} types will
 *       be coerced using {@link Number#intValue() intValue}. Strings
 *       that can be coerced using {@link Double#valueOf(String)} will be,
 *       and then cast to int.
 *   <li><a name="lossy">When the requested type is a long, other {@link Number} types will
 *       be coerced using {@link Number#longValue() longValue}. Strings
 *       that can be coerced using {@link Double#valueOf(String)} will be,
 *       and then cast to long. This two-step conversion is lossy for very
 *       large values. For example, the string "9223372036854775806" yields the
 *       long 9223372036854775807.</a>
 *   <li>When the requested type is a String, other non-null values will be
 *       coerced using {@link String#valueOf(Object)}. Although null cannot be
 *       coerced, the sentinel value {@link JSONObject#NULL} is coerced to the
 *       string "null".
 * </ul>
 *
 * <p>This class can look up both mandatory and optional values:
 * <ul>
 *   <li>Use <code>get<i>Type</i>()</code> to retrieve a mandatory value. This
 *       fails with a {@code JSONException} if the requested name has no value
 *       or if the value cannot be coerced to the requested type.
 *   <li>Use <code>opt<i>Type</i>()</code> to retrieve an optional value. This
 *       returns a system- or user-supplied default if the requested name has no
 *       value or if the value cannot be coerced to the requested type.
 * </ul>
 *
 * <p><strong>Warning:</strong> this class represents null in two incompatible
 * ways: the standard Java {@code null} reference, and the sentinel value {@link
 * JSONObject#NULL}. In particular, calling {@code put(name, null)} removes the
 * named entry from the object but {@code put(name, JSONObject.NULL)} stores an
 * entry whose value is {@code JSONObject.NULL}.
 *
 * <p>Instances of this class are not thread safe. Although this class is
 * nonfinal, it was not designed for inheritance and should not be subclassed.
 * In particular, self-use by overrideable methods is not specified. See
 * <i>Effective Java</i> Item 17, "Design and Document or inheritance or else
 * prohibit it" for further information.
 */

class JSONObject
    : public Object
    , public IJSONObject
{
    friend class JSONStringer;
public:
    class MyObject
        : public Object
    {
    public:
        MyObject();

        ~MyObject();

        // @Override
        CARAPI Equals(
            /* [in] */ IInterface* o,
            /* [out] */ Boolean* res);

        // @Override
        CARAPI ToString(
            /* [out] */ String* str);
    };
public:
    CAR_INTERFACE_DECL();

    JSONObject();

    ~JSONObject();

    /**
     * Creates a {@code JSONObject} with no name/value mappings.
     */
    CARAPI constructor();

    /**
     * Creates a new {@code JSONObject} by copying all name/value mappings from
     * the given map.
     *
     * @param copyFrom a map whose keys are of type {@link String} and whose
     *     values are of supported types.
     * @throws NullPointerException if any of the map's keys are null.
     */
    /* (accept a raw type for API compatibility) */
    CARAPI constructor(
        /* [in] */ IMap* copyFrom);

    /**
     * Creates a new {@code JSONObject} with name/value mappings from the next
     * object in the tokener.
     *
     * @param readFrom a tokener whose nextValue() method will yield a
     *     {@code JSONObject}.
     * @throws JSONException if the parse fails or doesn't yield a
     *     {@code JSONObject}.
     */
    CARAPI constructor(
        /* [in] */ IJSONTokener* readFrom);

    /**
     * Creates a new {@code JSONObject} with name/value mappings from the JSON
     * string.
     *
     * @param json a JSON-encoded string containing an object.
     * @throws JSONException if the parse fails or doesn't yield a {@code
     *     JSONObject}.
     */
    CARAPI constructor(
        /* [in] */ const String& json);

    /**
     * Creates a new {@code JSONObject} by copying mappings for the listed names
     * from the given object. Names that aren't present in {@code copyFrom} will
     * be skipped.
     */
    CARAPI constructor(
        /* [in] */ IJSONObject* copyFrom,
        /* [in] */ ArrayOf<String>* names);

    /**
     * Returns the number of name/value mappings in this object.
     */
    CARAPI GetLength(
        /* [out] */ Int32* len);

    /**
     * Maps {@code name} to {@code value}, clobbering any existing name/value
     * mapping with the same name.
     *
     * @return this object.
     */
    CARAPI Put(
        /* [in] */ const String& name,
        /* [in] */ Boolean value);

    /**
     * Maps {@code name} to {@code value}, clobbering any existing name/value
     * mapping with the same name.
     *
     * @param value a finite value. May not be {@link Double#isNaN() NaNs} or
     *     {@link Double#isInfinite() infinities}.
     * @return this object.
     */
    CARAPI Put(
        /* [in] */ const String& name,
        /* [in] */ Double value);

    /**
     * Maps {@code name} to {@code value}, clobbering any existing name/value
     * mapping with the same name.
     *
     * @return this object.
     */
    CARAPI Put(
        /* [in] */ const String& name,
        /* [in] */ Int32 value);

    /**
     * Maps {@code name} to {@code value}, clobbering any existing name/value
     * mapping with the same name.
     *
     * @return this object.
     */
    CARAPI Put(
        /* [in] */ const String& name,
        /* [in] */ Int64 value);

    /**
     * Maps {@code name} to {@code value}, clobbering any existing name/value
     * mapping with the same name. If the value is {@code null}, any existing
     * mapping for {@code name} is removed.
     *
     * @param value a {@link JSONObject}, {@link JSONArray}, String, Boolean,
     *     Integer, Long, Double, {@link #NULL}, or {@code null}. May not be
     *     {@link Double#isNaN() NaNs} or {@link Double#isInfinite()
     *     infinities}.
     * @return this object.
     */
    CARAPI Put(
        /* [in] */ const String& name,
        /* [in] */ IInterface* value);

    /**
     * Equivalent to {@code put(name, value)} when both parameters are non-null;
     * does nothing otherwise.
     */
    CARAPI PutOpt(
        /* [in] */ const String& name,
        /* [in] */ IInterface* value);

    /**
     * Appends {@code value} to the array already mapped to {@code name}. If
     * this object has no mapping for {@code name}, this inserts a new mapping.
     * If the mapping exists but its value is not an array, the existing
     * and new values are inserted in order into a new array which is itself
     * mapped to {@code name}. In aggregate, this allows values to be added to a
     * mapping one at a time.
     *
     * <p> Note that {@code append(String, Object)} provides better semantics.
     * In particular, the mapping for {@code name} will <b>always</b> be a
     * {@link JSONArray}. Using {@code accumulate} will result in either a
     * {@link JSONArray} or a mapping whose type is the type of {@code value}
     * depending on the number of calls to it.
     *
     * @param value a {@link JSONObject}, {@link JSONArray}, String, Boolean,
     *     Integer, Long, Double, {@link #NULL} or null. May not be {@link
     *     Double#isNaN() NaNs} or {@link Double#isInfinite() infinities}.
     */
    // TODO: Change {@code append) to {@link #append} when append is
    // unhidden.
    CARAPI Accumulate(
        /* [in] */ const String& name,
        /* [in] */ IInterface* value);

    /**
     * Appends values to the array mapped to {@code name}. A new {@link JSONArray}
     * mapping for {@code name} will be inserted if no mapping exists. If the existing
     * mapping for {@code name} is not a {@link JSONArray}, a {@link JSONException}
     * will be thrown.
     *
     * @throws JSONException if {@code name} is {@code null} or if the mapping for
     *         {@code name} is non-null and is not a {@link JSONArray}.
     *
     * @hide
     */
    CARAPI Append(
        /* [in] */ const String& name,
        /* [in] */ IInterface* value);

    /**
     * Removes the named mapping if it exists; does nothing otherwise.
     *
     * @return the value previously mapped by {@code name}, or null if there was
     *     no such mapping.
     */
    CARAPI Remove(
        /* [in] */ const String& name,
        /* [out] */ IInterface** value);

    /**
     * Returns true if this object has no mapping for {@code name} or if it has
     * a mapping whose value is {@link #NULL}.
     */
    CARAPI IsNull(
        /* [in] */ const String& name,
        /* [out] */ Boolean* res);

    /**
     * Returns true if this object has a mapping for {@code name}. The mapping
     * may be {@link #NULL}.
     */
    CARAPI Has(
        /* [in] */ const String& name,
        /* [out] */ Boolean* res);

    /**
     * Returns the value mapped by {@code name}, or throws if no such mapping exists.
     *
     * @throws JSONException if no such mapping exists.
     */
    CARAPI Get(
        /* [in] */ const String& name,
        /* [out] */ IInterface** obj);

    /**
     * Returns the value mapped by {@code name}, or null if no such mapping
     * exists.
     */
    CARAPI Opt(
        /* [in] */ const String& name,
        /* [out] */ IInterface** obj);

    /**
     * Returns the value mapped by {@code name} if it exists and is a boolean or
     * can be coerced to a boolean, or throws otherwise.
     *
     * @throws JSONException if the mapping doesn't exist or cannot be coerced
     *     to a boolean.
     */
    CARAPI GetBoolean(
        /* [in] */ const String& name,
        /* [out] */ Boolean* res);

    /**
     * Returns the value mapped by {@code name} if it exists and is a boolean or
     * can be coerced to a boolean, or false otherwise.
     */
    CARAPI OptBoolean(
        /* [in] */ const String& name,
        /* [out] */ Boolean* res);

    /**
     * Returns the value mapped by {@code name} if it exists and is a boolean or
     * can be coerced to a boolean, or {@code fallback} otherwise.
     */
    CARAPI OptBoolean(
        /* [in] */ const String& name,
        /* [in] */ Boolean fallback,
        /* [out] */ Boolean* res);

    /**
     * Returns the value mapped by {@code name} if it exists and is a double or
     * can be coerced to a double, or throws otherwise.
     *
     * @throws JSONException if the mapping doesn't exist or cannot be coerced
     *     to a double.
     */
    CARAPI GetDouble(
        /* [in] */ const String& name,
        /* [out] */ Double* data);

    /**
     * Returns the value mapped by {@code name} if it exists and is a double or
     * can be coerced to a double, or {@code NaN} otherwise.
     */
    CARAPI OptDouble(
        /* [in] */ const String& name,
        /* [out] */ Double* data);

    /**
     * Returns the value mapped by {@code name} if it exists and is a double or
     * can be coerced to a double, or {@code fallback} otherwise.
     */
    CARAPI OptDouble(
        /* [in] */ const String& name,
        /* [in] */ Double fallback,
        /* [out] */ Double* data);

    /**
     * Returns the value mapped by {@code name} if it exists and is an int or
     * can be coerced to an int, or throws otherwise.
     *
     * @throws JSONException if the mapping doesn't exist or cannot be coerced
     *     to an int.
     */
    CARAPI GetInt32(
        /* [in] */ const String& name,
        /* [out] */ Int32* data);

    /**
     * Returns the value mapped by {@code name} if it exists and is an int or
     * can be coerced to an int, or 0 otherwise.
     */
    CARAPI OptInt32(
        /* [in] */ const String& name,
        /* [out] */ Int32* data);

    /**
     * Returns the value mapped by {@code name} if it exists and is an int or
     * can be coerced to an int, or {@code fallback} otherwise.
     */
    CARAPI OptInt32(
        /* [in] */ const String& name,
        /* [in] */ Int32 fallback,
        /* [out] */ Int32* data);

    /**
     * Returns the value mapped by {@code name} if it exists and is a long or
     * can be coerced to a long, or throws otherwise.
     * Note that JSON represents numbers as doubles,
     * so this is <a href="#lossy">lossy</a>; use strings to transfer numbers via JSON.
     *
     * @throws JSONException if the mapping doesn't exist or cannot be coerced
     *     to a long.
     */
    CARAPI GetInt64(
        /* [in] */ const String& name,
        /* [out] */ Int64* data);

    /**
     * Returns the value mapped by {@code name} if it exists and is a long or
     * can be coerced to a long, or 0 otherwise. Note that JSON represents numbers as doubles,
     * so this is <a href="#lossy">lossy</a>; use strings to transfer numbers via JSON.
     */
    CARAPI OptInt64(
        /* [in] */ const String& name,
        /* [out] */ Int64* data);

    /**
     * Returns the value mapped by {@code name} if it exists and is a long or
     * can be coerced to a long, or {@code fallback} otherwise. Note that JSON represents
     * numbers as doubles, so this is <a href="#lossy">lossy</a>; use strings to transfer
     * numbers via JSON.
     */
    CARAPI OptInt64(
        /* [in] */ const String& name,
        /* [in] */ Int64 fallback,
        /* [out] */ Int64* data);

    /**
     * Returns the value mapped by {@code name} if it exists, coercing it if
     * necessary, or throws if no such mapping exists.
     *
     * @throws JSONException if no such mapping exists.
     */
    CARAPI GetString(
        /* [in] */ const String& name,
        /* [out] */ String* str);

    /**
     * Returns the value mapped by {@code name} if it exists, coercing it if
     * necessary, or the empty string if no such mapping exists.
     */
    CARAPI OptString(
        /* [in] */ const String& name,
        /* [out] */ String* str);

    /**
     * Returns the value mapped by {@code name} if it exists, coercing it if
     * necessary, or {@code fallback} if no such mapping exists.
     */
    CARAPI OptString(
        /* [in] */ const String& name,
        /* [in] */ const String& fallback,
        /* [out] */ String* str);

    /**
     * Returns the value mapped by {@code name} if it exists and is a {@code
     * JSONArray}, or throws otherwise.
     *
     * @throws JSONException if the mapping doesn't exist or is not a {@code
     *     JSONArray}.
     */
    CARAPI GetJSONArray(
        /* [in] */ const String& name,
        /* [out] */ IJSONArray** jsonArray);

    /**
     * Returns the value mapped by {@code name} if it exists and is a {@code
     * JSONArray}, or null otherwise.
     */
    CARAPI OptJSONArray(
        /* [in] */ const String& name,
        /* [out] */ IJSONArray** jsonArray);

    /**
     * Returns the value mapped by {@code name} if it exists and is a {@code
     * JSONObject}, or throws otherwise.
     *
     * @throws JSONException if the mapping doesn't exist or is not a {@code
     *     JSONObject}.
     */
    CARAPI GetJSONObject(
        /* [in] */ const String& name,
        /* [out] */ IJSONObject** jsonObject);

    /**
     * Returns the value mapped by {@code name} if it exists and is a {@code
     * JSONObject}, or null otherwise.
     */
    CARAPI OptJSONObject(
        /* [in] */ const String& name,
        /* [out] */ IJSONObject** jsonObject);

    /**
     * Returns an array with the values corresponding to {@code names}. The
     * array contains null for names that aren't mapped. This method returns
     * null if {@code names} is either null or empty.
     */
    CARAPI ToJSONArray(
        /* [in] */ IJSONArray* names,
        /* [out] */ IJSONArray** jsonArray);

    /**
     * Returns an iterator of the {@code String} names in this object. The
     * returned iterator supports {@link Iterator#remove() remove}, which will
     * remove the corresponding mapping from this object. If this object is
     * modified after the iterator is returned, the iterator's behavior is
     * undefined. The order of the keys is undefined.
     */
    CARAPI GetKeys(
        /* [out] */ IIterator** iterator);

    /**
     * Returns the set of {@code String} names in this object. The returned set
     * is a view of the keys in this object. {@link Set#remove(Object)} will remove
     * the corresponding mapping from this object and set iterator behaviour
     * is undefined if this object is modified after it is returned.
     *
     * See {@link #keys()}.
     *
     * @hide.
     */
    CARAPI GetKeySet(
        /* [out] */ ISet** set);

    /**
     * Returns an array containing the string names in this object. This method
     * returns null if this object contains no mappings.
     */
    CARAPI GetNames(
        /* [out] */ IJSONArray** jsonArray);

    /**
     * Encodes this object as a compact JSON string, such as:
     * <pre>{"query":"Pizza","locations":[94043,90210]}</pre>
     */
    // @Override
    CARAPI ToString(
        /* [out] */ String* str);

    /**
     * Encodes this object as a human readable JSON string for debugging, such
     * as:
     * <pre>
     * {
     *     "query": "Pizza",
     *     "locations": [
     *         94043,
     *         90210
     *     ]
     * }</pre>
     *
     * @param indentSpaces the number of spaces to indent for each level of
     *     nesting.
     */
    CARAPI ToString(
        /* [in] */ Int32 indentSpaces,
        /* [out] */ String* str);

    /**
     * Encodes the number as a JSON string.
     *
     * @param number a finite value. May not be {@link Double#isNaN() NaNs} or
     *     {@link Double#isInfinite() infinities}.
     */
    static CARAPI NumberToString(
        /* [in] */ INumber* number,
        /* [out] */ String* str);

    /**
     * Encodes {@code data} as a JSON string. This applies quotes and any
     * necessary character escaping.
     *
     * @param data the string to encode. Null will be interpreted as an empty
     *     string.
     */
    static CARAPI Quote(
        /* [in] */ const String& data,
        /* [out] */ String* str);

    /**
     * Wraps the given object if necessary.
     *
     * <p>If the object is null or , returns {@link #NULL}.
     * If the object is a {@code JSONArray} or {@code JSONObject}, no wrapping is necessary.
     * If the object is {@code NULL}, no wrapping is necessary.
     * If the object is an array or {@code Collection}, returns an equivalent {@code JSONArray}.
     * If the object is a {@code Map}, returns an equivalent {@code JSONObject}.
     * If the object is a primitive wrapper type or {@code String}, returns the object.
     * Otherwise if the object is from a {@code java} package, returns the result of {@code toString}.
     * If wrapping fails, returns null.
     */
    static CARAPI Wrap(
        /* [in] */ IInterface* o,
        /* [out] */ IInterface** obj);

protected:
    CARAPI CheckName(
        /* [in] */ const String& name);

    CARAPI WriteTo(
        /* [in] */ IJSONStringer* stringer);

public:
    /**
     * A sentinel value used to explicitly define a name with no value. Unlike
     * {@code null}, names with this value:
     * <ul>
     *   <li>show up in the {@link #names} array
     *   <li>show up in the {@link #keys} iterator
     *   <li>return {@code true} for {@link #has(String)}
     *   <li>do not throw on {@link #get(String)}
     *   <li>are included in the encoded JSON string.
     * </ul>
     *
     * <p>This value violates the general contract of {@link Object#equals} by
     * returning true when compared to {@code null}. Its {@link #toString}
     * method returns "null".
     */
    static const AutoPtr<IInterface> Object_NULL;

private:
    static const Double NEGATIVE_ZERO;

    AutoPtr<ILinkedHashMap> mNameValuePairs;
    /*private final LinkedHashMap<String, Object> nameValuePairs;*/
};

} //namespace Json
} //namespace Org

#endif //__ORG_JSON_JSONOBJECT_H__
