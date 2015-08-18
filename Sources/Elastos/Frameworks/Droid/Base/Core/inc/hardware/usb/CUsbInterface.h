
#ifndef __ELASTOS_DROID_HARDWARE_USB_CUSBINTERFACE_H__
#define __ELASTOS_DROID_HARDWARE_USB_CUSBINTERFACE_H__

#include "_Elastos_Droid_Hardware_Usb_CUsbInterface.h"
#include "ext/frameworkext.h"

using namespace Elastos::Core;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Usb {

/**
 * A class representing an interface on a {@link UsbDevice}.
 * USB devices can have one or more interfaces, each one providing a different
 * piece of functionality, separate from the other interfaces.
 * An interface will have one or more {@link UsbEndpoint}s, which are the
 * channels by which the host transfers data with the device.
 *
 * <div class="special reference">
 * <h3>Developer Guides</h3>
 * <p>For more information about communicating with USB hardware, read the
 * <a href="{@docRoot}guide/topics/usb/index.html">USB</a> developer guide.</p>
 * </div>
 */
CarClass(CUsbInterface)
{
public:

    CARAPI constructor();

    /**
     * UsbInterface should only be instantiated by UsbService implementation
     * @hide
     */
    CARAPI constructor(
        /* [in] */ Int32 id,
        /* [in] */ Int32 cls,
        /* [in] */ Int32 subCls,
        /* [in] */ Int32 protocol,
        /* [in] */ const ArrayOf<IParcelable*>& endpoints);

    /**
     * Returns the interface's ID field.
     * This is an integer that uniquely identifies the interface on the device.
     *
     * @return the interface's ID
     */
    CARAPI GetId(
        /* [out] */ Int32* id);

    /**
     * Returns the interface's class field.
     * Some useful constants for USB classes can be found in {@link UsbConstants}
     *
     * @return the interface's class
     */
    CARAPI GetInterfaceClass(
        /* [out] */ Int32* cls);

    /**
     * Returns the interface's subclass field.
     *
     * @return the interface's subclass
     */
    CARAPI GetInterfaceSubclass(
        /* [out] */ Int32* cls);

    /**
     * Returns the interface's protocol field.
     *
     * @return the interface's protocol
     */
    CARAPI GetInterfaceProtocol(
        /* [out] */ Int32* protocol);

    /**
     * Returns the number of {@link android.hardware.usb.UsbEndpoint}s this interface contains.
     *
     * @return the number of endpoints
     */
    CARAPI GetEndpointCount(
        /* [out] */ Int32* count);

    /**
     * Returns the {@link android.hardware.usb.UsbEndpoint} at the given index.
     *
     * @return the endpoint
     */
    CARAPI GetEndpoint(
        /* [in] */ Int32 index,
        /* [out] */ IUsbEndpoint** endpoint);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI DescribeContents (
        /* [out] */ Int32* contents);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [out] */ IParcel* dest);

private:
    Int32 mId;
    Int32 mClass;
    Int32 mSubclass;
    Int32 mProtocol;
    AutoPtr< ArrayOf< AutoPtr<IParcelable> > > mEndpoints;
};

} // namespace Usb
} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_HARDWARE_USB_CUSBINTERFACE_H__
