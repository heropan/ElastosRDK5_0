
#ifndef __ELASTOS_DROID_APP_CFRAGMENTHELPER_H__
#define __ELASTOS_DROID_APP_CFRAGMENTHELPER_H__

#include "_Elastos_Droid_App_CFragmentHelper.h"
#include <elastos/core/Singleton.h>

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CFragmentHelper)
    , public Singleton
    , public IFragmentHelper
{
public:
    CARAPI Instantiate(
        /* [in] */ IContext* context,
        /* [in] */ const String& fname,
        /* [out] */ IFragment** fragment);

    CARAPI Instantiate(
        /* [in] */ IContext* context,
        /* [in] */ const String& fname,
        /* [in] */ IBundle* args,
        /* [out] */ IFragment** fragment);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CFRAGMENTHELPER_H__
