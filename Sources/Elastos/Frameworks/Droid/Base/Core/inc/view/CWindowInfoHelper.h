#ifndef __ELASTOS_DROID_VIEW_CWINDOWINFOHELPER_H__
#define __ELASTOS_DROID_VIEW_CWINDOWINFOHELPER_H__

#include "_Elastos_Droid_View_CWindowInfoHelper.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CWindowInfoHelper)
{
public:
    CARAPI Obtain(
        /* [out] */ IWindowInfo** info);

    CARAPI Obtain(
        /* [in] */ IWindowInfo *other,
        /* [out] */ IWindowInfo** info);

};

}// namespace View
}// namespace Droid
}// namespace Elastos
#endif
