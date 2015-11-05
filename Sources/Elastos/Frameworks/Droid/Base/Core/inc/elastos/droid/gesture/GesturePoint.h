#ifndef __ELASTOS_DROID_GESTURE_CGESTUREPOINT_H__
#define __ELASTOS_DROID_GESTURE_CGESTUREPOINT_H__

#include "_Elastos_Droid_Gesture_CGesturePoint.h"

using Elastos::IO::IDataInputStream;

namespace Elastos {
namespace Droid {
namespace Gesture {

class GesturePoint
    : public Object
    , public IGesturePoint
    , public IGesturePointHelper
{
public:
    GesturePoint();
    virtual ~GesturePoint();

    CARAPI constructor(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int64 t);

    CARAPI GetX(
        /* [out] */ Float * x);

    CARAPI GetY(
        /* [out] */ Float * y);

    CARAPI GetTimestamp(
        /* [out] */ Int64 * timestamp);

    static CARAPI Deserialize(
        /* [in] */ IDataInputStream *in,
        /* [out] */ IGesturePoint **instance);

private:
    Float mX;
    Float mY;

    Int64 mTimestamp;
};

} // namespace Gesture
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GESTURE_CGESTUREPOINT_H__
