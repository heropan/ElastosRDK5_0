
#include "display/OverlayDisplayAdapter.h"
#include "display/OverlayDisplayWindow.h"
#include "display/DisplayDeviceInfo.h"
#include <elastos/Slogger.h>
#include "os/Handler.h"
#include "R.h"
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Utility::Regex::IPatternHelper;
using Elastos::Utility::Regex::CPatternHelper;
using Elastos::Utility::Regex::IMatcher;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Database::IContentObserver;
using Elastos::Droid::View::CSurface;
using Elastos::Droid::View::ISurfaceHelper;
using Elastos::Droid::View::CSurfaceHelper;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Provider::ISettingsGlobal;
using Elastos::Droid::Provider::CSettingsGlobal;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

const String OverlayDisplayAdapter::TAG("OverlayDisplayAdapter");
const Boolean OverlayDisplayAdapter::DEBUG = TRUE;
const Int32 OverlayDisplayAdapter::MIN_WIDTH;
const Int32 OverlayDisplayAdapter::MIN_HEIGHT;
const Int32 OverlayDisplayAdapter::MAX_WIDTH;
const Int32 OverlayDisplayAdapter::MAX_HEIGHT;

AutoPtr<IPattern> InitPattern()
{
    AutoPtr<IPatternHelper> ph;
    CPatternHelper::AcquireSingleton((IPatternHelper**)&ph);
    AutoPtr<IPattern> pattern;
    ph->Compile(String("(\\d+)x(\\d+)/(\\d+)"), (IPattern**)&pattern);

    return pattern;
}

const AutoPtr<IPattern> OverlayDisplayAdapter::SETTING_PATTERN = InitPattern();

//===================================================================================
//          OverlayDisplayAdapter::OverlayDisplayDevice
//===================================================================================
OverlayDisplayAdapter::OverlayDisplayDevice::OverlayDisplayDevice(
    /* [in] */ OverlayDisplayAdapter* owner,
    /* [in] */ IBinder* displayToken,
    /* [in] */ const String& name,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Float refreshRate,
    /* [in] */ Int32 densityDpi,
    /* [in] */ ISurfaceTexture* surfaceTexture)
    : DisplayDevice(owner, displayToken)
    , mName(name)
    , mWidth(width)
    , mHeight(height)
    , mRefreshRate(refreshRate)
    , mDensityDpi(densityDpi)
    , mSurfaceTexture(surfaceTexture)
    , mHost(owner)
{
}

void OverlayDisplayAdapter::OverlayDisplayDevice::ClearSurfaceTextureLocked()
{
    if (mSurfaceTexture != NULL) {
        mSurfaceTexture = NULL;
    }
    mHost->SendTraversalRequestLocked();
}

void OverlayDisplayAdapter::OverlayDisplayDevice::PerformTraversalInTransactionLocked()
{
    if (mSurfaceTexture != NULL) {
        if (mSurface == NULL) {
            ASSERT_SUCCEEDED(CSurface::New(mSurfaceTexture, (ISurface**)&mSurface));
        }
        SetSurfaceInTransactionLocked(mSurface);
    }
    else {
        SetSurfaceInTransactionLocked(NULL);
        if (mSurface != NULL) {
            mSurface->Destroy();
            mSurface = NULL;
        }
    }
}

AutoPtr<DisplayDeviceInfo> OverlayDisplayAdapter::OverlayDisplayDevice::GetDisplayDeviceInfoLocked()
{
    if (mInfo == NULL) {
        mInfo = new DisplayDeviceInfo();
        mInfo->mName = mName;
        mInfo->mWidth = mWidth;
        mInfo->mHeight = mHeight;
        mInfo->mRefreshRate = mRefreshRate;
        mInfo->mDensityDpi = mDensityDpi;
        mInfo->mXDpi = mDensityDpi;
        mInfo->mYDpi = mDensityDpi;
        mInfo->mFlags = 0;
        mInfo->mType = IDisplay::TYPE_OVERLAY;
        mInfo->mTouch = DisplayDeviceInfo::TOUCH_NONE;
    }
    return mInfo;
}

