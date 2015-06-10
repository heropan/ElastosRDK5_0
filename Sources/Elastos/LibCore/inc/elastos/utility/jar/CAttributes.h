
#ifndef __ELASTOS_UTILITY_CATTRIBUTES_H__
#define __ELASTOS_UTILITY_CATTRIBUTES_H__

#include "_Elastos_Utility_Jar_CAttributes.h"
#include "Object.h"

using Elastos::Utility::IMapEntry;
using Elastos::Utility::IMap;
using Elastos::Utility::ICollection;
using Elastos::Utility::IObjectMap;
using Elastos::Core::ICharSequence;
using Elastos::Core::Object;

namespace Elastos {
namespace Utility {
namespace Jar {

CarClass(CAttributes)
    , public Object
    , public IAttributes
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI GetValue(
        /* [in] */ IName* name,
        /* [out] */ String* value);

    CARAPI GetValue(
        /* [in] */ const String& name,
        /* [out] */ String* value);

    CARAPI PutValue(
        /* [in] */ const String& name,
        /* [in] */ const String& val,
        /* [out] */ String* oldVal);

    CARAPI Clone(
        /* [out] */ IInterface** object);

    CARAPI Clear();

    CARAPI ContainsKey(
        /* [in] */ IInterface* key,
        /* [out] */ Boolean* result);

    CARAPI ContainsValue(
        /* [in] */ IInterface* value,
        /* [out] */ Boolean* result);

    CARAPI GetEntrySet(
        /* [out] */ ISet** entries);

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

    CARAPI GetKeySet(
        /* [out] */ ISet** keySet);

    CARAPI Put(
        /* [in] */ PInterface key,
        /* [in] */ PInterface value,
        /* [out] */ PInterface* oldValue);

    CARAPI PutAll(
        /* [in] */ IMap* attrib);

    CARAPI Remove(
        /* [in] */ PInterface key,
        /* [out] */ PInterface* value);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI GetValues(
        /* [out] */ ICollection** value);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IAttributes* attributes);

    CARAPI constructor(
        /* [in] */ Int32 size);

public:
    /**
     * The {@code Attributes} as name/value pairs. Maps the attribute names (as
     * {@link Attributes.Name}) of a JAR file manifest to arbitrary values. The
     * attribute names thus are obtained from the {@link Manifest} for
     * convenience.
     */
    AutoPtr<IMap> mMap;
};

} // namespace Jar
} // namespace Utility
} // namespace Elastos

#endif // __ELASTOS_UTILITY_CATTRIBUTES_H__
