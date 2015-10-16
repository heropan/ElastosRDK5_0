
#ifndef __ELASTOS_DROID_WEBKIT_CWEBVIEWDATABASE_H__
#define __ELASTOS_DROID_WEBKIT_CWEBVIEWDATABASE_H__

#include "_Elastos_Droid_Webkit_CWebViewDatabase.h"
#include "elastos/droid/webkit/WebViewDatabaseClassic.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebViewDatabase), public WebViewDatabase
{
public:
    CARAPI HasUsernamePassword(
        /* [out] */ Boolean* result);

    CARAPI ClearUsernamePassword();

    CARAPI HasHttpAuthUsernamePassword(
        /* [out] */ Boolean* result);

    CARAPI ClearHttpAuthUsernamePassword();

    CARAPI HasFormData(
        /* [out] */ Boolean* result);

    CARAPI ClearFormData();
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CWEBVIEWDATABASE_H__
