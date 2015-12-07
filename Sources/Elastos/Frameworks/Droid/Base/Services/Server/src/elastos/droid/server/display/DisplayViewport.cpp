
#include "elastos/droid/server/display/DisplayViewport.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Droid::Graphics::CRect;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

CAR_INTERFACE_IMPL(DisplayViewport, IDisplayViewport, DisplayViewport)

DisplayViewport::DisplayViewport()
    : mValid(FALSE)
    , mDisplayId(0)
    , mOrientation(0)
    , mDeviceWidth(0)
    , mDeviceHeight(0)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mLogicalFrame));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mPhysicalFrame));
}

DisplayViewport::~DisplayViewport()
{}

ECode DisplayViewport::CopyFrom(
    /* [in] */ IDisplayViewport* vp)
{
    VALIDATE_NOT_NULL(vp)

    AutoPtr<DisplayViewport> viewport = (DisplayViewport*)vp;
    return CopyFrom(viewport);
}

ECode DisplayViewport::CopyFrom(
    /* [in] */ DisplayViewport* viewport)
{
    VALIDATE_NOT_NULL(viewport)
    mValid = viewport->mValid;
    mDisplayId = viewport->mDisplayId;
    mOrientation = viewport->mOrientation;
    mLogicalFrame->Set(viewport->mLogicalFrame);
    mPhysicalFrame->Set(viewport->mPhysicalFrame);
    mDeviceWidth = viewport->mDeviceWidth;
    mDeviceHeight = viewport->mDeviceHeight;
    return NOERROR;
}

ECode DisplayViewport::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuilder sb;
    sb.Append("DisplayViewport{valid=");
    sb.AppendBoolean(mValid);
    sb.Append(", displayId=");
    sb.Append(mDisplayId);
    sb.Append(", orientation=");
    sb.Append(mOrientation);
    // + ", logicalFrame=" + logicalFrame
    // + ", physicalFrame=" + physicalFrame
    sb.Append(", deviceWidth=");
    sb.Append(mDeviceWidth);
    sb.Append(", deviceHeight=");
    sb.Append(mDeviceHeight);
    sb.Append("}");

    *str = sb.ToString();
    return NOERROR;
}

ECode DisplayViewport::SetValid(
    /* [in] */ Boolean valid)
{
    mValid = valid;
    return NOERROR;
}

ECode DisplayViewport::IsValid(
    /* [out] */ Boolean* valid)
{
    VALIDATE_NOT_NULL(valid);
    *valid = mValid;
    return NOERROR;
}

ECode DisplayViewport::SetOrientation(
    /* [in] */ Int32 ori)
{
    mOrientation = ori;
    return NOERROR;
}

ECode DisplayViewport::GetOrientation(
    /* [out] */ Int32* ori)
{
    VALIDATE_NOT_NULL(ori);
    *ori = mOrientation;
    return NOERROR;
}

ECode DisplayViewport::SetLogicalFrame(
    /* [in] */ IRect* frame)
{
    mLogicalFrame = frame;
    return NOERROR;
}

ECode DisplayViewport::GetLogicalFrame(
    /* [out] */ IRect** frame)
{
    VALIDATE_NOT_NULL(frame);
    *frame = mLogicalFrame;
    REFCOUNT_ADD(*frame);
    return NOERROR;
}

ECode DisplayViewport::SetPhysicalFrame(
    /* [in] */ IRect* frame)
{
    mPhysicalFrame = frame;
    return NOERROR;
}

ECode DisplayViewport::GetPhysicalFrame(
    /* [out] */ IRect** frame)
{
    VALIDATE_NOT_NULL(frame);
    *frame = mPhysicalFrame;
    REFCOUNT_ADD(*frame);
    return NOERROR;
}

ECode DisplayViewport::SetDeviceWidth(
    /* [in] */ Int32 w)
{
    mDeviceWidth = w;
    return NOERROR;
}

ECode DisplayViewport::GetDeviceWidth(
    /* [out] */ Int32* w)
{
    VALIDATE_NOT_NULL(w);
    *w = mDeviceWidth;
    return NOERROR;
}

ECode DisplayViewport::SetDeviceHeight(
    /* [in] */ Int32 h)
{
    mDeviceHeight = h;
    return NOERROR;
}

ECode DisplayViewport::GetDeviceHeight(
    /* [out] */ Int32* h)
{
    VALIDATE_NOT_NULL(h);
    *h = mDeviceHeight;
    return NOERROR;
}

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos
