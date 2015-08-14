
#ifndef __ELASTOS_DROID_WEBKIT_CWEBICONDATABASEHELPER_H__
#define __ELASTOS_DROID_WEBKIT_CWEBICONDATABASEHELPER_H__

#include "_CWebIconDatabaseHelper.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebIconDatabaseHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IWebIconDatabase** database);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CWEBICONDATABASEHELPER_H__
