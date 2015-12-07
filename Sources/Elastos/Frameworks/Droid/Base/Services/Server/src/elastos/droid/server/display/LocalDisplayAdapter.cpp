
#include "elastos/droid/server/display/LocalDisplayAdapter.h"
#include <elastos/droid/R.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::R;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::View::CPhysicalDisplayInfo;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::ISurfaceHelper;
using Elastos::Droid::View::CSurfaceHelper;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

const String LocalDisplayAdapter::TAG("LocalDisplayAdapter");
const Int32 LocalDisplayAdapter::BUILT_IN_DISPLAY_IDS_TO_SCAN[] = {
    ISurface::BUILT_IN_DISPLAY_ID_MAIN,
    ISurface::BUILT_IN_DISPLAY_ID_HDMI,
};
const Int32 LocalDisplayAdapter::BUILT_IN_DISPLAY_IDS_COUNT;


//===================================================================================
//          LocalDisplayAdapter::LocalDisplayDevice
//===================================================================================
LocalDisplayAdapter::LocalDisplayDevice::LocalDisplayDevice(
    /* [in] */ LocalDisplayAdapter* owner,
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 builtInDisplayId,
    /* [in] */ IPhysicalDisplayInfo* phys)
    : DisplayDevice(owner, displayToken)
    , mBuiltInDisplayId(builtInDisplayId)
    , mHavePendingChanges(FALSE)
    , mBlanked(FALSE)
    , mHost(owner)
{
    ASSERT_SUCCEEDED(CPhysicalDisplayInfo::New(phys, (IPhysicalDisplayInfo**)&mPhys));
}

Boolean LocalDisplayAdapter::LocalDisplayDevice::UpdatePhysicalDisplayInfoLocked(
    /* [in] */ IPhysicalDisplayInfo* phys)
{
    Boolean isEqual;
    mPhys->Equals(phys, &isEqual);
    if (!isEqual) {
        mPhys->CopyFrom(phys);
        mHavePendingChanges = TRUE;
        return TRUE;
    }
    return FALSE;
}

void LocalDisplayAdapter::LocalDisplayDevice::ApplyPendingDisplayDeviceInfoChangesLocked()
{
    if (mHavePendingChanges) {
        mInfo = NULL;
        mHavePendingChanges = FALSE;
    }
}

AutoPtr<DisplayDeviceInfo> LocalDisplayAdapter::LocalDisplayDevice::GetDisplayDeviceInfoLocked()
{
    if (mInfo == NULL) {
        mInfo = new DisplayDeviceInfo();
        mPhys->GetWidth(&mInfo->mWidth);
        mPhys->GetHeight(&mInfo->mHeight);
        mPhys->GetRefreshRate(&mInfo->mRefreshRate);

        // Assume that all built-in displays that have secure output (eg. HDCP) also
        // support compositing from gralloc protected buffers.
        Boolean secure;
        if (mPhys->GetSecure(&secure), secure) {
            mInfo->mFlags = DisplayDeviceInfo::FLAG_SECURE
                | DisplayDeviceInfo::FLAG_SUPPORTS_PROTECTED_BUFFERS;
        }

        AutoPtr<IResources> resources;
        mHost->GetContext()->GetResources((IResources**)&resources);
        assert(resources != NULL);
        if (mBuiltInDisplayId == ISurface::BUILT_IN_DISPLAY_ID_MAIN) {
            resources->GetString(R::string::display_manager_built_in_display_name, &mInfo->mName);
            mInfo->mFlags |= DisplayDeviceInfo::FLAG_DEFAULT_DISPLAY
                | DisplayDeviceInfo::FLAG_ROTATES_WITH_CONTENT;
            mInfo->mType = IDisplay::TYPE_BUILT_IN;
            Float density;
            mPhys->GetDensity(&density);
            mInfo->mDensityDpi = (Int32)(density * 160 + 0.5f);
            mPhys->GetXDpi(&mInfo->mXDpi);
            mPhys->GetYDpi(&mInfo->mYDpi);
            mInfo->mTouch = DisplayDeviceInfo::TOUCH_INTERNAL;
        }
        else {
            mInfo->mType = IDisplay::TYPE_HDMI;
            resources->GetString(R::string::display_manager_hdmi_display_name, &mInfo->mName);
            mInfo->mTouch = DisplayDeviceInfo::TOUCH_EXTERNAL;
            Int32 width, height;
            mPhys->GetWidth(&width);
            mPhys->GetHeight(&height);
            mInfo->SetAssumedDensityForExternalDisplay(width, height);

            // For demonstration purposes, allow rotation of the external display.
            // In the future we might allow the user to configure this directly.
            AutoPtr<ISystemProperties> sysProp;
            CSystemProperties::AcquireSingleton((ISystemProperties**)&sysProp);
            String value;
            sysProp->Get(String("persist.demo.hdmirotation"), &value);
            if (value.Equals("portrait")) {
                mInfo->mRotation = ISurface::ROTATION_270;
            }
        }
    }
    return mInfo;
}

