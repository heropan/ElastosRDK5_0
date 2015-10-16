
#include "elastos/droid/internal/LockPatternViewSavedState.h"

namespace Elastos{
namespace Droid{
namespace Widget{
namespace Internal{

LockPatternViewSavedState::LockPatternViewSavedState()
    : mSerializedPattern(NULL)
    , mDisplayMode(0)
    , mInputEnabled(FALSE)
    , mInStealthMode(FALSE)
    , mTactileFeedbackEnabled(FALSE)
{}

LockPatternViewSavedState::LockPatternViewSavedState(
    /* [in] */ IParcelable* superState,
    /* [in] */ const String& serializedPattern,
    /* [in] */ Int32 displayMode,
    /* [in] */ Boolean inputEnabled,
    /* [in] */ Boolean inStealthMode,
    /* [in] */ Boolean tactileFeedbackEnabled)
    : ViewBaseSavedState(superState)
    , mSerializedPattern(serializedPattern)
    , mDisplayMode(displayMode)
    , mInputEnabled(inputEnabled)
    , mInStealthMode(inStealthMode)
    , mTactileFeedbackEnabled(tactileFeedbackEnabled)
{
}

LockPatternViewSavedState::LockPatternViewSavedState(
    /* [in] */ IParcel* in)
    : ViewBaseSavedState(in)
{
    in->ReadString(&mSerializedPattern);
    in->ReadInt32(&mDisplayMode);
    in->ReadBoolean(&mInputEnabled);
    in->ReadBoolean(&mInStealthMode);
    in->ReadBoolean(&mTactileFeedbackEnabled);
}

ECode LockPatternViewSavedState::Init(
    /* [in] */ IParcelable* superState,
    /* [in] */ const String& serializedPattern,
    /* [in] */ Int32 displayMode,
    /* [in] */ Boolean inputEnabled,
    /* [in] */ Boolean inStealthMode,
    /* [in] */ Boolean tactileFeedbackEnabled)
{
    ViewBaseSavedState::Init(superState);
    mSerializedPattern = serializedPattern;
    mDisplayMode = displayMode;
    mInputEnabled = inputEnabled;
    mInStealthMode = inStealthMode;
    mTactileFeedbackEnabled = tactileFeedbackEnabled;
    return NOERROR;
}

ECode LockPatternViewSavedState::Init(
    /* [in] */ IParcel* in)
{
    ViewBaseSavedState::Init(in);
    in->ReadString(&mSerializedPattern);
    in->ReadInt32(&mDisplayMode);
    in->ReadBoolean(&mInputEnabled);
    in->ReadBoolean(&mInStealthMode);
    in->ReadBoolean(&mTactileFeedbackEnabled);
    return NOERROR;
}

String LockPatternViewSavedState::GetSerializedPattern()
{
    return mSerializedPattern;
}

Int32 LockPatternViewSavedState::GetDisplayMode()
{
    return mDisplayMode;
}

Boolean LockPatternViewSavedState::IsInputEnabled()
{
    return mInputEnabled;
}

Boolean LockPatternViewSavedState::IsInStealthMode()
{
    return mInStealthMode;
}

Boolean LockPatternViewSavedState::IsTactileFeedbackEnabled()
{
    return mTactileFeedbackEnabled;
}

ECode LockPatternViewSavedState::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 flags)
{
    ViewBaseSavedState::WriteToParcel(dest);
    dest->WriteString(mSerializedPattern);
    dest->WriteInt32(mDisplayMode);
    dest->WriteBoolean(mInputEnabled);
    dest->WriteBoolean(mInStealthMode);
    dest->WriteBoolean(mTactileFeedbackEnabled);

    return NOERROR;
}


}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
