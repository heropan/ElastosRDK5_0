
#ifndef __ELASTOS_DROID_CONTENT_PM_CLAUNCHER_ACTIVITY_INFO_H__
#define __ELASTOS_DROID_CONTENT_PM_CLAUNCHER_ACTIVITY_INFO_H__

#include "_Elastos_Droid_Content_Pm_CLauncherActivityInfo.h"
#include "elastos/core/Object.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

/**
 * A representation of an activity that can belong to this user or a managed
 * profile associated with this user. It can be used to query the label, icon
 * and badged icon for the activity.
 */
CarClass(CLauncherActivityInfo)
    , public Object
    , public ILauncherActivityInfo
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()
};

}
}
}
}

#endif // __ELASTOS_DROID_CONTENT_PM_CLAUNCHER_ACTIVITY_INFO_H__
