
#ifndef __DROIDFRAMEWORKHASH_H__
#define __DROIDFRAMEWORKHASH_H__

// Hash functors definition macros
//
#ifndef DEFINE_HASH_FUNC_FOR_PTR_USING_ADDR
#define DEFINE_HASH_FUNC_FOR_PTR_USING_ADDR(TypeName) \
_ETL_NAMESPACE_BEGIN \
template<> struct Hash<TypeName *> \
{ \
    size_t operator()(const TypeName * s) const { return (size_t)s; }  \
}; \
_ETL_NAMESPACE_END
#endif

#ifndef DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR
#define DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(TypeName) \
_ETL_NAMESPACE_BEGIN \
template<> struct Hash<AutoPtr<TypeName> > \
{ \
    size_t operator()(const AutoPtr<TypeName> s) const { return (size_t)s.Get(); }  \
}; \
_ETL_NAMESPACE_END
#endif

#ifndef DEFINE_HASH_FUNC_FOR_PTR_USING_HASH_CODE
#define DEFINE_HASH_FUNC_FOR_PTR_USING_HASH_CODE(TypeName) \
_ETL_NAMESPACE_BEGIN \
template<> struct Hash<TypeName *> \
{ \
    size_t operator()(const TypeName * s) const \
    { \
        assert(s != NULL); \
        Int32 hashCode; \
        s->GetHashCode(&hashCode); \
        return (size_t)hashCode; \
    }  \
}; \
_ETL_NAMESPACE_END
#endif

#ifndef DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_HASH_CODE
#define DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_HASH_CODE(TypeName) \
_ETL_NAMESPACE_BEGIN \
template<> struct Hash<AutoPtr<TypeName> > \
{ \
    size_t operator()(const AutoPtr<TypeName> s) const \
    { \
        assert(s != NULL); \
        Int32 hashCode; \
        s->GetHashCode(&hashCode); \
        return (size_t)hashCode; \
    }  \
}; \
_ETL_NAMESPACE_END
#endif

#ifndef DEFINE_EQUALTO_FUNC_FOR_PTR
#define DEFINE_EQUALTO_FUNC_FOR_PTR(TypeName) \
_ETL_NAMESPACE_BEGIN \
template<> struct EqualTo<TypeName *> \
{ \
    size_t operator()(const TypeName * x, const TypeName * y) const \
    { \
        assert(x != NULL); \
        Boolean isEqual; \
        x->Equals(y, &isEqual); \
        return isEqual; \
    }  \
}; \
_ETL_NAMESPACE_END
#endif

#ifndef DEFINE_EQUALTO_FUNC_FOR_AUTOPTR
#define DEFINE_EQUALTO_FUNC_FOR_AUTOPTR(TypeName) \
_ETL_NAMESPACE_BEGIN \
template<> struct EqualTo<AutoPtr<TypeName> > \
{ \
    size_t operator()(const AutoPtr<TypeName> x, const AutoPtr<TypeName> y) const \
    { \
        assert(x != NULL); \
        Boolean isEqual; \
        x->Equals(y, &isEqual); \
        return isEqual; \
    }  \
}; \
_ETL_NAMESPACE_END
#endif


//==============================================================================
// Hash functor definitions for CAR interfaces.
//==============================================================================

#ifndef HASH_FUNC_FOR_AUTOPTR_USING_ADDR_IINTERFACE
#define HASH_FUNC_FOR_AUTOPTR_USING_ADDR_IINTERFACE
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(IInterface);
#endif

#ifndef HASH_EQUALTO_FUNC_FOR_AUTOPTR_IWEAKREFERENCE
#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_IWEAKREFERENCE
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(IWeakReference);
#endif

#ifndef HASH_EQUALTO_FUNC_FOR_AUTOPTR_ICLASSINFO
#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_ICLASSINFO
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(IClassInfo)
#endif

//
// Elastos::Core
//
#ifndef HASH_EQUALTO_FUNC_FOR_AUTOPTR_IINTEGER32
#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_IINTEGER32
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_HASH_CODE(Elastos::Core::IInteger32)
DEFINE_EQUALTO_FUNC_FOR_AUTOPTR(Elastos::Core::IInteger32)
#endif

#ifndef HASH_EQUALTO_FUNC_FOR_AUTOPTR_ICHARSEQUENCE
#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_ICHARSEQUENCE
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Core::ICharSequence)
#endif

//
// Elastos::Droid::Net
//
#ifndef HASH_EQUALTO_FUNC_FOR_AUTOPTR_IURI
#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_IURI
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_HASH_CODE(Elastos::Droid::Net::IUri)
DEFINE_EQUALTO_FUNC_FOR_AUTOPTR(Elastos::Droid::Net::IUri)
#endif

#ifndef HASH_EQUALTO_FUNC_FOR_AUTOPTR_INETWORKIDENTITY
#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_INETWORKIDENTITY
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_HASH_CODE(Elastos::Droid::Net::INetworkIdentity)
DEFINE_EQUALTO_FUNC_FOR_AUTOPTR(Elastos::Droid::Net::INetworkIdentity)
#endif

