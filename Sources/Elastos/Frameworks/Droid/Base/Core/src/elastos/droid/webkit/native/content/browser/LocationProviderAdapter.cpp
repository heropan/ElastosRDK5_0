
#include "elastos/droid/webkit/native/base/ThreadUtils.h"
#include "elastos/droid/webkit/native/content/browser/LocationProviderAdapter.h"
#include <elastos/utility/concurrent/FutureTask.h>

using Elastos::Droid::Webkit::Base::ThreadUtils;
using Elastos::Utility::Concurrent::FutureTask;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                LocationProviderAdapter::StartRunnable
//=====================================================================

LocationProviderAdapter::StartRunnable::StartRunnable(
    /* [in] */ LocationProviderAdapter* owner,
    /* [in] */ const Boolean& gpsEnabled)
    : mOwner(owner)
    , mGpsEnabled(gpsEnabled)
{
}

ECode LocationProviderAdapter::StartRunnable::Run()
{
    mOwner->mImpl->Start(mGpsEnabled);
    return NOERROR;
}

//=====================================================================
//                LocationProviderAdapter::StopRunnable
//=====================================================================
LocationProviderAdapter::StopRunnable::StopRunnable(
    /* [in] */ LocationProviderAdapter* owner)
    : mOwner(owner)
{
}

ECode LocationProviderAdapter::StopRunnable::Run()
{
    mOwner->mImpl->Stop();
    return NOERROR;
}

//=====================================================================
//                       LocationProviderAdapter
//=====================================================================
AutoPtr<LocationProviderAdapter> LocationProviderAdapter::Create(
    /* [in] */ IContext* context)
{
    return new LocationProviderAdapter(context);
}

Boolean LocationProviderAdapter::Start(
    /* [in] */ const Boolean& gpsEnabled)
{
    AutoPtr<IRunnable> runnable = new StartRunnable(this, gpsEnabled);
    AutoPtr<FutureTask> task = new FutureTask(runnable, NULL);
    ThreadUtils::RunOnUiThread(task);
    return TRUE;
}

ECode LocationProviderAdapter::Stop()
{
    AutoPtr<IRunnable> runnable = new StopRunnable(this);
    AutoPtr<FutureTask> task = new FutureTask(runnable, NULL);
    ThreadUtils::RunOnUiThread(task);
    return NOERROR;
}

Boolean LocationProviderAdapter::IsRunning()
{
    assert(ThreadUtils::RunningOnUiThread());
    return mImpl->IsRunning();
}

ECode LocationProviderAdapter::NewLocationAvailable(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Double timestamp,
    /* [in] */ Boolean hasAltitude,
    /* [in] */ Double altitude,
    /* [in] */ Boolean hasAccuracy,
    /* [in] */ Double accuracy,
    /* [in] */ Boolean hasHeading,
    /* [in] */ Double heading,
    /* [in] */ Boolean hasSpeed,
    /* [in] */ Double speed)
{
    NativeNewLocationAvailable(latitude, longitude, timestamp, hasAltitude, altitude,
             hasAccuracy, accuracy, hasHeading, heading, hasSpeed, speed);
    return NOERROR;
}

ECode LocationProviderAdapter::NewErrorAvailable(
    /* [in] */ const String& message)
{
    NativeNewErrorAvailable(message);
    return NOERROR;
}

LocationProviderAdapter::LocationProviderAdapter(
    /* [in] */ IContext* context)
{
    mImpl = LocationProviderFactory::Get(context);
}

ECode LocationProviderAdapter::NativeNewLocationAvailable(
    /* [in] */ Double latitude,
    /* [in] */ Double longitude,
    /* [in] */ Double timeStamp,
    /* [in] */ Boolean hasAltitude,
    /* [in] */ Double altitude,
    /* [in] */ Boolean hasAccuracy,
    /* [in] */ Double accuracy,
    /* [in] */ Boolean hasHeading,
    /* [in] */ Double heading,
    /* [in] */ Boolean hasSpeed,
    /* [in] */ Double speed)
{
    return NOERROR;
}

ECode LocationProviderAdapter::NativeNewErrorAvailable(
    /* [in] */ const String& message)
{
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
