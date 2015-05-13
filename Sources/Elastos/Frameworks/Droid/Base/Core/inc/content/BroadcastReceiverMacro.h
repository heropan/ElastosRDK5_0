#ifndef __BROADCASTRECEIVERMACRO_H__
#define __BROADCASTRECEIVERMACRO_H__

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IBroadcastReceiverPendingResult;
using Elastos::Droid::Os::IBinder;

#define IBROADCASTRECEIVER_METHODS_DECL()                                                                               \
        CARAPI OnReceive(                                                                                        \
        /* [in] */ IContext* context,                                                                                   \
            /* [in] */ IIntent* intent);                                                                                \
                                                                                                                        \
        CARAPI CARAPI GoAsync(                                                                                          \
            /* [out] */ IBroadcastReceiverPendingResult** pendingResult);                                               \
                                                                                                                        \
        CARAPI PeekService(                                                                                             \
        /* [in] */ IContext* myContext,                                                                                 \
            /* [in] */ IIntent* service,                                                                                \
        /* [out] */ IBinder** binder);                                                                                  \
                                                                                                                        \
        CARAPI CARAPI SetResultCode(                                                                                    \
            /* [in] */ Int32 code);                                                                                     \
                                                                                                                        \
        CARAPI CARAPI GetResultCode(                                                                                    \
            /* [out] */ Int32* code);                                                                                   \
                                                                                                                        \
        CARAPI CARAPI SetResultData(                                                                                    \
            /* [in] */ const String& data);                                                                                    \
                                                                                                                        \
        CARAPI CARAPI GetResultData(                                                                                    \
            /* [out] */ String* data);                                                                                  \
                                                                                                                        \
        CARAPI CARAPI SetResultExtras(                                                                                  \
            /* [in] */ IBundle* extras);                                                                                \
                                                                                                                        \
        CARAPI CARAPI GetResultExtras(                                                                                  \
            /* [in] */ Boolean makeMap,                                                                                 \
            /* [out] */ IBundle** extras);                                                                              \
                                                                                                                        \
        CARAPI CARAPI SetResult(                                                                                        \
            /* [in] */ Int32 code,                                                                                      \
            /* [in] */ const String& data,                                                                                     \
            /* [in] */ IBundle* extras);                                                                                \
                                                                                                                        \
        CARAPI CARAPI GetAbortBroadcast(                                                                                \
            /* [out] */ Boolean* isAborted);                                                                            \
                                                                                                                        \
        CARAPI AbortBroadcast();                                                                                        \
                                                                                                                        \
        CARAPI ClearAbortBroadcast();                                                                                   \
                                                                                                                        \
        CARAPI CARAPI IsOrderedBroadcast(                                                                               \
            /* [out] */ Boolean* isOrdered);                                                                            \
                                                                                                                        \
        CARAPI CARAPI IsInitialStickyBroadcast(                                                                         \
            /* [out] */ Boolean* isInitial);                                                                            \
                                                                                                                        \
        CARAPI CARAPI SetOrderedHint(                                                                                   \
            /* [in] */ Boolean isOrdered);                                                                              \
                                                                                                                        \
        CARAPI CARAPI SetPendingResult(                                                                                 \
            /* [in] */ IBroadcastReceiverPendingResult* result);                                                        \
                                                                                                                        \
        CARAPI CARAPI GetPendingResult(                                                                                 \
            /* [out] */ IBroadcastReceiverPendingResult** pendingResult);                                               \
                                                                                                                        \
        CARAPI CARAPI GetSendingUserId(                                                                                 \
            /* [out] */ Int32* userId);                                                                                 \
                                                                                                                        \
        CARAPI CARAPI SetDebugUnregister(                                                                               \
            /* [in] */ Boolean debug);                                                                                  \
                                                                                                                        \
        CARAPI CARAPI GetDebugUnregister(                                                                               \
            /* [out] */ Boolean* debugUnregister);

#define IBROADCASTRECEIVER_METHODS_IMPL(className, superClass)                                                          \
ECode className::OnReceive(                                                                                      \
        /* [in] */ IContext* context,                                                                                   \
        /* [in] */ IIntent* intent)                                                                                     \
{                                                                                                                       \
    return superClass::OnReceive(context, intent);                                                               \
}                                                                                                                       \
                                                                                                                        \
ECode className::GoAsync(                                                                                               \
        /* [out] */ IBroadcastReceiverPendingResult** pendingResult)                                                    \
{                                                                                                                       \
    *pendingResult = superClass::GoAsync();                                                                             \
    if(*pendingResult)                                                                                                  \
        (*pendingResult)->AddRef();                                                                                     \
    return NOERROR;                                                                                                     \
}                                                                                                                       \
                                                                                                                        \
ECode className::PeekService(                                                                                           \
        /* [in] */ IContext* myContext,                                                                                 \
        /* [in] */ IIntent* service,                                                                                    \
        /* [out] */ IBinder** binder)                                                                                   \
{                                                                                                                       \
    *binder = superClass::PeekService(myContext, service);                                                              \
    if(*binder)                                                                                                         \
        (*binder)->AddRef();                                                                                            \
    return NOERROR;                                                                                                     \
}                                                                                                                       \
                                                                                                                        \
