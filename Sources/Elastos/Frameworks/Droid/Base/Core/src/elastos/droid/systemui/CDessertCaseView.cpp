
#include "elastos/droid/systemui/CDessertCaseView.h"
//#include "elastos/droid/utility/SparseArray.h"
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/animation/CAnimatorSet.h"
#include "elastos/droid/animation/ObjectAnimator.h"
#include "elastos/droid/graphics/CBitmapFactoryOptions.h"
#include "elastos/droid/graphics/CBitmapFactory.h"
#include "elastos/droid/graphics/CColorMatrixColorFilter.h"
#include "elastos/droid/graphics/CCanvas.h"
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/graphics/CPoint.h"
#include "elastos/droid/graphics/CRect.h"
#include "elastos/droid/graphics/drawable/BitmapDrawable.h"
//#include "elastos/droid/webkit/ui/gfx/BitmapHelper.h"
//#include "elastos/droid/widget/CFrameLayoutLayoutParams.h"

#include <elastos/core/Math.h>

using Elastos::Droid::Animation::ITimeInterpolator;
using Elastos::Droid::Animation::IAnimatorSet;
using Elastos::Droid::Animation::CAnimatorSet;
using Elastos::Droid::Animation::ObjectAnimator;
using Elastos::Droid::Animation::EIID_IAnimatorListener;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Graphics::BitmapConfig_ALPHA_8;
using Elastos::Droid::Graphics::IBitmapFactoryOptions;
using Elastos::Droid::Graphics::CBitmapFactoryOptions;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Graphics::IColorMatrixColorFilter;
using Elastos::Droid::Graphics::CColorMatrixColorFilter;
using Elastos::Droid::Graphics::IColorFilter;
using Elastos::Droid::Graphics::CCanvas;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Graphics::CPoint;
using Elastos::Droid::Graphics::PaintStyle_STROKE;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::Drawable::IBitmapDrawable;
using Elastos::Droid::Graphics::Drawable::BitmapDrawable;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Widget::IFrameLayoutLayoutParams;
//using Elastos::Droid::Webkit::Ui::Gfx::BitmapHelper;
//using Elastos::Droid::Utility::SparseArray;
using Elastos::Droid::View::IViewOverlay;
using Elastos::Droid::View::IViewPropertyAnimator;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::Animation::IAccelerateInterpolator;
using Elastos::Droid::View::Animation::IAnticipateOvershootInterpolator;
using Elastos::Droid::View::Animation::IDecelerateInterpolator;
//using Elastos::Droid::View::Animation::CAccelerateInterpolator;

using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Utility::CHashSet;
using Elastos::Utility::IIterator;

