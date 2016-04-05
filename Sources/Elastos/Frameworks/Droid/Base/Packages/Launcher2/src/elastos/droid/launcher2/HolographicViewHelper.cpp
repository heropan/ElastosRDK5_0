
#include "elastos/droid/launcher2/HolographicViewHelper.h"
#include "elastos/droid/launcher2/FastBitmapDrawable.h"
#include "Elastos.Droid.Service.h"
#include "R.h"
#include "elastos/droid/R.h"
#include "Elastos.Droid.View.h"

using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Graphics::CCanvas;
using Elastos::Droid::Graphics::Drawable::CStateListDrawable;
using Elastos::Droid::Graphics::Drawable::IStateListDrawable;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::View::IView;
using Elastos::Droid::Graphics::BitmapConfig_ARGB_8888;
using Elastos::Droid::Graphics::CBitmapHelper;
using Elastos::Droid::Graphics::IBitmapHelper;
using Elastos::Droid::Graphics::PorterDuffMode_SRC_IN;

namespace Elastos {
namespace Droid {
namespace Launcher2 {

HolographicViewHelper::HolographicViewHelper(
    /* [in] */ IContext* context)
    : mStatesUpdated(FALSE)
    , mHighlightColor(0)
{
    CCanvas::New((ICanvas**)&mTempCanvas);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    res->GetColor(Elastos::Droid::R::color::white, &mHighlightColor);
}

void HolographicViewHelper::GeneratePressedFocusedStates(
    /* [in] */ IImageView* v)
{
    if (!mStatesUpdated && v != NULL) {
        mStatesUpdated = TRUE;
        AutoPtr<IBitmap> original = CreateOriginalImage(v, mTempCanvas);
        AutoPtr<IBitmap> outline = CreatePressImage(v, mTempCanvas);
        AutoPtr<FastBitmapDrawable> originalD = new FastBitmapDrawable();
        originalD->constructor(original);
        AutoPtr<FastBitmapDrawable> outlineD = new FastBitmapDrawable();
        outlineD->constructor(outline);

        AutoPtr<IStateListDrawable> states;
        CStateListDrawable::New((IStateListDrawable**)&states);
        AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(1);
        (*array)[0] = Elastos::Droid::R::attr::state_pressed;
        states->AddState(array, outlineD);
        AutoPtr<ArrayOf<Int32> > array2 = ArrayOf<Int32>::Alloc(1);
        (*array2)[0] = Elastos::Droid::R::attr::state_focused;
        states->AddState(array2, outlineD);
        AutoPtr<ArrayOf<Int32> > array3 = ArrayOf<Int32>::Alloc(0);
        states->AddState(array3, originalD);

        v->SetImageDrawable(IDrawable::Probe(states));
    }
}

void HolographicViewHelper::InvalidatePressedFocusedStates(
    /* [in] */ IImageView* v)
{
    mStatesUpdated = FALSE;
    if (v != NULL) {
        IView::Probe(v)->Invalidate();
    }
}

AutoPtr<IBitmap> HolographicViewHelper::CreateOriginalImage(
    /* [in] */ IImageView* v,
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IDrawable> d;
    v->GetDrawable((IDrawable**)&d);

    AutoPtr<IBitmap> b;
    Int32 width;
    d->GetIntrinsicWidth(&width);
    Int32 height;
    d->GetIntrinsicHeight(&height);

    AutoPtr<IBitmapHelper> helper;
    CBitmapHelper::AcquireSingleton((IBitmapHelper**)&helper);
    helper->CreateBitmap(width, height, BitmapConfig_ARGB_8888, (IBitmap**)&b);

    canvas->SetBitmap(b);
    Int32 tmp;
    canvas->Save(&tmp);
    d->Draw(canvas);
    canvas->Restore();
    canvas->SetBitmap(NULL);
    return b;
}

AutoPtr<IBitmap> HolographicViewHelper::CreatePressImage(
    /* [in] */ IImageView* v,
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IDrawable> d;
    v->GetDrawable((IDrawable**)&d);

    AutoPtr<IBitmap> b;
    Int32 width;
    d->GetIntrinsicWidth(&width);
    Int32 height;
    d->GetIntrinsicHeight(&height);
    AutoPtr<IBitmapHelper> helper;
    CBitmapHelper::AcquireSingleton((IBitmapHelper**)&helper);
    helper->CreateBitmap(width, height, BitmapConfig_ARGB_8888, (IBitmap**)&b);

    canvas->SetBitmap(b);
    Int32 tmp;
    canvas->Save(&tmp);
    d->Draw(canvas);
    canvas->Restore();
    canvas->DrawColor(mHighlightColor, PorterDuffMode_SRC_IN);
    canvas->SetBitmap(NULL);

    return b;
}

} // namespace Launcher2
} // namespace Droid
} // namespace Elastos