
#ifndef __ELASTOS_DROID_GRAPHICS_CNINEPATCHHELPER_H__
#define __ELASTOS_DROID_GRAPHICS_CNINEPATCHHELPER_H__

#include "_CNinePatchHelper.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CNinePatchHelper)
{
public:
    CARAPI IsNinePatchChunk(
        /* [in] */ ArrayOf<Byte>* chunk,
        /* [out] */ Boolean* result);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_CNINEPATCHHELPER_H__

