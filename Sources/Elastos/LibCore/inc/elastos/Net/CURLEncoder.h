#ifndef __CURLENCODER_H__
#define __CURLENCODER_H__

#include "cmdef.h"
#include "_CURLEncoder.h"
#include "UriCodec.h"

namespace Elastos {
namespace Net {

CarClass(CURLEncoder)
{
public:
    CARAPI Encode(
        /* [in] */ const String& s,
        /* [out] */ String* result);

    CARAPI EncodeEx(
        /* [in] */ const String& s,
        /* [in] */ const String& charsetName,
        /* [out] */ String* result);
private:
    class UriCodecEx : public UriCodec
    {
        Boolean IsRetained(
            /* [in] */ char c) const
        {
            String str(" .-*_");
            return str.IndexOf(c) != -1;
        }
    };

    UriCodecEx ENCODER;
};

} // namespace Net
} // namespace Elastos

#endif //__URLENCODER_H__
