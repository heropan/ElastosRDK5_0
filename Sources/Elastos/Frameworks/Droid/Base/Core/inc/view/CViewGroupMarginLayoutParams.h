
#ifndef __CVIEWGROUPMARGINLAYOUTPARAMS_H__
#define __CVIEWGROUPMARGINLAYOUTPARAMS_H__

#include "_CViewGroupMarginLayoutParams.h"
#include "view/ViewGroupMarginLayoutParams.h"
#include "view/ViewGroupLayoutParamsMacro.h"

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
    // TODO: Add your private member variables here.
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif // __CVIEWGROUPMARGINLAYOUTPARAMS_H__
