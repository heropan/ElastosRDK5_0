
#ifndef __ELASTOS_DROID_NET_CLINKCAPABILITIESHELPER_H__
#define __ELASTOS_DROID_NET_CLINKCAPABILITIESHELPER_H__

#include "_CLinkCapabilitiesHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CLinkCapabilitiesHelper)
{
public:
    CARAPI CreateNeedsMap(
        /* [in] */ const String& applicationRole,
        /* [out] */ ILinkCapabilities** result);

private:
    // TODO: Add your private member variables here.
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CLINKCAPABILITIESHELPER_H__
