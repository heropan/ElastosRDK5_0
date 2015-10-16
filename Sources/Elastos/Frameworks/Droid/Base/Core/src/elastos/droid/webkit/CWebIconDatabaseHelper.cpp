
#include "elastos/droid/webkit/CWebIconDatabaseHelper.h"
#include "elastos/droid/webkit/WebIconDatabase.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CWebIconDatabaseHelper::GetInstance(
    /* [out] */ IWebIconDatabase** database)
{
    VALIDATE_NOT_NULL(database);
    AutoPtr<IWebIconDatabase> temp = WebIconDatabase::GetInstance();
    *database = temp;
    REFCOUNT_ADD(*database);
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos