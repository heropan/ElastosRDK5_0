
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

/** {@hide} */
const String CWallpaperManager::PROP_WALLPAPER("ro.config.wallpaper");
/** {@hide} */
const String CWallpaperManager::PROP_WALLPAPER_COMPONENT("ro.config.wallpaper_component");

CAR_INTERFACE_IMPL(CWallpaperManager, Object, IWallpaperManager)

CAR_OBJECT_IMPL(CWallpaperManager)

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

ECode CWallpaperManager::GetBuiltInDrawable(
    /* [out] */ IDrawable** drawable)
{
    return GetBuiltInDrawable(0, 0, false, 0, 0, drawable);
}

ECode CWallpaperManager::GetBuiltInDrawable(
    /* [in] */ Int32 outWidth,
    /* [in] */ Int32 outHeight,
    /* [in] */ Boolean scaleToFit,
    /* [in] */ Float horizontalAlignment,
    /* [in] */ Float verticalAlignment
    /* [out] */ IDrawable** drawable)
{
    if (sGlobals.mService == null) {
        Log.w(TAG, "WallpaperService not running");
        return null;
    }
    Resources resources = mContext.getResources();
    horizontalAlignment = Math.max(0, Math.min(1, horizontalAlignment));
    verticalAlignment = Math.max(0, Math.min(1, verticalAlignment));

    InputStream is = new BufferedInputStream(openDefaultWallpaper(mContext));

    if (is == null) {
        Log.e(TAG, "default wallpaper input stream is null");
        return null;
    } else {
        if (outWidth <= 0 || outHeight <= 0) {
            Bitmap fullSize = BitmapFactory.decodeStream(is, null, null);
            return new BitmapDrawable(resources, fullSize);
        } else {
            int inWidth;
            int inHeight;
            {
                BitmapFactory.Options options = new BitmapFactory.Options();
                options.inJustDecodeBounds = true;
                BitmapFactory.decodeStream(is, null, options);
                if (options.outWidth != 0 && options.outHeight != 0) {
                    inWidth = options.outWidth;
                    inHeight = options.outHeight;
                } else {
                    Log.e(TAG, "default wallpaper dimensions are 0");
                    return null;
                }
            }

            is = new BufferedInputStream(openDefaultWallpaper(mContext));

            RectF cropRectF;

            outWidth = Math.min(inWidth, outWidth);
            outHeight = Math.min(inHeight, outHeight);
            if (scaleToFit) {
                cropRectF = getMaxCropRect(inWidth, inHeight, outWidth, outHeight,
                    horizontalAlignment, verticalAlignment);
            } else {
                float left = (inWidth - outWidth) * horizontalAlignment;
                float right = left + outWidth;
                float top = (inHeight - outHeight) * verticalAlignment;
                float bottom = top + outHeight;
                cropRectF = new RectF(left, top, right, bottom);
            }
            Rect roundedTrueCrop = new Rect();
            cropRectF.roundOut(roundedTrueCrop);

            if (roundedTrueCrop.width() <= 0 || roundedTrueCrop.height() <= 0) {
                Log.w(TAG, "crop has bad values for full size image");
                return null;
            }

            // See how much we're reducing the size of the image
            int scaleDownSampleSize = Math.min(roundedTrueCrop.width() / outWidth,
                    roundedTrueCrop.height() / outHeight);

            // Attempt to open a region decoder
            BitmapRegionDecoder decoder = null;
            try {
                decoder = BitmapRegionDecoder.newInstance(is, true);
            } catch (IOException e) {
                Log.w(TAG, "cannot open region decoder for default wallpaper");
            }

            Bitmap crop = null;
            if (decoder != null) {
                // Do region decoding to get crop bitmap
                BitmapFactory.Options options = new BitmapFactory.Options();
                if (scaleDownSampleSize > 1) {
                    options.inSampleSize = scaleDownSampleSize;
                }
                crop = decoder.decodeRegion(roundedTrueCrop, options);
                decoder.recycle();
            }

            if (crop == null) {
                // BitmapRegionDecoder has failed, try to crop in-memory
                is = new BufferedInputStream(openDefaultWallpaper(mContext));
                Bitmap fullSize = null;
                if (is != null) {
                    BitmapFactory.Options options = new BitmapFactory.Options();
                    if (scaleDownSampleSize > 1) {
                        options.inSampleSize = scaleDownSampleSize;
                    }
                    fullSize = BitmapFactory.decodeStream(is, null, options);
                }
                if (fullSize != null) {
                    crop = Bitmap.createBitmap(fullSize, roundedTrueCrop.left,
                            roundedTrueCrop.top, roundedTrueCrop.width(),
                            roundedTrueCrop.height());
                }
            }

            if (crop == null) {
                Log.w(TAG, "cannot decode default wallpaper");
                return null;
            }

            // Scale down if necessary
            if (outWidth > 0 && outHeight > 0 &&
                    (crop.getWidth() != outWidth || crop.getHeight() != outHeight)) {
                Matrix m = new Matrix();
                RectF cropRect = new RectF(0, 0, crop.getWidth(), crop.getHeight());
                RectF returnRect = new RectF(0, 0, outWidth, outHeight);
                m.setRectToRect(cropRect, returnRect, Matrix.ScaleToFit.FILL);
                Bitmap tmp = Bitmap.createBitmap((int) returnRect.width(),
                        (int) returnRect.height(), Bitmap.Config.ARGB_8888);
                if (tmp != null) {
                    Canvas c = new Canvas(tmp);
                    Paint p = new Paint();
                    p.setFilterBitmap(true);
                    c.drawBitmap(crop, m, p);
                    crop = tmp;
                }
            }

            return new BitmapDrawable(resources, crop);
        }
    }
}

