#ifndef __ELASTOS_DROID_VIEW_ACCESSIBILITY_CRANGEINFOHELPER_H__
#define __ELASTOS_DROID_VIEW_ACCESSIBILITY_CRANGEINFOHELPER_H__

#include "_Elastos_Droid_View_Accessibility_CRangeInfoHelper.h"
#include <elastos/core/Singleton.h>

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CarClass(CRangeInfoHelper)
    , public Singleton
    , public IRangeInfoHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI Obtain(
        /* [in] */ IRangeInfo* other,
        /* [out] */ IRangeInfo** info);

    CARAPI Obtain(
        /* [in] */ Int32 type,
        /* [in] */ Float min,
        /* [in] */ Float max,
        /* [in] */ Float current,
        /* [out] */ IRangeInfo** info);
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_VIEW_ACCESSIBILITY_CRANGEINFOHELPER_H__
