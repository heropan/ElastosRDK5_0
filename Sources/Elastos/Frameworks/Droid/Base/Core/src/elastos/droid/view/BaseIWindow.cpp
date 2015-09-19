
#include "view/BaseIWindow.h"

using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::EIID_IBinder;
using Elastos::Droid::View::IIWindow;
using Elastos::Droid::View::EIID_IIWindow;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {

ECode BaseIWindow::SetSession(
    /* [in] */ IWindowSession* session)
{
    mSession = session;
    return NOERROR;
}

ECode BaseIWindow::Resized(
    /* [in] */ IRect* frame,
    /* [in] */ IRect* contentInsets,
    /* [in] */ IRect* visibleInsets,
    /* [in] */ Boolean reportDraw,
    /* [in] */ IConfiguration* newConfig)
{
    if (reportDraw) {
        mSession->FinishDrawing((IIWindow*)this->Probe(EIID_IIWindow));
    }

    return NOERROR;
}

ECode BaseIWindow::Moved(
    /* [in] */ Int32 newX,
    /* [in] */ Int32 newY)
{
    return NOERROR;
}

ECode BaseIWindow::DispatchAppVisibility(
    /* [in] */ Boolean visible)
{
    return NOERROR;
}

ECode BaseIWindow::DispatchGetNewSurface()
{
    return NOERROR;
}

ECode BaseIWindow::DispatchScreenState(
    /* [in] */ Boolean on)
{
    return NOERROR;
}

ECode BaseIWindow::WindowFocusChanged(
    /* [in] */ Boolean hasFocus,
    /* [in] */ Boolean touchEnabled)
{
    return NOERROR;
}

ECode BaseIWindow::ExecuteCommand(
    /* [in] */ const String& command,
    /* [in] */ const String& parameters,
    /* [in] */ IParcelFileDescriptor* out)
{
    return NOERROR;
}

ECode BaseIWindow::CloseSystemDialogs(
    /* [in] */ const String& reason)
{
    return NOERROR;
}

ECode BaseIWindow::DispatchWallpaperOffsets(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float xStep,
    /* [in] */ Float yStep,
    /* [in] */ Boolean sync)
{
    if (sync) {
        mSession->WallpaperOffsetsComplete((IBinder*)this->Probe(EIID_IBinder));
    }
    return NOERROR;
}

ECode BaseIWindow::DispatchDragEvent(
    /* [in] */ IDragEvent* event)
{
    return NOERROR;
}

ECode BaseIWindow::DispatchSystemUiVisibilityChanged(
    /* [in] */ Int32 seq,
    /* [in] */ Int32 globalUi,
    /* [in] */ Int32 localValue,
    /* [in] */ Int32 localChanges)
{
    mSeq = seq;
    return NOERROR;
}

ECode BaseIWindow::DispatchWallpaperCommand(
    /* [in] */ const String& action,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean sync)
{
    if (sync) {
        mSession->WallpaperCommandComplete(
            (IBinder*)this->Probe(EIID_IBinder), NULL);
    }
    return NOERROR;
}

ECode BaseIWindow::DoneAnimating()
{
    return NOERROR;
}

} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos
