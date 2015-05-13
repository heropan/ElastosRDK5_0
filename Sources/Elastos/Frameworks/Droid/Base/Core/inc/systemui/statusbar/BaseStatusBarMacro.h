#ifndef __IBASESTATUSBAR_MACRO_H__
#define __IBASESTATUSBAR_MACRO_H__

using Elastos::Droid::Os::IBinder;
using Elastos::Droid::StatusBar::IIStatusBarService;
using Elastos::Droid::StatusBar::IStatusBarNotification;
using Elastos::Droid::SystemUI::ISystemUI;

#define IBASESTATUSBAR_METHODS_DECL()                                          \
CARAPI Start();                                                                 \
                                                                                \
CARAPI SetContext(                                                              \
    /* [in] */ IContext* context);                                              \
                                                                                \
CARAPI GetContext(                                                              \
    /* [out] */ IContext** context);                                            \
                                                                                \
CARAPI OnConfigurationChanged(                                                  \
    /* [in] */ IConfiguration* newConfig);                                      \
                                                                                \
CARAPI GetStatusBarService(                                                     \
    /* [out] */ IIStatusBarService** service);                                  \
                                                                                \
CARAPI IsDeviceProvisioned(                                                     \
    /* [out] */ Boolean* isDeviceProvisioned);                                  \
                                                                                \
CARAPI UserSwitched(                                                            \
    /* [in] */ Int32 newUserId);                                                \
                                                                                \
CARAPI NotificationIsForCurrentUser(                                            \
    /* [in] */ IStatusBarNotification* notification,                            \
    /* [out] */ Boolean* result);                                               \
                                                                                \
CARAPI DismissPopups();                                                         \
                                                                                \
CARAPI DismissIntruder();                                                       \
                                                                                \
CARAPI UpdateNotification(                                                      \
    /* [in] */ IBinder* keyBinder,                                              \
    /* [in] */ IStatusBarNotification* notification);                           \
                                                                                \
CARAPI InKeyguardRestrictedInputMode(                                           \
    /* [out] */ Boolean* result);                                               \
                                                                                \
CARAPI ShouldDisableNavbarGestures(                                             \
    /* [out] */ Boolean* result);

#define IBASESTATUSBAR_METHODS_IMPL(className, superClass)                     \
ECode className::Start()                                                        \
{                                                                               \
    return superClass::Start();                                                \
}                                                                               \
                                                                                \
ECode className::SetContext(                                                    \
    /* [in] */ IContext* context)                                               \
{                                                                               \
    return superClass::SetContext(context);                                    \
}                                                                               \
                                                                                \
ECode className::GetContext(                                                    \
    /* [out] */ IContext** context)                                             \
{                                                                               \
    VALIDATE_NOT_NULL(context);                                                 \
    AutoPtr<IContext> ctx = superClass::GetContext();                           \
    *context = ctx.Get();                                                       \
    INTERFACE_ADDREF(*context);                                                 \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::OnConfigurationChanged(                                        \
    /* [in] */ IConfiguration* newConfig)                                       \
{                                                                               \
    return superClass::OnConfigurationChanged(newConfig);                      \
}                                                                               \
                                                                                \
ECode className::GetStatusBarService(                                           \
    /* [out] */ IIStatusBarService** service)                                   \
{                                                                               \
    VALIDATE_NOT_NULL(service);                                                 \
    AutoPtr<IIStatusBarService> s = superClass::GetStatusBarService();          \
    *service = s.Get();                                                         \
    INTERFACE_ADDREF(*service);                                                 \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::IsDeviceProvisioned(                                           \
    /* [out] */ Boolean* isDeviceProvisioned)                                   \
{                                                                               \
    VALIDATE_NOT_NULL(isDeviceProvisioned);                                     \
    *isDeviceProvisioned = superClass::IsDeviceProvisioned();                   \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::UserSwitched(                                                  \
    /* [in] */ Int32 newUserId)                                                 \
{                                                                               \
    return superClass::UserSwitched(newUserId);                                \
}                                                                               \
                                                                                \
ECode className::NotificationIsForCurrentUser(                                  \
    /* [in] */ IStatusBarNotification* notification,                            \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    VALIDATE_NOT_NULL(result);                                                  \
    *result = superClass::NotificationIsForCurrentUser(notification);           \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::DismissPopups()                                                \
{                                                                               \
    return superClass::DismissPopups();                                        \
}                                                                               \
                                                                                \
ECode className::DismissIntruder()                                              \
{                                                                               \
    return superClass::DismissIntruder();                                      \
}                                                                               \
                                                                                \
ECode className::UpdateNotification(                                            \
    /* [in] */ IBinder* keyBinder,                                              \
    /* [in] */ IStatusBarNotification* notification)                            \
{                                                                               \
    return superClass::UpdateNotification(keyBinder, notification);            \
}                                                                               \
                                                                                \
ECode className::InKeyguardRestrictedInputMode(                                 \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    VALIDATE_NOT_NULL(result);                                                  \
    *result = superClass::InKeyguardRestrictedInputMode();                      \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::ShouldDisableNavbarGestures(                                   \
    /* [out] */ Boolean* result)                                                \
{                                                                               \
    VALIDATE_NOT_NULL(result);                                                  \
    *result = superClass::ShouldDisableNavbarGestures();                        \
    return NOERROR;                                                            \
}

#endif //__IBASESTATUSBAR_MACRO_H__
