#ifndef __ELASTOS_DROID_VIEW_MENU_CACTIONMENUVIEWLAYOUTPARAMS_H__
#define __ELASTOS_DROID_VIEW_MENU_CACTIONMENUVIEWLAYOUTPARAMS_H__

#include "_CActionMenuViewLayoutParams.h"

#include "widget/LinearLayoutLayoutParams.h"

using Elastos::Droid::Widget::ILinearLayoutLayoutParams;
using Elastos::Droid::Widget::LinearLayoutLayoutParams;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass(CActionMenuViewLayoutParams) , public LinearLayoutLayoutParams
{
public:
    CActionMenuViewLayoutParams();

    IVIEWGROUPLP_METHODS_DECL()

    IVIEWGROUPMARGINLP_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* other);

    CARAPI constructor(
        /* [in] */ IActionMenuViewLayoutParams* actLp);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Boolean isOverflowButton);

    CARAPI GetWeight(
        /* [out] */ Float* weight);

    CARAPI SetWeight(
        /* [in] */ Float weight);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetIsOverflowButton(
        /* [in] */ Boolean isOverflow);

    CARAPI GetIsOverflowButton(
        /* [out] */ Boolean* isOverflow);

    CARAPI SetCellsUsed(
        /* [in] */ Int32 cellsUsed);

    CARAPI GetCellsUsed(
        /* [out] */ Int32* cellsUsed);

    CARAPI SetExtraPixels(
        /* [in] */ Int32 extraPixels);

    CARAPI GetExtraPixels(
        /* [out] */ Int32* extraPixels);

    CARAPI SetExpandable(
        /* [in] */ Boolean expandable);

    CARAPI GetExpandable(
        /* [out] */ Boolean* expandable);

    CARAPI SetPreventEdgeOffset(
        /* [in] */ Boolean preventEdgeOffset);

    CARAPI GetPreventEdgeOffset(
        /* [out] */ Boolean* preventEdgeOffset);

    CARAPI SetExpanded(
        /* [in] */ Boolean expanded);

    CARAPI GetExpanded(
        /* [out] */ Boolean* expanded);
private:
    Boolean mIsOverflowButton;
    Int32 mCellsUsed;
    Int32 mExtraPixels;
    Boolean mExpandable;
    Boolean mPreventEdgeOffset;
    Boolean mExpanded;
};

} // namespace Menu
} // namespace View
} // namepsace Droid
} // namespace Elastos

#endif
