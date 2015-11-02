#ifndef __ELASTOS_DROID_VIEW_ACCESSIBILITY_CCOLLECTIONITEMINFOHELPER_H__
#define __ELASTOS_DROID_VIEW_ACCESSIBILITY_CCOLLECTIONITEMINFOHELPER_H__

#include "_Elastos_Droid_View_Accessibility_CCollectionItemInfoHelper.h"
#include <elastos/core/Singleton.h>

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CarClass(CCollectionItemInfoHelper)
    , public Singleton
    , public ICollectionItemInfoHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI Obtain(
        /* [in] */ ICollectionItemInfo* other,
        /* [out] */ ICollectionItemInfo** info);

    CARAPI Obtain(
        /* [in] */ Int32 rowIndex,
        /* [in] */ Int32 rowSpan,
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int32 columnSpan,
        /* [in] */ Boolean heading,
        /* [out] */ ICollectionItemInfo** info);

    CARAPI Obtain(
        /* [in] */ Int32 rowIndex,
        /* [in] */ Int32 rowSpan,
        /* [in] */ Int32 columnIndex,
        /* [in] */ Int32 columnSpan,
        /* [in] */ Boolean heading,
        /* [in] */ Boolean selected,
        /* [out] */ ICollectionItemInfo** info);
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_VIEW_ACCESSIBILITY_CCOLLECTIONITEMINFOHELPER_H__
