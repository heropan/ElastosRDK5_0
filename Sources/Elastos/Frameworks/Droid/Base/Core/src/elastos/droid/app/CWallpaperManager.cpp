
#include "elastos/droid/app/CWallpaperManager.h"
#include "elastos/droid/app/CFastBitmapDrawable.h"
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/graphics/CPorterDuffXfermode.h"
#include "elastos/droid/graphics/CRect.h"
#include "elastos/droid/graphics/CCanvas.h"
#include "elastos/droid/graphics/CBitmapFactory.h"
#include "elastos/droid/graphics/drawable/CBitmapDrawable.h"
#include "elastos/droid/view/CWindowManagerGlobal.h"
#include "elastos/droid/utility/CDisplayMetrics.h"
#include "elastos/droid/R.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Graphics::IPorterDuffXfermode;
using Elastos::Droid::Graphics::CPorterDuffXfermode;
using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::CCanvas;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::IXfermode;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawable;
using Elastos::Droid::Graphics::Drawable::IBitmapDrawable;
using Elastos::Droid::Graphics::Drawable::CBitmapDrawable;
using Elastos::Droid::View::CWindowManagerGlobal;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::IWindowManager;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Utility::CDisplayMetrics;
using Elastos::Droid::Os::IParcelFileDescriptor;

namespace Elastos{
namespace Droid{
namespace App{

const String CWallpaperManager::TAG("WallpaperManager");
Boolean CWallpaperManager::DEBUG = FALSE;
Mutex CWallpaperManager::sSync;
AutoPtr<CGlobalsWallpaperManagerCallback> CWallpaperManager::sGlobals;

CWallpaperManager::CWallpaperManager()
    : mWallpaperXStep(-1)
    , mWallpaperYStep(-1)
{}

void CWallpaperManager::InitGlobals(
    /* [in] */ ILooper* looper)
{
    AutoLock lock(sSync);
    if (sGlobals == NULL) {
        ASSERT_SUCCEEDED(CGlobalsWallpaperManagerCallback::NewByFriend(
                looper, (CGlobalsWallpaperManagerCallback**)&sGlobals));
    }
}

ECode CWallpaperManager::GetInstance(
    /* [in] */ IContext* context,
    /* [out] */ IWallpaperManager** manager)
{
    VALIDATE_NOT_NULL(manager);
    return context->GetSystemService(
            IContext::WALLPAPER_SERVICE, (IInterface**)manager);
}

ECode CWallpaperManager::GetIWallpaperManager(
    /* [out] */ IIWallpaperManager **manager)
{
    VALIDATE_NOT_NULL(manager);
    *manager = sGlobals->mService;
    REFCOUNT_ADD(*manager);
    return NOERROR;
}

ECode CWallpaperManager::GetDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IBitmap> bm = sGlobals->PeekWallpaperBitmap(mContext, TRUE);
    if (bm != NULL) {
        AutoPtr<IBitmapDrawable> dr;
        AutoPtr<IResources> res;
        mContext->GetResources((IResources**)&res);
        CBitmapDrawable::New(res, bm, (IBitmapDrawable**)&dr);
        dr->SetDither(FALSE);
        *drawable = (IDrawable*)dr;
        REFCOUNT_ADD(*drawable);
        return NOERROR;
    }
    *drawable = NULL;
    return NOERROR;
}

ECode CWallpaperManager::PeekDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IBitmap> bm = sGlobals->PeekWallpaperBitmap(mContext, FALSE);
    if (bm != NULL) {
        AutoPtr<IBitmapDrawable> dr;
        AutoPtr<IResources> res;
        mContext->GetResources((IResources**)&res);
        CBitmapDrawable::New(res, bm, (IBitmapDrawable**)&dr);
        dr->SetDither(FALSE);
        *drawable = (IDrawable*)dr;
        REFCOUNT_ADD(*drawable);
        return NOERROR;
    }
    *drawable = NULL;
    return NOERROR;
}

ECode CWallpaperManager::GetFastDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IBitmap> bm = sGlobals->PeekWallpaperBitmap(mContext, TRUE);
    if (bm != NULL) {
        AutoPtr<IFastBitmapDrawable> fd;
        CFastBitmapDrawable::New(bm, (IFastBitmapDrawable**)&fd);
        *drawable = IDrawable::Probe(fd.Get());
        REFCOUNT_ADD(*drawable);
        return NOERROR;
    }
    *drawable = NULL;
    return NOERROR;
}

