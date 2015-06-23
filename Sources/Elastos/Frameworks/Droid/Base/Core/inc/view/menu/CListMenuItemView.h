
#ifndef _CLISTMENUITEMVIEW_H__
#define _CLISTMENUITEMVIEW_H__

#include "_CListMenuItemView.h"


#include "view/menu/ListMenuItemView.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass(CListMenuItemView), public ListMenuItemView
{
public:
    IVIEW_METHODS_DECL()

    IVIEWGROUP_METHODS_DECL()

    IVIEWPARENT_METHODS_DECL()

    IVIEWMANAGER_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    ECode constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle = 0);

    CARAPI_(PInterface) Probe(
    /* [in] */ REIID riid);

    CARAPI Initialize(
        /* [in] */ IMenuItemImpl* itemData,
        /* [in] */ Int32 menuType);

    CARAPI SetForceShowIcon(
        /* [in] */ Boolean forceShow);

    CARAPI GetItemData(
        /* [out] */ IMenuItemImpl** itemData);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI SetShortcut(
        /* [in] */ Boolean showShortcut,
        /* [in] */ Char32 shortcutKey);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI PrefersCondensedTitle(
        /* [out] */ Boolean* prefer);

    CARAPI ShowsIcon(
        /* [out] */ Boolean* show);

    CARAPI IsBaselineAligned(
        /* [out] */ Boolean* baselineAligned);

    CARAPI SetBaselineAligned(
        /* [in] */ Boolean baselineAligned);

    CARAPI GetBaselineAlignedChildIndex(
        /* [out] */ Int32 * pBaselineAlignedChildIndex);

    CARAPI SetBaselineAlignedChildIndex(
        /* [in] */ Int32 baselineAlignedChildIndex);

    CARAPI GetWeightSum(
        /* [out] */ Float * pWeightSum);

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

    CARAPI IsMeasureWithLargestChildEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI SetMeasureWithLargestChildEnabled(
        /* [in] */ Boolean enabled);
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif   //_CLISTMENUITEMVIEW_H__
