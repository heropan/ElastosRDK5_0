
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/app/admin/CDeviceAdminReceiver.h"

using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace App {
namespace Admin {

const String TAG = String("DevicePolicy");

Boolean CDeviceAdminReceiver::sLocalLOGV = FALSE;

ECode CDeviceAdminReceiver::GetManager(
    /* [in] */ IContext* context,
    /* [out] */ IDevicePolicyManager** manager)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(manager);
    *manager = NULL;

    if (mManager != null) {
        *manager = mManager;
        REFCOUNT_ADD(*manager);
        return NOERROR;
    }

    return context->GetSystemService(IContext::DEVICE_POLICY_SERVICE,
            (IInterface**)manager);
}

ECode CDeviceAdminReceiver::GetWho(
    /* [in] */ IContext* context,
    /* [out] */ IComponentName** component)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(component);

    if (mWho != null) {
        *component = mWho;
        REFCOUNT_ADD(*component);
        return NOERROR;
    }

    //mWho = new ComponentName(context, getClass());
    String className("CDeviceAdminReceiver");
    CComponentName::New(context, className, (IComponentName**)&mWho);
    *component = mWho;
    REFCOUNT_ADD(*component);
    return NOERROR;
}

ECode CDeviceAdminReceiver::OnEnabled(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return NOERROR;
}

ECode CDeviceAdminReceiver::OnDisableRequested(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent,
    /* [out] */ ICharSequence** message)
{
    VALIDATE_NOT_NULL(message);
    *message = NULL;
    return NOERROR;
}

ECode CDeviceAdminReceiver::OnDisabled(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return NOERROR;
}


ECode CDeviceAdminReceiver::OnPasswordChanged(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return NOERROR;
}


ECode CDeviceAdminReceiver::OnPasswordFailed(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return NOERROR;
}


ECode CDeviceAdminReceiver::OnPasswordSucceeded(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return NOERROR;
}

ECode CDeviceAdminReceiver::OnPasswordExpiring(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return NOERROR;
}

ECode CDeviceAdminReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);

    if (IDeviceAdminReceiver::ACTION_PASSWORD_CHANGED == action) {
        OnPasswordChanged(context, intent);
    }
    else if (IDeviceAdminReceiver::ACTION_PASSWORD_FAILED == action) {
        OnPasswordFailed(context, intent);
    }
    else if (IDeviceAdminReceiver::ACTION_PASSWORD_SUCCEEDED == action) {
        OnPasswordSucceeded(context, intent);
    }
    else if (IDeviceAdminReceiver::ACTION_DEVICE_ADMIN_ENABLED == action) {
        OnEnabled(context, intent);
    }
    else if (IDeviceAdminReceiver::ACTION_DEVICE_ADMIN_DISABLE_REQUESTED
            == action) {
        AutoPtr < ICharSequence > res;
        OnDisableRequested(context, intent, (ICharSequence**)&res);
        if (res != NULL) {
            AutoPtr < IBundle > extras;
            GetResultExtras(TRUE, (IBundle**)&extras);
            extras->PutCharSequence(IDeviceAdminReceiver::EXTRA_DISABLE_WARNING, res);
        }
    }
    else if (IDeviceAdminReceiver::ACTION_DEVICE_ADMIN_DISABLED == action) {
        OnDisabled(context, intent);
    }
    else if (IDeviceAdminReceiver::ACTION_PASSWORD_EXPIRING == action) {
        OnPasswordExpiring(context, intent);
    }
}

} // namespace Admin
} // namespace App
} // namespace Droid
} // namespace Elastos
