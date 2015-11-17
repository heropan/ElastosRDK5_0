
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/internal/view/CInputBindResult.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {

ECode CInputBindResult::constructor()
{
    return NOERROR;
}

ECode CInputBindResult::constructor(
    /* [in] */ IIInputMethodSession* _method,
    /* [in] */ const String& _id,
    /* [in] */ Int32 _sequence)
{
    mMethod = _method;
    mId = _id;
    mSequence = _sequence;
    return NOERROR;
}

ECode CInputBindResult::ReadFromParcel(
    /* [in] */ IParcel *source)
{
    AutoPtr<IInterface> obj;
    source->ReadInterfacePtr((Handle32*)&obj);
    if (obj != NULL) {
        mMethod = IIInputMethodSession::Probe(obj);
    }
    source->ReadString(&mId);
    source->ReadInt32(&mSequence);
    return NOERROR;
}

ECode CInputBindResult::WriteToParcel(
    /* [in] */ IParcel *dest)
{
    dest->WriteInterfacePtr((IInterface*)mMethod.Get());
    dest->WriteString(mId);
    dest->WriteInt32(mSequence);
    return NOERROR;
}

ECode CInputBindResult::GetIIMSession(
    /* [out] */ IIInputMethodSession** session)
{
    VALIDATE_NOT_NULL(session);
    *session = mMethod;
    REFCOUNT_ADD(*session);
    return NOERROR;
}

ECode CInputBindResult::GetId(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

ECode CInputBindResult::GetSequence(
    /* [out] */ Int32* sequence)
{
    VALIDATE_NOT_NULL(sequence);
    *sequence = mSequence;
    return NOERROR;
}

} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos
