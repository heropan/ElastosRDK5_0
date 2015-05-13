
#include "content/CIntentFilterComparison.h"
#include <ext/frameworkdef.h>

namespace Elastos {
namespace Droid {
namespace Content {

CIntentFilterComparison::CIntentFilterComparison()
    : mIntent(NULL)
    , mHashCode(0)
{}

CIntentFilterComparison::~CIntentFilterComparison()
{}

ECode CIntentFilterComparison::GetIntent(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent)
    *intent = mIntent;
    INTERFACE_ADDREF(*intent)
    return NOERROR;
}

ECode CIntentFilterComparison::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual)
    if (IIntentFilterComparison::Probe(obj) != NULL) {
        AutoPtr<IIntent> otherIntent;
        FAIL_RETURN(((IIntentFilterComparison*)obj)->GetIntent((IIntent**)&otherIntent))
        return mIntent->FilterEquals(otherIntent, isEqual);
    }
    *isEqual = FALSE;
    return NOERROR;
}

ECode CIntentFilterComparison::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    *hashCode = mHashCode;
    return NOERROR;
}

ECode CIntentFilterComparison::constructor(
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(intent)
    mIntent = intent;
    FAIL_RETURN(intent->FilterHashCode(&mHashCode));
    return NOERROR;
}

}
}
}