//===================================================================================
//          OverlayDisplayAdapter::OverlayDisplayHandle::ShowRunnable
//===================================================================================
OverlayDisplayAdapter::OverlayDisplayHandle::ShowRunnable::ShowRunnable(
    /* [in] */ OverlayDisplayHandle* host)
    : mHost(host)
{}

ECode OverlayDisplayAdapter::OverlayDisplayHandle::ShowRunnable::Run()
{
    mHost->HandleShow();
    return NOERROR;
}

//===================================================================================
//          OverlayDisplayAdapter::OverlayDisplayHandle::DismissRunnable
//===================================================================================
OverlayDisplayAdapter::OverlayDisplayHandle::DismissRunnable::DismissRunnable(
    /* [in] */ OverlayDisplayHandle* host)
    : mHost(host)
{}

ECode OverlayDisplayAdapter::OverlayDisplayHandle::DismissRunnable::Run()
{
    mHost->HandleDismiss();
    return NOERROR;
}

//===================================================================================
//          OverlayDisplayAdapter::OverlayDisplayHandle
//===================================================================================
CAR_INTERFACE_IMPL(OverlayDisplayAdapter::OverlayDisplayHandle, IOverlayDisplayWindowListener)

OverlayDisplayAdapter::OverlayDisplayHandle::OverlayDisplayHandle(
    /* [in] */ OverlayDisplayAdapter* owner,
    /* [in] */ const String& name,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 densityDpi,
    /* [in] */ Int32 gravity)
    : mName(name)
    , mWidth(width)
    , mHeight(height)
    , mDensityDpi(densityDpi)
    , mGravity(gravity)
    , mHost(owner)
{
    mShowRunnable = new ShowRunnable(this);
    mDismissRunnable = new DismissRunnable(this);

    Boolean result;
    mHost->mUiHandler->Post(mShowRunnable, &result);
}

void OverlayDisplayAdapter::OverlayDisplayHandle::DismissLocked()
{
    mHost->mUiHandler->RemoveCallbacks(mShowRunnable);
    Boolean result;
    mHost->mUiHandler->Post(mDismissRunnable, &result);
}

ECode OverlayDisplayAdapter::OverlayDisplayHandle::OnWindowCreated(
    /* [in] */ ISurfaceTexture* surfaceTexture,
    /* [in] */ Float refreshRate)
{
    Slogger::D(TAG, "===================OverlayDisplayAdapter::OverlayDisplayHandle::OnWindowCreated()===========");
    Object::Autolock lock(mHost->GetSyncRoot());

    AutoPtr<ISurfaceHelper> surfaceHelper;
    CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&surfaceHelper);
    AutoPtr<IBinder> displayToken;
    surfaceHelper->CreateDisplay(mName, FALSE, (IBinder**)&displayToken);
    mDevice = new OverlayDisplayDevice(mHost, displayToken, mName,
        mWidth, mHeight, refreshRate, mDensityDpi, surfaceTexture);

    mHost->SendDisplayDeviceEventLocked(mDevice, DISPLAY_DEVICE_EVENT_ADDED);
    return NOERROR;
}

ECode OverlayDisplayAdapter::OverlayDisplayHandle::OnWindowDestroyed()
{
    Object::Autolock lock(mHost->GetSyncRoot());
    if (mDevice != NULL) {
        mDevice->ClearSurfaceTextureLocked();
        mHost->SendDisplayDeviceEventLocked(mDevice, DISPLAY_DEVICE_EVENT_REMOVED);
    }
    return NOERROR;
}

void OverlayDisplayAdapter::OverlayDisplayHandle::DumpLocked(
    /* [in] */ IPrintWriter* pw)
{
    pw->PrintStringln(String("  ") + mName + ":");
    pw->PrintStringln(String("    mWidth=") + StringUtils::Int32ToString(mWidth));
    pw->PrintStringln(String("    mHeight=") + StringUtils::Int32ToString(mHeight));
    pw->PrintStringln(String("    mDensityDpi=") + StringUtils::Int32ToString(mDensityDpi));
    pw->PrintStringln(String("    mGravity=") + StringUtils::Int32ToString(mGravity));

    // // Try to dump the window state.
    // if (mWindow != NULL) {
    //     final IndentingPrintWriter ipw = new IndentingPrintWriter(pw, "    ");
    //     ipw.increaseIndent();
    //     DumpUtils.dumpAsync(mUiHandler, mWindow, ipw, 200);
    // }
}

