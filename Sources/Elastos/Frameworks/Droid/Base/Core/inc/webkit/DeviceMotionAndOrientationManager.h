
#ifndef __ELASTOS_DROID_WEBKIT_DEVICEMOTIONANDORIENTATIONMANAGER_H__
#define __ELASTOS_DROID_WEBKIT_DEVICEMOTIONANDORIENTATIONMANAGER_H__

#include "ext/frameworkext.h"

using Elastos::Core::IDouble;

#ifdef __cplusplus
extern "C"
{
#endif
    extern void Elastos_DeviceMotionAndOrientationManager_setUseMock(IInterface* webViewCoreObject);
    extern void Elastos_DeviceMotionAndOrientationManager_onMotionChange(IInterface* webViewCoreObject,
                  Boolean canProvideX, Double x, Boolean canProvideY, Double y,
                  Boolean canProvideZ, Double z, Double interval);
    extern void Elastos_DeviceMotionAndOrientationManager_setMockOrientation(IInterface* webViewCoreObject,
                  Boolean canProvideAlpha, Double alpha,
                  Boolean canProvideBeta, Double beta,
                  Boolean canProvideGamma, Double gamma);
    extern void Elastos_DeviceMotionAndOrientationManager_onOrientationChange(IInterface* webViewCoreObject,
                            Boolean canProvideAlpha, Double alpha,
                            Boolean canProvideBeta, Double beta,
                            Boolean canProvideGamma, Double gamma);
#ifdef __cplusplus
}
#endif

namespace Elastos {
namespace Droid {
namespace Webkit {

class CWebViewCore;

/**
 * This class is simply a container for the methods used to implement DeviceMotion and
 * DeviceOrientation, including the mock DeviceOrientationClient for use in LayoutTests.
 *
 * This could be part of WebViewCore, but have moved it to its own class to
 * avoid bloat there.
 */
class DeviceMotionAndOrientationManager : public ElRefBase
{
public:
    DeviceMotionAndOrientationManager(
        /* [in] */ CWebViewCore* webViewCore);

    /**
     * Sets that the Page for this WebViewCore should use a mock DeviceOrientation
     * client.
     */
    CARAPI_(void) SetUseMock();

    /**
     * Set the position for the mock DeviceOrientation service for this WebViewCore.
     */
    CARAPI_(void) SetMockOrientation(
        /* [in] */ Boolean canProvideAlpha,
        /* [in] */ Double alpha,
        /* [in] */ Boolean canProvideBeta,
        /* [in] */ Double beta,
        /* [in] */ Boolean canProvideGamma,
        /* [in] */ Double gamma);

    // We only provide accelerationIncludingGravity.
    CARAPI_(void) OnMotionChange(
        /* [in] */ IDouble* x,
        /* [in] */ IDouble* y,
        /* [in] */ IDouble* z,
        /* [in] */ Double interval);

    CARAPI_(void) OnOrientationChange(
        /* [in] */ IDouble* alpha,
        /* [in] */ IDouble* beta,
        /* [in] */ IDouble* gamma);

private:
    // Native functions
    static CARAPI_(void) NativeSetUseMock(
        /* [in] */ CWebViewCore* webViewCore);

    static CARAPI_(void) NativeSetMockOrientation(
        /* [in] */ CWebViewCore* webViewCore,
        /* [in] */ Boolean canProvideAlpha,
        /* [in] */ Double alpha,
        /* [in] */ Boolean canProvideBeta,
        /* [in] */ Double beta,
        /* [in] */ Boolean canProvideGamma,
        /* [in] */ Double gamma);

    static CARAPI_(void) NativeOnMotionChange(
        /* [in] */ CWebViewCore* webViewCore,
        /* [in] */ Boolean canProvideX,
        /* [in] */ Double x,
        /* [in] */ Boolean canProvideY,
        /* [in] */ Double y,
        /* [in] */ Boolean canProvideZ,
        /* [in] */ Double z,
        /* [in] */ Double interval);

    static CARAPI_(void) NativeOnOrientationChange(
        /* [in] */ CWebViewCore* webViewCore,
        /* [in] */ Boolean canProvideAlpha,
        /* [in] */ Double alpha,
        /* [in] */ Boolean canProvideBeta,
        /* [in] */ Double beta,
        /* [in] */ Boolean canProvideGamma,
        /* [in] */ Double gamma);

private:
    AutoPtr<CWebViewCore> mWebViewCore;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_DEVICEMOTIONANDORIENTATIONMANAGER_H__
