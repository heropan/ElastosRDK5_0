
#ifndef __ELASTOS_DROID_HARDWARE_CCONSUMERIRMANAGER_H__
#define __ELASTOS_DROID_HARDWARE_CCONSUMERIRMANAGER_H__

#include "_Elastos_Droid_Hardware_CConsumerIrManager.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Content::IContext;
using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Hardware {

CarClass(CConsumerIrManager)
    , public Object
    , public IConsumerIrManager
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    /**
     * @hide to prevent subclassing from outside of the framework
     */
    CARAPI constructor(
        /* [in] */ IContext* context);

    /**
     * Check whether the device has an infrared emitter.
     *
     * @return true if the device has an infrared emitter, else false.
     */
    CARAPI HasIrEmitter(
        /* [out] */ Boolean* value);

    /**
     * Transmit an infrared pattern
     * <p>
     * This method is synchronous; when it returns the pattern has
     * been transmitted. Only patterns shorter than 2 seconds will
     * be transmitted.
     * </p>
     *
     * @param carrierFrequency The IR carrier frequency in Hertz.
     * @param pattern The alternating on/off pattern in microseconds to transmit.
     */
    CARAPI Transmit(
        /* [in] */ Int32 carrierFrequency,
        /* [in] */ ArrayOf<Int32>* pattern);

    /**
     * Query the infrared transmitter's supported carrier frequencies
     *
     * @return an array of
     * {@link android.hardware.ConsumerIrManager.CarrierFrequencyRange}
     * objects representing the ranges that the transmitter can support, or
     * null if there was an error communicating with the Consumer IR Service.
     */
    CARAPI GetCarrierFrequencies(
        /* [out, callee] */ ArrayOf<ICarrierFrequencyRange*>** outarr);

private:
    static const String TAG;

    String mPackageName;
    AutoPtr<IIConsumerIrService> mService;
};

} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_HARDWARE_CCONSUMERIRMANAGER_H__
