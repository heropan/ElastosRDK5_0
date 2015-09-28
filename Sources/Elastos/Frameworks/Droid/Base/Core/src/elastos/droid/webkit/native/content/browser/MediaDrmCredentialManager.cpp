
#include "webkit/native/content/browser/MediaDrmCredentialManager.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                      MediaDrmCredentialManager
//=====================================================================

ECode MediaDrmCredentialManager::ResetCredentials(
    /* [in] */ MediaDrmCredentialManagerCallback* callback)
{
    NativeResetCredentials(callback);
    return NOERROR;
}

ECode MediaDrmCredentialManager::NativeResetCredentials(
    /* [in] */ MediaDrmCredentialManagerCallback* callback)
{
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
