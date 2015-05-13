
#ifndef __CWIFICONFIGURATIONPAIRWISECIPHER_H__
#define __CWIFICONFIGURATIONPAIRWISECIPHER_H__

#include "_CWifiConfigurationPairwiseCipher.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CWifiConfigurationPairwiseCipher)
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

#endif // __CWIFICONFIGURATIONPAIRWISECIPHER_H__
