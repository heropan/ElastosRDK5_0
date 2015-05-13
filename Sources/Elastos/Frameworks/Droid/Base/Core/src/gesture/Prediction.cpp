#include "Prediction.h"
namespace Elastos {
namespace Droid {
namespace Gesture {

ECode Prediction::Init(
    /* [in] */ const String& label,
    /* [in] */ Double predictionScore) : mName(label), mScore(predictionScore)
{
    return NOERROR;
}

}//namespace Gesture
}//namespace Droid
}//namespace Elastos
