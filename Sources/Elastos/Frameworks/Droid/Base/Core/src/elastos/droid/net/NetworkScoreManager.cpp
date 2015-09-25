
#include "elastos/droid/net/NetworkScoreManager.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(NetworkScoreManager, Object, INetworkScoreManager)

ECode NetworkScoreManager::constructor(
    /* [in] */ IContext* context)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mContext = context;
        IBinder iBinder = ServiceManager.getService(Context.NETWORK_SCORE_SERVICE);
        mService = INetworkScoreService.Stub.asInterface(iBinder);

#endif
}

ECode NetworkScoreManager::GetActiveScorerPackage(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        NetworkScorerAppData app = NetworkScorerAppManager.getActiveScorer(mContext);
        if (app == null) {
            return null;
        }
        return app.mPackageName;

#endif
}

ECode NetworkScoreManager::UpdateScores(
    /* [in] */ ArrayOf<IScoredNetwork*>* networks,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            return mService.updateScores(networks);
        } catch (RemoteException e) {
            return false;
        }

#endif
}

ECode NetworkScoreManager::ClearScores(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            return mService.clearScores();
        } catch (RemoteException e) {
            return false;
        }

#endif
}

ECode NetworkScoreManager::SetActiveScorer(
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            return mService.setActiveScorer(packageName);
        } catch (RemoteException e) {
            return false;
        }

#endif
}

ECode NetworkScoreManager::DisableScoring()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            mService.disableScoring();
        } catch (RemoteException e) {
        }

#endif
}

ECode NetworkScoreManager::RequestScores(
    /* [in] */ ArrayOf<INetworkKey*>* networks,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String activeScorer = getActiveScorerPackage();
        if (activeScorer == null) {
            return false;
        }
        Intent intent = new Intent(ACTION_SCORE_NETWORKS);
        intent.setPackage(activeScorer);
        intent.setFlags(Intent.FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT);
        intent.putExtra(EXTRA_NETWORKS_TO_SCORE, networks);
        // A scorer should never become active if its package doesn't hold SCORE_NETWORKS, but
        // ensure the package still holds it to be extra safe.
        mContext.sendBroadcastAsUser(intent, UserHandle.OWNER, Manifest.permission.SCORE_NETWORKS);
        return true;

#endif
}

ECode NetworkScoreManager::RegisterNetworkScoreCache(
    /* [in] */ Int32 networkType,
    /* [in] */ IINetworkScoreCache* scoreCache)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            mService.registerNetworkScoreCache(networkType, scoreCache);
        } catch (RemoteException e) {
        }

#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
