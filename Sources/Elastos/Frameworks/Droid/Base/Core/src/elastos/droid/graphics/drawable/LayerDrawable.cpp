
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/graphics/drawable/LayerDrawable.h"
#include "elastos/droid/graphics/drawable/CLayerDrawable.h"
#include "elastos/droid/graphics/CRect.h"
#include "elastos/droid/R.h"

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
                state->NewDrawable(res, (IDrawable**)(&(r->mDrawable)));
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

ECode LayerDrawable::LayerState::NewDrawable(
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
        Drawable::ResolveOpacity(op, op2, &op);
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

CAR_INTERFACE_IMPL_2(LayerDrawable, Drawable, ILayerDrawable, IDrawableCallback);
LayerDrawable::LayerDrawable()
    : mOpacityOverride(IPixelFormat::UNKNOWN)
    , mMutated(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mTmpRect));
    constructor((LayerState*)NULL, NULL);
}

LayerDrawable::LayerDrawable(
    /* [in] */ ArrayOf<IDrawable*>* layers,
    /* [in] */ LayerState* state)
    : mOpacityOverride(IPixelFormat::UNKNOWN)
    , mMutated(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mTmpRect));
    constructor(layers, state);
}

LayerDrawable::LayerDrawable(
    /* [in] */ LayerState* state,
    /* [in] */ IResources* res)
    : mOpacityOverride(IPixelFormat::UNKNOWN)
    , mMutated(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mTmpRect));
    constructor(state, res);
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
    AutoPtr<ArrayOf<Int32> > states;
    GetState((ArrayOf<Int32>**)&states);
    OnStateChange(states);
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

ECode LayerDrawable::FindDrawableByLayerId(
    /* [in] */ Int32 id,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr< ArrayOf<ChildDrawable*> > layers = mLayerState->mChildren;

    for (Int32 i = mLayerState->mNum - 1; i >= 0; i--) {
        if ((*layers)[i]->mId == id) {
            *drawable = (*layers)[i]->mDrawable;
            REFCOUNT_ADD(*drawable);
        }
    }

    *drawable = NULL;
    return NOERROR;
}

ECode LayerDrawable::SetId(
    /* [in] */ Int32 index,
    /* [in] */ Int32 id)
{
    (*mLayerState->mChildren)[index]->mId = id;
    return NOERROR;
}

ECode LayerDrawable::GetNumberOfLayers(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = mLayerState->mNum;
    return NOERROR;
}

ECode LayerDrawable::GetDrawable(
    /* [in] */ Int32 index,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    *drawable = (*mLayerState->mChildren)[index]->mDrawable;
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode LayerDrawable::GetId(
    /* [in] */ Int32 index,
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = (*mLayerState->mChildren)[index]->mId;
    return NOERROR;
}

ECode LayerDrawable::SetDrawableByLayerId(
    /* [in] */ Int32 id,
    /* [in] */ IDrawable* drawable,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr< ArrayOf<ChildDrawable*> > layers = mLayerState->mChildren;

    for (Int32 i = mLayerState->mNum - 1; i >= 0; i--) {
        if ((*layers)[i]->mId == id) {
            if ((*layers)[i]->mDrawable != NULL) {
                if (drawable != NULL) {
                    AutoPtr<IRect> bounds;
                    (*layers)[i]->mDrawable->GetBounds((IRect**)&bounds);
                    drawable->SetBounds(bounds);
                }
                (*layers)[i]->mDrawable->SetCallback(NULL);
            }
            if (drawable != NULL) {
                drawable->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
            }
            (*layers)[i]->mDrawable = drawable;
            *res = TRUE;
            return NOERROR;
        }
    }

    *res = FALSE;
    return NOERROR;
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

ECode LayerDrawable::SetPaddingMode(
    /* [in] */ Int32 mode)
{
    assert(0 && "TODO");
    // if (mLayerState->mPaddingMode != mode) {
    //     mLayerState->mPaddingMode = mode;
    // }
    return NOERROR;
}

ECode LayerDrawable::GetPaddingMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    assert(0 && "TODO");
    // *mode = mLayerState->mPaddingMode;
    return NOERROR;
}

ECode LayerDrawable::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    AutoPtr<IDrawableCallback> callback;
    GetCallback((IDrawableCallback**)&callback);
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
    AutoPtr<IDrawableCallback> callback;
    GetCallback((IDrawableCallback**)&callback);
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
    AutoPtr<IDrawableCallback> callback;
    GetCallback((IDrawableCallback**)&callback);
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

ECode LayerDrawable::GetChangingConfigurations(
    /* [out] */ Int32* configs)
{
    VALIDATE_NOT_NULL(configs);
    Drawable::GetChangingConfigurations(configs);
    *configs = (*configs)
            | mLayerState->mChangingConfigurations
            | mLayerState->mChildrenChangingConfigurations;
    return NOERROR;
}

ECode LayerDrawable::GetPadding(
    /* [in] */ IRect* padding,
    /* [out] */ Boolean* isPadding)
{
    VALIDATE_NOT_NULL(isPadding);
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
    *isPadding = TRUE;
    return NOERROR;
}

ECode LayerDrawable::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart,
    /* [out] */ Boolean* isDifferent)
{
    VALIDATE_NOT_NULL(isDifferent);
    Boolean changed = FALSE;
    Drawable::SetVisible(visible, restart, &changed);
    AutoPtr< ArrayOf<ChildDrawable*> > array = mLayerState->mChildren;
    Int32 N = mLayerState->mNum;
    for (Int32 i = 0; i < N; i++) {
        Boolean res;
        (*array)[i]->mDrawable->SetVisible(visible, restart, &res);
    }
    *isDifferent = changed;
    return NOERROR;
}

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