ECode CWallpaperManager::PeekFastDrawable(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IBitmap> bm = sGlobals->PeekWallpaperBitmap(mContext, FALSE);
    if (bm != NULL) {
        AutoPtr<IFastBitmapDrawable> fd;
        CFastBitmapDrawable::New(bm, (IFastBitmapDrawable**)&fd);
        *drawable = IDrawable::Probe(fd.Get());
        REFCOUNT_ADD(*drawable);
        return NOERROR;
    }
    *drawable = NULL;
    return NOERROR;
}

ECode CWallpaperManager::GetBitmap(
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    AutoPtr<IBitmap> bm = sGlobals->PeekWallpaperBitmap(mContext, TRUE);
    *bitmap = bm;
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

ECode CWallpaperManager::ForgetLoadedWallpaper()
{
    sGlobals->ForgetLoadedWallpaper();
    return NOERROR;
}

ECode CWallpaperManager::GetWallpaperInfo(
    /* [out] */ IWallpaperInfo** info)
{
    VALIDATE_NOT_NULL(info);
    // try {
    if (sGlobals->mService == NULL) {
        Slogger::W(TAG, "WallpaperService not running");
        *info = NULL;
        return NOERROR;
    }
    else {
        return sGlobals->mService->GetWallpaperInfo(info);
    }
    // } catch (RemoteException e) {
    //     return null;
    // }
}

ECode CWallpaperManager::SetResource(
    /* [in] */ Int32 resid)
{
    if (sGlobals->mService == NULL) {
        Slogger::W(TAG, "WallpaperService not running");
        return NOERROR;
    }
    // try {
    AutoPtr<IResources> resources;
    ASSERT_SUCCEEDED(mContext->GetResources((IResources**)&resources));
    String name;
    resources->GetResourceName(resid, &name);
    /* Set the wallpaper to the default values */
    AutoPtr<IParcelFileDescriptor> fd;
    FAIL_RETURN(sGlobals->mService->SetWallpaper(
            String("res:") + name, (IParcelFileDescriptor**)&fd));
    if (fd != NULL) {
        AutoPtr<IFileOutputStream> fos;
        // try {
        // fos = new ParcelFileDescriptor.AutoCloseOutputStream(fd);
        AutoPtr<IFileDescriptor> des;
        fd->GetFileDescriptor((IFileDescriptor**)&des);
        ASSERT_SUCCEEDED(CFileOutputStream::New(des, (IFileOutputStream**)&fos));
        AutoPtr<IInputStream> res;
        ASSERT_SUCCEEDED(resources->OpenRawResource(resid, (IInputStream**)&res));
        SetWallpaper(res, fos);
        if (fos != NULL) {
            fos->Close();
        }
        fd->Close();
        // } finally {
        //     if (fos != null) {
        //         fos.close();
        //     }
        // }
    }
    // } catch (RemoteException e) {
    //     // Ignore
    // }
    return NOERROR;
}

ECode CWallpaperManager::SetBitmap(
    /* [in] */ IBitmap* bitmap)
{
    if (sGlobals->mService == NULL) {
        Slogger::W(TAG, "WallpaperService not running");
        return NOERROR;
    }
    // try {
    AutoPtr<IParcelFileDescriptor> fd;
    FAIL_RETURN(sGlobals->mService->SetWallpaper(String(NULL), (IParcelFileDescriptor**)&fd));
    if (fd == NULL) {
        return NOERROR;
    }
    AutoPtr<IFileOutputStream> fos;
    // try {
    AutoPtr<IFileDescriptor> des;
    fd->GetFileDescriptor((IFileDescriptor**)&des);
    ASSERT_SUCCEEDED(CFileOutputStream::New(des, (IFileOutputStream**)&fos));
    // fos = new ParcelFileDescriptor.AutoCloseOutputStream(fd);
    Boolean result;
    bitmap->Compress(Elastos::Droid::Graphics::BitmapCompressFormat_PNG, 90, fos, &result);
    // } finally {
    if (fos != NULL) {
        fos->Close();
    }
    fd->Close();
    return NOERROR;
    // }
    // } catch (RemoteException e) {
    //     // Ignore
    // }
}

ECode CWallpaperManager::SetStream(
    /* [in] */ IInputStream* data)
{
    if (sGlobals->mService == NULL) {
        Slogger::W(TAG, "WallpaperService not running");
        return NOERROR;
    }
    // try {
    AutoPtr<IParcelFileDescriptor> fd;
    FAIL_RETURN(sGlobals->mService->SetWallpaper(String(NULL), (IParcelFileDescriptor**)&fd));
    if (fd == NULL) {
        return NOERROR;
    }
    AutoPtr<IFileOutputStream> fos;
    // try {
    AutoPtr<IFileDescriptor> des;
    fd->GetFileDescriptor((IFileDescriptor**)&des);
    ASSERT_SUCCEEDED(CFileOutputStream::New(des, (IFileOutputStream**)&fos));
    // fos = new ParcelFileDescriptor.AutoCloseOutputStream(fd);
    SetWallpaper(data, fos);
    // } finally {
    if (fos != NULL) {
        fos->Close();
    }
    fd->Close();
    return NOERROR;
    // }
    // } catch (RemoteException e) {
    //     // Ignore
    // }
}

void CWallpaperManager::SetWallpaper(
    /* [in] */ IInputStream* data,
    /* [in] */ IFileOutputStream* fos)
{
    AutoPtr< ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(32768);
    Int32 amt;
    while (data->ReadBytes(buffer, &amt), amt > 0) {
        fos->WriteBytes(*buffer, 0, amt);
    }
    return;
}

ECode CWallpaperManager::HasResourceWallpaper(
    /* [in] */ Int32 resid,
    /* [out] */ Boolean* hasResource)
{
    VALIDATE_NOT_NULL(hasResource);
    if (sGlobals->mService == NULL) {
        Slogger::W(TAG, "WallpaperService not running");
        *hasResource = FALSE;
        return NOERROR;
    }
    // try {
    AutoPtr<IResources> resources;
    ASSERT_SUCCEEDED(mContext->GetResources((IResources**)&resources));
    String resName;
    resources->GetResourceName(resid, &resName);
    String name = String("res:") + resName;
    return sGlobals->mService->HasNamedWallpaper(name, hasResource);
    // } catch (RemoteException e) {
    //     return false;
    // }
}

ECode CWallpaperManager::GetDesiredMinimumWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    if (sGlobals->mService == NULL) {
        Slogger::W(TAG, "WallpaperService not running");
        *width = 0;
        return NOERROR;
    }
    // try {
    return sGlobals->mService->GetWidthHint(width);
    // } catch (RemoteException e) {
    //     // Shouldn't happen!
    //     return 0;
    // }
}

