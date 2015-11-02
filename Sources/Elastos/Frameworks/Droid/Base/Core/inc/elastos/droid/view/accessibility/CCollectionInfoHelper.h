#ifndef __ELASTOS_DROID_VIEW_ACCESSIBILITY_CCOLLECTIONINFOHELPER_H__
#define __ELASTOS_DROID_VIEW_ACCESSIBILITY_CCOLLECTIONINFOHELPER_H__

#include "_Elastos_Droid_View_Accessibility_CCollectionInfoHelper.h"
#include <elastos/core/Singleton.h>

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CarClass(CCollectionInfoHelper)
    , public Singleton
    , public ICollectionInfoHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI Obtain(
        /* [in] */ ICollectionInfo* other,
        /* [out] */ ICollectionInfo** info);

    CARAPI Obtain(
        /* [in] */ Int32 rowCount,
        /* [in] */ Int32 columnCount,
        /* [in] */ Boolean hierarchical,
        /* [out] */ ICollectionInfo** info);

    CARAPI Obtain(
        /* [in] */ Int32 rowCount,
        /* [in] */ Int32 columnCount,
        /* [in] */ Boolean hierarchical,
        /* [in] */ Int32 selectionMode,
        /* [out] */ ICollectionInfo** info);
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_VIEW_ACCESSIBILITY_CCOLLECTIONINFOHELPER_H__
