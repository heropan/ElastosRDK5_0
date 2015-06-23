
#include "view/CPhysicalDisplayInfo.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace View {

CPhysicalDisplayInfo::CPhysicalDisplayInfo()
    : mWidth(0)
    , mHeight(0)
    , mRefreshRate(0.0f)
    , mDensity(0.0f)
    , mXDpi(0.0f)
    , mYDpi(0.0f)
    , mSecure(FALSE)
{
}

ECode CPhysicalDisplayInfo::constructor()
{
    return NOERROR;
}

ECode CPhysicalDisplayInfo::constructor(
    /* [in] */ IPhysicalDisplayInfo* other)
{
    return CopyFrom(other);
}

// ECode CPhysicalDisplayInfo::Equals(
//     /* [in] */ IInterface* o,
//     /* [out] */ Boolean* equals)
// {
//     assert(equals != NULL);
//     if (IPhysicalDisplayInfo::Probe(o) != NULL) {
//         return Equals(IPhysicalDisplayInfo::Probe(o), equals);
//     }

//     *equals = FALSE;
//     return NOERROR;
// }

ECode CPhysicalDisplayInfo::Equals(
    /* [in] */ IPhysicalDisplayInfo* otherObj,
    /* [out] */ Boolean* equals)
{
    VALIDATE_NOT_NULL(equals);

    CPhysicalDisplayInfo* other = (CPhysicalDisplayInfo*)otherObj;
    *equals = other != NULL
        && mWidth == other->mWidth
        && mHeight == other->mHeight
        && mRefreshRate == other->mRefreshRate
        && mDensity == other->mDensity
        && mXDpi == other->mXDpi
        && mYDpi == other->mYDpi
        && mSecure == other->mSecure;

    return NOERROR;
}

ECode CPhysicalDisplayInfo::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(other);
    return Equals(IPhysicalDisplayInfo::Probe(other), result);
}

ECode CPhysicalDisplayInfo::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = 0; // don't care
    return NOERROR;
}

ECode CPhysicalDisplayInfo::CopyFrom(
    /* [in] */ IPhysicalDisplayInfo* otherObj)
{
    CPhysicalDisplayInfo* other = (CPhysicalDisplayInfo*)otherObj;
    mWidth = other->mWidth;
    mHeight = other->mHeight;
    mRefreshRate = other->mRefreshRate;
    mDensity = other->mDensity;
    mXDpi = other->mXDpi;
    mYDpi = other->mYDpi;
    mSecure = other->mSecure;

    return NOERROR;
}

ECode CPhysicalDisplayInfo::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = mWidth;

    return NOERROR;
}

ECode CPhysicalDisplayInfo::SetWidth(
    /* [in] */ Int32 width)
{
    mWidth = width;
    return NOERROR;
}

ECode CPhysicalDisplayInfo::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = mHeight;

    return NOERROR;
}

ECode CPhysicalDisplayInfo::SetHeight(
    /* [in] */ Int32 height)
{
    mHeight = height;
    return NOERROR;
}

ECode CPhysicalDisplayInfo::GetRefreshRate(
    /* [out] */ Float* refreshRate)
{
    VALIDATE_NOT_NULL(refreshRate);
    *refreshRate = mRefreshRate;

    return NOERROR;
}

ECode CPhysicalDisplayInfo::SetRefreshRate(
    /* [in] */ Float refreshRate)
{
    mRefreshRate = refreshRate;
    return NOERROR;
}

ECode CPhysicalDisplayInfo::GetDensity(
    /* [out] */ Float* density)
{
    VALIDATE_NOT_NULL(density);
    *density = mDensity;

    return NOERROR;
}

ECode CPhysicalDisplayInfo::SetDensity(
    /* [in] */ Float density)
{
    mDensity = density;
    return NOERROR;
}

ECode CPhysicalDisplayInfo::GetXDpi(
    /* [out] */ Float* xDpi)
{
    VALIDATE_NOT_NULL(xDpi);
    *xDpi = mXDpi;

    return NOERROR;
}

ECode CPhysicalDisplayInfo::SetXDpi(
    /* [in] */ Float xDpi)
{
    mXDpi = xDpi;
    return NOERROR;
}

ECode CPhysicalDisplayInfo::GetYDpi(
    /* [out] */ Float* yDpi)
{
    VALIDATE_NOT_NULL(yDpi);
    *yDpi = mYDpi;

    return NOERROR;
}

ECode CPhysicalDisplayInfo::SetYDpi(
    /* [in] */ Float yDpi)
{
    mYDpi = yDpi;
    return NOERROR;
}

ECode CPhysicalDisplayInfo::GetSecure(
    /* [out] */ Boolean* secure)
{
    VALIDATE_NOT_NULL(secure);
    *secure = mSecure;

    return NOERROR;
}

ECode CPhysicalDisplayInfo::SetSecure(
    /* [in] */ Boolean secure)
{
    mSecure = secure;
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
