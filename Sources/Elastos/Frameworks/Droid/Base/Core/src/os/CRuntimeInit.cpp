
#include "os/CRuntimeInit.h"

using Elastos::Core::IRunnable;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Os {


ECode CRuntimeInit::Main(
    /* [in] */ const ArrayOf<String>& args)
{
    return RuntimeInit::Main(args);
}

} // namespace Os
} // namespace Droid
} // namespace Internal
} // namespace Elastos
