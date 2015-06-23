
#ifndef __CBROADCASTRECEIVERPENDINGRESULT_H__
#define __CBROADCASTRECEIVERPENDINGRESULT_H__

#include "_CBroadcastReceiverPendingResult.h"
#include <ext/frameworkext.h>
#include "content/BroadcastReceiver.h"

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CBroadcastReceiverPendingResult), public BroadcastReceiver::PendingResult
{
public:
    /**
     * Version of {@link BroadcastReceiver#setResultCode(int)
     * BroadcastReceiver.setResultCode(int)} for
     * asynchronous broadcast handling.
     */
    CARAPI SetResultCode(
        /* [in] */ Int32 code);

    /**
     * Version of {@link BroadcastReceiver#getResultCode()
     * BroadcastReceiver.getResultCode()} for
     * asynchronous broadcast handling.
     */
    CARAPI GetResultCode(
        /* [out] */ Int32* resultCode);

    /**
     * Version of {@link BroadcastReceiver#setResultData(String)
     * BroadcastReceiver.setResultData(String)} for
     * asynchronous broadcast handling.
     */
    CARAPI SetResultData(
        /* [in] */ const String& data);

    /**
     * Version of {@link BroadcastReceiver#getResultData()
     * BroadcastReceiver.getResultData()} for
     * asynchronous broadcast handling.
     */
    CARAPI GetResultData(
        /* [out] */ String* resultData);

    /**
     * Version of {@link BroadcastReceiver#setResultExtras(Bundle)
     * BroadcastReceiver.setResultExtras(Bundle)} for
     * asynchronous broadcast handling.
     */
    CARAPI SetResultExtras(
        /* [in] */ IBundle* extras);

    /**
     * Version of {@link BroadcastReceiver#getResultExtras(boolean)
     * BroadcastReceiver.getResultExtras(boolean)} for
     * asynchronous broadcast handling.
     */
    CARAPI GetResultExtras(
        /* [in] */ Boolean makeMap,
        /* [out] */ IBundle** resultExtras);

    /**
     * Version of {@link BroadcastReceiver#setResult(int, String, Bundle)
     * BroadcastReceiver.setResult(int, String, Bundle)} for
     * asynchronous broadcast handling.
     */
    CARAPI SetResult(
        /* [in] */ Int32 code,
        /* [in] */ const String& data,
        /* [in] */ IBundle* extras);

    /**
     * Version of {@link BroadcastReceiver#getAbortBroadcast()
     * BroadcastReceiver.getAbortBroadcast()} for
     * asynchronous broadcast handling.
     */
    CARAPI GetAbortBroadcast(
        /* [out] */ Boolean* isAborted);

    /**
     * Version of {@link BroadcastReceiver#abortBroadcast()
     * BroadcastReceiver.abortBroadcast()} for
     * asynchronous broadcast handling.
     */
    CARAPI AbortBroadcast();

    /**
     * Version of {@link BroadcastReceiver#clearAbortBroadcast()
     * BroadcastReceiver.clearAbortBroadcast()} for
     * asynchronous broadcast handling.
     */
    CARAPI ClearAbortBroadcast();

    /**
     * Finish the broadcast.  The current result will be sent and the
     * next broadcast will proceed.
     */
    CARAPI Finish();

    /** @hide */
    CARAPI SetExtrasClassLoader(
        /* [in] */ IClassLoader* cl);

    /** @hide */
    CARAPI SendFinished(
        /* [in] */ IIActivityManager* am);

    /** @hide */
    CARAPI GetSendingUserId(
        /* [out] */ Int32* userId);

    CARAPI GetOrderedHint(
        /* [out] */ Boolean* orderedHint);

    CARAPI GetInitialStickyHint(
        /* [out] */ Boolean* initialStickyHint);

    CARAPI constructor(
        /* [in] */ Int32 resultCode,
        /* [in] */ const String& resultData,
        /* [in] */ IBundle* resultExtras,
        /* [in] */ Int32 type,
        /* [in] */ Boolean ordered,
        /* [in] */ Boolean sticky,
        /* [in] */ IBinder* token,
        /* [in] */ Int32 userId);
};

}
}
}

#endif // __CBROADCASTRECEIVERPENDINGRESULT_H__
