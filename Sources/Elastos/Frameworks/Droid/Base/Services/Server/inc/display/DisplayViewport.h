
#ifndef __ELASTOS_DROID_SERVER_DISPLAY_DISPLAYVIEWPORT_H__
#define __ELASTOS_DROID_SERVER_DISPLAY_DISPLAYVIEWPORT_H__

#include "Elastos.Droid.Server_server.h"
#include "ext/frameworkext.h"

using Elastos::Droid::Graphics::IRect;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

extern "C" const InterfaceID EIID_DisplayViewport;

/**
 * Describes how the pixels of physical display device reflects the content of
 * a logical display.
 * <p>
 * This information is used by the input system to translate touch input from
 * physical display coordinates into logical display coordinates.
 * </p>
 */
class DisplayViewport
    : public ElRefBase
    , public IDisplayViewport
{
public:
    CAR_INTERFACE_DECL()

    DisplayViewport();

    CARAPI CopyFrom(
        /* [in] */ IDisplayViewport* viewport);

    CARAPI CopyFrom(
        /* [in] */ DisplayViewport* vp);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI SetValid(
        /* [in] */ Boolean valid);

    CARAPI IsValid(
        /* [out] */ Boolean* valid);

    CARAPI SetOrientation(
        /* [in] */ Int32 ori);

    CARAPI GetOrientation(
        /* [out] */ Int32* ori);

    CARAPI SetLogicalFrame(
        /* [in] */ IRect* frame);

    CARAPI GetLogicalFrame(
        /* [out] */ IRect** frame);

    CARAPI SetPhysicalFrame(
        /* [in] */ IRect* frame);

    CARAPI GetPhysicalFrame(
        /* [out] */ IRect** frame);

    CARAPI SetDeviceWidth(
        /* [in] */ Int32 w);

    CARAPI GetDeviceWidth(
        /* [out] */ Int32* w);

    CARAPI SetDeviceHeight(
        /* [in] */ Int32 h);

    CARAPI GetDeviceHeight(
        /* [out] */ Int32* h);
public:
    // True if this viewport is valid.
    Boolean mValid;

    // The logical display id.
    Int32 mDisplayId;

    // The rotation applied to the physical coordinate system.
    Int32 mOrientation;

    // The portion of the logical display that are presented on this physical display.
    AutoPtr<IRect> mLogicalFrame;

    // The portion of the (rotated) physical display that shows the logical display contents.
    // The relation between logical and physical frame defines how the coordinate system
    // should be scaled or translated after rotation.
    AutoPtr<IRect> mPhysicalFrame;

    // The full width and height of the display device, rotated in the same
    // manner as physicalFrame.  This expresses the full native size of the display device.
    // The physical frame should usually fit within this area.
    Int32 mDeviceWidth;
    Int32 mDeviceHeight;
};

} // namespace Display
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_DISPLAY_DISPLAYVIEWPORT_H__
