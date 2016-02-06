
#ifndef __ELASTOS_DROID_MEDIA_CMEDIAROUTEINFO_H__
#define __ELASTOS_DROID_MEDIA_CMEDIAROUTEINFO_H__

#include "_Elastos_Droid_Media_CMediaRouteInfo.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/media/CMediaRouter.h"

using Elastos::Droid::Graphics::Drawable::IDrawable;

namespace Elastos {
namespace Droid {
namespace Media {

/**
 * Information about a media route.
 */
CarClass(CMediaRouteInfo)
    , public CMediaRouter::RouteInfo
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CMEDIAROUTEINFO_H__
