
#ifndef __ELASTOS_DROID_WEBKIT_CMIMETYPEMAP_H__
#define __ELASTOS_DROID_WEBKIT_CMIMETYPEMAP_H__

#include "_Elastos_Droid_Webkit_CMimeTypeMap.h"
#include "elastos/droid/webkit/MimeTypeMap.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CMimeTypeMap), public MimeTypeMap
{
public:
    CARAPI HasMimeType(
        /* [in] */ const String& mimeType,
        /* [out] */ Boolean* result);

    CARAPI GetMimeTypeFromExtension(
        /* [in] */ const String& extension,
        /* [out] */ String* mimeType);

    CARAPI HasExtension(
        /* [in] */ const String& extension,
        /* [out] */ Boolean* result);

    CARAPI GetExtensionFromMimeType(
        /* [in] */ const String& mimeType,
        /* [out] */ String* extension);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CMIMETYPEMAP_H__
