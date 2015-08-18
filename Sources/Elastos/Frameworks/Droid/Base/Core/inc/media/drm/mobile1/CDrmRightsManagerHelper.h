
#ifndef __ELASTOS_DROID_DRM_MOBILE1_CDRMRIGHTSMANAGERHELPER_H__
#define __ELASTOS_DROID_DRM_MOBILE1_CDRMRIGHTSMANAGERHELPER_H__

#include "_Elastos_Droid_Drm_Mobile1_CDrmRightsManagerHelper.h"

namespace Elastos {
namespace Droid {
namespace Drm {
namespace Mobile1 {

CarClass(CDrmRightsManagerHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IDrmRightsManager** manager);
};

} // namespace mobile1
} // namespace drm
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_DRM_MOBILE1_CDRMRIGHTSMANAGERHELPER_H__
