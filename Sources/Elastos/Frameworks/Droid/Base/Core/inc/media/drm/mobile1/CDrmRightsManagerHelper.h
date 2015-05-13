
#ifndef __CDRMRIGHTSMANAGERHELPER_H__
#define __CDRMRIGHTSMANAGERHELPER_H__

#include "_CDrmRightsManagerHelper.h"

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

#endif // __CDRMRIGHTSMANAGERHELPER_H__
