
#ifndef __CHOOSERACTIVITY_H__
#define __CHOOSERACTIVITY_H__

#include "app/ResolverActivity.h"

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

#endif //__CHOOSERACTIVITY_H__