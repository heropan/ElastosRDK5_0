
#ifndef __ELASTOS_SECURITY_PERMISSIONCOLLECTION_H__
#define __ELASTOS_SECURITY_PERMISSIONCOLLECTION_H__

using Elastos::Utility::IEnumeration;

namespace Elastos {
namespace Security {

class PermissionCollection
    : public Object
    , public IPermissionCollection
    , public ISerializable
{
public:
    CAR_INTERFACE_DECL();

    virtual CARAPI Add(
        /* [in] */ IPermission *permission) = 0;

    virtual CARAPI GetElements(
        /* [out] */ IEnumeration **elems) = 0;

    virtual CARAPI Implies(
        /* [in] */ IPermission *permission,
        /* [out] */ Boolean *ret) = 0;

    virtual CARAPI IsReadOnly(
        /* [out] */ Boolean *ret);

    virtual CARAPI SetReadOnly();
};

} // namespace Security
} // namespace Elastos

#endif // __ELASTOS_SECURITY_PERMISSIONCOLLECTION_H__
