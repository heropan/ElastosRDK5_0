#ifndef __ELASTOS_DROID_PROVIDER_CSETTINGSNAMEVALUETABLE_H__
#define __ELASTOS_DROID_PROVIDER_CSETTINGSNAMEVALUETABLE_H__

#include "_CSettingsNameValueTable.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CSettingsNameValueTable)
{
public:
    CARAPI GetUriFor(
        /* [in] */ IUri* uri,
        /* [in] */ const String& name,
        /* [out] */ IUri** value);
};

} //namespace Provider
} //namespace Droid
} //namespace Elastos

#endif //__ELASTOS_DROID_PROVIDER_CSETTINGSNAMEVALUETABLE_H__
