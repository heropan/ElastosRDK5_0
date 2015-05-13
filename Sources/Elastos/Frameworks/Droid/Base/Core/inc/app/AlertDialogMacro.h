
#ifndef __ALERTDIALOGMACRO_H__
#define __ALERTDIALOGMACRO_H__

#include "app/AlertDialog.h"
#include "app/DialogMacro.h"

#define IALERTDIALOG_METHODS_DECL()                                       \
    GetButton(                                                            \
        /* [in] */ Int32 whichButton,                                     \
        /* [out] */ IButton** button);                                    \
                                                                          \
    GetListView(                                                          \
        /* [out] */ IListView** listView);                                \
                                                                          \
    SetCustomTitle(                                                       \
        /* [in] */ IView* customTitleView);                               \
                                                                          \
    SetMessage(                                                           \
        /* [in] */ ICharSequence* message);                               \
                                                                          \
    SetView(                                                              \
        /* [in] */ IView* view);                                          \
                                                                          \
    SetViewEx(                                                            \
        /* [in] */ IView* view,                                           \
        /* [in] */ Int32 viewSpacingLeft,                                 \
        /* [in] */ Int32 viewSpacingTop,                                  \
        /* [in] */ Int32 viewSpacingRight,                                \
        /* [in] */ Int32 viewSpacingBottom);                              \
                                                                          \
    SetButton(                                                            \
        /* [in] */ Int32 whichButton,                                     \
        /* [in] */ ICharSequence* text,                                   \
        /* [in] */ IMessage* msg);                                        \
                                                                          \
    SetButtonEx(                                                          \
        /* [in] */ Int32 whichButton,                                     \
        /* [in] */ ICharSequence* text,                                   \
        /* [in] */ IDialogInterfaceOnClickListener* listener);            \
                                                                          \
    SetIcon(                                                              \
        /* [in] */ Int32 resId);                                          \
                                                                          \
    SetIconEx(                                                            \
        /* [in] */ IDrawable* icon);                                      \
                                                                          \
    SetIconAttribute(                                                     \
        /* [in] */ Int32 attrId);                                         \
                                                                          \
    SetInverseBackgroundForced(                                           \
        /* [in] */ Boolean forceInverseBackground);


#define IALERTDIALOG_METHODS_IMPL(className, superClass)                  \
    className::GetButton(                                                 \
        /* [in] */ Int32 whichButton,                                     \
        /* [out] */ IButton** button)                                     \
    {                                                                     \
        VALIDATE_NOT_NULL(button);                                        \
        AutoPtr<IButton> b = superClass::GetButton(whichButton);          \
        *button = b.Get();                                                \
        INTERFACE_ADDREF(*button);                                        \
        return NOERROR;                                                   \
    }                                                                     \
                                                                          \
    className::GetListView(                                               \
        /* [out] */ IListView** listView)                                 \
    {                                                                     \
        VALIDATE_NOT_NULL(listView);                                      \
        AutoPtr<IListView> l = superClass::GetListView();                 \
        *listView = l.Get();                                              \
        INTERFACE_ADDREF(*listView);                                      \
        return NOERROR;                                                   \
    }                                                                     \
                                                                          \
    className::SetCustomTitle(                                            \
        /* [in] */ IView* customTitleView)                                \
    {                                                                     \
        return superClass::SetCustomTitle(customTitleView);               \
    }                                                                     \
                                                                          \
    className::SetMessage(                                                \
        /* [in] */ ICharSequence* message)                                \
    {                                                                     \
        return superClass::SetMessage(message);                           \
    }                                                                     \
                                                                          \
    className::SetView(                                                   \
        /* [in] */ IView* view)                                           \
    {                                                                     \
        return superClass::SetView(view);                                 \
    }                                                                     \
                                                                          \
    className::SetViewEx(                                                 \
        /* [in] */ IView* view,                                           \
        /* [in] */ Int32 viewSpacingLeft,                                 \
        /* [in] */ Int32 viewSpacingTop,                                  \
        /* [in] */ Int32 viewSpacingRight,                                \
        /* [in] */ Int32 viewSpacingBottom)                               \
    {                                                                     \
        return superClass::SetView(view, viewSpacingLeft, viewSpacingTop, viewSpacingRight, viewSpacingBottom); \
    }                                                                     \
                                                                          \
    className::SetButton(                                                 \
        /* [in] */ Int32 whichButton,                                     \
        /* [in] */ ICharSequence* text,                                   \
        /* [in] */ IMessage* msg)                                         \
    {                                                                     \
        return superClass::SetButton(whichButton, text, msg);             \
    }                                                                     \
                                                                          \
    className::SetButtonEx(                                               \
        /* [in] */ Int32 whichButton,                                     \
        /* [in] */ ICharSequence* text,                                   \
        /* [in] */ IDialogInterfaceOnClickListener* listener)             \
    {                                                                     \
        return superClass::SetButton(whichButton, text, listener);        \
    }                                                                     \
                                                                          \
    className::SetIcon(                                                   \
        /* [in] */ Int32 resId)                                           \
    {                                                                     \
        return superClass::SetIcon(resId);                                \
    }                                                                     \
                                                                          \
    className::SetIconEx(                                                 \
        /* [in] */ IDrawable* icon)                                       \
    {                                                                     \
        return superClass::SetIcon(icon);                                 \
    }                                                                     \
                                                                          \
    className::SetIconAttribute(                                          \
        /* [in] */ Int32 attrId)                                          \
    {                                                                     \
        return superClass::SetIconAttribute(attrId);                      \
    }                                                                     \
                                                                          \
    className::SetInverseBackgroundForced(                                \
        /* [in] */ Boolean forceInverseBackground)                        \
    {                                                                     \
        return superClass::SetInverseBackgroundForced(forceInverseBackground); \
    }

#endif // __ALERTDIALOGMACRO_H__
