
#ifndef __CMANIFESTDIGESTHELPER_H__
#define __CMANIFESTDIGESTHELPER_H__

#include "_CManifestDigestHelper.h"

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

#endif // __CMANIFESTDIGESTHELPER_H__
