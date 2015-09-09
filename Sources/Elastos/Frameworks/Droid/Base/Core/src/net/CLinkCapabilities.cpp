
#include "net/CLinkCapabilities.h"
#include <elastos/core/StringBuffer.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::CObjectInt32Map;
using Elastos::Core::StringBuilder;
using Elastos::Core::CString;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Net {

const String CLinkCapabilities::TAG("LinkCapabilities");
const Boolean CLinkCapabilities::DBG = FALSE;
/**
 * Constructor
 */
ECode CLinkCapabilities::constructor()
{
    CObjectInt32Map::New((IObjectInt32Map**)&mCapabilities);
    return NOERROR;
}

/**
 * Copy constructor.
 *
 * @param source
 */
ECode CLinkCapabilities::constructor(
    /* [in] */ ILinkCapabilities* source)
{
    if (source != NULL) {
        source->GetCapabilities((IObjectInt32Map**)&mCapabilities);
    } else {
        CObjectInt32Map::New((IObjectInt32Map**)&mCapabilities);
    }
    return NOERROR;
}

/**
 * Create the {@code LinkCapabilities} with values depending on role type.
 * @param applicationRole a {@code LinkSocket.Role}
 * @return the {@code LinkCapabilities} associated with the applicationRole, empty if none
 */
ECode CLinkCapabilities::CreateNeedsMap(
    /* [in] */ const String& applicationRole,
    /* [out] */ ILinkCapabilities** result)
{
//    if (DBG) log("createNeededCapabilities(applicationRole) EX");
    return CLinkCapabilities::New(result);
}

/**
 * Remove all capabilities
 */
ECode CLinkCapabilities::Clear()
{
    AutoPtr< ArrayOf<Int32> > keys;
    GetKeySet((ArrayOf<Int32>**)&keys);
    Int32 size = keys->GetLength();
    for (Int32 i = 0; i < size ;i++) {
        mCapabilities->Remove((*keys)[i]);
    }
    return NOERROR;
}

/**
 * Returns whether this map is empty.
 */
ECode CLinkCapabilities::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 size;
    mCapabilities->GetSize(&size);
    if (size == 0)
        *result = TRUE;
    else
        *result = FALSE;

    return NOERROR;
}

/**
 * Returns the number of elements in this map.
 *
 * @return the number of elements in this map.
 */
ECode CLinkCapabilities::GetSize(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return mCapabilities->GetSize(result);
}

/**
 * Given the key return the capability string
 *
 * @param key
 * @return the capability string
 */
ECode CLinkCapabilities::GetValue(
    /* [in] */ Int32 key,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ICharSequence> value;
    mCapabilities->Get(key, (IInterface**)&value);
    value->ToString(result);
    return NOERROR;
}

/**
 * Store the key/value capability pair
 *
 * @param key
 * @param value
 */
ECode CLinkCapabilities::GetCapabilities(
     /* [out] */ IObjectInt32Map** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mCapabilities;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CLinkCapabilities::Put(
    /* [in] */ Int32 key,
    /* [in] */ const String& value)
{
    AutoPtr<ICharSequence> temp;
    CString::New(value, (ICharSequence**)&temp);
    return mCapabilities->Put(key, temp);
}

/**
 * Returns whether this map contains the specified key.
 *
 * @param key to search for.
 * @return {@code TRUE} if this map contains the specified key,
 *         {@code FALSE} otherwise.
 */
ECode CLinkCapabilities::ContainsKey(
    /* [in] */ Int32 key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mCapabilities->ContainsKey(key, result);
}

/**
 * Returns whether this map contains the specified value.
 *
 * @param value to search for.
 * @return {@code TRUE} if this map contains the specified value,
 *         {@code FALSE} otherwise.
 */
ECode CLinkCapabilities::ContainsValue(
    /* [in] */ const String& value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr< ArrayOf<Int32> > keys;
    GetKeySet((ArrayOf<Int32>**)&keys);
    Int32 size = keys->GetLength();
    for (Int32 i = 0; i < size ;i++) {
        String oriValue;
        GetValue((*keys)[i], &oriValue);
        if (value.Equals(oriValue)) {
            *result = TRUE;
            return NOERROR;
        }
    }
    *result = FALSE;
    return NOERROR;
}

/**
 * Returns a set containing all of the mappings in this map. Each mapping is
 * an instance of {@link Map.Entry}. As the set is backed by this map,
 * changes in one will be reflected in the other.
 *
 * @return a set of the mappings.
 */
ECode CLinkCapabilities::GetEntrySet(
    /* [out, callee] */ ArrayOf<Int32>** keys,
    /* [out] */ IObjectContainer** values)
{
    VALIDATE_NOT_NULL(values);
    *values = NULL;
    VALIDATE_NOT_NULL(keys);
    return mCapabilities->GetAllItems(keys, values);
}

/**
 * @return the set of the keys.
 */
ECode CLinkCapabilities::GetKeySet(
    /* [out, callee] */ ArrayOf<Int32>** keys)
{
    VALIDATE_NOT_NULL(keys);
    return mCapabilities->GetKeys(keys);
}

/**
 * @return the set of values
 */
ECode CLinkCapabilities::GetValues(
    /* [out] */ IObjectContainer** values)
{
    VALIDATE_NOT_NULL(values);
    return mCapabilities->GetValues(values);
}

/**
 * Convert to string for debugging
 */
//@Override
ECode CLinkCapabilities::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr< ArrayOf<Int32> > keys;
    GetKeySet((ArrayOf<Int32>**)&keys);

    Int32 size = keys->GetLength();
    StringBuilder sb("{");
    Boolean firstTime = TRUE;

    for (Int32 i = 0; i < size ;i++) {
        if (firstTime) {
            firstTime = FALSE;
        }
        else {
            sb += String(",");
        }
        String key = StringUtils::ToString((*keys)[i]);
        sb += key;
        sb += String(":\"");
        String value;
        GetValue((*keys)[i], &value);
        sb += value;
        sb += String("\"");
        //mCapabilities.toString();
    }
    *result = sb.ToString();
    return NOERROR;
}

/**
 * Implement the Parcelable interface.
 * @hide
 */
ECode CLinkCapabilities::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    AutoPtr< ArrayOf<Int32> > keys;
    GetKeySet((ArrayOf<Int32>**)&keys);

    Int32 size = keys->GetLength();
    dest->WriteInt32(size);
    for (Int32 i = 0; i < size ;i++) {
        dest->WriteInt32((*keys)[i]);
        String value;
        GetValue((*keys)[i], &value);
        dest->WriteString(value);
    }
    return NOERROR;
}

ECode CLinkCapabilities::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);

    Int32 size;
    source->ReadInt32(&size);

    while (size-- != 0) {
        Int32 key;
        source->ReadInt32(&key);
        String value;
        source->ReadString(&value);
        AutoPtr<ICharSequence> temp;
        CString::New(value, (ICharSequence**)&temp);
        mCapabilities->Put(key, temp);
    }
    return NOERROR;
}

/**
 * Debug logging
 */
void CLinkCapabilities::Log(
    /* [in] */ const String& s)
{
//    Log.d(TAG, s);
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