static AutoPtr<IRectF> GetMaxCropRect(
    /* [in] */ Int32 inWidth,
    /* [in] */ Int32 inHeight,
    /* [in] */ Int32 outWidth,
    /* [in] */ Int32 outHeight,
    /* [in] */ Float horizontalAlignment,
    /* [in] */ Float verticalAlignment)
{
    RectF cropRect = new RectF();
    // Get a crop rect that will fit this
    if (inWidth / (float) inHeight > outWidth / (float) outHeight) {
         cropRect.top = 0;
         cropRect.bottom = inHeight;
         float cropWidth = outWidth * (inHeight / (float) outHeight);
         cropRect.left = (inWidth - cropWidth) * horizontalAlignment;
         cropRect.right = cropRect.left + cropWidth;
    } else {
        cropRect.left = 0;
        cropRect.right = inWidth;
        float cropHeight = outHeight * (inWidth / (float) outWidth);
        cropRect.top = (inHeight - cropHeight) * verticalAlignment;
        cropRect.bottom = cropRect.top + cropHeight;
    }
    return cropRect;
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

ECode CWallpaperManager::GetCropAndSetWallpaperIntent(
    /* [in] */ IUri* imageUri,
    /* [out] */ IIntent** intent)
{
    if (imageUri == NULL) {
        throw new IllegalArgumentException("Image URI must not be null");
    }

    if (!ContentResolver.SCHEME_CONTENT.equals(imageUri.getScheme())) {
        throw new IllegalArgumentException("Image URI must be of the "
                + ContentResolver.SCHEME_CONTENT + " scheme type");
    }

    final PackageManager packageManager = mContext.getPackageManager();
    Intent cropAndSetWallpaperIntent =
            new Intent(ACTION_CROP_AND_SET_WALLPAPER, imageUri);
    cropAndSetWallpaperIntent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);

    // Find out if the default HOME activity supports CROP_AND_SET_WALLPAPER
    Intent homeIntent = new Intent(Intent.ACTION_MAIN).addCategory(Intent.CATEGORY_HOME);
    ResolveInfo resolvedHome = packageManager.resolveActivity(homeIntent,
            PackageManager.MATCH_DEFAULT_ONLY);
    if (resolvedHome != null) {
        cropAndSetWallpaperIntent.setPackage(resolvedHome.activityInfo.packageName);

        List<ResolveInfo> cropAppList = packageManager.queryIntentActivities(
                cropAndSetWallpaperIntent, 0);
        if (cropAppList.size() > 0) {
            return cropAndSetWallpaperIntent;
        }
    }

    // fallback crop activity
    cropAndSetWallpaperIntent.setPackage("com.android.wallpapercropper");
    List<ResolveInfo> cropAppList = packageManager.queryIntentActivities(
            cropAndSetWallpaperIntent, 0);
    if (cropAppList.size() > 0) {
        return cropAndSetWallpaperIntent;
    }

    // If the URI is not of the right type, or for some reason the system wallpaper
    // cropper doesn't exist, return null
    // throw new IllegalArgumentException("Cannot use passed URI to set wallpaper; " +
    //     "check that the type returned by ContentProvider matches image/*");
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
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
    /**
     * The framework makes no attempt to limit the window size
     * to the maximum texture size. Any window larger than this
     * cannot be composited.
     *
     * Read maximum texture size from system property and scale down
     * minimumWidth and minimumHeight accordingly.
     */
    int maximumTextureSize;
    try {
        maximumTextureSize = SystemProperties.getInt("sys.max_texture_size", 0);
    } catch (Exception e) {
        maximumTextureSize = 0;
    }

    if (maximumTextureSize > 0) {
        if ((minimumWidth > maximumTextureSize) ||
            (minimumHeight > maximumTextureSize)) {
            float aspect = (float)minimumHeight / (float)minimumWidth;
            if (minimumWidth > minimumHeight) {
                minimumWidth = maximumTextureSize;
                minimumHeight = (int)((minimumWidth * aspect) + 0.5);
            } else {
                minimumHeight = maximumTextureSize;
                minimumWidth = (int)((minimumHeight / aspect) + 0.5);
            }
        }
    }


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

ECode CWallpaperManager::SetDisplayPadding(
    /* [in] */ IRect* padding)
{
    try {
        if (sGlobals.mService == null) {
            Log.w(TAG, "WallpaperService not running");
        } else {
            sGlobals.mService.setDisplayPadding(padding);
        }
    } catch (RemoteException e) {
        // Ignore
    }
    return NOERROR;
}

ECode CWallpaperManager::SetDisplayOffset(
    /* [in] */ IBinder* windowToken,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    try {
        //Log.v(TAG, "Sending new wallpaper display offsets from app...");
        WindowManagerGlobal.getWindowSession().setWallpaperDisplayOffset(
                windowToken, x, y);
        //Log.v(TAG, "...app returning after sending display offset!");
    } catch (RemoteException e) {
        // Ignore.
    }
}

ECode CWallpaperManager::SetWallpaperOffsets(
    /* [in] */ IBinder* windowToken,
    /* [in] */ Float xOffset,
    /* [in] */ Float yOffset)
{
    // try {
    //Log.v(TAG, "Sending new wallpaper offsets from app...");
    return CWindowManagerGlobal::GetWindowSession()->SetWallpaperPosition(
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
    AutoPtr<IBundle> b;
    return CWindowManagerGlobal::GetWindowSession()->SendWallpaperCommand(
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
    return CWindowManagerGlobal::GetWindowSession()->SetWallpaperPosition(
            windowToken, -1, -1, -1, -1);
    // } catch (RemoteException e) {
    //     // Ignore.
    // }
}

ECode CWallpaperManager::Clear()
{
    return SetStream(OpenDefaultWallpaper(mContext));
}

AutoPtr<IInputStream> CWallpaperManager::OpenDefaultWallpaper(
    /* [in] */ IContext* context)
{
    String path = SystemProperties::Get(PROP_WALLPAPER);
    if (!TextUtils::IsEmpty(path)) {
        AutoPtr<IFile> file;
        CFile::New(path, (IFile**)&file);
        Boolean exist;
        if (file->Exists(&exist), exist) {
            // try {
        AutoPtr<IInputStream> is;
        CFileInputStream::New(file, (IInputStream**)&is);
        return is;
            // } catch (IOException e) {
            //     // Ignored, fall back to platform default below
            // }
        }
    }
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IInputStream> is;
    res->OpenRawResource(R::drawable::default_wallpaper, (IInputStream**)&is);
    return is;
}

AutoPtr<IComponentName> CWallpaperManager::GetDefaultWallpaperComponent(
    /* [in] */ IContext* context)
{
    String flat = SystemProperties::Get(PROP_WALLPAPER_COMPONENT);
    if (!TextUtils::IsEmpty(flat)) {
        AutoPtr<IComponentName> cn = ComponentName::UnflattenFromString(flat);
        if (cn != NULL) {
            return cn;
        }
    }

    context->GetString(R::string::default_wallpaper_component, &flat);
    if (!TextUtils::IsEmpty(flat)) {
        AutoPtr<IComponentName> cn = ComponentName::UnflattenFromString(flat);
        if (cn != NULL) {
            return cn;
        }
    }

    return NULL;
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
