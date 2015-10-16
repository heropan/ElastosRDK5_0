
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/app/CGlobalsWallpaperManagerCallback.h"
#include "elastos/droid/app/CWallpaperManager.h"
#include "elastos/droid/os/CBundle.h"
#include "elastos/droid/os/ServiceManager.h"
#include "elastos/droid/graphics/CBitmapFactoryOptions.h"
#include "elastos/droid/graphics/CBitmapFactory.h"
#include "elastos/droid/R.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IInputStream;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Graphics::IBitmapFactoryOptions;
using Elastos::Droid::Graphics::CBitmapFactoryOptions;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;

namespace Elastos{
namespace Droid{
namespace App{

const Int32 CGlobalsWallpaperManagerCallback::MSG_CLEAR_WALLPAPER = 1;

ECode CGlobalsWallpaperManagerCallback::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    switch(what) {
        case CGlobalsWallpaperManagerCallback::MSG_CLEAR_WALLPAPER:
            mHost->HandleClearWallpaper();
            break;
    }

    return NOERROR;
}

ECode CGlobalsWallpaperManagerCallback::OnWallpaperChanged()
{
    /* The wallpaper has changed but we shouldn't eagerly load the
     * wallpaper as that would be inefficient. Reset the cached wallpaper
     * to null so if the user requests the wallpaper again then we'll
     * fetch it.
     */
    Boolean result;
    return mHandler->SendEmptyMessage(MSG_CLEAR_WALLPAPER, &result);
}

AutoPtr<IBitmap> CGlobalsWallpaperManagerCallback::PeekWallpaperBitmap(
    /* [in] */ IContext* context,
    /* [in] */ Boolean returnDefault)
{
    AutoLock lock(mLock);
    if (mWallpaper != NULL) {
        return mWallpaper;
    }
    if (mDefaultWallpaper != NULL) {
        return mDefaultWallpaper;
    }
    mWallpaper = NULL;
    // try {
    mWallpaper = GetCurrentWallpaperLocked(context);
    // } catch (OutOfMemoryError e) {
    //     Log.w(TAG, "No memory load current wallpaper", e);
    // }
    if (returnDefault) {
        if (mWallpaper == NULL) {
            mDefaultWallpaper = GetDefaultWallpaperLocked(context);
            return mDefaultWallpaper;
        }
        else {
            mDefaultWallpaper = NULL;
        }
    }
    return mWallpaper;
}

void CGlobalsWallpaperManagerCallback::ForgetLoadedWallpaper()
{
    AutoLock lock(mLock);
    mWallpaper = NULL;
    mDefaultWallpaper = NULL;
}

AutoPtr<IBitmap> CGlobalsWallpaperManagerCallback::GetCurrentWallpaperLocked(
    /* [in] */ IContext* context)
{
    // try {
    AutoPtr<IBundle> params;
    AutoPtr<IParcelFileDescriptor> fd;
    ASSERT_SUCCEEDED(mService->GetWallpaper((IWallpaperManagerCallback*)this,
        (IBundle**)&params, (IParcelFileDescriptor**)&fd));
    if (fd != NULL) {
        Int32 width, height;
        params->GetInt32(String("width"), 0, &width);
        params->GetInt32(String("height"), 0, &height);

        // try {
        AutoPtr<IBitmapFactoryOptions> options;
        ASSERT_SUCCEEDED(CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&options));
        AutoPtr<IBitmapFactory> factory;
        ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory));
        AutoPtr<IFileDescriptor> desc;
        ASSERT_SUCCEEDED(fd->GetFileDescriptor((IFileDescriptor**)&desc));
        AutoPtr<IBitmap> bm;
        ASSERT_SUCCEEDED(factory->DecodeFileDescriptor(desc, NULL, options, (IBitmap**)&bm));
        fd->Close();
        return CWallpaperManager::GenerateBitmap(context, bm, width, height);
        // } catch (OutOfMemoryError e) {
        //     Log.w(TAG, "Can't decode file", e);
        // } finally {
        //     try {
        //         fd.close();
        //     } catch (IOException e) {
        //         // Ignore
        //     }
        // }
    }
    // } catch (RemoteException e) {
    //     // Ignore
    // }
    return NULL;
}

AutoPtr<IBitmap> CGlobalsWallpaperManagerCallback::GetDefaultWallpaperLocked(
    /* [in] */ IContext* context)
{
    // try {
    AutoPtr<IInputStream> is;
    AutoPtr<IResources> res;
    ASSERT_SUCCEEDED(context->GetResources((IResources**)&res));
    ASSERT_SUCCEEDED(res->OpenRawResource(R::drawable::default_wallpaper,
            (IInputStream**)&is));
    if (is != NULL) {
        Int32 width, height;
        mService->GetWidthHint(&width);
        mService->GetHeightHint(&height);

        // try {
        AutoPtr<IBitmapFactoryOptions> options;
        ASSERT_SUCCEEDED(CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&options));
        AutoPtr<IBitmapFactory> factory;
        ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory));
        AutoPtr<IBitmap> bm;
        ECode ec = factory->DecodeStream(is, NULL, options, (IBitmap**)&bm);
        if (ec == (ECode)E_OUT_OF_MEMORY_ERROR) {
            Slogger::W(String("GlobalsWallpaperManagerCallback"), "Can't decode stream");
            is->Close();
        }
        AutoPtr<IBitmap> result = CWallpaperManager::GenerateBitmap(context, bm, width, height);
        is->Close();
        return result;
        // } catch (OutOfMemoryError e) {
        //     Log.w(TAG, "Can't decode stream", e);
        // } finally {
        //     try {
        //         is.close();
        //     } catch (IOException e) {
        //         // Ignore
        //     }
        // }
    }
    // } catch (RemoteException e) {
    //     // Ignore
    // }
    return NULL;
}

void CGlobalsWallpaperManagerCallback::HandleClearWallpaper()
{
    AutoLock lock(mLock);
    mWallpaper = NULL;
    mDefaultWallpaper = NULL;
}

ECode CGlobalsWallpaperManagerCallback::constructor(
    /* [in] */ ILooper* looper)
{
    mService = (IIWallpaperManager*)ServiceManager::GetService(IContext::WALLPAPER_SERVICE).Get();
    mHandler = new MyHandler(this);
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
