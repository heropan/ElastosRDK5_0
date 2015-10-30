
#ifndef __ELASTOS_DROID_WEBKIT_ZOOMMANAGER_H__
#define __ELASTOS_DROID_WEBKIT_ZOOMMANAGER_H__

#include "elastos/droid/ext/frameworkext.h"

#include "elastos/droid/webkit/ZoomControlBase.h"
#include "elastos/droid/webkit/ZoomControlEmbedded.h"
#include "elastos/droid/webkit/ZoomControlExternal.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::View::IOnScaleGestureListener;
using Elastos::Droid::View::IScaleGestureDetector;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CallbackProxy;
class CWebViewClassic;
class CWebViewCoreDrawData;
class CWebViewCoreViewState;

/**
 * The ZoomManager is responsible for maintaining the WebView's current zoom
 * level state.  It is also responsible for managing the on-screen zoom controls
 * as well as any animation of the WebView due to zooming.
 *
 * Currently, there are two methods for animating the zoom of a WebView.
 *
 * (1) The first method is triggered by startZoomAnimation(...) and is a fixed
 * length animation where the const zoom scale is known at startup.  This type of
 * animation notifies webkit of the const scale BEFORE it animates. The animation
 * is then done by scaling the CANVAS incrementally based on a stepping function.
 *
 * (2) The second method is triggered by a multi-touch pinch and the new scale
 * is determined dynamically based on the user's gesture. This type of animation
 * only notifies webkit of new scale AFTER the gesture is complete. The animation
 * effect is achieved by scaling the VIEWS (both WebView and ViewManager.ChildView)
 * to the new scale in response to events related to the user's gesture.
 */
class ZoomManager : public Object
{
private:
    class FocusMovementQueue : public Object
    {
        friend class ZoomManager;
    public:
        FocusMovementQueue(
            /* [in] */ ZoomManager* owner);

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        CARAPI_(void) Clear();

        CARAPI_(void) Add(
            /* [in] */ Float focusDelta);

        CARAPI_(Float) GetSum();

    private:
        static const Int32 QUEUE_CAPACITY = 5;
        AutoPtr< ArrayOf<Float> > mQueue;
        Float mSum;
        Int32 mSize;
        Int32 mIndex;
        ZoomManager* mOwner;
    };

    class ScaleDetectorListener
        : public Object
        , public IOnScaleGestureListener
    {
    public:
        ScaleDetectorListener(
            /* [in] */ ZoomManager* owner);

        CAR_INTERFACE_DECL();

        CARAPI OnScaleBegin(
            /* [in] */ IScaleGestureDetector* detector,
            /* [out] */ Boolean* res);

        // If the user moves the fingers but keeps the same distance between them,
        // we should do panning only.
        CARAPI_(Boolean) IsPanningOnly(
            /* [in] */ IScaleGestureDetector* detector);

        CARAPI_(Boolean) HandleScale(
            /* [in] */ IScaleGestureDetector* detector);

        CARAPI OnScale(
            /* [in] */ IScaleGestureDetector* detector,
            /* [out] */ Boolean* res);

        CARAPI OnScaleEnd(
            /* [in] */ IScaleGestureDetector* detector);

        CARAPI ToString(
            /* [out] */ String* info);

    private:
        Float mAccumulatedSpan;
        ZoomManager* mOwner;
    };

    class PostScale : public Runnable
    {
    public:
        PostScale(
            /* [in] */ Boolean updateTextWrap,
            /* [in] */ Boolean inZoomOverview,
            /* [in] */ Boolean inPortraitMode,
            /* [in] */ ZoomManager* owner);

        CARAPI Run();

        CARAPI ToString(
            /* [out] */ String* info);

    public:
        Boolean mUpdateTextWrap;
        // Remember the zoom overview state right after rotation since
        // it could be changed between the time this callback is initiated and
        // the time it's actually run.
        Boolean mInZoomOverviewBeforeSizeChange;
        Boolean mInPortraitMode;

    private:
        ZoomManager* mOwner;
    };

public:
    ZoomManager(
        /* [in] */ CWebViewClassic* webView,
        /* [in] */ CallbackProxy* callbackProxy);

    /**
     * Initialize both the default and actual zoom scale to the given density.
     *
     * @param density The logical density of the display. This is a scaling factor
     * for the Density Independent Pixel unit, where one DIP is one pixel on an
     * approximately 160 dpi screen (see android.util.DisplayMetrics.density).
     */
    CARAPI_(void) Init(
        /* [in] */ Float density);

