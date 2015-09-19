
#include "content/pm/CPackageManagerHelper.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

ECode CPackageManagerHelper::GetDataDirForUser(
    /* [in] */ Int32 userId,
    /* [in] */ const String& packageName,
    /* [out] */ String * pDirectory)
{
    // TODO: This should be shared with Installer's knowledge of user directory
    // return Environment.getDataDirectory().toString() + "/user/" + userId
    //         + "/" + packageName;
    return E_NOT_IMPLEMENTED;
}

}
}
}
}
