
#ifndef __ELASTOS_DROID_CONTENT_PM_CPACKAGEMANAGERHELPER_H__
#define __ELASTOS_DROID_CONTENT_PM_CPACKAGEMANAGERHELPER_H__

#include "_Elastos_Droid_Content_Pm_CPackageManagerHelper.h"
#include <elastos/core/Singleton.h>

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CarClass(CPackageManagerHelper)
    , public Singleton
    , public IPackageManagerHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    /**
     * Returns the data directory for a particular user and package, given the uid of the package.
     * @param uid uid of the package, including the userId and appId
     * @param packageName name of the package
     * @return the user-specific data directory for the package
     * @hide
     */
    CARAPI GetDataDirForUser(
        /* [in] */ Int32 userId,
        /* [in] */ const String& packageName,
        /* [out] */ String * pDirectory);

};

}
}
}
}

#endif // __ELASTOS_DROID_CONTENT_PM_CPACKAGEMANAGERHELPER_H__
