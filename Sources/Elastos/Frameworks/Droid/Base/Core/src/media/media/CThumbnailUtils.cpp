
#include "media/CThumbnailUtils.h"
#include "media/ThumbnailUtils.h"

namespace Elastos {
namespace Droid {
namespace Media {

ECode CThumbnailUtils::CreateImageThumbnail(
    /* [in] */ const String& filePath,
    /* [in] */ Int32 kind,
    /* [out] */ IBitmap** result)
{
    VALIDATE_NOT_NULL(result);
    *result = ThumbnailUtils::CreateImageThumbnail(filePath, kind);
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CThumbnailUtils::CreateVideoThumbnail(
    /* [in] */ const String& filePath,
    /* [in] */ Int32 kind,
    /* [out] */ IBitmap** result)
{
    VALIDATE_NOT_NULL(result);
    *result = ThumbnailUtils::CreateVideoThumbnail(filePath, kind);
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CThumbnailUtils::ExtractThumbnail(
    /* [in] */ IBitmap* source,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [out] */ IBitmap** result)
{
    VALIDATE_NOT_NULL(result);
    *result = ThumbnailUtils::ExtractThumbnail(source, width, height);
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CThumbnailUtils::ExtractThumbnailEx(
    /* [in] */ IBitmap* source,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 options,
    /* [out] */ IBitmap** result)
{
    VALIDATE_NOT_NULL(result);
    *result = ThumbnailUtils::ExtractThumbnail(source, width, height, options);
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

} // namespace Media
} // namespace Droid
} // namespace Elastos
