
#include "CFadeOutRunnable.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

ECode CGestureOverlayView::constructor()
{
    GestureOverlayView::FadeOutRunnable::FadeOutRunnable();
    return NOERROR;
}

CARAPI CGestureOverlayView::SetFireActionPerformed(
    /* [in] */ Boolean fireActionPerformed)
{
    return GestureOverlayView::FadeOutRunnable::SetFireActionPerformed(
        fireActionPerformed);
}

CARAPI CGestureOverlayView::SetResetMultipleStrokes(
    /* [in] */ Boolean resetMultipleStrokes)
{
    return GestureOverlayView::FadeOutRunnable::SetResetMultipleStrokes(
        resetMultipleStrokes);
}

CARAPI CGestureOverlayView::GetFireActionPerformed(
    /* [out] */ Boolean *fireActionPerformed)
{
    return GestureOverlayView::FadeOutRunnable::GetFireActionPerformed(fireActionPerformed);
}

CARAPI CGestureOverlayView::GetResetMultipleStrokes(
    /* [out] */ Boolean *resetMultipleStrokes)
{
    return GestureOverlayView::FadeOutRunnable::GetResetMultipleStrokes(resetMultipleStrokes);
}

CARAPI CGestureOverlayView::Run()
{
    return GestureOverlayView::FadeOutRunnable::Run();
}
}
}
}

