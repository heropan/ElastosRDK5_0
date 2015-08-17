
#include "ext/frameworkdef.h"
#include "inputmethodservice/CIInputMethodSessionWrapper.h"
#include "os/HandlerCaller.h"
#include "os/SomeArgs.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::CString;
using Elastos::Core::CObjectContainer;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::SomeArgs;
using Elastos::Droid::Os::HandlerCaller;
using Elastos::Droid::Os::EIID_IHandlerCallerCallback;
using Elastos::Droid::View::InputMethod::EIID_ILocalInputMethodSessionEventCallback;
using Elastos::Droid::View::InputMethod::IInputMethodSession;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

String CIInputMethodSessionWrapper::TAG("CIInputMethodSessionWrapper");
const Boolean CIInputMethodSessionWrapper::DEBUG = FALSE;
const Int32 CIInputMethodSessionWrapper::DO_FINISH_INPUT = 60;
const Int32 CIInputMethodSessionWrapper::DO_DISPLAY_COMPLETIONS = 65;
const Int32 CIInputMethodSessionWrapper::DO_UPDATE_EXTRACTED_TEXT = 67;
const Int32 CIInputMethodSessionWrapper::DO_DISPATCH_KEY_EVENT = 70;
const Int32 CIInputMethodSessionWrapper::DO_DISPATCH_TRACKBALL_EVENT = 80;
const Int32 CIInputMethodSessionWrapper::DO_UPDATE_SELECTION = 90;
const Int32 CIInputMethodSessionWrapper::DO_UPDATE_CURSOR = 95;
const Int32 CIInputMethodSessionWrapper::DO_APP_PRIVATE_COMMAND = 100;
const Int32 CIInputMethodSessionWrapper::DO_TOGGLE_SOFT_INPUT = 105;
const Int32 CIInputMethodSessionWrapper::DO_FINISH_SESSION = 110;
const Int32 CIInputMethodSessionWrapper::DO_DISPATCH_GENERIC_MOTION_EVENT = 85;
const Int32 CIInputMethodSessionWrapper::DO_VIEW_CLICKED = 115;

CAR_INTERFACE_IMPL(CIInputMethodSessionWrapper::InputMethodEventCallbackWrapper, Object, ILocalInputMethodSessionEventCallback)

ECode CIInputMethodSessionWrapper::InputMethodEventCallbackWrapper::FinishedEvent(
    /* [in] */ Int32 seq,
    /* [in] */ Boolean handled)
{
    // try {
    return mCb->FinishedEvent(seq, handled);
    // } catch (RemoteException e) {
    // }
}

CAR_OBJECT_IMPL(CIInputMethodSessionWrapper);
CAR_INTERFACE_IMPL_3(CIInputMethodSessionWrapper, Object, IIInputMethodSession, IBinder, IHandlerCallerCallback);

ECode CIInputMethodSessionWrapper::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IInputMethodSession* inputMethodSession)
{
    mInputMethodSession = inputMethodSession;
    mCaller = new HandlerCaller(context, THIS_PROBE(IHandlerCallerCallback), FALSE);
    return NOERROR;
}

AutoPtr<IInputMethodSession> CIInputMethodSessionWrapper::GetInternalInputMethodSession()
{
    return mInputMethodSession;
}

