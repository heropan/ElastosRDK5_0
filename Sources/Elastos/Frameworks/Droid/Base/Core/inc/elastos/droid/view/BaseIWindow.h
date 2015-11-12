
#ifndef __ELASTOS_DROID_INTERNAL_VIEW_BASEIWINDOW_H__
#define __ELASTOS_DROID_INTERNAL_VIEW_BASEIWINDOW_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::View::IWindowSession;
using Elastos::Droid::View::IDragEvent;
using Elastos::Droid::View::IIWindow;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {

class BaseIWindow
    : public Object
    , public IIWindow
    , public IBinder
{
public:
    CAR_INTERFACE_DECL()

    virtual CARAPI SetSession(
        /* [in] */ IWindowSession* session);

    virtual CARAPI Resized(
        /* [in] */ IRect* frame,
        /* [in] */ IRect* overscanInsets,
        /* [in] */ IRect* contentInsets,
        /* [in] */ IRect* visibleInsets,
        /* [in] */ IRect* stableInsets,
        /* [in] */ Boolean reportDraw,
        /* [in] */ IConfiguration* newConfig);

    virtual CARAPI Moved(
        /* [in] */ Int32 newX,
        /* [in] */ Int32 newY);

    virtual CARAPI DispatchAppVisibility(
        /* [in] */ Boolean visible);

    virtual CARAPI DispatchGetNewSurface();

    virtual CARAPI DispatchScreenState(
        /* [in] */ Boolean on);

    virtual CARAPI WindowFocusChanged(
        /* [in] */ Boolean hasFocus,
        /* [in] */ Boolean touchEnabled);

    virtual CARAPI ExecuteCommand(
        /* [in] */ const String& command,
        /* [in] */ const String& parameters,
        /* [in] */ IParcelFileDescriptor* out);

    virtual CARAPI CloseSystemDialogs(
        /* [in] */ const String& reason);

    virtual CARAPI DispatchWallpaperOffsets(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float xStep,
        /* [in] */ Float yStep,
        /* [in] */ Boolean sync);

    virtual CARAPI DispatchDragEvent(
        /* [in] */ IDragEvent* event);

    virtual CARAPI DispatchSystemUiVisibilityChanged(
        /* [in] */ Int32 seq,
        /* [in] */ Int32 globalUi,
        /* [in] */ Int32 localValue,
        /* [in] */ Int32 localChanges);

    virtual CARAPI DispatchWallpaperCommand(
        /* [in] */ const String& action,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 z,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean sync);

    virtual CARAPI DoneAnimating();

public:
    Int32 mSeq;

private:
    AutoPtr<IWindowSession> mSession;
};

} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_VIEW_BASEIWINDOW_H__
