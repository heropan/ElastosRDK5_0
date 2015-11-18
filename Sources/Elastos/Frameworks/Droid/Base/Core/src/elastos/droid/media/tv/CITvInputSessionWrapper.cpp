#include "elastos/droid/media/tv/CITvInputSessionWrapper.h"
#include "elastos/droid/media/tv/TvInputServiceSession.h"
#include "elastos/droid/internal/os/CHandlerCaller.h"
#include "elastos/droid/internal/os/SomeArgs.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Internal::Os::CHandlerCaller;
using Elastos::Droid::Internal::Os::EIID_IHandlerCallerCallback;
using Elastos::Droid::Internal::Os::ISomeArgs;
using Elastos::Droid::Internal::Os::SomeArgs;
using Elastos::Droid::View::EIID_IInputEventReceiver;
using Elastos::Droid::View::IInputEventReceiver;
using Elastos::Droid::View::ISurface;
using Elastos::Core::CBoolean;
using Elastos::Core::CFloat;
using Elastos::Core::CInteger32;
using Elastos::Core::CString;
using Elastos::Core::IBoolean;
using Elastos::Core::ICharSequence;
using Elastos::Core::IFloat;
using Elastos::Core::IInteger32;
using Elastos::Core::ISystem;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Tv {

String CITvInputSessionWrapper::TAG("TvInputSessionWrapper");

Int32 CITvInputSessionWrapper::MESSAGE_HANDLING_DURATION_THRESHOLD_MILLIS = 50;

const Int32 CITvInputSessionWrapper::DO_RELEASE = 1;
const Int32 CITvInputSessionWrapper::DO_SET_MAIN = 2;
const Int32 CITvInputSessionWrapper::DO_SET_SURFACE = 3;
const Int32 CITvInputSessionWrapper::DO_DISPATCH_SURFACE_CHANGED = 4;
const Int32 CITvInputSessionWrapper::DO_SET_STREAM_VOLUME = 5;
const Int32 CITvInputSessionWrapper::DO_TUNE = 6;
const Int32 CITvInputSessionWrapper::DO_SET_CAPTION_ENABLED = 7;
const Int32 CITvInputSessionWrapper::DO_SELECT_TRACK = 8;
const Int32 CITvInputSessionWrapper::DO_APP_PRIVATE_COMMAND = 9;
const Int32 CITvInputSessionWrapper::DO_CREATE_OVERLAY_VIEW = 10;
const Int32 CITvInputSessionWrapper::DO_RELAYOUT_OVERLAY_VIEW = 11;
const Int32 CITvInputSessionWrapper::DO_REMOVE_OVERLAY_VIEW = 12;
const Int32 CITvInputSessionWrapper::DO_REQUEST_UNBLOCK_CONTENT = 13;

CAR_INTERFACE_IMPL_3(CITvInputSessionWrapper, Object, IITvInputSessionWrapper, IITvInputSession, IHandlerCallerCallback)

CAR_OBJECT_IMPL(CITvInputSessionWrapper)

CITvInputSessionWrapper::TvInputEventReceiver::TvInputEventReceiver(
    /* [in] */ IInputChannel * inputChannel,
    /* [in] */ ILooper * looper,
    /* [in] */ ITvInputServiceSession * impl)
{
    mImpl = impl;
//TODO: Need InputEventReceiver.h
    // InputEventReceiver(inputChannel, looper);
}

ECode CITvInputSessionWrapper::TvInputEventReceiver::OnInputEvent(
    /* [in] */ IInputEvent * event)
{
    if (mImpl == NULL) {
        // The session has been finished.
//TODO: Need InputEventReceiver.h
        // return FinishInputEvent(event, FALSE);
        return NOERROR;
    }

    Int32 handled;
    ((TvInputServiceSession*)mImpl.Get())->DispatchInputEvent(event, THIS_PROBE(IInputEventReceiver), &handled);
    if (handled != ITvInputManagerSession::DISPATCH_IN_PROGRESS) {
//TODO: Need InputEventReceiver.h
        // FinishInputEvent(event, handled == ITvInputManagerSession::DISPATCH_HANDLED);
    }
    return NOERROR;
}

CITvInputSessionWrapper::CITvInputSessionWrapper()
{
}

CITvInputSessionWrapper::~CITvInputSessionWrapper()
{
}

ECode CITvInputSessionWrapper::constructor(
    /* [in] */ IContext * context,
    /* [in] */ ITvInputServiceSession * sessionImpl,
    /* [in] */ IInputChannel * channel)
{
//TODO: Need CHandlerCaller
    // CHandlerCaller::New(context, NULL, this, TRUE /* asyncHandler */, (IHandlerCaller**)&mCaller);
    mTvInputSessionImpl = sessionImpl;
    mChannel = channel;
    if (channel != NULL) {
        AutoPtr<ILooper> mainLooper;
        context->GetMainLooper((ILooper**)&mainLooper);
        mReceiver = new TvInputEventReceiver(channel, mainLooper, mTvInputSessionImpl);
    }
    return NOERROR;
}

ECode CITvInputSessionWrapper::ExecuteMessage(
    /* [in] */ IMessage * msg)
{
    if (mTvInputSessionImpl == NULL) {
        return NOERROR;
    }

    Int64 startTime;
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    system->GetCurrentTimeMillis(&startTime);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);
    Int32 what;
    msg->GetWhat(&what);

    AutoPtr<TvInputServiceSession> impl = (TvInputServiceSession*)mTvInputSessionImpl.Get();
    switch (what) {
        case DO_RELEASE: {
            impl->ReleaseResources();
            mTvInputSessionImpl = NULL;
            if (mReceiver != NULL) {
//TODO: Need InputEventReceiver.h
                // mReceiver->Dispose();
                mReceiver = NULL;
            }
            if (mChannel != NULL) {
                mChannel->Dispose();
                mChannel = NULL;
            }
            break;
        }
        case DO_SET_MAIN: {
            Boolean b;
            IBoolean::Probe(obj)->GetValue(&b);
            impl->SetMain(b);
            break;
        }
        case DO_SET_SURFACE: {
            impl->SetSurface(ISurface::Probe(obj));
            break;
        }
        case DO_DISPATCH_SURFACE_CHANGED: {
            AutoPtr<SomeArgs> args = (SomeArgs*)ISomeArgs::Probe(obj);
            impl->DispatchSurfaceChanged(args->mArgi1, args->mArgi2, args->mArgi3);
            args->Recycle();
            break;
        }
        case DO_SET_STREAM_VOLUME: {
            Float f;
            IFloat::Probe(obj)->GetValue(&f);
            impl->SetStreamVolume(f);
            break;
        }
        case DO_TUNE: {
            AutoPtr<SomeArgs> args = (SomeArgs*)ISomeArgs::Probe(obj);
            impl->Tune(IUri::Probe(args->mArg1), IBundle::Probe(args->mArg2));
            args->Recycle();
            break;
        }
        case DO_SET_CAPTION_ENABLED: {
            Boolean b;
            IBoolean::Probe(obj)->GetValue(&b);
            impl->SetCaptionEnabled(b);
            break;
        }
        case DO_SELECT_TRACK: {
            AutoPtr<SomeArgs> args = (SomeArgs*)ISomeArgs::Probe(obj);
            Int32 val;
            IInteger32::Probe(args->mArg1)->GetValue(&val);
            String str;
            ICharSequence::Probe(args->mArg2)->ToString(&str);
            impl->SelectTrack(val, str);
            args->Recycle();
            break;
        }
        case DO_APP_PRIVATE_COMMAND: {
            AutoPtr<SomeArgs> args = (SomeArgs*)ISomeArgs::Probe(obj);
            String str;
            ICharSequence::Probe(args->mArg1)->ToString(&str);
            impl->AppPrivateCommand(str, IBundle::Probe(args->mArg2));
            args->Recycle();
            break;
        }
        case DO_CREATE_OVERLAY_VIEW: {
            AutoPtr<SomeArgs> args = (SomeArgs*)ISomeArgs::Probe(obj);
            impl->CreateOverlayView(IBinder::Probe(args->mArg1), IRect::Probe(args->mArg2));
            args->Recycle();
            break;
        }
        case DO_RELAYOUT_OVERLAY_VIEW: {
            impl->RelayoutOverlayView(IRect::Probe(obj));
            break;
        }
        case DO_REMOVE_OVERLAY_VIEW: {
            impl->RemoveOverlayView(TRUE);
            break;
        }
        case DO_REQUEST_UNBLOCK_CONTENT: {
            String str;
            ICharSequence::Probe(obj)->ToString(&str);
            impl->UnblockContent(str);
            break;
        }
        default: {
            Logger::W(TAG, "Unhandled message code: " + what);
            break;
        }
    }
    Int64 endTime;
    system->GetCurrentTimeMillis(&endTime);
    Int64 duration = endTime - startTime;
    if (duration > MESSAGE_HANDLING_DURATION_THRESHOLD_MILLIS) {
        Logger::W(TAG, String("Handling message (") + what + ") took too long time (duration="
                + duration + "ms)");
    }
    return NOERROR;
}

