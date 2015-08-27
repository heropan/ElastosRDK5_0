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
}

Float RenderCoordinates::NormalizedPoint::GetYAbsoluteCss()
{
    /*
    return mYAbsoluteCss;
    */
}

Float RenderCoordinates::NormalizedPoint::GetXLocalDip()
{
    /*
    return (mXAbsoluteCss - mScrollXCss) * mPageScaleFactor;
    */
}

Float RenderCoordinates::NormalizedPoint::GetYLocalDip()
{
    /*
    return (mYAbsoluteCss - mScrollYCss) * mPageScaleFactor;
    */
}

Float RenderCoordinates::NormalizedPoint::GetXPix()
{
    /*
    return getXLocalDip() * mDeviceScaleFactor;
    */
}

Float RenderCoordinates::NormalizedPoint::GetYPix()
{
    /*
    return getYLocalDip() * mDeviceScaleFactor + mContentOffsetYPix;
    */
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
}

Float RenderCoordinates::GetScrollX()
{
    /*
    return mScrollXCss;
    */
}

Float RenderCoordinates::GetScrollY()
{
    /*
    return mScrollYCss;
    */
}

Float RenderCoordinates::GetScrollXPix()
{
    /*
    return fromLocalCssToPix(mScrollXCss);
    */
}

Float RenderCoordinates::GetScrollYPix()
{
    /*
    return fromLocalCssToPix(mScrollYCss);
    */
}

Int32 RenderCoordinates::GetScrollXPixInt()
{
    /*
    return (int) Math.floor(getScrollXPix());
    */
}

Int32 RenderCoordinates::GetScrollYPixInt()
{
    /*
    return (int) Math.floor(getScrollYPix());
    */
}

Float RenderCoordinates::GetContentWidthCss()
{
    /*
    return mContentWidthCss;
    */
}

Float RenderCoordinates::GetContentHeightCss()
{
    /*
    return mContentHeightCss;
    */
}

Float RenderCoordinates::GetContentWidthPix()
{
    /*
    return fromLocalCssToPix(mContentWidthCss);
    */
}

Float RenderCoordinates::GetContentHeightPix()
{
    /*
    return fromLocalCssToPix(mContentHeightCss);
    */
}

Int32 RenderCoordinates::GetContentWidthPixInt()
{
    /*
    return (int) Math.ceil(getContentWidthPix());
    */
}

Int32 RenderCoordinates::GetContentHeightPixInt()
{
    /*
    return (int) Math.ceil(getContentHeightPix());
    */
}

Float RenderCoordinates::GetLastFrameViewportWidthCss()
{
    /*
    return mLastFrameViewportWidthCss;
    */
}

Float RenderCoordinates::GetLastFrameViewportHeightCss()
{
    /*
    return mLastFrameViewportHeightCss;
    */
}

Float RenderCoordinates::GetLastFrameViewportWidthPix()
{
    /*
    return fromLocalCssToPix(mLastFrameViewportWidthCss);
    */
}

Float RenderCoordinates::GetLastFrameViewportHeightPix()
{
    /*
    return fromLocalCssToPix(mLastFrameViewportHeightCss);
    */
}

Int32 RenderCoordinates::GetLastFrameViewportWidthPixInt()
{
    /*
    return (int) Math.ceil(getLastFrameViewportWidthPix());
    */
}

Int32 RenderCoordinates::GetLastFrameViewportHeightPixInt()
{
    /*
    return (int) Math.ceil(getLastFrameViewportHeightPix());
    */
}

Float RenderCoordinates::GetContentOffsetYPix()
{
    /*
    return mContentOffsetYPix;
    */
}

Float RenderCoordinates::GetPageScaleFactor()
{
    /*
    return mPageScaleFactor;
    */
}

Float RenderCoordinates::GetMinPageScaleFactor()
{
    /*
    return mMinPageScaleFactor;
    */
}

Float RenderCoordinates::GetMaxPageScaleFactor()
{
    /*
    return mMaxPageScaleFactor;
    */
}

Float RenderCoordinates::GetDeviceScaleFactor()
{
    /*
    return mDeviceScaleFactor;
    */
}

Float RenderCoordinates::GetMaxHorizontalScrollPix()
{
    /*
    return getContentWidthPix() - getLastFrameViewportWidthPix();
    */
}

Float RenderCoordinates::GetMaxVerticalScrollPix()
{
    /*
    return getContentHeightPix() - getLastFrameViewportHeightPix();
    */
}

Int32 RenderCoordinates::GetMaxHorizontalScrollPixInt()
{
    /*
    return (int) Math.floor(getMaxHorizontalScrollPix());
    */
}

Int32 RenderCoordinates::GetMaxVerticalScrollPixInt()
{
    /*
    return (int) Math.floor(getMaxVerticalScrollPix());
    */
}

Float RenderCoordinates::FromPixToDip(
    /* [in] */ Float pix)
{
    /*
    return pix / mDeviceScaleFactor;
    */
}

Float RenderCoordinates::FromDipToPix(
    /* [in] */ Float dip)
{
    /*
    return dip * mDeviceScaleFactor;
    */
}

Float RenderCoordinates::FromPixToLocalCss(
    /* [in] */ Float pix)
{
    /*
    return pix / (mDeviceScaleFactor * mPageScaleFactor);
    */
}

Float RenderCoordinates::FromLocalCssToPix(
    /* [in] */ Float css)
{
    /*
    return css * mPageScaleFactor * mDeviceScaleFactor;
    */
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


