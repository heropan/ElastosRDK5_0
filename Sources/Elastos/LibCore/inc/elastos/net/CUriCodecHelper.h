#ifndef __URICODECHELPER_H__
#define __URICODECHELPER_H__

#include "_CUriCodecHelper.h"

using Elastos::IO::Charset::ICharset;

namespace Elastos {
namespace Net {

CarClass(CUriCodecHelper)
{
public :

    CARAPI ValidateSimple(
        /* [in] */ const String& s,
        /* [in] */ const String& legal);

    CARAPI Decode(
        /* [in] */ const String& s,
        /* [out] */ String* result);

    CARAPI Decode(
        /* [in] */ const String& s,
        /* [in] */ Boolean convertPlus,
        /* [in] */ ICharset* charset,
        /* [in] */ Boolean throwOnFailure,
        /* [out] */ String* result);
};

} // namespace Net
} // namespace Elastos

#endif // __URICODECHELPER_H__
