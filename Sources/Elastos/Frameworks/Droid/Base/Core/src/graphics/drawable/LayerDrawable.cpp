
#include "ext/frameworkext.h"
#include "graphics/drawable/LayerDrawable.h"
#include "graphics/drawable/CLayerDrawable.h"
#include "graphics/CRect.h"
#include "R.h"

using Elastos::Droid::View::IView;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

LayerDrawable::LayerState::LayerState(
    /* [in] */ LayerState* orig,
    /* [in] */ LayerDrawable* owner,
    /* [in] */ IResources* res)
    : mNum(0)
    , mChangingConfigurations(0)
    , mChildrenChangingConfigurations(0)
    , mHaveOpacity(FALSE)
    , mOpacity(0)
    , mHaveStateful(FALSE)
    , mStateful(FALSE)
    , mCheckedConstantState(FALSE)
    , mCanConstantState(FALSE)
{
    if (orig != NULL) {
        AutoPtr<ArrayOf<ChildDrawable*> > origChildDrawable = orig->mChildren;
        Int32 N = orig->mNum;
        mNum = N;
        mChildren = ArrayOf<ChildDrawable*>::Alloc(N);

        mChangingConfigurations = orig->mChangingConfigurations;
        mChildrenChangingConfigurations = orig->mChildrenChangingConfigurations;

        for (Int32 i = 0; i < N; i++) {
            AutoPtr<ChildDrawable> r = new ChildDrawable();
            mChildren->Set(i, r);
            AutoPtr<ChildDrawable> r2 = (*origChildDrawable)[i];
            AutoPtr<IDrawableConstantState> state;
            r2->mDrawable->GetConstantState((IDrawableConstantState**)&state);
            if (res != NULL) {
                state->NewDrawableEx(res, (IDrawable**)(&(r->mDrawable)));
            }
            else {
                state->NewDrawable((IDrawable**)(&(r->mDrawable)));
            }
            r->mDrawable->SetCallback(
                owner != NULL ? (IDrawableCallback*)owner->Probe(EIID_IDrawableCallback) : NULL);
            r->mInsetL = r2->mInsetL;
            r->mInsetT = r2->mInsetT;
            r->mInsetR = r2->mInsetR;
            r->mInsetB = r2->mInsetB;
            r->mId = r2->mId;
        }

        mHaveOpacity = orig->mHaveOpacity;
        mOpacity = orig->mOpacity;
        mHaveStateful = orig->mHaveStateful;
        mStateful = orig->mStateful;
        mCheckedConstantState = mCanConstantState = TRUE;
    }
    else {
        mNum = 0;
        mChildren = NULL;
    }
}

LayerDrawable::LayerState::~LayerState()
{}

ECode LayerDrawable::LayerState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    return CLayerDrawable::New(this, NULL, (ILayerDrawable**)drawable);
}

ECode LayerDrawable::LayerState::NewDrawableEx(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return CLayerDrawable::New(this, res, (ILayerDrawable**)drawable);
}

ECode LayerDrawable::LayerState::GetChangingConfigurations(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChangingConfigurations;
    return NOERROR;
}

Int32 LayerDrawable::LayerState::GetOpacity()
{
    if (mHaveOpacity) {
        return mOpacity;
    }

    Int32 N = mNum;
    Int32 op = IPixelFormat::TRANSPARENT;
    if (N > 0) {
        (*mChildren)[0]->mDrawable->GetOpacity(&op);
    }
    for (Int32 i = 1; i < N; i++) {
        Int32 op2;
        (*mChildren)[i]->mDrawable->GetOpacity(&op2);
        op = Drawable::ResolveOpacity(op, op2);
    }
    mOpacity = op;
    mHaveOpacity = TRUE;
    return op;
}

Boolean LayerDrawable::LayerState::IsStateful()
{
    if (mHaveStateful) {
        return mStateful;
    }

    Boolean stateful = FALSE;
    Int32 N = mNum;
    for (Int32 i = 0; i < N; i++) {
        Boolean isStateful;
        (*mChildren)[i]->mDrawable->IsStateful(&isStateful);
        if (isStateful) {
            stateful = TRUE;
            break;
        }
    }

    mStateful = stateful;
    mHaveStateful = TRUE;
    return stateful;
}

Boolean LayerDrawable::LayerState::CanConstantState()
{
    if (!mCheckedConstantState && mChildren != NULL) {
        mCanConstantState = TRUE;
        Int32 N = mNum;
        for (Int32 i=0; i<N; i++) {
            AutoPtr<IDrawableConstantState> state;
            (*mChildren)[i]->mDrawable->GetConstantState(
                (IDrawableConstantState**)&state);
            if (state == NULL) {
                mCanConstantState = FALSE;
                break;
            }
        }
        mCheckedConstantState = TRUE;
    }

    return mCanConstantState;
}


LayerDrawable::LayerDrawable()
    : mOpacityOverride(IPixelFormat::UNKNOWN)
    , mMutated(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mTmpRect));
    Init((LayerState*)NULL, NULL);
}

/**
 * Create a new layer drawable with the specified list of layers and the specified
 * constant state.
 *
 * @param layers The list of layers to add to this drawable.
 * @param state The constant drawable state.
 */
LayerDrawable::LayerDrawable(
    /* [in] */ ArrayOf<IDrawable*>* layers,
    /* [in] */ LayerState* state)
    : mOpacityOverride(IPixelFormat::UNKNOWN)
    , mMutated(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mTmpRect));
    Init(layers, state);
}

LayerDrawable::LayerDrawable(
    /* [in] */ LayerState* state,
    /* [in] */ IResources* res)
    : mOpacityOverride(IPixelFormat::UNKNOWN)
    , mMutated(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mTmpRect));
    Init(state, res);
}

LayerDrawable::~LayerDrawable()
{}

AutoPtr<LayerDrawable::LayerState> LayerDrawable::CreateConstantState(
    /* [in] */ LayerState* state,
    /* [in] */ IResources* res)
{
    return new LayerState(state, this, res);
}

ECode LayerDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    Drawable::Inflate(r, parser, attrs);

    Int32 type;

    Int32 size = ARRAY_SIZE(R::styleable::LayerDrawable);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::LayerDrawable, size);

    AutoPtr<ITypedArray> a;
    r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);

    a->GetInt32(R::styleable::LayerDrawable_opacity,
            IPixelFormat::UNKNOWN, &mOpacityOverride);

    a->Recycle();

    Int32 innerDepth;
    parser->GetDepth(&innerDepth);
    innerDepth++;
    Int32 depth;
    while ((parser->Next(&type), type) != IXmlPullParser::END_DOCUMENT
        && ((parser->GetDepth(&depth), depth) >= innerDepth
        || type != IXmlPullParser::END_TAG)) {
        if (type != IXmlPullParser::START_TAG) {
            continue;
        }

        String name;
        if (depth > innerDepth || (parser->GetName(&name), !name.Equals("item"))) {
            continue;
        }

        size = ARRAY_SIZE(R::styleable::LayerDrawableItem);
        layout = ArrayOf<Int32>::Alloc(size);
        layout->Copy(R::styleable::LayerDrawableItem, size);

        a = NULL;
        r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);

        Int32 left;
        a->GetDimensionPixelOffset(
            R::styleable::LayerDrawableItem_left, 0, &left);
        Int32 top;
        a->GetDimensionPixelOffset(
            R::styleable::LayerDrawableItem_top, 0, &top);
        Int32 right;
        a->GetDimensionPixelOffset(
            R::styleable::LayerDrawableItem_right, 0, &right);
        Int32 bottom;
        a->GetDimensionPixelOffset(
            R::styleable::LayerDrawableItem_bottom, 0, &bottom);
        Int32 drawableRes;
        a->GetResourceId(
            R::styleable::LayerDrawableItem_drawable, 0, &drawableRes);
        Int32 id;
        a->GetResourceId(
            R::styleable::LayerDrawableItem_id, IView::NO_ID, &id);

        a->Recycle();

        AutoPtr<IDrawable> dr;
        if (drawableRes != 0) {
            r->GetDrawable(drawableRes, (IDrawable**)&dr);
        }
        else {
            while ((parser->Next(&type), type) == IXmlPullParser::TEXT) {
            }
            if (type != IXmlPullParser::START_TAG) {
                //throw new XmlPullParserException(parser->getPositionDescription()
                //        + ": <item> tag requires a 'drawable' attribute or "
                //        + "child tag defining a drawable");
                return E_XML_PULL_PARSER_EXCEPTION;
            }
            Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&dr);
        }

        FAIL_RETURN(AddLayer(dr, id, left, top, right, bottom));
    }

    EnsurePadding();
    OnStateChange(GetState());
    return NOERROR;
}