namespace Elastos {
namespace Droid {
namespace SystemUI {

//===============================================================
// CDessertCaseView::
//===============================================================
String CDessertCaseView::TAG;// = DessertCaseView.class.getSimpleName();

Boolean CDessertCaseView::DEBUG = FALSE;

Int32 CDessertCaseView::START_DELAY = 5000;
Int32 CDessertCaseView::DELAY = 2000;
Int32 CDessertCaseView::DURATION = 500;

Int32 CDessertCaseView::TAG_POS = 0x2000001;
Int32 CDessertCaseView::TAG_SPAN = 0x2000002;

AutoPtr<ArrayOf<Int32> > CDessertCaseView::PASTRIES;// = {
//         R.drawable.dessert_kitkat,      // used with permission
//         R.drawable.dessert_android,     // thx irina
// };

AutoPtr<ArrayOf<Int32> > CDessertCaseView::RARE_PASTRIES;// = {
//         R.drawable.dessert_cupcake,     // 2009
//         R.drawable.dessert_donut,       // 2009
//         R.drawable.dessert_eclair,      // 2009
//         R.drawable.dessert_froyo,       // 2010
//         R.drawable.dessert_gingerbread, // 2010
//         R.drawable.dessert_honeycomb,   // 2011
//         R.drawable.dessert_ics,         // 2011
//         R.drawable.dessert_jellybean,   // 2012
// };

AutoPtr<ArrayOf<Int32> > CDessertCaseView::XRARE_PASTRIES;// = {
//         R.drawable.dessert_petitfour,   // the original and still delicious

//         R.drawable.dessert_donutburger, // remember kids, this was long before cronuts

//         R.drawable.dessert_flan,        //     sholes final approach
//                                         //     landing gear punted to flan
//                                         //     runway foam glistens
//                                         //         -- mcleron

//         R.drawable.dessert_keylimepie,  // from an alternative timeline
// };

AutoPtr<ArrayOf<Int32> > CDessertCaseView::XXRARE_PASTRIES;// = {
//         R.drawable.dessert_zombiegingerbread, // thx hackbod
//         R.drawable.dessert_dandroid,    // thx morrildl
//         R.drawable.dessert_jandycane,   // thx nes
// };

Int32 CDessertCaseView::NUM_PASTRIES = PASTRIES->GetLength() + RARE_PASTRIES->GetLength()
        + XRARE_PASTRIES->GetLength() + XXRARE_PASTRIES->GetLength();

AutoPtr<ArrayOf<Float> > CDessertCaseView::MASK;

AutoPtr<ArrayOf<Float> > CDessertCaseView::ALPHA_MASK;

AutoPtr<ArrayOf<Float> > CDessertCaseView::WHITE_MASK;

Float CDessertCaseView::SCALE = 0.25f; // natural display size will be SCALE*mCellSize

Float CDessertCaseView::PROB_2X = 0.33f;
Float CDessertCaseView::PROB_3X = 0.1f;
Float CDessertCaseView::PROB_4X = 0.01f;

//CAR_INTERFACE_IMPL(CDessertCaseView, FrameLayout, IDessertCaseView)
CAR_INTERFACE_IMPL(CDessertCaseView, Object, IDessertCaseView)

CDessertCaseView::CDessertCaseView()
{
//    mDrawables = new SparseArray(NUM_PASTRIES);

    CHashSet::New((IHashSet**)&mFreeList);

    mHandler = new Handler();

    mJuggle = new Runnable_1(this);

    mHsv = ArrayOf<Float>::Alloc(3);
    (*mHsv)[0] = 0.f;
    (*mHsv)[1] = 1.f;
    (*mHsv)[2] = .85f;

    CHashSet::New((IHashSet**)&mTmpSet);
    assert(0 && "TODO");

    // AutoPtr<ArrayOf<Float> > CDessertCaseView::MASK = {
    //         0.f,  0.f,  0.f,  0.f, 255.f,
    //         0.f,  0.f,  0.f,  0.f, 255.f,
    //         0.f,  0.f,  0.f,  0.f, 255.f,
    //         1.f,  0.f,  0.f,  0.f, 0.f
    // };

    // AutoPtr<ArrayOf<Float> > CDessertCaseView::ALPHA_MASK = {
    //         0.f,  0.f,  0.f,  0.f, 255.f,
    //         0.f,  0.f,  0.f,  0.f, 255.f,
    //         0.f,  0.f,  0.f,  0.f, 255.f,
    //         0.f,  0.f,  0.f,  1.f, 0.f
    // };

    // AutoPtr<ArrayOf<Float> > CDessertCaseView::WHITE_MASK = {
    //         0.f,  0.f,  0.f,  0.f, 255.f,
    //         0.f,  0.f,  0.f,  0.f, 255.f,
    //         0.f,  0.f,  0.f,  0.f, 255.f,
    //         -1.f,  0.f,  0.f,  0.f, 255.f
    // };
}

ECode CDessertCaseView::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode CDessertCaseView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, 0);
}

ECode CDessertCaseView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    assert(0 && "TODO");
//    FrameLayout(context, attrs, defStyle);

    AutoPtr<IResources> res;
//    View::GetResources((IResources**)&res);

    mStarted = FALSE;

//    res->GetDimensionPixelSize(R::dimen::dessert_case_cell_size, &mCellSize);
    AutoPtr<IBitmapFactoryOptions> opts;
    CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&opts);
    AutoPtr<CBitmapFactoryOptions> copts = (CBitmapFactoryOptions*)opts.Get();
    if (mCellSize < 512) { // assuming 512x512 images
        copts->mInSampleSize = 2;
    }
    copts->mInMutable = TRUE;
    AutoPtr<IBitmap> loaded;
    // new Int32[][] { PASTRIES, RARE_PASTRIES, XRARE_PASTRIES, XXRARE_PASTRIES };
    // for () {
        AutoPtr<ArrayOf<Int32> > list;
        for (Int32 i = 0; i < list->GetLength(); i++) {
            Int32 resid = (*list)[i];
            copts->mInBitmap = loaded;
            AutoPtr<IBitmapFactory> bmpf;
            CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bmpf);
            bmpf->DecodeResource(res, resid, opts, (IBitmap**)&loaded);
            AutoPtr<IBitmapDrawable> d = new BitmapDrawable(res, ConvertToAlphaMask(loaded));
            AutoPtr<IColorMatrixColorFilter> cmcf;
            CColorMatrixColorFilter::New(*ALPHA_MASK, (IColorMatrixColorFilter**)&cmcf);
            IDrawable::Probe(d)->SetColorFilter(IColorFilter::Probe(cmcf));
            IDrawable::Probe(d)->SetBounds(0, 0, mCellSize, mCellSize);
            mDrawables->Append(resid, d);
        }
//     }
    loaded = NULL;
//    if (DEBUG) View::SetWillNotDraw(FALSE);
    return NOERROR;
}

AutoPtr<IBitmap> CDessertCaseView::ConvertToAlphaMask(
    /* [in] */ IBitmap* b)
{
    Int32 w = 0, h = 0;
    b->GetWidth(&w);
    b->GetHeight(&h);
    AutoPtr<IBitmap> a;// = BitmapHelper::CreateBitmap(w, h, BitmapConfig_ALPHA_8);
    AutoPtr<ICanvas> c;
    CCanvas::New(a, (ICanvas**)&c);
    AutoPtr<IPaint> pt;
    CPaint::New((IPaint**)&pt);
    AutoPtr<IColorMatrixColorFilter> p;
    CColorMatrixColorFilter::New(*MASK, (IColorMatrixColorFilter**)&p);
    pt->SetColorFilter(IColorFilter::Probe(p));
    c->DrawBitmap(b, 0.0f, 0.0f, pt);
    return a;
}

ECode CDessertCaseView::Start()
{
    if (!mStarted) {
        mStarted = TRUE;
        FillFreeList(DURATION * 4);
    }
    Boolean res = FALSE;
    mHandler->PostDelayed(mJuggle, START_DELAY, &res);
    return NOERROR;
}

ECode CDessertCaseView::Stop()
{
    mStarted = FALSE;
    mHandler->RemoveCallbacks(mJuggle);
    return NOERROR;
}

Int32 CDessertCaseView::Pick(
    /* [in] */ ArrayOf<Int32>* a)
{
    return (*a)[(Int32)(Elastos::Core::Math::Random()*a->GetLength())];
}

AutoPtr<IInterface> CDessertCaseView::Pick(
    /* [in] */ ArrayOf<IInterface*>* a)
{
    return (*a)[(Int32)(Elastos::Core::Math::Random()*a->GetLength())];
}

AutoPtr<IInterface> CDessertCaseView::Pick(
    /* [in] */ ISparseArray* sa)
{
    Int32 s = 0;
    sa->GetSize(&s);
    AutoPtr<IInterface> node;
    sa->ValueAt((Int32)(Elastos::Core::Math::Random()*s), (IInterface**)&node);
    return node;
}

Int32 CDessertCaseView::Random_color()
{
//        return 0xFF000000 | (int) (Math.random() * (Float) 0xFFFFFF); // totally random
    Int32 COLORS = 12;
    (*mHsv)[0] = Irand(0, COLORS) * (360.f/COLORS);
    assert(0 && "TODO");
//    return Color::HSVToColor(mHsv);
    return 0;
}

