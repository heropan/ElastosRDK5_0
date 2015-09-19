// wuweizuo automatic build .cpp file from .java file.

#include "RenderCoordinates.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                  RenderCoordinates::NormalizedPoint
//=====================================================================
Float RenderCoordinates::NormalizedPoint::GetXAbsoluteCss()
{
    // ==================before translated======================
    // return mXAbsoluteCss;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::NormalizedPoint::GetYAbsoluteCss()
{
    // ==================before translated======================
    // return mYAbsoluteCss;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::NormalizedPoint::GetXLocalDip()
{
    // ==================before translated======================
    // return (mXAbsoluteCss - mScrollXCss) * mPageScaleFactor;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::NormalizedPoint::GetYLocalDip()
{
    // ==================before translated======================
    // return (mYAbsoluteCss - mScrollYCss) * mPageScaleFactor;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::NormalizedPoint::GetXPix()
{
    // ==================before translated======================
    // return getXLocalDip() * mDeviceScaleFactor;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::NormalizedPoint::GetYPix()
{
    // ==================before translated======================
    // return getYLocalDip() * mDeviceScaleFactor + mContentOffsetYPix;
    assert(0);
    return 0.0f;
}

ECode RenderCoordinates::NormalizedPoint::SetAbsoluteCss(
    /* [in] */ Float xCss,
    /* [in] */ Float yCss)
{
    // ==================before translated======================
    // mXAbsoluteCss = xCss;
    // mYAbsoluteCss = yCss;
    assert(0);
    return NOERROR;
}

ECode RenderCoordinates::NormalizedPoint::SetLocalDip(
    /* [in] */ Float xDip,
    /* [in] */ Float yDip)
{
    // ==================before translated======================
    // setAbsoluteCss(
    //         xDip / mPageScaleFactor + mScrollXCss,
    //         yDip / mPageScaleFactor + mScrollYCss);
    assert(0);
    return NOERROR;
}

ECode RenderCoordinates::NormalizedPoint::SetScreen(
    /* [in] */ Float xPix,
    /* [in] */ Float yPix)
{
    // ==================before translated======================
    // setLocalDip(xPix / mDeviceScaleFactor, yPix / mDeviceScaleFactor);
    assert(0);
    return NOERROR;
}

RenderCoordinates::NormalizedPoint::NormalizedPoint()
{
}

//=====================================================================
//                          RenderCoordinates
//=====================================================================
ECode RenderCoordinates::Reset()
{
    // ==================before translated======================
    // mScrollXCss = mScrollYCss = 0;
    // mPageScaleFactor = 1.0f;
    assert(0);
    return NOERROR;
}

ECode RenderCoordinates::UpdateContentSizeCss(
    /* [in] */ Float contentWidthCss,
    /* [in] */ Float contentHeightCss)
{
    // ==================before translated======================
    // mContentWidthCss = contentWidthCss;
    // mContentHeightCss = contentHeightCss;
    assert(0);
    return NOERROR;
}

ECode RenderCoordinates::SetDeviceScaleFactor(
    /* [in] */ Float deviceScaleFactor)
{
    // ==================before translated======================
    // mDeviceScaleFactor = deviceScaleFactor;
    assert(0);
    return NOERROR;
}

ECode RenderCoordinates::UpdateFrameInfo(
    /* [in] */ Float scrollXCss,
    /* [in] */ Float scrollYCss,
    /* [in] */ Float contentWidthCss,
    /* [in] */ Float contentHeightCss,
    /* [in] */ Float viewportWidthCss,
    /* [in] */ Float viewportHeightCss,
    /* [in] */ Float pageScaleFactor,
    /* [in] */ Float minPageScaleFactor,
    /* [in] */ Float maxPageScaleFactor,
    /* [in] */ Float contentOffsetYPix)
{
    // ==================before translated======================
    // mScrollXCss = scrollXCss;
    // mScrollYCss = scrollYCss;
    // mPageScaleFactor = pageScaleFactor;
    // mMinPageScaleFactor = minPageScaleFactor;
    // mMaxPageScaleFactor = maxPageScaleFactor;
    // mContentOffsetYPix = contentOffsetYPix;
    //
    // updateContentSizeCss(contentWidthCss, contentHeightCss);
    // mLastFrameViewportWidthCss = viewportWidthCss;
    // mLastFrameViewportHeightCss = viewportHeightCss;
    assert(0);
    return NOERROR;
}

AutoPtr<NormalizedPoint> RenderCoordinates::CreateNormalizedPoint()
{
    // ==================before translated======================
    // return new NormalizedPoint();
    assert(0);
    AutoPtr<NormalizedPoint> empty;
    return empty;
}

Float RenderCoordinates::GetScrollX()
{
    // ==================before translated======================
    // return mScrollXCss;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::GetScrollY()
{
    // ==================before translated======================
    // return mScrollYCss;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::GetScrollXPix()
{
    // ==================before translated======================
    // return fromLocalCssToPix(mScrollXCss);
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::GetScrollYPix()
{
    // ==================before translated======================
    // return fromLocalCssToPix(mScrollYCss);
    assert(0);
    return 0.0f;
}

Int32 RenderCoordinates::GetScrollXPixInt()
{
    // ==================before translated======================
    // return (int) Math.floor(getScrollXPix());
    assert(0);
    return 0;
}

Int32 RenderCoordinates::GetScrollYPixInt()
{
    // ==================before translated======================
    // return (int) Math.floor(getScrollYPix());
    assert(0);
    return 0;
}

Float RenderCoordinates::GetContentWidthCss()
{
    // ==================before translated======================
    // return mContentWidthCss;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::GetContentHeightCss()
{
    // ==================before translated======================
    // return mContentHeightCss;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::GetContentWidthPix()
{
    // ==================before translated======================
    // return fromLocalCssToPix(mContentWidthCss);
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::GetContentHeightPix()
{
    // ==================before translated======================
    // return fromLocalCssToPix(mContentHeightCss);
    assert(0);
    return 0.0f;
}

Int32 RenderCoordinates::GetContentWidthPixInt()
{
    // ==================before translated======================
    // return (int) Math.ceil(getContentWidthPix());
    assert(0);
    return 0;
}

Int32 RenderCoordinates::GetContentHeightPixInt()
{
    // ==================before translated======================
    // return (int) Math.ceil(getContentHeightPix());
    assert(0);
    return 0;
}

Float RenderCoordinates::GetLastFrameViewportWidthCss()
{
    // ==================before translated======================
    // return mLastFrameViewportWidthCss;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::GetLastFrameViewportHeightCss()
{
    // ==================before translated======================
    // return mLastFrameViewportHeightCss;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::GetLastFrameViewportWidthPix()
{
    // ==================before translated======================
    // return fromLocalCssToPix(mLastFrameViewportWidthCss);
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::GetLastFrameViewportHeightPix()
{
    // ==================before translated======================
    // return fromLocalCssToPix(mLastFrameViewportHeightCss);
    assert(0);
    return 0.0f;
}

Int32 RenderCoordinates::GetLastFrameViewportWidthPixInt()
{
    // ==================before translated======================
    // return (int) Math.ceil(getLastFrameViewportWidthPix());
    assert(0);
    return 0;
}

Int32 RenderCoordinates::GetLastFrameViewportHeightPixInt()
{
    // ==================before translated======================
    // return (int) Math.ceil(getLastFrameViewportHeightPix());
    assert(0);
    return 0;
}

Float RenderCoordinates::GetContentOffsetYPix()
{
    // ==================before translated======================
    // return mContentOffsetYPix;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::GetPageScaleFactor()
{
    // ==================before translated======================
    // return mPageScaleFactor;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::GetMinPageScaleFactor()
{
    // ==================before translated======================
    // return mMinPageScaleFactor;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::GetMaxPageScaleFactor()
{
    // ==================before translated======================
    // return mMaxPageScaleFactor;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::GetDeviceScaleFactor()
{
    // ==================before translated======================
    // return mDeviceScaleFactor;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::GetMaxHorizontalScrollPix()
{
    // ==================before translated======================
    // return getContentWidthPix() - getLastFrameViewportWidthPix();
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::GetMaxVerticalScrollPix()
{
    // ==================before translated======================
    // return getContentHeightPix() - getLastFrameViewportHeightPix();
    assert(0);
    return 0.0f;
}

Int32 RenderCoordinates::GetMaxHorizontalScrollPixInt()
{
    // ==================before translated======================
    // return (int) Math.floor(getMaxHorizontalScrollPix());
    assert(0);
    return 0;
}

Int32 RenderCoordinates::GetMaxVerticalScrollPixInt()
{
    // ==================before translated======================
    // return (int) Math.floor(getMaxVerticalScrollPix());
    assert(0);
    return 0;
}

Float RenderCoordinates::FromPixToDip(
    /* [in] */ Float pix)
{
    // ==================before translated======================
    // return pix / mDeviceScaleFactor;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::FromDipToPix(
    /* [in] */ Float dip)
{
    // ==================before translated======================
    // return dip * mDeviceScaleFactor;
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::FromPixToLocalCss(
    /* [in] */ Float pix)
{
    // ==================before translated======================
    // return pix / (mDeviceScaleFactor * mPageScaleFactor);
    assert(0);
    return 0.0f;
}

Float RenderCoordinates::FromLocalCssToPix(
    /* [in] */ Float css)
{
    // ==================before translated======================
    // return css * mPageScaleFactor * mDeviceScaleFactor;
    assert(0);
    return 0.0f;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


