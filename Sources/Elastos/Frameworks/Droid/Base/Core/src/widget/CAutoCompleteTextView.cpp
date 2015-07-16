#include "CAutoCompleteTextView.h"

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CAutoCompleteTextView, AutoCompleteTextView)
IDRAWABLECALLBACK_METHODS_IMPL(CAutoCompleteTextView, AutoCompleteTextView)
IKEYEVENTCALLBACK_METHODS_IMPL(CAutoCompleteTextView, AutoCompleteTextView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CAutoCompleteTextView, AutoCompleteTextView)
ITEXTVIEW_METHODS_IMPL(CAutoCompleteTextView, AutoCompleteTextView)

PInterface CAutoCompleteTextView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    } else if (riid == EIID_TextView) {
        return reinterpret_cast<PInterface>((TextView*)this);
    }
    return _CAutoCompleteTextView::Probe(riid);
}

ECode CAutoCompleteTextView::constructor(
    /* [in] */ IContext* context)
{
    return AutoCompleteTextView::Init(context);
}

ECode CAutoCompleteTextView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return AutoCompleteTextView::Init(context, attrs);
}

ECode CAutoCompleteTextView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return AutoCompleteTextView::Init(context, attrs, defStyle);
}

ECode CAutoCompleteTextView::OnPreDraw(
    /* [out] */ Boolean* result)
{
    *result = AutoCompleteTextView::OnPreDraw();
    return NOERROR;
}
/**
 * Convenience for {@link Selection#setSelection(Spannable, Int32, Int32)}.
 */
ECode CAutoCompleteTextView::SetSelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 stop)
{
    return AutoCompleteTextView::SetSelection(start, stop);
}

/**
 * Convenience for {@link Selection#setSelection(Spannable, Int32)}.
 */
ECode CAutoCompleteTextView::SetSelection(
    /* [in] */ Int32 index)
{
    return AutoCompleteTextView::SetSelection(index);
}

/**
 * Convenience for {@link Selection#selectAll}.
 */
ECode CAutoCompleteTextView::SelectAll()
{
    return AutoCompleteTextView::SelectAll();
}

/**
 * Convenience for {@link Selection#extendSelection}.
 */
ECode CAutoCompleteTextView::ExtendSelection(
    /* [in] */ Int32 index)
{
    return AutoCompleteTextView::ExtendSelection(index);
}

ECode CAutoCompleteTextView::SetCompletionHint(
    /* [in] */ ICharSequence* hint)
{
    return AutoCompleteTextView::SetCompletionHint(hint);
}

ECode CAutoCompleteTextView::GetCompletionHint(
    /* [out] */ ICharSequence** hint)
{
    AutoPtr<ICharSequence> temp;
    temp = AutoCompleteTextView::GetCompletionHint();
    *hint = temp;
    REFCOUNT_ADD(*hint);
    return NOERROR;
}

ECode CAutoCompleteTextView::GetDropDownWidth(
    /* [out] */ Int32* width)
{
    *width = AutoCompleteTextView::GetDropDownWidth();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownWidth(
    /* [in] */ Int32 width)
{
    return AutoCompleteTextView::SetDropDownWidth(width);
}

ECode CAutoCompleteTextView::GetDropDownHeight(
    /* [out] */ Int32* height)
{
    *height = AutoCompleteTextView::GetDropDownHeight();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownHeight(
    /* [in] */ Int32 height)
{
    return AutoCompleteTextView::SetDropDownHeight(height);
}

ECode CAutoCompleteTextView::GetDropDownAnchor(
    /* [out] */ Int32* id)
{
    *id = AutoCompleteTextView::GetDropDownAnchor();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownAnchor(
    /* [in] */ Int32 id)
{
    return AutoCompleteTextView::SetDropDownAnchor(id);
}

ECode CAutoCompleteTextView::GetDropDownBackground(
    /* [out] */ IDrawable** drawable)
{
    AutoPtr<IDrawable> temp = AutoCompleteTextView::GetDropDownBackground();
    (*drawable) = temp;
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownBackgroundDrawable(
    /* [in] */ IDrawable* d)
{
    return AutoCompleteTextView::SetDropDownBackgroundDrawable(d);
}

ECode CAutoCompleteTextView::SetDropDownBackgroundResource(
    /* [in] */ Int32 id)
{
    return AutoCompleteTextView::SetDropDownBackgroundResource(id);
}

ECode CAutoCompleteTextView::SetDropDownVerticalOffset(
    /* [in] */ Int32 offset)
{
    return AutoCompleteTextView::SetDropDownVerticalOffset(offset);
}

ECode CAutoCompleteTextView::GetDropDownVerticalOffset(
    /* [out] */ Int32* offset)
{
    *offset = AutoCompleteTextView::GetDropDownVerticalOffset();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownHorizontalOffset(
    /* [in] */ Int32 offset)
{
    return AutoCompleteTextView::SetDropDownHorizontalOffset(offset);
}

ECode CAutoCompleteTextView::GetDropDownHorizontalOffset(
    /* [out] */ Int32* offset)
{
    *offset = AutoCompleteTextView::GetDropDownHorizontalOffset();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownAnimationStyle(
    /* [in] */ Int32 animationStyle)
{
    return AutoCompleteTextView::SetDropDownAnimationStyle(animationStyle);
}

ECode CAutoCompleteTextView::GetDropDownAnimationStyle(
    /* [out] */ Int32* style)
{
    *style = AutoCompleteTextView::GetDropDownAnimationStyle();
    return NOERROR;
}

ECode CAutoCompleteTextView::IsDropDownAlwaysVisible(
    /* [out] */ Boolean* visible)
{
    *visible = AutoCompleteTextView::IsDropDownAlwaysVisible();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownAlwaysVisible(
    /* [in] */ Boolean dropDownAlwaysVisible)
{
    return AutoCompleteTextView::SetDropDownAlwaysVisible(dropDownAlwaysVisible);
}

ECode CAutoCompleteTextView::IsDropDownDismissedOnCompletion(
    /* [out] */ Boolean* dropDownDismissedOnCompletion)
{
    *dropDownDismissedOnCompletion = AutoCompleteTextView::IsDropDownDismissedOnCompletion();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetDropDownDismissedOnCompletion(
    /* [in] */ Boolean dropDownDismissedOnCompletion)
{
    return AutoCompleteTextView::SetDropDownDismissedOnCompletion(dropDownDismissedOnCompletion);
}

ECode CAutoCompleteTextView::GetThreshold(
    /* [out] */ Int32* threshold)
{
    *threshold = AutoCompleteTextView::GetThreshold();
    return NOERROR;
}

ECode CAutoCompleteTextView::SetThreshold(
    /* [in] */ Int32 threshold)
{
    return AutoCompleteTextView::SetThreshold(threshold);
}

ECode CAutoCompleteTextView::SetOnItemClickListener(
    /* [in] */ IAdapterViewOnItemClickListener* listener)
{
    return AutoCompleteTextView::SetOnItemClickListener(listener);
}

ECode CAutoCompleteTextView::SetOnItemSelectedListener(
    /* [in] */ IAdapterViewOnItemSelectedListener* listener)
{
    return AutoCompleteTextView::SetOnItemSelectedListener(listener);
}

ECode CAutoCompleteTextView::GetOnItemClickListener(
    /* [out] */ IAdapterViewOnItemClickListener** l)
{
    AutoPtr<IAdapterViewOnItemClickListener> temp;
    *l = temp;
    REFCOUNT_ADD(*l)
    return NOERROR;
}

ECode CAutoCompleteTextView::GetOnItemSelectedListener(
    /* [out] */ IAdapterViewOnItemSelectedListener** l)
{
    AutoPtr<IAdapterViewOnItemSelectedListener> temp;
    temp = AutoCompleteTextView::GetOnItemSelectedListener();
    (*l) = temp;
    REFCOUNT_ADD(*l)
    return NOERROR;
}

ECode CAutoCompleteTextView::GetItemClickListener(
    /* [out] */ IAdapterViewOnItemClickListener** l)
{
    AutoPtr<IAdapterViewOnItemClickListener> temp;
    temp = AutoCompleteTextView::GetOnItemClickListener();
    *l = temp;
    REFCOUNT_ADD(*l)
    return NOERROR;
}

ECode CAutoCompleteTextView::GetItemSelectedListener(
    /* [out] */ IAdapterViewOnItemSelectedListener** l)
{
    AutoPtr<IAdapterViewOnItemSelectedListener> temp;
    temp = AutoCompleteTextView::GetOnItemSelectedListener();
    (*l) = temp;
    REFCOUNT_ADD(*l)
    return NOERROR;
}

ECode CAutoCompleteTextView::SetOnDismissListener(
    /* [in] */ IAutoCompleteTextViewOnDismissListener* dismissListener)
{
    return AutoCompleteTextView::SetOnDismissListener(dismissListener);
}

ECode CAutoCompleteTextView::GetAdapter(
    /* [out] */ IListAdapter** adapter)
{
    AutoPtr<IListAdapter> temp;
    temp = AutoCompleteTextView::GetAdapter();
    *adapter = temp;
    REFCOUNT_ADD(*adapter);
    return NOERROR;
}

ECode CAutoCompleteTextView::SetAdapter(
    /* [in] */ IListAdapter* adapter)
{
    return AutoCompleteTextView::SetAdapter(adapter);
}

ECode CAutoCompleteTextView::EnoughToFilter(
    /* [out] */ Boolean* result)
{
    *result = AutoCompleteTextView::EnoughToFilter();
    return NOERROR;
}

ECode CAutoCompleteTextView::IsPopupShowing(
    /* [out] */ Boolean* result)
{
    *result = AutoCompleteTextView::IsPopupShowing();
    return NOERROR;
}

ECode CAutoCompleteTextView::ClearListSelection()
{
    return AutoCompleteTextView::ClearListSelection();
}

ECode CAutoCompleteTextView::SetListSelection(
    /* [in] */ Int32 position)
{
    return AutoCompleteTextView::SetListSelection(position);
}

ECode CAutoCompleteTextView::GetListSelection(
    /* [out] */ Int32* selection)
{
    *selection = AutoCompleteTextView::GetListSelection();
    return NOERROR;
}

ECode CAutoCompleteTextView::PerformCompletion()
{
    return AutoCompleteTextView::PerformCompletion();
}

ECode CAutoCompleteTextView::IsPerformingCompletion(
    /* [out] */ Boolean* result)
{
    *result = AutoCompleteTextView::IsPerformingCompletion();
    return NOERROR;
}


ECode CAutoCompleteTextView::DismissDropDown()
{
    return AutoCompleteTextView::DismissDropDown();
}

ECode CAutoCompleteTextView::ShowDropDownAfterLayout()
{
    return AutoCompleteTextView::ShowDropDownAfterLayout();
}

ECode CAutoCompleteTextView::EnsureImeVisible(
    /* [in] */ Boolean visible)
{
    return AutoCompleteTextView::EnsureImeVisible(visible);
}

ECode CAutoCompleteTextView::IsInputMethodNotNeeded(
    /* [out] */ Boolean* needed)
{
    *needed = AutoCompleteTextView::IsInputMethodNotNeeded();
    return NOERROR;
}

ECode CAutoCompleteTextView::ShowDropDown()
{
    return AutoCompleteTextView::ShowDropDown();
}

ECode CAutoCompleteTextView::SetForceIgnoreOutsideTouch(
    /* [in] */ Boolean forceIgnoreOutsideTouch)
{
    return AutoCompleteTextView::SetForceIgnoreOutsideTouch(forceIgnoreOutsideTouch);
}

ECode CAutoCompleteTextView::SetValidator(
    /* [in] */ IValidator* validator)
{
    return AutoCompleteTextView::SetValidator(validator);
}

ECode CAutoCompleteTextView::GetValidator(
    /* [out] */ IValidator** validator)
{
    VALIDATE_NOT_NULL(validator);
    AutoPtr<IValidator> temp = AutoCompleteTextView::GetValidator();
    *validator = temp;
    REFCOUNT_ADD(*validator);
    return NOERROR;
}

ECode CAutoCompleteTextView::PerformValidation()
{
    return AutoCompleteTextView::PerformValidation();
}

ECode CAutoCompleteTextView::OnFilterComplete(
        /* [in] */ Int32 count)
{
    return AutoCompleteTextView::OnFilterComplete(count);
}

ECode CAutoCompleteTextView::SetTextFilter(
    /* [in] */ ICharSequence* text,
    /* [in] */ Boolean filter)
{
    return AutoCompleteTextView::SetText(text, filter);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
