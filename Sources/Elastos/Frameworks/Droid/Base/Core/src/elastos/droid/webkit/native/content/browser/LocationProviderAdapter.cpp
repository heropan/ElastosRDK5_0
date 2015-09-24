// wuweizuo automatic build .cpp file from .java file.

#include "LocationProviderAdapter.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                LocationProviderAdapter::InnerRunnable
//=====================================================================
LocationProviderAdapter::InnerRunnable::InnerRunnable(
    /* [in] */ LocationProviderAdapter* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode LocationProviderAdapter::InnerRunnable::Run()
{
    // ==================before translated======================
    // mImpl.start(gpsEnabled);
    assert(0);
    return NOERROR;
}

//=====================================================================
//                LocationProviderAdapter::InnerRunnable
//=====================================================================
LocationProviderAdapter::InnerRunnable::InnerRunnable(
    /* [in] */ LocationProviderAdapter* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode LocationProviderAdapter::InnerRunnable::Run()
{
    // ==================before translated======================
    // mImpl.stop();
    assert(0);
    return NOERROR;
}

//=====================================================================
//                       LocationProviderAdapter
//=====================================================================
AutoPtr<LocationProviderAdapter> LocationProviderAdapter::Create(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // return new LocationProviderAdapter(context);
    assert(0);
    AutoPtr<LocationProviderAdapter> empty;
    return empty;
}

Boolean LocationProviderAdapter::Start(
    /* [in] */ const Boolean& gpsEnabled)
{
    // ==================before translated======================
    // FutureTask<Void> task = new FutureTask<Void>(new Runnable() {
    //     @Override
    //     public void run() {
    //         mImpl.start(gpsEnabled);
    //     }
    // }, null);
    // ThreadUtils.runOnUiThread(task);
    // return true;
    assert(0);
    return FALSE;
}

ECode LocationProviderAdapter::Stop()
{
    // ==================before translated======================
    // FutureTask<Void> task = new FutureTask<Void>(new Runnable() {
    //     @Override
    //     public void run() {
    //         mImpl.stop();
    //     }
    // }, null);
    // ThreadUtils.runOnUiThread(task);
    assert(0);
    return NOERROR;
}

Boolean LocationProviderAdapter::IsRunning()
{
    // ==================before translated======================
    // assert ThreadUtils.runningOnUiThread();
    // return mImpl.isRunning();
    assert(0);
    return FALSE;
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
    // ==================before translated======================
    // nativeNewLocationAvailable(latitude, longitude, timestamp, hasAltitude, altitude,
    //         hasAccuracy, accuracy, hasHeading, heading, hasSpeed, speed);
    assert(0);
    return NOERROR;
}

ECode LocationProviderAdapter::NewErrorAvailable(
    /* [in] */ const String& message)
{
    // ==================before translated======================
    // nativeNewErrorAvailable(message);
    assert(0);
    return NOERROR;
}

LocationProviderAdapter::LocationProviderAdapter(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // mImpl = LocationProviderFactory.get(context);
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
    assert(0);
    return NOERROR;
}

ECode LocationProviderAdapter::NativeNewErrorAvailable(
    /* [in] */ const String& message)
{
    assert(0);
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


