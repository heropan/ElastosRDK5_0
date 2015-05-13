#ifndef __CCURSORWINDOWHELPER_H__
#define __CCURSORWINDOWHELPER_H__

#include "_CCursorWindowHelper.h"

namespace Elastos {
namespace Droid {
namespace Database {

CarClass(CCursorWindowHelper)
{
public:
    CARAPI NewFromParcel(
        /* [in] */ IParcel* p,
        /* [out] */ ICursorWindow** result);
};

} //Database
} //Droid
} //Elastos

#endif //__CCURSORWINDOWHELPER_H__