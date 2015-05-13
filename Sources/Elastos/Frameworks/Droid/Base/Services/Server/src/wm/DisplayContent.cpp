
#include "wm/DisplayContent.h"

using Elastos::Droid::View::CDisplayInfo;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

CAR_INTERFACE_IMPL(DisplayContent, IInterface)

DisplayContent::DisplayContent(
    /* [in] */ IDisplay* display)
    : mInitialDisplayWidth(0)
    , mInitialDisplayHeight(0)
    , mInitialDisplayDensity(0)
    , mBaseDisplayWidth(0)
    , mBaseDisplayHeight(0)
    , mBaseDisplayDensity(0)
    , mLayoutNeeded(FALSE)
    , mPendingLayoutChanges(0)
    , mIsDefaultDisplay(FALSE)
    , mDisplay(display)
{
    CDisplayInfo::New((IDisplayInfo**)&mDisplayInfo);

    display->GetDisplayId(&mDisplayId);
    Boolean result;
    display->GetDisplayInfo(mDisplayInfo, &result);
    mIsDefaultDisplay = mDisplayId == IDisplay::DEFAULT_DISPLAY;
}

Int32 DisplayContent::GetDisplayId()
{
    return mDisplayId;
}

List< AutoPtr<WindowState> >& DisplayContent::GetWindowList()
{
    return mWindows;
}

AutoPtr<IDisplay> DisplayContent::GetDisplay()
{
    return mDisplay;
}

AutoPtr<IDisplayInfo> DisplayContent::GetDisplayInfo()
{
    return mDisplayInfo;
}

void DisplayContent::UpdateDisplayInfo()
{
    Boolean result;
    mDisplay->GetDisplayInfo(mDisplayInfo, &result);
}


} // Wm
} // Server
} // Droid
} // Elastos
