
#ifndef  __ELASTOS_DROID_INPUTMEHTODSERVICE_CIINPUTMETHODSESSIONWRAPPER_H__
#define  __ELASTOS_DROID_INPUTMEHTODSERVICE_CIINPUTMETHODSESSIONWRAPPER_H__

#include "_Elastos_Droid_InputMethodService_CIInputMethodSessionWrapper.h"
#include "ext/frameworkext.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IHandlerCaller;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::View::IInputMethodCallback;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::InputMethod::ILocalInputMethodSessionEventCallback;
using Elastos::Droid::View::InputMethod::ICompletionInfo;
using Elastos::Droid::View::InputMethod::IExtractedText;
using Elastos::Droid::View::InputMethod::IInputMethodSession;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

CarClass(CIInputMethodSessionWrapper)
    , public Object
    , public IIInputMethodSession
    , public IBinder
    , public IHandlerCallerCallback
{
protected:
    // NOTE: we should have a cache of these.
    class InputMethodEventCallbackWrapper
        : public Object
        , public ILocalInputMethodSessionEventCallback
    {
    public:
        InputMethodEventCallbackWrapper(
            /* [in] */ IInputMethodCallback* cb)
            : mCb(cb)
        {}

        CAR_INTERFACE_DECL();

        CARAPI FinishedEvent(
            /* [in] */ Int32 seq,
            /* [in] */ Boolean handled);

    private:
        AutoPtr<IInputMethodCallback> mCb;
    };

public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IInputMethodSession* inputMethodSession);

    CARAPI_(AutoPtr<IInputMethodSession>) GetInternalInputMethodSession();

    CARAPI ExecuteMessage(
        /* [in] */ IMessage* msg);

    CARAPI FinishInput();

    CARAPI DisplayCompletions(
        /* [in] */ ArrayOf<ICompletionInfo*>* completions);

    CARAPI UpdateExtractedText(
        /* [in] */ Int32 token,
        /* [in] */ IExtractedText* text);

    CARAPI DispatchKeyEvent(
        /* [in] */ Int32 seq,
        /* [in] */ IKeyEvent* event,
        /* [in] */ IInputMethodCallback* callback);

    CARAPI DispatchTrackballEvent(
        /* [in] */ Int32 seq,
        /* [in] */ IMotionEvent* event,
        /* [in] */ IInputMethodCallback* callback);

    CARAPI UpdateSelection(
        /* [in] */ Int32 oldSelStart,
        /* [in] */ Int32 oldSelEnd,
        /* [in] */ Int32 newSelStart,
        /* [in] */ Int32 newSelEnd,
        /* [in] */ Int32 candidatesStart,
        /* [in] */ Int32 candidatesEnd);

    CARAPI UpdateCursor(
        /* [in] */ IRect* newCursor);

    CARAPI AppPrivateCommand(
        /* [in] */ const String& action,
        /* [in] */ IBundle* data);

    CARAPI ToggleSoftInput(
        /* [in] */ Int32 showFlags,
        /* [in] */ Int32 hideFlags);

    CARAPI FinishSession();

    CARAPI DispatchGenericMotionEvent(
        /* [in] */ Int32 seq,
        /* [in] */ IMotionEvent* event,
        /* [in] */ IInputMethodCallback* callback);

    CARAPI ViewClicked(
        /* [in] */ Boolean focusChanged);

    CARAPI ToString(
        /* [out] */ String* str);

protected:
    AutoPtr<IHandlerCaller> mCaller;
    AutoPtr<IInputMethodSession> mInputMethodSession;

private:
    static String TAG;
    static const Boolean DEBUG;

    static const Int32 DO_FINISH_INPUT;
    static const Int32 DO_DISPLAY_COMPLETIONS;
    static const Int32 DO_UPDATE_EXTRACTED_TEXT;
    static const Int32 DO_DISPATCH_KEY_EVENT;
    static const Int32 DO_DISPATCH_TRACKBALL_EVENT;
    static const Int32 DO_DISPATCH_GENERIC_MOTION_EVENT;
    static const Int32 DO_UPDATE_SELECTION;
    static const Int32 DO_UPDATE_CURSOR;
    static const Int32 DO_APP_PRIVATE_COMMAND;
    static const Int32 DO_TOGGLE_SOFT_INPUT;
    static const Int32 DO_FINISH_SESSION;
    static const Int32 DO_VIEW_CLICKED;
};

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos

#endif  // __ELASTOS_DROID_INPUTMEHTODSERVICE_CIINPUTMETHODSESSIONWRAPPER_H__