    /**
     * Update the default zoom scale using the given density. It will also reset
     * the current min and max zoom scales to the default boundaries as well as
     * ensure that the actual scale falls within those boundaries.
     *
     * @param density The logical density of the display. This is a scaling factor
     * for the Density Independent Pixel unit, where one DIP is one pixel on an
     * approximately 160 dpi screen (see android.util.DisplayMetrics.density).
     */
    CARAPI_(void) UpdateDefaultZoomDensity(
        /* [in] */ Float density);

    CARAPI_(Float) GetScale();

    CARAPI_(Float) GetInvScale();

    CARAPI_(Float) GetTextWrapScale();

    CARAPI_(Float) GetMaxZoomScale();

    CARAPI_(Float) GetMinZoomScale();

    CARAPI_(Float) GetDefaultScale();

    /**
     * Returns the zoom scale used for reading text on a double-tap.
     */
    CARAPI_(Float) GetReadingLevelScale();

    /* package */
    CARAPI_(Float) ComputeReadingLevelScale(
        /* [in] */ Float scale);

    CARAPI_(Float) GetInvDefaultScale();

    CARAPI_(Float) GetDefaultMaxZoomScale();

    CARAPI_(Float) GetDefaultMinZoomScale();

    CARAPI_(Int32) GetDocumentAnchorX();

    CARAPI_(Int32) GetDocumentAnchorY();

    CARAPI_(void) ClearDocumentAnchor();

    CARAPI_(void) SetZoomCenter(
        /* [in] */ Float x,
        /* [in] */ Float y);

    CARAPI_(void) SetInitialScaleInPercent(
        /* [in] */ Int32 scaleInPercent);

    CARAPI_(Float) ComputeScaleWithLimits(
        /* [in] */ Float scale);

    CARAPI_(Boolean) IsScaleOverLimits(
        /* [in] */ Float scale);

    CARAPI_(Boolean) IsZoomScaleFixed();

    static CARAPI_(Boolean) ExceedsMinScaleIncrement(
        /* [in] */ Float scaleA,
        /* [in] */ Float scaleB);

    CARAPI_(Boolean) WillScaleTriggerZoom(
        /* [in] */ Float scale);

    CARAPI_(Boolean) CanZoomIn();

    CARAPI_(Boolean) CanZoomOut();

    CARAPI_(Boolean) ZoomIn();

    CARAPI_(Boolean) ZoomOut();

    /**
     * Initiates an animated zoom of the WebView.
     *
     * @return true if the new scale triggered an animation and false otherwise.
     */
    CARAPI_(Boolean) StartZoomAnimation(
        /* [in] */ Float scale,
        /* [in] */ Boolean reflowText);

    /**
     * This method is called by the WebView's drawing code when a fixed length zoom
     * animation is occurring. Its purpose is to animate the zooming of the canvas
     * to the desired scale which was specified in startZoomAnimation(...).
     *
     * A fixed length animation begins when startZoomAnimation(...) is called and
     * continues until the ZOOM_ANIMATION_LENGTH time has elapsed. During that
     * interval each time the WebView draws it calls this function which is
     * responsible for generating the animation.
     *
     * Additionally, the WebView can check to see if such an animation is currently
     * in progress by calling isFixedLengthAnimationInProgress().
     */
    CARAPI_(void) AnimateZoom(
        /* [in] */ ICanvas* canvas);

    CARAPI_(Boolean) IsZoomAnimating();

    CARAPI_(Boolean) IsFixedLengthAnimationInProgress();

    CARAPI_(void) UpdateDoubleTapZoom(
        /* [in] */ Int32 doubleTapZoom);

    CARAPI_(void) RefreshZoomScale(
        /* [in] */ Boolean reflowText);

    CARAPI_(void) SetZoomScale(
        /* [in] */ Float scale,
        /* [in] */ Boolean reflowText);

    CARAPI_(Boolean) IsDoubleTapEnabled();

    /**
     * The double tap gesture can result in different behaviors depending on the
     * content that is tapped.
     *
     * (1) PLUGINS: If the taps occur on a plugin then we maximize the plugin on
     * the screen. If the plugin is already maximized then zoom the user into
     * overview mode.
     *
     * (2) HTML/OTHER: If the taps occur outside a plugin then the following
     * heuristic is used.
     *   A. If the current text wrap scale differs from newly calculated and the
     *      layout algorithm specifies the use of NARROW_COLUMNS, then fit to
     *      column by reflowing the text.
     *   B. If the page is not in overview mode then change to overview mode.
     *   C. If the page is in overmode then change to the default scale.
     */
    CARAPI_(void) HandleDoubleTap(
        /* [in] */ Float lastTouchX,
        /* [in] */ Float lastTouchY);

    /* package */
    CARAPI_(Float) GetZoomOverviewScale();

    CARAPI_(Boolean) IsInZoomOverview();

