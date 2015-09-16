
#ifndef __ELASTOS_DROID_CONTENT_RES_RESOURCESKEY_H__
#define __ELASTOS_DROID_CONTENT_RES_RESOURCESKEY_H__

#include "ext/frameworkext.h"
#include <elastos/core/Object.h>

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

/** @hide */
class ResourcesKey
    : public Object
    , public IResourcesKey
{
public:
    CAR_INTERFACE_DECL()

    ResourcesKey(
        /* [in] */ const String& resDir,
        /* [in] */ Int32 displayId,
        /* [in] */ IConfiguration* overrideConfiguration,
        /* [in] */ Float scale,
        /* [in] */ IBinder* token);

    Boolean HasOverrideConfiguration();

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* result);

    CARAPI ToString(
        /* [out] */ String* str);

public:
    Int32 mDisplayId;
    AutoPtr<IConfiguration> mOverrideConfiguration;// = new Configuration();

private:
    String mResDir;
    Float mScale;
    Int32 mHash;
    AutoPtr<IBinder> mToken;
};

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_RES_RESOURCESKEY_H__
