
#include "widget/TabWidget.h"
#include <elastos/Math.h>
#include "os/Build.h"
#include "widget/CLinearLayoutLayoutParams.h"
#include "widget/CImageView.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::View::EIID_IViewOnFocusChangeListener;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::EIID_IView;

namespace Elastos{
namespace Droid{
namespace Widget{

// {ca8d2eff-aa0f-43c3-881c-108012f2d868}
const InterfaceID EIID_TabWidget =
{ 0xca8d2eff, 0xaa0f, 0x43c3, { 0x88, 0x1c, 0x10, 0x80, 0x12, 0xf2, 0xd8, 0x68 } };


//==============================================================================
//              TabWidget::TabClickListener
//==============================================================================

CAR_INTERFACE_IMPL(TabWidget::TabClickListener, IViewOnClickListener)

TabWidget::TabClickListener::TabClickListener(
    /* [in] */ Int32 tabIndex,
    /* [in] */ TabWidget* owner)
    : mTabIndex(tabIndex)
    , mHost(owner)
{}

ECode TabWidget::TabClickListener::OnClick(
     /* [in] */ IView* v)
{
    return mHost->mSelectionChangedListener->OnTabSelectionChanged(mTabIndex, TRUE);
}


//==============================================================================
//              TabWidget
//==============================================================================

TabWidget::TabWidget()
    : mSelectedTab(-1)
    , mDrawBottomStrips(TRUE)
    , mStripMoved(FALSE)
    , mImposedTabsHeight(-1)
{
    CRect::New((IRect**)&mBounds);
}

TabWidget::TabWidget(
    /* [in] */ IContext* context)
    : LinearLayout(context, NULL, R::attr::tabWidgetStyle)
    , mSelectedTab(-1)
    , mDrawBottomStrips(TRUE)
    , mStripMoved(FALSE)
    , mImposedTabsHeight(-1)
{
    CRect::New((IRect**)&mBounds);
    ASSERT_SUCCEEDED(InitFromAttributes(context, NULL, R::attr::tabWidgetStyle));
    InitTabWidget();
}

TabWidget::TabWidget(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : LinearLayout(context, attrs, R::attr::tabWidgetStyle)
    , mSelectedTab(-1)
    , mDrawBottomStrips(TRUE)
    , mStripMoved(FALSE)
    , mImposedTabsHeight(-1)
{
    CRect::New((IRect**)&mBounds);
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, R::attr::tabWidgetStyle));
    InitTabWidget();
}

TabWidget::TabWidget(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : LinearLayout(context, attrs, defStyle)
    , mSelectedTab(-1)
    , mDrawBottomStrips(TRUE)
    , mStripMoved(FALSE)
    , mImposedTabsHeight(-1)
{
    CRect::New((IRect**)&mBounds);
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
    InitTabWidget();
}

ECode TabWidget::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::TabWidget),
        ARRAY_SIZE(R::styleable::TabWidget));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx3(
        attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

    Boolean res;
    a->GetBoolean(R::styleable::TabWidget_tabStripEnabled, TRUE, &res);
    SetStripEnabled(res);

    AutoPtr<IDrawable> dr;
    a->GetDrawable(R::styleable::TabWidget_tabStripLeft, (IDrawable**)&dr);
    SetLeftStripDrawable(dr);
    dr = NULL;
    a->GetDrawable(R::styleable::TabWidget_tabStripRight, (IDrawable**)&dr);
    SetRightStripDrawable(dr);

    a->Recycle();
    return NOERROR;
}

ECode TabWidget::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(LinearLayout::Init(context, attrs, defStyle));
    FAIL_RETURN(InitFromAttributes(context, attrs, defStyle));
    InitTabWidget();
    return NOERROR;
}

void TabWidget::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    mStripMoved = TRUE;
    LinearLayout::OnSizeChanged(w, h, oldw, oldh);
}

Int32 TabWidget::GetChildDrawingOrder(
    /* [in] */ Int32 childCount,
    /* [in] */ Int32 i)
{
    if (mSelectedTab == -1) {
        return i;
    } else {
        // Always draw the selected tab last, so that drop shadows are drawn
        // in the correct z-order.
        if (i == childCount - 1) {
            return mSelectedTab;
        }
        else if (i >= mSelectedTab) {
            return i + 1;
        }
        else {
            return i;
        }
    }
}

void TabWidget::InitTabWidget()
{
    SetChildrenDrawingOrderEnabled(TRUE);

    IContext* context = mContext;
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);

    AutoPtr<IApplicationInfo> ai;
    context->GetApplicationInfo((IApplicationInfo**)&ai);
    Int32 version;
    ai->GetTargetSdkVersion(&version);
    if (version <= Build::VERSION_CODES::DONUT) {
        // Donut apps get old color scheme
        if (mLeftStrip == NULL) {
            resources->GetDrawable(R::drawable::tab_bottom_left_v4, (IDrawable**)&mLeftStrip);
        }
        if (mRightStrip == NULL) {
            resources->GetDrawable(R::drawable::tab_bottom_right_v4, (IDrawable**)&mRightStrip);
        }
    }
    else {
        // Use modern color scheme for Eclair and beyond
        if (mLeftStrip == NULL) {
            resources->GetDrawable(R::drawable::tab_bottom_left, (IDrawable**)&mLeftStrip);
        }
        if (mRightStrip == NULL) {
            resources->GetDrawable(R::drawable::tab_bottom_right, (IDrawable**)&mRightStrip);
        }
    }

    // Deal with focus, as we don't want the focus to go by default
    // to a tab other than the current tab
    SetFocusable(TRUE);
    SetOnFocusChangeListener(THIS_PROBE(IViewOnFocusChangeListener));
}

void TabWidget::MeasureChildBeforeLayout(
    /* [in] */ IView* child,
    /* [in] */ Int32 childIndex,
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 totalWidth,
    /* [in] */ Int32 heightMeasureSpec,
    /* [in] */ Int32 totalHeight)
{
    if (!IsMeasureWithLargestChildEnabled() && mImposedTabsHeight >= 0) {
        widthMeasureSpec = MeasureSpec::MakeMeasureSpec(
            totalWidth + (*mImposedTabWidths)[childIndex], MeasureSpec::EXACTLY);
        heightMeasureSpec = MeasureSpec::MakeMeasureSpec(mImposedTabsHeight,
            MeasureSpec::EXACTLY);
    }

    LinearLayout::MeasureChildBeforeLayout(child, childIndex,
        widthMeasureSpec, totalWidth, heightMeasureSpec, totalHeight);
}

void TabWidget::MeasureHorizontal(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    if (MeasureSpec::GetMode(widthMeasureSpec) == MeasureSpec::UNSPECIFIED) {
        LinearLayout::MeasureHorizontal(widthMeasureSpec, heightMeasureSpec);
        return;
    }

    // First, measure with no constraint
    Int32 unspecifiedWidth = MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED);
    mImposedTabsHeight = -1;
    LinearLayout::MeasureHorizontal(unspecifiedWidth, heightMeasureSpec);

    Int32 extraWidth = GetMeasuredWidth() - MeasureSpec::GetSize(widthMeasureSpec);
    if (extraWidth > 0) {
        Int32 count = GetChildCount();

        Int32 childCount = 0;
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> child = GetChildAt(i);
            Int32 visible;
            child->GetVisibility(&visible);

            if (visible == IView::GONE) continue;
            childCount++;
        }

        if (childCount > 0) {
            if (mImposedTabWidths == NULL || mImposedTabWidths->GetLength() != count) {
                mImposedTabWidths = ArrayOf<Int32>::Alloc(count);
            }
            Int32 visibility;
            for (Int32 i = 0; i < count; i++) {
                AutoPtr<IView> child = GetChildAt(i);
                child->GetVisibility(&visibility);
                if (visibility == IView::GONE) continue;
                Int32 childWidth, childHeight;
                child->GetMeasuredWidth(&childWidth);
                child->GetMeasuredHeight(&childHeight);
                Int32 delta = extraWidth / childCount;
                Int32 newWidth = Elastos::Core::Math::Max(0, childWidth - delta);
                (*mImposedTabWidths)[i] = newWidth;
                // Make sure the extra width is evenly distributed, no Int32 division remainder
                extraWidth -= childWidth - newWidth; // delta may have been clamped
                childCount--;
                mImposedTabsHeight = Elastos::Core::Math::Max(mImposedTabsHeight, childHeight);
            }
        }
    }

    // Measure again, this time with imposed tab widths and respecting initial spec request
    LinearLayout::MeasureHorizontal(widthMeasureSpec, heightMeasureSpec);
}

