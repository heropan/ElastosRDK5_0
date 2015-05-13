
#ifndef __CATTRIBUTES_H__
#define __CATTRIBUTES_H__

#include "_CAttributes.h"

#ifdef ELASTOS_CORE
#include "Elastos.Core_server.h"
#else
#include "Elastos.Core.h"
#endif

using Elastos::Utility::IMapEntry;
using Elastos::Utility::IMap;
using Elastos::Utility::ICollection;
using Elastos::Utility::IObjectMap;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Utility {
namespace Jar {

CarClass(CAttributes)
{
public:
    CARAPI GetValue(
        /* [in] */ IName* name,
        /* [out] */ String* value);

    CARAPI GetValueEx(
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

    CARAPI EntrySet(
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

    CARAPI KeySet(
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

    CARAPI Values(
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

#endif // __CATTRIBUTES_H__
