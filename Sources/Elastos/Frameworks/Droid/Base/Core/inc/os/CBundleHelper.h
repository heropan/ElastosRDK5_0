
#ifndef __ELASTOS_DROID_OS_CBUNDLEHELPER_H__
#define  __ELASTOS_DROID_OS_CBUNDLEHELPER_H__

#include "_CBundleHelper.h"

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CBundleHelper)
{
public:
    CARAPI GetEmpty(
        /* [out] */ IBundle** bundle);

    CARAPI ForPair(
        /* [in] */ const String& key,
        /* [in] */ const String& value,
        /* [out] */ IBundle** bundle);
};


} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_OS_CBUNDLEHELPER_H__