/**
 * Returns the tab indicator view at the given index.
 *
 * @param index the zero-based index of the tab indicator view to return
 * @return the tab indicator view at the given index
 */
AutoPtr<IView> TabWidget::GetChildTabViewAt(
    /* [in] */ Int32 index)
{
    return GetChildAt(index);
}

/**
 * Returns the number of tab indicator views.
 * @return the number of tab indicator views.
 */
Int32 TabWidget::GetTabCount()
{
    return GetChildCount();
}

/**
 * Sets the drawable to use as a divider between the tab indicators.
 * @param drawable the divider drawable
 */
ECode TabWidget::SetDividerDrawable(
    /* [in] */ IDrawable* drawable)
{
    return LinearLayout::SetDividerDrawable(drawable);
}

/**
 * Sets the drawable to use as a divider between the tab indicators.
 * @param resId the resource identifier of the drawable to use as a
 * divider.
 */
ECode TabWidget::SetDividerDrawable(
    /* [in] */ Int32 resId)
{
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);

    AutoPtr<IDrawable> dr;
    resources->GetDrawable(resId, (IDrawable**)&dr);

    return SetDividerDrawable(dr);
}

/**
 * Sets the drawable to use as the left part of the strip below the
 * tab indicators.
 * @param drawable the left strip drawable
 */
ECode TabWidget::SetLeftStripDrawable(
    /* [in] */ IDrawable* drawable)
{
    mLeftStrip = drawable;
    RequestLayout();
    Invalidate();

    return NOERROR;
}

/**
 * Sets the drawable to use as the left part of the strip below the
 * tab indicators.
 * @param resId the resource identifier of the drawable to use as the
 * left strip drawable
 */
ECode TabWidget::SetLeftStripDrawable(
    /* [in] */ Int32 resId)
{
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);

    AutoPtr<IDrawable> dr;
    resources->GetDrawable(resId, (IDrawable**)&dr);

    return SetLeftStripDrawable(dr);
}

/**
 * Sets the drawable to use as the right part of the strip below the
 * tab indicators.
 * @param drawable the right strip drawable
 */
ECode TabWidget::SetRightStripDrawable(
    /* [in] */ IDrawable* drawable)
{
    mRightStrip = drawable;
    RequestLayout();
    Invalidate();

    return NOERROR;
}

/**
 * Sets the drawable to use as the right part of the strip below the
 * tab indicators.
 * @param resId the resource identifier of the drawable to use as the
 * right strip drawable
 */
ECode TabWidget::SetRightStripDrawable(
    /* [in] */ Int32 resId)
{
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);

    AutoPtr<IDrawable> dr;
    resources->GetDrawable(resId, (IDrawable**)&dr);

    return SetRightStripDrawable(dr);
}

/**
 * Controls whether the bottom strips on the tab indicators are drawn or
 * not.  The default is to draw them.  If the user specifies a custom
 * view for the tab indicators, then the TabHost class calls this method
 * to disable drawing of the bottom strips.
 * @param stripEnabled TRUE if the bottom strips should be drawn.
 */
ECode TabWidget::SetStripEnabled(
    /* [in] */ Boolean stripEnabled)
{
    mDrawBottomStrips = stripEnabled;
    Invalidate();

    return NOERROR;
}

/**
 * Indicates whether the bottom strips on the tab indicators are drawn
 * or not.
 */
Boolean TabWidget::IsStripEnabled()
{
    return mDrawBottomStrips;
}

ECode TabWidget::ChildDrawableStateChanged(
    /* [in] */ IView* child)
{
    if (GetTabCount() > 0 && child == GetChildTabViewAt(mSelectedTab)) {
        // To make sure that the bottom strip is redrawn
        Invalidate();
    }
    return LinearLayout::ChildDrawableStateChanged(child);
}

