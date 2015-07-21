
#include "hardware/display/DisplayManagerGlobal.h"
#include "hardware/display/CWifiDisplayStatus.h"
#include "hardware/display/CDisplayManagerCallback.h"
#include "os/ServiceManager.h"
#include "os/Looper.h"
#include "view/Display.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::EIID_IBinder;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Display {

const String DisplayManagerGlobal::TAG("DisplayManagerGlobal");
const Boolean DisplayManagerGlobal::DEBUG;
const Boolean DisplayManagerGlobal::USE_CACHE;

AutoPtr<DisplayManagerGlobal> DisplayManagerGlobal::sInstance;
Mutex DisplayManagerGlobal::sInstanceLock;

DisplayManagerGlobal::DisplayListenerDelegate::DisplayListenerDelegate(
    /* [in] */ IDisplayListener* listener,
    /* [in] */ ILooper* looper)
    : HandlerBase(looper)
    , mListener(listener)
{
}

void DisplayManagerGlobal::DisplayListenerDelegate::SendDisplayEvent(
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 event)
{
    AutoPtr<IMessage> msg;
    ObtainMessage(event, displayId, 0, (IMessage**)&msg);
    Boolean result;
    SendMessage(msg, &result);
}

void DisplayManagerGlobal::DisplayListenerDelegate::ClearEvents()
{
    RemoveCallbacksAndMessages(NULL);
}

