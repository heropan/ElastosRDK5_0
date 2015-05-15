#ifndef __CCHARSETHELPER_H__
#define __CCHARSETHELPER_H__

#include "_CCharsetHelper.h"

namespace Elastos {
namespace IO {
namespace Charset {

CarClass(CCharsetHelper)
{
public:
    // TODO:
    // public static SortedMap<String, Charset> availableCharsets();

    CARAPI ForName(
        /* [in] */ const String& charsetName,
        /* [out] */ ICharset** charset);

    CARAPI IsSupported(
        /* [in] */ const String& charsetName,
        /* [out] */ Boolean* result);

    CARAPI DefaultCharset(
        /* [out] */ICharset** charset);
};

} // namespace Charset
} // namespace IO
} // namespace Elastos

#endif //__CCHARSETHELPER_H__
