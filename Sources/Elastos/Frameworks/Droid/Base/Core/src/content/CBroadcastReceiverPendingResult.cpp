
#include "content/CBroadcastReceiverPendingResult.h"

namespace Elastos {
namespace Droid {
namespace Content {

ECode CBroadcastReceiverPendingResult::SetResultCode(
    /* [in] */ Int32 code)
{
    return BroadcastReceiver::PendingResult::SetResultCode(code);
}

ECode CBroadcastReceiverPendingResult::GetResultCode(
    /* [out] */ Int32* resultCode)
{
    return BroadcastReceiver::PendingResult::GetResultCode(resultCode);
}

ECode CBroadcastReceiverPendingResult::SetResultData(
    /* [in] */ const String& data)
{
    return BroadcastReceiver::PendingResult::SetResultData(data);
}

ECode CBroadcastReceiverPendingResult::GetResultData(
    /* [out] */ String* resultData)
{
    return BroadcastReceiver::PendingResult::GetResultData(resultData);
}

ECode CBroadcastReceiverPendingResult::SetResultExtras(
    /* [in] */ IBundle* extras)
{
    return BroadcastReceiver::PendingResult::SetResultExtras(extras);
}

ECode CBroadcastReceiverPendingResult::GetResultExtras(
    /* [in] */ Boolean makeMap,
    /* [out] */ IBundle** resultExtras)
{
    return BroadcastReceiver::PendingResult::GetResultExtras(makeMap, resultExtras);
}

ECode CBroadcastReceiverPendingResult::SetResult(
    /* [in] */ Int32 code,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras)
{
    return BroadcastReceiver::PendingResult::SetResult(code, data, extras);
}

ECode CBroadcastReceiverPendingResult::GetAbortBroadcast(
    /* [out] */ Boolean* isAborted)
{
    return BroadcastReceiver::PendingResult::GetAbortBroadcast(isAborted);
}

ECode CBroadcastReceiverPendingResult::AbortBroadcast()
{
    return BroadcastReceiver::PendingResult::AbortBroadcast();
}

ECode CBroadcastReceiverPendingResult::ClearAbortBroadcast()
{
    return BroadcastReceiver::PendingResult::ClearAbortBroadcast();
}

ECode CBroadcastReceiverPendingResult::Finish()
{
    return BroadcastReceiver::PendingResult::Finish();
}

ECode CBroadcastReceiverPendingResult::SetExtrasClassLoader(
    /* [in] */ IClassLoader* cl)
{
    return BroadcastReceiver::PendingResult::SetExtrasClassLoader(cl);
}

ECode CBroadcastReceiverPendingResult::SendFinished(
    /* [in] */ IIActivityManager* am)
{
    return BroadcastReceiver::PendingResult::SendFinished(am);
}

ECode CBroadcastReceiverPendingResult::GetSendingUserId(
    /* [out] */ Int32* userId)
{
    return BroadcastReceiver::PendingResult::GetSendingUserId(userId);
}

ECode CBroadcastReceiverPendingResult::GetOrderedHint(
    /* [out] */ Boolean* orderedHint)
{
    return BroadcastReceiver::PendingResult::GetOrderedHint(orderedHint);
}

ECode CBroadcastReceiverPendingResult::GetInitialStickyHint(
    /* [out] */ Boolean* initialStickyHint)
{
    return BroadcastReceiver::PendingResult::GetInitialStickyHint(initialStickyHint);
}

ECode CBroadcastReceiverPendingResult::constructor(
    /* [in] */ Int32 resultCode,
    /* [in] */ const String& resultData,
    /* [in] */ IBundle* resultExtras,
    /* [in] */ Int32 type,
    /* [in] */ Boolean ordered,
    /* [in] */ Boolean sticky,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 userId)
{
    return BroadcastReceiver::PendingResult::Init(resultCode, resultData, resultExtras, type, ordered, sticky, token, userId);
}

}
}
}

