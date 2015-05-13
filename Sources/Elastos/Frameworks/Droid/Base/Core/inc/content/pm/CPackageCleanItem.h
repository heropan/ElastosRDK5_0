
#ifndef __CPACKAGECLEANITEM_H__
#define __CPACKAGECLEANITEM_H__

#include "_CPackageCleanItem.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/** @hide */
CarClass(CPackageCleanItem)
{
public:
    CPackageCleanItem();

    ~CPackageCleanItem();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 userId,
        /* [in] */ const String& packageName,
        /* [in] */ Boolean andCode);

    CARAPI Equals(
        /* [in] */ IPackageCleanItem* obj,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* code);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetUserId(
        /* [out] */ Int32* userId);

    CARAPI GetPackageName(
        /* [out] */ String* packageName);

    CARAPI GetAndCode(
        /* [out] */ Boolean* andCode);

public:
    Int32 mUserId;
    String mPackageName;
    Boolean mAndCode;
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __CPACKAGECLEANITEM_H__
