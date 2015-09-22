
#ifndef __ELASTOS_DROID_CONTENT_PM_CPATHPERMISSION_H__
#define __ELASTOS_DROID_CONTENT_PM_CPATHPERMISSION_H__

#include "_Elastos_Droid_Content_Pm_CPathPermission.h"
#include "os/PatternMatcher.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/**
 * Description of permissions needed to access a particular path
 * in a {@link ProviderInfo}.
 */
CarClass(CPathPermission)
    , public Elastos::Droid::Os::PatternMatcher
    , public IPathPermission
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& pattern,
        /* [in] */ Int32 type,
        /* [in] */ const String& readPermission,
        /* [in] */ const String& writePermission);

    CARAPI GetReadPermission(
        /* [out] */ String* readPermission);

    CARAPI GetWritePermission(
        /* [out] */ String* writePermission);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetType(
        /* [out] */ Int32* type);

    CARAPI Match(
        /* [in] */ const String& str,
        /* [out] */ Boolean* isMatch);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    String mReadPermission;
    String mWritePermission;
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_PM_CPATHPERMISSION_H__
