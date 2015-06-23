
#include "view/CSurfaceHelper.h"
#include "view/CSurface.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace View {

ECode CSurfaceHelper::Screenshot(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    return CSurface::Screenshot(width, height, bitmap);
}

ECode CSurfaceHelper::Screenshot(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 minLayer,
    /* [in] */ Int32 maxLayer,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    return CSurface::Screenshot(width, height, minLayer, maxLayer, bitmap);
}

ECode CSurfaceHelper::OpenTransaction()
{
    return CSurface::OpenTransaction();
}

ECode CSurfaceHelper::CloseTransaction()
{
    return CSurface::CloseTransaction();
}

ECode CSurfaceHelper::SetAnimationTransaction()
{
    return CSurface::SetAnimationTransaction();
}

ECode CSurfaceHelper::GetBuiltInDisplay(
    /* [in] */ Int32 builtInDisplayId,
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);
    return CSurface::GetBuiltInDisplay(builtInDisplayId, binder);
}

ECode CSurfaceHelper::CreateDisplay(
    /* [in] */ const String& name,
    /* [in] */ Boolean secure,
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);
    return CSurface::CreateDisplay(name, secure, binder);
}

ECode CSurfaceHelper::SetDisplaySurface(
    /* [in] */ IBinder* displayToken,
    /* [in] */ ISurface* surface)
{
    return CSurface::SetDisplaySurface(displayToken, surface);
}

ECode CSurfaceHelper::SetDisplayLayerStack(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 layerStack)
{
    return CSurface::SetDisplayLayerStack(displayToken, layerStack);
}

ECode CSurfaceHelper::SetDisplayProjection(
    /* [in] */ IBinder* displayToken,
    /* [in] */ Int32 orientation,
    /* [in] */ IRect* layerStackRect,
    /* [in] */ IRect* displayRect)
{
    return CSurface::SetDisplayProjection(
        displayToken, orientation, layerStackRect, displayRect);
}

ECode CSurfaceHelper::GetDisplayInfo(
    /* [in] */ IBinder* displayToken,
    /* [in] */ IPhysicalDisplayInfo* outInfo,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return CSurface::GetDisplayInfo(displayToken, outInfo, result);
}

ECode CSurfaceHelper::BlankDisplay(
    /* [in] */ IBinder* displayToken)
{
    return CSurface::BlankDisplay(displayToken);
}

ECode CSurfaceHelper::UnblankDisplay(
    /* [in] */ IBinder* displayToken)
{
    return CSurface::UnblankDisplay(displayToken);
}

} // namespace View
} // namespace Droid
} // namespace Elastos
