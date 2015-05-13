#ifndef __HH_DROID_NIO_UTILS_H
#define __HH_DROID_NIO_UTILS_H

#include <Elastos.Core.h>

using Elastos::IO::IBuffer;

namespace Elastos {
namespace Droid {
namespace Utility {

class AutoBufferPointer
{
public:
    AutoBufferPointer(
        /* [in] */ IBuffer* nioBuffer,
        /* [in] */ Boolean commit);

    ~AutoBufferPointer();

    void* Pointer() const { return mPointer; }

private:
    void*   mPointer;
//      jarray  fArray;
    Int32    mRemaining;
    Boolean mCommit;
};

} // namespace Utility
} // namespace Droid
} // namespace Elastos

#endif // __HH_DROID_NIO_UTILS_H