// synchronized
void CDessertCaseView::OnSizeChanged (
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    assert(0 && "TODO");
//    FrameLayout::OnSizeChanged(w, h, oldw, oldh);
    if (mWidth == w && mHeight == h) return;

    Boolean wasStarted = mStarted;
    if (wasStarted) {
        Stop();
    }

    mWidth = w;
    mHeight = h;

    mCells = NULL;
//    ViewGroup::RemoveAllViewsInLayout();
    mFreeList->Clear();

    mRows = mHeight / mCellSize;
    mColumns = mWidth / mCellSize;

    mCells = ArrayOf<IView*>::Alloc(mRows * mColumns);

//    if (DEBUG) Log.v(TAG, String.format("New dimensions: %dx%d", mColumns, mRows));

//    View::SetScaleX(SCALE);
//    View::SetScaleY(SCALE);
//    View::SetTranslationX(0.5f * (mWidth - mCellSize * mColumns) * SCALE);
//    View::SetTranslationY(0.5f * (mHeight - mCellSize * mRows) * SCALE);

    for (Int32 j = 0; j < mRows; j++) {
        for (Int32 i = 0; i < mColumns; i++) {
            AutoPtr<IPoint> pt;
            CPoint::New(i, j, (IPoint**)&pt);
            mFreeList->Add(pt);
        }
    }

    if (wasStarted) {
        Start();
    }
}

ECode CDessertCaseView::FillFreeList()
{
    return FillFreeList(DURATION);
}

//synchronized
ECode CDessertCaseView::FillFreeList(
    /* [in] */ Int32 animationLen)
{
    assert(0 && "TODO");
    AutoPtr<IContext> ctx;
//    View::GetContext((IContext**)&ctx);
    AutoPtr<IFrameLayoutLayoutParams> lp;
//    CFrameLayoutLayoutParams::New(mCellSize, mCellSize, (IFrameLayoutLayoutParams**)&lp);

    Boolean bEmp = FALSE;
    while (! (mFreeList->IsEmpty(&bEmp), bEmp)) {
        AutoPtr<IIterator> it;
        mFreeList->GetIterator((IIterator**)&it);
        AutoPtr<IInterface> nt;
        it->GetNext((IInterface**)&nt);
        AutoPtr<IPoint> pt = IPoint::Probe(nt);
        mFreeList->Remove(pt);
        AutoPtr<CPoint> cpt = (CPoint*)pt.Get();
        Int32 i = cpt->mX;
        Int32 j = cpt->mY;

        if ((*mCells)[j*mColumns+i] != NULL) continue;
        AutoPtr<IImageView> v;
        // CImageView::New(ctx, (IImageView**)&v);
        IView::Probe(v)->SetOnClickListener(new OnClickListener(this, v));

        Int32 c = Random_color();
        IView::Probe(v)->SetBackgroundColor(c);

        Float which = Frand();
        AutoPtr<IDrawable> d;
        AutoPtr<IInterface> pD;
        if (which < 0.0005f) {
            mDrawables->Get(Pick(XXRARE_PASTRIES), (IInterface**)&pD);
        }
        else if (which < 0.005f) {
            mDrawables->Get(Pick(XRARE_PASTRIES), (IInterface**)&pD);
        }
        else if (which < 0.5f) {
            mDrawables->Get(Pick(RARE_PASTRIES), (IInterface**)&pD);
        }
        else if (which < 0.7f) {
            mDrawables->Get(Pick(PASTRIES), (IInterface**)&pD);
        }
        else {
            pD = NULL;
        }

        if (pD != NULL) {
            d = IDrawable::Probe(pD);
            AutoPtr<IViewOverlay> ol;
            IView::Probe(v)->GetOverlay((IViewOverlay**)&ol);
            ol->Add(d);
        }

        // AutoPtr<CFrameLayoutLayoutParams> clp = (CFrameLayoutLayoutParams*)lp.Get();
        // clp->mWidth = clp->mHeight = mCellSize;
        // ViewGroup::AddView(v, lp);
        Place(IView::Probe(v), pt, FALSE);
        if (animationLen > 0) {
            Float s;// = (Integer) v->GetTag(TAG_SPAN);
            IView::Probe(v)->SetScaleX(0.5f * s);
            IView::Probe(v)->SetScaleY(0.5f * s);
            IView::Probe(v)->SetAlpha(0.f);
            AutoPtr<IViewPropertyAnimator> anim;
            IView::Probe(v)->Animate((IViewPropertyAnimator**)&anim);
            anim->WithLayer();
            anim->ScaleX(s);
            anim->ScaleY(s);
            anim->Alpha(1.f);
            anim->SetDuration(animationLen);
        }
    }
    return NOERROR;
}