ECode LayerDrawable::AddLayer(
    /* [in] */ IDrawable* layer,
    /* [in] */ Int32 id,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    LayerState* st = mLayerState;
    Int32 N = st->mChildren != NULL ? st->mChildren->GetLength() : 0;
    Int32 i = st->mNum;
    if (i >= N) {
        AutoPtr<ArrayOf<ChildDrawable*> > nu = ArrayOf<ChildDrawable*>::Alloc(N + 10);
        if (i > 0) {
            if (i >= N + 10) {
                return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
            }

            for (Int32 j = 0; j < i; j++) {
                nu->Set(j, (*st->mChildren)[j]);
            }
        }
        st->mChildren = nu;
    }

    Int32 config;
    layer->GetChangingConfigurations(&config);
    mLayerState->mChildrenChangingConfigurations |= config;

    AutoPtr<ChildDrawable> childDrawable = new ChildDrawable();
    st->mChildren->Set(i, childDrawable);
    childDrawable->mId = id;
    childDrawable->mDrawable = layer;
    childDrawable->mInsetL = left;
    childDrawable->mInsetT = top;
    childDrawable->mInsetR = right;
    childDrawable->mInsetB = bottom;
    st->mNum++;

    return layer->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
}

AutoPtr<IDrawable> LayerDrawable::FindDrawableByLayerId(
    /* [in] */ Int32 id)
{
    AutoPtr< ArrayOf<ChildDrawable*> > layers = mLayerState->mChildren;

    for (Int32 i = mLayerState->mNum - 1; i >= 0; i--) {
        if ((*layers)[i]->mId == id) {
            return (*layers)[i]->mDrawable;
        }
    }

    return NULL;
}

ECode LayerDrawable::SetId(
    /* [in] */ Int32 index,
    /* [in] */ Int32 id)
{
    (*mLayerState->mChildren)[index]->mId = id;
    return NOERROR;
}

Int32 LayerDrawable::GetNumberOfLayers()
{
    return mLayerState->mNum;
}

AutoPtr<IDrawable> LayerDrawable::GetDrawable(
    /* [in] */ Int32 index)
{
    return (*mLayerState->mChildren)[index]->mDrawable;
}

Int32 LayerDrawable::GetId(
    /* [in] */ Int32 index)
{
    return (*mLayerState->mChildren)[index]->mId;
}

Boolean LayerDrawable::SetDrawableByLayerId(
    /* [in] */ Int32 id,
    /* [in] */ IDrawable* drawable)
{
    AutoPtr< ArrayOf<ChildDrawable*> > layers = mLayerState->mChildren;

    for (Int32 i = mLayerState->mNum - 1; i >= 0; i--) {
        if ((*layers)[i]->mId == id) {
            if ((*layers)[i]->mDrawable != NULL) {
                if (drawable != NULL) {
                    AutoPtr<IRect> bounds;
                    (*layers)[i]->mDrawable->GetBounds((IRect**)&bounds);
                    drawable->SetBoundsEx(bounds);
                }
                (*layers)[i]->mDrawable->SetCallback(NULL);
            }
            if (drawable != NULL) {
                drawable->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
            }
            (*layers)[i]->mDrawable = drawable;
            return TRUE;
        }
    }

    return FALSE;
}

ECode LayerDrawable::SetLayerInset(
    /* [in] */ Int32 index,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    AutoPtr<ChildDrawable> childDrawable = (*mLayerState->mChildren)[index];
    childDrawable->mInsetL = l;
    childDrawable->mInsetT = t;
    childDrawable->mInsetR = r;
    childDrawable->mInsetB = b;
    return NOERROR;
}

ECode LayerDrawable::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->InvalidateDrawable((IDrawable*)this->Probe(EIID_IDrawable));
    }
    return NOERROR;
}

ECode LayerDrawable::ScheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what,
    /* [in] */ Int64 when)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->ScheduleDrawable(
            (IDrawable*)this->Probe(EIID_IDrawable), what, when);
    }
    return NOERROR;
}

ECode LayerDrawable::UnscheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->UnscheduleDrawable(
            (IDrawable*)this->Probe(EIID_IDrawable), what);
    }
    return NOERROR;
}

ECode LayerDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr< ArrayOf<ChildDrawable*> > array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    for (Int32 i = 0; i < N; i++) {
        (*array)[i]->mDrawable->Draw(canvas);
    }
    return NOERROR;
}

//@Override
Int32 LayerDrawable::GetChangingConfigurations()
{
    return Drawable::GetChangingConfigurations()
            | mLayerState->mChangingConfigurations
            | mLayerState->mChildrenChangingConfigurations;
}

