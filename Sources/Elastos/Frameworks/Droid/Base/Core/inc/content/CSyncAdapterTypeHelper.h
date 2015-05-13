
#ifndef __CSYNCADAPTERTYPEHELPER_H__
#define __CSYNCADAPTERTYPEHELPER_H__

#include "_CSyncAdapterTypeHelper.h"

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CSyncAdapterTypeHelper)
{
public:
    CARAPI NewKey(
        /* [in] */ const String& authority,
        /* [in] */ const String& accountType,
        /* [out] */ ISyncAdapterType** syncAdapterType);

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __CSYNCADAPTERTYPEHELPER_H__
