
#include "ext/frameworkext.h"
#include "graphics/drawable/StateListDrawable.h"
#include "graphics/drawable/CStateListDrawable.h"
#include "util/StateSet.h"
#include "R.h"

using Elastos::Droid::Utility::StateSet;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

const Boolean StateListDrawable::DEFAULT_DITHER;
Boolean StateListDrawable::DEBUG = FALSE;
String StateListDrawable::TAG = String("StateListDrawable");

StateListDrawable::StateListDrawable()
    : mMutated(FALSE)
{
}

StateListDrawable::StateListDrawable(
    /* [in] */ StateListState* state,
    /* [in] */ IResources* res)
    : mMutated(FALSE)
{
    Init(state, res);
}

ECode StateListDrawable::Init(
    /* [in] */ StateListState* state,
    /* [in] */ IResources* res)
{
    mStateListState = new StateListState(state, this, res);
    SetConstantState(mStateListState);
    OnStateChange(GetState());
    return NOERROR;
}

/**
 * Add a new image/string ID to the set of images.
 *
 * @param stateSet - An array of resource Ids to associate with the image.
 *                 Switch to this image by calling setState().
 * @param drawable -The image to show.
 */
ECode StateListDrawable::AddState(
    /* [in] */ const ArrayOf<Int32>& stateSet,
    /* [in] */ IDrawable* drawable)
{
    if (drawable != NULL) {
        mStateListState->AddStateSet(&stateSet, drawable);
        // in case the new state matches our current state...
        OnStateChange(GetState());
    }
    return NOERROR;
}

Boolean StateListDrawable::IsStateful()
{
    return TRUE;
}

Boolean StateListDrawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* stateSet)
{
    Int32 idx = mStateListState->IndexOfStateSet(stateSet);
    // if (DEBUG) android.util.Log.i(TAG, "onStateChange " + this + " states "
    //             + Arrays.toString(stateSet) + " found " + idx);
    if (idx < 0) {
        idx = mStateListState->IndexOfStateSet(const_cast<ArrayOf<Int32>*>(StateSet::WILD_CARD.Get()));
    }
    if (SelectDrawable(idx)) {
        return TRUE;
    }
    return DrawableContainer::OnStateChange(stateSet);
}

ECode StateListDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::StateListDrawable),
            ARRAY_SIZE(R::styleable::StateListDrawable));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(r->ObtainAttributes(attrs, attrIds, (ITypedArray**)&a));

    Drawable::InflateWithAttributes(r, parser, a,
            R::styleable::StateListDrawable_visible);

    Boolean value;
    a->GetBoolean(R::styleable::StateListDrawable_variablePadding, FALSE, &value);
    mStateListState->SetVariablePadding(value);
    a->GetBoolean(R::styleable::StateListDrawable_constantSize, FALSE, &value);
    mStateListState->SetConstantSize(value);
    Int32 iVal = 0;
    a->GetInt32(R::styleable::StateListDrawable_enterFadeDuration, 0, &iVal);
    mStateListState->SetEnterFadeDuration(iVal);
    a->GetInt32(R::styleable::StateListDrawable_exitFadeDuration, 0, &iVal);
    mStateListState->SetExitFadeDuration(iVal);

    a->GetBoolean(R::styleable::StateListDrawable_dither, DEFAULT_DITHER, &value);
    SetDither(value);

    a->Recycle();

    Int32 innerDepth;
    parser->GetDepth(&innerDepth);
    innerDepth += 1;
    Int32 type, depth;
    while((parser->Next(&type), type != IXmlPullParser::END_DOCUMENT) &&
            ((parser->GetDepth(&depth), depth >= innerDepth) || type != IXmlPullParser::END_TAG)) {
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        String name;
        if (depth > innerDepth || (parser->GetName(&name), !name.Equals("item"))) {
            continue;
        }

        Int32 drawableRes = 0;

        Int32 i;
        Int32 j = 0;
        Int32 numAttrs;
        attrs->GetAttributeCount(&numAttrs);
        AutoPtr< ArrayOf<Int32> > states = ArrayOf<Int32>::Alloc(numAttrs);
        for (i = 0; i < numAttrs; i++) {
            Int32 stateResId;
            attrs->GetAttributeNameResource(i, &stateResId);
            if (stateResId == 0) break;
            if (stateResId == R::attr::drawable) {
                attrs->GetAttributeResourceValueEx(i, 0, &drawableRes);
            }
            else {
                Boolean value;
                attrs->GetAttributeBooleanValueEx(i, FALSE, &value);
                (*states)[j++] = value ? stateResId : -stateResId;
            }
        }
        states = StateSet::TrimStateSet(states, j);

        AutoPtr<IDrawable> dr;
        if (drawableRes != 0) {
            r->GetDrawable(drawableRes, (IDrawable**)&dr);
        }
        else {
            while (parser->Next(&type), type == IXmlPullParser::TEXT) {
            }
            if (type != IXmlPullParser::START_TAG) {
//                throw new XmlPullParserException(
//                        parser.getPositionDescription()
//                                + ": <item> tag requires a 'drawable' attribute or "
//                                + "child tag defining a drawable");
                return E_XML_PULL_PARSER_EXCEPTION;
            }
            Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&dr);
        }

        mStateListState->AddStateSet(states, dr);
    }

    OnStateChange(GetState());
    return NOERROR;
}

