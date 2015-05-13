
#include "ext/frameworkext.h"
#include "CMediaImageItemHelper.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

ECode CMediaImageItemHelper::NextPowerOf2(
    /* [in] */ Int32 n,
    /* [out] */ Int32* result)
{
    return CMediaImageItem::NextPowerOf2(n, result);
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