//@Override
Boolean LayerDrawable::GetPadding(
    /* [in] */ IRect* padding)
{
    CRect* rect = (CRect*)padding;
    // Arbitrarily get the padding from the first image.
    // Technically we should maybe do something more intelligent,
    // like take the max padding of all the images.
    rect->mLeft = 0;
    rect->mTop = 0;
    rect->mRight = 0;
    rect->mBottom = 0;
    AutoPtr< ArrayOf<ChildDrawable*> > array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    for (Int32 i = 0; i < N; i++) {
        ReapplyPadding(i, (*array)[i]);
        rect->mLeft += (*mPaddingL)[i];
        rect->mTop += (*mPaddingT)[i];
        rect->mRight += (*mPaddingT)[i];
        rect->mBottom += (*mPaddingB)[i];
    }
    return TRUE;
}

//@Override
Boolean LayerDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart)
{
    Boolean changed = Drawable::SetVisible(visible, restart);
    AutoPtr< ArrayOf<ChildDrawable*> > array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    for (Int32 i = 0; i < N; i++) {
        Boolean res;
        (*array)[i]->mDrawable->SetVisible(visible, restart, &res);
    }
    return changed;
}

//@Override
ECode LayerDrawable::SetDither(
    /* [in] */ Boolean dither)
{
    AutoPtr< ArrayOf<ChildDrawable*> > array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    for (Int32 i = 0; i < N; i++) {
        (*array)[i]->mDrawable->SetDither(dither);
    }
    return NOERROR;
}

ECode LayerDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    AutoPtr< ArrayOf<ChildDrawable*> > array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    for (Int32 i = 0; i < N; i++) {
        (*array)[i]->mDrawable->SetAlpha(alpha);
    }
    return NOERROR;
}

ECode LayerDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    AutoPtr< ArrayOf<ChildDrawable*> > array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    for (Int32 i = 0; i < N; i++) {
        (*array)[i]->mDrawable->SetColorFilter(cf);
    }
    return NOERROR;
}

ECode LayerDrawable::SetOpacity(
    /* [in] */ Int32 opacity)
{
    mOpacityOverride = opacity;
    return NOERROR;
}

Int32 LayerDrawable::GetOpacity()
{
    if (mOpacityOverride != IPixelFormat::UNKNOWN) {
        return mOpacityOverride;
    }
    return mLayerState->GetOpacity();
}

Boolean LayerDrawable::IsStateful()
{
    return mLayerState->IsStateful();
}

Boolean LayerDrawable::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* state)
{
    AutoPtr< ArrayOf<ChildDrawable*> > array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    Boolean paddingChanged = FALSE;
    Boolean changed = FALSE;
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<ChildDrawable> r = (*array)[i];
        Boolean res;
        r->mDrawable->SetState(const_cast<ArrayOf<Int32>*>(state), &res);
        if (res) {
            changed = TRUE;
        }
        if (ReapplyPadding(i, r)) {
            paddingChanged = TRUE;
        }
    }
    if (paddingChanged) {
        OnBoundsChange(GetBounds());
    }
    return changed;
}

//@Override
Boolean LayerDrawable::OnLevelChange(
    /* [in] */ Int32 level)
{
    AutoPtr< ArrayOf<ChildDrawable*> > array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    Boolean paddingChanged = FALSE;
    Boolean changed = FALSE;
    for (Int32 i = 0; i < N; i++) {
        Boolean res;
        (*array)[i]->mDrawable->SetLevel(level, &res);
        if (res) {
            changed = TRUE;
        }
        if (ReapplyPadding(i, (*array)[i])) {
            paddingChanged = TRUE;
        }
    }
    if (paddingChanged) {
        OnBoundsChange(GetBounds());
    }
    return changed;
}

//@Override
void LayerDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    CRect* rect = (CRect*)bounds;
    AutoPtr< ArrayOf<ChildDrawable*> > array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    Int32 padL = 0, padT = 0, padR = 0, padB = 0;
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<ChildDrawable> r = (*array)[i];
        r->mDrawable->SetBounds(
            rect->mLeft + r->mInsetL + padL,
            rect->mTop + r->mInsetT + padT,
            rect->mRight - r->mInsetR - padR,
            rect->mBottom - r->mInsetB - padB);
        padL += (*mPaddingL)[i];
        padR += (*mPaddingT)[i];
        padT += (*mPaddingT)[i];
        padB += (*mPaddingB)[i];
    }
}

//@Override
Int32 LayerDrawable::GetIntrinsicWidth()
{
    Int32 width = -1;
    AutoPtr< ArrayOf<ChildDrawable*> > array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    Int32 padL = 0, padR = 0;
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<ChildDrawable> r = (*array)[i];
        Int32 w;
        r->mDrawable->GetIntrinsicWidth(&w);
        w += r->mInsetL + r->mInsetR + padL + padR;
        if (w > width) {
            width = w;
        }
        padL += (*mPaddingL)[i];
        padR += (*mPaddingT)[i];
    }
    return width;
}

