
#ifndef __CFRAGMENTMANAGERIMPLHELPER_H__
#define __CFRAGMENTMANAGERIMPLHELPER_H__

#include "_CFragmentManagerImplHelper.h"

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CFragmentManagerImplHelper)
{
public:
    CARAPI ReverseTransit(
        /* [in] */ Int32 transit,
        /* [out] */ Int32* reverse);

    CARAPI TransitToStyleIndex(
        /* [in] */ Int32 transit,
        /* [in] */ Boolean enter,
        /* [out] */ Int32* index);

    CARAPI SetDEBUG(
        /* [in] */ Boolean debug);

    CARAPI GetDEBUG(
        /* [out] */ Boolean* debug);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__CFRAGMENTMANAGERIMPLHELPER_H__