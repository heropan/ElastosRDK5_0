
#include "elastos/droid/view/PhysicalDisplayInfo.h"
#include <elastos/core/StringBuffer.h>

using Elastos::Core::StringBuffer;

namespace Elastos {
namespace Droid {
namespace View {

CAR_INTERFACE_IMPL(PhysicalDisplayInfo, Object, IPhysicalDisplayInfo)

PhysicalDisplayInfo::PhysicalDisplayInfo()
    : mWidth(0)
    , mHeight(0)
    , mRefreshRate(0.f)
    , mDensity(0.f)
    , mXDpi(0.f)
    , mYDpi(0.f)
    , mSecure(FALSE)
    , mAppVsyncOffsetNanos(0LL)
    , mPresentationDeadlineNanos(0LL)
{
}

ECode PhysicalDisplayInfo::constructor()
{
    return NOERROR;
}

ECode PhysicalDisplayInfo::constructor(
    /* [in] */ IPhysicalDisplayInfo* other)
{
    return CopyFrom(other);
}

ECode PhysicalDisplayInfo::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    *hashCode = 0;
    return NOERROR;
}

ECode PhysicalDisplayInfo::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    *result = FALSE;
    AutoPtr<IPhysicalDisplayInfo> infoOther = IPhysicalDisplayInfo::Probe(other);
    if (infoOther == NULL) {
        return NOERROR;
    }

    return Equals(infoOther, result);
}

ECode PhysicalDisplayInfo::Equals(
    /* [in] */ IPhysicalDisplayInfo* other,
    /* [out] */ Boolean* result)
{
    PhysicalDisplayInfo* impOther = (PhysicalDisplayInfo*)other;
    *result = impOther != NULL
            && mWidth == impOther->mWidth
            && mHeight == impOther->mHeight
            && mRefreshRate == impOther->mRefreshRate
            && mDensity == impOther->mDensity
            && mXDpi == impOther->mXDpi
            && mYDpi == impOther->mYDpi
            && mSecure == impOther->mSecure
            && mAppVsyncOffsetNanos == impOther->mAppVsyncOffsetNanos
            && mPresentationDeadlineNanos == impOther->mPresentationDeadlineNanos;
    return NOERROR;
}

ECode PhysicalDisplayInfo::ToString(
    /* [out] */ String* info)
{
    StringBuffer buf("PhysicalDisplayInfo{");

    buf += mWidth;
    buf += " x ";
    buf += mHeight;
    buf += ", ";
    buf += mRefreshRate;
    buf += " fps, ";
    buf += "density ";
    buf += mDensity;
    buf += ", ";
    buf += mXDpi;
    buf += " x ";
    buf += mYDpi;
    buf += " dpi, secure ";
    buf += mSecure;
    buf += ", appVsyncOffset ";
    buf += mAppVsyncOffsetNanos;
    buf += ", bufferDeadline ";
    buf += mPresentationDeadlineNanos;
    buf += "}";
    return buf.ToString(info);
}

ECode PhysicalDisplayInfo::GetWidth(
    /* [out] */ Int32* width)
{
    *width = mWidth;
    return NOERROR;
}

ECode PhysicalDisplayInfo::GetHeight(
    /* [out] */ Int32* height)
{
    *height = mHeight;
    return NOERROR;
}

ECode PhysicalDisplayInfo::GetRefreshRate(
    /* [out] */ Float* refreshRate)
{
    *refreshRate = mRefreshRate;
    return NOERROR;
}

ECode PhysicalDisplayInfo::GetDensity(
    /* [out] */ Float* density)
{
    *density = mDensity;
    return NOERROR;
}

ECode PhysicalDisplayInfo::GetXDpi(
    /* [out] */ Float* xDpi)
{
    *xDpi = mXDpi;
    return NOERROR;
}

ECode PhysicalDisplayInfo::GetYDpi(
    /* [out] */ Float* yDpi)
{
    *yDpi = mYDpi;
    return NOERROR;
}

ECode PhysicalDisplayInfo::GetSecure(
    /* [out] */ Boolean* secure)
{
    *secure = mSecure;
    return NOERROR;
}

ECode PhysicalDisplayInfo::GetAppVsyncOffsetNanos(
    /* [out] */ Int64* nanos)
{
    *nanos = mAppVsyncOffsetNanos;
    return NOERROR;
}

ECode PhysicalDisplayInfo::GetPresentationDeadlineNanos(
    /* [out] */ Int64* nanos)
{
    *nanos = mPresentationDeadlineNanos;
    return NOERROR;
}

ECode PhysicalDisplayInfo::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;
    return NOERROR;
}

ECode PhysicalDisplayInfo::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;
    return NOERROR;
}

ECode PhysicalDisplayInfo::SetRefreshRate(
    /* [in] */ Float refreshRate)
{
    mRefreshRate = refreshRate;
    return NOERROR;
}

ECode PhysicalDisplayInfo::SetDensity(
    /* [in] */ Float density)
{
    mDensity = density;
    return NOERROR;
}

ECode PhysicalDisplayInfo::SetXDpi(
    /* [in] */ Float xDpi)
{
    mXDpi = xDpi;
    return NOERROR;
}

ECode PhysicalDisplayInfo::SetYDpi(
    /* [in] */ Float yDpi)
{
    mYDpi = yDpi;
    return NOERROR;
}

ECode PhysicalDisplayInfo::SetSecure(
    /* [in] */ Boolean secure)
{
    mSecure = secure;
    return NOERROR;
}

ECode PhysicalDisplayInfo::SetAppVsyncOffsetNanos(
    /* [in] */ Int64 nanos)
{
    mAppVsyncOffsetNanos = nanos;
    return NOERROR;
}

ECode PhysicalDisplayInfo::SetPresentationDeadlineNanos(
    /* [in] */ Int64 nanos)
{
    mPresentationDeadlineNanos = nanos;
    return NOERROR;
}

ECode PhysicalDisplayInfo::CopyFrom(
    /* [in] */ IPhysicalDisplayInfo* other)
{
    PhysicalDisplayInfo* impOther = (PhysicalDisplayInfo*)other;
    if (impOther == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mWidth = impOther->mWidth;
    mHeight = impOther->mHeight;
    mRefreshRate = impOther->mRefreshRate;
    mDensity = impOther->mDensity;
    mXDpi = impOther->mXDpi;
    mYDpi = impOther->mYDpi;
    mSecure = impOther->mSecure;
    mAppVsyncOffsetNanos = impOther->mAppVsyncOffsetNanos;
    mPresentationDeadlineNanos = impOther->mPresentationDeadlineNanos;
    return NOERROR;
}

} // view
} // droid
} // elastos
