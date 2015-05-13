
#ifndef __DIALOGMACRO_H__
#define __DIALOGMACRO_H__

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::View::IMenuItem;
using Elastos::Droid::View::IContextMenu;
using Elastos::Droid::View::IContextMenuInfo;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IDialogInterfaceOnCancelListener;
using Elastos::Droid::Content::IDialogInterfaceOnDismissListener;
using Elastos::Droid::Content::IDialogInterfaceOnShowListener;
using Elastos::Droid::Content::IDialogInterfaceOnKeyListener;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Graphics::Drawable::IDrawable;

namespace Elastos {
namespace Droid {
namespace App {

#define IDIALOG_METHODS_DECL()                                          \
    CARAPI Cancel();                                                    \
                                                                        \
    CARAPI Dismiss();                                                   \
                                                                        \
    CARAPI GetContext(                                                  \
        /* [out] */ IContext** context);                                \
                                                                        \
    CARAPI GetActionBar(                                                \
        /* [out] */ IActionBar** bar);                                  \
                                                                        \
    CARAPI SetOwnerActivity(                                            \
        /* [in] */ IActivity* activity);                                \
                                                                        \
    CARAPI GetOwnerActivity(                                            \
        /* [out] */ IActivity** activity);                              \
                                                                        \
    CARAPI IsShowing(                                                   \
        /* [out] */ Boolean* isShowing);                                \
                                                                        \
    CARAPI Show();                                                      \
                                                                        \
    CARAPI Hide();                                                      \
                                                                        \
    CARAPI DispatchOnCreate(                                            \
        /* [in] */ IBundle* savedInstanceState);                        \
                                                                        \
    CARAPI OnSaveInstanceState(                                         \
        /* [out] */ IBundle** bundle);                                  \
                                                                        \
    CARAPI OnRestoreInstanceState(                                      \
        /* [in] */ IBundle* savedInstanceState);                        \
                                                                        \
    CARAPI GetWindow(                                                   \
        /* [out] */ IWindow** window);                                  \
                                                                        \
    CARAPI GetCurrentFocus(                                             \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI FindViewById(                                                \
        /* [in] */ Int32 id,                                            \
        /* [out] */ IView** view);                                      \
                                                                        \
    CARAPI SetContentView(                                              \
        /* [in] */ Int32 layoutResID);                                  \
                                                                        \
    CARAPI SetContentViewEx(                                            \
        /* [in] */ IView* view);                                        \
                                                                        \
    CARAPI SetContentViewEx2(                                           \
        /* [in] */ IView* view,                                         \
        /* [in] */ IViewGroupLayoutParams* params);                     \
                                                                        \
    CARAPI AddContentView(                                              \
        /* [in] */ IView* view,                                         \
        /* [in] */ IViewGroupLayoutParams* params);                     \
                                                                        \
    CARAPI SetTitle(                                                    \
        /* [in] */ ICharSequence* title);                               \
                                                                        \
    CARAPI SetTitleEx(                                                  \
        /* [in] */ Int32 titleId);                                      \
                                                                        \
    CARAPI OnBackPressed();                                             \
                                                                        \
    CARAPI OnKeyShortcut(                                               \
        /* [in] */ Int32 keyCode,                                       \
        /* [in] */ IKeyEvent* event,                                    \
        /* [out] */ Boolean* res);                                      \
                                                                        \
    CARAPI OnTouchEvent(                                                \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* res);                                      \
                                                                        \
    CARAPI OnTrackballEvent(                                            \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* res);                                      \
                                                                        \
    CARAPI OnGenericMotionEvent(                                        \
        /* [in] */ IMotionEvent* event,                                 \
        /* [out] */ Boolean* res);                                      \
                                                                        \
    CARAPI OnCreateOptionsMenu(                                         \
        /* [in] */ IMenu* menu,                                         \
        /* [out] */ Boolean* res);                                      \
                                                                        \
    CARAPI OnPrepareOptionsMenu(                                        \
        /* [in] */ IMenu* menu,                                         \
        /* [out] */ Boolean* res);                                      \
                                                                        \
    CARAPI OnOptionsItemSelected(                                       \
        /* [in] */ IMenuItem* item,                                     \
        /* [out] */ Boolean* res);                                      \
                                                                        \
    CARAPI OnOptionsMenuClosed(                                         \
        /* [in] */ IMenu* menu);                                        \
                                                                        \
    CARAPI OpenOptionsMenu();                                           \
                                                                        \
    CARAPI CloseOptionsMenu();                                          \
                                                                        \
    CARAPI InvalidateOptionsMenu();                                     \
                                                                        \
    CARAPI RegisterForContextMenu(                                      \
        /* [in] */ IView* view);                                        \
                                                                        \
    CARAPI UnregisterForContextMenu(                                    \
        /* [in] */ IView* view);                                        \
                                                                        \
    CARAPI OpenContextMenu(                                             \
        /* [in] */ IView* view);                                        \
                                                                        \
    CARAPI OnContextItemSelected(                                       \
        /* [in] */ IMenuItem* item,                                     \
        /* [out] */ Boolean* res);                                      \
                                                                        \
    CARAPI OnContextMenuClosed(                                         \
        /* [in] */ IMenu* menu);                                        \
                                                                        \
    CARAPI TakeKeyEvents(                                               \
        /* [in] */ Boolean get);                                        \
                                                                        \
    CARAPI RequestWindowFeature(                                        \
        /* [in] */ Int32 featureId,                                     \
        /* [out] */ Boolean* res);                                      \
                                                                        \
    CARAPI SetFeatureDrawableResource(                                  \
        /* [in] */ Int32 featureId,                                     \
        /* [in] */ Int32 resId);                                        \
                                                                        \
    CARAPI SetFeatureDrawableUri(                                       \
        /* [in] */ Int32 featureId,                                     \
        /* [in] */ IUri* uri);                                          \
                                                                        \
    CARAPI SetFeatureDrawable(                                          \
        /* [in] */ Int32 featureId,                                     \
        /* [in] */ IDrawable* drawable);                                \
                                                                        \
    CARAPI SetFeatureDrawableAlpha(                                     \
        /* [in] */ Int32 featureId,                                     \
        /* [in] */ Int32 alpha);                                        \
                                                                        \
    CARAPI GetLayoutInflater(                                           \
        /* [out] */ ILayoutInflater** layout);                          \
                                                                        \
    CARAPI SetCancelable(                                               \
        /* [in] */ Boolean flag);                                       \
                                                                        \
    CARAPI SetCanceledOnTouchOutside(                                   \
        /* [in] */ Boolean cancel);                                     \
                                                                        \
    CARAPI SetOnCancelListener(                                         \
        /* [in] */ IDialogInterfaceOnCancelListener* listener);         \
                                                                        \
    CARAPI SetOnDismissListener(                                        \
        /* [in] */ IDialogInterfaceOnDismissListener* listener);        \
                                                                        \
    CARAPI SetOnShowListener(                                           \
        /* [in] */ IDialogInterfaceOnShowListener* listener);           \
                                                                        \
    CARAPI SetCancelMessage(                                            \
        /* [in] */ IMessage* msg);                                      \
                                                                        \
    CARAPI SetDismissMessage(                                           \
        /* [in] */ IMessage* msg);                                      \
                                                                        \
    CARAPI SetVolumeControlStream(                                      \
        /* [in] */ Int32 streamType);                                   \
                                                                        \
    CARAPI GetVolumeControlStream(                                      \
        /* [out] */ Int32* stream);                                     \
                                                                        \
    CARAPI TakeCancelAndDismissListeners(                               \
        /* [in] */ const String& msg,                                   \
        /* [in] */ IDialogInterfaceOnCancelListener* cancel,            \
        /* [in] */ IDialogInterfaceOnDismissListener* dismiss,          \
        /* [out] */ Boolean* result);                                   \
                                                                        \
    CARAPI SetOnKeyListener(                                            \
        /* [in] */ IDialogInterfaceOnKeyListener* onKeyListener);

#define IDIALOG_METHODS_IMPL(className, superClass)                    \
ECode className::Cancel()                                               \
{                                                                       \
    return superClass::Cancel();                                        \
}                                                                       \
                                                                        \
ECode className::Dismiss()                                              \
{                                                                       \
    return superClass::Dismiss();                                       \
}                                                                       \
                                                                        \
ECode className::GetContext(                                            \
    /* [out] */ IContext** context)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(context);                                         \
    AutoPtr<IContext> temp = superClass::GetContext();                  \
    *context = temp;                                                    \
    if (*context) {                                                     \
        (*context)->AddRef();                                           \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetActionBar(                                          \
    /* [out] */ IActionBar** bar)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(bar);                                             \
    AutoPtr<IActionBar> temp = superClass::GetActionBar();              \
    *bar = temp;                                                        \
    if (*bar) {                                                         \
        (*bar)->AddRef();                                               \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetOwnerActivity(                                      \
    /* [in] */ IActivity* activity)                                     \
{                                                                       \
    return superClass::SetOwnerActivity(activity);                      \
}                                                                       \
                                                                        \
ECode className::GetOwnerActivity(                                      \
    /* [out] */ IActivity** activity)                                   \
{                                                                       \
    VALIDATE_NOT_NULL(activity);                                        \
    AutoPtr<IActivity> temp = superClass::GetOwnerActivity();           \
    *activity = temp;                                                   \
    if (*activity) {                                                    \
        (*activity)->AddRef();                                          \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::IsShowing(                                             \
    /* [out] */ Boolean* isShowing)                                     \
{                                                                       \
    VALIDATE_NOT_NULL(isShowing);                                       \
    *isShowing = superClass::IsShowing();                               \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::Show()                                                 \
{                                                                       \
    return superClass::Show();                                          \
}                                                                       \
                                                                        \
ECode className::Hide()                                                 \
{                                                                       \
    return superClass::Hide();                                          \
}                                                                       \
                                                                        \
ECode className::DispatchOnCreate(                                      \
    /* [in] */ IBundle* savedInstanceState)                             \
{                                                                       \
    return superClass::DispatchOnCreate(savedInstanceState);            \
}                                                                       \
                                                                        \
ECode className::OnSaveInstanceState(                                   \
    /* [out] */ IBundle** bundle)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(bundle);                                          \
    AutoPtr<IBundle> temp = superClass::OnSaveInstanceState();          \
    *bundle = temp;                                                     \
    if (*bundle) {                                                      \
        (*bundle)->AddRef();                                            \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnRestoreInstanceState(                                \
    /* [in] */ IBundle* savedInstanceState)                             \
{                                                                       \
    return superClass::OnRestoreInstanceState(savedInstanceState);      \
}                                                                       \
                                                                        \
ECode className::GetWindow(                                             \
    /* [out] */ IWindow** window)                                       \
{                                                                       \
    VALIDATE_NOT_NULL(window);                                          \
    AutoPtr<IWindow> temp = superClass::GetWindow();                    \
    *window = temp;                                                     \
    if (*window) {                                                      \
        (*window)->AddRef();                                            \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::GetCurrentFocus(                                       \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
    AutoPtr<IView> temp = superClass::GetCurrentFocus();                \
    *view = temp;                                                       \
    if (*view) {                                                        \
        (*view)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::FindViewById(                                          \
    /* [in] */ Int32 id,                                                \
    /* [out] */ IView** view)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(view);                                            \
    AutoPtr<IView> temp = superClass::FindViewById(id);                 \
    *view = temp;                                                       \
    if (*view) {                                                        \
        (*view)->AddRef();                                              \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetContentView(                                        \
    /* [in] */ Int32 layoutResID)                                       \
{                                                                       \
    return superClass::SetContentView(layoutResID);                     \
}                                                                       \
                                                                        \
ECode className::SetContentViewEx(                                      \
    /* [in] */ IView* view)                                             \
{                                                                       \
    return superClass::SetContentView(view);                            \
}                                                                       \
                                                                        \
ECode className::SetContentViewEx2(                                     \
    /* [in] */ IView* view,                                             \
    /* [in] */ IViewGroupLayoutParams* params)                          \
{                                                                       \
    return superClass::SetContentView(view, params);                    \
}                                                                       \
                                                                        \
ECode className::AddContentView(                                        \
    /* [in] */ IView* view,                                             \
    /* [in] */ IViewGroupLayoutParams* params)                          \
{                                                                       \
    return superClass::AddContentView(view, params);                    \
}                                                                       \
                                                                        \
ECode className::SetTitle(                                              \
    /* [in] */ ICharSequence* title)                                    \
{                                                                       \
    return superClass::SetTitle(title);                                 \
}                                                                       \
                                                                        \
ECode className::SetTitleEx(                                            \
    /* [in] */ Int32 titleId)                                           \
{                                                                       \
    return superClass::SetTitle(titleId);                               \
}                                                                       \
                                                                        \
ECode className::OnBackPressed()                                        \
{                                                                       \
    return superClass::OnBackPressed();                                 \
}                                                                       \
                                                                        \
ECode className::OnKeyShortcut(                                         \
    /* [in] */ Int32 keyCode,                                           \
    /* [in] */ IKeyEvent* event,                                        \
    /* [out] */ Boolean* res)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(res);                                             \
    *res = superClass::OnKeyShortcut(keyCode, event);                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnTouchEvent(                                          \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* res)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(res);                                             \
    *res = superClass::OnTouchEvent(event);                             \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnTrackballEvent(                                      \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* res)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(res);                                             \
    *res = superClass::OnTrackballEvent(event);                         \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnGenericMotionEvent(                                  \
    /* [in] */ IMotionEvent* event,                                     \
    /* [out] */ Boolean* res)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(res);                                             \
    *res = superClass::OnGenericMotionEvent(event);                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnCreateOptionsMenu(                                   \
    /* [in] */ IMenu* menu,                                             \
    /* [out] */ Boolean* res)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(res);                                             \
    *res = superClass::OnCreateOptionsMenu(menu);                       \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnPrepareOptionsMenu(                                  \
    /* [in] */ IMenu* menu,                                             \
    /* [out] */ Boolean* res)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(res);                                             \
    *res = superClass::OnPrepareOptionsMenu(menu);                      \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnOptionsItemSelected(                                 \
    /* [in] */ IMenuItem* item,                                         \
    /* [out] */ Boolean* res)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(res);                                             \
    *res = superClass::OnOptionsItemSelected(item);                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnOptionsMenuClosed(                                   \
    /* [in] */ IMenu* menu)                                             \
{                                                                       \
    return superClass::OnOptionsMenuClosed(menu);                       \
}                                                                       \
                                                                        \
ECode className::OpenOptionsMenu()                                      \
{                                                                       \
    return superClass::OpenOptionsMenu();                               \
}                                                                       \
                                                                        \
ECode className::CloseOptionsMenu()                                     \
{                                                                       \
    return superClass::CloseOptionsMenu();                              \
}                                                                       \
                                                                        \
ECode className::InvalidateOptionsMenu()                                \
{                                                                       \
    return superClass::InvalidateOptionsMenu();                         \
}                                                                       \
                                                                        \
ECode className::RegisterForContextMenu(                                \
    /* [in] */ IView* view)                                             \
{                                                                       \
    return superClass::RegisterForContextMenu(view);                    \
}                                                                       \
                                                                        \
ECode className::UnregisterForContextMenu(                              \
    /* [in] */ IView* view)                                             \
{                                                                       \
    return superClass::UnregisterForContextMenu(view);                  \
}                                                                       \
                                                                        \
ECode className::OpenContextMenu(                                       \
    /* [in] */ IView* view)                                             \
{                                                                       \
    return superClass::OpenContextMenu(view);                           \
}                                                                       \
                                                                        \
ECode className::OnContextItemSelected(                                 \
    /* [in] */ IMenuItem* item,                                         \
    /* [out] */ Boolean* res)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(res);                                             \
    *res = superClass::OnContextItemSelected(item);                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::OnContextMenuClosed(                                   \
    /* [in] */ IMenu* menu)                                             \
{                                                                       \
    return superClass::OnContextMenuClosed(menu);                       \
}                                                                       \
                                                                        \
ECode className::TakeKeyEvents(                                         \
    /* [in] */ Boolean get)                                             \
{                                                                       \
    return superClass::TakeKeyEvents(get);                              \
}                                                                       \
                                                                        \
ECode className::RequestWindowFeature(                                  \
    /* [in] */ Int32 featureId,                                         \
    /* [out] */ Boolean* res)                                           \
{                                                                       \
    VALIDATE_NOT_NULL(res);                                             \
    *res = superClass::RequestWindowFeature(featureId);                 \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetFeatureDrawableResource(                            \
    /* [in] */ Int32 featureId,                                         \
    /* [in] */ Int32 resId)                                             \
{                                                                       \
    return superClass::SetFeatureDrawableResource(featureId, resId);    \
}                                                                       \
                                                                        \
ECode className::SetFeatureDrawableUri(                                 \
    /* [in] */ Int32 featureId,                                         \
    /* [in] */ IUri* uri)                                               \
{                                                                       \
    return superClass::SetFeatureDrawableUri(featureId, uri);           \
}                                                                       \
                                                                        \
ECode className::SetFeatureDrawable(                                    \
    /* [in] */ Int32 featureId,                                         \
    /* [in] */ IDrawable* drawable)                                     \
{                                                                       \
    return superClass::SetFeatureDrawable(featureId, drawable);         \
}                                                                       \
                                                                        \
ECode className::SetFeatureDrawableAlpha(                               \
    /* [in] */ Int32 featureId,                                         \
    /* [in] */ Int32 alpha)                                             \
{                                                                       \
    return superClass::SetFeatureDrawableAlpha(featureId, alpha);       \
}                                                                       \
                                                                        \
ECode className::GetLayoutInflater(                                     \
    /* [out] */ ILayoutInflater** layout)                               \
{                                                                       \
    VALIDATE_NOT_NULL(layout);                                          \
    AutoPtr<ILayoutInflater> temp = superClass::GetLayoutInflater();    \
    *layout = temp;                                                     \
    if (*layout) {                                                      \
        (*layout)->AddRef();                                            \
    }                                                                   \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetCancelable(                                         \
    /* [in] */ Boolean flag)                                            \
{                                                                       \
    return superClass::SetCancelable(flag);                             \
}                                                                       \
                                                                        \
ECode className::SetCanceledOnTouchOutside(                             \
    /* [in] */ Boolean cancel)                                          \
{                                                                       \
    return superClass::SetCanceledOnTouchOutside(cancel);               \
}                                                                       \
                                                                        \
ECode className::SetOnCancelListener(                                   \
    /* [in] */ IDialogInterfaceOnCancelListener* listener)              \
{                                                                       \
    return superClass::SetOnCancelListener(listener);                   \
}                                                                       \
                                                                        \
ECode className::SetOnDismissListener(                                  \
    /* [in] */ IDialogInterfaceOnDismissListener* listener)             \
{                                                                       \
    return superClass::SetOnDismissListener(listener);                  \
}                                                                       \
                                                                        \
ECode className::SetOnShowListener(                                     \
    /* [in] */ IDialogInterfaceOnShowListener* listener)                \
{                                                                       \
    return superClass::SetOnShowListener(listener);                     \
}                                                                       \
                                                                        \
ECode className::SetCancelMessage(                                      \
    /* [in] */ IMessage* msg)                                           \
{                                                                       \
    return superClass::SetCancelMessage(msg);                           \
}                                                                       \
                                                                        \
ECode className::SetDismissMessage(                                     \
    /* [in] */ IMessage* msg)                                           \
{                                                                       \
    return superClass::SetDismissMessage(msg);                          \
}                                                                       \
                                                                        \
ECode className::SetVolumeControlStream(                                \
    /* [in] */ Int32 streamType)                                        \
{                                                                       \
    return superClass::SetVolumeControlStream(streamType);              \
}                                                                       \
                                                                        \
ECode className::GetVolumeControlStream(                                \
    /* [out] */ Int32* stream)                                          \
{                                                                       \
    VALIDATE_NOT_NULL(stream);                                          \
    *stream = superClass::GetVolumeControlStream();                     \
                                                                        \
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::TakeCancelAndDismissListeners(                         \
    /* [in] */ const String& msg,                                       \
    /* [in] */ IDialogInterfaceOnCancelListener* cancel,                \
    /* [in] */ IDialogInterfaceOnDismissListener* dismiss,              \
    /* [out] */ Boolean* result)                                        \
{                                                                       \
    VALIDATE_NOT_NULL(result)                                           \
    *result = superClass::TakeCancelAndDismissListeners(msg, cancel, dismiss);\
    return NOERROR;                                                     \
}                                                                       \
                                                                        \
ECode className::SetOnKeyListener(                                      \
    /* [in] */ IDialogInterfaceOnKeyListener* onKeyListener)            \
{                                                                       \
    return superClass::SetOnKeyListener(onKeyListener);                 \
}

}
}
}

#endif  //__DIALOGMACRO_H__