#ifndef HASH_EQUALTO_FUNC_FOR_AUTOPTR_INETWORKPOLICY
#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_INETWORKPOLICY
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_HASH_CODE(Elastos::Droid::Net::INetworkPolicy)
DEFINE_EQUALTO_FUNC_FOR_AUTOPTR(Elastos::Droid::Net::INetworkPolicy)
#endif

#ifndef HASH_EQUALTO_FUNC_FOR_AUTOPTR_INETWORKTEMPLATE
#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_INETWORKTEMPLATE
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_HASH_CODE(Elastos::Droid::Net::INetworkTemplate)
DEFINE_EQUALTO_FUNC_FOR_AUTOPTR(Elastos::Droid::Net::INetworkTemplate)
#endif

//
// Elastos::Droid::Os
//
#ifndef HASH_FUNC_FOR_AUTOPTR_IBINDER
#define HASH_FUNC_FOR_AUTOPTR_IBINDER
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Os::IBinder)
#endif

#ifndef HASH_EQUALTO_FUNC_FOR_AUTOPTR_IMESSENGER
#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_IMESSENGER
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_HASH_CODE(Elastos::Droid::Os::IMessenger)
DEFINE_EQUALTO_FUNC_FOR_AUTOPTR(Elastos::Droid::Os::IMessenger)
#endif

//
// Elastos::Droid::Hardware
//
#ifndef HASH_FUNC_FOR_AUTOPTR_ISENSORLISTENER
#define HASH_FUNC_FOR_AUTOPTR_ISENSORLISTENER
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Hardware::ISensorListener);
#endif

#ifndef HASH_FUNC_FOR_AUTOPTR_IUSBACCESSORY
#define HASH_FUNC_FOR_AUTOPTR_IUSBACCESSORY
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Hardware::Usb::IUsbAccessory);
#endif

//
// Elastos::Droid::InputMethodService
//
#ifndef HASH_FUNC_FOR_AUTOPTR_IKEYBOARDKEY
#define HASH_FUNC_FOR_AUTOPTR_IKEYBOARDKEY
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::InputMethodService::IKeyboardKey)
#endif

//
// Elastos::Droid::Content
//
#ifndef HASH_FUNC_FOR_AUTOPTR_ICONTEXT
#define HASH_FUNC_FOR_AUTOPTR_ICONTEXT
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Content::IContext)
#endif

#ifndef HASH_FUNC_FOR_AUTOPTR_IBROADCASTRECEIVER
#define HASH_FUNC_FOR_AUTOPTR_IBROADCASTRECEIVER
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Content::IBroadcastReceiver)
#endif

#ifndef HASH_FUNC_FOR_AUTOPTR_IINTENTRECEIVER
#define HASH_FUNC_FOR_AUTOPTR_IINTENTRECEIVER
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Content::IIntentReceiver);
#endif

#ifndef HASH_FUNC_FOR_AUTOPTR_ISERVICECONNECTION
#define HASH_FUNC_FOR_AUTOPTR_ISERVICECONNECTION
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Content::IServiceConnection)
#endif

#ifndef HASH_EQUALTO_FUNC_FOR_AUTOPTR_IINTENTFILTERCOMPARISON
#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_IINTENTFILTERCOMPARISON
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_HASH_CODE(Elastos::Droid::Content::IIntentFilterComparison)
DEFINE_EQUALTO_FUNC_FOR_AUTOPTR(Elastos::Droid::Content::IIntentFilterComparison)
#endif

#ifndef HASH_FUNC_FOR_AUTOPTR_ISHAREDPREFERENCESCHANGELISTENER
#define HASH_FUNC_FOR_AUTOPTR_ISHAREDPREFERENCESCHANGELISTENER
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Content::ISharedPreferencesOnSharedPreferenceChangeListener)
#endif

//
// Elastos::Droid::Content::Res
//
#ifndef HASH_FUNC_FOR_AUTOPTR_ICOMPATIBILITYINFO
#define HASH_FUNC_FOR_AUTOPTR_ICOMPATIBILITYINFO
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Content::Res::ICompatibilityInfo)
#endif

#ifndef HASH_EQUALTO_FUNC_FOR_AUTOPTR_ICOMPONENTNAME
#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_ICOMPONENTNAME
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_HASH_CODE(Elastos::Droid::Content::IComponentName)
DEFINE_EQUALTO_FUNC_FOR_AUTOPTR(Elastos::Droid::Content::IComponentName)
#endif

//
// Elastos::Droid::Content::Pm
//
#ifndef HASH_FUNC_FOR_AUTOPTR_ISIGNATURE
#define HASH_FUNC_FOR_AUTOPTR_ISIGNATURE
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Content::Pm::ISignature)
#endif

#ifndef HASH_FUNC_FOR_AUTOPTR_IAPPLICATIONINFO
#define HASH_FUNC_FOR_AUTOPTR_IAPPLICATIONINFO
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Content::Pm::IApplicationInfo)
#endif