void OverlayDisplayAdapter::OverlayDisplayHandle::HandleShow()
{
    AutoPtr<OverlayDisplayWindow> window = new OverlayDisplayWindow(
        mHost->GetContext(), mName, mWidth, mHeight, mDensityDpi,
        mGravity, this);
    window->Show();
    {
        Object::Autolock lock(mHost->GetSyncRoot());
        mWindow = window;
    }
}

void OverlayDisplayAdapter::OverlayDisplayHandle::HandleDismiss()
{
    AutoPtr<OverlayDisplayWindow> window;
    {
        Object::Autolock lock(mHost->GetSyncRoot());
        window = mWindow;
        mWindow = NULL;
    }

    if (window != NULL) {
        window->Dismiss();
    }
}

//===================================================================================
//          OverlayDisplayAdapter::RegisterRunnable
//===================================================================================
OverlayDisplayAdapter::RegisterRunnable::RegisterRunnable(
    /* [in] */ OverlayDisplayAdapter* host)
    : mHost(host)
{}

ECode OverlayDisplayAdapter::RegisterRunnable::Run()
{
    AutoPtr<IContentResolver> cr;
    mHost->GetContext()->GetContentResolver((IContentResolver**)&cr);
    AutoPtr<ISettingsGlobal> sg;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sg);
    AutoPtr<IUri> uri;
    sg->GetUriForEx(ISettingsGlobal::OVERLAY_DISPLAY_DEVICES, (IUri**)&uri);
    AutoPtr<IContentObserver> co = new OverlayDisplayContentObserver(
        mHost->GetHandler(), mHost);
    cr->RegisterContentObserver(uri, TRUE, co);

    mHost->UpdateOverlayDisplayDevices();

    return NOERROR;
}

//===================================================================================
//          OverlayDisplayAdapter::OverlayDisplayContentObserver
//===================================================================================

OverlayDisplayAdapter::OverlayDisplayContentObserver::OverlayDisplayContentObserver(
    /* [in] */ IHandler* handler,
    /* [in] */ OverlayDisplayAdapter* host)
    : ContentObserver(handler)
    , mHost(host)
{}

ECode OverlayDisplayAdapter::OverlayDisplayContentObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    mHost->UpdateOverlayDisplayDevices();
    return NOERROR;
}

//===================================================================================
//          OverlayDisplayAdapter
//===================================================================================
OverlayDisplayAdapter::OverlayDisplayAdapter(
    /* [in] */ Object* syncRoot,
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler,
    /* [in] */ IDisplayAdapterListener* listener,
    /* [in] */ IHandler* uiHandler)
    : DisplayAdapter(syncRoot, context, handler, listener, String(TAG))
    , mCurrentOverlaySetting("")
{
    mUiHandler = uiHandler;
}

void OverlayDisplayAdapter::DumpLocked(
    /* [in] */ IPrintWriter* pw)
{
    DisplayAdapter::DumpLocked(pw);

    pw->PrintStringln(String("mCurrentOverlaySetting=") + mCurrentOverlaySetting);
    pw->PrintStringln(String("mOverlays: size=") + StringUtils::Int32ToString(mOverlays.GetSize()));

    List<AutoPtr<OverlayDisplayHandle> >::Iterator iter = mOverlays.Begin();
    for (; iter != mOverlays.End(); ++iter) {
        (*iter)->DumpLocked(pw);
    }
}

void OverlayDisplayAdapter::RegisterLocked()
{
    DisplayAdapter::RegisterLocked();

    AutoPtr<IRunnable> runnable = new RegisterRunnable(this);
    Boolean result;
    GetHandler()->Post(runnable, &result);
}

void OverlayDisplayAdapter::UpdateOverlayDisplayDevices()
{
    Object::Autolock lock(GetSyncRoot());

    UpdateOverlayDisplayDevicesLocked();
}

void OverlayDisplayAdapter::UpdateOverlayDisplayDevicesLocked()
{
    AutoPtr<IContext> context = GetContext();

    AutoPtr<IContentResolver> cr;
    context->GetContentResolver((IContentResolver**)&cr);
    AutoPtr<ISettingsGlobal> sg;
    CSettingsGlobal::AcquireSingleton((ISettingsGlobal**)&sg);
    String value;
    sg->GetString(cr, ISettingsGlobal::OVERLAY_DISPLAY_DEVICES, &value);
    if (value.IsNull()) {
        value = String("");
    }

    if (value.Equals(mCurrentOverlaySetting)) {
        return;
    }
    mCurrentOverlaySetting = value;

    if (!mOverlays.IsEmpty()) {
        Slogger::I(TAG, "Dismissing all overlay display devices.");

        List<AutoPtr<OverlayDisplayHandle> >::Iterator iter = mOverlays.Begin();
        for (; iter != mOverlays.End(); ++iter) {
            (*iter)->DismissLocked();
        }
        mOverlays.Clear();
    }



    AutoPtr<ArrayOf<String> > parts;
    StringUtils::Split(value, String(";"), (ArrayOf<String> **)&parts);
    if (parts != NULL) {
        AutoPtr<IResources> resources;
        context->GetResources((IResources**)&resources);
        assert(resources != NULL);

        String part;
        Int32 count = 0, width, height, densityDpi;
        Boolean matched;
        ECode ec = NOERROR;
        for (Int32 i = 0; i < parts->GetLength(); ++i) {
            part = (*parts)[i];

            AutoPtr<IMatcher> matcher;
            SETTING_PATTERN->Matcher(part, (IMatcher**)&matcher);
            matcher->Matches(&matched);
            if (matched) {
                if (count >= 4) {
                    Slogger::W(TAG, "Too many overlay display devices specified: %s", value.string());
                    break;
                }

                String strGroup;
                matcher->GroupEx(1, &strGroup);
                ec = StringUtils::ParseInt32(strGroup, 10, &width);
                if (ec == (ECode)E_NUMBER_FORMAT_EXCEPTION) continue;
                matcher->GroupEx(2, &strGroup);
                ec = StringUtils::ParseInt32(strGroup, 10, &height);
                if (ec == (ECode)E_NUMBER_FORMAT_EXCEPTION) continue;
                matcher->GroupEx(3, &strGroup);
                ec = StringUtils::ParseInt32(strGroup, 10, &densityDpi);
                if (ec == (ECode)E_NUMBER_FORMAT_EXCEPTION) continue;

                if (width >= MIN_WIDTH && width <= MAX_WIDTH
                    && height >= MIN_HEIGHT && height <= MAX_HEIGHT
                    && densityDpi >= IDisplayMetrics::DENSITY_LOW
                    && densityDpi <= IDisplayMetrics::DENSITY_XXHIGH) {
                    Int32 number = ++count;

                    String name;
                    resources->GetString(R::string::display_manager_overlay_display_name, &name);
                    Int32 gravity = ChooseOverlayGravity(number);

                    Slogger::I(TAG, "Showing overlay display device #%d: name=%s, width=%d, height=%d, densityDpi=%d",
                        number, name.string(), width, height, densityDpi);

                    AutoPtr<OverlayDisplayHandle> handle = new OverlayDisplayHandle(this, name,
                        width, height, densityDpi, gravity);
                    mOverlays.PushBack(handle);

                    continue;
                }
            }
            else if (part.IsEmpty()) {
                continue;
            }

            Slogger::W(TAG, "Malformed overlay display devices setting: %s", value.string());
        }
    }
}

Int32 OverlayDisplayAdapter::ChooseOverlayGravity(
    /* [in] */ Int32 overlayNumber)
{
    switch (overlayNumber) {
        case 1:
            return IGravity::TOP | IGravity::LEFT;
        case 2:
            return IGravity::BOTTOM | IGravity::RIGHT;
        case 3:
            return IGravity::TOP | IGravity::RIGHT;
        case 4:
        default:
            return IGravity::BOTTOM | IGravity::LEFT;
    }
}

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos
