
#ifndef __ELASTOS_DROID_WIDGET_CTABWIDGET_H__
#define __ELASTOS_DROID_WIDGET_CTABWIDGET_H__

#include "_CTabWidget.h"



#include "widget/TabWidget.h"


namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CTabWidget), public TabWidget
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    ILINEARLAYOUT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);


    /**
     * Returns the tab indicator view at the given index.
     *
     * @param index the zero-based index of the tab indicator view to return
     * @return the tab indicator view at the given index
     */
    CARAPI GetChildTabViewAt(
        /* [in] */ Int32 index,
        /* [out] */ IView** view);

    /**
     * Returns the number of tab indicator views.
     * @return the number of tab indicator views.
     */
    CARAPI GetTabCount(
        /* [out] */ Int32* count);

    /**
     * Sets the drawable to use as a divider between the tab indicators.
     * @param resId the resource identifier of the drawable to use as a
     * divider.
     */
    CARAPI SetDividerDrawable(
        /* [in] */ Int32 resId);

    /**
     * Sets the drawable to use as the left part of the strip below the
     * tab indicators.
     * @param drawable the left strip drawable
     */
    CARAPI SetLeftStripDrawable(
        /* [in] */ IDrawable* drawable);

    /**
     * Sets the drawable to use as the left part of the strip below the
     * tab indicators.
     * @param resId the resource identifier of the drawable to use as the
     * left strip drawable
     */
    CARAPI SetLeftStripDrawable(
        /* [in] */ Int32 resId);

    /**
     * Sets the drawable to use as the right part of the strip below the
     * tab indicators.
     * @param drawable the right strip drawable
     */
    CARAPI SetRightStripDrawable(
        /* [in] */ IDrawable* drawable);

    /**
     * Sets the drawable to use as the right part of the strip below the
     * tab indicators.
     * @param resId the resource identifier of the drawable to use as the
     * right strip drawable
     */
    CARAPI SetRightStripDrawable(
        /* [in] */ Int32 resId);

    /**
     * Controls whether the bottom strips on the tab indicators are drawn or
     * not.  The default is to draw them.  If the user specifies a custom
     * view for the tab indicators, then the TabHost class calls this method
     * to disable drawing of the bottom strips.
     * @param stripEnabled true if the bottom strips should be drawn.
     */
    CARAPI SetStripEnabled(
        /* [in] */ Boolean stripEnabled);

    /**
     * Indicates whether the bottom strips on the tab indicators are drawn
     * or not.
     */
    CARAPI IsStripEnabled(
        /* [out] */ Boolean* stripEnabled);

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
    CARAPI SetCurrentTab(
        /* [in] */ Int32 index);

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
    CARAPI FocusCurrentTab(
        /* [in] */ Int32 index);


    // IViewOnFocusChangeListener

    /**
     * Called when the focus state of a view has changed.
     *
     * @param v The view whose state has changed.
     * @param hasFocus The new focus state of v.
     */
    CARAPI OnFocusChange(
        /* [in] */ IView* v,
        /* [in] */ Boolean hasFocus);
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CTABWIDGET_H__