void TabWidget::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    LinearLayout::DispatchDraw(canvas);

    // Do nothing if there are no tabs.
    if (GetTabCount() == 0) return;

    // If the user specified a custom view for the tab indicators, then
    // do not draw the bottom strips.
    if (!mDrawBottomStrips) {
        // Skip drawing the bottom strips.
        return;
    }

    AutoPtr<IView> selectedChild = GetChildTabViewAt(mSelectedTab);

    IDrawable* leftStrip = mLeftStrip;
    IDrawable* rightStrip = mRightStrip;

    AutoPtr< ArrayOf<Int32> > drawableState;
    selectedChild->GetDrawableState((ArrayOf<Int32>**)&drawableState);

    Boolean res;
    leftStrip->SetState(drawableState, &res);
    rightStrip->SetState(drawableState, &res);

    if (mStripMoved) {
        IRect* bounds = mBounds;
        Int32 l, r;
        selectedChild->GetLeft(&l);
        selectedChild->GetRight(&r);
        bounds->SetLeft(l);
        bounds->SetRight(r);

        Int32 myHeight = GetHeight();

        Int32 w, h;
        leftStrip->GetIntrinsicWidth(&w);
        leftStrip->GetIntrinsicHeight(&h);
        leftStrip->SetBounds(Elastos::Core::Math::Min(0, l - w),
                myHeight - h, l, myHeight);

        rightStrip->GetIntrinsicWidth(&w);
        rightStrip->GetIntrinsicHeight(&h);
        rightStrip->SetBounds(r, myHeight - h,
            Elastos::Core::Math::Max(GetWidth(), r + w), myHeight);
        mStripMoved = FALSE;
    }

    leftStrip->Draw(canvas);
    rightStrip->Draw(canvas);
}

/**
 * Sets the current tab.
 * This method is used to bring a tab to the front of the Widget,
 * and is used to post to the rest of the UI that a different tab
 * has been brought to the foreground.
 *
 * Note, this is separate from the traditional "focus" that is
 * employed from the view logic.
 *
 * For instance, if we have a list in a tabbed view, a user may be
 * navigating up and down the list, moving the UI focus (orange
 * highlighting) through the list items.  The cursor movement does
 * not effect the "selected" tab though, because what is being
 * scrolled through is all on the same tab.  The selected tab only
 * changes when we navigate between tabs (moving from the list view
 * to the next tabbed view, in this example).
 *
 * To move both the focus AND the selected tab at once, please use
 * {@link #setCurrentTab}. Normally, the view logic takes care of
 * adjusting the focus, so unless you're circumventing the UI,
 * you'll probably just focus your interest here.
 *
 *  @param index The tab that you want to indicate as the selected
 *  tab (tab brought to the front of the widget)
 *
 *  @see #focusCurrentTab
 */
ECode TabWidget::SetCurrentTab(
    /* [in] */ Int32 index)
{
    if (index < 0 || index >= GetTabCount() || index == mSelectedTab) {
        return NOERROR;
    }

    if (mSelectedTab != -1) {
        GetChildTabViewAt(mSelectedTab)->SetSelected(FALSE);
    }

    mSelectedTab = index;
    AutoPtr<IView> tabView = GetChildTabViewAt(mSelectedTab);
    tabView->SetSelected(TRUE);
    mStripMoved = TRUE;

    if (IsShown()) {
        SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_SELECTED);
    }

    return NOERROR;
}

Boolean TabWidget::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    OnPopulateAccessibilityEvent(event);
    // Dispatch only to the selected tab.
    if (mSelectedTab != -1) {
        AutoPtr<IView> tabView = GetChildTabViewAt(mSelectedTab);
        Int32 visible;
        if (tabView != NULL && (tabView->GetVisibility(&visible), visible) == IView::VISIBLE) {
            Boolean result;
            tabView->DispatchPopulateAccessibilityEvent(event, &result);
            return result;
        }
    }
    return FALSE;
}

ECode TabWidget::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    LinearLayout::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CTabWidget"), (ICharSequence**)&seq);
    event->SetClassName(seq);
    event->SetItemCount(GetTabCount());
    event->SetCurrentItemIndex(mSelectedTab);
    return NOERROR;
}