ECode className::SetResultCode(                                                                                         \
            /* [in] */ Int32 code)                                                                                      \
{                                                                                                                       \
    return superClass::SetResultCode(code);                                                                             \
}                                                                                                                       \
                                                                                                                        \
ECode className::GetResultCode(                                                                                         \
            /* [out] */ Int32* code)                                                                                    \
{                                                                                                                       \
    *code = superClass::GetResultCode();                                                                                \
    return NOERROR;                                                                                                     \
}                                                                                                                       \
                                                                                                                        \
ECode className::SetResultData(                                                                                         \
            /* [in] */ const String& data)                                                                                     \
{                                                                                                                       \
    return superClass::SetResultData();                                                                                 \
}                                                                                                                       \
                                                                                                                        \
ECode className::GetResultData(                                                                                         \
            /* [out] */ String* data)                                                                                   \
{                                                                                                                       \
    *data = superClass::GetResultData();                                                                                \
    return NOERROR;                                                                                                     \
}                                                                                                                       \
                                                                                                                        \
ECode className::SetResultExtras(                                                                                       \
            /* [in] */ IBundle* extras)                                                                                 \
{                                                                                                                       \
    return superClass::SetResultExtras(extras);                                                                         \
}                                                                                                                       \
                                                                                                                        \
ECode className::GetResultExtras(                                                                                       \
            /* [in] */ Boolean makeMap,                                                                                 \
            /* [out] */ IBundle** extras)                                                                               \
{                                                                                                                       \
    *extras = superClass::GetResultExtras();                                                                            \
    if(*extras)                                                                                                         \
        (*extras)->AddRef();                                                                                            \
    return NOERROR;                                                                                                     \
}                                                                                                                       \
                                                                                                                        \
ECode className::SetResult(                                                                                             \
            /* [in] */ Int32 code,                                                                                      \
            /* [in] */ const String& data,                                                                                     \
            /* [in] */ IBundle* extras)                                                                                 \
{                                                                                                                       \
    return superClass::SetResult(code, data, extras);                                                                   \
}                                                                                                                       \
                                                                                                                        \
ECode className::GetAbortBroadcast(                                                                                     \
            /* [out] */ Boolean* isAborted)                                                                             \
{                                                                                                                       \
    *isAborted = superClass::GetAbortBroadcast();                                                                       \
    return NOERROR;                                                                                                     \
}                                                                                                                       \
                                                                                                                        \
ECode className::AbortBroadcast()                                                                                       \
{                                                                                                                       \
    return superClass::AbortBroadcast();                                                                                \
}                                                                                                                       \
                                                                                                                        \
ECode className::ClearAbortBroadcast()                                                                                  \
{                                                                                                                       \
    return superClass::ClearAbortBroadcast();                                                                           \
}                                                                                                                       \
                                                                                                                        \
ECode className::IsOrderedBroadcast(                                                                                    \
            /* [out] */ Boolean* isOrdered)                                                                             \
{                                                                                                                       \
    *isOrdered = superClass::IsOrderedBroadcast();                                                                      \
    return NOERROR;                                                                                                     \
}                                                                                                                       \
                                                                                                                        \
ECode className::IsInitialStickyBroadcast(                                                                              \
            /* [out] */ Boolean* isInitial)                                                                             \
{                                                                                                                       \
    *isInitial = superClass::IsInitialStickyBroadcast();                                                                \
    return NOERROR;                                                                                                     \
}                                                                                                                       \
                                                                                                                        \
ECode className::SetOrderedHint(                                                                                        \
            /* [in] */ Boolean isOrdered)                                                                               \
{                                                                                                                       \
    return superClass::SetOrderedHint(isOrdered);                                                                       \
}                                                                                                                       \
                                                                                                                        \
ECode className::SetPendingResult(                                                                                      \
            /* [in] */ IBroadcastReceiverPendingResult* result)                                                         \
{                                                                                                                       \
    return superClass::SetPendingResult(result);                                                                        \
}                                                                                                                       \
                                                                                                                        \
ECode className::GetPendingResult(                                                                                      \
            /* [out] */ IBroadcastReceiverPendingResult** pendingResult)                                                \
{                                                                                                                       \
    *pendingResult = superClass::GetPendingResult();                                                                    \
    if(*pendingResult)                                                                                                  \
        (*pendingResult)->AddRef();                                                                                     \
    return NOERROR;                                                                                                     \
}                                                                                                                       \
                                                                                                                        \
ECode className::GetSendingUserId(                                                                                      \
            /* [out] */ Int32* userId)                                                                                  \
{                                                                                                                       \
    *userId = superClass::GetSendingUserId();                                                                           \
    return NOERROR;                                                                                                     \
}                                                                                                                       \
                                                                                                                        \
ECode className::SetDebugUnregister(                                                                                    \
            /* [in] */ Boolean debug)                                                                                   \
{                                                                                                                       \
    return superClass::SetDebugUnregister(debug);                                                                       \
}                                                                                                                       \
                                                                                                                        \
ECode className::GetDebugUnregister(                                                                                    \
            /* [out] */ Boolean* debugUnregister)                                                                       \
{                                                                                                                       \
    *debugUnregister = superClass::GetDebugUnregister();                                                                \
    return NOERROR;                                                                                                     \
}
#endif