ECode CDessertCaseView::Place(
    /* [in] */ IView* v,
    /* [in] */ Boolean animate)
{
    AutoPtr<IPoint> pt;
    CPoint::New(Irand(0, mColumns), Irand(0, mRows), (IPoint**)&pt);
    return Place(v, pt, animate);
}

// we don't have .withLayer() on general Animators
AutoPtr<IAnimatorListener> CDessertCaseView::MakeHardwareLayerListener(
    /* [in] */ IView* v)
{
    return new AnimatorListenerAdapter_1(v);
}

ECode CDessertCaseView::Place(
    /* [in] */ IView* v,
    /* [in] */ IPoint* pt,
    /* [in] */ Boolean animate)
{
    AutoPtr<CPoint> cpt = (CPoint*)pt;
    Int32 i = cpt->mX;
    Int32 j = cpt->mY;
    Float rnd = Frand();
    AutoPtr<IInterface> t;
    v->GetTag(TAG_POS, (IInterface**)&t);
    if (t != NULL) {
        AutoPtr<ArrayOf<IPoint*> > arr = GetOccupied(v);
        for (Int32 i = 0; i < arr->GetLength(); i++) {
            AutoPtr<IPoint> oc = (*arr)[i];
            mFreeList->Add(oc);
            AutoPtr<CPoint> coc = (CPoint*)oc.Get();
            (*mCells)[coc->mY*mColumns + coc->mX] = NULL;
        }
    }
    Int32 scale = 1;
    if (rnd < PROB_4X) {
        if (!(i >= mColumns-3 || j >= mRows-3)) {
            scale = 4;
        }
    }
    else if (rnd < PROB_3X) {
        if (!(i >= mColumns-2 || j >= mRows-2)) {
            scale = 3;
        }
    }
    else if (rnd < PROB_2X) {
        if (!(i == mColumns-1 || j == mRows-1)) {
            scale = 2;
        }
    }

    v->SetTag(TAG_POS, pt);
    AutoPtr<IInteger32> pInt;
    CInteger32::New(scale, (IInteger32**)&pInt);
    v->SetTag(TAG_SPAN, pInt);

    mTmpSet->Clear();

    AutoPtr<ArrayOf<IPoint*> > occupied = GetOccupied(v);
    for (Int32 i = 0; i < occupied->GetLength(); i++) {
        AutoPtr<IPoint> oc = (*occupied)[i];
        AutoPtr<CPoint> coc = (CPoint*)oc.Get();
        AutoPtr<IView> squatter = (*mCells)[coc->mY*mColumns + coc->mX];
        if (squatter != NULL) {
            mTmpSet->Add(squatter);
        }
    }

    AutoPtr<IIterator> it;
    mTmpSet->GetIterator((IIterator**)&it);
    Boolean bHasMore = FALSE;
    while ((it->HasNext(&bHasMore), bHasMore)) {
        AutoPtr<IInterface> pSq;
        it->GetNext((IInterface**)&pSq);
        AutoPtr<IView> squatter = IView::Probe(pSq);
        AutoPtr<ArrayOf<IPoint*> > arr = GetOccupied(squatter);
        for (Int32 i = 0; i < arr->GetLength(); i++) {
            AutoPtr<IPoint> sq = (*arr)[i];
            mFreeList->Add(sq);
            AutoPtr<CPoint> csq = (CPoint*)sq.Get();
            (*mCells)[csq->mY*mColumns + csq->mX] = NULL;
        }
        if (!Object::Equals(squatter->Probe(EIID_IInterface), v->Probe(EIID_IInterface))) {
            squatter->SetTag(TAG_POS, NULL);
            if (animate) {
                AutoPtr<IViewPropertyAnimator> anim;
                squatter->Animate((IViewPropertyAnimator**)&anim);
                anim->WithLayer();
                anim->ScaleX(0.5f);
                anim->ScaleY(0.5f);
                anim->Alpha(0);
                anim->SetDuration(DURATION);
                AutoPtr<IAccelerateInterpolator> ai;
                assert(0 && "TODO");
                // CAccelerateInterpolator::New((IAccelerateInterpolator**)&ai);
                anim->SetInterpolator(ITimeInterpolator::Probe(ai));
                anim->SetListener(new AnimatorListener(this));
                anim->Start();
            }
            else {
                assert(0 && "TODO");
                // ViewGroup::RemoveView(squatter);
            }
        }
    }

    for (Int32 i = 0; i < occupied->GetLength(); i++) {
        AutoPtr<IPoint> oc = (*occupied)[i];
        AutoPtr<CPoint> coc = (CPoint*)oc.Get();
        (*mCells)[coc->mY*mColumns + coc->mX] = v;
        mFreeList->Remove(oc);
    }

    Float rot = (Float)Irand(0, 4) * 90.f;

    if (animate) {
        v->BringToFront();

        AutoPtr<IAnimatorSet> set1;
        CAnimatorSet::New((IAnimatorSet**)&set1);
        assert(0 && "TODO");
        // set1->PlayTogether(
        //         ObjectAnimator::OfFloat(v, View::SCALE_X, (Float) scale),
        //         ObjectAnimator::OfFloat(v, View::SCALE_Y, (Float) scale)
        // );
        AutoPtr<IAnticipateOvershootInterpolator> p;
        assert(0 && "TODO");
//        CAnticipateOvershootInterpolator::New((IAnticipateOvershootInterpolator**)&p);
        IAnimator::Probe(set1)->SetInterpolator(ITimeInterpolator::Probe(p));
        IAnimator::Probe(set1)->SetDuration(DURATION);

        AutoPtr<IAnimatorSet> set2;
        CAnimatorSet::New((IAnimatorSet**)&set2);
        assert(0 && "TODO");
        // set2->PlayTogether(
        //         ObjectAnimator::OfFloat(v, View::ROTATION, rot),
        //         ObjectAnimator::OfFloat(v, View::X, i* mCellSize + (scale-1) * mCellSize /2),
        //         ObjectAnimator::OfFloat(v, View::Y, j* mCellSize + (scale-1) * mCellSize /2)
        // );
        AutoPtr<IDecelerateInterpolator> di;
//        CDecelerateInterpolator::New((IDecelerateInterpolator**)&di);
        IAnimator::Probe(set2)->SetInterpolator(ITimeInterpolator::Probe(di));
        IAnimator::Probe(set2)->SetDuration(DURATION);

        IAnimator::Probe(set1)->AddListener(MakeHardwareLayerListener(v));

        IAnimator::Probe(set1)->Start();
        IAnimator::Probe(set2)->Start();
    }
    else {
        v->SetX(i * mCellSize + (scale-1) * mCellSize /2);
        v->SetY(j * mCellSize + (scale-1) * mCellSize /2);
        v->SetScaleX((Float) scale);
        v->SetScaleY((Float) scale);
        v->SetRotation(rot);
    }
    return NOERROR;
}