ECode LayerDrawable::GetOpacity(
    /* [out] */ Int32* opacity)
{
    VALIDATE_NOT_NULL(opacity);
    if (mOpacityOverride != IPixelFormat::UNKNOWN) {
        *opacity = mOpacityOverride;
        return NOERROR;
    }
    *opacity = mLayerState->GetOpacity();
    return NOERROR;
}

ECode LayerDrawable::IsStateful(
    /* [out] */ Boolean* isStateful)
{
    VALIDATE_NOT_NULL(isStateful);
    *isStateful = mLayerState->IsStateful();
    return NOERROR;
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
        AutoPtr<IRect> rect;
        GetBounds((IRect**)&rect);
        OnBoundsChange(rect);
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
        AutoPtr<IRect> rect;
        GetBounds((IRect**)&rect);
        OnBoundsChange(rect);
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

Int32 LayerDrawable::GetIntrinsicWidth(
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(res);
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
    *res = width;
    return NOERROR;
}

Int32 LayerDrawable::GetIntrinsicHeight(
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(res);
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
    *res = height;
    return NOERROR;
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

ECode LayerDrawable::GetConstantState(
    /* [out] */ IDrawableConstantState** state)
{
    VALIDATE_NOT_NULL(state);
    if (mLayerState->CanConstantState()) {
        GetChangingConfigurations(&mLayerState->mChangingConfigurations);
        *state = mLayerState;
        REFCOUNT_ADD(*state);
        return NOERROR;
    }
    *state = NULL;
    return NOERROR;
}

ECode LayerDrawable::Mutate(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> tmp;
    if (!mMutated && ((Drawable::Mutate((IDrawable**)&tmp), tmp.Get())
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
    *drawable = (IDrawable*)this->Probe(EIID_IDrawable);
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode LayerDrawable::SetLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    Int32 dir = 0;
    if ((GetLayoutDirection(&dir), dir) != layoutDirection) {
        AutoPtr< ArrayOf<ChildDrawable*> > array = mLayerState->mChildren;
        const Int32 N = mLayerState->mNum;
        for (Int32 i = 0; i < N; i++) {
            (*array)[i]->mDrawable->SetLayoutDirection(layoutDirection);
        }
    }
    return Drawable::SetLayoutDirection(layoutDirection);
}

ECode LayerDrawable::constructor(
    /* [in] */ ArrayOf<IDrawable*>* layers,
    /* [in] */ LayerState* state)
{
    constructor(state, NULL);
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

ECode LayerDrawable::constructor(
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