ECode CWallpaperManager::GetDesiredMinimumHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    if (sGlobals->mService == NULL) {
        Slogger::W(TAG, "WallpaperService not running");
        *height = 0;
        return NOERROR;
    }
    // try {
    return sGlobals->mService->GetHeightHint(height);
    // } catch (RemoteException e) {
    //     // Shouldn't happen!
    //     return 0;
    // }
}

ECode CWallpaperManager::SuggestDesiredDimensions(
    /* [in] */ Int32 minimumWidth,
    /* [in] */ Int32 minimumHeight)
{
    // try {
    if (sGlobals->mService == NULL) {
        Slogger::W(TAG, "WallpaperService not running");
        return NOERROR;
    }
    else {
        return sGlobals->mService->SetDimensionHints(minimumWidth, minimumHeight);
    }
    // } catch (RemoteException e) {
    //     // Ignore
    // }
}

ECode CWallpaperManager::SetWallpaperOffsets(
    /* [in] */ IBinder* windowToken,
    /* [in] */ Float xOffset,
    /* [in] */ Float yOffset)
{
    // try {
    //Log.v(TAG, "Sending new wallpaper offsets from app...");
    AutoPtr<ILooper> looper;
    mContext->GetMainLooper((ILooper**)&looper);
    return CWindowManagerGlobal::GetWindowSession(looper)->SetWallpaperPosition(
            windowToken, xOffset, yOffset, mWallpaperXStep, mWallpaperYStep);
    //Log.v(TAG, "...app returning after sending offsets!");
    // } catch (RemoteException e) {
    //     // Ignore.
    // }
}

ECode CWallpaperManager::SetWallpaperOffsetSteps(
    /* [in] */ Float xStep,
    /* [in] */ Float yStep)
{
    mWallpaperXStep = xStep;
    mWallpaperYStep = yStep;
    return NOERROR;
}

ECode CWallpaperManager::SendWallpaperCommand(
    /* [in] */ IBinder *windowToken,
    /* [in] */ const String &action,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ IBundle* extras)
{
    // try {
    //Log.v(TAG, "Sending new wallpaper offsets from app...");
    AutoPtr<ILooper> looper;
    mContext->GetMainLooper((ILooper**)&looper);
    AutoPtr<IBundle> b;
    return CWindowManagerGlobal::GetWindowSession(looper)->SendWallpaperCommand(
            windowToken, action, x, y, z, extras, FALSE, (IBundle**)&b);
    //Log.v(TAG, "...app returning after sending offsets!");
    // } catch (RemoteException e) {
    //     // Ignore.
    // }
}