AutoPtr<ArrayOf<IPoint*> > CDessertCaseView::GetOccupied(
    /* [in] */ IView* v)
{
    AutoPtr<IInterface> pTS;
    v->GetTag(TAG_SPAN, (IInterface**)&pTS);
    AutoPtr<IInteger32> pInt = IInteger32::Probe(pTS);
    Int32 scale = 0;
    pInt->GetValue(&scale);
    AutoPtr<IInterface> pPt;
    v->GetTag(TAG_POS, (IInterface**)&pPt);
    AutoPtr<IPoint> pt = IPoint::Probe(pPt);
    if (pt == NULL || scale == 0) {
        AutoPtr<IPoint> ip;
        CPoint::New((IPoint**)&ip);
        AutoPtr<ArrayOf<IPoint*> > arr = ArrayOf<IPoint*>::Alloc(1);
        (*arr)[0] = ip;
        return arr;
    }

    AutoPtr<ArrayOf<IPoint*> > result = ArrayOf<IPoint*>::Alloc(scale * scale);
    Int32 p = 0;
    for (Int32 i = 0; i < scale; i++) {
        for (Int32 j = 0; j < scale; j++) {
            AutoPtr<CPoint> cpt = (CPoint*)pt.Get();
            CPoint::New(cpt->mX + i, cpt->mY + j, &((*result)[p++]));
        }
    }
    return result;
}

Float CDessertCaseView::Frand()
{
    return (Float)(Elastos::Core::Math::Random());
}

Float CDessertCaseView::Frand(
    /* [in] */ Float a,
    /* [in] */ Float b)
{
    return (Frand() * (b-a) + a);
}

Int32 CDessertCaseView::Irand(
    /* [in] */ Int32 a,
    /* [in] */ Int32 b)
{
    return (Int32)(Frand(a, b));
}

void CDessertCaseView::OnDraw(
    /* [in] */ ICanvas* c)
{
    assert(0 && "TODO");
//    FrameLayout::OnDraw(c);
    if (!DEBUG) return;

    AutoPtr<IPaint> pt;
    CPaint::New((IPaint**)&pt);
    pt->SetStyle(PaintStyle_STROKE);
    pt->SetColor(0xFFCCCCCC);
    pt->SetStrokeWidth(2.0f);

    AutoPtr<IRect> check;
    CRect::New((IRect**)&check);
    Int32 N = 0;
//    ViewGroup::GetChildCount(&N);
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<IView> stone;
//        ViewGroup::GetChildAt(i, (IView**)&stone);

        stone->GetHitRect(check);

        c->DrawRect(check, pt);
    }
}

//===============================================================
// CDessertCaseView::RescalingContainer::
//===============================================================

CDessertCaseView::RescalingContainer::RescalingContainer(
    /* [in] */ IContext* context)// : FrameLayout(context)
{
    assert(0 && "TODO");
    // View::SetSystemUiVisibility(0
    //         | IView::SYSTEM_UI_FLAG_FULLSCREEN
    //         | IView::SYSTEM_UI_FLAG_HIDE_NAVIGATION
    //         | IView::SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
    //         | IView::SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
    //         | IView::SYSTEM_UI_FLAG_IMMERSIVE_STICKY
    // );
}

void CDessertCaseView::RescalingContainer::SetView(
    /* [in] */ IDessertCaseView* v)
{
    assert(0 && "TODO");
//    ViewGroup::AddView(v);
    mView = v;
}

void CDessertCaseView::RescalingContainer::OnLayout (
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    assert(0 && "TODO");
    // Float w = right-left;
    // Float h = bottom-top;
    // Int32 w2 = (Int32) (w / mView->SCALE / 2);
    // Int32 h2 = (Int32) (h / mView->SCALE / 2);
    // Int32 cx = (Int32) (left + w * 0.5f);
    // Int32 cy = (Int32) (top + h * 0.5f);
//    mView->Layout(cx - w2, cy - h2, cx + w2, cy + h2);
}

void CDessertCaseView::RescalingContainer::SetDarkness(
    /* [in] */ Float p)
{
    mDarkness = p;
    GetDarkness();
    Int32 x = (Int32) (p * 0xff);
    assert(0 && "TODO");
//    View::SetBackgroundColor(x << 24 & 0xFF000000);
}

Float CDessertCaseView::RescalingContainer::GetDarkness()
{
    return mDarkness;
}

