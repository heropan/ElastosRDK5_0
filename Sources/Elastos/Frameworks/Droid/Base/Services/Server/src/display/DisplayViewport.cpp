
#include "display/DisplayViewport.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Droid::Graphics::CRect;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

// 256b8877-ddec-4d20-a4fd-5b95b047b6fc
extern "C" const InterfaceID EIID_DisplayViewport =
    { 0x256b8877, 0xddec, 0x4d20, { 0xa4, 0xfd, 0x5b, 0x95, 0xb0, 0x47, 0xb6, 0xfc } };

// CAR_INTERFACE_IMPL_WITH_CPP_CAST(DisplayViewport, IDisplayViewport, DisplayViewport)

PInterface DisplayViewport::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IDisplayViewport *)this;
    }
    if (riid == EIID_IDisplayViewport) {
        return (IInterface*)(IDisplayViewport *)this;
    }
    if (riid == EIID_DisplayViewport ) {
        return reinterpret_cast<PInterface>(this);
    }
    return NULL;
}
UInt32 DisplayViewport::AddRef()
{
    return ElRefBase::AddRef();
}
UInt32 DisplayViewport::Release()
{
    return ElRefBase::Release();
}
ECode DisplayViewport::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IDisplayViewport *)this) {
        *iid = EIID_IDisplayViewport;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

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

ECode DisplayViewport::CopyFrom(
    /* [in] */ IDisplayViewport* vp)
{
    assert(vp != NULL);

    AutoPtr<DisplayViewport> viewport = reinterpret_cast<DisplayViewport*>(vp->Probe(EIID_DisplayViewport));
    return CopyFrom(viewport);
}

ECode DisplayViewport::CopyFrom(
    /* [in] */ DisplayViewport* viewport)
{
    assert(viewport != NULL);
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
    sb.AppendCStr("DisplayViewport{valid=");
    sb.AppendBoolean(mValid);
    sb.AppendCStr(", displayId=");
    sb.AppendInt32(mDisplayId);
    sb.AppendCStr(", orientation=");
    sb.AppendInt32(mOrientation);
    // + ", logicalFrame=" + logicalFrame
    // + ", physicalFrame=" + physicalFrame
    sb.AppendCStr(", deviceWidth=");
    sb.AppendInt32(mDeviceWidth);
    sb.AppendCStr(", deviceHeight=");
    sb.AppendInt32(mDeviceHeight);
    sb.AppendCStr("}");

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
