
#ifndef __ELASTOS_DROID_MEDIA_CUSERROUTEINFO_H__
#define __ELASTOS_DROID_MEDIA_CUSERROUTEINFO_H__

#include "_Elastos_Droid_Media_CUserRouteInfo.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/media/CMediaRouter.h"

using Elastos::Droid::Graphics::Drawable::IDrawable;

namespace Elastos {
namespace Droid {
namespace Media {

/**
 * Information about a route that the application may define and modify.
 * A user route defaults to {@link RouteInfo#PLAYBACK_TYPE_REMOTE} and
 * {@link RouteInfo#PLAYBACK_VOLUME_FIXED}.
 *
 * @see MediaRouter.RouteInfo
 */
CarClass(CUserRouteInfo)
    , public CMediaRouter::UserRouteInfo
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CMEDIAROUTER_H__
