
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/gesture/CPrediction.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

ECode CPrediction::constructor(
    /* [in] */ const String& label,
    /* [in] */ Double predictionScore)
{
    mName = label;
    mScore = predictionScore;
    return NOERROR;
}

ECode CPrediction::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = mName;
    return NOERROR;
}

ECode CPrediction::GetScore(
    /* [out] */ Double* score)
{
    VALIDATE_NOT_NULL(score);

    *score = mScore;
    return NOERROR;
}

}
}
}