
#ifndef __CWEBSTORAGE_H__
#define __CWEBSTORAGE_H__

#include "_CWebStorage.h"
#include "webkit/WebStorage.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebStorage), public WebStorage
{
public:
    CARAPI constructor();

    CARAPI GetOrigins(
        /* [in] */ IValueCallback* callBack);

    CARAPI GetUsageForOrigin(
        /* [in] */ const String& origin,
        /* [in] */ IValueCallback* callBack);

    CARAPI GetQuotaForOrigin(
        /* [in] */ const String& origin,
        /* [in] */ IValueCallback* callBack);

    CARAPI SetQuotaForOrigin(
        /* [in] */ const String& origin,
        /* [in] */ Int64 quota);

    CARAPI DeleteOrigin(
        /* [in] */ const String& origin);

    CARAPI DeleteAllData();
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __CWEBSTORAGE_H__
