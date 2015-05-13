#ifndef __POPUPWINDOWMACRO_H__
#define __POPUPWINDOWMACRO_H__

#include "widget/PopupWindow.h"

using Elastos::Droid::Widget::PopupWindow;
using Elastos::Droid::Widget::IPopupWindow;

#define IPOPUPWINDOW_METHODS_DECL()                                         \
    CARAPI GetBackground(                                                   \
        /* [out] */ IDrawable** background);                                \
                                                                            \
    CARAPI SetBackgroundDrawable(                                           \
        /* [in] */ IDrawable* background);                                  \
                                                                            \
    CARAPI GetAnimationStyle(                                               \
        /* [out] */ Int32* style);                                          \
                                                                            \
    CARAPI SetIgnoreCheekPress();                                           \
                                                                            \
    CARAPI SetAnimationStyle(                                               \
        /* [in] */ Int32 animationStyle);                                   \
                                                                            \
    CARAPI GetContentView(                                                  \
        /* [out] */ IView** contentView);                                   \
                                                                            \
    CARAPI SetContentView(                                                  \
        /* [in] */ IView* contentView);                                     \
                                                                            \
    CARAPI SetTouchInterceptor(                                             \
        /* [in] */ IViewOnTouchListener* l);                                \
                                                                            \
    CARAPI IsFocusable(                                                     \
        /* [out] */ Boolean* isFocusable);                                  \
                                                                            \
    CARAPI  SetFocusable(                                                   \
         /* [in] */ Boolean focusable);                                     \
                                                                            \
    CARAPI GetInputMethodMode(                                              \
        /* [out] */ Int32* inputMethodMode);                                \
                                                                            \
    CARAPI SetInputMethodMode(                                              \
        /* [in] */ Int32 mode);                                             \
                                                                            \
    CARAPI SetSoftInputMode(                                                \
        /* [in] */ Int32 mode);                                             \
                                                                            \
    CARAPI GetSoftInputMode(                                                \
        /* [out] */ Int32* softInputMode);                                  \
                                                                            \
    CARAPI IsTouchable(                                                     \
        /* [out] */ Boolean* touchable);                                    \
                                                                            \
    CARAPI SetTouchable(                                                    \
        /* [in] */ Boolean touchable);                                      \
                                                                            \
    CARAPI IsOutsideTouchable(                                              \
        /* [out] */ Boolean* touchable);                                    \
                                                                            \
    CARAPI SetOutsideTouchable(                                             \
        /* [in] */ Boolean touchable);                                      \
                                                                            \
    CARAPI IsClippingEnabled(                                               \
        /* [out] */ Boolean* enabled);                                      \
                                                                            \
    CARAPI SetClippingEnabled(                                              \
        /* [in] */ Boolean enabled);                                        \
                                                                            \
    CARAPI SetClipToScreenEnabled(                                          \
        /* [in] */ Boolean enabled);                                        \
                                                                            \
    CARAPI IsSplitTouchEnabled(                                             \
        /* [out] */ Boolean* enabled);                                      \
                                                                            \
    CARAPI SetSplitTouchEnabled(                                            \
        /* [in] */ Boolean enabled);                                        \
                                                                            \
    CARAPI IsLayoutInScreenEnabled(                                         \
        /* [out] */ Boolean* enabled);                                      \
                                                                            \
    CARAPI SetLayoutInScreenEnabled(                                        \
        /* [in] */ Boolean enabled);                                        \
                                                                            \
    CARAPI SetLayoutInsetDecor(                                             \
        /* [in] */ Boolean enabled);                                        \
                                                                            \
    CARAPI SetWindowLayoutType(                                             \
        /* [in] */ Int32 layoutType);                                       \
                                                                            \
    CARAPI GetWindowLayoutType(                                             \
        /* [out] */ Int32* layoutType);                                     \
                                                                            \
    CARAPI SetTouchModal(                                                   \
        /* [in] */ Boolean touchModal);                                     \
                                                                            \
    CARAPI SetWindowLayoutMode(                                             \
        /* [in] */ Int32 widthSpec,                                         \
        /* [in] */ Int32 heightSpec);                                       \
                                                                            \
    CARAPI GetHeight(                                                       \
        /* [out] */ Int32* height);                                         \
                                                                            \
    CARAPI SetHeight(                                                       \
        /* [in] */ Int32 height);                                           \
                                                                            \
    CARAPI GetWidth(                                                        \
        /* [out] */ Int32* width);                                          \
                                                                            \
    CARAPI SetWidth(                                                        \
        /* [in] */ Int32 width);                                            \
                                                                            \
    CARAPI IsShowing(                                                       \
        /* [out] */ Boolean* isShowing);                                    \
                                                                            \
    CARAPI ShowAtLocation(                                                  \
        /* [in] */ IView* parent,                                           \
        /* [in] */ Int32 gravity,                                           \
        /* [in] */ Int32 x,                                                 \
        /* [in] */ Int32 y);                                                \
                                                                            \
    CARAPI ShowAtLocationEx(                                                \
        /* [in] */ IBinder* token,                                          \
        /* [in] */ Int32 gravity,                                           \
        /* [in] */ Int32 x,                                                 \
        /* [in] */ Int32 y);                                                \
                                                                            \
    CARAPI ShowAsDropDown(                                                  \
        /* [in] */ IView* anchor);                                          \
                                                                            \
    CARAPI ShowAsDropDownEx(                                                \
        /* [in] */ IView* anchor,                                           \
        /* [in] */ Int32 xoff,                                              \
        /* [in] */ Int32 yoff);                                             \
                                                                            \
    CARAPI IsAboveAnchor(                                                   \
        /* [out] */ Boolean* isAboveAnchor);                                \
                                                                            \
    CARAPI GetMaxAvailableHeight(                                           \
        /* [in] */ IView* anchor,                                           \
        /* [out] */ Int32* maxAvailableHeight);                             \
                                                                            \
    CARAPI GetMaxAvailableHeightEx(                                         \
        /* [in] */ IView* anchor,                                           \
        /* [in] */ Int32 yOffset,                                           \
        /* [out] */ Int32* maxAvailableHeight);                             \
                                                                            \
    CARAPI GetMaxAvailableHeightEx2(                                        \
        /* [in] */ IView* anchor,                                           \
        /* [in] */ Int32 yOffset,                                           \
        /* [in] */ Boolean ignoreBottomDecorations,                         \
        /* [out] */ Int32* maxAvailableHeight);                             \
                                                                            \
    CARAPI Dismiss();                                                       \
                                                                            \
    CARAPI SetOnDismissListener(                                            \
        /* [in] */ IPopupWindowOnDismissListener* l);                       \
                                                                            \
    CARAPI Update();                                                        \
                                                                            \
    CARAPI UpdateEx(                                                        \
        /* [in] */ Int32 width,                                             \
        /* [in] */ Int32 height);                                           \
                                                                            \
    CARAPI UpdateEx2(                                                       \
        /* [in] */ Int32 x,                                                 \
        /* [in] */ Int32 y,                                                 \
        /* [in] */ Int32 width,                                             \
        /* [in] */ Int32 height);                                           \
                                                                            \
    CARAPI UpdateEx3(                                                       \
        /* [in] */ Int32 x,                                                 \
        /* [in] */ Int32 y,                                                 \
        /* [in] */ Int32 width,                                             \
        /* [in] */ Int32 height,                                            \
        /* [in] */ Boolean force);                                          \
                                                                            \
    CARAPI UpdateEx4(                                                       \
        /* [in] */ IView* anchor,                                           \
        /* [in] */ Int32 width,                                             \
        /* [in] */ Int32 height);                                           \
                                                                            \
    CARAPI UpdateEx5(                                                       \
        /* [in] */ IView* anchor,                                           \
        /* [in] */ Int32 xoff,                                              \
        /* [in] */ Int32 yoff,                                              \
        /* [in] */ Int32 width,                                             \
        /* [in] */ Int32 height);                                           \
                                                                            \
    CARAPI SetAllowScrollingAnchorParent(                                   \
        /* [in] */ Boolean enabled);


#define IPOPUPWINDOW_METHODS_IMPL(className, superClass)                    \
ECode className::GetBackground(                                             \
    /* [out] */ IDrawable** background)                                     \
{                                                                           \
    VALIDATE_NOT_NULL(background);                                          \
    AutoPtr<IDrawable> bk = superClass::GetBackground();                    \
    *background = bk.Get();                                                 \
    INTERFACE_ADDREF(*background);                                          \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetBackgroundDrawable(                                     \
    /* [in] */ IDrawable* background)                                       \
{                                                                           \
    return superClass::SetBackgroundDrawable(background);                   \
}                                                                           \
                                                                            \
ECode className::GetAnimationStyle(                                         \
    /* [out] */ Int32* style)                                               \
{                                                                           \
    VALIDATE_NOT_NULL(style);                                               \
    *style = superClass::GetAnimationStyle();                               \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetIgnoreCheekPress()                                      \
{                                                                           \
    return superClass::SetIgnoreCheekPress();                               \
}                                                                           \
                                                                            \
ECode className::SetAnimationStyle(                                         \
    /* [in] */ Int32 animationStyle)                                        \
{                                                                           \
    return superClass::SetAnimationStyle(animationStyle);                   \
}                                                                           \
                                                                            \
ECode className::GetContentView(                                            \
    /* [out] */ IView** contentView)                                        \
{                                                                           \
    VALIDATE_NOT_NULL(contentView);                                         \
    AutoPtr<IView> cv = superClass::GetContentView();                       \
    *contentView = cv.Get();                                                \
    INTERFACE_ADDREF(*contentView);                                         \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetContentView(                                            \
    /* [in] */ IView* contentView)                                          \
{                                                                           \
    return superClass::SetContentView(contentView);                         \
}                                                                           \
                                                                            \
ECode className::SetTouchInterceptor(                                       \
    /* [in] */ IViewOnTouchListener* l)                                     \
{                                                                           \
    return superClass::SetTouchInterceptor(l);                              \
}                                                                           \
                                                                            \
ECode className::IsFocusable(                                               \
    /* [out] */ Boolean* isFocusable)                                       \
{                                                                           \
    VALIDATE_NOT_NULL(isFocusable);                                         \
    *isFocusable = superClass::IsFocusable();                               \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetFocusable(                                              \
     /* [in] */ Boolean focusable)                                          \
{                                                                           \
    return superClass::SetFocusable(focusable);                             \
}                                                                           \
                                                                            \
ECode className::GetInputMethodMode(                                        \
    /* [out] */ Int32* inputMethodMode)                                     \
{                                                                           \
    VALIDATE_NOT_NULL(inputMethodMode);                                     \
    *inputMethodMode = superClass::GetInputMethodMode();                    \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetInputMethodMode(                                        \
    /* [in] */ Int32 mode)                                                  \
{                                                                           \
    return superClass::SetInputMethodMode(mode);                            \
}                                                                           \
                                                                            \
ECode className::SetSoftInputMode(                                          \
    /* [in] */ Int32 mode)                                                  \
{                                                                           \
    return superClass::SetSoftInputMode(mode);                              \
}                                                                           \
                                                                            \
ECode className::GetSoftInputMode(                                          \
    /* [out] */ Int32* softInputMode)                                       \
{                                                                           \
    VALIDATE_NOT_NULL(softInputMode);                                       \
    *softInputMode = superClass::GetSoftInputMode();                        \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::IsTouchable(                                               \
    /* [out] */ Boolean* touchable)                                         \
{                                                                           \
    VALIDATE_NOT_NULL(touchable);                                           \
    *touchable = superClass::IsTouchable();                                 \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetTouchable(                                              \
    /* [in] */ Boolean touchable)                                           \
{                                                                           \
    return superClass::SetTouchable(touchable);                             \
}                                                                           \
                                                                            \
ECode className::IsOutsideTouchable(                                        \
    /* [out] */ Boolean* touchable)                                         \
{                                                                           \
    VALIDATE_NOT_NULL(touchable);                                           \
    *touchable = superClass::IsOutsideTouchable();                          \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetOutsideTouchable(                                       \
    /* [in] */ Boolean touchable)                                           \
{                                                                           \
    return superClass::SetOutsideTouchable(touchable);                      \
}                                                                           \
                                                                            \
ECode className::IsClippingEnabled(                                         \
    /* [out] */ Boolean* enabled)                                           \
{                                                                           \
    VALIDATE_NOT_NULL(enabled);                                             \
    *enabled = superClass::IsClippingEnabled();                             \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetClippingEnabled(                                        \
    /* [in] */ Boolean enabled)                                             \
{                                                                           \
    return superClass::SetClippingEnabled(enabled);                         \
}                                                                           \
                                                                            \
ECode className::SetClipToScreenEnabled(                                    \
    /* [in] */ Boolean enabled)                                             \
{                                                                           \
    return superClass::SetClipToScreenEnabled(enabled);                     \
}                                                                           \
                                                                            \
ECode className::IsSplitTouchEnabled(                                       \
    /* [out] */ Boolean* enabled)                                           \
{                                                                           \
    VALIDATE_NOT_NULL(enabled);                                             \
    *enabled = superClass::IsSplitTouchEnabled();                           \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetSplitTouchEnabled(                                      \
    /* [in] */ Boolean enabled)                                             \
{                                                                           \
    return superClass::SetSplitTouchEnabled(enabled);                       \
}                                                                           \
                                                                            \
ECode className::IsLayoutInScreenEnabled(                                   \
    /* [out] */ Boolean* enabled)                                           \
{                                                                           \
    VALIDATE_NOT_NULL(enabled);                                             \
    *enabled = superClass::IsLayoutInScreenEnabled();                       \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetLayoutInScreenEnabled(                                  \
    /* [in] */ Boolean enabled)                                             \
{                                                                           \
    return superClass::SetLayoutInScreenEnabled(enabled);                   \
}                                                                           \
                                                                            \
ECode className::SetLayoutInsetDecor(                                       \
    /* [in] */ Boolean enabled)                                             \
{                                                                           \
    return superClass::SetLayoutInsetDecor(enabled);                        \
}                                                                           \
                                                                            \
ECode className::SetWindowLayoutType(                                       \
    /* [in] */ Int32 layoutType)                                            \
{                                                                           \
    return superClass::SetWindowLayoutType(layoutType);                     \
}                                                                           \
                                                                            \
ECode className::GetWindowLayoutType(                                       \
    /* [out] */ Int32* layoutType)                                          \
{                                                                           \
    VALIDATE_NOT_NULL(layoutType);                                          \
    *layoutType = superClass::GetWindowLayoutType();                        \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetTouchModal(                                             \
    /* [in] */ Boolean touchModal)                                          \
{                                                                           \
    return superClass::SetTouchModal(touchModal);                           \
}                                                                           \
                                                                            \
ECode className::SetWindowLayoutMode(                                       \
    /* [in] */ Int32 widthSpec,                                             \
    /* [in] */ Int32 heightSpec)                                            \
{                                                                           \
    return superClass::SetWindowLayoutMode(widthSpec, heightSpec);          \
}                                                                           \
                                                                            \
ECode className::GetHeight(                                                 \
    /* [out] */ Int32* height)                                              \
{                                                                           \
    VALIDATE_NOT_NULL(height);                                              \
    *height = superClass::GetHeight();                                      \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetHeight(                                                 \
    /* [in] */ Int32 height)                                                \
{                                                                           \
    return superClass::SetHeight(height);                                   \
}                                                                           \
                                                                            \
ECode className::GetWidth(                                                  \
    /* [out] */ Int32* width)                                               \
{                                                                           \
    VALIDATE_NOT_NULL(width);                                               \
    *width = superClass::GetWidth();                                        \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetWidth(                                                  \
    /* [in] */ Int32 width)                                                 \
{                                                                           \
    return superClass::SetWidth(width);                                     \
}                                                                           \
                                                                            \
ECode className::IsShowing(                                                 \
    /* [out] */ Boolean* isShowing)                                         \
{                                                                           \
    VALIDATE_NOT_NULL(isShowing);                                           \
    *isShowing = superClass::IsShowing();                                   \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::ShowAtLocation(                                            \
    /* [in] */ IView* parent,                                               \
    /* [in] */ Int32 gravity,                                               \
    /* [in] */ Int32 x,                                                     \
    /* [in] */ Int32 y)                                                     \
{                                                                           \
    return superClass::ShowAtLocation(parent, gravity, x, y);               \
}                                                                           \
                                                                            \
ECode className::ShowAtLocationEx(                                          \
    /* [in] */ IBinder* token,                                              \
    /* [in] */ Int32 gravity,                                               \
    /* [in] */ Int32 x,                                                     \
    /* [in] */ Int32 y)                                                     \
{                                                                           \
    return superClass::ShowAtLocation(token, gravity, x, y);                \
}                                                                           \
                                                                            \
ECode className::ShowAsDropDown(                                            \
    /* [in] */ IView* anchor)                                               \
{                                                                           \
    return superClass::ShowAsDropDown(anchor);                              \
}                                                                           \
                                                                            \
ECode className::ShowAsDropDownEx(                                          \
    /* [in] */ IView* anchor,                                               \
    /* [in] */ Int32 xoff,                                                  \
    /* [in] */ Int32 yoff)                                                  \
{                                                                           \
    return superClass::ShowAsDropDown(anchor, xoff, yoff);                  \
}                                                                           \
                                                                            \
ECode className::IsAboveAnchor(                                             \
    /* [out] */ Boolean* isAboveAnchor)                                     \
{                                                                           \
    VALIDATE_NOT_NULL(isAboveAnchor);                                       \
    *isAboveAnchor = superClass::IsAboveAnchor();                           \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::GetMaxAvailableHeight(                                     \
    /* [in] */ IView* anchor,                                               \
    /* [out] */ Int32* maxAvailableHeight)                                  \
{                                                                           \
    VALIDATE_NOT_NULL(maxAvailableHeight);                                  \
    *maxAvailableHeight = superClass::GetMaxAvailableHeight(anchor);        \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::GetMaxAvailableHeightEx(                                   \
    /* [in] */ IView* anchor,                                               \
    /* [in] */ Int32 yOffset,                                               \
    /* [out] */ Int32* maxAvailableHeight)                                  \
{                                                                           \
    VALIDATE_NOT_NULL(maxAvailableHeight);                                  \
    *maxAvailableHeight = superClass::GetMaxAvailableHeight(anchor, yOffset);   \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::GetMaxAvailableHeightEx2(                                  \
    /* [in] */ IView* anchor,                                               \
    /* [in] */ Int32 yOffset,                                               \
    /* [in] */ Boolean ignoreBottomDecorations,                             \
    /* [out] */ Int32* maxAvailableHeight)                                  \
{                                                                           \
    VALIDATE_NOT_NULL(maxAvailableHeight);                                  \
    *maxAvailableHeight = superClass::GetMaxAvailableHeight(anchor, yOffset, ignoreBottomDecorations); \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::Dismiss()                                                  \
{                                                                           \
    return superClass::Dismiss();                                           \
}                                                                           \
                                                                            \
ECode className::SetOnDismissListener(                                      \
    /* [in] */ IPopupWindowOnDismissListener* l)                            \
{                                                                           \
    return superClass::SetOnDismissListener(l);                             \
}                                                                           \
                                                                            \
ECode className::Update()                                                   \
{                                                                           \
    return superClass::Update();                                            \
}                                                                           \
                                                                            \
ECode className::UpdateEx(                                                  \
    /* [in] */ Int32 width,                                                 \
    /* [in] */ Int32 height)                                                \
{                                                                           \
    return superClass::Update(width, height);                               \
}                                                                           \
                                                                            \
ECode className::UpdateEx2(                                                 \
    /* [in] */ Int32 x,                                                     \
    /* [in] */ Int32 y,                                                     \
    /* [in] */ Int32 width,                                                 \
    /* [in] */ Int32 height)                                                \
{                                                                           \
    return superClass::Update(x, y, width, height);                         \
}                                                                           \
                                                                            \
ECode className::UpdateEx3(                                                 \
    /* [in] */ Int32 x,                                                     \
    /* [in] */ Int32 y,                                                     \
    /* [in] */ Int32 width,                                                 \
    /* [in] */ Int32 height,                                                \
    /* [in] */ Boolean force)                                               \
{                                                                           \
    return superClass::Update(x, y, width, height, force);                  \
}                                                                           \
                                                                            \
ECode className::UpdateEx4(                                                 \
    /* [in] */ IView* anchor,                                               \
    /* [in] */ Int32 width,                                                 \
    /* [in] */ Int32 height)                                                \
{                                                                           \
    return superClass::Update(anchor, width, height);                       \
}                                                                           \
                                                                            \
ECode className::UpdateEx5(                                                 \
    /* [in] */ IView* anchor,                                               \
    /* [in] */ Int32 xoff,                                                  \
    /* [in] */ Int32 yoff,                                                  \
    /* [in] */ Int32 width,                                                 \
    /* [in] */ Int32 height)                                                \
{                                                                           \
    return superClass::Update(anchor, xoff, yoff, width, height);           \
}                                                                           \
                                                                            \
ECode className::SetAllowScrollingAnchorParent(                             \
        /* [in] */ Boolean enabled)                                         \
{                                                                           \
    return superClass::SetAllowScrollingAnchorParent(enabled);              \
}

#endif // __POPUPWINDOWMACRO_H__
