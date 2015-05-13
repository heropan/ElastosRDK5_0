#ifndef __PREDICTION_H__
#define __PREDICTION_H__

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
#endif //__PREDICTION_H__
