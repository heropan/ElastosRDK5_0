
#ifndef __CDISABLECARMODEACTIVITY_H__
#define __CDISABLECARMODEACTIVITY_H__

#include "app/Activity.h"
#include "_CDisableCarModeActivity.h"

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

#endif //__CDISABLECARMODEACTIVITY_H__