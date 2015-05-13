
#include "database/CCursorWindowHelper.h"
#include "database/CursorWindow.h"

namespace Elastos {
namespace Droid {
namespace Database {

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