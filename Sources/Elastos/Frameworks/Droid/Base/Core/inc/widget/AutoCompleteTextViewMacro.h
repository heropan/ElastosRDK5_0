#ifndef __AUTOCOMPLETETEXTVIEWMACRO_H__
#define __AUTOCOMPLETETEXTVIEWMACRO_H__

#include "ext/frameworkext.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::IAdapterViewOnItemSelectedListener;
using Elastos::Droid::Widget::IAutoCompleteTextViewOnDismissListener;
using Elastos::Droid::Widget::IListAdapter;

#define IAUTOCOMPLETETEXTVIEW_METHODS_DECL()                        \
    CARAPI SetCompletionHint(                                       \
        /* [in] */ ICharSequence* hint);                            \
                                                                    \
    CARAPI GetCompletionHint(                                       \
        /* [out] */ ICharSequence** hint);                          \
                                                                    \
    CARAPI GetDropDownWidth(                                        \
        /* [out] */ Int32* width);                                  \
                                                                    \
    CARAPI SetDropDownWidth(                                        \
        /* [in] */ Int32 width);                                    \
                                                                    \
    CARAPI GetDropDownHeight(                                       \
        /* [out] */ Int32* height);                                 \
                                                                    \
    CARAPI SetDropDownHeight(                                       \
        /* [in] */ Int32 height);                                   \
                                                                    \
    CARAPI GetDropDownAnchor(                                       \
        /* [out] */ Int32* id);                                     \
                                                                    \
    CARAPI SetDropDownAnchor(                                       \
        /* [in] */ Int32 id);                                       \
                                                                    \
    CARAPI GetDropDownBackground(                                   \
        /* [out] */ IDrawable** drawable);                          \
                                                                    \
    CARAPI SetDropDownBackgroundDrawable(                           \
        /* [in] */ IDrawable* d);                                   \
                                                                    \
    CARAPI SetDropDownBackgroundResource(                           \
        /* [in] */ Int32 id);                                       \
                                                                    \
    CARAPI SetDropDownVerticalOffset(                               \
        /* [in] */ Int32 offset);                                   \
                                                                    \
    CARAPI GetDropDownVerticalOffset(                               \
        /* [out] */ Int32* offset);                                 \
                                                                    \
    CARAPI SetDropDownHorizontalOffset(                             \
        /* [in] */ Int32 offset);                                   \
                                                                    \
    CARAPI GetDropDownHorizontalOffset(                             \
        /* [out] */ Int32* offset);                                 \
                                                                    \
    CARAPI SetDropDownAnimationStyle(                               \
        /* [in] */ Int32 animationStyle);                           \
                                                                    \
    CARAPI GetDropDownAnimationStyle(                               \
        /* [out] */ Int32* style);                                  \
                                                                    \
    CARAPI IsDropDownAlwaysVisible(                                 \
        /* [out] */ Boolean* visible);                              \
                                                                    \
    CARAPI SetDropDownAlwaysVisible(                                \
        /* [in] */ Boolean dropDownAlwaysVisible);                  \
                                                                    \
    CARAPI IsDropDownDismissedOnCompletion(                         \
        /* [out] */ Boolean* dropDownDismissedOnCompletion);        \
                                                                    \
    CARAPI SetDropDownDismissedOnCompletion(                        \
        /* [in] */ Boolean dropDownDismissedOnCompletion);          \
                                                                    \
    CARAPI GetThreshold(                                            \
        /* [out] */ Int32* threshold);                              \
                                                                    \
    CARAPI SetThreshold(                                            \
        /* [in] */ Int32 threshold);                                \
                                                                    \
    CARAPI SetOnItemClickListener(                                  \
        /* [in] */ IAdapterViewOnItemClickListener* listener);      \
                                                                    \
    CARAPI SetOnItemSelectedListener(                               \
        /* [in] */ IAdapterViewOnItemSelectedListener* listener);   \
                                                                    \
    CARAPI GetOnItemClickListener(                                  \
        /* [out] */ IAdapterViewOnItemClickListener** l);           \
                                                                    \
    CARAPI GetOnItemSelectedListener(                               \
        /* [out] */ IAdapterViewOnItemSelectedListener** l);        \
                                                                    \
    CARAPI GetItemClickListener(                                    \
        /* [out] */ IAdapterViewOnItemClickListener** l);           \
                                                                    \
    CARAPI GetItemSelectedListener(                                 \
        /* [out] */ IAdapterViewOnItemSelectedListener** l);        \
                                                                    \
    CARAPI SetOnDismissListener(                                    \
        /* [in] */ IAutoCompleteTextViewOnDismissListener* l);      \
                                                                    \
    CARAPI GetAdapter(                                              \
        /* [out] */ IListAdapter** adapter);                        \
                                                                    \
    CARAPI SetAdapter(                                              \
        /* [in] */ IListAdapter* adapter);                          \
                                                                    \
    CARAPI EnoughToFilter(                                          \
        /* [out] */ Boolean* result);                               \
                                                                    \
    CARAPI IsPopupShowing(                                          \
        /* [out] */ Boolean* result);                               \
                                                                    \
    CARAPI ClearListSelection();                                    \
                                                                    \
    CARAPI SetListSelection(                                        \
        /* [in] */ Int32 position);                                 \
                                                                    \
    CARAPI GetListSelection(                                        \
        /* [out] */ Int32* selection);                              \
                                                                    \
    CARAPI PerformCompletion();                                     \
                                                                    \
    CARAPI IsPerformingCompletion(                                  \
        /* [out] */ Boolean* result);                               \
                                                                    \
    CARAPI SetTextFilter(                                      \
        /* [in] */ ICharSequence* text,                             \
        /* [in] */ Boolean filter);                                 \
                                                                    \
    CARAPI DismissDropDown();                                       \
                                                                    \
    CARAPI ShowDropDownAfterLayout();                               \
                                                                    \
    CARAPI EnsureImeVisible(                                        \
        /* [in] */ Boolean visible);                                \
                                                                    \
    CARAPI IsInputMethodNotNeeded(                                  \
        /* [out] */ Boolean* needed);                               \
                                                                    \
    CARAPI ShowDropDown();                                          \
                                                                    \
    CARAPI SetForceIgnoreOutsideTouch(                              \
        /* [in] */ Boolean forceIgnoreOutsideTouch);                \
                                                                    \
    CARAPI SetValidator(                                            \
        /* [in] */ IValidator* validator);                          \
                                                                    \
    CARAPI GetValidator(                                            \
        /* [out] */ IValidator** validator);                        \
                                                                    \
    CARAPI PerformValidation();                                     \

#define IAUTOCOMPLETETEXTVIEW_METHODS_IMPL(className, superClass)                                       \
ECode className::SetCompletionHint(                                                                     \
    /* [in] */ ICharSequence* hint)                                                                     \
{                                                                                                       \
    return superClass::SetCompletionHint(hint);                                                         \
}                                                                                                       \
                                                                                                        \
ECode className::GetCompletionHint(                                                                     \
    /* [out] */ ICharSequence** hint)                                                                   \
{                                                                                                       \
    VALIDATE_NOT_NULL(hint);                                                                            \
    AutoPtr<ICharSequence> csq = superClass::GetCompletionHint();                                       \
    *hint = csq;                                                                                        \
    INTERFACE_ADDREF(*hint);                                                                            \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::GetDropDownWidth(                                                                      \
    /* [out] */ Int32* width)                                                                           \
{                                                                                                       \
    VALIDATE_NOT_NULL(width);                                                                           \
    *width = superClass::GetDropDownWidth();                                                            \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::SetDropDownWidth(                                                                      \
    /* [in] */ Int32 width)                                                                             \
{                                                                                                       \
    return superClass::SetDropDownWidth(width);                                                         \
}                                                                                                       \
                                                                                                        \
ECode className::GetDropDownHeight(                                                                     \
    /* [out] */ Int32* height)                                                                          \
{                                                                                                       \
    VALIDATE_NOT_NULL(height);                                                                          \
    *height = superClass::GetDropDownHeight();                                                          \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::SetDropDownHeight(                                                                     \
    /* [in] */ Int32 height)                                                                            \
{                                                                                                       \
    return superClass::SetDropDownHeight(height);                                                       \
}                                                                                                       \
                                                                                                        \
ECode className::GetDropDownAnchor(                                                                     \
    /* [out] */ Int32* id)                                                                              \
{                                                                                                       \
    VALIDATE_NOT_NULL(id);                                                                              \
    *id = superClass::GetDropDownAnchor();                                                              \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::SetDropDownAnchor(                                                                     \
    /* [in] */ Int32 id)                                                                                \
{                                                                                                       \
    return superClass::SetDropDownAnchor(id);                                                           \
}                                                                                                       \
                                                                                                        \
ECode className::GetDropDownBackground(                                                                 \
    /* [out] */ IDrawable** drawable)                                                                   \
{                                                                                                       \
    VALIDATE_NOT_NULL(drawable);                                                                        \
    AutoPtr<IDrawable> d = superClass::GetDropDownBackground();                                         \
    *drawable = d;                                                                                      \
    INTERFACE_ADDREF(*drawable);                                                                        \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::SetDropDownBackgroundDrawable(                                                         \
    /* [in] */ IDrawable* d)                                                                            \
{                                                                                                       \
    return superClass::SetDropDownBackgroundDrawable(d);                                                \
}                                                                                                       \
                                                                                                        \
ECode className::SetDropDownBackgroundResource(                                                         \
    /* [in] */ Int32 id)                                                                                \
{                                                                                                       \
    return superClass::SetDropDownBackgroundResource(id);                                               \
}                                                                                                       \
                                                                                                        \
ECode className::SetDropDownVerticalOffset(                                                             \
    /* [in] */ Int32 offset)                                                                            \
{                                                                                                       \
    return superClass::SetDropDownVerticalOffset(offset);                                               \
}                                                                                                       \
                                                                                                        \
ECode className::GetDropDownVerticalOffset(                                                             \
    /* [out] */ Int32* offset)                                                                          \
{                                                                                                       \
    VALIDATE_NOT_NULL(offset);                                                                          \
    *offset = superClass::GetDropDownVerticalOffset();                                                  \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::SetDropDownHorizontalOffset(                                                           \
    /* [in] */ Int32 offset)                                                                            \
{                                                                                                       \
    return superClass::SetDropDownHorizontalOffset(offset);                                             \
}                                                                                                       \
                                                                                                        \
ECode className::GetDropDownHorizontalOffset(                                                           \
    /* [out] */ Int32* offset)                                                                          \
{                                                                                                       \
    VALIDATE_NOT_NULL(offset);                                                                          \
    *offset = superClass::GetDropDownHorizontalOffset();                                                \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::SetDropDownAnimationStyle(                                                             \
    /* [in] */ Int32 animationStyle)                                                                    \
{                                                                                                       \
    return superClass::SetDropDownAnimationStyle(animationStyle);                                       \
}                                                                                                       \
                                                                                                        \
ECode className::GetDropDownAnimationStyle(                                                             \
    /* [out] */ Int32* style)                                                                           \
{                                                                                                       \
    VALIDATE_NOT_NULL(style);                                                                           \
    *style = superClass::GetDropDownAnimationStyle();                                                   \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::IsDropDownAlwaysVisible(                                                               \
    /* [out] */ Boolean* visible)                                                                       \
{                                                                                                       \
    VALIDATE_NOT_NULL(visible);                                                                         \
    *visible = superClass::IsDropDownAlwaysVisible();                                                   \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::SetDropDownAlwaysVisible(                                                              \
    /* [in] */ Boolean dropDownAlwaysVisible)                                                           \
{                                                                                                       \
    return superClass::SetDropDownAlwaysVisible(dropDownAlwaysVisible);                                 \
}                                                                                                       \
                                                                                                        \
ECode className::IsDropDownDismissedOnCompletion(                                                       \
    /* [out] */ Boolean* dropDownDismissedOnCompletion)                                                 \
{                                                                                                       \
    VALIDATE_NOT_NULL(dropDownDismissedOnCompletion);                                                   \
    *dropDownDismissedOnCompletion = superClass::IsDropDownDismissedOnCompletion();                     \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::SetDropDownDismissedOnCompletion(                                                      \
    /* [in] */ Boolean dropDownDismissedOnCompletion)                                                   \
{                                                                                                       \
    return superClass::SetDropDownDismissedOnCompletion(dropDownDismissedOnCompletion);                 \
}                                                                                                       \
                                                                                                        \
ECode className::GetThreshold(                                                                          \
    /* [out] */ Int32* threshold)                                                                       \
{                                                                                                       \
    VALIDATE_NOT_NULL(threshold);                                                                       \
    *threshold = superClass::GetThreshold();                                                            \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::SetThreshold(                                                                          \
    /* [in] */ Int32 threshold)                                                                         \
{                                                                                                       \
    return superClass::SetThreshold(threshold);                                                         \
}                                                                                                       \
                                                                                                        \
ECode className::SetOnItemClickListener(                                                                \
    /* [in] */ IAdapterViewOnItemClickListener* listener)                                               \
{                                                                                                       \
    return superClass::SetOnItemClickListener(listener);                                                \
}                                                                                                       \
                                                                                                        \
ECode className::SetOnItemSelectedListener(                                                             \
    /* [in] */ IAdapterViewOnItemSelectedListener* listener)                                            \
{                                                                                                       \
    return superClass::SetOnItemSelectedListener(listener);                                             \
}                                                                                                       \
                                                                                                        \
ECode className::GetOnItemClickListener(                                                                \
    /* [out] */ IAdapterViewOnItemClickListener** l)                                                    \
{                                                                                                       \
    VALIDATE_NOT_NULL(l);                                                                               \
    AutoPtr<IAdapterViewOnItemClickListener> listener = superClass::GetOnItemClickListener();           \
    *l = listener;                                                                                      \
    INTERFACE_ADDREF(*l);                                                                               \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::GetOnItemSelectedListener(                                                             \
    /* [out] */ IAdapterViewOnItemSelectedListener** l)                                                 \
{                                                                                                       \
    VALIDATE_NOT_NULL(l);                                                                               \
    AutoPtr<IAdapterViewOnItemSelectedListener> listener = superClass::GetOnItemSelectedListener();     \
    *l = listener;                                                                                      \
    INTERFACE_ADDREF(*l);                                                                               \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::GetItemClickListener(                                                                  \
    /* [out] */ IAdapterViewOnItemClickListener** l)                                                    \
{                                                                                                       \
    VALIDATE_NOT_NULL(l);                                                                               \
    AutoPtr<IAdapterViewOnItemClickListener> listener = superClass::GetOnItemClickListener();           \
    *l = listener;                                                                                      \
    INTERFACE_ADDREF(*l);                                                                               \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::GetItemSelectedListener(                                                               \
    /* [out] */ IAdapterViewOnItemSelectedListener** l)                                                 \
{                                                                                                       \
    VALIDATE_NOT_NULL(l);                                                                               \
    AutoPtr<IAdapterViewOnItemSelectedListener> listener = superClass::GetOnItemSelectedListener();     \
    *l = listener;                                                                                      \
    INTERFACE_ADDREF(*l);                                                                               \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::SetOnDismissListener(                                                                  \
    /* [in] */ IAutoCompleteTextViewOnDismissListener* l)                                               \
{                                                                                                       \
    return superClass::SetOnDismissListener(l);                                                         \
}                                                                                                       \
                                                                                                        \
ECode className::GetAdapter(                                                                            \
    /* [out] */ IListAdapter** adapter)                                                                 \
{                                                                                                       \
    VALIDATE_NOT_NULL(adapter);                                                                         \
    AutoPtr<IListAdapter> ad = superClass::GetAdapter();                                                \
    *adapter = ad;                                                                                      \
    INTERFACE_ADDREF(*adapter);                                                                         \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::SetAdapter(                                                                            \
    /* [in] */ IListAdapter* adapter)                                                                   \
{                                                                                                       \
    return superClass::SetAdapter(adapter);                                                             \
}                                                                                                       \
                                                                                                        \
ECode className::EnoughToFilter(                                                                        \
    /* [out] */ Boolean* result)                                                                        \
{                                                                                                       \
    VALIDATE_NOT_NULL(result);                                                                          \
    *result = superClass::EnoughToFilter();                                                             \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::IsPopupShowing(                                                                        \
    /* [out] */ Boolean* result)                                                                        \
{                                                                                                       \
    VALIDATE_NOT_NULL(result);                                                                          \
    *result = superClass::IsPopupShowing();                                                             \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::ClearListSelection()                                                                   \
{                                                                                                       \
    return superClass::ClearListSelection();                                                            \
}                                                                                                       \
                                                                                                        \
ECode className::SetListSelection(                                                                      \
    /* [in] */ Int32 position)                                                                          \
{                                                                                                       \
    return superClass::SetListSelection(position);                                                      \
}                                                                                                       \
                                                                                                        \
ECode className::GetListSelection(                                                                      \
    /* [out] */ Int32* selection)                                                                       \
{                                                                                                       \
    VALIDATE_NOT_NULL(selection);                                                                       \
    *selection = superClass::GetListSelection();                                                        \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::PerformCompletion()                                                                    \
{                                                                                                       \
    return superClass::PerformCompletion();                                                             \
}                                                                                                       \
                                                                                                        \
ECode className::IsPerformingCompletion(                                                                \
    /* [out] */ Boolean* result)                                                                        \
{                                                                                                       \
    VALIDATE_NOT_NULL(result);                                                                          \
    *result = superClass::IsPerformingCompletion();                                                     \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::SetTextFilter(                                                                    \
    /* [in] */ ICharSequence* text,                                                                     \
    /* [in] */ Boolean filter)                                                                          \
{                                                                                                       \
    return superClass::SetText(text, filter);                                                           \
}                                                                                                       \
                                                                                                        \
ECode className::DismissDropDown()                                                                      \
{                                                                                                       \
    return superClass::DismissDropDown();                                                               \
}                                                                                                       \
                                                                                                        \
ECode className::ShowDropDownAfterLayout()                                                              \
{                                                                                                       \
    return superClass::ShowDropDownAfterLayout();                                                       \
}                                                                                                       \
                                                                                                        \
ECode className::EnsureImeVisible(                                                                      \
    /* [in] */ Boolean visible)                                                                         \
{                                                                                                       \
    return superClass::EnsureImeVisible(visible);                                                       \
}                                                                                                       \
                                                                                                        \
ECode className::IsInputMethodNotNeeded(                                                                \
    /* [out] */ Boolean* needed)                                                                        \
{                                                                                                       \
    VALIDATE_NOT_NULL(needed);                                                                          \
    *needed = superClass::IsInputMethodNotNeeded();                                                     \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::ShowDropDown()                                                                         \
{                                                                                                       \
    return superClass::ShowDropDown();                                                                  \
}                                                                                                       \
                                                                                                        \
ECode className::SetForceIgnoreOutsideTouch(                                                            \
    /* [in] */ Boolean forceIgnoreOutsideTouch)                                                         \
{                                                                                                       \
    return superClass::SetForceIgnoreOutsideTouch(forceIgnoreOutsideTouch);                             \
}                                                                                                       \
                                                                                                        \
ECode className::SetValidator(                                                                          \
    /* [in] */ IValidator* validator)                                                                   \
{                                                                                                       \
    return superClass::SetValidator(validator);                                                         \
}                                                                                                       \
                                                                                                        \
ECode className::GetValidator(                                                                          \
    /* [out] */ IValidator** validator)                                                                 \
{                                                                                                       \
    VALIDATE_NOT_NULL(validator);                                                                       \
    AutoPtr<IValidator> ad = superClass::GetValidator();                                                \
    *validator = ad;                                                                                    \
    INTERFACE_ADDREF(*validator);                                                                       \
    return NOERROR;                                                                                     \
}                                                                                                       \
                                                                                                        \
ECode className::PerformValidation()                                                                    \
{                                                                                                       \
    return superClass::PerformValidation();                                                             \
}                                                                                                       \

#endif //__AUTOCOMPLETETEXTVIEWMACRO_H__