ECode CIInputMethodSessionWrapper::ExecuteMessage(
    /* [in] */ IMessage* msg)
{
    if (mInputMethodSession == NULL) {
        return NOERROR;
    }

    Int32 what, arg1, arg2;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);
    msg->GetArg2(&arg2);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);

    switch (what) {
        case DO_FINISH_INPUT:
            mInputMethodSession->FinishInput();
            return NOERROR;
        case DO_DISPLAY_COMPLETIONS: {
            AutoPtr<ArrayOf<ICompletionInfo*> > array;
            AutoPtr<IObjectContainer> container = IObjectContainer::Probe(obj);
            if (container) {
                Int32 count;
                container->GetObjectCount(&count);
                array = ArrayOf<ICompletionInfo*>::Alloc(count);
                AutoPtr<IObjectEnumerator> emu;
                container->GetObjectEnumerator((IObjectEnumerator**)&emu);
                Boolean hasNext = FALSE;
                Int32 i = 0;
                while(emu->MoveNext(&hasNext), hasNext) {
                    AutoPtr<IInterface> current;
                    emu->Current((IInterface**)&current);
                    array->Set(i++, ICompletionInfo::Probe(current));
                }
            }
            else {
                array = ArrayOf<ICompletionInfo*>::Alloc(0);
            }

            mInputMethodSession->DisplayCompletions(array);
            return NOERROR;
        }
        case DO_UPDATE_EXTRACTED_TEXT:
            mInputMethodSession->UpdateExtractedText(arg1, IExtractedText::Probe(obj));
            return NOERROR;
        case DO_DISPATCH_KEY_EVENT: {
            SomeArgs* args = (SomeArgs*)obj.Get();
            IKeyEvent* event = IKeyEvent::Probe(args->mArg1);
            IInputMethodCallback* callback = IInputMethodCallback::Probe(args->mArg2);
            AutoPtr<ILocalInputMethodSessionEventCallback> cb =
                new InputMethodEventCallbackWrapper(callback);
            mInputMethodSession->DispatchKeyEvent(arg1, event, cb);
            args->Recycle();
            return NOERROR;
        }
        case DO_DISPATCH_TRACKBALL_EVENT: {
            SomeArgs* args = (SomeArgs*)obj.Get();
            IMotionEvent* event = IMotionEvent::Probe(args->mArg1);
            IInputMethodCallback* callback = IInputMethodCallback::Probe(args->mArg2);
            AutoPtr<ILocalInputMethodSessionEventCallback> cb =
                new InputMethodEventCallbackWrapper(callback);
            mInputMethodSession->DispatchTrackballEvent(arg1, event, cb);
            args->Recycle();
            return NOERROR;
        }
        case DO_DISPATCH_GENERIC_MOTION_EVENT: {
            SomeArgs* args = (SomeArgs*)obj.Get();
            IMotionEvent* event = IMotionEvent::Probe(args->mArg1);
            IInputMethodCallback* callback = IInputMethodCallback::Probe(args->mArg2);
            AutoPtr<ILocalInputMethodSessionEventCallback> cb =
                new InputMethodEventCallbackWrapper(callback);
            mInputMethodSession->DispatchGenericMotionEvent(arg1, event, cb);
            args->Recycle();
            return NOERROR;
        }
        case DO_UPDATE_SELECTION: {
            SomeArgs* args = (SomeArgs*)obj.Get();
            mInputMethodSession->UpdateSelection(args->mArgi1, args->mArgi2,
                args->mArgi3, args->mArgi4, args->mArgi5, args->mArgi6);
            args->Recycle();
            return NOERROR;
        }
        case DO_UPDATE_CURSOR: {
            mInputMethodSession->UpdateCursor(IRect::Probe(obj));
            return NOERROR;
        }
        case DO_APP_PRIVATE_COMMAND: {
            SomeArgs* args = (SomeArgs*)obj.Get();
            ICharSequence* seq = ICharSequence::Probe(args->mArg1);
            String info;
            seq->ToString(&info);
            mInputMethodSession->AppPrivateCommand(info, IBundle::Probe(args->mArg2));
            args->Recycle();
            return NOERROR;
        }
        case DO_TOGGLE_SOFT_INPUT: {
            mInputMethodSession->ToggleSoftInput(arg1, arg2);
            return NOERROR;
        }
        case DO_FINISH_SESSION: {
            mInputMethodSession = NULL;
            return NOERROR;
        }
        case DO_VIEW_CLICKED: {
            mInputMethodSession->ViewClicked(arg1 == 1);
            return NOERROR;
        }
    }

    Logger::W(TAG, "Unhandled message code: %d", what);
    return NOERROR;
}

ECode CIInputMethodSessionWrapper::FinishInput()
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_FINISH_INPUT, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodSessionWrapper::DisplayCompletions(
    /* [in] */ ArrayOf<ICompletionInfo*>* completions)
{
    AutoPtr<IObjectContainer> container;
    if (completions != NULL) {
        CObjectContainer::New((IObjectContainer**)&container);
        for (Int32 i = 0; i < completions->GetLength(); ++i) {
            container->Add((*completions)[i]);
        }
    }

    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_DISPLAY_COMPLETIONS, container, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodSessionWrapper::UpdateExtractedText(
    /* [in] */ Int32 token,
    /* [in] */ IExtractedText* text)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_UPDATE_EXTRACTED_TEXT, token, text, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodSessionWrapper::DispatchKeyEvent(
    /* [in] */ Int32 seq,
    /* [in] */ IKeyEvent* event,
    /* [in] */ IInputMethodCallback* callback)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_DISPATCH_KEY_EVENT, seq, event, callback, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodSessionWrapper::DispatchTrackballEvent(
    /* [in] */ Int32 seq,
    /* [in] */ IMotionEvent* event,
    /* [in] */ IInputMethodCallback* callback)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_DISPATCH_TRACKBALL_EVENT, seq, event, callback, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodSessionWrapper::DispatchGenericMotionEvent(
    /* [in] */ Int32 seq,
    /* [in] */ IMotionEvent* event,
    /* [in] */ IInputMethodCallback* callback)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_DISPATCH_GENERIC_MOTION_EVENT, seq, event, callback, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodSessionWrapper::UpdateSelection(
    /* [in] */ Int32 oldSelStart,
    /* [in] */ Int32 oldSelEnd,
    /* [in] */ Int32 newSelStart,
    /* [in] */ Int32 newSelEnd,
    /* [in] */ Int32 candidatesStart,
    /* [in] */ Int32 candidatesEnd)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_UPDATE_SELECTION,
        oldSelStart, oldSelEnd, newSelStart, newSelEnd,
        candidatesStart, candidatesEnd, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodSessionWrapper::ViewClicked(
    /* [in] */ Boolean focusChanged)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_VIEW_CLICKED, focusChanged ? 1 : 0, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodSessionWrapper::UpdateCursor(
    /* [in] */ IRect* newCursor)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_UPDATE_CURSOR, newCursor, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodSessionWrapper::AppPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    AutoPtr<ICharSequence> seq;
    CString::New(action, (ICharSequence**)&seq);
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_APP_PRIVATE_COMMAND, seq, data, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodSessionWrapper::ToggleSoftInput(
    /* [in] */ Int32 showFlags,
    /* [in] */ Int32 hideFlags)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_APP_PRIVATE_COMMAND, showFlags, hideFlags, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodSessionWrapper::FinishSession()
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_FINISH_SESSION, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CIInputMethodSessionWrapper::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String("CIInputMethodSessionWrapper");
    return NOERROR;
}

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos
