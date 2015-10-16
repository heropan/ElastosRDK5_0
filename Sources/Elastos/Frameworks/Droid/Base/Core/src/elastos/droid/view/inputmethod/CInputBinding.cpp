
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/view/inputmethod/CInputBinding.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

CInputBinding::CInputBinding()
    : mUid(0)
    , mPid(0)
{
}

ECode CInputBinding::constructor(
    /* [in] */ IInputConnection* conn,
    /* [in] */ IBinder* connToken,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 pid)
{
    mConnection = conn;
    mConnectionToken = connToken;
    mUid = uid;
    mPid = pid;
    return NOERROR;
}

ECode CInputBinding::constructor(
    /* [in] */ IInputConnection* conn,
    /* [in] */ IInputBinding* binding)
{
    mConnection = conn;
    assert(binding != NULL);
    binding->GetConnectionToken((IBinder**)&mConnectionToken);
    binding->GetUid(&mUid);
    binding->GetPid(&mPid);

    return NOERROR;
}

ECode CInputBinding::constructor()
{
    mConnection = NULL;
    return NOERROR;
}

ECode CInputBinding::GetConnection(
    /* [out] */ IInputConnection** inputConnection)
{
    VALIDATE_NOT_NULL(inputConnection);

    *inputConnection = mConnection;
    REFCOUNT_ADD(*inputConnection);

    return NOERROR;
}

ECode CInputBinding::GetConnectionToken(
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);

    *binder = mConnectionToken;
    REFCOUNT_ADD(*binder);

    return NOERROR;
}

ECode CInputBinding::GetUid(
    /* [out] */ Int32* uid)
{
    VALIDATE_NOT_NULL(uid);

    *uid = mUid;
    return NOERROR;
}

ECode CInputBinding::GetPid(
    /* [out] */ Int32* pid)
{
    VALIDATE_NOT_NULL(pid);

    *pid = mPid;
    return NOERROR;
}

ECode CInputBinding::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    AutoPtr<IInterface> obj;
    FAIL_RETURN(source->ReadInterfacePtr((Handle32*)&obj));
    mConnectionToken = IBinder::Probe(obj.Get());
    FAIL_RETURN(source->ReadInt32(&mUid));
    FAIL_RETURN(source->ReadInt32(&mPid));
    return NOERROR;
}

ECode CInputBinding::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    FAIL_RETURN(dest->WriteInterfacePtr((IInterface*)mConnectionToken.Get()));
    FAIL_RETURN(dest->WriteInt32(mUid));
    FAIL_RETURN(dest->WriteInt32(mPid));
    return NOERROR;
}

ECode CInputBinding::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);

    *contents = 0;
    return NOERROR;
}

ECode CInputBinding::ToString(
    /* [out] */ String* str)
{
    assert(0);
    return NOERROR;
}

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos
