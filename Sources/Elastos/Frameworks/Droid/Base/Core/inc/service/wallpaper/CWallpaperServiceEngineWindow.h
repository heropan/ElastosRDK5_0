
#ifndef __CWALLPAPERSERVICEENGINEWINDOW_H__
#define __CWALLPAPERSERVICEENGINEWINDOW_H__

#include "_CWallpaperServiceEngineWindow.h"
#include "service/wallpaper/WallpaperService.h"
#include "view/BaseIWindow.h"

using Elastos::Droid::Internal::View::BaseIWindow;

namespace Elastos {
namespace Droid {
namespace Service {
namespace Wallpaper {

CarClass(CWallpaperServiceEngineWindow), public BaseIWindow
{
public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI constructor(
        /* [in] */ Handle32 engine);

    CARAPI SetSession(
        /* [in] */ IWindowSession* session);

    CARAPI SetSeq(
        /* [in] */ Int32 seq);

    CARAPI GetSeq(
        /* [out] */ Int32* seq);

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

    CARAPI DispatchScreenState(
        /* [in] */ Boolean on);

    CARAPI DispatchGetNewSurface();

    CARAPI WindowFocusChanged(
        /* [in] */ Boolean hasFocus,
        /* [in] */ Boolean inTouchMode);

    CARAPI ExecuteCommand(
        /* [in] */ const String& command,
        /* [in] */ const String& parameters,
        /* [in] */ IParcelFileDescriptor* descriptor);

    CARAPI CloseSystemDialogs(
        /* [in] */ const String& reason);

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

    /* Drag/drop */
    CARAPI DispatchDragEvent(
        /* [in] */ IDragEvent* event);

    CARAPI DispatchSystemUiVisibilityChanged(
        /* [in] */ Int32 seq,
        /* [in] */ Int32 globalVisibility,
        /* [in] */ Int32 localValue,
        /* [in] */ Int32 localChanges);

    CARAPI DoneAnimating();

    CARAPI ToString(
        /* [out] */ String* description);

private:
    WallpaperService::Engine* mEngine;
};

} // namespace Wallpaper
} // namespace Service
} // namepsace Droid
} // namespace Elastos

#endif //__CWALLPAPERSERVICEENGINEWINDOW_H__