ECode DisplayManagerGlobal::DisplayListenerDelegate::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, displayId;
    msg->GetWhat(&what);
    msg->GetArg1(&displayId);

    switch (what) {
        case DisplayManagerGlobal::EVENT_DISPLAY_ADDED:
            mListener->OnDisplayAdded(displayId);
            break;
        case DisplayManagerGlobal::EVENT_DISPLAY_CHANGED:
            mListener->OnDisplayChanged(displayId);
            break;
        case DisplayManagerGlobal::EVENT_DISPLAY_REMOVED:
            mListener->OnDisplayRemoved(displayId);
            break;
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL(DisplayManagerGlobal, IDisplayManagerGlobal);

DisplayManagerGlobal::DisplayManagerGlobal(
    /* [in] */ IIDisplayManager* dm)
{
    mDm = dm;
}

DisplayManagerGlobal::~DisplayManagerGlobal()
{
    mDisplayListeners.Clear();
}

AutoPtr<DisplayManagerGlobal> DisplayManagerGlobal::GetInstance()
{
    AutoLock lock(sInstanceLock);

    if (sInstance == NULL) {
        AutoPtr<IInterface> service = ServiceManager::GetService(IContext::DISPLAY_SERVICE);
        if (service != NULL) {
            AutoPtr<IIDisplayManager> idm = IIDisplayManager::Probe(service);
            sInstance = new DisplayManagerGlobal(idm);
        }
        else {
            Logger::E(TAG, "GetInstance() failed to get service %s", IContext::DISPLAY_SERVICE.string());
        }
    }

    return sInstance;
}

ECode DisplayManagerGlobal::GetDisplayInfo(
    /* [in] */ Int32 displayId,
    /* [out] */ IDisplayInfo** displayInfo)
{
    VALIDATE_NOT_NULL(displayInfo);

    //try {
    AutoLock lock(mLock);

    AutoPtr<IDisplayInfo> info;
    if (USE_CACHE) {
        HashMap<Int32, AutoPtr<IDisplayInfo> >::Iterator find
            = mDisplayInfoCache.Find(displayId);
        if (find != mDisplayInfoCache.End()) {
            *displayInfo = find->mSecond;
            REFCOUNT_ADD(*displayInfo);
            return NOERROR;
        }
    }

    mDm->GetDisplayInfo(displayId, displayInfo);
    if (*displayInfo == NULL) {
        return NOERROR;
    }

    if (USE_CACHE) {
        mDisplayInfoCache[displayId] = *displayInfo;
    }

    RegisterCallbackIfNeededLocked();

    if (DEBUG) {
        Logger::D(TAG, "getDisplayInfo: displayId=%d, info", displayId/*, info*/);
    }

    return NOERROR;

    // } catch (RemoteException ex) {
    //     Log.e(TAG, "Could not get display information from display manager.", ex);
    //     return NULL;
    // }
}

ECode DisplayManagerGlobal::GetDisplayIds(
    /* [out, calee] */ ArrayOf<Int32>** displayIds)
{
    VALIDATE_NOT_NULL(displayIds);

    //try {
    AutoLock lock(mLock);

    if (USE_CACHE) {
        if (mDisplayIdCache != NULL) {
            *displayIds = mDisplayIdCache;
            REFCOUNT_ADD(*displayIds);
            return NOERROR;
        }
    }

    if (FAILED(mDm->GetDisplayIds(displayIds))) {
        *displayIds = ArrayOf<Int32>::Alloc(1);
        REFCOUNT_ADD(*displayIds);
        (**displayIds)[0] = IDisplay::DEFAULT_DISPLAY;
        return NOERROR;
    }

    if (USE_CACHE) {
        mDisplayIdCache = *displayIds;
    }
    RegisterCallbackIfNeededLocked();

    return NOERROR;

    // } catch (RemoteException ex) {
    //     Log.e(TAG, "Could not get display ids from display manager.", ex);
    //     return new Int32[] { Display.DEFAULT_DISPLAY };
    // }
}

ECode DisplayManagerGlobal::GetCompatibleDisplay(
    /* [in] */ Int32 displayId,
    /* [in] */ ICompatibilityInfoHolder* cih,
    /* [out] */ IDisplay** display)
{
    VALIDATE_NOT_NULL(display);

    AutoPtr<IDisplayInfo> displayInfo;
    GetDisplayInfo(displayId, (IDisplayInfo**)&displayInfo);
    if (displayInfo == NULL) {
        *display = NULL;
        return NOERROR;
    }

    *display = new View::Display(this, displayId, displayInfo, cih);
    if (*display) {
        REFCOUNT_ADD(*display);
        return NOERROR;
    }
    else {
        return E_OUT_OF_MEMORY_ERROR;
    }
}

ECode DisplayManagerGlobal::GetRealDisplay(
    /* [in] */ Int32 displayId,
    /* [out] */ IDisplay** display)
{
    return GetCompatibleDisplay(displayId, NULL, display);
}

ECode DisplayManagerGlobal::RegisterDisplayListener(
    /* [in] */ IDisplayListener* listener,
    /* [in] */ IHandler* handler)
{
    if (listener == NULL) {
        Logger::E(TAG, "listener must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ILooper> looper;
    if (NULL == handler) {
        looper = Looper::GetMyLooper();
    }
    else {
        handler->GetLooper((ILooper**)&looper);
    }

    AutoLock lock(mLock);

    List<AutoPtr<DisplayListenerDelegate> >::Iterator iter = FindDisplayListenerLocked(listener);
    if (iter == mDisplayListeners.End()) {
        AutoPtr<DisplayListenerDelegate> delegate = new DisplayListenerDelegate(listener, looper);
        mDisplayListeners.PushBack(delegate);
        RegisterCallbackIfNeededLocked();
    }

    return NOERROR;
}

ECode DisplayManagerGlobal::UnregisterDisplayListener(
    /* [in] */ IDisplayListener* listener)
{
    if (listener == NULL) {
        Logger::E(TAG, "listener must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoLock lock(mLock);

    List<AutoPtr<DisplayListenerDelegate> >::Iterator iter = FindDisplayListenerLocked(listener);
    if (iter != mDisplayListeners.End()) {
        AutoPtr<DisplayListenerDelegate> d = *iter;
        d->ClearEvents();
        mDisplayListeners.Erase(iter);
    }

    return NOERROR;
}

List<AutoPtr<DisplayManagerGlobal::DisplayListenerDelegate> >::Iterator
DisplayManagerGlobal::FindDisplayListenerLocked(
    /* [in] */ IDisplayListener* listener)
{
    List<AutoPtr<DisplayManagerGlobal::DisplayListenerDelegate> >::Iterator iter = mDisplayListeners.Begin();
    for (; iter != mDisplayListeners.End(); ++iter) {
        if ((*iter)->mListener.Get() == listener)
            return iter;
    }

    return mDisplayListeners.End();
}

void DisplayManagerGlobal::RegisterCallbackIfNeededLocked()
{
    if (mCallback == NULL) {
        CDisplayManagerCallback::New((Handle32)this, (IDisplayManagerCallback**)&mCallback);
        if (FAILED(mDm->RegisterCallback(mCallback))) {
            Logger::E(TAG, "Failed to register callback with display manager service.");
            mCallback = NULL;
        }
    }
}

void DisplayManagerGlobal::HandleDisplayEvent(
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 event)
{
    AutoLock lock(mLock);

    if (USE_CACHE) {
        mDisplayInfoCache.Erase(displayId);

        if (event == EVENT_DISPLAY_ADDED || event == EVENT_DISPLAY_REMOVED) {
            mDisplayIdCache = NULL;
        }
    }

    List<AutoPtr<DisplayListenerDelegate> >::Iterator iter = mDisplayListeners.Begin();
    for (; iter != mDisplayListeners.End(); ++iter) {
        (*iter)->SendDisplayEvent(displayId, event);
    }
}

ECode DisplayManagerGlobal::ScanWifiDisplays()
{
    if (FAILED(mDm->ScanWifiDisplays())) {
        Logger::E(TAG, "Failed to scan for Wifi displays.");
    }

    return NOERROR;
}

ECode DisplayManagerGlobal::ConnectWifiDisplay(
    /* [in] */ const String& deviceAddress)
{
    if (deviceAddress.IsNull()) {
        Logger::E(TAG, "deviceAddress must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (FAILED(mDm->ConnectWifiDisplay(deviceAddress))) {
        Logger::E(TAG, "Failed to connect to Wifi display.%s", deviceAddress.string());
    }

    return NOERROR;
}

ECode DisplayManagerGlobal::DisconnectWifiDisplay()
{
    if (FAILED(mDm->DisconnectWifiDisplay())) {
        Logger::E(TAG, "Failed to disconnect from Wifi display.");
    }

    return NOERROR;

}

ECode DisplayManagerGlobal::RenameWifiDisplay(
    /* [in] */ const String& deviceAddress,
    /* [in] */ const String& alias)
{
    if (deviceAddress.IsNull()) {
        Logger::E(TAG, "deviceAddress must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (FAILED(mDm->RenameWifiDisplay(deviceAddress, alias))) {
        Logger::E(TAG, "Failed to rename Wifi display %s with alias %s.",
            deviceAddress.string(), alias.string());
    }

    return NOERROR;
}

ECode DisplayManagerGlobal::ForgetWifiDisplay(
    /* [in] */ const String& deviceAddress)
{
    if (deviceAddress.IsNull()) {
        Logger::E(TAG, "deviceAddress must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (FAILED(mDm->ForgetWifiDisplay(deviceAddress))) {
        Logger::E(TAG, "Failed to forget Wifi display.");
    }

    return NOERROR;
}

ECode DisplayManagerGlobal::GetWifiDisplayStatus(
    /* [out] */ IWifiDisplayStatus** status)
{
    VALIDATE_NOT_NULL(status);
    *status = NULL;

    if (FAILED(mDm->GetWifiDisplayStatus(status))) {
        Logger::E(TAG, "Failed to get Wifi display status.");
        ASSERT_SUCCEEDED(CWifiDisplayStatus::New(status));
    }

    return NOERROR;
}

} // namespace Display
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos
