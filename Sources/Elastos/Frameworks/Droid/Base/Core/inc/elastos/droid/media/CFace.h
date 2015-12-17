
#ifndef __ELASTOS_DROID_MEDIA_CFACE_H__
#define __ELASTOS_DROID_MEDIA_CFACE_H__

#include "_Elastos_Droid_Media_CFace.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Graphics::IPointF;

namespace Elastos {
namespace Droid {
namespace Media {

/**
 * A Face contains all the information identifying the location
 * of a face in a bitmap.
 */
CarClass(CFace)
    , public Object
    , public IFace
{
public:
    CFace();

    virtual ~CFace();

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor();

    /**
     * Returns a confidence factor between 0 and 1. This indicates how
     * certain what has been found is actually a face. A confidence
     * factor above 0.3 is usually good enough.
     */
    CARAPI Confidence(
        /* [out] */ Float* factor);

    /**
     * Sets the position of the mid-point between the eyes.
     * @param point the PointF coordinates (Float values) of the
     *              face's mid-point
     */
    CARAPI GetMidPoint(
        /* [in] */ IPointF* point);

    /**
     * Returns the distance between the eyes.
     */
    CARAPI EyesDistance(
        /* [out] */ Float* distance);

    /**
     * Returns the face's pose. That is, the rotations around either
     * the X, Y or Z axis (the positions in 3-dimensional Euclidean space).
     *
     * @param euler the Euler axis to retrieve an angle from
     *              (<var>EULER_X</var>, <var>EULER_Y</var> or
     *              <var>EULER_Z</var>)
     * @return the Euler angle of the of the face, for the given axis
     */
    CARAPI Pose(
        /* [in] */ Int32 euler,
        /* [out] */ Float* angle);

public:
    Float mConfidence;
    Float mMidPointX;
    Float mMidPointY;
    Float mEyesDist;
    Float mPoseEulerX;
    Float mPoseEulerY;
    Float mPoseEulerZ;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CFACE_H__
