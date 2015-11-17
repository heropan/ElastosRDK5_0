
#include "elastos/droid/gesture/CGestureUtils.h"

using Elastos::IO::ICloseable;
using Elastos::Utility::Etl::List;
using Elastos::Utility::IArrayList;

namespace Elastos {
namespace Droid {
namespace Gesture {

CAR_SINGLETON_IMPL(CGestureUtils);
CAR_INTERFACE_IMPL(CGestureUtils, Singleton, IGestureUtils);

CGestureUtils::CGestureUtils()
{}

CGestureUtils::~CGestureUtils()
{}

ECode CGestureUtils::CloseStream(
    /* [in] */ ICloseable * pParam0)
{
    return GestureUtils::CloseStream(pParam0);
}

ECode CGestureUtils::SpatialSampling(
    /* [in] */ IGesture *pParam0,
    /* [in] */ Int32 Param1,
    /* [out, callee] */ ArrayOf<Float> **ppParam2)
{
    AutoPtr<ArrayOf<Float> > ret;

    ret = GestureUtils::SpatialSampling(pParam0, Param1);
    *ppParam2 = ret;

    return NOERROR;
}

ECode CGestureUtils::SpatialSampling(
    /* [in] */ IGesture *pParam0,
    /* [in] */ Int32 Param1,
    /* [in] */ Boolean Param2,
    /* [out, callee] */ ArrayOf<Float> **ppParam3)
{
    AutoPtr<ArrayOf<Float> > ret;

    ret = GestureUtils::SpatialSampling(pParam0, Param1, Param2);
    *ppParam3 = ret;

    return NOERROR;
}

ECode CGestureUtils::TemporalSampling(
    /* [in] */ IGestureStroke *pParam0,
    /* [in] */ Int32 Param1,
    /* [out, callee] */ ArrayOf<Float> **ppParam2)
{
    AutoPtr<ArrayOf<Float> > ret;

    ret = GestureUtils::TemporalSampling(pParam0, Param1);
    *ppParam2 = ret;

    return NOERROR;
}

ECode CGestureUtils::ComputeCentroid(
    /* [in] */ ArrayOf<Float> * pParam0,
    /* [out, callee] */ ArrayOf<Float> ** ppParam1)
{
    AutoPtr<ArrayOf<Float> > ret;

    ret = GestureUtils::ComputeCentroid(pParam0);
    *ppParam1 = ret;

    return NOERROR;
}

ECode CGestureUtils::ComputeTotalLength(
    /* [in] */ ArrayOf<Float> *pParam0,
    /* [out] */ Float *pParam1)
{
    *pParam1 = GestureUtils::ComputeTotalLength(pParam0);
    return NOERROR;
}

ECode CGestureUtils::ComputeStraightness(
    /* [in] */ ArrayOf<Float> *pParam0,
    /* [out] */ Float *pParam1)
{
    *pParam1 = GestureUtils::ComputeStraightness(pParam0);
    return NOERROR;
}

ECode CGestureUtils::ComputeStraightness(
    /* [in] */ ArrayOf<Float> *pParam0,
    /* [in] */ Float Param1,
    /* [out] */ Float *pParam2)
{
    *pParam2 = GestureUtils::ComputeStraightness(pParam0, Param1);
    return NOERROR;
}

ECode CGestureUtils::SquaredEuclideanDistance(
    /* [in] */ ArrayOf<Float> *pParam0,
    /* [in] */ ArrayOf<Float> *pParam1,
    /* [out] */ Float *pParam2)
{
    *pParam2 = GestureUtils::SquaredEuclideanDistance(pParam0, pParam1);
    return NOERROR;
}

ECode CGestureUtils::CosineDistance(
    /* [in] */ ArrayOf<Float> *pParam0,
    /* [in] */ ArrayOf<Float> *pParam1,
    /* [out] */ Float *pParam2)
{
    *pParam2 = GestureUtils::CosineDistance(pParam0, pParam1);
    return NOERROR;
}

ECode CGestureUtils::MinimumCosineDistance(
    /* [in] */ ArrayOf<Float> *pParam0,
    /* [in] */ ArrayOf<Float> *pParam1,
    /* [in] */ Int32 Param2,
    /* [out] */ Float *pParam3)
{
    *pParam3 = GestureUtils::MinimumCosineDistance(pParam0, pParam1, Param2);
    return NOERROR;
}

ECode CGestureUtils::ComputeOrientedBoundingBox(
    /* [in] */ Elastos::Utility::IArrayList *pParam0,
    /* [out] */ IOrientedBoundingBox ** ppParam1)
{
    AutoPtr<IOrientedBoundingBox> ret;

    ret = GestureUtils::ComputeOrientedBoundingBox(pParam0);

    *ppParam1 = ret;
    return NOERROR;

}

ECode CGestureUtils::ComputeOrientedBoundingBox(
    /* [in] */ ArrayOf<Float> * pParam0,
    /* [out] */ IOrientedBoundingBox **ppParam1)
{
    AutoPtr<IOrientedBoundingBox> ret;

    ret = GestureUtils::ComputeOrientedBoundingBox(pParam0);

    *ppParam1 = ret;
    return NOERROR;

}

ECode CGestureUtils::Rotate(
    /* [in] */ ArrayOf<Float> *pParam0,
    /* [in] */ Float Param1,
    /* [out, callee] */ ArrayOf<Float> **ppParam2)
{
    *ppParam2 = GestureUtils::Rotate(pParam0, Param1);
    return NOERROR;
}

ECode CGestureUtils::Translate(
    /* [in] */ ArrayOf<Float> *pParam0,
    /* [in] */ Float Param1,
    /* [in] */ Float Param2,
    /* [out, callee] */ ArrayOf<Float> **ppParam3)
{
    *ppParam3 = GestureUtils::Translate(pParam0, Param1, Param2);
    return NOERROR;
}

ECode CGestureUtils::Scale(
    /* [in] */ ArrayOf<Float> *pParam0,
    /* [in] */ Float Param1,
    /* [in] */ Float Param2,
    /* [out, callee] */ ArrayOf<Float> **ppParam3)
{
    *ppParam3 = GestureUtils::Scale(pParam0, Param1, Param2);
    return NOERROR;
}

} // namespace Gesture
} // namespace Droid
} // namespace Elastos
