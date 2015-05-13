
#include "wm/DimSurface.h"
#include "wm/CWindowManagerService.h"
#include <elastos/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::View::CSurface;
using Elastos::Droid::Graphics::IPixelFormat;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

const String DimSurface::TAG("DimSurface");

DimSurface::DimSurface(
    /* [in] */ ISurfaceSession* session,
    /* [in] */ Int32 layerStack)
    : mDimShown(FALSE)
    , mDimColor(0)
    , mLayer(-1)
    , mLastDimWidth(0)
    , mLastDimHeight(0)
{
    // try {
    // if (WindowManagerService.DEBUG_SURFACE_TRACE) {
    //     mDimSurface = new WindowStateAnimator.SurfaceTrace(session,
    //         "DimSurface",
    //         16, 16, PixelFormat.OPAQUE,
    //         Surface.FX_SURFACE_DIM | Surface.HIDDEN);
    // } else {
    ASSERT_SUCCEEDED(CSurface::New(session, String("DimSurface"),
            16, 16, IPixelFormat::OPAQUE,
            ISurface::FX_SURFACE_DIM | ISurface::HIDDEN, (ISurface**)&mDimSurface));
    // }
    // if (WindowManagerService.SHOW_TRANSACTIONS ||
    //         WindowManagerService.SHOW_SURFACE_ALLOC) Slog.i(WindowManagerService.TAG,
    //                 "  DIM " + mDimSurface + ": CREATE");
    mDimSurface->SetLayerStack(layerStack);
    mDimSurface->SetAlpha(0.0f);
    mDimSurface->Show();
    // } catch (Exception e) {
    //     Slog.e(WindowManagerService.TAG, "Exception creating Dim surface", e);
    // }
}

void DimSurface::Show(
    /* [in] */ Int32 dw,
    /* [in] */ Int32 dh,
    /* [in] */ Int32 layer,
    /* [in] */ Int32 color)
{
    if (mDimSurface == NULL) {
        Slogger::E(TAG, "show: no Surface");
        return;
    }

    if (!mDimShown) {
        if (CWindowManagerService::SHOW_TRANSACTIONS) {
            Slogger::I(CWindowManagerService::TAG, "  DIM %p: SHOW pos=(0,0) (%dx%d layer=%d)",
                    mDimSurface.Get(), dw, dh, layer);
        }
        mDimShown = TRUE;
        // try {
        mLastDimWidth = dw;
        mLastDimHeight = dh;
        mDimSurface->SetPosition(0, 0);
        mDimSurface->SetSize(dw, dh);
        mDimSurface->SetLayer(layer);
        ECode ec = mDimSurface->Show();
        if (FAILED(ec)) {
            Slogger::W(CWindowManagerService::TAG, "Failure showing dim surface 0x%08x", ec);
        }
        // } catch (RuntimeException e) {
        //     Slog.w(WindowManagerService.TAG, "Failure showing dim surface", e);
        // }
    }
    else if (mLastDimWidth != dw || mLastDimHeight != dh || mDimColor != color
            || mLayer != layer) {
        if (CWindowManagerService::SHOW_TRANSACTIONS) {
            Slogger::I(CWindowManagerService::TAG, "  DIM %p: pos=(0,0) (%dx%d layer=%d)",
                    mDimSurface.Get(), dw, dh, layer);
        }
        mLastDimWidth = dw;
        mLastDimHeight = dh;
        mLayer = layer;
        mDimColor = color;
        mDimSurface->SetSize(dw, dh);
        mDimSurface->SetLayer(layer);
        mDimSurface->SetAlpha(((color >> 24) & 0xff) / 255.0f);
    }
}

void DimSurface::Hide()
{
    if (mDimSurface == NULL) {
        Slogger::E(TAG, "hide: no Surface");
        return;
    }

    if (mDimShown) {
        mDimShown = FALSE;
        // try {
        if (CWindowManagerService::SHOW_TRANSACTIONS) {
            Slogger::I(CWindowManagerService::TAG, "  HIDE %p", mDimSurface.Get());
        }
        if (FAILED(mDimSurface->Hide())) {
            Slogger::W(CWindowManagerService::TAG, "Illegal argument exception hiding dim surface");
        }
        // } catch (RuntimeException e) {
        //     Slog.w(WindowManagerService.TAG, "Illegal argument exception hiding dim surface");
        // }
    }
}

void DimSurface::Kill()
{
    if (mDimSurface != NULL) {
        mDimSurface->Destroy();
        mDimSurface = NULL;
    }
}


} // Wm
} // Server
} // Droid
} // Elastos
