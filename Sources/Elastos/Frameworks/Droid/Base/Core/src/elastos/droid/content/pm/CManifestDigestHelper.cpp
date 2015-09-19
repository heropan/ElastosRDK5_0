
#include "content/pm/CManifestDigestHelper.h"
#include "content/pm/CManifestDigest.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

ECode CManifestDigestHelper::FromAttributes(
        /* [in] */ IAttributes* attributes,
        /* [out] */ IManifestDigest** digest)
{
    return CManifestDigest::FromAttributes(attributes, digest);
}

}
}
}
}
