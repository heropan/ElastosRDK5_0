
#include "os/Binder.h"
#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>

namespace Elastos {
namespace Droid {
namespace Os {

// {C565CD41-ACB7-4CCC-ADE5-5E7DF763DACF}
extern const InterfaceID EIID_Binder =
    { 0xc565cd41, 0xacb7, 0x4ccc, { 0xad, 0xe5, 0x5e, 0x7d, 0xf7, 0x63, 0xda, 0xcf } };

Binder::Binder()
    : mObject(0)
{}

Binder::~Binder()
{
    // Destroy();
}

Int32 Binder::GetCallingPid()
{
    return android::IPCThreadState::self()->getCallingPid();
}

Int32 Binder::GetCallingUid()
{
    return android::IPCThreadState::self()->getCallingUid();
}

Int64 Binder::ClearCallingIdentity()
{
    return android::IPCThreadState::self()->clearCallingIdentity();
}

ECode Binder::RestoreCallingIdentity(
    /* [in] */ Int64 token)
{
    // XXX temporary sanity check to debug crashes.
    Int32 uid = (Int32)(token >> 32);
    if (uid > 0 && uid < 999) {
        // In Android currently there are no uids in this range.
//        char buf[128];
//        sprintf(buf, "Restoring bad calling ident: 0x%Lx", token);
//        jniThrowException(env, "java/lang/IllegalStateException", buf);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    android::IPCThreadState::self()->restoreCallingIdentity(token);
    return NOERROR;
}

ECode Binder::SetThreadStrictModePolicy(
    /* [in] */ Int32 policyMask)
{
    android::IPCThreadState::self()->setStrictModePolicy(policyMask);
    return NOERROR;
}

Int32 Binder::GetThreadStrictModePolicy()
{
    return android::IPCThreadState::self()->getStrictModePolicy();
}

ECode Binder::FlushPendingCommands()
{
    android::IPCThreadState::self()->flushCommands();
    return NOERROR;
}

ECode Binder::JoinThreadPool()
{
    android::sp<android::IBinder> b = android::ProcessState::self()->getContextObject(NULL);
    android::IPCThreadState::self()->joinThreadPool();
    return NOERROR;
}

void Binder::Destroy()
{
}

} // namespace Os
} // namespace Droid
} // namespace Elastos

