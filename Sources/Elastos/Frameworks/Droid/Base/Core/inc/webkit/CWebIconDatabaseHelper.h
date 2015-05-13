
#ifndef __CWEBICONDATABASEHELPER_H__
#define __CWEBICONDATABASEHELPER_H__

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

#endif // __CWEBICONDATABASEHELPER_H__
