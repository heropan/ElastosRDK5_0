
#include "wm/FakeWindowImpl.h"
#include "os/Process.h"

using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::IInputChannelHelper;
using Elastos::Droid::View::CInputChannelHelper;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

FakeWindowImpl::FakeWindowImpl(
    /* [in] */ CWindowManagerService* service,
    /* [in] */ ILooper* looper,
    /* [in] */ IInputEventReceiverFactory* inputEventReceiverFactory,
    /* [in] */ const String& name,
    /* [in] */ Int32 windowType,
    /* [in] */ Int32 layoutParamsFlags,
    /* [in] */ Boolean canReceiveKeys,
    /* [in] */ Boolean hasFocus,
    /* [in] */ Boolean touchFullscreen)
    : mService(service)
{
    AutoPtr<IInputChannelHelper> helper;
    ASSERT_SUCCEEDED(CInputChannelHelper::AcquireSingleton(
            (IInputChannelHelper**)&helper));
    helper->OpenInputChannelPair(name,
            (IInputChannel**)&mServerChannel, (IInputChannel**)&mClientChannel);
    mService->mInputManager->RegisterInputChannel(mServerChannel, NULL);

    inputEventReceiverFactory->CreateInputEventReceiver(
            mClientChannel, looper, (IInputEventReceiver**)&mInputEventReceiver);

    mApplicationHandle = new InputApplicationHandle(NULL);
    mApplicationHandle->mName = name;
    mApplicationHandle->mDispatchingTimeoutNanos =
            CWindowManagerService::DEFAULT_INPUT_DISPATCHING_TIMEOUT_NANOS;

    mWindowHandle = new InputWindowHandle(mApplicationHandle, NULL,
            IDisplay::DEFAULT_DISPLAY);
    mWindowHandle->mName = name;
    mWindowHandle->mInputChannel = mServerChannel;
    mWindowLayer = GetLayerLw(windowType);
    mWindowHandle->mLayer = mWindowLayer;
    mWindowHandle->mLayoutParamsFlags = layoutParamsFlags;
    mWindowHandle->mLayoutParamsType = windowType;
    mWindowHandle->mDispatchingTimeoutNanos =
            CWindowManagerService::DEFAULT_INPUT_DISPATCHING_TIMEOUT_NANOS;
    mWindowHandle->mVisible = TRUE;
    mWindowHandle->mCanReceiveKeys = canReceiveKeys;
    mWindowHandle->mHasFocus = hasFocus;
    mWindowHandle->mHasWallpaper = FALSE;
    mWindowHandle->mPaused = FALSE;
    mWindowHandle->mOwnerPid = Process::MyPid();
    mWindowHandle->mOwnerUid = Process::MyUid();
    mWindowHandle->mInputFeatures = 0;
    mWindowHandle->mScaleFactor = 1.0f;

    mTouchFullscreen = touchFullscreen;
}

PInterface FakeWindowImpl::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IFakeWindow*)this;
    }
    else if (riid == Elastos::Droid::View::EIID_IFakeWindow) {
        return (IFakeWindow*)this;
    }
    return NULL;
}

UInt32 FakeWindowImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 FakeWindowImpl::Release()
{
    return ElRefBase::Release();
}

ECode FakeWindowImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

void FakeWindowImpl::Layout(
    /* [in] */ Int32 dw,
    /* [in] */ Int32 dh)
{
    if (mTouchFullscreen) {
        Boolean res;
        mWindowHandle->mTouchableRegion->Set(0, 0, dw, dh, &res);
    }
    else {
        mWindowHandle->mTouchableRegion->SetEmpty();
    }
    mWindowHandle->mFrameLeft = 0;
    mWindowHandle->mFrameTop = 0;
    mWindowHandle->mFrameRight = dw;
    mWindowHandle->mFrameBottom = dh;
}

ECode FakeWindowImpl::Dismiss()
{
    AutoLock lock(mService->mWindowMapLock);
    if (mService->RemoveFakeWindowLocked((IFakeWindow*)this)) {
        mInputEventReceiver->Dispose();
        mService->mInputManager->UnregisterInputChannel(mServerChannel);
        mClientChannel->Dispose();
        mServerChannel->Dispose();
    }
    return NOERROR;
}

Int32 FakeWindowImpl::GetLayerLw(
    /* [in] */ Int32 windowType)
{
    Int32 result;
    mService->mPolicy->WindowTypeToLayerLw(windowType, &result);
    return result * CWindowManagerService::TYPE_LAYER_MULTIPLIER
            + CWindowManagerService::TYPE_LAYER_OFFSET;
}


} // Wm
} // Server
} // Droid
} // Elastos
