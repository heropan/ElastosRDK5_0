
#ifndef __CINFOACCESSSYNTAXHELPER_H__
#define __CINFOACCESSSYNTAXHELPER_H__

#include "_CInfoAccessSyntaxHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CInfoAccessSyntaxHelper)
{
public:
    CARAPI Decode(
        /* [in] */ ArrayOf<Byte> * pEncoding,
        /* [out] */ Org::Apache::Harmony::Security::X509::IInfoAccessSyntax ** ppSyntax);

    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Type ** ppAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CINFOACCESSSYNTAXHELPER_H__
