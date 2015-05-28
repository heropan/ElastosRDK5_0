#ifndef __UTILITY_MAPENTRY_H__
#define __UTILITY_MAPENTRY_H__

#include "Elastos.CoreLibrary_server.h"

namespace Elastos {
namespace Utility {

class MapEntry
    : public ElLightRefBase
    , public IMapEntry
{
public:
    class Type
        : public IInterface
        , public ElLightRefBase
    {
    public:
        CAR_INTERFACE_DECL();

        virtual CARAPI Get(
            /* [in] */ IMapEntry* entry,
            /* [out] */ IInterface** value) = 0;
    };

public:
    MapEntry(
        /* [in] */ IInterface* theKey)
        : mKey(theKey)
    {}

    MapEntry(
        /* [in] */ IInterface* theKey,
        /* [in] */ IInterface* theValue)
        : mKey(theKey)
        , mValue(theValue)
    {}

    CAR_INTERFACE_DECL();

    CARAPI Clone(
        /* [out] */ IInterface** outface);

    CARAPI Equals(
        /* [in] */ IInterface* entry,
        /* [out] */ Boolean* result);

    CARAPI GetKey(
        /* [out] */ IInterface** key);

    CARAPI GetValue(
        /* [out] */ IInterface** value);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI SetValue(
        /* [in] */ IInterface* object,
        /* [out] */ IInterface** oldObject);

    CARAPI ToString(
        /* [out] */ String* str);

public:
    AutoPtr<IInterface> mKey;
    AutoPtr<IInterface> mValue;
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_MAPENTRY_H__
