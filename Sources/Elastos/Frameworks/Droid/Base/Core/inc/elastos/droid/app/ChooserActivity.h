
#ifndef __ELASTOS_DROID_INTERNAL_APP_CHOOSERACTIVITY_H__
#define __ELASTOS_DROID_INTERNAL_APP_CHOOSERACTIVITY_H__

#include "elastos/droid/app/ResolverActivity.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

class ChooserActivity
    : public ResolverActivity
{
public:
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);
};

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_APP_CHOOSERACTIVITY_H__
