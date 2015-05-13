
#ifndef  __CIINPUTMETHODWRAPPER_H__
#define  __CIINPUTMETHODWRAPPER_H__

#include "_CIInputMethodWrapper.h"
#include "ext/frameworkext.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Internal::View::IInputContext;
using Elastos::Droid::Os::IResultReceiver;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IHandlerCaller;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::View::IInputMethodCallback;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::InputMethod::IInputMethodSession;
using Elastos::Droid::Internal::View::IIInputMethodSession;
using Elastos::Droid::View::InputMethod::IInputBinding;
using Elastos::Droid::View::InputMethod::IEditorInfo;
using Elastos::Droid::View::InputMethod::ICompletionInfo;
using Elastos::Droid::View::InputMethod::IExtractedText;
using Elastos::Droid::View::InputMethod::IInputMethodSubtype;
using Elastos::Droid::View::InputMethod::IInputMethodSessionEventCallback;
using Elastos::Droid::View::InputMethod::IInputMethod;


namespace Elastos {
namespace Droid {
namespace InputMethodService {


/**
 * Implements the internal IInputMethod interface to convert incoming calls
 * on to it back to calls on the public InputMethod interface, scheduling
 * them on the main thread of the process.
 */
CarClass(CIInputMethodWrapper)
{
private:
    // static class Notifier {
    //     Boolean notified;
    // };

    // NOTE: we should have a cache of these.
    class InputMethodSessionCallbackWrapper
        : public ElRefBase
        , public IInputMethodSessionEventCallback
    {
    public:
        InputMethodSessionCallbackWrapper(
            /* [in] */ IContext* context,
            /* [in] */ IInputMethodCallback* cb);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI SessionCreated(
            /* [in] */ IInputMethodSession* session);

    private:
        AutoPtr<IContext> mContext;
        AutoPtr<IInputMethodCallback> mCb;
    };

public:
    CARAPI constructor(
        /* [in] */ IAbstractInputMethodService* context,
        /* [in] */ IInputMethod* inputMethod);

    CARAPI_(AutoPtr<IInputMethod>) GetInternalInputMethod();

    CARAPI ExecuteMessage(
        /* [in] */ IMessage* msg);

    CARAPI AttachToken(
        /* [in] */ IBinder* token);

    CARAPI BindInput(
        /* [in] */ IInputBinding* binding);

    CARAPI UnbindInput();

    CARAPI StartInput(
        /* [in] */ IInputContext* inputContext,
        /* [in] */ IEditorInfo* attribute);

    CARAPI RestartInput(
        /* [in] */ IInputContext* inputContext,
        /* [in] */ IEditorInfo* attribute);

    CARAPI CreateSession(
        /* [in] */ IInputMethodCallback* callback);

    CARAPI SetSessionEnabled(
        /* [in] */ IIInputMethodSession* session,
        /* [in] */ Boolean enabled);

    CARAPI RevokeSession(
        /* [in] */ IIInputMethodSession* session);

    CARAPI ShowSoftInput(
        /* [in] */ Int32 flags,
        /* [in] */ IResultReceiver* resultReceiver);

    CARAPI HideSoftInput(
        /* [in] */ Int32 flags,
        /* [in] */ IResultReceiver* resultReceiver);

    CARAPI ChangeInputMethodSubtype(
        /* [in] */ IInputMethodSubtype* subtype);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    static const String TAG;
    static const Boolean DEBUG;

    static const Int32 DO_DUMP;
    static const Int32 DO_ATTACH_TOKEN;
    static const Int32 DO_SET_INPUT_CONTEXT;
    static const Int32 DO_UNSET_INPUT_CONTEXT;
    static const Int32 DO_START_INPUT;
    static const Int32 DO_RESTART_INPUT;
    static const Int32 DO_CREATE_SESSION;
    static const Int32 DO_SET_SESSION_ENABLED;
    static const Int32 DO_REVOKE_SESSION;
    static const Int32 DO_SHOW_SOFT_INPUT;
    static const Int32 DO_HIDE_SOFT_INPUT;
    static const Int32 DO_CHANGE_INPUTMETHOD_SUBTYPE;

    AutoPtr<IWeakReference> mTarget;//final WeakReference<AbstractInputMethodService> ;
    AutoPtr<IHandlerCaller> mCaller;
    AutoPtr<IWeakReference> mInputMethod; // WeakReference<InputMethod>
    AutoPtr<IContext> mContext;
    Int32 mTargetSdkVersion;
};

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos

#endif  //__CIINPUTMETHODWRAPPER_H__
