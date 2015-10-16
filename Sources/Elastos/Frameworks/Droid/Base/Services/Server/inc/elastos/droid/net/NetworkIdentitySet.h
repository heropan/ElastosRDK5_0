
#ifndef __ELASTOS_DROID_SERVER_NET_NETWORKIDENTITYSET_H__
#define __ELASTOS_DROID_SERVER_NET_NETWORKIDENTITYSET_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/HashSet.h>

using Elastos::Utility::Etl::HashSet;
using Elastos::IO::IDataInput;
using Elastos::IO::IDataOutput;
using Elastos::Droid::Net::INetworkIdentity;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

/**
 * Identity of a {@code iface}, defined by the set of {@link NetworkIdentity}
 * active on that interface.
 *
 * @hide
 */
class NetworkIdentitySet
    : public HashSet< AutoPtr<INetworkIdentity> >
{
public:
    NetworkIdentitySet();

    NetworkIdentitySet(
    /* [in] */ IDataInput* in);

    CARAPI WriteToStream(
        /* [out] */ IDataOutput* out);

private:
    CARAPI_(void) WriteOptionalString(
        /* [in] */ IDataOutput* out,
        /* [in] */ const String& value);

    CARAPI_(String) ReadOptionalString(
        /* [in] */ IDataInput* in);

private:
    static const Int32 VERSION_INIT;
    static const Int32 VERSION_ADD_ROAMING;
    static const Int32 VERSION_ADD_NETWORK_ID;
};

} // namespace Net
} // namespace Server
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_NET_NETWORKIDENTITYSET_H__
