
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
#ifdef HASH_FOR_ACCOUNTS
#include "Elastos.Droid.Accounts.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Accounts::IAccount)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Accounts::IAccountAndUser)
#endif

//
// App
//
#ifdef HASH_FOR_APP
#include "Elastos.Droid.App.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::App::IActivity)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::App::IAppOpsManagerOnOpChangedListener)
#endif

//
// Content
//
#ifdef HASH_FOR_CONTENT
#include "Elastos.Droid.Content.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::IBroadcastReceiver)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::IComponentName)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::IContext)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::IIntentFilterComparison)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::IServiceConnection)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::ISharedPreferencesOnSharedPreferenceChangeListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Content::Pm::ISignature)
#endif

//
// Location
//
#ifdef HASH_FOR_LOCATION
#include "Elastos.Droid.Location.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::ICountryListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::IGpsStatusListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::IGpsStatusNmeaListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::ILocationListener)
#endif

//
// Os
//
#ifdef HASH_FOR_OS
#include "Elastos.Droid.Os.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Os::IBinder)
#endif

//
// View
//
#ifdef HASH_FOR_VIEW
#include "Elastos.Droid.View.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::View::IView)
#endif

//
// Widget
//
#ifdef HASH_FOR_WIDGET
#include "Elastos.Droid.Widget.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Widget::IGridLayoutSpec)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Widget::IInterval)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Widget::IRemoteViewsCacheKey)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Widget::IRemoteViewsFrameLayout)
#endif

//
//Text::Style
//
#ifdef HASH_FOR_TEXT_STYLE
#include "Elastos.Droid.Text.h"
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Text::Style::ISuggestionSpan)
#endif

#endif // __ELASTOS_DROID_EXT_FRAMEWORKHASH_H__


