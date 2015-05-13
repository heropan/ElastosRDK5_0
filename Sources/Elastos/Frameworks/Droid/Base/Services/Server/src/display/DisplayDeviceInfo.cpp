
#include "display/DisplayDeviceInfo.h"
#include <elastos/Math.h>
#include <elastos/StringBuilder.h>
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Droid::View::ISurface;
using Elastos::Droid::Utility::IDisplayMetrics;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

const Int32 DisplayDeviceInfo::FLAG_DEFAULT_DISPLAY;
const Int32 DisplayDeviceInfo::FLAG_ROTATES_WITH_CONTENT;
const Int32 DisplayDeviceInfo::FLAG_SECURE;
const Int32 DisplayDeviceInfo::FLAG_SUPPORTS_PROTECTED_BUFFERS;
const Int32 DisplayDeviceInfo::TOUCH_NONE;
const Int32 DisplayDeviceInfo::TOUCH_INTERNAL;
const Int32 DisplayDeviceInfo::TOUCH_EXTERNAL;

DisplayDeviceInfo::DisplayDeviceInfo()
    : mWidth(0)
    , mHeight(0)
    , mRefreshRate(0)
    , mDensityDpi(0)
    , mXDpi(0)
    , mYDpi(0)
    , mFlags(0)
    , mTouch(0)
    , mRotation(ISurface::ROTATION_0)
    , mType(0)
{}

void DisplayDeviceInfo::SetAssumedDensityForExternalDisplay(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mDensityDpi = Elastos::Core::Math::Min(width, height) * IDisplayMetrics::DENSITY_XHIGH / 1080;
    // Technically, these values should be smaller than the apparent density
    // but we don't know the physical size of the display.
    mXDpi = mDensityDpi;
    mYDpi = mDensityDpi;
}

Boolean DisplayDeviceInfo::Equals(
    /* [in] */ DisplayDeviceInfo* other)
{
    return other != NULL
        && ((mName.IsNull() && other->mName.IsNull()) || mName.Equals(other->mName))
        && mWidth == other->mWidth
        && mHeight == other->mHeight
        && mRefreshRate == other->mRefreshRate
        && mDensityDpi == other->mDensityDpi
        && mXDpi == other->mXDpi
        && mYDpi == other->mYDpi
        && mFlags == other->mFlags
        && mTouch == other->mTouch
        && mRotation == other->mRotation
        && mType == other->mType
        && ((mAddress.IsNull() && other->mAddress.IsNull()) || mAddress.Equals(other->mAddress));
}

//@Override
Int32 DisplayDeviceInfo::GetHashCode()
{
    return 0; // don't care
}

void DisplayDeviceInfo::CopyFrom(
    /* [in] */ DisplayDeviceInfo* other)
{
    mName = other->mName;
    mWidth = other->mWidth;
    mHeight = other->mHeight;
    mRefreshRate = other->mRefreshRate;
    mDensityDpi = other->mDensityDpi;
    mXDpi = other->mXDpi;
    mYDpi = other->mYDpi;
    mFlags = other->mFlags;
    mTouch = other->mTouch;
    mRotation = other->mRotation;
    mType = other->mType;
    mAddress = other->mAddress;
}

//@Override
String DisplayDeviceInfo::ToString()
{
    StringBuilder sb("DisplayDeviceInfo{\"");
    sb += mName;
    sb += "\": ";
    sb += mWidth;
    sb += " x ";
    sb += mHeight;
    sb += ", ";
    sb += mRefreshRate;
    sb += " fps, ";
    sb += "density ";
    sb += mDensityDpi;
    sb += ", ";
    sb += mXDpi;
    sb += " x ";
    sb += mYDpi;
    sb += " dpi, touch ";
    sb += TouchToString(mTouch);
    sb += FlagsToString(mFlags);
    sb += ", rotation ";
    sb += mRotation;
    sb += ", type ";
    sb += mType;//Display.typeToString(type)
    sb += ", address ";
    sb += mAddress;
    sb += "}";

    return sb.ToString();
}

String DisplayDeviceInfo::TouchToString(
    /* [in] */ Int32 touch)
{
    switch (touch) {
        case TOUCH_NONE:
            return String("NONE");
        case TOUCH_INTERNAL:
            return String("INTERNAL");
        case TOUCH_EXTERNAL:
            return String("EXTERNAL");
        default:
            return StringUtils::Int32ToString(touch);
    }
}

String DisplayDeviceInfo::FlagsToString(
    /* [in] */ Int32 flags)
{
    StringBuilder msg;
    if ((flags & FLAG_DEFAULT_DISPLAY) != 0) {
        msg += (", FLAG_DEFAULT_DISPLAY");
    }
    if ((flags & FLAG_ROTATES_WITH_CONTENT) != 0) {
        msg += (", FLAG_ROTATES_WITH_CONTENT");
    }
    if ((flags & FLAG_SECURE) != 0) {
        msg += (", FLAG_SECURE");
    }
    if ((flags & FLAG_SUPPORTS_PROTECTED_BUFFERS) != 0) {
        msg += (", FLAG_SUPPORTS_PROTECTED_BUFFERS");
    }
    return msg.ToString();
}

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos
