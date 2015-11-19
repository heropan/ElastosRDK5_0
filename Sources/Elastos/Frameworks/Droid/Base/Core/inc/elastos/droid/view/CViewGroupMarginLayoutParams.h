
#ifndef __ELASTOS_DROID_VIEW_CVIEWGROUPMARGINLAYOUTPARAMS_H__
#define __ELASTOS_DROID_VIEW_CVIEWGROUPMARGINLAYOUTPARAMS_H__

#include "_Elastos_Droid_View_CViewGroupMarginLayoutParams.h"
#include "elastos/droid/view/ViewGroupMarginLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CViewGroupMarginLayoutParams)
    , public ViewGroupMarginLayoutParams
{
public:
    CAR_OBJECT_DECL();

    CARAPI ToString(
        /* [out] */ String* description);
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_VIEW_CVIEWGROUPMARGINLAYOUTPARAMS_H__
