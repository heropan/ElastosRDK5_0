// wuweizuo automatic build .cpp file from .java file.

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
    /*
    return mXAbsoluteCss;
    */
    return 0.0f;
}

Float RenderCoordinates::NormalizedPoint::GetYAbsoluteCss()
{
    /*
    return mYAbsoluteCss;
    */
    return 0.0f;
}

Float RenderCoordinates::NormalizedPoint::GetXLocalDip()
{
    /*
    return (mXAbsoluteCss - mScrollXCss) * mPageScaleFactor;
    */
    return 0.0f;
}

Float RenderCoordinates::NormalizedPoint::GetYLocalDip()
{
    /*
    return (mYAbsoluteCss - mScrollYCss) * mPageScaleFactor;
    */
    return 0.0f;
}

Float RenderCoordinates::NormalizedPoint::GetXPix()
{
    /*
    return getXLocalDip() * mDeviceScaleFactor;
    */
    return 0.0f;
}

Float RenderCoordinates::NormalizedPoint::GetYPix()
{
    /*
    return getYLocalDip() * mDeviceScaleFactor + mContentOffsetYPix;
    */
    return 0.0f;
}

ECode RenderCoordinates::NormalizedPoint::SetAbsoluteCss(
    /* [in] */ Float xCss,
    /* [in] */ Float yCss)
{
    /*
    mXAbsoluteCss = xCss;
    mYAbsoluteCss = yCss;
    */
    return NOERROR;
}

ECode RenderCoordinates::NormalizedPoint::SetLocalDip(
    /* [in] */ Float xDip,
    /* [in] */ Float yDip)
{
    /*
    setAbsoluteCss(
            xDip / mPageScaleFactor + mScrollXCss,
            yDip / mPageScaleFactor + mScrollYCss);
    */
    return NOERROR;
}

ECode RenderCoordinates::NormalizedPoint::SetScreen(
    /* [in] */ Float xPix,
    /* [in] */ Float yPix)
{
    /*
    setLocalDip(xPix / mDeviceScaleFactor, yPix / mDeviceScaleFactor);
    */
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
    /*
    mScrollXCss = mScrollYCss = 0;
    mPageScaleFactor = 1.0f;
    */
    return NOERROR;
}

ECode RenderCoordinates::UpdateContentSizeCss(
    /* [in] */ Float contentWidthCss,
    /* [in] */ Float contentHeightCss)
{
    /*
    mContentWidthCss = contentWidthCss;
    mContentHeightCss = contentHeightCss;
    */
    return NOERROR;
}

ECode RenderCoordinates::SetDeviceScaleFactor(
    /* [in] */ Float deviceScaleFactor)
{
    /*
    mDeviceScaleFactor = deviceScaleFactor;
    */
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
    /*
    mScrollXCss = scrollXCss;
    mScrollYCss = scrollYCss;
    mPageScaleFactor = pageScaleFactor;
    mMinPageScaleFactor = minPageScaleFactor;
    mMaxPageScaleFactor = maxPageScaleFactor;
    mContentOffsetYPix = contentOffsetYPix;


    updateContentSizeCss(contentWidthCss, contentHeightCss);
    mLastFrameViewportWidthCss = viewportWidthCss;
    mLastFrameViewportHeightCss = viewportHeightCss;
    */
    return NOERROR;
}

AutoPtr<NormalizedPoint> RenderCoordinates::CreateNormalizedPoint()
{
    /*
    return new NormalizedPoint();
    */
    return AutoPtr<NormalizedPoint>(this);
}

Float RenderCoordinates::GetScrollX()
{
    /*
    return mScrollXCss;
    */
    return 0.0f;
}

Float RenderCoordinates::GetScrollY()
{
    /*
    return mScrollYCss;
    */
    return 0.0f;
}

Float RenderCoordinates::GetScrollXPix()
{
    /*
    return fromLocalCssToPix(mScrollXCss);
    */
    return 0.0f;
}

Float RenderCoordinates::GetScrollYPix()
{
    /*
    return fromLocalCssToPix(mScrollYCss);
    */
    return 0.0f;
}

Int32 RenderCoordinates::GetScrollXPixInt()
{
    /*
    return (int) Math.floor(getScrollXPix());
    */
    return 0;
}

Int32 RenderCoordinates::GetScrollYPixInt()
{
    /*
    return (int) Math.floor(getScrollYPix());
    */
    return 0;
}

