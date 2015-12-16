
#include "elastos/droid/graphics/drawable/AnimatedStateListDrawable.h"
#include "elastos/droid/graphics/drawable/CAnimatedStateListDrawable.h"
#include "elastos/droid/animation/ObjectAnimator.h"
#include "elastos/droid/utility/StateSet.h"
#include "elastos/droid/R.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Animation::IAnimator;
using Elastos::Droid::Animation::ObjectAnimator;
using Elastos::Droid::Animation::EIID_ITimeInterpolator;
using Elastos::Droid::R;
using Elastos::Droid::Utility::StateSet;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

AnimatedStateListDrawable::AnimationDrawableTransition::AnimationDrawableTransition(
    /* [in] */ IAnimationDrawable* ad,
    /* [in] */ Boolean reversed)
{
    Int32 frameCount = 0;
    ad->GetNumberOfFrames(&frameCount);
    Int32 fromFrame = reversed ? frameCount - 1 : 0;
    Int32 toFrame = reversed ? 0 : frameCount - 1;
    AutoPtr<FrameInterpolator> interp = new FrameInterpolator(ad, reversed);
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(2);
    (*array)[0] = fromFrame;
    (*array)[1] = toFrame;
    AutoPtr<IObjectAnimator> anim = ObjectAnimator::OfInt32(ad, String("currentIndex"), array);
    anim->SetAutoCancel(TRUE);
    IAnimator::Probe(anim)->SetDuration(interp->GetTotalDuration());
    IAnimator::Probe(anim)->SetInterpolator(interp);

    mAnim = anim;
}

Boolean AnimatedStateListDrawable::AnimationDrawableTransition::CanReverse()
{
    return TRUE;
}

void AnimatedStateListDrawable::AnimationDrawableTransition::Start()
{
    IAnimator::Probe(mAnim)->Start();
}

void AnimatedStateListDrawable::AnimationDrawableTransition::Reverse()
{
    IAnimator::Probe(mAnim)->Reverse();
}

void AnimatedStateListDrawable::AnimationDrawableTransition::Stop()
{
    IAnimator::Probe(mAnim)->Cancel();
}

AnimatedStateListDrawable::AnimatedVectorDrawableTransition::AnimatedVectorDrawableTransition(
    /* [in] */ IAnimatedVectorDrawable* avd,
    /* [in] */ Boolean reversed)
    : mReversed(FALSE)
{
    mAvd = avd;
    mReversed = reversed;
}

Boolean AnimatedStateListDrawable::AnimatedVectorDrawableTransition::CanReverse()
{
    Boolean res = FALSE;
    mAvd->CanReverse(&res);
    return res;
}

void AnimatedStateListDrawable::AnimatedVectorDrawableTransition::Start()
{
    if (mReversed) {
        Reverse();
    } else {
        IAnimatable::Probe(mAvd)->Start();
    }
}

void AnimatedStateListDrawable::AnimatedVectorDrawableTransition::Reverse()
{
    if (CanReverse()) {
        mAvd->Reverse();
    } else {
        Logger::W(LOGTAG, String("Reverse() is called on a drawable can't reverse"));
    }
}

void AnimatedStateListDrawable::AnimatedVectorDrawableTransition::Stop()
{
    IAnimatable::Probe(mAvd)->Stop();
}

const Int32 AnimatedStateListDrawable::AnimatedStateListState::REVERSE_SHIFT = 32;
const Int32 AnimatedStateListDrawable::AnimatedStateListState::REVERSE_MASK = 0x1;
AnimatedStateListDrawable::AnimatedStateListState::AnimatedStateListState(
    /* [in] */ /*@Nullable*/ AnimatedStateListState* orig,
    /* [in] */ /*@NonNull*/ AnimatedStateListDrawable* owner,
    /* [in] */ /*@Nullable*/ IResources* res)
    : StateListState(orig, owner, res)
{
    assert(0 && "TODO");
    // if (orig != NULL) {
    //     mTransitions = orig->mTransitions.clone();
    //     mStateIds = orig->mStateIds.clone();
    // } else {
    //     mTransitions = new LongSparseLongArray();
    //     mStateIds = new SparseIntArray();
    // }
}

Int32 AnimatedStateListDrawable::AnimatedStateListState::AddTransition(
    /* [in] */ Int32 fromId,
    /* [in] */ Int32 toId,
    /* [in] */ /*@NonNull*/ IDrawable* anim,
    /* [in] */ Boolean reversible)
{
    assert(0 && "TODO");
    // Int32 pos = super.addChild(anim);
    // Int64 keyFromTo = generateTransitionKey(fromId, toId);
    // mTransitions.append(keyFromTo, pos);

    // if (reversible) {
    //     Int64 keyToFrom = generateTransitionKey(toId, fromId);
    //     mTransitions.append(keyToFrom, pos | (1L << REVERSE_SHIFT));
    // }

    return AddChild(anim);
}

Int32 AnimatedStateListDrawable::AnimatedStateListState::AddStateSet(
    /* [in] */ /*@NonNull*/ ArrayOf<Int32>* stateSet,
    /* [in] */ /*@NonNull*/ IDrawable* drawable,
    /* [in] */ Int32 id)
{
    Int32 index = StateListState::AddStateSet(stateSet, drawable);
    assert(0 && "TODO");
    // mStateIds.put(index, id);
    return index;
}

Int32 AnimatedStateListDrawable::AnimatedStateListState::IndexOfKeyframe(
    /* [in] */ /*@NonNull*/ ArrayOf<Int32>* stateSet)
{
    Int32 index = StateListState::IndexOfStateSet(stateSet);
    if (index >= 0) {
        return index;
    }

    return StateListState::IndexOfStateSet(StateSet::WILD_CARD);
}

Int32 AnimatedStateListDrawable::AnimatedStateListState::GetKeyframeIdAt(
    /* [in] */ Int32 index)
{
    assert(0 && "TODO");
    // return index < 0 ? 0 : mStateIds.get(index, 0);
    return -1;
}

Int32 AnimatedStateListDrawable::AnimatedStateListState::IndexOfTransition(
    /* [in] */ Int32 fromId,
    /* [in] */ Int32 toId)
{
    Int64 keyFromTo = GenerateTransitionKey(fromId, toId);
    assert(0 && "TODO");
    // return (Int32) mTransitions.get(keyFromTo, -1);
    return -1;
}

Boolean AnimatedStateListDrawable::AnimatedStateListState::IsTransitionReversed(
    /* [in] */ Int32 fromId,
    /* [in] */ Int32 toId)
{
    Int64 keyFromTo = GenerateTransitionKey(fromId, toId);
    assert(0 && "TODO");
    // return (mTransitions.get(keyFromTo, -1) >> REVERSE_SHIFT & REVERSE_MASK) == 1;
    return FALSE;
}

ECode AnimatedStateListDrawable::AnimatedStateListState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    return CAnimatedStateListDrawable::New(this, NULL, drawable);
}

ECode AnimatedStateListDrawable::AnimatedStateListState::NewDrawable(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return CAnimatedStateListDrawable::New(this, res, drawable);
}

Int64 AnimatedStateListDrawable::AnimatedStateListState::GenerateTransitionKey(
    /* [in] */ Int32 fromId,
    /* [in] */ Int32 toId)
{
    return (Int64) fromId << 32 | toId;
}

CAR_INTERFACE_IMPL(AnimatedStateListDrawable::FrameInterpolator, Object, ITimeInterpolator);
AnimatedStateListDrawable::FrameInterpolator::FrameInterpolator(
    /* [in] */ IAnimationDrawable* d,
    /* [in] */ Boolean reversed)
    : mFrames(0)
    , mTotalDuration(0)
{
    UpdateFrames(d, reversed);
}

Int32 AnimatedStateListDrawable::FrameInterpolator::UpdateFrames(
    /* [in] */ IAnimationDrawable* d,
    /* [in] */ Boolean reversed)
{
    Int32 N = 0;
    d->GetNumberOfFrames(&N);
    mFrames = N;

    if (mFrameTimes == NULL || mFrameTimes->GetLength() < N) {
        mFrameTimes = ArrayOf<Int32>::Alloc(N);
    }

    AutoPtr<ArrayOf<Int32> > frameTimes = mFrameTimes;
    Int32 totalDuration = 0;
    for (Int32 i = 0; i < N; i++) {
        Int32 duration = 0;
        d->GetDuration(reversed ? N - i - 1 : i, &duration);
        (*frameTimes)[i] = duration;
        totalDuration += duration;
    }

    mTotalDuration = totalDuration;
    return totalDuration;
}

