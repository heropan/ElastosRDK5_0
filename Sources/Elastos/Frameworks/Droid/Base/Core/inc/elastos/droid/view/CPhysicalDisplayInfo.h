
#ifndef __ELASTOS_DROID_VIEW_CPHYSICALDISPLAYINFO_H__
#define  __ELASTOS_DROID_VIEW_CPHYSICALDISPLAYINFO_H__

#include "_Elastos_Droid_View_CPhysicalDisplayInfo.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CPhysicalDisplayInfo)
{
public:
    CPhysicalDisplayInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IPhysicalDisplayInfo* other);

    ECode Equals(
        /* [in] */ IPhysicalDisplayInfo* other,
        /* [out] */ Boolean* equals);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean * result);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);

    ECode CopyFrom(
        /* [in] */ IPhysicalDisplayInfo* other);

    // // For debugging purposes
    // @Override
    // public String toString() {
    //     return "PhysicalDisplayInfo{" + width + " x " + height + ", " + refreshRate + " fps, "
    //             + "density " + density + ", " + xDpi + " x " + yDpi + " dpi, secure " + secure
    //             + "}";
    // }

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI SetWidth(
        /* [in] */ Int32 width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI SetHeight(
        /* [in] */ Int32 height);

    CARAPI GetRefreshRate(
        /* [out] */ Float* refreshRate);

    CARAPI SetRefreshRate(
        /* [in] */ Float refreshRate);

    CARAPI GetDensity(
        /* [out] */ Float* density);

    CARAPI SetDensity(
        /* [in] */ Float density);

    CARAPI GetXDpi(
        /* [out] */ Float* xDpi);

    CARAPI SetXDpi(
        /* [in] */ Float xDpi);

    CARAPI GetYDpi(
        /* [out] */ Float* yDpi);

    CARAPI SetYDpi(
        /* [in] */ Float yDpi);

    CARAPI GetSecure(
        /* [out] */ Boolean* secure);

    CARAPI SetSecure(
        /* [in] */ Boolean secure);

public:
    Int32 mWidth;
    Int32 mHeight;
    Float mRefreshRate;
    Float mDensity;
    Float mXDpi;
    Float mYDpi;
    Boolean mSecure;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_VIEW_CPHYSICALDISPLAYINFO_H__
