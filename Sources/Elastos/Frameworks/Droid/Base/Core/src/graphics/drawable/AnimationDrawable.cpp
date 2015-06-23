
#include "ext/frameworkext.h"
#include "graphics/drawable/AnimationDrawable.h"
#include "graphics/drawable/CAnimationDrawable.h"
#include "os/SystemClock.h"
#include "R.h"

using Elastos::Core::EIID_IRunnable;

using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

AnimationDrawable::AnimationState::AnimationState(
    /* [in] */ AnimationState* orig,
    /* [in] */ AnimationDrawable* owner,
    /* [in] */ IResources* res)
    : DrawableContainerState(orig, owner, res)
    , mOneShot(FALSE)
{
    if (orig != NULL) {
        mDurations = orig->mDurations;
        mOneShot = orig->mOneShot;
    }
    else {
        mDurations = ArrayOf<Int32>::Alloc(GetChildren()->GetLength());
        mOneShot = TRUE;
    }
}

AnimationDrawable::AnimationState::~AnimationState()
{
}

ECode AnimationDrawable::AnimationState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    return CAnimationDrawable::New((Handle32)this, NULL, (IAnimationDrawable**)drawable);
}

ECode AnimationDrawable::AnimationState::NewDrawable(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return CAnimationDrawable::New((Handle32)this, res, (IAnimationDrawable**)drawable);
}

void AnimationDrawable::AnimationState::AddFrame(
    /* [in] */ IDrawable* dr,
    /* [in] */ Int32 dur)
{
    // Do not combine the following. The array index must be evaluated before
    // the array is accessed because super.addChild(dr) has a side effect on mDurations.
    Int32 pos = DrawableContainerState::AddChild(dr);
    (*mDurations)[pos] = dur;
}

void AnimationDrawable::AnimationState::GrowArray(
    /* [in] */ Int32 oldSize,
    /* [in] */ Int32 newSize)
{
    DrawableContainerState::GrowArray(oldSize, newSize);
    AutoPtr<ArrayOf<Int32> > newDurations = ArrayOf<Int32>::Alloc(newSize);
    newDurations->Copy(mDurations, 0, oldSize);
    mDurations = newDurations;
}


AnimationDrawable::AnimationDrawable()
    : mCurFrame(-1)
    , mMutated(FALSE)
{
}

Boolean AnimationDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean changed = DrawableContainer::SetVisible(visible, restart);
    if (visible) {
        if (changed || restart) {
            SetFrame(0, TRUE, TRUE);
        }
    }
    else {
        UnscheduleSelf((IRunnable*)this->Probe(EIID_IRunnable));
    }
    return changed;
}

ECode AnimationDrawable::Start()
{
    if (!IsRunning()) {
        return Run();
    }
    return NOERROR;
}

ECode AnimationDrawable::Stop()
{
    if (IsRunning()) {
        return UnscheduleSelf((IRunnable*)this->Probe(EIID_IRunnable));
    }
    return NOERROR;
}

Boolean AnimationDrawable::IsRunning()
{
    return mCurFrame > -1;
}

ECode AnimationDrawable::Run()
{
    return NextFrame(FALSE);
}

ECode AnimationDrawable::UnscheduleSelf(
    /* [in] */ IRunnable* what)
{
    mCurFrame = -1;
    return DrawableContainer::UnscheduleSelf(what);
}

Int32 AnimationDrawable::GetNumberOfFrames()
{
    return mAnimationState->GetChildCount();
}

AutoPtr<IDrawable> AnimationDrawable::GetFrame(
    /* [in] */ Int32 index)
{
    return (*mAnimationState->GetChildren())[index];
}

Int32 AnimationDrawable::GetDuration(
    /* [in] */ Int32 i)
{
    return (*mAnimationState->mDurations)[i];
}

Boolean AnimationDrawable::IsOneShot()
{
    return mAnimationState->mOneShot;
}

ECode AnimationDrawable::SetOneShot(
    /* [in] */ Boolean oneShot)
{
    mAnimationState->mOneShot = oneShot;
    return NOERROR;
}

ECode AnimationDrawable::AddFrame(
    /* [in] */ IDrawable* frame,
    /* [in] */ Int32 duration)
{
    mAnimationState->AddFrame(frame, duration);
    if (mCurFrame < 0) {
        SetFrame(0, TRUE, FALSE);
    }
    return NOERROR;
}

ECode AnimationDrawable::NextFrame(
    /* [in] */ Boolean unschedule)
{
    Int32 next = mCurFrame + 1;
    const Int32 N = mAnimationState->GetChildCount();
    if (next >= N) {
        next = 0;
    }
    return SetFrame(next, unschedule, !mAnimationState->mOneShot || next < (N - 1));
}

ECode AnimationDrawable::SetFrame(
    /* [in] */ Int32 frame,
    /* [in] */ Boolean unschedule,
    /* [in] */ Boolean animate)
{
    if (frame >= mAnimationState->GetChildCount()) {
        return NOERROR;
    }
    mCurFrame = frame;
    SelectDrawable(frame);
    if (unschedule) {
        FAIL_RETURN(UnscheduleSelf((IRunnable*)this->Probe(EIID_IRunnable)));
    }
    if (animate) {
        // Unscheduling may have clobbered this value; restore it to record that we're animating
        mCurFrame = frame;
        FAIL_RETURN(ScheduleSelf((IRunnable*)this->Probe(EIID_IRunnable),
                SystemClock::GetUptimeMillis() + (*mAnimationState->mDurations)[frame]));
    }
    return NOERROR;
}

ECode AnimationDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    Int32 size = ARRAY_SIZE(R::styleable::AnimationDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AnimationDrawable, size);

    AutoPtr<ITypedArray> a;
    r->ObtainAttributes(attrs, layout,
            (ITypedArray**)&a);

    FAIL_RETURN(DrawableContainer::InflateWithAttributes(r, parser, a,
            R::styleable::AnimationDrawable_visible));

    Boolean padding;
    a->GetBoolean(
            R::styleable::AnimationDrawable_variablePadding, FALSE, &padding);
    mAnimationState->SetVariablePadding(padding);

    a->GetBoolean(
            R::styleable::AnimationDrawable_oneshot, FALSE, &mAnimationState->mOneShot);

    a->Recycle();

    Int32 type;

    Int32 innerDepth, depth;
    parser->GetDepth(&depth);
    innerDepth = depth + 1;
    while ((parser->Next(&type), type != IXmlPullParser::END_DOCUMENT) &&
            ((parser->GetDepth(&depth), depth >= innerDepth) || type != IXmlPullParser::END_TAG)) {
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        String name;
        if (depth > innerDepth || (parser->GetName(&name), !name.Equals("item"))) {
            continue;
        }

        a = NULL;
        size = ARRAY_SIZE(R::styleable::AnimationDrawableItem);
        layout = ArrayOf<Int32>::Alloc(size);
        layout->Copy(R::styleable::AnimationDrawableItem, size);

        r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);
        Int32 duration;
        a->GetInt32(
                R::styleable::AnimationDrawableItem_duration, -1, &duration);
        if (duration < 0) {
//            throw new XmlPullParserException(
//                    parser.getPositionDescription()
//                    + ": <item> tag requires a 'duration' attribute");
            return E_XML_PULL_PARSER_EXCEPTION;
        }
        Int32 drawableRes;
        a->GetResourceId(
                R::styleable::AnimationDrawableItem_drawable, 0, &drawableRes);

        a->Recycle();

        AutoPtr<IDrawable> dr;
        if (drawableRes != 0) {
            r->GetDrawable(drawableRes, (IDrawable**)&dr);
        }
        else {
            while (parser->Next(&type), type == IXmlPullParser::TEXT) {
                // Empty
            }
            if (type != IXmlPullParser::START_TAG) {
//                throw new XmlPullParserException(parser.getPositionDescription() +
//                        ": <item> tag requires a 'drawable' attribute or child tag" +
//                        " defining a drawable");
                return E_XML_PULL_PARSER_EXCEPTION;
            }
            Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&dr);
        }

        mAnimationState->AddFrame(dr, duration);
        if (dr != NULL) {
            dr->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
        }
    }

    return SetFrame(0, TRUE, FALSE);
}

AutoPtr<IDrawable> AnimationDrawable::Mutate()
{
    if (!mMutated && DrawableContainer::Mutate().Get() ==
            (IDrawable*)this->Probe(EIID_IDrawable)) {
        mAnimationState->mDurations = mAnimationState->mDurations->Clone();
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

AnimationDrawable::AnimationDrawable(
    /* [in] */ AnimationState* state,
    /* [in] */ IResources* res)
{
    ASSERT_SUCCEEDED(Init(state, res));
}

ECode AnimationDrawable::Init(
    /* [in] */ AnimationState* state,
    /* [in] */ IResources* res)
{
    mAnimationState = new AnimationState(state, this, res);
    SetConstantState(mAnimationState);
    if (state != NULL) {
        SetFrame(0, TRUE, FALSE);
    }
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
