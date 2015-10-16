
#include "elastos/droid/media/CMediaFileType.h"
#include "elastos/droid/ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Media {

CMediaFileType::CMediaFileType()
    : mFileType(0)
{}

CMediaFileType::~CMediaFileType()
{}

ECode CMediaFileType::constructor(
    /* [in] */ Int32 fileType,
    /* [in] */ const String& mimeType)
{
    mFileType = fileType;
    mMimeType = mimeType;
    return NOERROR;
}

ECode CMediaFileType::SetFileType(
    /* [in] */ Int32 result)
{
    mFileType = result;
    return NOERROR;
}

ECode CMediaFileType::SetMimeType(
    /* [in] */ const String& result)
{
    mMimeType = result;
    return NOERROR;
}

ECode CMediaFileType::GetFileType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mFileType;
    return NOERROR;
}

ECode CMediaFileType::GetMimeType(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mMimeType;
    return NOERROR;
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
