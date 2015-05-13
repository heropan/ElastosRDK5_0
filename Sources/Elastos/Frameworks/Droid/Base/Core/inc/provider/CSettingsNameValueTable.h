#ifndef __CSETTINGSNAMEVALUETABLE_H__
#define __CSETTINGSNAMEVALUETABLE_H__

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

#endif //__CSETTINGSNAMEVALUETABLE_H__
