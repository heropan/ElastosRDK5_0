
#include "util/CPair.h"
#include <ext/frameworkdef.h>
#include <elastos/ObjectUtils.h>

using Elastos::Core::ObjectUtils;

namespace Elastos {
namespace Droid {
namespace Utility {

ECode CPair::GetFirst(
    /* [out] */ IInterface **first)
{
    VALIDATE_NOT_NULL(first)
    *first = mFirst;
    INTERFACE_ADDREF(*first)
    return NOERROR;
}

ECode CPair::GetSecond(
    /* [out] */ IInterface **second)
{
    VALIDATE_NOT_NULL(second)
    *second = mSecond;
    INTERFACE_ADDREF(*second)
    return NOERROR;
}

ECode CPair::Equals(
    /* [in] */ IInterface *o,
    /* [out] */ Boolean *isEqual)
{
    VALIDATE_NOT_NULL(isEqual)
    if (!IPair::Probe(o)) {
        *isEqual = FALSE;
        return NOERROR;
    }
    AutoPtr<CPair> p = (CPair*)IPair::Probe(o);
    *isEqual = ObjectUtils::Equals(p->mFirst, mFirst) && ObjectUtils::Equals(p->mSecond, mSecond);
    return NOERROR;
}

ECode CPair::GetHashCode(
    /* [out] */ Int32 *hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    if (mFirst == NULL) {
        *hashCode = 0;
    }
    else {
        IObject::Probe(mFirst)->GetHashCode(hashCode);
    }
    Int32 hc;
    if (mSecond == NULL) {
        hc = 0;
    }
    else {
        IObject::Probe(mSecond)->GetHashCode(&hc);
    }
    *hashCode ^= hc;
    return NOERROR;
}

ECode CPair::constructor(
    /* [in] */ IInterface *first,
    /* [in] */ IInterface *second)
{
    mFirst = first;
    mSecond = second;
    return NOERROR;
}

ECode CPair::Create(
    /* [in] */ IInterface *A,
    /* [in] */ IInterface *B,
    /* [out] */ IPair **pair)
{
    return CPair::New(A, B, pair);
}

} // namespace Utility
} // namespace Droid
} // namespace Elastos
