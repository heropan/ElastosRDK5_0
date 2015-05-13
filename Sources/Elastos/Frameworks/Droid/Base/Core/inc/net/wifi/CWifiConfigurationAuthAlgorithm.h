
#ifndef __CWIFICONFIGURATIONAUTHALGORITHM_H__
#define __CWIFICONFIGURATIONAUTHALGORITHM_H__

#include "_CWifiConfigurationAuthAlgorithm.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CWifiConfigurationAuthAlgorithm)
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

#endif // __CWIFICONFIGURATIONAUTHALGORITHM_H__
