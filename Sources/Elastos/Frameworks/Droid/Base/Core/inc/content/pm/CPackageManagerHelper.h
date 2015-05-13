
#ifndef __CPACKAGEMANAGERHELPER_H__
#define __CPACKAGEMANAGERHELPER_H__

#include "_CPackageManagerHelper.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CarClass(CPackageManagerHelper)
{
public:
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

#endif // __CPACKAGEMANAGERHELPER_H__
