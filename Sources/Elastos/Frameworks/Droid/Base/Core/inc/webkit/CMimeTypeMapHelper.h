
#ifndef __CMIMETYPEMAPHELPER_H__
#define __CMIMETYPEMAPHELPER_H__

#include "_CMimeTypeMapHelper.h"

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

#endif // __CMIMETYPEMAPHELPER_H__
