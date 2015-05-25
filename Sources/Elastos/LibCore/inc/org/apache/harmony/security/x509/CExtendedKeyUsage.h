
#ifndef __CEXTENDEDKEYUSAGE_H__
#define __CEXTENDEDKEYUSAGE_H__

#include "_CExtendedKeyUsage.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CExtendedKeyUsage)
{
public:
    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncode);

    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb,
        /* [in] */ const String& prefix);

    CARAPI DumpValueEx(
        /* [in] */ Elastos::Core::IStringBuilder * pSb);

    CARAPI GetExtendedKeyUsage(
        /* [out] */ Elastos::Utility::IList ** ppExtendedKeyUsage);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte> * pEncoding);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CEXTENDEDKEYUSAGE_H__