Int32 AnimatedStateListDrawable::FrameInterpolator::GetTotalDuration()
{
    return mTotalDuration;
}

ECode AnimatedStateListDrawable::FrameInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* result)
{
    Int32 elapsed = (Int32) (input * mTotalDuration + 0.5f);
    Int32 N = mFrames;
    AutoPtr<ArrayOf<Int32> > frameTimes = mFrameTimes;

    // Find the current frame and remaining time within that frame.
    Int32 remaining = elapsed;
    Int32 i = 0;
    while (i < N && remaining >= (*frameTimes)[i]) {
        remaining -= (*frameTimes)[i];
        i++;
    }

    // Remaining time is relative of total duration.
    Float frameElapsed;
    if (i < N) {
        frameElapsed = remaining / (Float) mTotalDuration;
    } else {
        frameElapsed = 0;
    }

    return i / (Float) N + frameElapsed;
}

ECode AnimatedStateListDrawable::FrameInterpolator::HasNativeInterpolator(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

const String AnimatedStateListDrawable::LOGTAG = String("AnimatedStateListDrawable")/*AnimatedStateListDrawable.class.getSimpleName()*/;
const String AnimatedStateListDrawable::ELEMENT_TRANSITION = String("transition");
const String AnimatedStateListDrawable::ELEMENT_ITEM = String("item");

CAR_INTERFACE_IMPL(AnimatedStateListDrawable, StateListDrawable, IAnimatedStateListDrawable);
AnimatedStateListDrawable::AnimatedStateListDrawable()
    : mTransitionToIndex(-1)
    , mTransitionFromIndex(-1)
    , mMutated(FALSE)
{
    constructor(NULL, NULL);
}

ECode AnimatedStateListDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean changed = FALSE;
    StateListDrawable::SetVisible(visible, restart, &changed);

    if (mTransition != NULL && (changed || restart)) {
        if (visible) {
            mTransition->Start();
        } else {
            // Ensure we're showing the correct state when visible.
            JumpToCurrentState();
        }
    }

    *result = changed;
    return NOERROR;
}

ECode AnimatedStateListDrawable::AddState(
    /* [in] */ /*@NonNull*/ ArrayOf<Int32>* stateSet,
    /* [in] */ /*@NonNull*/ IDrawable* drawable,
    /* [in] */ Int32 id)
{
    if (drawable == NULL) {
        // throw new IllegalArgumentException("Drawable must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mState->AddStateSet(stateSet, drawable, id);
    AutoPtr<ArrayOf<Int32> > states;
    GetState((ArrayOf<Int32>**)&states);
    OnStateChange(states);
    return NOERROR;
}

ECode AnimatedStateListDrawable::AddTransition(
    /* [in] */ Int32 fromId,
    /* [in] */ Int32 toId,
    /* [in] */ /*@NonNull*/ IDrawable* transition,
    /* [in] */ Boolean reversible)
{
    if (transition == NULL) {
        // throw new IllegalArgumentException("Transition drawable must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    assert(IDrawable::Probe(transition) != NULL || IAnimatable::Probe(transition) != NULL);

    return mState->AddTransition(fromId, toId, transition, reversible);
}

ECode AnimatedStateListDrawable::IsStateful(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = TRUE;
    return NOERROR;
}

Boolean AnimatedStateListDrawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* stateSet)
{
    Int32 keyframeIndex = mState->IndexOfKeyframe(const_cast<ArrayOf<Int32>*>(stateSet));
    Int32 value = 0;
    if (keyframeIndex == (GetCurrentIndex(&value), value)) {
        // Propagate state change to current keyframe.
        AutoPtr<IDrawable> current;
        GetCurrent((IDrawable**)&current);
        if (current != NULL) {
            Boolean isStateful = FALSE;
            current->SetState(const_cast<ArrayOf<Int32>*>(stateSet), &isStateful);
            return isStateful;
        }
        return FALSE;
    }

    // Attempt to find a valid transition to the keyframe.
    if (SelectTransition(keyframeIndex)) {
        return TRUE;
    }

    // No valid transition, attempt to jump directly to the keyframe.
    Boolean result = FALSE;
    if (SelectDrawable(keyframeIndex, &result), result) {
        return TRUE;
    }

    return StateListDrawable::OnStateChange(stateSet);
}

Boolean AnimatedStateListDrawable::SelectTransition(
    /* [in] */ Int32 toIndex)
{
    Int32 fromIndex = 0;
    AutoPtr<Transition> currentTransition = mTransition;
    if (currentTransition != NULL) {
        if (toIndex == mTransitionToIndex) {
            // Already animating to that keyframe.
            return TRUE;
        } else if (toIndex == mTransitionFromIndex && currentTransition->CanReverse()) {
            // Reverse the current animation.
            currentTransition->Reverse();
            mTransitionToIndex = mTransitionFromIndex;
            mTransitionFromIndex = toIndex;
            return TRUE;
        }

        // Start the next transition from the end of the current one.
        fromIndex = mTransitionToIndex;

        // Changing animation, end the current animation.
        currentTransition->Stop();
    } else {
        GetCurrentIndex(&fromIndex);
    }

    // Reset state.
    mTransition = NULL;
    mTransitionFromIndex = -1;
    mTransitionToIndex = -1;

    AutoPtr<AnimatedStateListState> state = mState;
    Int32 fromId = state->GetKeyframeIdAt(fromIndex);
    Int32 toId = state->GetKeyframeIdAt(toIndex);
    if (toId == 0 || fromId == 0) {
        // Missing a keyframe ID.
        return FALSE;
    }

    Int32 transitionIndex = state->IndexOfTransition(fromId, toId);
    if (transitionIndex < 0) {
        // Couldn't select a transition.
        return FALSE;
    }

    // This may fail if we're already on the transition, but that's okay!
    Boolean result = FALSE;
    SelectDrawable(transitionIndex, &result);

    AutoPtr<Transition> transition;
    AutoPtr<IDrawable> d;
    GetCurrent((IDrawable**)&d);
    if (IAnimationDrawable::Probe(d)) {
        Boolean reversed = state->IsTransitionReversed(fromId, toId);
        transition = new AnimationDrawableTransition(IAnimationDrawable::Probe(d), reversed);
    } else if (IAnimatedVectorDrawable::Probe(d)) {
        Boolean reversed = state->IsTransitionReversed(fromId, toId);
        transition = new AnimatedVectorDrawableTransition(IAnimatedVectorDrawable::Probe(d), reversed);
    } else if (IAnimatable::Probe(d)) {
        transition = new AnimatableTransition(IAnimatable::Probe(d));
    } else {
        // We don't know how to animate this transition.
        return FALSE;
    }

    transition->Start();

    mTransition = transition;
    mTransitionFromIndex = fromIndex;
    mTransitionToIndex = toIndex;
    return TRUE;
}

ECode AnimatedStateListDrawable::JumpToCurrentState()
{
    StateListDrawable::JumpToCurrentState();

    if (mTransition != NULL) {
        mTransition->Stop();
        mTransition = NULL;

        Boolean result = FALSE;
        SelectDrawable(mTransitionToIndex, &result);
        mTransitionToIndex = -1;
        mTransitionFromIndex = -1;
    }
    return NOERROR;
}

ECode AnimatedStateListDrawable::Inflate(
    /* [in] */ /*@NonNull*/ IResources* r,
    /* [in] */ /*@NonNull*/ IXmlPullParser* parser,
    /* [in] */ /*@NonNull*/ IAttributeSet* attrs,
    /* [in] */ /*@Nullable*/ IResourcesTheme* theme) /*throws XmlPullParserException, IOException*/
{
    AutoPtr<ITypedArray> a;
    Int32 size = ArraySize(R::styleable::AnimatedStateListDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AnimatedStateListDrawable, size);
    FAIL_RETURN(ObtainAttributes(r, theme, attrs, layout, (ITypedArray**)&a));

    StateListDrawable::InflateWithAttributes(r, parser, a, R::styleable::AnimatedStateListDrawable_visible);

    AutoPtr<StateListState> stateListState = GetStateListState();
    Boolean value = FALSE;
    Int32 ivalue = 0;
    a->GetBoolean(R::styleable::AnimatedStateListDrawable_variablePadding, FALSE, &value);
    stateListState->SetVariablePadding(value);
    a->GetBoolean(R::styleable::AnimatedStateListDrawable_constantSize, FALSE, &value);
    stateListState->SetConstantSize(value);

    a->GetInt32(R::styleable::AnimatedStateListDrawable_enterFadeDuration, 0, &ivalue);
    stateListState->SetEnterFadeDuration(ivalue);
    a->GetInt32(R::styleable::AnimatedStateListDrawable_exitFadeDuration, 0, &ivalue);
    stateListState->SetExitFadeDuration(ivalue);

    a->GetBoolean(R::styleable::AnimatedStateListDrawable_dither, TRUE, &value);
    SetDither(value);
    a->GetBoolean(R::styleable::AnimatedStateListDrawable_autoMirrored, FALSE, &value);
    SetAutoMirrored(value);
    a->Recycle();

    Int32 type = 0;

    Int32 innerDepth = 0;
    parser->GetDepth(&innerDepth);
    innerDepth += 1;
    Int32 depth = 0;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
            && ((parser->GetDepth(&depth), depth) >= innerDepth
            || type != IXmlPullParser::END_TAG)) {
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        if (depth > innerDepth) {
            continue;
        }

        String name;
        parser->GetName(&name);
        if (name.Equals(ELEMENT_ITEM)) {
            Int32 value = 0;
            FAIL_RETURN(ParseItem(r, parser, attrs, theme, &value));
        } else if (name.Equals(ELEMENT_TRANSITION)) {
            Int32 value = 0;
            FAIL_RETURN(ParseTransition(r, parser, attrs, theme, &value));
        }
    }

    AutoPtr<ArrayOf<Int32> > states;
    GetState((ArrayOf<Int32>**)&states);
    OnStateChange(states);
    return NOERROR;
}

