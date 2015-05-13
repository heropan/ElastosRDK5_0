
#ifndef __CSURFACESESSION_H__
#define __CSURFACESESSION_H__

#include "_CSurfaceSession.h"
#include <gui/SurfaceComposerClient.h>

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CSurfaceSession)
{
    friend class CSurface;

public:
    CSurfaceSession();

    ~CSurfaceSession();

    /**
     * Forcibly detach native resources associated with this object.
     * Unlike destroy(), after this call any surfaces that were created
     * from the session will no longer work.
     */
    CARAPI Kill();

private:
    // Note: This field is accessed by native code.
    android::sp<android::SurfaceComposerClient> mNativeClient;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__CSURFACESESSION_H__
