
#include "elastos/droid/view/accessibility/CRangeInfo.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

const Int32 CRangeInfo::MAX_POOL_SIZE = 10;
AutoPtr<Pools::SynchronizedPool<IRangeInfo> > CRangeInfo::sPool =
        new Pools::SynchronizedPool<IRangeInfo>(MAX_POOL_SIZE);

CAR_INTERFACE_IMPL(CRangeInfo, Object, IRangeInfo)

CAR_OBJECT_IMPL(CRangeInfo)

CRangeInfo::CRangeInfo()
    : mType(0)
    , mMin(0.0f)
    , mMax(0.0f)
    , mCurrent(0.0f)
{
}

CRangeInfo::~CRangeInfo()
{
}

ECode CRangeInfo::constructor()
{
    return NOERROR;
}

ECode CRangeInfo::constructor(
    /* [in] */ Int32 type,
    /* [in] */ Float min,
    /* [in] */ Float max,
    /* [in] */ Float current)
{
    mType = type;
    mMin = min;
    mMax = max;
    mCurrent = current;
    return NOERROR;
}

ECode CRangeInfo::Obtain(
    /* [in] */ IRangeInfo* other,
    /* [out] */ IRangeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    Int32 type;
    Float min, max, current;
    other->GetType(&type);
    other->GetMin(&min);
    other->GetMax(&max);
    other->GetCurrent(&current);

    return Obtain(type, min, max, current, info);
}

ECode CRangeInfo::Obtain(
    /* [in] */ Int32 type,
    /* [in] */ Float min,
    /* [in] */ Float max,
    /* [in] */ Float current,
    /* [out] */ IRangeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    AutoPtr<IRangeInfo> _info = sPool->AcquireItem();

    if (_info != NULL) {
        *info = _info;
        REFCOUNT_ADD(*info);
        return NOERROR;
    }

    AutoPtr<IRangeInfo> rangeInfo;
    CRangeInfo::New(type, min, max, current, (IRangeInfo**)&rangeInfo);
    *info = rangeInfo;
    REFCOUNT_ADD(*info);

    return NOERROR;
}

ECode CRangeInfo::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type)
    *type = mType;
    return NOERROR;
}

ECode CRangeInfo::GetMin(
    /* [out] */ Float* min)
{
    VALIDATE_NOT_NULL(min)
    *min = mMin;
    return NOERROR;
}

ECode CRangeInfo::GetMax(
    /* [out] */ Float* max)
{
    VALIDATE_NOT_NULL(max)
    *max = mMax;
    return NOERROR;
}

ECode CRangeInfo::GetCurrent(
    /* [out] */ Float* current)
{
    VALIDATE_NOT_NULL(current)
    *current = mCurrent;
    return NOERROR;
}

ECode CRangeInfo::Recycle()
{
    Clear();
    sPool->ReleaseItem(THIS_PROBE(IRangeInfo));
    return NOERROR;
}

void CRangeInfo::Clear()
{
    mType = 0;
    mMin = 0;
    mMax = 0;
    mCurrent = 0;
}

} // Accessibility
} // View
} // Droid
} // Elastos
