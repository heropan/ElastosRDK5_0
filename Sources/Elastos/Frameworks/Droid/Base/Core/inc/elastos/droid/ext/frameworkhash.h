
#ifndef __ELASTOS_DROID_EXT_FRAMEWORKHASH_H__
#define __ELASTOS_DROID_EXT_FRAMEWORKHASH_H__

#include <elastos/core/Object.h>

//
// Runtime
//
DEFINE_OBJECT_HASH_FUNC_FOR(IInterface)
DEFINE_OBJECT_HASH_FUNC_FOR(IWeakReference)

//
// Accounts
//
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Accounts::IAccount)

//
// App
//
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::App::IActivity)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::App::IAppOpsManagerOnOpChangedListener)

//
// Content
//
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::IBroadcastReceiver)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::IComponentName)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::IContext)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::IServiceConnection)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::ISharedPreferencesOnSharedPreferenceChangeListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::Pm::ISignature)

//
// Location
//
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::ICountryListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::IGpsStatusListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::IGpsStatusNmeaListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::ILocationListener)

//
// Os
//
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Os::IBinder)

//
// View
//
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::View::IView)

//
// Widget
//
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Widget::IGridLayoutSpec)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Widget::IInterval)

#endif // __ELASTOS_DROID_EXT_FRAMEWORKHASH_H__


