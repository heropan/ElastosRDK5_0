#include "graphics/TemporaryBuffer.h"
#include "util/ArrayUtils.h"

using Elastos::Droid::Internal::Utility::ArrayUtils;

namespace Elastos {
namespace Droid {
namespace Graphics {

AutoPtr< ArrayOf<Char32> > TemporaryBuffer::sTemp;
Mutex TemporaryBuffer::sClassLock;

AutoPtr< ArrayOf<Char32> > TemporaryBuffer::Obtain(
    /* [in] */ Int32 len)
{
    AutoPtr< ArrayOf<Char32> > buf;

    {
        AutoLock lock(sClassLock);

        buf = sTemp;
        sTemp = NULL;
    }

    if (buf == NULL || buf->GetLength() < len) {
        buf = ArrayOf<Char32>::Alloc(ArrayUtils::IdealCharArraySize(len));
    }

    return buf;
}

void TemporaryBuffer::Recycle(
    /* [in] */ ArrayOf<Char32>* temp)
{
    if (temp->GetLength() > 1000) return;

    {
        AutoLock lock(sClassLock);

        sTemp = temp;
    }
}


} // namespace Graphics
} // namepsace Droid
} // namespace Elastos