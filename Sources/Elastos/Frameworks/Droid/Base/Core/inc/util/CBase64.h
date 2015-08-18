
#ifndef __ELASTOS_DROID_UTILITY_CBASE64_H__
#define __ELASTOS_DROID_UTILITY_CBASE64_H__

#include "_Elastos_Droid_Utility_CBase64.h"

namespace Elastos {
namespace Droid {
namespace Utility {

CarClass(CBase64)
{
public:
    CARAPI Decode(
        /* [in] */ const String& str,
        /* [in] */ Int32 flags,
        /* [out, callee] */ ArrayOf<Byte>** result);

    CARAPI Decode2(
        /* [in] */ const ArrayOf<Byte>& input,
        /* [in] */ Int32 flags,
        /* [out, callee] */ ArrayOf<Byte>** result);

    CARAPI Decode3(
        /* [in] */ const ArrayOf<Byte>& input,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 len,
        /* [in] */ Int32 flags,
        /* [out, callee] */ ArrayOf<Byte>** result);

    CARAPI EncodeToString(
        /* [in] */ const ArrayOf<Byte>& input,
        /* [in] */ Int32 flags,
        /* [out] */ String* result);

    CARAPI EncodeToString2(
        /* [in] */ const ArrayOf<Byte>& input,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 len,
        /* [in] */ Int32 flags,
        /* [out] */ String* result);

    CARAPI Encode(
        /* [in] */ const ArrayOf<Byte>& input,
        /* [in] */ Int32 flags,
        /* [out, callee] */ ArrayOf<Byte>** result);

    CARAPI Encode2(
        /* [in] */ const ArrayOf<Byte>& input,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 len,
        /* [in] */ Int32 flags,
        /* [out, callee] */ ArrayOf<Byte>** result);

private:
    // TODO: Add your private member variables here.
};

}
}
}
#endif // __ELASTOS_DROID_UTILITY_CBASE64_H__