void LocalDisplayAdapter::LocalDisplayDevice::BlankLocked()
{
    mBlanked = TRUE;
    AutoPtr<ISurfaceHelper> surfaceHelper;
    CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&surfaceHelper);
    surfaceHelper->BlankDisplay(GetDisplayTokenLocked());
}

void LocalDisplayAdapter::LocalDisplayDevice::UnblankLocked()
{
    mBlanked = FALSE;
    AutoPtr<ISurfaceHelper> surfaceHelper;
    CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&surfaceHelper);
    surfaceHelper->UnblankDisplay(GetDisplayTokenLocked());
}

void LocalDisplayAdapter::LocalDisplayDevice::DumpLocked(
    /* [in] */ IPrintWriter* pw)
{
//    DisplayDevice::DumpLocked(pw);
//    pw->PrintStringln(String("mBuiltInDisplayId=") + StringUtils::Int32ToString(mBuiltInDisplayId);
//    pw->PrintStringln(String("mPhys=") + mPhys->ToString());
//    pw->PrintStringln(String("mBlanked=") + StringUtils::BooleanToString(mBlanked);
    assert(0);
}


//===================================================================================
//          LocalDisplayAdapter::HotplugDisplayEventReceiver
//===================================================================================
LocalDisplayAdapter::HotplugDisplayEventReceiver::HotplugDisplayEventReceiver(
    /* [in] */ ILooper* looper,
    /* [in] */ LocalDisplayAdapter* owner)
    : DisplayEventReceiver(looper)
    , mHost(owner)
{
}

//@Override
void LocalDisplayAdapter::HotplugDisplayEventReceiver::OnHotplug(
    /* [in] */ Int64 timestampNanos,
    /* [in] */ Int32 builtInDisplayId,
    /* [in] */ Boolean connected)
{
    AutoLock lock(mHost->GetSyncRoot());
    mHost->ScanDisplaysLocked();
}

//===================================================================================
//          LocalDisplayAdapter
//===================================================================================
// Called with SyncRoot lock held.
LocalDisplayAdapter::LocalDisplayAdapter(
    /* [in] */ Object* syncRoot,
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler,
    /* [in] */ IDisplayAdapterListener* listener)
    : DisplayAdapter(syncRoot, context, handler, listener, TAG)
    , mDevices(5)
{
    ASSERT_SUCCEEDED(CPhysicalDisplayInfo::New((IPhysicalDisplayInfo**)&mTempPhys));
}

LocalDisplayAdapter::~LocalDisplayAdapter()
{
    mDevices.Clear();
}

//@Override
void LocalDisplayAdapter::RegisterLocked()
{
    DisplayAdapter::RegisterLocked();

    AutoPtr<ILooper> looper;
    GetHandler()->GetLooper((ILooper**)&looper);
    mHotplugReceiver = new HotplugDisplayEventReceiver(looper, this);
    ScanDisplaysLocked();
}

void LocalDisplayAdapter::ScanDisplaysLocked()
{
    HashMap<Int32, AutoPtr<LocalDisplayDevice> >::Iterator it;
    for (Int32 i = 0; i < BUILT_IN_DISPLAY_IDS_COUNT; i++) {
        Int32 builtInDisplayId = BUILT_IN_DISPLAY_IDS_TO_SCAN[i];

        AutoPtr<ISurfaceHelper> surfaceHelper;
        CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&surfaceHelper);
        AutoPtr<IBinder> displayToken;
        surfaceHelper->GetBuiltInDisplay(builtInDisplayId, (IBinder**)&displayToken);
        Boolean res;
        if (displayToken != NULL && (surfaceHelper->GetDisplayInfo(
            displayToken, mTempPhys, &res), res)) {
            AutoPtr<LocalDisplayDevice> device;
            it = mDevices.Find(builtInDisplayId);
            if (it != mDevices.End()) {
                device = it->mSecond;
            }

            if (device == NULL) {
                // Display was added.
                device = new LocalDisplayDevice(this, displayToken, builtInDisplayId, mTempPhys);
                mDevices[builtInDisplayId] = device;
                SendDisplayDeviceEventLocked(device, DISPLAY_DEVICE_EVENT_ADDED);
            }
            else if (device->UpdatePhysicalDisplayInfoLocked(mTempPhys)) {
                // Display properties changed.
                SendDisplayDeviceEventLocked(device, DISPLAY_DEVICE_EVENT_CHANGED);
            }
        }
        else {
            AutoPtr<LocalDisplayDevice> device;
            HashMap<Int32, AutoPtr<LocalDisplayDevice> >::Iterator it = mDevices.Find(builtInDisplayId);
            if (it != mDevices.End()) {
                device = it->mSecond;
                // Display was removed.
                mDevices.Erase(it);
                SendDisplayDeviceEventLocked(device, DISPLAY_DEVICE_EVENT_REMOVED);
            }
        }
    }
}

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos
