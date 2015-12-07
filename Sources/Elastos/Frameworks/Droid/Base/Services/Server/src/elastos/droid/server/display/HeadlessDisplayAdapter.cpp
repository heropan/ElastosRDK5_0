
#include "elastos/droid/server/display/HeadlessDisplayAdapter.h"
#include "elastos/droid/R.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::R;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::View::IDisplay;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

const String HeadlessDisplayAdapter::TAG("HeadlessDisplayAdapter");


//===================================================================================
//          HeadlessDisplayAdapter::HeadlessDisplayDevice
//===================================================================================
HeadlessDisplayAdapter::HeadlessDisplayDevice::HeadlessDisplayDevice(
    /* [in] */ HeadlessDisplayAdapter* owner)
    : DisplayDevice(owner, NULL)
    , mOwner(owner)
{
}

//@Override
AutoPtr<DisplayDeviceInfo> HeadlessDisplayAdapter::HeadlessDisplayDevice::GetDisplayDeviceInfoLocked()
{
    if (mInfo == NULL) {
        mInfo = new DisplayDeviceInfo();
        AutoPtr<IResources> resources;
        mOwner->GetContext()->GetResources((IResources**)&resources);
        assert(resources != NULL);
        resources->GetString(R::string::display_manager_built_in_display_name, &mInfo->mName);
        mInfo->mWidth = 640;
        mInfo->mHeight = 480;
        mInfo->mRefreshRate = 60;
        mInfo->mDensityDpi = IDisplayMetrics::DENSITY_DEFAULT;
        mInfo->mXDpi = 160;
        mInfo->mYDpi = 160;
        mInfo->mFlags = DisplayDeviceInfo::FLAG_DEFAULT_DISPLAY
            | DisplayDeviceInfo::FLAG_SECURE
            | DisplayDeviceInfo::FLAG_SUPPORTS_PROTECTED_BUFFERS;
        mInfo->mType = IDisplay::TYPE_BUILT_IN;
        mInfo->mTouch = DisplayDeviceInfo::TOUCH_NONE;
    }
    return mInfo;
}

//===================================================================================
//          HeadlessDisplayAdapter
//===================================================================================
HeadlessDisplayAdapter::HeadlessDisplayAdapter(
    /* [in] */ Object* syncRoot,
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler,
    /* [in] */ IDisplayAdapterListener* listener)
    : DisplayAdapter(syncRoot, context, handler, listener, TAG)
{
}

void HeadlessDisplayAdapter::RegisterLocked()
{
    Slogger::D(TAG, "===================HeadlessDisplayAdapter::RegisterLocked()===========");
    DisplayAdapter::RegisterLocked();
    AutoPtr<HeadlessDisplayDevice> device = new HeadlessDisplayDevice(this);
    SendDisplayDeviceEventLocked(device, DISPLAY_DEVICE_EVENT_ADDED);
}

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos
