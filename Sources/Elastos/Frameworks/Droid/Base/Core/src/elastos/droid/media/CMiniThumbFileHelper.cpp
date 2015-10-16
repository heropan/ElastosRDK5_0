
#include "elastos/droid/media/CMiniThumbFileHelper.h"
#include "elastos/droid/media/CMiniThumbFile.h"

namespace Elastos {
namespace Droid {
namespace Media {

ECode CMiniThumbFileHelper::Reset()
{
    return CMiniThumbFile::Reset();
}

ECode CMiniThumbFileHelper::Instance(
    /* [in] */ IUri* uri,
    /* [out] */ IMiniThumbFile** result)
{
    return CMiniThumbFile::Instance(uri, result);
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
