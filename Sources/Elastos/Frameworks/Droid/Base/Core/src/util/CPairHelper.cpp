
#include "util/CPairHelper.h"
#include "util/CPair.h"

namespace Elastos {
namespace Droid {
namespace Utility {

ECode CPairHelper::Create(
    /* [in] */ IInterface *A,
    /* [in] */ IInterface *B,
    /* [out] */ IPair **pair)
{
    return CPair::Create(A, B, pair);
}

} // namespace Utility
} // namespace Droid
} // namespace Elastos