//
// Elastos::Droid::App
//
#ifndef HASH_FUNC_FOR_AUTOPTR_IACTIVITY
#define HASH_FUNC_FOR_AUTOPTR_IACTIVITY
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::App::IActivity)
#endif

#ifndef HASH_FUNC_FOR_AUTOPTR_IPENDINGINTENT
#define HASH_FUNC_FOR_AUTOPTR_IPENDINGINTENT
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::App::IPendingIntent)
#endif

//
// Elastos::Droid::View
//
#ifndef HASH_FUNC_FOR_AUTOPTR_IVIEW
#define HASH_FUNC_FOR_AUTOPTR_IVIEW
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::View::IView)
#endif

#ifndef HASH_FUNC_FOR_AUTOPTR_IDISPLISTDISPFORMAT
#define HASH_FUNC_FOR_AUTOPTR_IDISPLISTDISPFORMAT
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::View::IDispListDispFormat)
#endif

#ifndef HASH_FUNC_FOR_AUTOPTR_IWINDOWSESSION
#define HASH_FUNC_FOR_AUTOPTR_IWINDOWSESSION
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::View::IWindowSession)
#endif

#ifndef HASH_FUNC_FOR_AUTOPTR_IACCESSIBILITYNODEINFO
#define HASH_FUNC_FOR_AUTOPTR_IACCESSIBILITYNODEINFO
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_HASH_CODE(Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo)
DEFINE_EQUALTO_FUNC_FOR_AUTOPTR(Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo)
#endif

//
// Elastos::Droid::View::InputMethod
//
#ifndef HASH_EQUALTO_FUNC_FOR_AUTOPTR_IINPUTMETHODINFO
#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_IINPUTMETHODINFO
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_HASH_CODE(Elastos::Droid::View::InputMethod::IInputMethodInfo)
DEFINE_EQUALTO_FUNC_FOR_AUTOPTR(Elastos::Droid::View::InputMethod::IInputMethodInfo)
#endif

//
// Elastos::Droid::Widget
//
#ifndef HASH_FUNC_FOR_AUTOPTR_ISPEC
#define HASH_FUNC_FOR_AUTOPTR_ISPEC
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Widget::ISpec)
#endif

//
// Elastos::Droid::Animation
//
#ifndef HASH_FUNC_FOR_AUTOPTR_IANIMATOR
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Animation::IAnimator)
#endif

//
// Elastos::Droid::Text::Style
//
#ifndef HASH_EQUALTO_FUNC_FOR_AUTOPTR_ISUGGESTIONSPAN
#define HASH_EQUALTO_FUNC_FOR_AUTOPTR_ISUGGESTIONSPAN
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_HASH_CODE(Elastos::Droid::Text::Style::ISuggestionSpan)
DEFINE_EQUALTO_FUNC_FOR_AUTOPTR(Elastos::Droid::Text::Style::ISuggestionSpan)
#endif

//
// Elastos::Droid::Accounts
//
#ifndef HASH_FUNC_FOR_AUTOPTR_IONACCOUNTSUPDATELISTENER
#define HASH_FUNC_FOR_AUTOPTR_IONACCOUNTSUPDATELISTENER
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Accounts::IOnAccountsUpdateListener);
#endif

#ifndef HASH_FUNC_FOR_AUTOPTR_IACCOUNT
#define HASH_FUNC_FOR_AUTOPTR_IACCOUNT
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Accounts::IAccount);
#endif

#ifndef HASH_FUNC_FOR_AUTOPTR_IAUTHENTICATORDESCRIPTION
#define HASH_FUNC_FOR_AUTOPTR_IAUTHENTICATORDESCRIPTION
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Accounts::IAuthenticatorDescription);
#endif

//
// Elastos::Droid::Location
//
#ifndef HASH_FUNC_FOR_AUTOPTR_IGPSSTATUSLISTENER
#define HASH_FUNC_FOR_AUTOPTR_IGPSSTATUSLISTENER
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Location::IGpsStatusListener);
#endif

#ifndef HASH_FUNC_FOR_AUTOPTR_IGPSSTATUSNMEALISTENER
#define HASH_FUNC_FOR_AUTOPTR_IGPSSTATUSNMEALISTENER
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Location::IGpsStatusNmeaListener);
#endif

#ifndef HASH_FUNC_FOR_AUTOPTR_ILOCATIONLISTENER
#define HASH_FUNC_FOR_AUTOPTR_ILOCATIONLISTENER
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(Elastos::Droid::Location::ILocationListener);
#endif

//
// Elastos::Droid::Net::Wifi
//
#ifndef HASH_FUNC_FOR_AUTOPTR_IWIFICONFIGURATION
#define HASH_FUNC_FOR_AUTOPTR_IWIFICONFIGURATION
DEFINE_HASH_FUNC_FOR_PTR_USING_ADDR(Elastos::Droid::Net::Wifi::IWifiConfiguration);
#endif

#endif // __DROIDFRAMEWORKHASH_H__


