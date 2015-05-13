
#include "ext/frameworkext.h"
#include "graphics/CNinePatchHelper.h"
#include "graphics/NinePatch.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CNinePatchHelper::IsNinePatchChunk(
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NinePatch::IsNinePatchChunk(chunk);
    return NOERROR;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
