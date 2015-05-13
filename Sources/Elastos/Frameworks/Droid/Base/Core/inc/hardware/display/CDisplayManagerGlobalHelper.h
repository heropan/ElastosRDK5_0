
#ifndef __CDISPLAYMANAGERGLOBALHELPER_H__
#define __CDISPLAYMANAGERGLOBALHELPER_H__

#include "_CDisplayManagerGlobalHelper.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Display {

CarClass(CDisplayManagerGlobalHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IDisplayManagerGlobal** global);
};

} // namespace Display
} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif //__CDISPLAYMANAGERGLOBALHELPER_H__