AutoPtr<StateListDrawable::StateListState> StateListDrawable::GetStateListState()
{
    return mStateListState;
}

/**
 * Gets the number of states contained in this drawable.
 *
 * @return The number of states contained in this drawable.
 * @hide pending API council
 * @see #getStateSet(Int32)
 * @see #getStateDrawable(Int32)
 */
Int32 StateListDrawable::GetStateCount()
{
    return mStateListState->GetChildCount();
}

/**
 * Gets the state set at an index.
 *
 * @param index The index of the state set.
 * @return The state set at the index.
 * @hide pending API council
 * @see #getStateCount()
 * @see #getStateDrawable(Int32)
 */
AutoPtr< ArrayOf<Int32> > StateListDrawable::GetStateSet(
    /* [in] */ Int32 index)
{
    return (*mStateListState->mStateSets)[index];
}

/**
 * Gets the drawable at an index.
 *
 * @param index The index of the drawable.
 * @return The drawable at the index.
 * @hide pending API council
 * @see #getStateCount()
 * @see #getStateSet(Int32)
 */
AutoPtr<IDrawable> StateListDrawable::GetStateDrawable(
    /* [in] */ Int32 index)
{
    return (*mStateListState->GetChildren())[index];
}

/**
 * Gets the index of the drawable with the provided state set.
 *
 * @param stateSet the state set to look up
 * @return the index of the provided state set, or -1 if not found
 * @hide pending API council
 * @see #getStateDrawable(Int32)
 * @see #getStateSet(Int32)
 */
Int32 StateListDrawable::GetStateDrawableIndex(
    /* [in] */ const ArrayOf<Int32>& stateSet)
{
    return mStateListState->IndexOfStateSet((ArrayOf<Int32>*)&stateSet);
}

AutoPtr<IDrawable> StateListDrawable::Mutate()
{
    if (!mMutated && DrawableContainer::Mutate().Get() == (IDrawable*)this->Probe(EIID_IDrawable)) {
        AutoPtr<ArrayOf< AutoPtr<ArrayOf<Int32> > > > sets = mStateListState->mStateSets;
        Int32 count = sets->GetLength();
        mStateListState->mStateSets = ArrayOf< AutoPtr<ArrayOf<Int32> > >::Alloc(count);

        for (Int32 i = 0; i < count; i++) {
            AutoPtr<ArrayOf<Int32> > set = (*sets)[i];
            if (set != NULL) {
                mStateListState->mStateSets->Set(i, set);
            }
        }
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

ECode StateListDrawable::SetLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    const Int32 numStates = GetStateCount();
    for (Int32 i = 0; i < numStates; i++) {
        GetStateDrawable(i)->SetLayoutDirection(layoutDirection);
    }
    return Drawable::SetLayoutDirection(layoutDirection);
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

StateListDrawable::StateListState::StateListState(
    /* [in] */ StateListState* orig,
    /* [in] */ StateListDrawable* owner,
    /* [in] */ IResources* res)
    : DrawableContainerState(orig, owner, res)
{
    if (orig != NULL) {
        mStateSets = orig->mStateSets;
    }
    else {
        mStateSets = ArrayOf< AutoPtr<ArrayOf<Int32> > >::Alloc(GetChildren()->GetLength());
    }
}

StateListDrawable::StateListState::~StateListState()
{
}

Int32 StateListDrawable::StateListState::AddStateSet(
    /* [in] */ const ArrayOf<Int32>* stateSet,
    /* [in] */ IDrawable* drawable)
{
    Int32 pos = AddChild(drawable);
    AutoPtr<ArrayOf<Int32> > set = const_cast< ArrayOf<Int32>* >(stateSet);
    mStateSets->Set(pos, set);
    return pos;
}

Int32 StateListDrawable::StateListState::IndexOfStateSet(
    /* [in] */ const ArrayOf<Int32>* stateSet)
{
    Int32 N = GetChildCount();
    for (Int32 i = 0; i < N; i++) {
        if (StateSet::StateSetMatches((*mStateSets)[i].Get(), stateSet)) {
            return i;
        }
    }
    return -1;
}

// IDrawableConstantState
//
ECode StateListDrawable::StateListState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    return CStateListDrawable::New((Handle32)this, NULL,
        (IStateListDrawable**)drawable);
}

ECode StateListDrawable::StateListState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return CStateListDrawable::New((Handle32)this, res,
        (IStateListDrawable**)drawable);
}

void StateListDrawable::StateListState::GrowArray(
    /* [in] */ Int32 oldSize,
    /* [in] */ Int32 newSize)
{
    DrawableContainerState::GrowArray(oldSize, newSize);
    AutoPtr<ArrayOf< AutoPtr<ArrayOf<Int32> > > > newStateSets
        = ArrayOf< AutoPtr<ArrayOf<Int32> > >::Alloc(newSize);
    for (Int32 i = 0; i < oldSize; i++) {
        newStateSets->Set(i, (*mStateSets)[i]);
    }

    mStateSets = newStateSets;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
