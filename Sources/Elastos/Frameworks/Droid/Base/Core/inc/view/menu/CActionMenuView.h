
#ifndef  __CACTIONMENUVIEW_H__
#define  __CACTIONMENUVIEW_H__

#include "_CActionMenuView.h"
#include "view/menu/ActionMenuView.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass(CActionMenuView), public ActionMenuView
{
public:
    IVIEW_METHODS_DECL()

    IVIEWGROUP_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    IVIEWPARENT_METHODS_DECL()

    IVIEWMANAGER_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetPresenter(
        /* [in] */ IActionMenuPresenter* presenter);

    CARAPI IsExpandedFormat(
        /* [out] */ Boolean* res);

    CARAPI SetMaxItemHeight(
        /* [in] */ Int32 maxItemHeight);

    CARAPI IsOverflowReserved(
        /* [out] */ Boolean* res);

    CARAPI SetOverflowReserved(
        /* [in] */ Boolean reserveOverflow);

    CARAPI GenerateOverflowButtonLayoutParams(
        /* [out] */ IViewGroupLayoutParams** pm);

    CARAPI InvokeItem(
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* result);

    CARAPI Initialize(
        /* [in] */ IMenuBuilder* menu);

    CARAPI GetWindowAnimations(
        /* [out] */ Int32* animations);

    CARAPI SetShowDividers(
        /* [in] */ Int32 showDividers);

    CARAPI GetShowDividers(
        /* [out] */ Int32* showDividers);

    CARAPI GetDividerDrawable(
        /* [out] */ IDrawable** divider);

    CARAPI SetDividerDrawable(
        /* [in] */ IDrawable* divider);

    CARAPI SetDividerPadding(
        /* [in] */ Int32 padding);

    CARAPI GetDividerPadding(
        /* [out] */ Int32* padding);

    CARAPI GetDividerWidth(
        /* [out] */ Int32* width);

    CARAPI IsBaselineAligned(
        /* [out] */ Boolean* baselineAligned);

    CARAPI SetBaselineAligned(
        /* [in] */ Boolean baselineAligned);

    CARAPI IsMeasureWithLargestChildEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetMeasureWithLargestChildEnabled(
        /* [in] */ Boolean enabled);

    CARAPI GetBaselineAlignedChildIndex(
        /* [out] */ Int32* index);

    CARAPI SetBaselineAlignedChildIndex(
        /* [in] */ Int32 index);

    CARAPI GetWeightSum(
        /* [out] */ Float* weightSum);

    CARAPI SetWeightSum(
        /* [in] */ Float weightSum);

    CARAPI SetOrientation(
        /* [in] */ Int32 orientation);

    CARAPI GetOrientation(
        /* [out] */ Int32* orientation);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetHorizontalGravity(
        /* [in] */ Int32 horizontalGravity);

    CARAPI SetVerticalGravity(
        /* [in] */ Int32 verticalGravity);
};


} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  // __CACTIONMENUVIEW_H__
