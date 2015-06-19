#ifndef __ELASTOS_IO_CCHARSETHELPER_H__
#define __ELASTOS_IO_CCHARSETHELPER_H__

#include "_Elastos_IO_Charset_CCharsetHelper.h"
#include "Singleton.h"


namespace Elastos {
namespace IO {
namespace Charset {

CarClass(CCharsetHelper)
    , public Singleton
    , public ICharsetHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

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

#endif //__ELASTOS_IO_CCHARSETHELPER_H__
