
#include "content/CSyncResultHelper.h"
#include "content/CSyncResult.h"
#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace Content {

ECode CSyncResultHelper::GetALREADY_IN_PROGRESS(
    /* [out] */ ISyncResult** syncResult)
{
    VALIDATE_NOT_NULL(syncResult)
    return CSyncResult::New(TRUE, syncResult);
}

}
}
}
