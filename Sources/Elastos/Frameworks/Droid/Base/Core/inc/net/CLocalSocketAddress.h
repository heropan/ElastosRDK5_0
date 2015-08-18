
#ifndef __ELASTOS_DROID_NET_CLOCALSOCKETADDRESS_H__
#define __ELASTOS_DROID_NET_CLOCALSOCKETADDRESS_H__

#include "_Elastos_Droid_Net_CLocalSocketAddress.h"

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * A UNIX-domain (AF_LOCAL) socket address. For use with
 * android.net.LocalSocket and android.net.LocalServerSocket.
 *
 * On the Android system, these names refer to names in the Linux
 * abstract (non-filesystem) UNIX domain namespace.
 */
CarClass(CLocalSocketAddress)
{
public:
    /**
     * Creates an instance with a given name.
     *
     * @param name non-null name
     * @param namespace namespace the name should be created in.
     */
    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ LocalSocketAddressNamespace ns);

    /**
     * Creates an instance with a given name in the {@link Namespace#ABSTRACT}
     * namespace
     *
     * @param name non-null name
     */
    CARAPI constructor(
        /* [in] */ const String& name);

    /**
     * Retrieves the string name of this address
     * @return string name
     */
    CARAPI GetName(
        /* [out] */ String* name);

    /**
     * Returns the namespace used by this address.
     *
     * @return non-null a namespace
     */
    CARAPI GetNamespace(
        /* [out] */ LocalSocketAddressNamespace* ns);

private:
    String mName;
    LocalSocketAddressNamespace mNamespace;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_NET_CLOCALSOCKETADDRESS_H__