ECode CWallpaperManager::ClearWallpaperOffsets(
    /* [in] */ IBinder* windowToken)
{
    // try {
    AutoPtr<ILooper> looper;
    mContext->GetMainLooper((ILooper**)&looper);
    return CWindowManagerGlobal::GetWindowSession(looper)->SetWallpaperPosition(
            windowToken, -1, -1, -1, -1);
    // } catch (RemoteException e) {
    //     // Ignore.
    // }
}

ECode CWallpaperManager::Clear()
{
    SetResource(R::drawable::default_wallpaper);
    return NOERROR;
}

AutoPtr<IBitmap> CWallpaperManager::GenerateBitmap(
    /* [in] */ IContext* context,
    /* [in] */ IBitmap* bm,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (bm == NULL) {
        return NULL;
    }

    AutoPtr<IWindowManager> wm;
    ASSERT_SUCCEEDED(context->GetSystemService(IContext::WINDOW_SERVICE,
            (IInterface**)&wm));
    AutoPtr<IDisplayMetrics> metrics;
    ASSERT_SUCCEEDED(CDisplayMetrics::New((IDisplayMetrics**)&metrics));
    AutoPtr<IDisplay> display;
    ASSERT_SUCCEEDED(wm->GetDefaultDisplay((IDisplay**)&display));
    ASSERT_SUCCEEDED(display->GetMetrics(metrics));
    Int32 dpi;
    metrics->GetNoncompatDensityDpi(&dpi);
    bm->SetDensity(dpi);

    Int32 w, h;
    if (width <= 0 || height <= 0
            || ((bm->GetWidth(&w), w == width)
                    && (bm->GetHeight(&h), h == height))) {
        return bm;
    }

    // This is the final bitmap we want to return.
    // try {
    AutoPtr<IBitmapFactory> factory;
    ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory));
    AutoPtr<IBitmap> newbm;
    ECode ec = factory->CreateBitmap(width, height,
            Elastos::Droid::Graphics::BitmapConfig_ARGB_8888, (IBitmap**)&newbm);
    if (ec == (ECode)E_OUT_OF_MEMORY_ERROR) {
        Slogger::W(TAG, "Can't generate default bitmap");
        return bm;
    }
    newbm->SetDensity(dpi);

    AutoPtr<ICanvas> c;
    ASSERT_SUCCEEDED(CCanvas::New(newbm, (ICanvas**)&c));
    AutoPtr<IRect> targetRect;
    ASSERT_SUCCEEDED(CRect::New((IRect**)&targetRect));
    Int32 targetRight, targetBottom;
    bm->GetWidth(&targetRight);
    bm->GetHeight(&targetBottom);
    targetRect->SetRight(targetRight);
    targetRect->SetBottom(targetBottom);

    Int32 deltaw = width - targetRight;
    Int32 deltah = height - targetBottom;

    if (deltaw > 0 || deltah > 0) {
        // We need to scale up so it covers the entire area.
        Float scale;
        if (deltaw > deltah) {
            scale = width / (Float)targetRight;
        }
        else {
            scale = height / (Float)targetBottom;
        }
        targetRight = (Int32)(Float)targetRight * scale;
        targetBottom = (Int32)(Float)targetBottom * scale;
        targetRect->SetRight(targetRight);
        targetRect->SetBottom(targetBottom);
        deltaw = width - targetRight;
        deltah = height - targetBottom;
    }

    targetRect->Offset(deltaw/2, deltah/2);

    AutoPtr<IPaint> paint;
    ASSERT_SUCCEEDED(CPaint::New((IPaint**)&paint));
    paint->SetFilterBitmap(TRUE);
    AutoPtr<IPorterDuffXfermode> mode;
    ASSERT_SUCCEEDED(CPorterDuffXfermode::New(Elastos::Droid::Graphics::PorterDuffMode_SRC,
            (IPorterDuffXfermode**)&mode));
    paint->SetXfermode((IXfermode*)mode.Get());
    c->DrawBitmap(bm, NULL, targetRect, paint);

    bm->Recycle();
    c->SetBitmap(NULL);
    return newbm;
    // } catch (OutOfMemoryError e) {
    //     Log.w(TAG, "Can't generate default bitmap", e);
    //     return bm;
    // }
}

ECode CWallpaperManager::constructor(
    /* [in] */ IContext *context,
    /* [in] */ IHandler *handler)
{
    mContext = context;
    AutoPtr<ILooper> looper;
    FAIL_RETURN(context->GetMainLooper((ILooper**)&looper));
    InitGlobals(looper);
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
