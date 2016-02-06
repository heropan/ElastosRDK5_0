
#ifndef __ELASTOS_DROID_MEDIA_CROUTEGROUP_H__
#define __ELASTOS_DROID_MEDIA_CROUTEGROUP_H__

#include "_Elastos_Droid_Media_CRouteGroup.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/media/CMediaRouter.h"

using Elastos::Droid::Graphics::Drawable::IDrawable;

namespace Elastos {
namespace Droid {
namespace Media {

/**
 * Information about a route that consists of multiple other routes in a group.
 */
CarClass(CRouteGroup)
    , public CMediaRouter::RouteGroup
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CROUTEGROUP_H__
