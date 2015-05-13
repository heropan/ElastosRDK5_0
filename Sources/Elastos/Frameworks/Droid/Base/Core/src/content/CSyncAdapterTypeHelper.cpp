
#include "content/CSyncAdapterTypeHelper.h"
#include "content/CSyncAdapterType.h"
#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace Content {

ECode CSyncAdapterTypeHelper::NewKey(
    /* [in] */ const String& authority,
    /* [in] */ const String& accountType,
    /* [out] */ ISyncAdapterType** syncAdapterType)
{
    VALIDATE_NOT_NULL(syncAdapterType);
    return CSyncAdapterType::New(authority, accountType, syncAdapterType);
}

}
}
}

