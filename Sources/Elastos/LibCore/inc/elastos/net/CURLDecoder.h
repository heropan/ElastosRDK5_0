#ifndef __ELASTOS_NET_CURLDCODER_H__
#define __ELASTOS_NET_CURLDCODER_H__

#include "_ELASTOS_NET_CURLDecoder.h"
#include <elastos/core/Singleton.h>

using Elastos::Core::Singleton;

namespace Elastos {
namespace Net {

CarClass(CURLDecoder)
    : public Singleton
    , public IURLDecoderHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI Decode(
        /* [in] */ const String& s,
        /* [out] */ String* result);

    CARAPI Decode(
        /* [in] */ const String& s,
        /* [in] */ const String& charsetName,
        /* [out] */ String* result);
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_URLENCODER_H__
