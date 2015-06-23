#include "os/CUserHandle.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CUserHandle::constructor()
{
    return NOERROR;
}

ECode CUserHandle::constructor(
    /* [in] */ Int32 h)
{
    return UserHandle::constructor(h);
}

ECode CUserHandle::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return UserHandle::ReadFromParcel(source);
}

ECode CUserHandle::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return UserHandle::WriteToParcel(dest);
}

ECode CUserHandle::GetIdentifier(
        /* [out] */ Int32* id)
{
    return UserHandle::GetIdentifier(id);
}

ECode CUserHandle::ToString(
    /* [out] */ String* str)
{
    return UserHandle::ToString(str);
}

ECode CUserHandle::Equals(
    /* [in] */ IUserHandle* other,
    /* [out] */ Boolean* equals)
{
    VALIDATE_NOT_NULL(equals);
    return UserHandle::Equals(other, equals);
}

ECode CUserHandle::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* equals)
{
    return Equals(IUserHandle::Probe(other), equals);
}

ECode CUserHandle::GetHashCode(
    /* [out] */ Int32 * hash)
{
    VALIDATE_NOT_NULL(hash);
    return UserHandle::GetHashCode(hash);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
