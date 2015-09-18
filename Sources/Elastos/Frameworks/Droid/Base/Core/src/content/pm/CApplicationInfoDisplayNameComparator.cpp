
#include "ext/frameworkext.h"
#include "content/pm/CApplicationInfoDisplayNameComparator.h"

using Elastos::Text::ICollator;
using Elastos::Text::ICollatorHelper;
using Elastos::Text::CCollatorHelper;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CAR_INTERFACE_IMPL(CApplicationInfoDisplayNameComparator, Object, IComparator)

CAR_OBJECT_IMPL(CApplicationInfoDisplayNameComparator)

ECode CApplicationInfoDisplayNameComparator::constructor(
    /* [in] */ IPackageManager* pm)
{
    mPM = pm;
    return NOERROR;
}

ECode CApplicationInfoDisplayNameComparator::Compare(
    /* [in] */ IInterface* aa,
    /* [in] */ IInterface* ab,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = -1;

    AutoPtr<ICharSequence> sa;
    mPM->GetApplicationLabel(IApplicationInfo::Probe(aa), (ICharSequence**)&sa);
    if (sa == NULL) {
        aa->GetPackageName((ICharSequence**)&sa);
    }


    AutoPtr<ICharSequence> sb;
    mPM->GetApplicationLabel(IApplicationInfo::Probe(ab), (ICharSequence**)&sb);
    if (sb == NULL) {
        ab->GetPackageName((ICharSequence**)&sb);
    }

    AutoPtr<ICollator> collator;
    AutoPtr<ICollatorHelper> helper;
    CCollatorHelper::AcquireSingleton((ICollatorHelper**)&helper);
    helper->GetInstance((ICollator**)&collator);
    return collator->Compare(Object::ToString(sa), Object::ToString(sb), result);

}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