    CARAPI_(void) UpdateMultiTouchSupport(
        /* [in] */ IContext* context);

    CARAPI_(Boolean) SupportsMultiTouchZoom();

    CARAPI_(Boolean) SupportsPanDuringZoom();

    /**
     * Notifies the caller that the ZoomManager is requesting that scale related
     * updates should not be sent to webkit. This can occur in cases where the
     * ZoomManager is performing an animation and does not want webkit to update
     * until the animation is complete.
     *
     * @return true if scale related updates should not be sent to webkit and
     *         false otherwise.
     */
    CARAPI_(Boolean) IsPreventingWebkitUpdates();

    CARAPI_(AutoPtr<IScaleGestureDetector>) GetScaleGestureDetector();

    CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 ow,
        /* [in] */ Int32 oh);

    CARAPI_(void) UpdateZoomRange(
        /* [in] */ CWebViewCoreViewState* viewState,
        /* [in] */ Int32 viewWidth,
        /* [in] */ Int32 minPrefWidth);

    /**
     * Updates zoom values when Webkit produces a new picture. This method
     * should only be called from the UI thread's message handler.
     *
     * @return True if zoom value has changed
     */
    CARAPI_(Boolean) OnNewPicture(
        /* [in] */ CWebViewCoreDrawData* drawData);

    /**
     * Updates zoom values after Webkit completes the initial page layout. It
     * is called when visiting a page for the first time as well as when the
     * user navigates back to a page (in which case we may need to restore the
     * zoom levels to the state they were when you left the page). This method
     * should only be called from the UI thread's message handler.
     */
    CARAPI_(void) OnFirstLayout(
        /* [in] */ CWebViewCoreDrawData* drawData);

    CARAPI_(void) SaveZoomState(
        /* [in] */ IBundle* b);

    CARAPI_(void) RestoreZoomState(
        /* [in] */ IBundle* b);

    CARAPI_(void) InvokeZoomPicker();

    CARAPI_(void) DismissZoomPicker();

    CARAPI_(Boolean) IsZoomPickerVisible();

    CARAPI_(void) UpdateZoomPicker();

    /**
     * The embedded zoom control intercepts touch events and automatically stays
     * visible. The external control needs to constantly refresh its internal
     * timer to stay visible.
     */
    CARAPI_(void) KeepZoomPickerVisible();

    CARAPI_(AutoPtr<IView>) GetExternalZoomPicker();

    CARAPI_(void) SetHardwareAccelerated();

    /**
     * OnPageFinished called by webview when a page is fully loaded.
     */
    /* package*/
    CARAPI_(void) OnPageFinished(
        /* [in] */ const String& url);

    CARAPI ToString(
        /* [out] */ String* info);

private:
    CARAPI_(void) SetDefaultZoomScale(
        /* [in] */ Float defaultScale);

    // returns TRUE if zoom out succeeds and FALSE if no zoom changes.
    CARAPI_(Boolean) Zoom(
        /* [in] */ Float zoomMultiplier);

    CARAPI_(void) SetZoomScale(
        /* [in] */ Float scale,
        /* [in] */ Boolean reflowText,
        /* [in] */ Boolean force);

    CARAPI_(void) SetZoomOverviewWidth(
        /* [in] */ Int32 width);

    CARAPI_(void) ZoomToOverview();

    CARAPI_(void) ZoomToReadingLevel();

    CARAPI_(void) SanitizeMinMaxScales();

    /**
     * Set up correct zoom overview width based on different settings.
     *
     * @param drawData webviewcore draw data
     * @param viewWidth current view width
     */
    CARAPI_(Boolean) SetupZoomOverviewWidth(
        /* [in] */ CWebViewCoreDrawData* drawData,
        /* [in] */ const Int32 viewWidth);

    CARAPI_(AutoPtr<IZoomControlBase>) GetCurrentZoomControl();

public:
    static const String LOGTAG;

protected:
    /*
     * The scale factors that determine the upper and lower bounds for the
     * default zoom scale.
     */
    static const Float DEFAULT_MAX_ZOOM_SCALE_FACTOR = 4.00f;
    static const Float DEFAULT_MIN_ZOOM_SCALE_FACTOR = 0.25f;

