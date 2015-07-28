
#ifndef __CBROWSERCONTRACT_H__
#define __CBROWSERCONTRACT_H__

#include "_CBrowserContract.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CBrowserContract)
{
public:
    CARAPI GetAUTHORITY_URI(
        /* [out] */ IUri** uri);
};

}
}
}