ECode TabWidget::SendAccessibilityEventUnchecked(
    /* [in] */ IAccessibilityEvent* event)
{
    // this class fires events only when tabs are focused or selected
    Int32 type;
    event->GetEventType(&type);
    if (type == IAccessibilityEvent::TYPE_VIEW_FOCUSED && IsFocused()) {
        event->Recycle();
        return NOERROR;
    }
    return LinearLayout::SendAccessibilityEventUnchecked(event);
}

ECode TabWidget::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    LinearLayout::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> clsName;
    CStringWrapper::New(String("CTabWidget"), (ICharSequence**)&clsName);
    info->SetClassName(clsName);
    return NOERROR;
}

/**
 * Sets the current tab and focuses the UI on it.
 * This method makes sure that the focused tab matches the selected
 * tab, normally at {@link #setCurrentTab}.  Normally this would not
 * be an issue if we go through the UI, since the UI is responsible
 * for calling TabWidget.onFocusChanged(), but in the case where we
 * are selecting the tab programmatically, we'll need to make sure
 * focus keeps up.
 *
 *  @param index The tab that you want focused (highlighted in orange)
 *  and selected (tab brought to the front of the widget)
 *
 *  @see #setCurrentTab
 */
ECode TabWidget::FocusCurrentTab(
    /* [in] */ Int32 index)
{
    Int32 oldTab = mSelectedTab;

    // set the tab
    SetCurrentTab(index);

    // change the focus if applicable.
    if (oldTab != index) {
        AutoPtr<IView> tabView = GetChildTabViewAt(index);
        Boolean res;
        tabView->RequestFocus(&res);
    }

    return NOERROR;
}

ECode TabWidget::SetEnabled(
    /* [in] */ Boolean enabled)
{
    LinearLayout::SetEnabled(enabled);
    Int32 count = GetTabCount();

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> tabView = GetChildTabViewAt(i);
        tabView->SetEnabled(enabled);
    }

    return NOERROR;
}

ECode TabWidget::AddView(
    /* [in] */ IView* child)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    if (lp == NULL) {
        AutoPtr<ILinearLayoutLayoutParams> lp;
        CLinearLayoutLayoutParams::New(0, IViewGroupLayoutParams::MATCH_PARENT,
                1.0f, (ILinearLayoutLayoutParams**)&lp);
        lp->SetMargins(0, 0, 0, 0);
        child->SetLayoutParams(lp);
    }

    // Ensure you can navigate to the tab with the keyboard, and you can touch it
    child->SetFocusable(TRUE);
    child->SetClickable(TRUE);

    LinearLayout::AddView(child);

    // TODO: detect this via geometry with a tabwidget listener rather
    // than potentially interfere with the view's listener
    AutoPtr<TabClickListener> listener = new TabClickListener(GetTabCount() - 1, this);
    child->SetOnClickListener((IViewOnClickListener*)listener);
    child->SetOnFocusChangeListener(THIS_PROBE(IViewOnFocusChangeListener));

    return NOERROR;
}

ECode TabWidget::RemoveAllViews()
{
    LinearLayout::RemoveAllViews();
    mSelectedTab = -1;
    return NOERROR;
}

/**
 * Provides a way for {@link TabHost} to be notified that the user clicked on a tab indicator.
 */
void TabWidget::SetTabSelectionListener(
    /* [in] */ ITabWidgetOnTabSelectionChanged* listener)
{
    mSelectionChangedListener = listener;
}

ECode TabWidget::OnFocusChange(
    /* [in] */ IView* v,
    /* [in] */ Boolean hasFocus)
{
    if (v == THIS_PROBE(IView) && hasFocus && GetTabCount() > 0) {
        AutoPtr<IView> tabView = GetChildTabViewAt(mSelectedTab);
        Boolean res;
        return tabView->RequestFocus(&res);
    }

    if (hasFocus) {
        Int32 i = 0;
        Int32 numTabs = GetTabCount();
        while (i < numTabs) {
            AutoPtr<IView> tabView = GetChildTabViewAt(mSelectedTab);
            if (tabView.Get() == v) {
                SetCurrentTab(i);
                mSelectionChangedListener->OnTabSelectionChanged(i, FALSE);

                if (IsShown()) {
                    // a tab is focused so send an event to announce the tab widget state
                    SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_FOCUSED);
                }

                break;
            }
            i++;
        }
    }

    return NOERROR;
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos
