#include "elastos/droid/widget/internal/CLockPatternViewSavedState.h"

namespace Elastos{
namespace Droid{
namespace Widget{
namespace Internal{

ECode CLockPatternViewSavedState::constructor(
    /* [in] */ IParcel* in)
{
    return LockPatternViewSavedState::Init(in);
}

ECode CLockPatternViewSavedState::constructor(
    /* [in] */ IParcelable* superState,
    /* [in] */ const String& serializedPattern,
    /* [in] */ Int32 displayMode,
    /* [in] */ Boolean inputEnabled,
    /* [in] */ Boolean inStealthMode,
    /* [in] */ Boolean tactileFeedbackEnabled)
{
    return LockPatternViewSavedState::Init(superState, serializedPattern, displayMode, inputEnabled, inStealthMode, tactileFeedbackEnabled);
}

ECode CLockPatternViewSavedState::GetSerializedPattern(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = LockPatternViewSavedState::GetSerializedPattern();
    return NOERROR;
}

ECode CLockPatternViewSavedState::GetDisplayMode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = LockPatternViewSavedState::GetDisplayMode();
    return NOERROR;
}

ECode CLockPatternViewSavedState::IsInputEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = LockPatternViewSavedState::IsInputEnabled();
    return NOERROR;
}

ECode CLockPatternViewSavedState::IsInStealthMode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = LockPatternViewSavedState::IsInStealthMode();
    return NOERROR;
}

ECode CLockPatternViewSavedState::IsTactileFeedbackEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = LockPatternViewSavedState::IsTactileFeedbackEnabled();
    return NOERROR;
}

ECode CLockPatternViewSavedState::GetSuperState(
    /* [out] */ IParcelable** superState)
{
    VALIDATE_NOT_NULL(superState);
    *superState = LockPatternViewSavedState::GetSuperState();
    REFCOUNT_ADD(*superState);
    return NOERROR;
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos