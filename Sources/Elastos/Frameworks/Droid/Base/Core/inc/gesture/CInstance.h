#ifndef __CINSTANCE_H__
#define __CINSTANCE_H__

#include "_CInstance.h"
#include "Instance.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

CarClass(CInstance), public Instance
{
public:
    CARAPI Normalize();
};

}
}
}

#endif