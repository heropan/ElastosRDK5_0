
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/graphics/drawable/StateListDrawable.h"
#include "elastos/droid/graphics/drawable/CStateListDrawable.h"
#include "elastos/droid/utility/StateSet.h"
#include "elastos/droid/R.h"

using Elastos::Droid::Utility::StateSet;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

const Boolean StateListDrawable::DEFAULT_DITHER;
Boolean StateListDrawable::DEBUG = FALSE;
String StateListDrawable::TAG("StateListDrawable");

CAR_INTERFACE_IMPL(StateListDrawable, DrawableContainer, IStateListDrawable)
StateListDrawable::StateListDrawable()
    : mMutated(FALSE)
{
}

StateListDrawable::StateListDrawable(
    /* [in] */ StateListState* state,
    /* [in] */ IResources* res)
    : mMutated(FALSE)
{
    constructor(state, res);
}

ECode StateListDrawable::constructor(
    /* [in] */ StateListState* state,
    /* [in] */ IResources* res)
{
    mStateListState = new StateListState(state, this, res);
    SetConstantState(mStateListState);

    AutoPtr<ArrayOf<Int32> > states;
    GetState((ArrayOf<Int32>**)&states);
    OnStateChange(states);
    return NOERROR;
}

ECode StateListDrawable::AddState(
    /* [in] */ ArrayOf<Int32>* stateSet,
    /* [in] */ IDrawable* drawable)
{
    if (drawable != NULL) {
        mStateListState->AddStateSet(stateSet, drawable);
        // in case the new state matches our current state...
        AutoPtr<ArrayOf<Int32> > states;
        GetState((ArrayOf<Int32>**)&states);
        OnStateChange(states);
    }
    return NOERROR;
}

ECode StateListDrawable::IsStateful(
    /* [out] */ Boolean* isStateful)
{
    VALIDATE_NOT_NULL(isStateful);
    *isStateful = TRUE;
    return NOERROR;
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

    Boolean res = FALSE;
    if (SelectDrawable(idx, &res), res) {
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
                attrs->GetAttributeResourceValue(i, 0, &drawableRes);
            }
            else {
                Boolean value;
                attrs->GetAttributeBooleanValue(i, FALSE, &value);
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

    AutoPtr<ArrayOf<Int32> > states;
    GetState((ArrayOf<Int32>**)&states);
    OnStateChange(states);
    return NOERROR;
}

AutoPtr<StateListDrawable::StateListState> StateListDrawable::GetStateListState()
{
    return mStateListState;
}

ECode StateListDrawable::GetStateCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mStateListState->GetChildCount();
    return NOERROR;
}

ECode StateListDrawable::GetStateSet(
    /* [in] */ Int32 index,
    /* [out, callee] */ ArrayOf<Int32>** stateSet)
{
    VALIDATE_NOT_NULL(stateSet);
    *stateSet = (*mStateListState->mStateSets)[index];
    REFCOUNT_ADD(*stateSet);
    return NOERROR;
}

ECode StateListDrawable::GetStateDrawable(
    /* [in] */ Int32 index,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    *drawable = (*mStateListState->GetChildren())[index];
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode StateListDrawable::GetStateDrawableIndex(
    /* [in] */ ArrayOf<Int32>* stateSet,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mStateListState->IndexOfStateSet((ArrayOf<Int32>*)&stateSet);
    return NOERROR;
}

ECode StateListDrawable::Mutate(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> tmp;
    if (!mMutated && (DrawableContainer::Mutate((IDrawable**)&tmp), tmp.Get()) == (IDrawable*)this->Probe(EIID_IDrawable)) {
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
    *drawable = (IDrawable*)this->Probe(EIID_IDrawable);
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode StateListDrawable::SetLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    Int32 numStates = 0;
    GetStateCount(&numStates);
    for (Int32 i = 0; i < numStates; i++) {
        AutoPtr<IDrawable> drawable;
        GetStateDrawable(i, (IDrawable**)&drawable);
        drawable->SetLayoutDirection(layoutDirection);
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

ECode StateListDrawable::StateListState::NewDrawable(
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
