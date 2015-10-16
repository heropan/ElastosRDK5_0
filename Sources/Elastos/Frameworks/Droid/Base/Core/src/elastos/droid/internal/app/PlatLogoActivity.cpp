
#include "elastos/droid/app/PlatLogoActivity.h"
#include "elastos/droid/utility/CDisplayMetrics.h"
#include "elastos/droid/os/Build.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/widget/CTextView.h"
#include "elastos/droid/widget/CImageView.h"
#include "elastos/droid/widget/CLinearLayout.h"
#include "elastos/droid/widget/CLinearLayoutLayoutParams.h"
#include "elastos/droid/graphics/CTypefaceHelper.h"
#include "elastos/droid/view/CViewGroupLayoutParams.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Droid::Os::Build;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::EIID_IViewOnLongClickListener;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Widget::CTextView;
using Elastos::Droid::Widget::CImageView;
using Elastos::Droid::Widget::ILinearLayout;
using Elastos::Droid::Widget::CLinearLayout;
using Elastos::Droid::Widget::ILinearLayoutLayoutParams;
using Elastos::Droid::Widget::CLinearLayoutLayoutParams;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Utility::CDisplayMetrics;
using Elastos::Droid::Graphics::CTypefaceHelper;
using Elastos::Droid::Graphics::ITypefaceHelper;
using Elastos::Droid::Graphics::ITypeface;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

CAR_INTERFACE_IMPL(LocalViewOnClickListener, IViewOnClickListener);

ECode LocalViewOnClickListener::OnClick(
    /* [in] */ IView* v)
{
    mHost->mToast->Show();
    mHost->mContent->SetImageResource(0/*com.android.internal.R.drawable.platlogo*/);
    return NOERROR;
}

CAR_INTERFACE_IMPL(LocalViewOnLongClickListener, IViewOnLongClickListener);

ECode LocalViewOnLongClickListener::OnLongClick(
    /* [in] */ IView* v,
    /* [out] */ Boolean* result)
{
//    try {
    AutoPtr<IIntent> intent;
    CIntent::New(IIntent::ACTION_MAIN, (IIntent**)&intent);
    intent->SetFlags(IIntent::FLAG_ACTIVITY_NEW_TASK
            | IIntent::FLAG_ACTIVITY_CLEAR_TASK
            | IIntent::FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS);
    intent->AddCategory(String("com.android.internal.category.PLATLOGO"));
    mHost->StartActivity(intent);
        //.setClassName("com.android.systemui","com.android.systemui.BeanBag"));
//    } catch (ActivityNotFoundException ex) {
//        android.util.Log.e("PlatLogoActivity", "Couldn't find a bag of beans.");
//    }
    mHost->Finish();
    *result = TRUE;
    return NOERROR;
}


AutoPtr<IView> PlatLogoActivity::MakeView()
{
    AutoPtr<IDisplayMetrics> metrics;
    CDisplayMetrics::New((IDisplayMetrics**)&metrics);
    AutoPtr<IDisplay> display;
    GetWindowManager()->GetDefaultDisplay((IDisplay**)&display);
    display->GetMetrics(metrics);

    AutoPtr<ILinearLayout> view;
    CLinearLayout::New(this, (ILinearLayout**)&view);
    view->SetOrientation(ILinearLayout::VERTICAL);
    AutoPtr<IViewGroupLayoutParams> params;
    CViewGroupLayoutParams::New(
                IViewGroupLayoutParams::WRAP_CONTENT,
                IViewGroupLayoutParams::WRAP_CONTENT,
                (IViewGroupLayoutParams**)&params);
    view->SetLayoutParams(params);
    Float density;
    metrics->GetDensity(&density);
    Int32 p = (Int32)(8 * density);
    view->SetPadding(p, p, p, p);

    AutoPtr<ITypefaceHelper> tfHelper;
    CTypefaceHelper::AcquireSingleton((ITypefaceHelper**)&tfHelper);
    AutoPtr<ITypeface> light;
    tfHelper->Create(String("sans-serif-light"), ITypeface::NORMAL, (ITypeface**)&light);
    AutoPtr<ITypeface> normal;
    tfHelper->Create(String("sans-serif"), ITypeface::BOLD, (ITypeface**)&normal);

    Float size = 14 * density;
    AutoPtr<ILinearLayoutLayoutParams> lp;
    CLinearLayoutLayoutParams::New(
            ILinearLayoutLayoutParams::WRAP_CONTENT,
            ILinearLayoutLayoutParams::WRAP_CONTENT,
            (ILinearLayoutLayoutParams**)&lp);
    lp->SetGravity(IGravity::CENTER_HORIZONTAL);
    lp->SetBottomMargin((Int32) (-4*density));

    AutoPtr<ITextView> tv;
    CTextView::New(this, (ITextView**)&tv);
    if (light != NULL) tv->SetTypeface(light);
    tv->SetTextSize(1.25f*size);
    tv->SetTextColor(0xFFFFFFFF);
    tv->SetShadowLayer(4*density, 0, 2*density, 0x66000000);
//    tv->SetText(String("Android ") + Build::VERSION::RELEASE);
    view->AddView(tv, lp);

    tv = NULL;
    CTextView::New(this, (ITextView**)&tv);
    if (normal != NULL) tv->SetTypeface(normal);
    tv->SetTextSize(size);
    tv->SetTextColor(0xFFFFFFFF);
    tv->SetShadowLayer(4*density, 0, 2*density, 0x66000000);
    AutoPtr<ICharSequence> txt;
    CString::New(String("JELLY BEAN"), (ICharSequence**)&txt);
    tv->SetText(txt);
    view->AddView(tv, lp);

    return view;
}

ECode PlatLogoActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    Activity::OnCreate(savedInstanceState);

//     mToast = Toast.makeText(this, "", Toast.LENGTH_LONG);
    mToast->SetView(MakeView());

    AutoPtr<IDisplayMetrics> metrics;
    CDisplayMetrics::New((IDisplayMetrics**)&metrics);
    AutoPtr<IDisplay> display;
    GetWindowManager()->GetDefaultDisplay((IDisplay**)&display);
    display->GetMetrics(metrics);

    CImageView::New(this, (IImageView**)&mContent);
    mContent->SetImageResource(0/*com.android.internal.R.drawable.platlogo_alt*/);
    mContent->SetScaleType(Elastos::Droid::Widget::ImageViewScaleType_CENTER_INSIDE);

    Float density;
    metrics->GetDensity(&density);
    Int32 p = (Int32)(32 * density);
    mContent->SetPadding(p, p, p, p);

    mContent->SetOnClickListener((IViewOnClickListener*)new LocalViewOnClickListener(this));

    mContent->SetOnLongClickListener((IViewOnLongClickListener*)new LocalViewOnLongClickListener(this));

    SetContentView(mContent);
    return NOERROR;
}

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos
