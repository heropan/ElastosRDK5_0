
#ifndef __ELASTOS_DROID_CONTENT_PM_CMANIFESTDIGESTHELPER_H__
#define __ELASTOS_DROID_CONTENT_PM_CMANIFESTDIGESTHELPER_H__

#include "_Elastos_Droid_Content_Pm_CManifestDigestHelper.h"

using Elastos::Utility::Jar::IAttributes;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CarClass(CManifestDigestHelper)
{
public:
    CARAPI FromAttributes(
            /* [in] */ IAttributes* attributes,
            /* [out] */ IManifestDigest** digest);
};

}
}
}
}

#endif // __ELASTOS_DROID_CONTENT_PM_CMANIFESTDIGESTHELPER_H__
