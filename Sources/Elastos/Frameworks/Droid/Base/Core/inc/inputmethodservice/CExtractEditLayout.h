
#ifndef  __CEXTRACTEDITLAYOUT_H__
#define  __CEXTRACTEDITLAYOUT_H__

#include "_CExtractEditLayout.h"
#include "widget/LinearLayout.h"


#include "view/ActionMode.h"

using Elastos::Droid::Widget::LinearLayout;
using Elastos::Droid::Widget::IButton;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::ActionMode;
using Elastos::Droid::View::IMenuItem;
using Elastos::Droid::View::IMenuInflater;
using Elastos::Droid::View::IViewOnClickListener;;
using Elastos::Droid::View::Menu::IMenuBuilder;
using Elastos::Droid::View::Menu::IMenuBuilderCallback;


namespace Elastos {
namespace Droid {
namespace InputMethodService {

class ExtractEditLayout : public LinearLayout
{
protected:
    class ExtractActionMode
        : public ElRefBase
        , public ActionMode
        , public IMenuBuilderCallback
    {
    public:
        ExtractActionMode(
            /* [in] */ IActionModeCallback* cb,
            /* [in] */ ExtractEditLayout* host);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        //@Override
        CARAPI SetTitle(
            /* [in] */ ICharSequence* title);

        //@Override
        CARAPI SetTitle(
            /* [in] */ Int32 resId);

        //@Override
        CARAPI SetSubtitle(
            /* [in] */ ICharSequence* subtitle);

        //@Override
        CARAPI SetSubtitle(
            /* [in] */ Int32 resId);

        //@Override
        CARAPI IsTitleOptional(
            /* [out] */ Boolean* result);

        //@Override
        CARAPI SetCustomView(
            /* [in] */ IView* view);

        //@Override
        CARAPI Invalidate();

        CARAPI_(Boolean) DispatchOnCreate();

        //@Override
        CARAPI Finish();

        //@Override
        CARAPI GetMenu(
            /* [out] */ IMenu** menu);

        //@Override
        CARAPI GetTitle(
            /* [out] */ ICharSequence** title);

        //@Override
        CARAPI GetSubtitle(
            /* [out] */ ICharSequence** subtitle);

        //@Override
        CARAPI GetCustomView(
            /* [out] */ IView** view);

        //@Override
        CARAPI GetMenuInflater(
            /* [out] */ IMenuInflater** inflater);

        //@Override
        CARAPI OnMenuItemSelected(
            /* [in] */ IMenuBuilder* menu,
            /* [in] */ IMenuItem* item,
            /* [out] */ Boolean* result);

        //@Override
        CARAPI OnMenuModeChange(
            /* [in] */ IMenuBuilder* menu);

        virtual CARAPI SetTag(
            /* [in] */ IInterface* tag);

        virtual CARAPI GetTag(
            /* [out] */ IInterface** tag);

        virtual CARAPI SetTitleOptionalHint(
            /* [in] */ Boolean titleOptional);

        virtual CARAPI GetTitleOptionalHint(
            /* [out] */ Boolean* titleOptionalHint);

        virtual CARAPI IsUiFocusable(
            /* [out] */ Boolean* isUiFocusable);


    private:
        AutoPtr<IActionModeCallback> mCallback;
        AutoPtr<IMenuBuilder> mMenu;
        ExtractEditLayout* mHost;
    };

    class _OnClickListener
        : public ElRefBase
        , public IViewOnClickListener
    {
    public:
        _OnClickListener(
            /* [in] */ ExtractEditLayout* host);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI OnClick(
            /* [in] */ IView* clicked);

    private:
        ExtractEditLayout* mHost;
    };

public:
    //@Override
    CARAPI_(AutoPtr<IActionMode>) StartActionModeForChild(
        /* [in] */ IView* sourceView,
        /* [in] */ IActionModeCallback* cb);

    //@Override
    CARAPI OnFinishInflate();

protected:
    AutoPtr<ExtractActionMode> mActionMode;
    AutoPtr<IButton> mExtractActionButton;
    AutoPtr<IButton> mEditButton;
};

/**
 * ExtractEditLayout provides an ActionMode presentation for the
 * limited screen real estate in extract mode.
 *
 * @hide
 */
CarClass(CExtractEditLayout), public ExtractEditLayout
{
public:
    IVIEW_METHODS_DECL()

    IVIEWGROUP_METHODS_DECL()

    IVIEWPARENT_METHODS_DECL()

    IVIEWMANAGER_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    //TODO ILINEARLAYOUT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    /**
     * @return true if an action mode is currently active.
     */
    CARAPI IsActionModeStarted(
        /* [out] */ Boolean* started);

    /**
     * Finishes a possibly started action mode.
     */
    CARAPI FinishActionMode();

    /**
     * Set how dividers should be shown between items in this layout
     *
     * @param showDividers One or more of {@link #SHOW_DIVIDER_BEGINNING},
     *                     {@link #SHOW_DIVIDER_MIDDLE}, or {@link #SHOW_DIVIDER_END},
     *                     or {@link #SHOW_DIVIDER_NONE} to show no dividers.
     */
    CARAPI SetShowDividers(
        /* [in] */ Int32 showDividers);

    /**
     * @return A flag set indicating how dividers should be shown around items.
     * @see #setShowDividers(int)
     */
    CARAPI GetShowDividers(
        /* [out] */ Int32* showDividers);

    /**
     * @return the divider Drawable that will divide each item.
     *
     * @see #setDividerDrawable(Drawable)
     *
     * @attr ref android.R.styleable#LinearLayout_divider
     */
    CARAPI GetDividerDrawable(
        /* [out] */ IDrawable** divider);

    /**
     * Set a drawable to be used as a divider between items.
     *
     * @param divider Drawable that will divide each item.
     *
     * @see #setShowDividers(int)
     *
     * @attr ref android.R.styleable#LinearLayout_divider
     */
    CARAPI SetDividerDrawable(
        /* [in] */ IDrawable* divider);

    /**
     * Set padding displayed on both ends of dividers.
     *
     * @param padding Padding value in pixels that will be applied to each end
     *
     * @see #setShowDividers(int)
     * @see #setDividerDrawable(Drawable)
     * @see #getDividerPadding()
     */
    CARAPI SetDividerPadding(
        /* [in] */ Int32 padding);

    /**
     * Get the padding size used to inset dividers in pixels
     *
     * @see #setShowDividers(int)
     * @see #setDividerDrawable(Drawable)
     * @see #setDividerPadding(int)
     */
    CARAPI GetDividerPadding(
        /* [out] */ Int32* padding);

    /**
     * Get the width of the current divider drawable.
     *
     * @hide Used internally by framework.
     */
    CARAPI GetDividerWidth(
        /* [out] */ Int32* width);

    /**
     * <p>Indicates whether widgets contained within this layout are aligned
     * on their baseline or not.</p>
     *
     * @return true when widgets are baseline-aligned, false otherwise
     */
    CARAPI IsBaselineAligned(
        /* [out] */ Boolean* baselineAligned);

    /**
     * <p>Defines whether widgets contained in this layout are
     * baseline-aligned or not.</p>
     *
     * @param baselineAligned true to align widgets on their baseline,
     *         false otherwise
     *
     * @attr ref android.R.styleable#LinearLayout_baselineAligned
     */
    CARAPI SetBaselineAligned(
        /* [in] */ Boolean baselineAligned);

    /**
     * When true, all children with a weight will be considered having
     * the minimum size of the largest child. If false, all children are
     * measured normally.
     *
     * @return True to measure children with a weight using the minimum
     *         size of the largest child, false otherwise.
     *
     * @attr ref android.R.styleable#LinearLayout_measureWithLargestChild
     */
    CARAPI IsMeasureWithLargestChildEnabled(
        /* [out] */ Boolean* enabled);

    /**
     * When set to true, all children with a weight will be considered having
     * the minimum size of the largest child. If false, all children are
     * measured normally.
     *
     * Disabled by default.
     *
     * @param enabled True to measure children with a weight using the
     *        minimum size of the largest child, false otherwise.
     *
     * @attr ref android.R.styleable#LinearLayout_measureWithLargestChild
     */
    CARAPI SetMeasureWithLargestChildEnabled(
        /* [in] */ Boolean enabled);

    /**
     * @return The index of the child that will be used if this layout is
     *   part of a larger layout that is baseline aligned, or -1 if none has
     *   been set.
     */
    CARAPI GetBaselineAlignedChildIndex(
        /* [out] */ Int32* index);

    /**
     * @param i The index of the child that will be used if this layout is
     *          part of a larger layout that is baseline aligned.
     *
     * @attr ref android.R.styleable#LinearLayout_baselineAlignedChildIndex
     */
    CARAPI SetBaselineAlignedChildIndex(
        /* [in] */ Int32 index);

    /**
     * Returns the desired weights sum.
     *
     * @return A number greater than 0.0f if the weight sum is defined, or
     *         a number lower than or equals to 0.0f if not weight sum is
     *         to be used.
     */
    CARAPI GetWeightSum(
        /* [out] */ Float* weightSum);

    /**
     * Defines the desired weights sum. If unspecified the weights sum is computed
     * at layout time by adding the layout_weight of each child.
     *
     * This can be used for instance to give a single child 50% of the total
     * available space by giving it a layout_weight of 0.5 and setting the
     * weightSum to 1.0.
     *
     * @param weightSum a number greater than 0.0f, or a number lower than or equals
     *        to 0.0f if the weight sum should be computed from the children's
     *        layout_weight
     */
    CARAPI SetWeightSum(
        /* [in] */ Float weightSum);

    /**
     * Should the layout be a column or a row.
     * @param orientation Pass HORIZONTAL or VERTICAL. Default
     * value is HORIZONTAL.
     *
     * @attr ref android.R.styleable#LinearLayout_orientation
     */
    CARAPI SetOrientation(
        /* [in] */ Int32 orientation);

    /**
     * Returns the current orientation.
     *
     * @return either {@link #HORIZONTAL} or {@link #VERTICAL}
     */
    CARAPI GetOrientation(
        /* [out] */ Int32* orientation);

    /**
     * Describes how the child views are positioned. Defaults to GRAVITY_TOP. If
     * this layout has a VERTICAL orientation, this controls where all the child
     * views are placed if there is extra vertical space. If this layout has a
     * HORIZONTAL orientation, this controls the alignment of the children.
     *
     * @param gravity See {@link android.view.Gravity}
     *
     * @attr ref android.R.styleable#LinearLayout_gravity
     */
    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetHorizontalGravity(
        /* [in] */ Int32 horizontalGravity);

    CARAPI SetVerticalGravity(
        /* [in] */ Int32 verticalGravity);
};

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos

#endif  // __CEXTRACTEDITLAYOUT_H__
