
#include "ext/frameworkext.h"
#include "graphics/CInterpolator.h"
#include "SkScalar.h"
#include "SkTemplates.h"
#include "os/SystemClock.h"
#include <skia/utils/SkInterpolator.h>

using Elastos::Droid::Os::SystemClock;

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CInterpolator::constructor(
    /* [in] */ Int32 valueCount)
{
    mValueCount = valueCount;
    mFrameCount = 2;
    native_instance = (Int32)(new SkInterpolator(valueCount, 2));
    return NOERROR;
}

ECode CInterpolator::constructor(
    /* [in] */ Int32 valueCount,
    /* [in] */ Int32 frameCount)
{
    mValueCount = valueCount;
    mFrameCount = frameCount;
    native_instance = (Int32)(new SkInterpolator(valueCount, frameCount));
    return NOERROR;
}

CInterpolator::~CInterpolator()
{
    if((SkInterpolator*)native_instance)
        delete (SkInterpolator*)native_instance;
}

ECode CInterpolator::GetKeyFrameCount(
    /* [out] */ Int32* count)
{
    *count = mFrameCount;
    return NOERROR;
}

ECode CInterpolator::GetValueCount(
    /* [out] */ Int32* count)
{
    *count = mValueCount;
    return NOERROR;
}

ECode CInterpolator::Reset(
    /* [in] */ Int32 valueCount)
{
    ResetEx(valueCount, 2);
    return NOERROR;
}

ECode CInterpolator::ResetEx(
    /* [in] */ Int32 valueCount,
    /* [in] */ Int32 frameCount)
{
    mValueCount = valueCount;
    mFrameCount = frameCount;
    ((SkInterpolator*)native_instance)->reset(valueCount, frameCount);
    return NOERROR;
}

ECode CInterpolator::SetKeyFrame(
    /* [in] */ Int32 index,
    /* [in] */ Int32 msec,
    /* [in] */ const ArrayOf<Float>& values)
{
    SetKeyFrameEx(index, msec, values, NULL);
    return NOERROR;
}

ECode CInterpolator::SetKeyFrameEx(
    /* [in] */ Int32 index,
    /* [in] */ Int32 msec,
    /* [in] */ const ArrayOf<Float>& values,
    /* [in] */ ArrayOf<Float>* blendArray)
{
    if (index < 0 || index >= mFrameCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (values.GetLength() < mValueCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (blendArray != NULL && blendArray->GetLength() < 4) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    SkScalar    blendStorage[4];
    SkScalar*   blend = NULL;

    Int32 i, n = values.GetLength();

    SkAutoSTMalloc<16, SkScalar>  storage(n);
    SkScalar* scalars = storage.get();

    for (i = 0; i < n; i++)
        scalars[i] = SkFloatToScalar(values[i]);

    if (blendArray != NULL) {

        for (i = 0; i < 4; i++)
            blendStorage[i] = SkFloatToScalar((*blendArray)[i]);
        blend = blendStorage;
    }

    ((SkInterpolator*)native_instance)->setKeyFrame(index, msec, scalars, blend);
    return NOERROR;
}

ECode CInterpolator::SetRepeatMirror(
    /* [in] */ Float repeatCount,
    /* [in] */ Boolean mirror)
{
    if (repeatCount > 32000)
        repeatCount = 32000;

    ((SkInterpolator*)native_instance)->setRepeatCount(SkFloatToScalar(repeatCount));
    ((SkInterpolator*)native_instance)->setMirror(mirror != 0);
    return NOERROR;
}

ECode CInterpolator::TimeToValues(
    /* [out] */ ArrayOf<Float>* values,
    /* [out] */ InterpolatorResult* result)
{
    return TimeToValuesEx((Int32)SystemClock::GetUptimeMillis(), values, result);
}

ECode CInterpolator::TimeToValuesEx(
    /* [in] */ Int32 msec,
    /* [out] */ ArrayOf<Float>* values,
    /* [out] */ InterpolatorResult* result)
{
    VALIDATE_NOT_NULL(result);

    if (values != NULL && values->GetLength() < mValueCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    SkInterpolatorBase::Result skResult;

    skResult = ((SkInterpolator*)native_instance)->timeToValues(msec, (SkScalar*)values->GetPayload());

    if (values) {
        Int32 n = values->GetLength();
        for (Int32 i = 0; i < n; i++) {
            (*values)[i] = SkScalarToFloat(*(SkScalar*)&(*values)[i]);
        }
    }

    switch (skResult) {
        case 0: *result = InterpolatorResult_NORMAL;
        case 1: *result = InterpolatorResult_FREEZE_START;
        default: *result = InterpolatorResult_FREEZE_END;
    }
    return NOERROR;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
