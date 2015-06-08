
#ifndef __CBASE64_H__
#define __CBASE64_H__

#include "_CBase64.h"

namespace Libcore {
namespace IO {

CarClass(CBase64)
{
public:
    CARAPI Decode(
        /* [in] */ ArrayOf<Byte>* inData,
        /* [out, callee] */ ArrayOf<Byte>** outData);

    CARAPI Decode(
        /* [in] */ ArrayOf<Byte>* inData,
        /* [in] */ Int32 len,
        /* [out, callee] */ ArrayOf<Byte>** outData);

    CARAPI Encoder(
        /* [in] */ ArrayOf<Byte>* inData,
        /* [out] */ String* outData);

private:
    // TODO: Add your private member variables here.
};

} // namespace IO
} // namespace Libcore

#endif // __CBASE64_H__
