
#ifndef __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTACCOUNTS_H__
#define __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTACCOUNTS_H__

#include "_CBrowserContractAccounts.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CBrowserContractAccounts)
{
public:
    /**
     * Directory under {@link Bookmarks#CONTENT_URI}
     */
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);
};

}
}
}

#endif //__ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTACCOUNTS_H__
