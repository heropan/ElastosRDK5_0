
#ifndef __CFRAGMENTHELPER_H__
#define __CFRAGMENTHELPER_H__

#include "_CFragmentHelper.h"
#include "app/Fragment.h"

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CFragmentHelper)
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

#endif //__CFRAGMENTHELPER_H__