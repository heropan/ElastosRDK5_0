
#include "ext/frameworkext.h"
#include "net/Uri.h"
#include "provider/CDownloadsImpl.h"
#include <elastos/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Droid::App::IDownloadManagerRequest;
using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CDownloadsImpl::GetCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://downloads/my_downloads"), uri);
}

ECode CDownloadsImpl::GetALLDOWNLOADSCONTENTURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://downloads/all_downloads"), uri);
}

ECode CDownloadsImpl::GetPUBLICLYACCESSIBLEDOWNLOADSURI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://downloads/") + IDownloadsImpl::PUBLICLY_ACCESSIBLE_DOWNLOADS_URI_SEGMENT, uri);
}

ECode CDownloadsImpl::IsStatusInformational(
    /* [in] */ Int32 status,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    *res = (status >= 100 && status < 200);
    return NOERROR;
}

ECode CDownloadsImpl::IsStatusSuccess(
    /* [in] */ Int32 status,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    *res = (status >= 200 && status < 300);
    return NOERROR;
}

ECode CDownloadsImpl::IsStatusError(
    /* [in] */ Int32 status,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    *res = (status >= 400 && status < 600);
    return NOERROR;
}

ECode CDownloadsImpl::IsStatusClientError(
    /* [in] */ Int32 status,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    *res = (status >= 400 && status < 500);
    return NOERROR;
}

ECode CDownloadsImpl::IsStatusServerError(
    /* [in] */ Int32 status,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    *res = (status >= 500 && status < 600);
    return NOERROR;
}

ECode CDownloadsImpl::IsNotificationToBeDisplayed(
    /* [in] */ Int32 visibility,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    *res = visibility == IDownloadManagerRequest::VISIBILITY_VISIBLE_NOTIFY_COMPLETED ||
            visibility == IDownloadManagerRequest::VISIBILITY_VISIBLE_NOTIFY_ONLY_COMPLETION;
    return NOERROR;
}

ECode CDownloadsImpl::IsStatusCompleted(
    /* [in] */ Int32 status,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);

    *res = (status >= 200 && status < 300) || (status >= 400 && status < 600);
    return NOERROR;
}

ECode CDownloadsImpl::StatusToString(
    /* [in] */ Int32 status,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);

    switch (status) {
        case STATUS_PENDING: *value = String("PENDING"); return NOERROR;
        case STATUS_RUNNING: *value = String("RUNNING"); return NOERROR;
        case STATUS_PAUSED_BY_APP: *value = String("PAUSED_BY_APP"); return NOERROR;
        case STATUS_WAITING_TO_RETRY: *value = String("WAITING_TO_RETRY"); return NOERROR;
        case STATUS_WAITING_FOR_NETWORK: *value = String("WAITING_FOR_NETWORK"); return NOERROR;
        case STATUS_QUEUED_FOR_WIFI: *value = String("QUEUED_FOR_WIFI"); return NOERROR;
        case STATUS_INSUFFICIENT_SPACE_ERROR: *value = String("INSUFFICIENT_SPACE_ERROR"); return NOERROR;
        case STATUS_DEVICE_NOT_FOUND_ERROR: *value = String("DEVICE_NOT_FOUND_ERROR"); return NOERROR;
        case STATUS_SUCCESS: *value = String("SUCCESS"); return NOERROR;
        case STATUS_BAD_REQUEST: *value = String("BAD_REQUEST"); return NOERROR;
        case STATUS_NOT_ACCEPTABLE: *value = String("NOT_ACCEPTABLE"); return NOERROR;
        case STATUS_LENGTH_REQUIRED: *value = String("LENGTH_REQUIRED"); return NOERROR;
        case STATUS_PRECONDITION_FAILED: *value = String("PRECONDITION_FAILED"); return NOERROR;
        case STATUS_FILE_ALREADY_EXISTS_ERROR: *value = String("FILE_ALREADY_EXISTS_ERROR"); return NOERROR;
        case STATUS_CANNOT_RESUME: *value = String("CANNOT_RESUME"); return NOERROR;
        case STATUS_CANCELED: *value = String("CANCELED"); return NOERROR;
        case STATUS_UNKNOWN_ERROR: *value = String("UNKNOWN_ERROR"); return NOERROR;
        case STATUS_FILE_ERROR: *value = String("FILE_ERROR"); return NOERROR;
        case STATUS_UNHANDLED_REDIRECT: *value = String("UNHANDLED_REDIRECT"); return NOERROR;
        case STATUS_UNHANDLED_HTTP_CODE: *value = String("UNHANDLED_HTTP_CODE"); return NOERROR;
        case STATUS_HTTP_DATA_ERROR: *value = String("HTTP_DATA_ERROR"); return NOERROR;
        case STATUS_HTTP_EXCEPTION: *value = String("HTTP_EXCEPTION"); return NOERROR;
        case STATUS_TOO_MANY_REDIRECTS: *value = String("TOO_MANY_REDIRECTS"); return NOERROR;
        case STATUS_BLOCKED: *value = String("BLOCKED"); return NOERROR;
        default: *value = StringUtils::Int32ToString(status); return NOERROR;
    }
}

}
}
}
