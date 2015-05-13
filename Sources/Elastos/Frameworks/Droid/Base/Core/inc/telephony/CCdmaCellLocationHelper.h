
#ifndef __CCDMACELLLOCATIONHELPER_H__
#define __CCDMACELLLOCATIONHELPER_H__

#include "_CCdmaCellLocationHelper.h"

namespace Elastos {
namespace Droid {
namespace Telephony {

CarClass(CCdmaCellLocationHelper)
{
public:
    CARAPI ConvertQuartSecToDecDegrees(
        /* [in] */ Int32 quartSec,
        /* [out] */ Double* val);

};

} // namespace Telephony
} // namespace Droid
} // namespace Elastos

#endif // __CCDMACELLLOCATIONHELPER_H__
