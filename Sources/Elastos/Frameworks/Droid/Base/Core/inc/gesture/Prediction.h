#ifndef __ELASTOS_DROID_GESTURE_PREDICTION_H__
#define __ELASTOS_DROID_GESTURE_PREDICTION_H__

namespace Elastos {
namespace Droid {
namespace Gesture {

class Prediction
{
public:
    CARAPI Init(
        /* [in] */ const String& label,
        /* [in] */ Double predictionScore);

public:
    const String mName;

    Double mScore;

};

}//namespace Gesture
}//namespace Droid
}//namespace Elastos
#endif //__ELASTOS_DROID_GESTURE_PREDICTION_H__
