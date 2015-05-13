
#ifndef  __ACTIONMENUVIEW_H__
#define  __ACTIONMENUVIEW_H__

#include "view/ViewGroupLayoutParamsMacro.h"
#include "widget/LinearLayoutLayoutParams.h"
#include "widget/LinearLayout.h"
#include "view/menu/ActionMenuPresenter.h"

using Elastos::Droid::View::Menu::IActionMenuViewLayoutParams;
using Elastos::Droid::Widget::ILinearLayoutLayoutParams;
using Elastos::Droid::Widget::LinearLayoutLayoutParams;
using Elastos::Droid::Widget::ILinearLayout;
using Elastos::Droid::Widget::LinearLayout;


namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

extern "C" const InterfaceID EIID_ActionMenuView;

class ActionMenuView: public LinearLayout
{
public:
    ActionMenuView();

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetPresenter(
        /* [in] */ IActionMenuPresenter* presenter);

    CARAPI_(Boolean) IsExpandedFormat();

    CARAPI SetMaxItemHeight(
        /* [in] */ Int32 maxItemHeight);



    /**
     * Measure a child view to fit within cell-based formatting. The child's width
     * will be measured to a whole multiple of cellSize.
     *
     * <p>Sets the expandable and cellsUsed fields of LayoutParams.
     *
     * @param child Child to measure
     * @param cellSize Size of one cell
     * @param cellsRemaining Number of cells remaining that this view can expand to fill
     * @param parentHeightMeasureSpec MeasureSpec used by the parent view
     * @param parentHeightPadding Padding present in the parent view
     * @return Number of cells this child was measured to occupy
     */
    static CARAPI_(Int32) MeasureChildForCells(
        /* [in] */ IView* child,
        /* [in] */ Int32 cellSize,
        /* [in] */ Int32 cellsRemaining,
        /* [in] */ Int32 parentHeightMeasureSpec,
        /* [in] */ Int32 parentHeightPadding);

    virtual CARAPI OnDetachedFromWindow();

    CARAPI_(Boolean) IsOverflowReserved();

    CARAPI SetOverflowReserved(
        /* [in] */ Boolean reserveOverflow);

    CARAPI GenerateDefaultLayoutParams(
        /* [out] */ ILinearLayoutLayoutParams** lp);

    CARAPI GenerateLayoutParams(
        /* [in] */ IAttributeSet* attrs,
        /* [out] */ IViewGroupLayoutParams** params);

    CARAPI_(AutoPtr<IViewGroupLayoutParams>) GenerateLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    CARAPI_(AutoPtr<IActionMenuViewLayoutParams>) GenerateOverflowButtonLayoutParams();

    CARAPI InvokeItem(
        /* [in] */ IMenuItemImpl* item,
        /* [out] */ Boolean* result);

    CARAPI GetWindowAnimations(
        /* [out] */ Int32* animations);

    CARAPI Initialize(
        /* [in] */ IMenuBuilder* menu);

    CARAPI_(Boolean) DispatchPopulateAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

protected:
    CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI_(void) OnLayout(
        /* [in] */ Boolean changed,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    //@Override
    CARAPI_(Boolean) CheckLayoutParams(
        /* [in] */ IViewGroupLayoutParams* p);

    //@Override
    CARAPI_(Boolean) HasDividerBeforeChildAt(
        /* [in] */ Int32 childIndex);

    CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

private:
    CARAPI_(void) InitActionMenu(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(void) OnMeasureExactFormat(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    CARAPI_(AutoPtr<IActionMenuPresenter>) GetPresenter();

public:
    static const Int32 MIN_CELL_SIZE; // dips
    static const Int32 GENERATED_ITEM_PADDING; // dips

private:
    static String TAG;

    AutoPtr<IMenuBuilder> mMenu;

    Boolean mReserveOverflow;
    AutoPtr<IWeakReference> mPresenter;
    Boolean mFormatItems;
    Int32 mFormatItemsWidth;
    Int32 mMinCellSize;
    Int32 mGeneratedItemPadding;
    Int32 mMeasuredExtraWidth;
    Int32 mMaxItemHeight;

    friend class ActionMenuPresenter;
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  //__ACTIONMENUVIEW_H__
