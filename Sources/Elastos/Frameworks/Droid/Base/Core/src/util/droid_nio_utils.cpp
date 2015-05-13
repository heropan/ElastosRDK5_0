
#include "util/droid_nio_utils.h"

namespace Elastos {
namespace Droid {
namespace Utility {

///////////////////////////////////////////////////////////////////////////////

AutoBufferPointer::AutoBufferPointer(
    /* [in] */ IBuffer* nioBuffer,
    /* [in] */ Boolean commit)
{
    assert(0);
    // fEnv = env;
    // fCommit = commit;
    // fPointer = android::nio_getPointer(env, nioBuffer, &fArray);
}

AutoBufferPointer::~AutoBufferPointer()
{
    assert(0);
    // if (NULL != fArray) {
    //     android::nio_releasePointer(fEnv, fArray, fPointer, fCommit);
    // }
}

} // namespace Utility
} // namespace Droid
} // namespace Elastos
