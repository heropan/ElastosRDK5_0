
#include "os/CBundleHelper.h"
#include "os/CBundle.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CBundleHelper::GetEmpty(
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle)
    *bundle = CBundle::EMPTY;
    REFCOUNT_ADD(*bundle);
    return NOERROR;
}

ECode CBundleHelper::ForPair(
    /* [in] */ const String& key,
    /* [in] */ const String& value,
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle)
    AutoPtr<IBundle> b = CBundle::ForPair(key, value);
    *bundle = b;
    REFCOUNT_ADD(*bundle)
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
