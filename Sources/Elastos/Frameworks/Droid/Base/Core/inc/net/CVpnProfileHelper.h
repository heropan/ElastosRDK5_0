
#ifndef __CVPNPROFILEHELPER_H__
#define __CVPNPROFILEHELPER_H__

#include "_CVpnProfileHelper.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Net {

CarClass(CVpnProfileHelper)
{
public:
    CARAPI Decode(
        /* [in] */ const String& key,
        /* [in] */ const ArrayOf<Byte>& value,
        /* [out] */ IVpnProfile** profile);
};

} // namespace Net
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__CVPNPROFILEHELPER_H__
