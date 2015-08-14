#ifndef __ELASTOS_DROID_UTILITY_H_H__
#define __ELASTOS_DROID_UTILITY_H_H__

#include <Elastos.CoreLibrary.h>

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

#endif // __ELASTOS_DROID_UTILITY_H_H__
