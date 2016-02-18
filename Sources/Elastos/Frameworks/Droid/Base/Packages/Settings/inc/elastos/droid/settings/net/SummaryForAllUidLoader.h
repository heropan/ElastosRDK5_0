

package com.android.settings.net;

using Elastos::Droid::Content::IAsyncTaskLoader;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Net::IINetworkStatsSession;
using Elastos::Droid::Net::INetworkStats;
using Elastos::Droid::Net::INetworkTemplate;
using Elastos::Droid::Os::IBundle;

public class SummaryForAllUidLoader extends AsyncTaskLoader<NetworkStats> {
    private static const String KEY_TEMPLATE = "template";
    private static const String KEY_START = "start";
    private static const String KEY_END = "end";

    private final INetworkStatsSession mSession;
    private final Bundle mArgs;

    public static Bundle BuildArgs(NetworkTemplate template, Int64 start, Int64 end) {
        final Bundle args = new Bundle();
        args->PutParcelable(KEY_TEMPLATE, template);
        args->PutLong(KEY_START, start);
        args->PutLong(KEY_END, end);
        return args;
    }

    public SummaryForAllUidLoader(Context context, INetworkStatsSession session, Bundle args) {
        Super(context);
        mSession = session;
        mArgs = args;
    }

    //@Override
    protected void OnStartLoading() {
        super->OnStartLoading();
        ForceLoad();
    }

    //@Override
    public NetworkStats LoadInBackground() {
        final NetworkTemplate template = mArgs->GetParcelable(KEY_TEMPLATE);
        final Int64 start = mArgs->GetLong(KEY_START);
        final Int64 end = mArgs->GetLong(KEY_END);

        try {
            return mSession->GetSummaryForAllUid(template, start, end, FALSE);
        } catch (RemoteException e) {
            return NULL;
        }
    }

    //@Override
    protected void OnStopLoading() {
        super->OnStopLoading();
        CancelLoad();
    }

    //@Override
    protected void OnReset() {
        super->OnReset();
        CancelLoad();
    }
}
