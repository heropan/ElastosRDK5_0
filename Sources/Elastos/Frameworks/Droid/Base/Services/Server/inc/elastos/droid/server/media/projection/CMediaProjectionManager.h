
#ifndef __ELASTOS_DROID_SERVER_MEDIA_PROJECTION_CMEDIAPROJECTIONMANAGER_H__
#define __ELASTOS_DROID_SERVER_MEDIA_PROJECTION_CMEDIAPROJECTIONMANAGER_H__

#include "_Elastos_Droid_Server_Media_Projection_CMediaProjectionManager.h"
#include "elastos/droid/server/media/projection/MediaProjectionManagerService.h"

using Elastos::Droid::Media::Projection::IIMediaProjectionWatcherCallback;
using Elastos::Droid::Media::Projection::IIMediaProjectionManager;
using Elastos::Droid::Media::Projection::IIMediaProjection;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Media {
namespace Projection {

CarClass(CMediaProjectionManager)
    , public Object
    , public IIMediaProjectionManager
    , public IBinder
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ Handle64 host);

    CARAPI HasProjectionPermission(
        /* [in] */ Int32 uid,
        /* [in] */ const String& packageName,
        /* [out] */ Boolean* result);

    CARAPI CreateProjection(
        /* [in] */ Int32 uid,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 type,
        /* [in] */ Boolean permanentGrant,
        /* [out] */ IIMediaProjection** result);

    CARAPI IsValidMediaProjection(
        /* [in] */ IIMediaProjection* projection,
        /* [out] */ Boolean* result);

    CARAPI GetActiveProjectionInfo(
        /* [out] */ IMediaProjectionInfo** result);

    CARAPI StopActiveProjection();

    CARAPI AddCallback(
        /* [in] */ IIMediaProjectionWatcherCallback* cb);

    CARAPI RemoveCallback(
        /* [in] */ IIMediaProjectionWatcherCallback* cb);

    // @Override // Binder call
    // void dump(FileDescriptor fd, final PrintWriter pw, String[] args);

private:
    CARAPI_(Boolean) CheckPermission(
        /* [in] */ const String& packageName,
        /* [in] */ const String& permission);

private:
    MediaProjectionManagerService* mHost;
};

} // namespace Projection
} // namespace Media
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_MEDIA_PROJECTION_CMEDIAPROJECTIONMANAGER_H__
