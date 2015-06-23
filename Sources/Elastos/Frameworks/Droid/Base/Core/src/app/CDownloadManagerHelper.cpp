
#include "app/CDownloadManagerHelper.h"
#include "app/CDownloadManager.h"

namespace Elastos {
namespace Droid {
namespace App {

ECode CDownloadManagerHelper::GetUNDERLYINGCOLUMNS(
    /* [out] */ ArrayOf<String>** columns)
{
    VALIDATE_NOT_NULL(columns);

    *columns = CDownloadManager::UNDERLYING_COLUMNS;
    REFCOUNT_ADD(*columns);
    return NOERROR;
}

ECode CDownloadManagerHelper::GetMaxBytesOverMobile(
    /* [in] */ IContext* context,
    /* [out] */ Int64* size)
{
    return CDownloadManager::GetMaxBytesOverMobile(context, size);
}

ECode CDownloadManagerHelper::GetRecommendedMaxBytesOverMobile(
    /* [in] */ IContext* context,
    /* [out] */ Int64* size)
{
    return CDownloadManager::GetRecommendedMaxBytesOverMobile(context, size);
}

ECode CDownloadManagerHelper::IsActiveNetworkExpensive(
    /* [in] */ IContext* context,
    /* [out] */ Boolean* actived)
{
    return CDownloadManager::IsActiveNetworkExpensive(context, actived);
}

ECode CDownloadManagerHelper::GetActiveNetworkWarningBytes(
    /* [in] */ IContext* context,
    /* [out] */ Int64* bytes)
{
    return CDownloadManager::GetActiveNetworkWarningBytes(context, bytes);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
