
#include "webkit/CWebStorage.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CWebStorage::constructor()
{
    return NOERROR;
}

ECode CWebStorage::GetOrigins(
    /* [in] */ IValueCallback* callBack)
{
    return WebStorage::GetOrigins(callBack);
}

ECode CWebStorage::GetUsageForOrigin(
    /* [in] */ const String& origin,
    /* [in] */ IValueCallback* callBack)
{
    return WebStorage::GetUsageForOrigin(origin, callBack);
}

ECode CWebStorage::GetQuotaForOrigin(
    /* [in] */ const String& origin,
    /* [in] */ IValueCallback* callBack)
{
    return WebStorage::GetQuotaForOrigin(origin, callBack);
}

ECode CWebStorage::SetQuotaForOrigin(
    /* [in] */ const String& origin,
    /* [in] */ Int64 quota)
{
    return WebStorage::SetQuotaForOrigin(origin, quota);
}

ECode CWebStorage::DeleteOrigin(
    /* [in] */ const String& origin)
{
    return WebStorage::DeleteOrigin(origin);
}

ECode CWebStorage::DeleteAllData()
{
    return WebStorage::DeleteAllData();
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
