#ifndef __ELASTOS_DROID_GESTURE_PREDICTION_H__
#define __ELASTOS_DROID_GESTURE_PREDICTION_H__

namespace Elastos {
namespace Droid {
namespace Gesture {

class Prediction
	: Object
{
public:
    Prediction();
    virtual ~Prediction();

    CARAPI constructor(
        /* [in] */ const String& label,
        /* [in] */ Double predictionScore);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetScore(
        /* [out] */ Double* score);

private:
    String mName;
    Double mScore;
};

} // namespace Gesture
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GESTURE_PREDICTION_H__
