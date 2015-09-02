
#include "webkit/native/base/PathService.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

const Int32 PathService::DIR_MODULE;

// Prevent instantiation.
PathService::PathService()
{
}

void PathService::Override(
    /* [in] */ Int32 what,
    /* [in] */ String path)
{
    NativeOverride(what, path);
}

void PathService::NativeOverride(
    /* [in] */ Int32 what,
    /* [in] */ String path)
{
}

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos