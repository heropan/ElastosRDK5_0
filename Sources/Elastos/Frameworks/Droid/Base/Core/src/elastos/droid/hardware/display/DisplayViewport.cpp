
#include "elastos/droid/hardware/display/DisplayViewport.h"
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>
//#include "elastos/droid/graphics/CRect.h"

//using Elastos::Droid::Graphics::CRect;
using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Display {

CAR_INTERFACE_IMPL(DisplayViewport, Object, IDisplayViewport)

DisplayViewport::DisplayViewport()
{
    assert(0 && "TODO CRect::New");
    //CRect::New((IRect**)&mLogicalFrame);
    //CRect::New((IRect**)&mPysicalFrame);
}

ECode DisplayViewport::CopyFrom(
    /* [in] */ IDisplayViewport* viewport)
{
    mValid = ((DisplayViewport*)viewport)->mValid;
    mDisplayId = ((DisplayViewport*)viewport)->mDisplayId;
    mOrientation = ((DisplayViewport*)viewport)->mOrientation;
    mLogicalFrame->Set(((DisplayViewport*)viewport)->mLogicalFrame);
    mPhysicalFrame->Set(((DisplayViewport*)viewport)->mPhysicalFrame);
    mDeviceWidth = ((DisplayViewport*)viewport)->mDeviceWidth;
    mDeviceHeight = ((DisplayViewport*)viewport)->mDeviceHeight;
    return NOERROR;
}

ECode DisplayViewport::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    StringBuilder sb;
    String tmp;
    sb += "DisplayViewport{valid=";
    sb += StringUtils::BooleanToString(mValid);;
    sb += ", displayId=";
    sb += StringUtils::ToString(mDisplayId);
    sb += ", orientation=";
    sb += StringUtils::ToString(mOrientation);
    sb += ", logicalFrame=";
    IObject::Probe(mLogicalFrame)->ToString(&tmp);
    sb += tmp;
    sb += ", physicalFrame=";
    IObject::Probe(mPhysicalFrame)->ToString(&tmp);
    sb += tmp;
    sb += ", deviceWidth=";
    sb += StringUtils::ToString(mDeviceWidth);
    sb += ", deviceHeight=";
    sb += StringUtils::ToString(mDeviceHeight);
    sb += "}";

    return sb.ToString(result);
}

} // namespace Display
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos