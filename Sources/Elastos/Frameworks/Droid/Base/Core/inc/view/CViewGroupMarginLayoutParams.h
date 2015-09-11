
#ifndef __ELASTOS_DROID_VIEW_CVIEWGROUPMARGINLAYOUTPARAMS_H__
#define __ELASTOS_DROID_VIEW_CVIEWGROUPMARGINLAYOUTPARAMS_H__

#include "_Elastos_Droid_View_CViewGroupMarginLayoutParams.h"
#include "view/ViewGroupMarginLayoutParams.h"


namespace Elastos {
namespace Droid {
namespace View {

CarClass(CViewGroupMarginLayoutParams), public ViewGroupMarginLayoutParams
{
public:
    IVIEWGROUPLP_METHODS_DECL();

    IVIEWGROUPMARGINLP_METHODS_DECL();

    CARAPI ToString(
        /* [out] */ String* description);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IViewGroupMarginLayoutParams* source);

private:
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_VIEW_CVIEWGROUPMARGINLAYOUTPARAMS_H__
