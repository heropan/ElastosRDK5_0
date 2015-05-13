
#ifndef __CDELETEONEXITHELPER_H__
#define __CDELETEONEXITHELPER_H__

#include "_CDeleteOnExitHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Luni {
namespace Util {

CarClass(CDeleteOnExitHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IDeleteOnExit** instance);
};

} // namespace Util
} // namespace Luni
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CDELETEONEXITHELPER_H__
