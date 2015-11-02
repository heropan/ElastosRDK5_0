#include "elastos/droid/accessibilityservice/CAccessibilityServiceInfoCapabilityInfo.h"

namespace Elastos {
namespace Droid {
namespace AccessibilityService {

CAR_INTERFACE_IMPL(CAccessibilityServiceInfoCapabilityInfo, Object, IAccessibilityServiceInfoCapabilityInfo)

CAR_OBJECT_IMPL(CAccessibilityServiceInfoCapabilityInfo)

CAccessibilityServiceInfoCapabilityInfo::CAccessibilityServiceInfoCapabilityInfo()
    : mCapability(0)
    , mTitleResId(0)
    , mDescResId(0)
{}

CAccessibilityServiceInfoCapabilityInfo::~CAccessibilityServiceInfoCapabilityInfo()
{}

ECode CAccessibilityServiceInfoCapabilityInfo::constructor(
    /* [in] */ Int32 capability,
    /* [in] */ Int32 titleResId,
    /* [in] */ Int32 descResId)
{
    mCapability = capability;
    mTitleResId = titleResId;
    mDescResId = descResId;

    return NOERROR;
}

}
}
}