ECode AnimatedStateListDrawable::ParseTransition(
    /* [in] */ /*@NonNull*/ IResources* r,
    /* [in] */ /*@NonNull*/ IXmlPullParser* parser,
    /* [in] */ /*@NonNull*/ IAttributeSet* attrs,
    /* [in] */ /*@Nullable*/ IResourcesTheme* theme,
    /* [out] */ Int32* result) /*throws XmlPullParserException, IOException*/
{
    VALIDATE_NOT_NULL(result);
    Int32 drawableRes = 0;
    Int32 fromId = 0;
    Int32 toId = 0;
    Boolean reversible = FALSE;

    Int32 numAttrs = 0;
    attrs->GetAttributeCount(&numAttrs);
    for (Int32 i = 0; i < numAttrs; i++) {
        Int32 stateResId = 0;
        attrs->GetAttributeNameResource(i, &stateResId);
        switch (stateResId) {
            case 0:
                break;
            case R::attr::fromId:
                attrs->GetAttributeResourceValue(i, 0, &fromId);
                break;
            case R::attr::toId:
                attrs->GetAttributeResourceValue(i, 0, &toId);
                break;
            case R::attr::drawable:
                attrs->GetAttributeResourceValue(i, 0, &drawableRes);
                break;
            case R::attr::reversible:
                attrs->GetAttributeBooleanValue(i, FALSE, &reversible);
                break;
        }
    }

    AutoPtr<IDrawable> dr;
    if (drawableRes != 0) {
        r->GetDrawable(drawableRes, theme, (IDrawable**)&dr);
    } else {
        Int32 type = 0;
        while ((parser->Next(&type), type) == IXmlPullParser::TEXT) {
        }
        if (type != IXmlPullParser::START_TAG) {
            // throw new XmlPullParserException(
            //         parser.getPositionDescription()
            //                 + ": <item> tag requires a 'drawable' attribute or "
            //                 + "child tag defining a drawable");
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        FAIL_RETURN(Drawable::CreateFromXmlInner(r, parser, attrs, theme, (IDrawable**)&dr));
    }

    *result = mState->AddTransition(fromId, toId, dr, reversible);
    return NOERROR;
}

ECode AnimatedStateListDrawable::ParseItem(
    /* [in] */ /*@NonNull*/ IResources* r,
    /* [in] */ /*@NonNull*/ IXmlPullParser* parser,
    /* [in] */ /*@NonNull*/ IAttributeSet* attrs,
    /* [in] */ /*@Nullable*/ IResourcesTheme* theme,
    /* [out] */ Int32* result) /*throws XmlPullParserException, IOException*/
{
    VALIDATE_NOT_NULL(result);
    Int32 drawableRes = 0;
    Int32 keyframeId = 0;

    Int32 j = 0;
    Int32 numAttrs = 0;
    attrs->GetAttributeCount(&numAttrs);
    AutoPtr<ArrayOf<Int32> > states = ArrayOf<Int32>::Alloc(numAttrs);
    for (Int32 i = 0; i < numAttrs; i++) {
        Int32 stateResId = 0;
        attrs->GetAttributeNameResource(i, &stateResId);
        switch (stateResId) {
            case 0:
                break;
            case R::attr::id:
                attrs->GetAttributeResourceValue(i, 0, &keyframeId);
                break;
            case R::attr::drawable:
                attrs->GetAttributeResourceValue(i, 0, &drawableRes);
                break;
            default:
                Boolean hasState = FALSE;
                attrs->GetAttributeBooleanValue(i, FALSE, &hasState);
                (*states)[j++] = hasState ? stateResId : -stateResId;
        }
    }
    states = StateSet::TrimStateSet(states, j);

    AutoPtr<IDrawable> dr;
    if (drawableRes != 0) {
        r->GetDrawable(drawableRes, theme, (IDrawable**)&dr);
    } else {
        Int32 type = 0;
        while ((parser->Next(&type), type) == IXmlPullParser::TEXT) {
        }
        if (type != IXmlPullParser::START_TAG) {
            // throw new XmlPullParserException(
            //         parser.getPositionDescription()
            //                 + ": <item> tag requires a 'drawable' attribute or "
            //                 + "child tag defining a drawable");
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        FAIL_RETURN(Drawable::CreateFromXmlInner(r, parser, attrs, theme, (IDrawable**)&dr));
    }

    *result = mState->AddStateSet(states, dr, keyframeId);
    return NOERROR;
}

ECode AnimatedStateListDrawable::Mutate(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> dr;
    if (!mMutated && (StateListDrawable::Mutate((IDrawable**)&dr), dr.Get()) == this) {
        AutoPtr<AnimatedStateListState> newState = new AnimatedStateListState(mState, this, NULL);
        SetConstantState(newState);
        mMutated = TRUE;
    }

    *drawable = THIS_PROBE(IDrawable);
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

void AnimatedStateListDrawable::SetConstantState(
    /* [in] */ /*@NonNull*/ AnimatedStateListState* state)
{
    assert(state != NULL);
    StateListDrawable::SetConstantState(state);

    mState = state;
}

AnimatedStateListDrawable::AnimatedStateListDrawable(
    /* [in] */ /*@Nullable*/ AnimatedStateListState* state,
    /* [in] */ /*@Nullable*/ IResources* res)
    : mTransitionToIndex(-1)
    , mTransitionFromIndex(-1)
    , mMutated(FALSE)
{
    constructor(state, res);
}

ECode AnimatedStateListDrawable::constructor(
    /* [in] */ /*@Nullable*/ AnimatedStateListState* state,
    /* [in] */ /*@Nullable*/ IResources* res)
{
    StateListDrawable::constructor(NULL);
    AutoPtr<AnimatedStateListState> newState = new AnimatedStateListState(state, this, res);
    SetConstantState(newState);
    AutoPtr<ArrayOf<Int32> > states;
    GetState((ArrayOf<Int32>**)&states);
    OnStateChange(states);
    return JumpToCurrentState();
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
