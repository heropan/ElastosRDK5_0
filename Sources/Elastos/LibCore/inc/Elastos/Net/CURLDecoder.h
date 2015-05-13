#ifndef __CURLDCODER_H__
#define __CURLDCODER_H__

#include "cmdef.h"
#include "_CURLDecoder.h"

namespace Elastos {
namespace Net {

CarClass(CURLDecoder)
{
public:
    CARAPI Decode(
        /* [in] */ const String& s,
        /* [out] */ String* result);

    CARAPI DecodeEx(
        /* [in] */ const String& s,
        /* [in] */ const String& charsetName,
        /* [out] */ String* result);
};

} // namespace Net
} // namespace Elastos

#endif //__URLENCODER_H__
