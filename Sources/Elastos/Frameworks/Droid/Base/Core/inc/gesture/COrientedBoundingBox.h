#ifndef __ORIENTEDBOUNDINGBOX_H__
#define __ORIENTEDBOUNDINGBOX_H__

#include "_COrientedBoundingBox.h"

using Elastos::Droid::Graphics::IPath;

namespace Elastos {
namespace Droid {
namespace Gesture {

CarClass(COrientedBoundingBox)
{
public:
    COrientedBoundingBox()
        : mSquareness(0)
        , mWidth(0)
        , mHeight(0)
        , mOrientation(0)
        , mCenterX(0)
        , mCenterY(0)
    {}

    CARAPI constructor(
        /* [in] */ Float angle,
        /* [in] */ Float cx,
        /* [in] */ Float cy,
        /* [in] */ Float w,
        /* [in] */ Float h);
    /**
     * Currently used for debugging purpose only.
     *
     * @hide
     */
    CARAPI ToPath(
        /* [out] */ IPath **outPath);

    CARAPI GetSquareness(
        /* [out] */ Float* squareness);

    CARAPI GetWidth(
        /* [out] */ Float *width);

    CARAPI GetHeight(
        /* [out] */ Float *height);

    CARAPI GetOrientation(
        /* [out] */ Float *orientation);

    CARAPI GetCenterX(
        /* [out] */ Float *centerX);
    
    CARAPI GetCenterY(
        /* [out] */ Float *centerY);

public:
    Float mSquareness;

    Float mWidth;
    Float mHeight;
    Float mOrientation; 

    Float mCenterX;
    Float mCenterY;
};
}//namespace Gesture
}//namespace Droid
}//namespace Elastos
#endif //__ORIENTEDBOUNDINGBOX_H__