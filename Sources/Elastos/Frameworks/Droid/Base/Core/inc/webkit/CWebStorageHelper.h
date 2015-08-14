
#ifndef __ELASTOS_DROID_WEBKIT_CWEBSTORAGEHELPER_H__
#define __ELASTOS_DROID_WEBKIT_CWEBSTORAGEHELPER_H__

#include "_CWebStorageHelper.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebStorageHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IWebStorage** instance);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CWEBSTORAGEHELPER_H__
