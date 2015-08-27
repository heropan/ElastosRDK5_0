// wuweizuo automatic build .cpp file from .java file.

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
    /*
    mOwner = owner;
    */
}

ECode LocationProviderAdapter::InnerRunnable::Run()
{
    /*
    mImpl.start(gpsEnabled);
    */
    return NOERROR;
}

//=====================================================================
//                LocationProviderAdapter::InnerRunnable
//=====================================================================
LocationProviderAdapter::InnerRunnable::InnerRunnable(
    /* [in] */ LocationProviderAdapter* owner)
    : mOwner(owner)
{
    /*
    mOwner = owner;
    */
}

ECode LocationProviderAdapter::InnerRunnable::Run()
{
    /*
    mImpl.stop();
    */
    return NOERROR;
}

//=====================================================================
//                       LocationProviderAdapter
//=====================================================================
AutoPtr<LocationProviderAdapter> LocationProviderAdapter::Create(
    /* [in] */ IContext* context)
{
    /*
    return new LocationProviderAdapter(context);
    */
    return AutoPtr<LocationProviderAdapter>(this);
}

const Boolean LocationProviderAdapter::Start(
    /* [in] */ Boolean gpsEnabled)
{
    /*
    FutureTask<Void> task = new FutureTask<Void>(// [wuweizuo auto add note here: new InnerRunnable(this)] new Runnable() {
        @Override
        public void run() {
            mImpl.start(gpsEnabled);
        }
    }, null);
    ThreadUtils.runOnUiThread(task);
    return true;
    */
    return FALSE;
}

ECode LocationProviderAdapter::Stop()
{
    /*
    FutureTask<Void> task = new FutureTask<Void>(// [wuweizuo auto add note here: new InnerRunnable(this)] new Runnable() {
        @Override
        public void run() {
            mImpl.stop();
        }
    }, null);
    ThreadUtils.runOnUiThread(task);
    */
    return NOERROR;
}

Boolean LocationProviderAdapter::IsRunning()
{
    /*
    assert ThreadUtils.runningOnUiThread();
    return mImpl.isRunning();
    */
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
    /*
    nativeNewLocationAvailable(latitude, longitude, timestamp, hasAltitude, altitude,
            hasAccuracy, accuracy, hasHeading, heading, hasSpeed, speed);
    */
    return NOERROR;
}

ECode LocationProviderAdapter::NewErrorAvailable(
    /* [in] */ String message)
{
    /*
    nativeNewErrorAvailable(message);
    */
    return NOERROR;
}

LocationProviderAdapter::LocationProviderAdapter(
    /* [in] */ IContext* context)
{
    /*
    mImpl = LocationProviderFactory.get(context);
    */
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
    /* [in] */ String message)
{
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


