
#ifndef __CWALLPAPERENGINEWRAPPER_H__
#define __CWALLPAPERENGINEWRAPPER_H__

#include "_CWallpaperEngineWrapper.h"
#include "service/wallpaper/WallpaperService.h"

using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::Os::IHandlerCaller;
using Elastos::Droid::Os::IBinder;

namespace Elastos {
namespace Droid {
namespace Service {
namespace Wallpaper {

CarClass(CWallpaperEngineWrapper)
{
    friend class WallpaperService;
    friend class CWallpaperServiceEngineWindow;
public:
    CARAPI constructor(
        /* [in] */ Handle32 context,
        /* [in] */ IWallpaperConnection* conn,
        /* [in] */ IBinder* windowToken,
        /* [in] */ Int32 windowType,
        /* [in] */ Boolean isPreview,
        /* [in] */ Int32 reqWidth,
        /* [in] */ Int32 reqHeight);

    CARAPI SetDesiredSize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI SetVisibility(
        /* [in] */ Boolean visible);

    CARAPI DispatchPointer(
        /* [in] */ IMotionEvent* event);

    CARAPI DispatchWallpaperCommand(
        /* [in] */ const String& action,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 z,
        /* [in] */ IBundle* extras);

    CARAPI ReportShown();

    CARAPI Destroy();

    CARAPI GetConnection(
        /* [out] */ IWallpaperConnection** connection);

    CARAPI GetWindowToken(
        /* [out] */ IBinder** windowToken);

    CARAPI SetWindowType(
        /* [in] */ Int32 windowType);

    CARAPI GetWindowType(
        /* [out] */ Int32* windowType);

    CARAPI SetIsPreview(
        /* [in] */ Boolean isPreview);

    CARAPI GetIsPreview(
        /* [out] */ Boolean* isPreview);

    CARAPI SetShownReported(
        /* [in] */ Boolean shownReported);

    CARAPI GetShownReported(
        /* [out] */ Boolean* shownReported);

    CARAPI SetReqWidth(
        /* [in] */ Int32 reqWidth);

    CARAPI GetReqWidth(
        /* [out] */ Int32* reqWidth);

    CARAPI SetReqHeight(
        /* [in] */ Int32 reqHeight);

    CARAPI GetReqHeight(
        /* [out] */ Int32* reqHeight);

    CARAPI ExecuteMessage(
        /* [in] */ IMessage* msg);

    CARAPI ToString(
        /* [out] */ String* description);

    CARAPI GetHandlerCaller(
        /* [out] */ IHandlerCaller** caller);

private:
    static const String TAG;// = "WallpaperService";
    static const Boolean DEBUG;// = false;

    const static Int32 DO_ATTACH;// = 10;
    const static Int32 DO_DETACH;// = 20;
    const static Int32 DO_SET_DESIRED_SIZE;// = 30;

    const static Int32 MSG_UPDATE_SURFACE;// = 10000;
    const static Int32 MSG_VISIBILITY_CHANGED;// = 10010;
    const static Int32 MSG_WALLPAPER_OFFSETS;// = 10020;
    const static Int32 MSG_WALLPAPER_COMMAND;// = 10025;
    const static Int32 MSG_WINDOW_RESIZED;// = 10030;
    const static Int32 MSG_WINDOW_MOVED;// = 10035;
    const static Int32 MSG_TOUCH_EVENT;// = 10040;

private:
    CARAPI_(void) DoAttach();

    CARAPI_(void) DoDetach();

    CARAPI_(void) HandleTouchEvent(
        /* [in] */ IMotionEvent* ev);

private:
    AutoPtr<IHandlerCaller> mCaller;
    AutoPtr<IWallpaperConnection> mConnection;
    AutoPtr<IBinder> mWindowToken;
    Int32 mWindowType;
    Boolean mIsPreview;
    Boolean mShownReported;
    Int32 mReqWidth;
    Int32 mReqHeight;
    AutoPtr<WallpaperService::Engine> mEngine;
    AutoPtr<WallpaperService> mOwner;
};

} // namespace Wallpaper
} // namespace Service
} // namepsace Droid
} // namespace Elastos

#endif //__CWALLPAPERENGINEWRAPPER_H__
