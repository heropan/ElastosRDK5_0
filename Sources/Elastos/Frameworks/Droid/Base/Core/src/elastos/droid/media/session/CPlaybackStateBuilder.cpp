#include "elastos/droid/media/session/CPlaybackStateBuilder.h"
#include "elastos/droid/media/session/CPlaybackStateCustomAction.h"
#include "elastos/droid/media/session/CPlaybackState.h"
#include "elastos/droid/os/SystemClock.h"

using Elastos::Core::CString;
using Elastos::Core::ICharSequence;
using Elastos::Utility::CArrayList;
using Elastos::Utility::ICollection;
using Elastos::Droid::Os::SystemClock;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Session {

CAR_INTERFACE_IMPL(CPlaybackStateBuilder, Object, IPlaybackStateBuilder)

CAR_OBJECT_IMPL(CPlaybackStateBuilder)

CPlaybackStateBuilder::CPlaybackStateBuilder()
    : mState(0)
    , mPosition(0)
    , mBufferedPosition(0)
    , mSpeed(0)
    , mActions(0)
    , mUpdateTime(0)
    , mActiveItemId(0)
{
    CArrayList::New((IArrayList**)&mCustomActions);
}

CPlaybackStateBuilder::~CPlaybackStateBuilder()
{
}

ECode CPlaybackStateBuilder::constructor()
{
    return NOERROR;
}

ECode CPlaybackStateBuilder::constructor(
    /* [in] */ IPlaybackState * from)
{
    if (from == NULL) {
        return NOERROR;
    }
    AutoPtr<CPlaybackStateBuilder> f = (CPlaybackStateBuilder*)from;
    mState = f->mState;
    mPosition = f->mPosition;
    mBufferedPosition = f->mBufferedPosition;
    mSpeed = f->mSpeed;
    mActions = f->mActions;
    if (f->mCustomActions != NULL) {
        mCustomActions->AddAll(ICollection::Probe(f->mCustomActions));
    }
    mErrorMessage = f->mErrorMessage;
    mUpdateTime = f->mUpdateTime;
    mActiveItemId = f->mActiveItemId;
    return NOERROR;
}

ECode CPlaybackStateBuilder::SetState(
    /* [in] */ Int32 state,
    /* [in] */ Int64 position,
    /* [in] */ Float playbackSpeed,
    /* [in] */ Int64 updateTime,
    /* [out] */ IPlaybackStateBuilder ** result)
{
    VALIDATE_NOT_NULL(result)
    mState = state;
    mPosition = position;
    mUpdateTime = updateTime;
    mSpeed = playbackSpeed;
    *result = this;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CPlaybackStateBuilder::SetState(
    /* [in] */ Int32 state,
    /* [in] */ Int64 position,
    /* [in] */ Float playbackSpeed,
    /* [out] */ IPlaybackStateBuilder ** result)
{
    return SetState(state, position, playbackSpeed, SystemClock::GetElapsedRealtime(), result);
}

ECode CPlaybackStateBuilder::SetActions(
    /* [in] */ Int64 actions,
    /* [out] */ IPlaybackStateBuilder ** result)
{
    VALIDATE_NOT_NULL(result)
    mActions = actions;
    *result = this;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CPlaybackStateBuilder::AddCustomAction(
    /* [in] */ const String& action,
    /* [in] */ const String& name,
    /* [in] */ Int32 icon,
    /* [out] */ IPlaybackStateBuilder ** result)
{
    AutoPtr<ICharSequence> csq;
    CString::New(name, (ICharSequence**)&csq);
    AutoPtr<IPlaybackStateCustomAction> customAction;
    CPlaybackStateCustomAction::New(action, csq, icon, NULL
        , (IPlaybackStateCustomAction**)&customAction);
    return AddCustomAction(customAction, result);
}

ECode CPlaybackStateBuilder::AddCustomAction(
    /* [in] */ IPlaybackStateCustomAction * customAction,
    /* [out] */ IPlaybackStateBuilder ** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;
    if (customAction == NULL) {
        // throw new IllegalArgumentException(
        //         "You may not add a null CustomAction to PlaybackState.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mCustomActions->Add(customAction);
    *result = this;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CPlaybackStateBuilder::SetBufferedPosition(
    /* [in] */ Int64 bufferedPosition,
    /* [out] */ IPlaybackStateBuilder ** result)
{
    VALIDATE_NOT_NULL(result)
    mBufferedPosition = bufferedPosition;
    *result = this;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CPlaybackStateBuilder::SetActiveQueueItemId(
    /* [in] */ Int64 id,
    /* [out] */ IPlaybackStateBuilder ** result)
{
    VALIDATE_NOT_NULL(result)
    mActiveItemId = id;
    *result = this;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CPlaybackStateBuilder::SetErrorMessage(
    /* [in] */ ICharSequence * error,
    /* [out] */ IPlaybackStateBuilder ** result)
{
    VALIDATE_NOT_NULL(result)
    mErrorMessage = error;
    *result = this;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CPlaybackStateBuilder::Build(
    /* [out] */ IPlaybackState ** result)
{
    VALIDATE_NOT_NULL(result)
    return CPlaybackState::New(mState, mPosition, mUpdateTime, mSpeed, mBufferedPosition,
            mActions, IList::Probe(mCustomActions), mActiveItemId, mErrorMessage, result);
}

} // namespace Session
} // namespace Media
} // namepsace Droid
} // namespace Elastos
