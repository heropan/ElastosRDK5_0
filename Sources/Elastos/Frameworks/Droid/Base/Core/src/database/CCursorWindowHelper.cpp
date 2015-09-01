
#include "database/CCursorWindowHelper.h"
#include "database/CursorWindow.h"

namespace Elastos {
namespace Droid {
namespace Database {

CAR_INTERFACE_IMPL(CCursorWindowHelper, Singleton, ICursorWindowHelper)

CAR_SINGLETON_IMPL(CCursorWindowHelper)

ECode  CCursorWindowHelper::NewFromParcel(
    /* [in] */ IParcel* p,
    /* [out] */ ICursorWindow** result)
{
    VALIDATE_NOT_NULL(result)
    return CursorWindow::NewFromParcel(p, result);
}

} //Database
} //Droid
} //Elastos