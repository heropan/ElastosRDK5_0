#include "_CAutoCompleteTextView.h"
#include "AutoCompleteTextView.h"
#include "view/ViewMacro.h"
#include "TextViewMacro.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::Drawable::IDrawable;

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CAutoCompleteTextView), public AutoCompleteTextView
{
public:

    IVIEW_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    ITEXTVIEW_METHODS_DECL()

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

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);
    /**
     * Convenience for {@link Selection#setSelection(Spannable, Int32, Int32)}.
     */
    CARAPI SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 stop);

    /**
     * Convenience for {@link Selection#setSelection(Spannable, Int32)}.
     */
    CARAPI SetSelectionEx(
        /* [in] */ Int32 index);

    /**
     * Convenience for {@link Selection#selectAll}.
     */
    CARAPI SelectAll();

    /**
     * Convenience for {@link Selection#extendSelection}.
     */
    CARAPI ExtendSelection(
        /* [in] */ Int32 index);

    CARAPI SetCompletionHint(
        /* [in] */ ICharSequence* hint);

    CARAPI GetCompletionHint(
        /* [out] */ ICharSequence** hint);

    CARAPI GetDropDownWidth(
        /* [out] */ Int32* width);

    CARAPI SetDropDownWidth(
        /* [in] */ Int32 width);

    CARAPI GetDropDownHeight(
        /* [out] */ Int32* height);

    CARAPI SetDropDownHeight(
        /* [in] */ Int32 height);

    CARAPI GetDropDownAnchor(
        /* [out] */ Int32* id);

    CARAPI SetDropDownAnchor(
        /* [in] */ Int32 id);

    CARAPI GetDropDownBackground(
        /* [out] */ IDrawable** drawable);

    CARAPI SetDropDownBackgroundDrawable(
        /* [in] */ IDrawable* d);

    CARAPI SetDropDownBackgroundResource(
        /* [in] */ Int32 id);

    CARAPI SetDropDownVerticalOffset(
        /* [in] */ Int32 offset);

    CARAPI GetDropDownVerticalOffset(
        /* [out] */ Int32* offset);

    CARAPI SetDropDownHorizontalOffset(
        /* [in] */ Int32 offset);

    CARAPI GetDropDownHorizontalOffset(
        /* [out] */ Int32* offset);

    CARAPI SetDropDownAnimationStyle(
        /* [in] */ Int32 animationStyle);

    CARAPI GetDropDownAnimationStyle(
        /* [out] */ Int32* style);

    CARAPI IsDropDownAlwaysVisible(
        /* [out] */ Boolean* visible);

    CARAPI SetDropDownAlwaysVisible(
        /* [in] */ Boolean dropDownAlwaysVisible);

    CARAPI IsDropDownDismissedOnCompletion(
        /* [out] */ Boolean* dropDownDismissedOnCompletion);

    CARAPI SetDropDownDismissedOnCompletion(
        /* [in] */ Boolean dropDownDismissedOnCompletion);

    CARAPI GetThreshold(
        /* [out] */ Int32* threshold);

    CARAPI SetThreshold(
        /* [in] */ Int32 threshold);

    CARAPI SetOnItemClickListener(
        /* [in] */ IAdapterViewOnItemClickListener* listener);

    CARAPI SetOnItemSelectedListener(
        /* [in] */ IAdapterViewOnItemSelectedListener* listener);

    CARAPI GetOnItemClickListener(
        /* [out] */ IAdapterViewOnItemClickListener** l);

    CARAPI GetOnItemSelectedListener(
        /* [out] */ IAdapterViewOnItemSelectedListener** l);

    CARAPI GetItemClickListener(
        /* [out] */ IAdapterViewOnItemClickListener** l);

    CARAPI GetItemSelectedListener(
        /* [out] */ IAdapterViewOnItemSelectedListener** l);

    CARAPI SetOnDismissListener(
        /* [in] */ IAutoCompleteTextViewOnDismissListener* dismissListener);

    CARAPI GetAdapter(
        /* [out] */ IListAdapter** adapter);

    CARAPI SetAdapter(
        /* [in] */ IListAdapter* adapter);

    CARAPI EnoughToFilter(
        /* [out] */ Boolean* result);

    CARAPI IsPopupShowing(
        /* [out] */ Boolean* result);

    CARAPI ClearListSelection();

    CARAPI SetListSelection(
        /* [in] */ Int32 position);

    CARAPI GetListSelection(
        /* [out] */ Int32* selection);

    CARAPI PerformCompletion();

    CARAPI IsPerformingCompletion(
        /* [out] */ Boolean* result);
    CARAPI DismissDropDown();

    CARAPI ShowDropDownAfterLayout();

    CARAPI EnsureImeVisible(
        /* [in] */ Boolean visible);

    CARAPI IsInputMethodNotNeeded(
        /* [out] */ Boolean* needed);

    CARAPI ShowDropDown();

    CARAPI SetForceIgnoreOutsideTouch(
        /* [in] */ Boolean forceIgnoreOutsideTouch);

    CARAPI SetValidator(
        /* [in] */ IValidator* validator);

    CARAPI GetValidator(
        /* [out] */ IValidator** validator);

    CARAPI PerformValidation();

    CARAPI OnFilterComplete(
        /* [in] */ Int32 count);

    CARAPI SetTextFilter(
        /* [in] */ ICharSequence* text,
        /* [in] */ Boolean filter);

};

}// namespace Widget
}// namespace Droid
}// namespace Elastos
