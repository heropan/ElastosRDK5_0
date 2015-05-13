
#include "webkit/CMimeTypeMap.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

ECode CMimeTypeMap::HasMimeType(
    /* [in] */ const String& mimeType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = MimeTypeMap::HasMimeType(mimeType);
    return NOERROR;
}

ECode CMimeTypeMap::GetMimeTypeFromExtension(
    /* [in] */ const String& extension,
    /* [out] */ String* mimeType)
{
    VALIDATE_NOT_NULL(mimeType);
    *mimeType = MimeTypeMap::GetMimeTypeFromExtension(extension);
    return NOERROR;
}

ECode CMimeTypeMap::HasExtension(
    /* [in] */ const String& extension,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = MimeTypeMap::HasExtension(extension);
    return NOERROR;
}

ECode CMimeTypeMap::GetExtensionFromMimeType(
    /* [in] */ const String& mimeType,
    /* [out] */ String* extension)
{
    VALIDATE_NOT_NULL(extension);
    *extension = MimeTypeMap::GetExtensionFromMimeType(mimeType);
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos