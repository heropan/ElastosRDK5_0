
#ifndef __CWIFICONFIGURATIONGROUPCIPHER_H__
#define __CWIFICONFIGURATIONGROUPCIPHER_H__

#include "_CWifiConfigurationGroupCipher.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CWifiConfigurationGroupCipher)
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

#endif // __CWIFICONFIGURATIONGROUPCIPHER_H__
