
#include "elastos/droid/net/LocalSocketAddress.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(LocalSocketAddressNamespace, Object, ILocalSocketAddressNamespace)

CAR_INTERFACE_IMPL(LocalSocketAddress, Object, ILocalSocketAddress)

ECode LocalSocketAddress::constructor(
    /* [in] */ const String& name,
    /* [in] */ ILocalSocketAddressNamespace* ns)
{
    mName = name;
    mNamespace = ns;
    return NOERROR;
}

ECode LocalSocketAddress::constructor(
    /* [in] */ const String& name)
{
    AutoPtr<ILocalSocketAddressNamespace> ns = new LocalSocketAddressNamespace();
    ns->SetId(ILocalSocketAddressNamespace::LocalSocketAddressNamespace_ABSTRACT);
    return constructor(name, ns);
}

ECode LocalSocketAddress::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = mName;
    return NOERROR;
}

ECode LocalSocketAddress::GetNamespace(
    /* [out] */ ILocalSocketAddressNamespace** ns)
{
    VALIDATE_NOT_NULL(ns);

    *ns = mNamespace;
    REFCOUNT_ADD(*ns)
    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
