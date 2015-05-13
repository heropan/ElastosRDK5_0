
#ifndef __CNINEPATCHHELPER_H__
#define __CNINEPATCHHELPER_H__

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

#endif // __CNINEPATCHHELPER_H__

