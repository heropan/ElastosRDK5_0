
#ifndef __CWIFICONFIGURATIONHELPER_H__ 
#define __CWIFICONFIGURATIONHELPER_H__

#include "_CWifiConfigurationHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

CarClass(CWifiConfigurationHelper)
{
public:
    CARAPI GetWepKeyVarName(
        /* [in] */ Int32 index,
        /* [out] */ String* wepKeyVarName);
};

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //__CWIFICONFIGURATIONHELPER_H__