Float RenderCoordinates::GetContentWidthCss()
{
    /*
    return mContentWidthCss;
    */
    return 0.0f;
}

Float RenderCoordinates::GetContentHeightCss()
{
    /*
    return mContentHeightCss;
    */
    return 0.0f;
}

Float RenderCoordinates::GetContentWidthPix()
{
    /*
    return fromLocalCssToPix(mContentWidthCss);
    */
    return 0.0f;
}

Float RenderCoordinates::GetContentHeightPix()
{
    /*
    return fromLocalCssToPix(mContentHeightCss);
    */
    return 0.0f;
}

Int32 RenderCoordinates::GetContentWidthPixInt()
{
    /*
    return (int) Math.ceil(getContentWidthPix());
    */
    return 0;
}

Int32 RenderCoordinates::GetContentHeightPixInt()
{
    /*
    return (int) Math.ceil(getContentHeightPix());
    */
    return 0;
}

Float RenderCoordinates::GetLastFrameViewportWidthCss()
{
    /*
    return mLastFrameViewportWidthCss;
    */
    return 0.0f;
}

Float RenderCoordinates::GetLastFrameViewportHeightCss()
{
    /*
    return mLastFrameViewportHeightCss;
    */
    return 0.0f;
}

Float RenderCoordinates::GetLastFrameViewportWidthPix()
{
    /*
    return fromLocalCssToPix(mLastFrameViewportWidthCss);
    */
    return 0.0f;
}

Float RenderCoordinates::GetLastFrameViewportHeightPix()
{
    /*
    return fromLocalCssToPix(mLastFrameViewportHeightCss);
    */
    return 0.0f;
}

Int32 RenderCoordinates::GetLastFrameViewportWidthPixInt()
{
    /*
    return (int) Math.ceil(getLastFrameViewportWidthPix());
    */
    return 0;
}

Int32 RenderCoordinates::GetLastFrameViewportHeightPixInt()
{
    /*
    return (int) Math.ceil(getLastFrameViewportHeightPix());
    */
    return 0;
}

Float RenderCoordinates::GetContentOffsetYPix()
{
    /*
    return mContentOffsetYPix;
    */
    return 0.0f;
}

Float RenderCoordinates::GetPageScaleFactor()
{
    /*
    return mPageScaleFactor;
    */
    return 0.0f;
}

Float RenderCoordinates::GetMinPageScaleFactor()
{
    /*
    return mMinPageScaleFactor;
    */
    return 0.0f;
}

Float RenderCoordinates::GetMaxPageScaleFactor()
{
    /*
    return mMaxPageScaleFactor;
    */
    return 0.0f;
}

Float RenderCoordinates::GetDeviceScaleFactor()
{
    /*
    return mDeviceScaleFactor;
    */
    return 0.0f;
}

Float RenderCoordinates::GetMaxHorizontalScrollPix()
{
    /*
    return getContentWidthPix() - getLastFrameViewportWidthPix();
    */
    return 0.0f;
}

Float RenderCoordinates::GetMaxVerticalScrollPix()
{
    /*
    return getContentHeightPix() - getLastFrameViewportHeightPix();
    */
    return 0.0f;
}

Int32 RenderCoordinates::GetMaxHorizontalScrollPixInt()
{
    /*
    return (int) Math.floor(getMaxHorizontalScrollPix());
    */
    return 0;
}

Int32 RenderCoordinates::GetMaxVerticalScrollPixInt()
{
    /*
    return (int) Math.floor(getMaxVerticalScrollPix());
    */
    return 0;
}

Float RenderCoordinates::FromPixToDip(
    /* [in] */ Float pix)
{
    /*
    return pix / mDeviceScaleFactor;
    */
    return 0.0f;
}

Float RenderCoordinates::FromDipToPix(
    /* [in] */ Float dip)
{
    /*
    return dip * mDeviceScaleFactor;
    */
    return 0.0f;
}

Float RenderCoordinates::FromPixToLocalCss(
    /* [in] */ Float pix)
{
    /*
    return pix / (mDeviceScaleFactor * mPageScaleFactor);
    */
    return 0.0f;
}

Float RenderCoordinates::FromLocalCssToPix(
    /* [in] */ Float css)
{
    /*
    return css * mPageScaleFactor * mDeviceScaleFactor;
    */
    return 0.0f;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


