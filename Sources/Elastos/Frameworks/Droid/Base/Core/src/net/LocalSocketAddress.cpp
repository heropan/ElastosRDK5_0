
#include "LocalSocketAddress.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(LocalSocketAddress, Object, ILocalSocketAddress)

ECode LocalSocketAddress::constructor(
    /* [in] */ const String& name,
    /* [in] */ ILocalSocketAddressNamespace* ns)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    mName = name;
    mNamespace = ns;
    return NOERROR;
#endif
}

ECode LocalSocketAddress::constructor(
    /* [in] */ const String& name)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    // TODO: for socket elzygote
    return constructor(name, LocalSocketAddressNamespace_ABSTRACT/*LocalSocketAddressNamespace_ABSTRACT*/);
#endif
}

ECode LocalSocketAddress::GetName(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(name);

    *name = mName;
    return NOERROR;
#endif
}

ECode LocalSocketAddress::GetNamespace(
    /* [out] */ ILocalSocketAddressNamespace** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(ns);

    *ns = mNamespace;
    return NOERROR;
#endif
}

CAR_INTERFACE_IMPL(LocalSocketAddressNamespace, Object, ILocalSocketAddressNamespace)

} // namespace Net
} // namespace Droid
} // namespace Elastos
