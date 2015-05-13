
#include "ext/frameworkdef.h"
#include "net/CLocalSocketAddress.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CLocalSocketAddress::constructor(
    /* [in] */ const String& name,
    /* [in] */ LocalSocketAddressNamespace ns)
{
    mName = name;
    mNamespace = ns;
    return NOERROR;
}

ECode CLocalSocketAddress::constructor(
    /* [in] */ const String& name)
{
    // TODO: for socket elzygote
    return constructor(name, LocalSocketAddressNamespace_ABSTRACT/*LocalSocketAddressNamespace_ABSTRACT*/);
}

ECode CLocalSocketAddress::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);

    *name = mName;
    return NOERROR;
}

ECode CLocalSocketAddress::GetNamespace(
    /* [out] */ LocalSocketAddressNamespace* ns)
{
    VALIDATE_NOT_NULL(ns);

    *ns = mNamespace;
    return NOERROR;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos