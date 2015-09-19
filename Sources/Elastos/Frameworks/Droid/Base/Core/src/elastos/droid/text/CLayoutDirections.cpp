#include "ext/frameworkext.h"
#include "text/CLayoutDirections.h"

namespace Elastos {
namespace Droid {
namespace Text {

ECode CLayoutDirections::constructor(
    /* [in] */ ArrayOf<Int32>* directions)
{
    if (directions != NULL) {
        mDirections = directions->Clone();
    }
    return NOERROR;
}

ECode CLayoutDirections::SetDirections(
    /* [in] */ ArrayOf<Int32>* directions)
{
    mDirections = directions;
    return NOERROR;
}

ECode CLayoutDirections::GetDirections(
    /* [out, callee] */ ArrayOf<Int32>** directions)
{
    VALIDATE_NOT_NULL(directions);
    *directions = mDirections;
    REFCOUNT_ADD(*directions);
    return NOERROR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
