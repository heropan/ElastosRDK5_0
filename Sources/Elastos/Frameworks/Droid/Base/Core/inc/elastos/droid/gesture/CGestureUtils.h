#ifndef __ELASTOS_DROID_GESTURE_CGESTUREUTILS_H__
#define __ELASTOS_DROID_GESTURE_CGESTUREUTILS_H__

#include "elastos/droid/gesture/GestureUtils.h"
#include "elastos/droid/ext/frameworkext.h"
#include "_Elastos_Droid_Gesture_CGestureUtils.h"
#include <elastos/core/Singleton.h>

using Elastos::Utility::IArrayList;
using Elastos::IO::ICloseable;

namespace Elastos {
namespace Droid {
namespace Gesture {

CarClass(CGestureUtils)
    , public Singleton
    , public IGestureUtils
{
public:
    CAR_SINGLETON_DECL();

    CAR_INTERFACE_DECL();

    CGestureUtils();

    virtual ~CGestureUtils();

    CARAPI CloseStream(
        /* [in] */ ICloseable * pParam0);

    CARAPI SpatialSampling(
        /* [in] */ IGesture * pParam0,
        /* [in] */ Int32 Param1,
        /* [out, callee] */ ArrayOf<Float> ** ppParam2);

    CARAPI SpatialSampling(
        /* [in] */ IGesture * pParam0,
        /* [in] */ Int32 Param1,
        /* [in] */ Boolean Param2,
        /* [out, callee] */ ArrayOf<Float> ** ppParam3);

    CARAPI TemporalSampling(
        /* [in] */ IGestureStroke * pParam0,
        /* [in] */ Int32 Param1,
        /* [out, callee] */ ArrayOf<Float> ** ppParam2);

    CARAPI ComputeCentroid(
        /* [in] */ ArrayOf<Float> * pParam0,
        /* [out, callee] */ ArrayOf<Float> ** ppParam1);

    CARAPI ComputeTotalLength(
        /* [in] */ ArrayOf<Float> * pParam0,
        /* [out] */ Float * pParam1);

    CARAPI ComputeStraightness(
        /* [in] */ ArrayOf<Float> * pParam0,
        /* [out] */ Float * pParam1);

    CARAPI ComputeStraightness(
        /* [in] */ ArrayOf<Float> * pParam0,
        /* [in] */ Float Param1,
        /* [out] */ Float * pParam2);

    CARAPI SquaredEuclideanDistance(
        /* [in] */ ArrayOf<Float> * pParam0,
        /* [in] */ ArrayOf<Float> * pParam1,
        /* [out] */ Float * pParam2);

    CARAPI CosineDistance(
        /* [in] */ ArrayOf<Float> * pParam0,
        /* [in] */ ArrayOf<Float> * pParam1,
        /* [out] */ Float * pParam2);

    CARAPI MinimumCosineDistance(
        /* [in] */ ArrayOf<Float> * pParam0,
        /* [in] */ ArrayOf<Float> * pParam1,
        /* [in] */ Int32 Param2,
        /* [out] */ Float * pParam3);

    CARAPI ComputeOrientedBoundingBox(
        /* [in] */ IArrayList * pParam0,
        /* [out] */ IOrientedBoundingBox ** ppParam1);

    CARAPI ComputeOrientedBoundingBox(
        /* [in] */ ArrayOf<Float> * pParam0,
        /* [out] */ IOrientedBoundingBox ** ppParam1);

    CARAPI Rotate(
        /* [in] */ ArrayOf<Float> * pParam0,
        /* [in] */ Float Param1,
        /* [out, callee] */ ArrayOf<Float> ** ppParam2);

    CARAPI Translate(
        /* [in] */ ArrayOf<Float> * pParam0,
        /* [in] */ Float Param1,
        /* [in] */ Float Param2,
        /* [out, callee] */ ArrayOf<Float> ** ppParam3);

    CARAPI Scale(
        /* [in] */ ArrayOf<Float> * pParam0,
        /* [in] */ Float Param1,
        /* [in] */ Float Param2,
        /* [out, callee] */ ArrayOf<Float> ** ppParam3);
};

} // namespace Gesture
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GESTURE_CGESTUREUTILS_H__