//===============================================================
// CDessertCaseView::Runnable_1::
//===============================================================

CDessertCaseView::Runnable_1::Runnable_1(
    /* [in] */ CDessertCaseView* owner)
{
    mOwner = owner;
}

ECode CDessertCaseView::Runnable_1::Run()
{
    Int32 N = 0;
    assert(0 && "TODO");
//    ViewGroup::GetChildCount(&N);

    Int32 K = 1; //irand(1,3);
    for (Int32 i = 0; i < K; i++) {
        AutoPtr<IView> child;
        IViewGroup::Probe(mOwner)->GetChildAt((Int32) (Elastos::Core::Math::Random() * N), (IView**)&child);
        mOwner->Place(child, TRUE);
    }

    mOwner->FillFreeList();

    if (mOwner->mStarted) {
        Boolean res = FALSE;
        mOwner->mHandler->PostDelayed(mOwner->mJuggle, DELAY, &res);
    }
    return NOERROR;
}

//===============================================================
// CDessertCaseView::AnimatorListenerAdapter_1::
//===============================================================
CDessertCaseView::AnimatorListenerAdapter_1::AnimatorListenerAdapter_1(
    /* [in] */ IView* v)
{
    mV = v;
}

ECode CDessertCaseView::AnimatorListenerAdapter_1::OnAnimationStart(
    /* [in] */ IAnimator* animator)
{
    mV->SetLayerType(IView::LAYER_TYPE_HARDWARE, NULL);
    mV->BuildLayer();
    return NOERROR;
}

ECode CDessertCaseView::AnimatorListenerAdapter_1::OnAnimationEnd(
    /* [in] */ IAnimator* animator)
{
    mV->SetLayerType(IView::LAYER_TYPE_NONE, NULL);
    return NOERROR;
}

//===============================================================
// CDessertCaseView::AnimatorListener::
//===============================================================
CAR_INTERFACE_IMPL(CDessertCaseView::AnimatorListener, Object, IAnimatorListener)

CDessertCaseView::AnimatorListener::AnimatorListener(
    /* [in] */ CDessertCaseView* owner)
{
    mOwner = owner;
}

ECode CDessertCaseView::AnimatorListener::OnAnimationStart(
    /* [in] */ IAnimator* animator)
{
    return NOERROR;
}

ECode CDessertCaseView::AnimatorListener::OnAnimationEnd(
    /* [in] */ IAnimator* animator)
{
//    mOwner->RemoveView(squatter);
    return NOERROR;
}

ECode CDessertCaseView::AnimatorListener::OnAnimationCancel(
    /* [in] */ IAnimator* animator)
{
    return NOERROR;
}

ECode CDessertCaseView::AnimatorListener::OnAnimationRepeat(
    /* [in] */ IAnimator* animator)
{
    return NOERROR;
}

//===============================================================
// CDessertCaseView::OnClickListener::
//===============================================================
CAR_INTERFACE_IMPL(CDessertCaseView::OnClickListener, Object, IViewOnClickListener)

CDessertCaseView::OnClickListener::OnClickListener(
    /* [in] */ CDessertCaseView* owner,
    /* [in] */ IImageView* v)
{
    mOwner = owner;
    mV = v;
}

ECode CDessertCaseView::OnClickListener::OnClick(
    /* [in] */ IView* view)
{
    mOwner->Place(IView::Probe(mV), TRUE);
    Boolean res = FALSE;
    IView::Probe(mOwner)->PostDelayed(new Runnable_2(mOwner), DURATION/2, &res);
    return NOERROR;
}

//===============================================================
// CDessertCaseView::Runnable_2::
//===============================================================
CDessertCaseView::Runnable_2::Runnable_2(
    /* [in] */ CDessertCaseView* owner)
{
    mOwner = owner;
}

ECode CDessertCaseView::Runnable_2::Run()
{
    mOwner->FillFreeList();
    return NOERROR;
}

} // namespace SystemUI
} // namepsace Droid
} // namespace Elastos