
#ifndef __CVIEWROOTIMPLW_H__
#define __CVIEWROOTIMPLW_H__

#include "_CViewRootImplW.h"

using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Content::Res::IConfiguration;

namespace Elastos {
namespace Droid {
namespace View {

class ViewRootImpl;

CarClass(CViewRootImplW)
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 viewRoot);

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
    static CARAPI_(Int32) CheckCallingPermission(
        /* [in] */ const String& permission);

    CARAPI_(AutoPtr<ViewRootImpl>) GetViewRootImpl();

private:
    AutoPtr<IWeakReference> mViewAncestor;
    AutoPtr<IWindowSession> mWindowSession;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__CVIEWROOTIMPLW_H__
