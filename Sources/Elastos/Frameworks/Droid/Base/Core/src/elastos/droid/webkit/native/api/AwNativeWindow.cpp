#include "elastos/droid/webkit/native/api/AwNativeWindow.h"
#include "elastos/droid/webkit/native/api/NativeWindow_dec.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::View::ISurface;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Api {

ANativeWindow* AwNativeWindow::GetFromSurface(
    /* [in] */ IInterface* obj)
{
    AutoPtr<ISurface> surface = ISurface::Probe(obj);
    if (NULL == surface)
    {
        Logger::E("AwNativeWindow", "AwNativeWindow::GetFromSurface, surface is NULL");
        return NULL;
    }
    //TODO AutoPtr<CSurface> cs = (CSurface*)surface.Get();
    //CSurface define a static function return ANativeWindow from mNativeObject;= reinterpret_cast<Surface *>(mNativeObject);
    //ANativeWindow_fromSurface  ->>  base/native/android/native_window.cpp
    //    |
    //android_view_Surface_getNativeWindow  ->> base/core/jni/android_view_Surface.cpp
    /*
   ANativeWindow* GetSurface()
   {
       sp<android::Surface> surface(reinterpret_cast<android::Surface *>(mNativeObject));
       if (surface != NULL) {
           surface->incStrong(&sRefBaseOwner);
       }
       return Surface.get();
   }
   */
    return NULL;
    //TODO return cs->GetSurface();
}

} // namespace Api
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
