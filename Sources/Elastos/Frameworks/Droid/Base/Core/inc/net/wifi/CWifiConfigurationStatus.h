
#ifndef __CWIFICONFIGURATIONSTATUS_H__
#define __CWIFICONFIGURATIONSTATUS_H__

#include "_CWifiConfigurationStatus.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CWifiConfigurationStatus)
{
public:
    CARAPI GetStrings(
        /* [out, callee] */ ArrayOf<String>** strings);

    CARAPI ToString(
        /* [in] */ Int32 index,
        /* [out] */ String* strValue);

public:
    static const AutoPtr< ArrayOf<String> > sStrings;
};

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif // __CWIFICONFIGURATIONSTATUS_H__
