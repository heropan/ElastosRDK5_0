
#include "elastos/droid/launcher2/CHotseat.h"
#include "elastos/droid/view/LayoutInflater.h"
#include "Elastos.Droid.Service.h"
#include "R.h"
#include <elastos/core/CoreUtils.h>

using Elastos::Core::CoreUtils;
using Elastos::Droid::View::LayoutInflater;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IViewOnKeyListener;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::EIID_IViewOnTouchListener;
using Elastos::Droid::Widget::ITextView;

namespace Elastos {
namespace Droid {
namespace Launcher2 {

CAR_INTERFACE_IMPL(CHotseat::MyOnTouchListener, Object, IViewOnTouchListener);

CHotseat::MyOnTouchListener::MyOnTouchListener(
    /* [in] */ ILauncher* launcher)
    : mLauncher(launcher)
{
}

ECode CHotseat::MyOnTouchListener::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mLauncher != NULL) {
        Int32 action;
        event->GetAction(&action);
        if ((action & IMotionEvent::ACTION_MASK) == IMotionEvent::ACTION_DOWN) {
            assert(0 && "need class mLauncher");
            //mLauncher->OnTouchDownAllAppsButton(v, &result);
        }
    }
    *result = FALSE;
    return NOERROR;
}

CAR_INTERFACE_IMPL(CHotseat::MyOnClickListener, Object, IViewOnClickListener);

CHotseat::MyOnClickListener::MyOnClickListener(
    /* [in] */ ILauncher* launcher)
    : mLauncher(launcher)
{
}

ECode CHotseat::MyOnClickListener::OnClick(
    /* [in] */ IView* v)
{
    if (mLauncher != NULL) {
        assert(0 && "need class mLauncher");
        //return mLauncher->OnClickAllAppsButton(v);
    }
    return NOERROR;
}

const String CHotseat::TAG("Hotseat");

CAR_INTERFACE_IMPL(CHotseat, FrameLayout, IHotseat);

CHotseat::CHotseat()
    : mCellCountX(0)
    , mCellCountY(0)
    , mAllAppsButtonRank(0)
    , mTransposeLayoutWithOrientation(FALSE)
    , mIsLandscape(FALSE)
{
}

ECode CHotseat::constructor()
{
    return NOERROR;
}

ECode CHotseat::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode CHotseat::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, 0);
}

ECode CHotseat::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FrameLayout::constructor(context, attrs, defStyle);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(Elastos::Droid::Launcher2::R::styleable::Hotseat),
            ArraySize(Elastos::Droid::Launcher2::R::styleable::Hotseat));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

    AutoPtr<IResources> r;
    context->GetResources((IResources**)&r);
    a->GetInt32(Elastos::Droid::Launcher2::R::styleable::Hotseat_cellCountX,
            -1, &mCellCountX);
    a->GetInt32(Elastos::Droid::Launcher2::R::styleable::Hotseat_cellCountY,
            -1, &mCellCountY);
    r->GetInteger(Elastos::Droid::Launcher2::R::integer::hotseat_all_apps_index,
            &mAllAppsButtonRank);
    r->GetBoolean(Elastos::Droid::Launcher2::R::bool_::
            hotseat_transpose_layout_with_orientation, &mTransposeLayoutWithOrientation);
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    AutoPtr<IConfiguration> config;
    resources->GetConfiguration((IConfiguration**)&config);
    Int32 orientation;
    config->GetOrientation(&orientation);
    mIsLandscape = orientation == IConfiguration::ORIENTATION_LANDSCAPE;
    return NOERROR;
}

ECode CHotseat::Setup(
    /* [in] */ ILauncher* launcher)
{
    mLauncher = launcher;
    assert(0 && "need class FocusHelper");
    //AutoPtr<IViewOnKeyListener> listener = new HotseatIconKeyEventListener();
    //return SetOnKeyListener(listener);
    return NOERROR;
}

ECode CHotseat::GetLayout(
    /* [out] */ ICellLayout** cout)
{
    VALIDATE_NOT_NULL(cout);

    *cout = mContent;
    REFCOUNT_ADD(*cout);
    return NOERROR;
}

Boolean CHotseat::HasVerticalHotseat()
{
    return (mIsLandscape && mTransposeLayoutWithOrientation);
}

