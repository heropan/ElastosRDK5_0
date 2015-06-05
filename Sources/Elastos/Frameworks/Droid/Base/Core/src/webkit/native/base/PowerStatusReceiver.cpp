
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

//@Override
ECode PowerStatusReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    PowerMonitor::OnBatteryChargingChanged(intent);
}

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos