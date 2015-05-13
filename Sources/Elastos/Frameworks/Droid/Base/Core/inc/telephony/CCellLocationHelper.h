
#ifndef __CCELLLOCATIONHELPER_H__
#define __CCELLLOCATIONHELPER_H__

#include "_CCellLocationHelper.h"

using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Telephony {

CarClass(CCellLocationHelper)
{
public:
    CARAPI RequestLocationUpdate();

    CARAPI NewFromBundle(
        /* [in] */ IBundle* bundle,
        /* [out] */ ICellLocation** cl);

    CARAPI GetEmpty(
        /* [out] */ ICellLocation** cl);

};

} //namespace Elastos
} //namespace Droid
} //namespace Telephony

#endif // __CCELLLOCATIONHELPER_H__
