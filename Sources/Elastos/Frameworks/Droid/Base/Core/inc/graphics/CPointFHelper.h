
#ifndef __ELASTOS_DROID_GRAPHICS_CPOINTFHELPER_H__
#define __ELASTOS_DROID_GRAPHICS_CPOINTFHELPER_H__

#include "_Elastos_Droid_Graphics_CPointFHelper.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CPointFHelper)
{
public:
    CARAPI Length(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [out] */ Float* length);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_CPOINTFHELPER_H__