//@Override
Int32 LayerDrawable::GetIntrinsicHeight()
{
    Int32 height = -1;
    AutoPtr< ArrayOf<ChildDrawable*> > array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    Int32 padT = 0, padB = 0;
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<ChildDrawable> r = (*array)[i];
        Int32 h;
        r->mDrawable->GetIntrinsicHeight(&h);
        h += r->mInsetT + r->mInsetB + + padT + padB;
        if (h > height) {
            height = h;
        }
        padT += (*mPaddingT)[i];
        padB += (*mPaddingB)[i];
    }
    return height;
}

Boolean LayerDrawable::ReapplyPadding(
    /* [in] */ Int32 i,
    /* [in] */ ChildDrawable* r)
{
    CRect* rect = (CRect*)mTmpRect.Get();
    Boolean res;
    r->mDrawable->GetPadding(rect, &res);
    if (rect->mLeft != (*mPaddingL)[i] || rect->mTop != (*mPaddingT)[i] ||
        rect->mRight != (*mPaddingT)[i] || rect->mBottom != (*mPaddingB)[i]) {
        (*mPaddingL)[i] = rect->mLeft;
        (*mPaddingT)[i] = rect->mTop;
        (*mPaddingT)[i] = rect->mRight;
        (*mPaddingB)[i] = rect->mBottom;
        return TRUE;
    }
    return FALSE;
}

void LayerDrawable::EnsurePadding()
{
    Int32 N = mLayerState->mNum;
    if (mPaddingL != NULL && mPaddingL->GetLength() >= N) {
        return;
    }
    mPaddingL = ArrayOf<Int32>::Alloc(N);
    mPaddingT = ArrayOf<Int32>::Alloc(N);
    mPaddingR = ArrayOf<Int32>::Alloc(N);
    mPaddingB = ArrayOf<Int32>::Alloc(N);
}

//@Override
AutoPtr<IDrawableConstantState> LayerDrawable::GetConstantState()
{
    if (mLayerState->CanConstantState()) {
        mLayerState->mChangingConfigurations = GetChangingConfigurations();
        return mLayerState;
    }
    return NULL;
}

AutoPtr<IDrawable> LayerDrawable::Mutate()
{
    if (!mMutated && (Drawable::Mutate().Get()
        == (IDrawable*)this->Probe(EIID_IDrawable))) {

        mLayerState = new LayerState(mLayerState, this, NULL);
        AutoPtr< ArrayOf<ChildDrawable*> > array = mLayerState->mChildren;
        Int32 N = mLayerState->mNum;
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IDrawable> drawable;
            (*array)[i]->mDrawable->Mutate((IDrawable**)&drawable);
        }
        mMutated = TRUE;
    }
    return (IDrawable*)this->Probe(EIID_IDrawable);
}

ECode LayerDrawable::SetLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    if (GetLayoutDirection() != layoutDirection) {
        AutoPtr< ArrayOf<ChildDrawable*> > array = mLayerState->mChildren;
        const Int32 N = mLayerState->mNum;
        for (Int32 i = 0; i < N; i++) {
            (*array)[i]->mDrawable->SetLayoutDirection(layoutDirection);
        }
    }
    return Drawable::SetLayoutDirection(layoutDirection);
}

ECode LayerDrawable::Init(
    /* [in] */ ArrayOf<IDrawable*>* layers,
    /* [in] */ LayerState* state)
{
    Init(state, NULL);
    Int32 length = layers->GetLength();
    AutoPtr< ArrayOf<ChildDrawable*> > r = ArrayOf<ChildDrawable*>::Alloc(length);

    for (Int32 i = 0; i < length; i++) {
        AutoPtr<ChildDrawable> cdTemp = new ChildDrawable();
        r->Set(i, cdTemp);
        (*r)[i]->mDrawable = (*layers)[i];
        (*layers)[i]->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
        Int32 config;
        (*layers)[i]->GetChangingConfigurations(&config);
        mLayerState->mChildrenChangingConfigurations |= config;
    }
    mLayerState->mNum = length;
    mLayerState->mChildren = r;

    EnsurePadding();
    return NOERROR;
}

ECode LayerDrawable::Init(
    /* [in] */ LayerState* state,
    /* [in] */ IResources* res)
{
    AutoPtr<LayerState> as = CreateConstantState(state, res);
    mLayerState = as;
    if (as->mNum > 0) {
        EnsurePadding();
    }
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
