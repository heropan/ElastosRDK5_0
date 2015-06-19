
#ifndef __CBASE64_H__
#define __CBASE64_H__

#include "Singleton.h"
#include "_Libcore_IO_CBase64.h"


namespace Libcore {
namespace IO {

CarClass(CBase64)
    , public Singleton
    , public IBase64
{
public:
    CAR_SINGLETON_DECL()

    CAR_INTERFACE_DECL()

    CARAPI Decode(
        /* [in] */ ArrayOf<Byte>* inData,
        /* [out, callee] */ ArrayOf<Byte>** outData);

    CARAPI Decode(
        /* [in] */ ArrayOf<Byte>* inData,
        /* [in] */ Int32 len,
        /* [out, callee] */ ArrayOf<Byte>** outData);

    CARAPI Encode(
        /* [in] */ ArrayOf<Byte>* inData,
        /* [out] */ String* outData);

private:
    // TODO: Add your private member variables here.
};

} // namespace IO
} // namespace Libcore

#endif // __CBASE64_H__
