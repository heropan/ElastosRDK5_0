
#include "ext/frameworkext.h"
#include "os/Handler.h"

using Elastos::Droid::Os::Handler;

#define IHANDLER_METHODS_DECL()                                                 \
    CARAPI RunWithScissors(                                                     \
        /* [in] */ IRunnable* r,                                                \
        /* [in] */ Int64 timeout,                                               \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI ObtainMessage(                                                       \
        /* [in] */ Int32 what,                                                  \
        /* [out] */ IMessage** msg);                                            \
                                                                                \
    CARAPI ObtainMessageEx(                                                     \
        /* [in] */ Int32 what,                                                  \
        /* [in] */ IInterface* obj,                                             \
        /* [out] */ IMessage** msg);                                            \
                                                                                \
    CARAPI ObtainMessageEx2(                                                    \
        /* [in] */ Int32 what,                                                  \
        /* [in] */ Int32 arg1,                                                  \
        /* [in] */ Int32 arg2,                                                  \
        /* [out] */ IMessage** msg);                                            \
                                                                                \
    CARAPI ObtainMessageEx3(                                                    \
        /* [in] */ Int32 what,                                                  \
        /* [in] */ Int32 arg1,                                                  \
        /* [in] */ Int32 arg2,                                                  \
        /* [in] */ IInterface* obj,                                             \
        /* [out] */ IMessage** msg);                                            \
                                                                                \
    CARAPI Post(                                                                \
        /* [in] */ IRunnable* runnable,                                         \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI PostAtTime(                                                          \
        /* [in] */ IRunnable* runnable,                                         \
        /* [in] */ Int64 uptimeMillis,                                          \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI PostAtTimeEx(                                                        \
        /* [in] */ IRunnable* runnable,                                         \
        /* [in] */ IInterface* token,                                           \
        /* [in] */ Int64 uptimeMillis,                                          \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI PostDelayed(                                                         \
        /* [in] */ IRunnable* runnable,                                         \
        /* [in] */ Int64 delayMillis,                                           \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI PostAtFrontOfQueue(                                                  \
        /* [in] */ IRunnable* runnable,                                         \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI RemoveCallbacks(                                                     \
        /* [in] */ IRunnable* action);                                          \
                                                                                \
    CARAPI RemoveCallbacksEx(                                                   \
        /* [in] */ IRunnable* action,                                           \
        /* [in] */ IInterface* token);                                          \
                                                                                \
    CARAPI GetLooper(                                                           \
        /* [out] */ ILooper** looper);                                          \
                                                                                \
    CARAPI GetMessageQueue(                                                     \
        /* [out] */ IMessageQueue** cq);                                        \
                                                                                \
    CARAPI GetIMessenger(                                                       \
        /* [out] */ IIMessenger** mgr);                                         \
                                                                                \
    CARAPI SendEmptyMessage(                                                    \
        /* [in] */ Int32 what,                                                  \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI SendEmptyMessageAtTime(                                              \
        /* [in] */ Int32 what,                                                  \
        /* [in] */ Int64 delayMillis,                                           \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI SendEmptyMessageDelayed(                                             \
        /* [in] */ Int32 what,                                                  \
        /* [in] */ Int64 delayMillis,                                           \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI SendMessage(                                                         \
        /* [in] */ IMessage* msg,                                               \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI SendMessageAtTime(                                                   \
        /* [in] */ IMessage* msg,                                               \
        /* [in] */ Int64 delayMillis,                                           \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI SendMessageDelayed(                                                  \
        /* [in] */ IMessage* msg,                                               \
        /* [in] */ Int64 delayMillis,                                           \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI SendMessageAtFrontOfQueue(                                           \
        /* [in] */ IMessage* msg,                                               \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI HasCallbacks(                                                        \
        /* [in] */ IRunnable* action,                                           \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI HasMessages(                                                         \
        /* [in] */ Int32 what,                                                  \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI HasMessagesEx(                                                       \
        /* [in] */ Int32 what,                                                  \
        /* [in] */ IInterface* obj,                                             \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI RemoveMessages(                                                      \
        /* [in] */ Int32 what);                                                 \
                                                                                \
    CARAPI RemoveMessagesEx(                                                    \
        /* [in] */ Int32 what,                                                  \
        /* [in] */ IInterface* obj);                                            \
                                                                                \
    CARAPI RemoveCallbacksAndMessages(                                          \
        /* [in] */ IInterface* obj);                                            \
                                                                                \
    CARAPI DispatchMessage(                                                     \
        /* [in] */ IMessage* msg);                                              \
                                                                                \
    CARAPI GetMessageName(                                                      \
        /* [in] */ IMessage* msg,                                               \
        /* [out] */ String* name);

#define IHANDLER_METHODS_IMPL(className, superClass)                            \
ECode className::RunWithScissors(                                               \
    /* [in] */ IRunnable* r,                                                    \
    /* [in] */ Int64 timeout,                                                   \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::RunWithScissors(r, timeout, result);                     \
}                                                                               \
                                                                                \
ECode className::ObtainMessage(                                                 \
    /* [in] */ Int32 what,                                                      \
    /* [out] */ IMessage** msg)                                                 \
{                                                                               \
    return superClass::ObtainMessageEx3(what, 0, 0, NULL, msg);                 \
}                                                                               \
                                                                                \
ECode className::ObtainMessageEx(                                               \
    /* [in] */ Int32 what,                                                      \
    /* [in] */ IInterface* obj,                                                 \
    /* [out] */ IMessage** msg)                                                 \
{                                                                               \
    return superClass::ObtainMessageEx3(what, 0, 0, obj, msg);                  \
}                                                                               \
                                                                                \
ECode className::ObtainMessageEx2(                                              \
    /* [in] */ Int32 what,                                                      \
    /* [in] */ Int32 arg1,                                                      \
    /* [in] */ Int32 arg2,                                                      \
    /* [out] */ IMessage** msg)                                                 \
{                                                                               \
    return superClass::ObtainMessageEx3(what, arg1, arg2, NULL, msg);           \
}                                                                               \
                                                                                \
ECode className::ObtainMessageEx3(                                              \
    /* [in] */ Int32 what,                                                      \
    /* [in] */ Int32 arg1,                                                      \
    /* [in] */ Int32 arg2,                                                      \
    /* [in] */ IInterface* obj,                                                 \
    /* [out] */ IMessage** msg)                                                 \
{                                                                               \
    return superClass::ObtainMessageEx3(what, arg1, arg2, obj, msg);            \
}                                                                               \
                                                                                \
ECode className::Post(                                                          \
    /* [in] */ IRunnable* runnable,                                             \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::Post(runnable, result);                                  \
}                                                                               \
                                                                                \
ECode className::PostAtTime(                                                    \
    /* [in] */ IRunnable* runnable,                                             \
    /* [in] */ Int64 uptimeMillis,                                              \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::PostAtTime(runnable, uptimeMillis, result);              \
}                                                                               \
                                                                                \
ECode className::PostAtTimeEx(                                                  \
    /* [in] */ IRunnable* runnable,                                             \
    /* [in] */ IInterface* token,                                               \
    /* [in] */ Int64 uptimeMillis,                                              \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::PostAtTimeEx(runnable, token, uptimeMillis, result);     \
}                                                                               \
                                                                                \
ECode className::PostDelayed(                                                   \
    /* [in] */ IRunnable* runnable,                                             \
    /* [in] */ Int64 delayMillis,                                               \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::PostDelayed(runnable, delayMillis, result);              \
}                                                                               \
                                                                                \
ECode className::PostAtFrontOfQueue(                                            \
    /* [in] */ IRunnable* runnable,                                             \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::PostAtFrontOfQueue(runnable, result);                    \
}                                                                               \
                                                                                \
ECode className::RemoveCallbacks(                                               \
    /* [in] */ IRunnable* runnable)                                             \
{                                                                               \
    return superClass::RemoveCallbacks(runnable);                               \
}                                                                               \
                                                                                \
ECode className::RemoveCallbacksEx(                                             \
    /* [in] */ IRunnable* runnable,                                             \
    /* [in] */ IInterface* token)                                               \
{                                                                               \
    return superClass::RemoveCallbacksEx(runnable, token);                      \
}                                                                               \
                                                                                \
ECode className::GetLooper(                                                     \
    /* [out] */ ILooper** looper)                                               \
{                                                                               \
    VALIDATE_NOT_NULL(looper);                                                  \
    *looper = mLooper.Get();                                                    \
    if (*looper)                                                                \
        (*looper)->AddRef();                                                    \
                                                                                \
    return NOERROR;                                                             \
}                                                                               \
                                                                                \
ECode className::GetMessageQueue(                                              \
    /* [out] */ IMessageQueue** cq)                                            \
{                                                                               \
    VALIDATE_NOT_NULL(cq);                                                      \
    *cq = mQueue.Get();                                                         \
    if (*cq)                                                                    \
        (*cq)->AddRef();                                                        \
                                                                                \
    return NOERROR;                                                             \
}                                                                               \
                                                                                \
ECode className::GetIMessenger(                                                 \
    /* [out] */ IIMessenger** mgr)                                              \
{                                                                               \
    VALIDATE_NOT_NULL(mgr);                                                     \
    return superClass::GetIMessenger(mgr);                                      \
}                                                                               \
                                                                                \
ECode className::SendEmptyMessage(                                              \
    /* [in] */ Int32 what,                                                      \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::SendEmptyMessage(what, result);                          \
}                                                                               \
                                                                                \
ECode className::SendEmptyMessageAtTime(                                        \
    /* [in] */ Int32 what,                                                      \
    /* [in] */ Int64 uptimeMillis,                                              \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::SendEmptyMessageAtTime(what, uptimeMillis, result);      \
}                                                                               \
                                                                                \
ECode className::SendEmptyMessageDelayed(                                       \
    /* [in] */ Int32 what,                                                      \
    /* [in] */ Int64 delayMillis,                                               \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::SendEmptyMessageDelayed(what, delayMillis, result);      \
}                                                                               \
                                                                                \
ECode className::SendMessage(                                                   \
    /* [in] */ IMessage* msg,                                                   \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::SendMessage(msg, result);                                \
}                                                                               \
                                                                                \
ECode className::SendMessageAtTime(                                             \
    /* [in] */ IMessage* msg,                                                   \
    /* [in] */ Int64 uptimeMillis,                                              \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::SendMessageAtTime(msg, uptimeMillis, result);            \
}                                                                               \
                                                                                \
ECode className::SendMessageDelayed(                                            \
    /* [in] */ IMessage* msg,                                                   \
    /* [in] */ Int64 delayMillis,                                               \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::SendMessageDelayed(msg, delayMillis, result);            \
}                                                                               \
                                                                                \
ECode className::SendMessageAtFrontOfQueue(                                     \
    /* [in] */ IMessage* msg,                                                   \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::SendMessageAtFrontOfQueue(msg, result);                  \
}                                                                               \
                                                                                \
ECode className::HasCallbacks(                                                  \
    /* [in] */ IRunnable* action,                                               \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::HasCallbacks(action, result);                            \
}                                                                               \
                                                                                \
ECode className::HasMessages(                                                   \
    /* [in] */ Int32 what,                                                      \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::HasMessages(what, result);                               \
}                                                                               \
                                                                                \
ECode className::HasMessagesEx(                                                 \
    /* [in] */ Int32 what,                                                      \
    /* [in] */ IInterface* obj,                                                 \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    return superClass::HasMessagesEx(what, obj, result);                        \
}                                                                               \
                                                                                \
ECode className::RemoveMessages(                                                \
    /* [in] */ Int32 what)                                                      \
{                                                                               \
    return superClass::RemoveMessages(what);                                    \
}                                                                               \
                                                                                \
ECode className::RemoveMessagesEx(                                              \
    /* [in] */ Int32 what,                                                      \
    /* [in] */ IInterface* obj)                                                 \
{                                                                               \
    return superClass::RemoveMessagesEx(what, obj);                             \
}                                                                               \
                                                                                \
ECode className::RemoveCallbacksAndMessages(                                    \
    /* [in] */ IInterface* obj)                                                 \
{                                                                               \
    return superClass::RemoveCallbacksAndMessages(obj);                         \
}                                                                               \
                                                                                \
ECode className::DispatchMessage(                                               \
    /* [in] */ IMessage* msg)                                                   \
{                                                                               \
    return superClass::DispatchMessage(msg);                                    \
}                                                                               \
                                                                                \
ECode className::GetMessageName(                                                \
    /* [in] */ IMessage* msg,                                                   \
    /* [out] */ String* name)                                                   \
{                                                                               \
    return superClass::GetMessageName(msg, name);                               \
}