ECode CHotseat::GetOrderInHotseat(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    if (HasVerticalHotseat()) {
        Int32 countY;
        assert(0 && "need class ICellLayout");
        //mContent->GetCountY(&countY);
        *result = countY - y - 1;
    }
    else {
        *result = x;
    }
    return NOERROR;
}

ECode CHotseat::GetCellXFromOrder(
    /* [in] */ Int32 rank,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = HasVerticalHotseat() ? 0 : rank;
    return NOERROR;
}

ECode CHotseat::GetCellYFromOrder(
    /* [in] */ Int32 rank,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    if (HasVerticalHotseat()) {
        Int32 countY;
        assert(0 && "need class ICellLayout");
        //mContent->GetCountY(&countY);
        *result = countY - (rank + 1);
    }
    else {
        *result = 0;
    }
    return NOERROR;
}

ECode CHotseat::IsAllAppsButtonRank(
    /* [in] */ Int32 rank,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = rank == mAllAppsButtonRank;
    return NOERROR;
}

ECode CHotseat::OnFinishInflate()
{
    FrameLayout::OnFinishInflate();
    if (mCellCountX < 0) {
        assert(0 && "need class LauncherModel");
        //LauncherModel::GetCellCountX(&mCellCountX);
    }
    if (mCellCountY < 0) {
        assert(0 && "need class LauncherModel");
        //LauncherModel::GetCellCountY(&mCellCountY);
    }
    AutoPtr<IView> view;
    FindViewById(Elastos::Droid::Launcher2::R::id::layout, (IView**)&view);
    mContent = ICellLayout::Probe(view);
    assert(0 && "need class ICellLayout");
    // mContent->SetGridSize(mCellCountX, mCellCountY);
    // mContent->SetIsHotseat(TRUE);

    return ResetLayout();
}

ECode CHotseat::ResetLayout()
{
    assert(0 && "need class ICellLayout");
    // mContent->RemoveAllViewsInLayout();

    // Add the Apps button
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<ILayoutInflater> inflater;
    LayoutInflater::From(context, (ILayoutInflater**)&inflater);

    AutoPtr<IView> view;
    inflater->Inflate(Elastos::Droid::Launcher2::R::layout::application,
            IViewGroup::Probe(mContent), FALSE, (IView**)&view);
    AutoPtr<IBubbleTextView> allAppsButton = IBubbleTextView::Probe(view);

    AutoPtr<IResources> r;
    context->GetResources((IResources**)&r);
    AutoPtr<IDrawable> drawable;
    r->GetDrawable(Elastos::Droid::Launcher2::R::
            drawable::all_apps_button_icon, (IDrawable**)&drawable);
    ITextView::Probe(allAppsButton)->SetCompoundDrawablesWithIntrinsicBounds(NULL, drawable, NULL, NULL);

    String lable;
    context->GetString(Elastos::Droid::Launcher2::R::
            string::all_apps_button_label, &lable);
    AutoPtr<ICharSequence> cchar = CoreUtils::Convert(lable);
    IView::Probe(allAppsButton)->SetContentDescription(cchar);

    AutoPtr<MyOnTouchListener> listener1 = new MyOnTouchListener(mLauncher);
    IView::Probe(allAppsButton)->SetOnTouchListener(IViewOnTouchListener::Probe(listener1));

    AutoPtr<MyOnClickListener> listener2 = new MyOnClickListener(mLauncher);
    IView::Probe(allAppsButton)->SetOnClickListener(IViewOnClickListener::Probe(listener2));

    // Note: We do this to ensure that the hotseat is always laid out in the orientation of
    // the hotseat in order regardless of which orientation they were added
    Int32 x;
    GetCellXFromOrder(mAllAppsButtonRank, &x);
    Int32 y;
    GetCellYFromOrder(mAllAppsButtonRank, &y);
    assert(0 && "need class CellLayout");
    // AutoPtr<CellLayout::LayoutParams> lp = new CellLayout::LayoutParams(x,y,1,1);
    // lp->mCanReorder = FALSE;
    assert(0 && "need class ICellLayout");
    //return mContent->AddViewToCellLayout(allAppsButton, -1, 0, lp, TRUE);
    return NOERROR;
}

} // namespace Launcher2
} // namespace Droid
} // namespace Elastos