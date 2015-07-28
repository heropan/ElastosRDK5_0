
#ifndef __CBROWSERCONTRACTACCOUNTS_H__
#define __CBROWSERCONTRACTACCOUNTS_H__

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

#endif //__CBROWSERCONTRACTACCOUNTS_H__