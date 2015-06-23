
#include "graphics/drawable/MipmapDrawable.h"
#include "graphics/CRect.h"

MipmapDrawable::MipmapDrawable()
    : mMutated(FALSE)
{
    Init(NULL, NULL);
}

ECode MipmapDrawable::AddDrawable(
    /* [in] */ IDrawable* drawable)
{
    if (drawable != null) {
        mMipmapContainerState->AddDrawable(drawable);
        OnDrawableAdded();
    }
}

void MipmapDrawable::OnDrawableAdded() {
    // selectDrawable assumes that the container content does not change.
    // When a Drawable is added, the same index can correspond to a new Drawable, and since
    // selectDrawable has a fast exit case when oldIndex==newIndex, the new drawable could end
    // up not being used in place of the previous one if they happen to share the same index.
    // This make sure the new computed index can actually replace the previous one.
    SelectDrawable(-1);
    OnBoundsChange(GetBounds());
}

void MipmapDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    const Int32 index = mMipmapContainerState->IndexForBounds(bounds);

    // Will call invalidateSelf() if needed
    SelectDrawable(index);

    Drawable::OnBoundsChange(bounds);
}

ECode MipmapDrawable::Inflate(
    /* [in] */ IResources* r,
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IAttributeSet* attrs)
{
    Drawable::Inflate(r, parser, attrs);

    Int32 type;

    Int32 innerDepth = 0;
    parser->GetDepth(&innerDepth);
    innerDepth ++;

    Int32 depth;
    while ((parser->Next(&type), type) != END_DOCUMENT
            && ((parser->GetDepth(&depth), depth) >= innerDepth
                    || type != END_TAG)) {
        if (type != START_TAG) {
            continue;
        }

        String name;
        parser->GetName(&name);
        if (depth > innerDepth || name.Equals("item") != 0) {
            continue;
        }

        Int32 size = ARRAY_SIZE(R::styleable::MipmapDrawableItem);
        AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
        layout->Copy(R::styleable::MipmapDrawableItem, size);

        AutoPtr<ITypedArray> a;
        r->ObtainAttributes(attrs, layout, (ITypedArray**)&a);

        Int32 drawableRes = 0;
        a->GetResourceId(
                R::styleable::MipmapDrawableItem_drawable, 0, &drawableRes);

        a->Recycle();

        AutoPtr<IDrawable> dr;
        if (drawableRes != 0) {
            r->GetDrawable(drawableRes, (IDrawable**)&dr);
        } else {
            while ((parser->Next(&type), type) == TEXT) {
            }
            if (type != START_TAG) {
                assert(0);
                // throw new XmlPullParserException(
                //         parser.getPositionDescription()
                //         + ": <item> tag requires a 'drawable' attribute or "
                //         + "child tag defining a drawable");
            }

            Drawable::CreateFromXmlInner(r, parser, attrs, (IDrawable**)&dr);
        }

        mMipmapContainerState->AddDrawable(dr);
    }

    OnDrawableAdded();

    return NOERROR;
}

AutoPtr<IDrawable> MipmapDrawable::Mutate()
{
    if (!mMutated && Drawable::Mutate() == this) {
        mMipmapContainerState->mMipmapHeights = mMipmapContainerState->mMipmapHeights->Clone();
        mMutated = true;
    }
    return this;
}

MipmapDrawable::MipmapContainerState::MipmapContainerState(
    /* [in] */ MipmapContainerState* orig,
    /* [in] */ MipmapDrawable* owner,
    /* [in] */ IResources* res)
    : DrawableContainerState(orig, owner, res)
{
    if (orig != NULL) {
        mMipmapHeights = orig->mMipmapHeights;
    } else {
        mMipmapHeights = ArrayOf<Int32>::Alloc(GetChildren()->GetLength());
    }

    // Change the default value
    SetConstantSize(TRUE);
}

Int32 MipmapDrawable::MipmapContainerState::IndexForBounds(
    /* [in] */ IRect* bounds)
{
    Int32 boundsHeight = 0;
    bounds->GetHeight(&boundsHeight);
    const Int32 N = GetChildCount();
    for (Int32 i = 0; i < N; i++) {
        if (boundsHeight <= (*mMipmapHeights)[i]) {
            return i;
        }
    }

    // No mipmap larger than bounds found. Use largest one which will be scaled up.
    if (N > 0) {
        return N - 1;
    }
    // No Drawable mipmap at all
    return -1;
}

void MipmapDrawable::MipmapContainerState::AddDrawable(
    /* [in] */ IDrawable* drawable)
{
    // Insert drawable in last position, correctly resetting cached values and
    // especially mComputedConstantSize
    Int32 pos = AddChild(drawable);

    // Bubble sort the last drawable to restore the sort by intrinsic height
    Int32 drawableHeight = 0;
    drawable->GetIntrinsicHeight(&drawableHeight);

    while (pos > 0) {
        AutoPtr<IDrawable> previousDrawable = (*mDrawables)[pos-1];
        Int32 previousIntrinsicHeight = 0;
        previousDrawable->GetIntrinsicHeight(&previousIntrinsicHeight);

        if (drawableHeight < previousIntrinsicHeight) {
            (*mDrawables)[pos] = previousDrawable;
            (*mMipmapHeights)[pos] = previousIntrinsicHeight;

            (*mDrawables)[pos-1] = drawable;
            (*mMipmapHeights)[pos-1] = drawableHeight;
            pos--;
        } else {
            break;
        }
    }
}

void MipmapDrawable::MipmapContainerState::ComputeConstantSize()
{
    const Int32 N = GetChildCount();
    if (N > 0) {
        AutoPtr<IDrawable> smallestDrawable = (*mDrawables)[0];
        smallestDrawable->GetMinimumWidth(&mConstantMinimumWidth);
        smallestDrawable->GetMinimumHeight(&mConstantMinimumHeight);

        AutoPtr<IDrawable> largestDrawable = (*mDrawables)[N-1];
        largestDrawable->GetIntrinsicWidth(&mConstantWidth);
        largestDrawable->GetIntrinsicHeight(&mConstantHeight);
    } else {
        mConstantWidth = mConstantHeight = -1;
        mConstantMinimumWidth = mConstantMinimumHeight = 0;
    }
    mComputedConstantSize = TRUE;
}

ECode MipmapDrawable::MipmapContainerState::NewDrawable(
    /* [out] */ IDrawable** drawable)
{
    return CMipmapDrawable::New(this, NULL, drawable);
}

ECode MipmapDrawable::MipmapContainerState::NewDrawable(
    /* [in] */ IResources* res,
    /* [out] */ IDrawable** drawable)
{
    return CMipmapDrawable::New(this, res, drawable);
}

void MipmapDrawable::MipmapContainerState::GrowArray(
    /* [in] */ Int32 oldSize,
    /* [in] */ Int32 newSize)
{
    DrawableContainerState::GrowArray(oldSize, newSize);
    AutoPtr<ArrayOf<Int32> > newInts = ArrayOf<Int32>::Alloc(newSize);
    newInts->Copy(0, mMipmapHeights, 0, oldSize);
    mMipmapHeights = newInts;
}

ECode MipmapDrawable::Init(
    /* [in] */ MipmapContainerState* state,
    /* [in] */ IResources* res)
{
    mMipmapContainerState = new MipmapContainerState(state, this, res);
    SetConstantState(mMipmapContainerState);
    OnDrawableAdded();
}


