#ifndef __ELASTOS_DROID_GESTURE_ORIENTEDBOUNDINGBOX_H__
#define __ELASTOS_DROID_GESTURE_ORIENTEDBOUNDINGBOX_H__

#include "_Elastos_Droid_Gesture_COrientedBoundingBox.h"

using Elastos::Droid::Graphics::IPath;

namespace Elastos {
namespace Droid {
namespace Gesture {

class OrientedBoundingBox
    : public Object
    , public IOrientedBoundingBox
{
public:
    CAR_INTERFACE_DECL();

    OrientedBoundingBox();
    virtual ~OrientedBoundingBox();
};

} // namespace Gesture
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GESTURE_ORIENTEDBOUNDINGBOX_H__
