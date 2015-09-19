
#include "app/CFragmentSavedState.h"
#include "ext/frameworkdef.h"

namespace Elastos {
namespace Droid {
namespace App {

ECode CFragmentSavedState::constructor(
        /* [in] */ IBundle* state)
{
    mState = state;
    return NOERROR;
}

ECode CFragmentSavedState::constructor(
    /* [in] */ IParcel* in,
    /* [in] */ IClassLoader* loader)
{
    mLoader = loader;
    ReadFromParcel(in);
    return NOERROR;
}

ECode CFragmentSavedState::GetState(
    /* [out] */ IBundle** state)
{
    VALIDATE_NOT_NULL(state);
    *state = mState;
    REFCOUNT_ADD(*state)
    return NOERROR;
}

ECode CFragmentSavedState::SetState(
    /* [in] */ IBundle* state)
{
    mState = state;
    return NOERROR;
}

ECode CFragmentSavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
//    mState = source->ReadBundle();
    if (mLoader != NULL && mState != NULL) {
        mState->SetClassLoader(mLoader);
    }
    return NOERROR;
}

ECode CFragmentSavedState::WriteToParcel(
        /* [in] */ IParcel* dest)
{
//    dest->WriteBundle(mState);
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
