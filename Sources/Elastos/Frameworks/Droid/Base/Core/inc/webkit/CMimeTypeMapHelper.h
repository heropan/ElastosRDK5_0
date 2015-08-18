
#ifndef __ELASTOS_DROID_WEBKIT_CMIMETYPEMAPHELPER_H__
#define __ELASTOS_DROID_WEBKIT_CMIMETYPEMAPHELPER_H__

#include "_Elastos_Droid_Webkit_CMimeTypeMapHelper.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CMimeTypeMapHelper)
{
public:
    CARAPI GetFileExtensionFromUrl(
        /* [in] */ const String& url,
        /* [out] */ String* fileExtension);

    CARAPI GetSingleton(
        /* [out] */ IMimeTypeMap** mimeTypeMap);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CMIMETYPEMAPHELPER_H__
