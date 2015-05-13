#ifndef __CPREDICTION_H__
#define __CPREDICTION_H__

#include "_CPrediction.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

CarClass(CPrediction)
{
public:
    CPrediction()
        : mScore(0)
    {}

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

}
}
}

#endif