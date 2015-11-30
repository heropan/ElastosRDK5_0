
#include "elastos/droid/net/NetworkScorerAppManager.h"

namespace Elastos {
namespace Droid {
namespace Net {

AutoPtr<IIntent> NetworkScorerAppManager::CreateScoreIntent()
{
    AutoPtr<IIntent> rev;
#if 0 // TODO: Waiting for CIntent
    CIntent::New((IIntent**)&rev);
#else
    assert(0);
#endif
    return rev;
}

const AutoPtr<IIntent> NetworkScorerAppManager::SCORE_INTENT = CreateScoreIntent();

const String NetworkScorerAppManager::TAG = String("NetworkScorerAppManager");

NetworkScorerAppManager::NetworkScorerAppManager()
{}

ECode NetworkScorerAppManager::GetAllValidScorers(
    /* [in] */ IContext* context,
    /* [out] */ ICollection** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        List<NetworkScorerAppData> scorers = new ArrayList<>();
        PackageManager pm = context.getPackageManager();
        // Only apps installed under the primary user of the device can be scorers.
        List<ResolveInfo> receivers =
                pm.queryBroadcastReceivers(SCORE_INTENT, 0 /* flags */, UserHandle.USER_OWNER);
        for (ResolveInfo receiver : receivers) {
            // This field is a misnomer, see android.content.pm.ResolveInfo#activityInfo
            final ActivityInfo receiverInfo = receiver.activityInfo;
            if (receiverInfo == NULL) {
                // Should never happen with queryBroadcastReceivers, but invalid nonetheless.
                continue;
            }
            if (!permission.BROADCAST_SCORE_NETWORKS.equals(receiverInfo.permission)) {
                // Receiver doesn't require the BROADCAST_SCORE_NETWORKS permission, which means
                // anyone could trigger network scoring and flood the framework with score requests.
                continue;
            }
            if (pm.checkPermission(permission.SCORE_NETWORKS, receiverInfo.packageName) !=
                    PackageManager.PERMISSION_GRANTED) {
                // Application doesn't hold the SCORE_NETWORKS permission, so the user never
                // approved it as a network scorer.
                continue;
            }
            // Optionally, this package may specify a configuration activity.
            String configurationActivityClassName = NULL;
            Intent intent = new Intent(NetworkScoreManager.ACTION_CUSTOM_ENABLE);
            intent.setPackage(receiverInfo.packageName);
            List<ResolveInfo> configActivities = pm.queryIntentActivities(intent, 0 /* flags */);
            if (!configActivities.isEmpty()) {
                ActivityInfo activityInfo = configActivities.get(0).activityInfo;
                if (activityInfo != NULL) {
                    configurationActivityClassName = activityInfo.name;
                }
            }
            // NOTE: loadLabel will attempt to load the receiver's label and fall back to the app
            // label if none is present.
            scorers.add(new NetworkScorerAppData(receiverInfo.packageName,
                    receiverInfo.loadLabel(pm), configurationActivityClassName));
        }
        return scorers;
#endif
}

ECode NetworkScorerAppManager::GetActiveScorer(
    /* [in] */ IContext* context,
    /* [out] */ INetworkScorerAppData** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String scorerPackage = Settings.Global.getString(context.getContentResolver(),
                Settings.Global.NETWORK_SCORER_APP);
        return getScorer(context, scorerPackage);
#endif
}

ECode NetworkScorerAppManager::SetActiveScorer(
    /* [in] */ IContext* context,
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String oldPackageName = Settings.Global.getString(context.getContentResolver(),
                Settings.Global.NETWORK_SCORER_APP);
        if (TextUtils.equals(oldPackageName, packageName)) {
            // No change.
            return TRUE;
        }
        Log.i(TAG, "Changing network scorer from " + oldPackageName + " to " + packageName);
        if (packageName == NULL) {
            Settings.Global.putString(context.getContentResolver(),
                    Settings.Global.NETWORK_SCORER_APP, NULL);
            return TRUE;
        } else {
            // We only make the change if the new package is valid.
            if (getScorer(context, packageName) != NULL) {
                Settings.Global.putString(context.getContentResolver(),
                        Settings.Global.NETWORK_SCORER_APP, packageName);
                return TRUE;
            } else {
                Log.w(TAG, "Requested network scorer is not valid: " + packageName);
                return FALSE;
            }
        }
#endif
}

ECode NetworkScorerAppManager::IsCallerActiveScorer(
    /* [in] */ IContext* context,
    /* [in] */ Int32 callingUid,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        NetworkScorerAppData defaultApp = getActiveScorer(context);
        if (defaultApp == NULL) {
            return FALSE;
        }
        AppOpsManager appOpsMgr = (AppOpsManager) context.getSystemService(Context.APP_OPS_SERVICE);
        try {
            appOpsMgr.checkPackage(callingUid, defaultApp.mPackageName);
        } catch (SecurityException e) {
            return FALSE;
        }
        // To be extra safe, ensure the caller holds the SCORE_NETWORKS permission. It always
        // should, since it couldn't become the active scorer otherwise, but this can't hurt.
        return context.checkCallingPermission(Manifest.permission.SCORE_NETWORKS) ==
                PackageManager.PERMISSION_GRANTED;
#endif
}

ECode NetworkScorerAppManager::GetScorer(
    /* [in] */ IContext* context,
    /* [in] */ const String& packageName,
    /* [out] */ INetworkScorerAppData** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (TextUtils.isEmpty(packageName)) {
            return NULL;
        }
        Collection<NetworkScorerAppData> applications = getAllValidScorers(context);
        for (NetworkScorerAppData app : applications) {
            if (packageName.equals(app.mPackageName)) {
                return app;
            }
        }
        return NULL;
#endif
}

//=====================================================================
// NetworkScorerAppData
//=====================================================================
CAR_INTERFACE_IMPL(NetworkScorerAppData, Object, INetworkScorerAppData)

ECode NetworkScorerAppData::constructor(
    /* [in] */ const String& packageName,
    /* [in] */ ICharSequence* scorerName,
    /* [in] */ /* @Nullable */ const String& configurationActivityClassName)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                mScorerName = scorerName;
                mPackageName = packageName;
                mConfigurationActivityClassName = configurationActivityClassName;
#endif
}

ECode NetworkScorerAppData::GetPackageName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mPackageName;
    return NOERROR;
}

ECode NetworkScorerAppData::GetScorerName(
    /* [out] */ ICharSequence** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = mScorerName;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode NetworkScorerAppData::GetConfigurationActivityClassName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)

    *result = mConfigurationActivityClassName;
    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos


