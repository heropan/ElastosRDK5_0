
#ifndef __ELASTOS_DROID_INTERNAL_APP_CDISABLECARMODEACTIVITY_H__
#define __ELASTOS_DROID_INTERNAL_APP_CDISABLECARMODEACTIVITY_H__

#include "elastos/droid/app/Activity.h"
#include "_Elastos_Droid_Internal_App_CDisableCarModeActivity.h"

using Elastos::Droid::App::Activity;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

class CDisableCarModeActivity : public Activity
{
public:
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI constructor();

private:
    static const String TAG;
};

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_APP_CDISABLECARMODEACTIVITY_H__
