
#include "webkit/CWebIconDatabaseHelper.h"
#include "webkit/WebIconDatabase.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CWebIconDatabaseHelper::GetInstance(
    /* [out] */ IWebIconDatabase** database)
{
    VALIDATE_NOT_NULL(database);
    AutoPtr<IWebIconDatabase> temp = WebIconDatabase::GetInstance();
    *database = temp;
    INTERFACE_ADDREF(*database);
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos