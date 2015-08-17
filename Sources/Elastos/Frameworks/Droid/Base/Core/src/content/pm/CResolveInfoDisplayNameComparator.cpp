
#include "ext/frameworkext.h"
#include "content/pm/CResolveInfoDisplayNameComparator.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Text::ICollatorHelper;
using Elastos::Text::CCollatorHelper;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CResolveInfoDisplayNameComparator::CResolveInfoDisplayNameComparator()
{
    AutoPtr<ICollatorHelper> helper;
    CCollatorHelper::AcquireSingleton((ICollatorHelper**)&helper);
    helper->GetInstance((ICollator**)&mCollator);
}

ECode CResolveInfoDisplayNameComparator::constructor(
    /* [in] */ IPackageManager* pm)
{
    mPM = pm;
    return NOERROR;
}

ECode CResolveInfoDisplayNameComparator::Compare(
    /* [in] */ IResolveInfo* a,
    /* [in] */ IResolveInfo* b,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    String aStr;
    AutoPtr<ICharSequence> sa;
    a->LoadLabel(mPM, (ICharSequence**)&sa);
    if (sa == NULL) {
        AutoPtr<IActivityInfo> aInfo;
        a->GetActivityInfo((IActivityInfo**)&aInfo);
        aInfo->GetName(&aStr);
    }
    else {
        sa->ToString(&aStr);
    }
    String bStr;
    AutoPtr<ICharSequence> sb;
    b->LoadLabel(mPM, (ICharSequence**)&sb);
    if (sb == NULL) {
        AutoPtr<IActivityInfo> aInfo;
        b->GetActivityInfo((IActivityInfo**)&aInfo);
        aInfo->GetName(&bStr);
    }
    else {
        sb->ToString(&bStr);
    }

    return mCollator->Compare(aStr, bStr, result);
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
