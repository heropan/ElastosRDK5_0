
#include "elastos/droid/systemui/CDessertCase.h"

using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IContextWrapper;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace SystemUI {

//===============================================================
// CDessertCase::
//===============================================================
CAR_INTERFACE_IMPL(CDessertCase, Object, IDessertCase)

ECode CDessertCase::OnStart()
{
    assert(0 && "TODO");
//    Activity::OnStart();

    AutoPtr<IPackageManager> pm;
//    IContextWrapper::Probe(this)->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IComponentName> cn;
//    CComponentName::New(this, DessertCaseDream.class, (IComponentName**)&cn);
    Int32 ces = 0;
    pm->GetComponentEnabledSetting(cn, &ces);
    if (ces != IPackageManager::COMPONENT_ENABLED_STATE_ENABLED) {
    //     Slog.v("DessertCase", "ACHIEVEMENT UNLOCKED");
        pm->SetComponentEnabledSetting(cn,
                IPackageManager::COMPONENT_ENABLED_STATE_ENABLED,
                IPackageManager::DONT_KILL_APP);
    }

    assert(0 && "TODO");
//     mView = new DessertCaseView(this);

//    DessertCaseView.RescalingContainer container = new DessertCaseView.RescalingContainer(this);

//    container.setView(mView);

//    SetContentView(container);
    return NOERROR;
}

ECode CDessertCase::OnResume()
{
    assert(0 && "TODO");
//    Activity::OnResume();
    AutoPtr<Runnable_1> r = new Runnable_1(this);
    Boolean res = FALSE;
    IView::Probe(mView)->PostDelayed(IRunnable::Probe(r), 1000, &res);
    return NOERROR;
}

ECode CDessertCase::OnPause()
{
    assert(0 && "TODO");
//    Activity::OnPause();
    mView->Stop();
    return NOERROR;
}

//===============================================================
// CDessertCase::Runnable_1::
//===============================================================

CDessertCase::Runnable_1::Runnable_1(
    /* [in] */ CDessertCase* owner)
{
    mOwner = owner;
}

ECode CDessertCase::Runnable_1::Run()
{
    mOwner->mView->Start();
    return NOERROR;
}

} // namespace SystemUI
} // namepsace Droid
} // namespace Elastos