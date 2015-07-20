#include <ext/frameworkdef.h>
#include "os/CBinder.h"
//#include "os/CUserHandle.h"

// using Elastos::Droid::Os::IUserHandle;
// using Elastos::Droid::Os::CUserHandle;

namespace Elastos {
namespace Droid {
namespace Os {



//ECode CBinder::GetInterfaceDescriptor(
//    /* [out] */ String* str)
//{
//    return E_NOT_IMPLEMENTED;
//}
//
//ECode CBinder::PingBinder(
//    /* [out] */ Boolean* result)
//{
//    return E_NOT_IMPLEMENTED;
//}
//
//ECode CBinder::IsBinderAlive(
//    /* [out] */ Boolean* result)
//{
//    return E_NOT_IMPLEMENTED;
//}
//
//ECode CBinder::QueryLocalInterface(
//    /* [in] */ const String& descriptor,
//    /* [out] */ IInterface** object)
//{
//    return E_NOT_IMPLEMENTED;
//}
//
//ECode CBinder::Dump(
//    /* [in] */ IFileDescriptor* fd,
//    /* [in] */ ArrayOf<String>* args)
//{
//    return E_NOT_IMPLEMENTED;
//}
//
//ECode CBinder::DumpAsync(
//    /* [in] */ IFileDescriptor* fd,
//    /* [in] */ ArrayOf<String>* args)
//{
//    return E_NOT_IMPLEMENTED;
//}
//
//ECode CBinder::Transact(
//    /* [in] */ Int32 code,
//    /* [in] */ IParcel* data,
//    /* [in] */ IParcel* reply,
//    /* [in] */ Int32 flags)
//{
//    return E_NOT_IMPLEMENTED;
//}
//
//ECode CBinder::DeathRecipient(
//    /* [out] */ IInterface** object)
//{
//    return E_NOT_IMPLEMENTED;
//}
//
//ECode CBinder::LinkToDeath(
//    /* [in] */ IProxyDeathRecipient* recipient,
//    /* [in] */ Int32 flags)
//{
//    return E_NOT_IMPLEMENTED;
//}
//
//ECode CBinder::UnlinkToDeath(
//    /* [in] */ IProxyDeathRecipient* recipient,
//    /* [in] */ Int32 flags)
//{
//    return E_NOT_IMPLEMENTED;
//}

ECode CBinder::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String("CBinder");
    return NOERROR;
}

//
// native methods
//

Int32 CBinder::GetCallingPid()
{
    return Binder::GetCallingPid();
}

Int32 CBinder::GetCallingUid()
{
    return Binder::GetCallingUid();
}

// AutoPtr<IUserHandle> CBinder::GetCallingUserHandle()
// {
//     Int32 userId = CUserHandle::GetUserId(GetCallingUid());
//     AutoPtr<IUserHandle> userHandle;
//     CUserHandle::New(userId, (IUserHandle**)&userHandle);
//     return userHandle;
// }

Int64 CBinder::ClearCallingIdentity()
{
    return Binder::ClearCallingIdentity();
}

ECode CBinder::RestoreCallingIdentity(
    /* [in] */ Int64 token)
{
    return Binder::RestoreCallingIdentity(token);
}

ECode CBinder::SetThreadStrictModePolicy(
    /* [in] */ Int32 policyMask)
{
    return Binder::SetThreadStrictModePolicy(policyMask);
}

Int32 CBinder::GetThreadStrictModePolicy()
{
    return Binder::GetThreadStrictModePolicy();
}

ECode CBinder::FlushPendingCommands()
{
    return Binder::FlushPendingCommands();
}

ECode CBinder::JoinThreadPool()
{
    return Binder::JoinThreadPool();
}


} // namespace Os
} // namespace Droid
} // namespace Elastos
