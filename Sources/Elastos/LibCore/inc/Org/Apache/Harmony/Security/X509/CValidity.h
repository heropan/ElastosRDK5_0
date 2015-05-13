
#ifndef __CVALIDITY_H__
#define __CVALIDITY_H__

#include "_CValidity.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CValidity)
{
public:
    CARAPI GetNotBefore(
        /* [out] */ Elastos::Utility::IDate ** ppDate);

    CARAPI GetNotAfter(
        /* [out] */ Elastos::Utility::IDate ** ppDate);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncoded);

    CARAPI constructor(
        /* [in] */ Elastos::Utility::IDate * pNotBefore,
        /* [in] */ Elastos::Utility::IDate * pNotAfter);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CVALIDITY_H__
