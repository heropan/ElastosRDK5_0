
#ifndef __IDN_H__
#define __IDN_H__

#include "Elastos.Core_server.h"

namespace Elastos {
namespace Net {

class IDN
{
public:
    static CARAPI ToASCII(
        /* [in] */ const String& input,
        /* [in] */ Int32 flags,
        /* [out] */ String* result);

    static CARAPI ToASCII(
        /* [in] */ const String& input,
        /* [out] */ String* result);

    static CARAPI ToUnicode(
        /* [in] */ const String& input,
        /* [in] */ Int32 flags,
        /* [out] */ String* result);

    static CARAPI ToUnicode(
        /* [in] */ const String& input,
        /* [out] */ String* result);

private:
    IDN() {}

    static CARAPI Convert(
        /* [in] */ const String& s,
        /* [in] */ Int32 flags,
        /* [in] */ Boolean toAscii,
        /* [out] */ String* result);

    static CARAPI NativeConvert(
        /* [in] */ const String& s,
        /* [in] */ Int32 flags,
        /* [in] */ Boolean toAscii,
        /* [out] */ String* result);
};

} // namespace Net
} // namespace Elastos

#endif //__IDN_H__
