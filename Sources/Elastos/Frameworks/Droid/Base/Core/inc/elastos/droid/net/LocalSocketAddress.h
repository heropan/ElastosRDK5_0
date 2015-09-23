
#ifndef __ELASTOS_DROID_NET_LOCALSOCKETADDRESS_H__
#define __ELASTOS_DROID_NET_LOCALSOCKETADDRESS_H__

#include "elastos/droid/ext/frameworkext.h"

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
class LocalSocketAddress
    : public Object
    , public ILocalSocketAddress
{
public:
    CAR_INTERFACE_DECL()

    /**
     * Creates an instance with a given name.
     *
     * @param name non-null name
     * @param namespace namespace the name should be created in.
     */
    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ ILocalSocketAddressNamespace* ns);

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
        /* [out] */ String* result);

    /**
     * Returns the namespace used by this address.
     *
     * @return non-null a namespace
     */
    CARAPI GetNamespace(
        /* [out] */ ILocalSocketAddressNamespace** result);

private:
    String mName;

    AutoPtr<ILocalSocketAddressNamespace> mNamespace;

};

/**
 * The namespace that this address exists in. See also
 * include/cutils/sockets.h ANDROID_SOCKET_NAMESPACE_*
 */
class LocalSocketAddressNamespace
    : public Object
    , public ILocalSocketAddressNamespace
{
public:
    CAR_INTERFACE_DECL()

    Int32 mValue;

    ECode constructor(
        /* [in] */ Int32 id)
    {
        mId = id;
        return NOERROR;
    }

    /**
     * @return int constant shared with native code
     */
    /*package*/
    ECode GetId(
        /* [out] */ Int32* result)
    {
        VALIDATE_NOT_NULL(result)

        *result = mId;
        return NOERROR;
    }

private:
    /** The id matches with a #define in include/cutils/sockets.h */
    Int32 mId;
};

} // namespace Net
} // namespace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_NET_LOCALSOCKETADDRESS_H__
