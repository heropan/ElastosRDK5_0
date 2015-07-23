
#include "content/CPendingResult.h"

namespace Elastos {
namespace Droid {
namespace Content {

ECode CPendingResult::SetResultCode(
    /* [in] */ Int32 code)
{
    return BroadcastReceiver::PendingResult::SetResultCode(code);
}

ECode CPendingResult::GetResultCode(
    /* [out] */ Int32* resultCode)
{
    return BroadcastReceiver::PendingResult::GetResultCode(resultCode);
}

ECode CPendingResult::SetResultData(
    /* [in] */ const String& data)
{
    return BroadcastReceiver::PendingResult::SetResultData(data);
}

ECode CPendingResult::GetResultData(
    /* [out] */ String* resultData)
{
    return BroadcastReceiver::PendingResult::GetResultData(resultData);
}

ECode CPendingResult::SetResultExtras(
    /* [in] */ IBundle* extras)
{
    return BroadcastReceiver::PendingResult::SetResultExtras(extras);
}

ECode CPendingResult::GetResultExtras(
    /* [in] */ Boolean makeMap,
    /* [out] */ IBundle** resultExtras)
{
    return BroadcastReceiver::PendingResult::GetResultExtras(makeMap, resultExtras);
}

ECode CPendingResult::SetResult(
    /* [in] */ Int32 code,
    /* [in] */ const String& data,
    /* [in] */ IBundle* extras)
{
    return BroadcastReceiver::PendingResult::SetResult(code, data, extras);
}

ECode CPendingResult::GetAbortBroadcast(
    /* [out] */ Boolean* isAborted)
{
    return BroadcastReceiver::PendingResult::GetAbortBroadcast(isAborted);
}

ECode CPendingResult::AbortBroadcast()
{
    return BroadcastReceiver::PendingResult::AbortBroadcast();
}

ECode CPendingResult::ClearAbortBroadcast()
{
    return BroadcastReceiver::PendingResult::ClearAbortBroadcast();
}

ECode CPendingResult::Finish()
{
    return BroadcastReceiver::PendingResult::Finish();
}

ECode CPendingResult::SetExtrasClassLoader(
    /* [in] */ IClassLoader* cl)
{
    return BroadcastReceiver::PendingResult::SetExtrasClassLoader(cl);
}

ECode CPendingResult::SendFinished(
    /* [in] */ IIActivityManager* am)
{
    return BroadcastReceiver::PendingResult::SendFinished(am);
}

ECode CPendingResult::GetSendingUserId(
    /* [out] */ Int32* userId)
{
    return BroadcastReceiver::PendingResult::GetSendingUserId(userId);
}

ECode CPendingResult::GetOrderedHint(
    /* [out] */ Boolean* orderedHint)
{
    return BroadcastReceiver::PendingResult::GetOrderedHint(orderedHint);
}

ECode CPendingResult::GetInitialStickyHint(
    /* [out] */ Boolean* initialStickyHint)
{
    return BroadcastReceiver::PendingResult::GetInitialStickyHint(initialStickyHint);
}

ECode CPendingResult::constructor(
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

