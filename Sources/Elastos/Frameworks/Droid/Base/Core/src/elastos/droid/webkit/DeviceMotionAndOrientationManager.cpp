#include "elastos/droid/webkit/DeviceMotionAndOrientationManager.h"
#include "elastos/droid/webkit/CWebViewCore.h"

#include <elastos/core/Thread.h>

using Elastos::Core::Thread;

namespace Elastos {
namespace Droid {
namespace Webkit {

DeviceMotionAndOrientationManager::DeviceMotionAndOrientationManager(
    /* [in] */ CWebViewCore* webViewCore)
    : mWebViewCore(webViewCore)
{
}

/**
 * Sets that the Page for this WebViewCore should use a mock DeviceOrientation
 * client.
 */
void DeviceMotionAndOrientationManager::SetUseMock()
{
    String currentThreadName;
    Thread::GetCurrentThread()->GetName(&currentThreadName);
    assert(CWebViewCore::THREAD_NAME.Equals(currentThreadName));
    NativeSetUseMock(mWebViewCore);
}

/**
 * Set the position for the mock DeviceOrientation service for this WebViewCore.
 */
void DeviceMotionAndOrientationManager::SetMockOrientation(
    /* [in] */ Boolean canProvideAlpha,
    /* [in] */ Double alpha,
    /* [in] */ Boolean canProvideBeta,
    /* [in] */ Double beta,
    /* [in] */ Boolean canProvideGamma,
    /* [in] */ Double gamma)
{
    String currentThreadName;
    Thread::GetCurrentThread()->GetName(&currentThreadName);
    assert(CWebViewCore::THREAD_NAME.Equals(currentThreadName));
    NativeSetMockOrientation(mWebViewCore, canProvideAlpha, alpha, canProvideBeta, beta,
            canProvideGamma, gamma);
}

// We only provide accelerationIncludingGravity.
void DeviceMotionAndOrientationManager::OnMotionChange(
    /* [in] */ IDouble* x,
    /* [in] */ IDouble* y,
    /* [in] */ IDouble* z,
    /* [in] */ Double interval)
{
    Double dx, dy, dz;
    NativeOnMotionChange(mWebViewCore,
            x != NULL, x != NULL ? (x->GetValue(&dx), dx) : 0.0,
            y != NULL, y != NULL ? (y->GetValue(&dy), dy) : 0.0,
            z != NULL, z != NULL ? (z->GetValue(&dz), dz) : 0.0,
            interval);
}

void DeviceMotionAndOrientationManager::OnOrientationChange(
    /* [in] */ IDouble* alpha,
    /* [in] */ IDouble* beta,
    /* [in] */ IDouble* gamma)
{
    Double dAlpha, dBeta, dBamma;
    NativeOnOrientationChange(mWebViewCore,
            alpha != NULL, alpha != NULL ? (alpha->GetValue(&dAlpha), dAlpha) : 0.0,
            beta != NULL, beta != NULL ? (beta->GetValue(&dBeta), dBeta) : 0.0,
            gamma != NULL, gamma != NULL ? (gamma->GetValue(&dBamma), dBamma) : 0.0);
}

void DeviceMotionAndOrientationManager::NativeSetUseMock(
    /* [in] */ CWebViewCore* webViewCoreObject)
{
    Elastos_DeviceMotionAndOrientationManager_setUseMock(webViewCoreObject->Probe(EIID_IInterface));
}

void DeviceMotionAndOrientationManager::NativeSetMockOrientation(
    /* [in] */ CWebViewCore* webViewCoreObject,
    /* [in] */ Boolean canProvideAlpha,
    /* [in] */ Double alpha,
    /* [in] */ Boolean canProvideBeta,
    /* [in] */ Double beta,
    /* [in] */ Boolean canProvideGamma,
    /* [in] */ Double gamma)
{
    Elastos_DeviceMotionAndOrientationManager_setMockOrientation(webViewCoreObject->Probe(EIID_IInterface),
                                canProvideAlpha, alpha, canProvideBeta, beta, canProvideGamma, gamma);
}

void DeviceMotionAndOrientationManager::NativeOnMotionChange(
    /* [in] */ CWebViewCore* webViewCoreObject,
    /* [in] */ Boolean canProvideX,
    /* [in] */ Double x,
    /* [in] */ Boolean canProvideY,
    /* [in] */ Double y,
    /* [in] */ Boolean canProvideZ,
    /* [in] */ Double z,
    /* [in] */ Double interval)
{
    Elastos_DeviceMotionAndOrientationManager_onMotionChange(webViewCoreObject->Probe(EIID_IInterface),
                                        canProvideX, x, canProvideY, y, canProvideZ, z, interval);
}

void DeviceMotionAndOrientationManager::NativeOnOrientationChange(
    /* [in] */ CWebViewCore* webViewCoreObject,
    /* [in] */ Boolean canProvideAlpha,
    /* [in] */ Double alpha,
    /* [in] */ Boolean canProvideBeta,
    /* [in] */ Double beta,
    /* [in] */ Boolean canProvideGamma,
    /* [in] */ Double gamma)
{
    Elastos_DeviceMotionAndOrientationManager_onOrientationChange(webViewCoreObject->Probe(EIID_IInterface),
                                canProvideAlpha, alpha, canProvideBeta, beta, canProvideGamma, gamma);
}

ECode DeviceMotionAndOrientationManager::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "DeviceMotionAndOrientationManager";
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