ECode CITvInputSessionWrapper::ReleaseResources()
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_RELEASE, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CITvInputSessionWrapper::SetMain(
    /* [in] */ Boolean isMain)
{
    AutoPtr<IMessage> msg;
    AutoPtr<IBoolean> b;
    CBoolean::New(isMain, (IBoolean**)&b);
    mCaller->ObtainMessageO(DO_SET_MAIN, b, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CITvInputSessionWrapper::SetSurface(
    /* [in] */ ISurface * surface)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessageO(DO_SET_SURFACE, surface, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CITvInputSessionWrapper::DispatchSurfaceChanged(
    /* [in] */ Int32 format,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessageIIII(DO_DISPATCH_SURFACE_CHANGED,
                format, width, height, 0, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CITvInputSessionWrapper::SetVolume(
    /* [in] */ Float volume)
{
    AutoPtr<IMessage> msg;
    AutoPtr<IFloat> f;
    CFloat::New(volume, (IFloat**)&f);
    mCaller->ObtainMessageO(DO_SET_STREAM_VOLUME, f, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CITvInputSessionWrapper::Tune(
    /* [in] */ IUri * channelUri,
    /* [in] */ IBundle * params)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessageOO(DO_TUNE, channelUri, params, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CITvInputSessionWrapper::SetCaptionEnabled(
    /* [in] */ Boolean enabled)
{
    AutoPtr<IMessage> msg;
    AutoPtr<IBoolean> b;
    CBoolean::New(enabled, (IBoolean**)&b);
    mCaller->ObtainMessageO(DO_SET_CAPTION_ENABLED, b, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CITvInputSessionWrapper::SelectTrack(
    /* [in] */ Int32 type,
    /* [in] */ const String& trackId)
{
    AutoPtr<IMessage> msg;
    AutoPtr<ICharSequence> csq;
    CString::New(trackId, (ICharSequence**)&csq);
    AutoPtr<IInteger32> i32;
    CInteger32::New(type, (IInteger32**)&i32);
    mCaller->ObtainMessageOO(DO_SELECT_TRACK, i32, csq, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CITvInputSessionWrapper::AppPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle * data)
{
    AutoPtr<IMessage> msg;
    AutoPtr<ICharSequence> csq;
    CString::New(action, (ICharSequence**)&csq);
    mCaller->ObtainMessageOO(DO_APP_PRIVATE_COMMAND, csq,
                data, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CITvInputSessionWrapper::CreateOverlayView(
    /* [in] */ IBinder * windowToken,
    /* [in] */ IRect * frame)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessageOO(DO_CREATE_OVERLAY_VIEW, windowToken,
                frame, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CITvInputSessionWrapper::RelayoutOverlayView(
    /* [in] */ IRect * frame)
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessageO(DO_RELAYOUT_OVERLAY_VIEW, frame, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CITvInputSessionWrapper::RemoveOverlayView()
{
    AutoPtr<IMessage> msg;
    mCaller->ObtainMessage(DO_REMOVE_OVERLAY_VIEW, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

ECode CITvInputSessionWrapper::RequestUnblockContent(
    /* [in] */ const String& unblockedRating)
{
    AutoPtr<IMessage> msg;
    AutoPtr<ICharSequence> csq;
    CString::New(unblockedRating, (ICharSequence**)&csq);
    mCaller->ObtainMessageO(DO_REQUEST_UNBLOCK_CONTENT, csq, (IMessage**)&msg);
    return mCaller->ExecuteOrSendMessage(msg);
}

} // namespace Tv
} // namespace Media
} // namepsace Droid
} // namespace Elastos
