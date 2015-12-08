#ifndef __ELASTOS_DROID_INTERNAL_APP_CRECYCLELISTVIEW_H__
#define __ELASTOS_DROID_INTERNAL_APP_CRECYCLELISTVIEW_H__

#include "_Elastos_Droid_Internal_App_CRecycleListView.h"
#include "elastos/droid/internal/app/CAlertController.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

CarClass(CRecycleListView)
    , public CAlertController::RecycleListView
{
public:
    CAR_OBJECT_DECL()
};

} // namespace App
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_APP_CRECYCLELISTVIEW_H__
