
#ifndef __ELASTOS_DROID_NET_CCOMPARERESULT_H__
#define __ELASTOS_DROID_NET_CCOMPARERESULT_H__

#include "_Elastos_Droid_Net_CCompareResult.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/List.h>
#include <elastos/core/StringBuffer.h>

using Elastos::Utility::Etl::List;
using Elastos::Core::StringBuffer;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CCompareResult)
{
public:
    CARAPI constructor();

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI GetRemoved(
        /* [out] */ IObjectContainer** result);

    CARAPI AddRemoved(
        /* [in] */ IInterface* removed);

    CARAPI GetAdded(
        /* [out] */ IObjectContainer** result);

    CARAPI AddAdded(
        /* [in] */ IInterface* added);

public:
    List< AutoPtr<IInterface> > mRemoved;
    List< AutoPtr<IInterface> > mAdded;
};

} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CCOMPARERESULT_H__