private:
    CWebViewClassic* mWebView;
    CallbackProxy* mCallbackProxy;

    // Widgets responsible for the on-screen zoom functions of the WebView.
    AutoPtr<ZoomControlEmbedded> mEmbeddedZoomControl;
    AutoPtr<ZoomControlExternal> mExternalZoomControl;

    // The default scale limits, which are dependent on the display density.
    Float mDefaultMaxZoomScale;
    Float mDefaultMinZoomScale;

    // The actual scale limits, which can be set through a webpage's viewport
    // meta-tag.
    Float mMaxZoomScale;
    Float mMinZoomScale;

    // Locks the minimum ZoomScale to the value currently set in mMinZoomScale.
    Boolean mMinZoomScaleFixed;

    /*
     * When loading a new page the WebView does not initially know the final
     * width of the page. Therefore, when a new page is loaded in overview mode
     * the overview scale is initialized to a default value. This flag is then
     * set and used to notify the ZoomManager to take the width of the next
     * picture from webkit and use that width to enter into zoom overview mode.
     */
    Boolean mInitialZoomOverview;

    /*
     * When in the zoom overview mode, the page's width is fully fit to the
     * current window. Additionally while the page is in this state it is
     * active, in other words, you can click to follow the links. We cache a
     * Boolean to enable us to quickly check whether or not we are in overview
     * mode, but this value should only be modified by changes to the zoom
     * scale.
     */
    Boolean mInZoomOverview;
    Int32 mZoomOverviewWidth;
    Float mInvZoomOverviewWidth;

    /*
     * These variables track the center point of the zoom and they are used to
     * determine the point around which we should zoom. They are stored in view
     * coordinates.
     */
    Float mZoomCenterX;
    Float mZoomCenterY;

    /*
     * Similar to mZoomCenterX(Y), these track the focus point of the scale
     * gesture. The difference is these get updated every time when onScale is
     * invoked no matter if a zooming really happens.
     */
    Float mFocusX;
    Float mFocusY;

    /*
     * mFocusMovementQueue keeps track of the previous focus point movement
     * has been through. Comparing to the difference of the gesture's previous
     * span and current span, it determines if the gesture is for panning or
     * zooming or both.
     */
    AutoPtr<FocusMovementQueue> mFocusMovementQueue;

    /*
     * These values represent the point around which the screen should be
     * centered after zooming. In other words it is used to determine the center
     * point of the visible document after the page has finished zooming. This
     * is important because the zoom may have potentially reflowed the text and
     * we need to ensure the proper portion of the document remains on the
     * screen.
     */
    Int32 mAnchorX;
    Int32 mAnchorY;

    // The scale factor that is used to determine the column width for text
    Float mTextWrapScale;

    /*
     * The default zoom scale is the scale factor used when the user triggers a
     * zoom in by double tapping on the WebView. The value is initially set
     * based on the display density, but can be changed at any time via the
     * WebSettings.
     */
    Float mDefaultScale;
    Float mInvDefaultScale;

    /*
     * The logical density of the display. This is a scaling factor for the
     * Density Independent Pixel unit, where one DIP is one pixel on an
     * approximately 160 dpi screen (see android.util.DisplayMetrics.density)
     */
    Float mDisplayDensity;

    /*
     * The factor that is used to tweak the zoom scale on a double-tap,
     * and can be changed via WebSettings. Range is from 0.75f to 1.25f.
     */
    Float mDoubleTapZoomFactor;

    /*
     * The scale factor that is used as the minimum increment when going from
     * overview to reading level on a double tap.
     */
    static Float MIN_DOUBLE_TAP_SCALE_INCREMENT;

    // the current computed zoom scale and its inverse.
    Float mActualScale;
    Float mInvActualScale;

    /*
     * The initial scale for the WebView. 0 means default. If initial scale is
     * greater than 0, the WebView starts with this value as its initial scale.
     */
    Float mInitialScale;

    static Float MINIMUM_SCALE_INCREMENT;

    /*
     *  The touch points could be changed even the fingers stop moving.
     *  We use the following to filter out the zooming jitters.
     */
    static Float MINIMUM_SCALE_WITHOUT_JITTER;

    /*
     * The following member variables are only to be used for animating zoom. If
     * mZoomScale is non-zero then we are in the middle of a zoom animation. The
     * other variables are used as a cache (e.g. inverse) or as a way to store
     * the state of the view prior to animating (e.g. initial scroll coords).
     */
    Float mZoomScale;
    Float mInvInitialZoomScale;
    Float mInvFinalZoomScale;
    Int32 mInitialScrollX;
    Int32 mInitialScrollY;
    Int64 mZoomStart;

    static const Int32 ZOOM_ANIMATION_LENGTH = 175;

    // whether support multi-touch
    Boolean mSupportMultiTouch;

    /**
     * True if we have a touch panel capable of detecting smooth pan/scale at the same time
     */
    Boolean mAllowPanAndScale;

    // use the framework's ScaleGestureDetector to handle scaling gestures
    AutoPtr<IScaleGestureDetector> mScaleDetector;
    Boolean mPinchToZoomAnimating;

    Boolean mHardwareAccelerated;
    Boolean mInHWAcceleratedZoom;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_ZOOMMANAGER_H__
