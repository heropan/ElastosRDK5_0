
#ifndef __ELASTOS_DROID_VIEW_CSURFACEVIEWWINDOW_H__
#define  __ELASTOS_DROID_VIEW_CSURFACEVIEWWINDOW_H__

#include "_Elastos_Droid_View_CSurfaceViewWindow.h"
#include "ext/frameworkext.h"

using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Content::Res::IConfiguration;

namespace Elastos {
namespace Droid {
namespace View {

class SurfaceView;

CarClass(CSurfaceViewWindow)
{
public:
    CSurfaceViewWindow();

    CARAPI constructor(
        /* [in] */ Handle32 surfaceView);

    CARAPI ExecuteCommand(
        /* [in] */ const String& command,
        /* [in] */ const String& parameters,
        /* [in] */ IParcelFileDescriptor* out);

    CARAPI Resized(
        /* [in] */ IRect* frame,
        /* [in] */ IRect* coveredInsets,
        /* [in] */ IRect* visibleInsets,
        /* [in] */ Boolean reportDraw,
        /* [in] */ IConfiguration* newConfig);

    CARAPI Moved(
        /* [in] */ Int32 newX,
        /* [in] */ Int32 newY);

    CARAPI DispatchAppVisibility(
        /* [in] */ Boolean visible);

    CARAPI DispatchGetNewSurface();

    CARAPI DispatchScreenState(
        /* [in] */ Boolean on);

    /**
     * Tell the window that it is either gaining or losing focus.  Keep it up
     * to date on the current state showing navigational focus (touch mode) too.
     */
    CARAPI WindowFocusChanged(
        /* [in] */ Boolean hasFocus,
        /* [in] */ Boolean touchEnabled);

    CARAPI CloseSystemDialogs(
        /* [in] */ const String& reason);

    /**
     * Called for wallpaper windows when their offsets change.
     */
    CARAPI DispatchWallpaperOffsets(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float xStep,
        /* [in] */ Float yStep,
        /* [in] */ Boolean sync);

    CARAPI DispatchWallpaperCommand(
        /* [in] */ const String& action,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 z,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean sync);

    /**
     * Drag/drop events
     */
    CARAPI DispatchDragEvent(
        /* [in] */ IDragEvent* event);

    /**
     * System chrome visibility changes
     */
    CARAPI DispatchSystemUiVisibilityChanged(
        /* [in] */ Int32 seq,
        /* [in] */ Int32 globalVisibility,
        /* [in] */ Int32 localValue,
        /* [in] */ Int32 localChanges);

    /**
     * If the window manager returned RELAYOUT_RES_ANIMATING
     * from relayout(), this method will be called when the animation
     * is done.
     */
    CARAPI DoneAnimating();

    CARAPI SetSession(
        /* [in] */ IWindowSession* session);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    friend class SurfaceView;

    SurfaceView* mSurfaceView;

    Int32 mCurWidth;// = -1;
    Int32 mCurHeight;// = -1;

    AutoPtr<IWindowSession> mSession;
    Int32 mSeq;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_VIEW_CSURFACEVIEWWINDOW_H__
