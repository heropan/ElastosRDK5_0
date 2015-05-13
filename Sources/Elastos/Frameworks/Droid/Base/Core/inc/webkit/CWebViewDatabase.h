
#ifndef __CWEBVIEWDATABASE_H__
#define __CWEBVIEWDATABASE_H__

#include "_CWebViewDatabase.h"
#include "webkit/WebViewDatabaseClassic.h"

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

#endif // __CWEBVIEWDATABASE_H__
