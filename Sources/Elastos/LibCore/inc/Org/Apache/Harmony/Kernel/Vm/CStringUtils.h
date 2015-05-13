
#ifndef __CSTRINGUTILS_H__
#define __CSTRINGUTILS_H__

#include "_CStringUtils.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Kernel {
namespace Vm {

CarClass(CStringUtils)
{
public:
    CARAPI CombineStrings(
        /* [in] */ ArrayOf<PInterface>* list,
        /* [out] */ String* cmbStr);

private:
    // TODO: Add your private member variables here.
};

} // namespace Vm
} // namespace Kernel
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CSTRINGUTILS_H__
