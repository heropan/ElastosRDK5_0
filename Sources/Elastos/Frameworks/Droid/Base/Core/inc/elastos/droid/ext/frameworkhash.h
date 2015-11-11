
#ifndef __ELASTOS_DROID_EXT_FRAMEWORKHASH_H__
#define __ELASTOS_DROID_EXT_FRAMEWORKHASH_H__

#include <elastos/core/Object.h>

DEFINE_OBJECT_HASH_FUNC_FOR(IInterface)

//
// Os
//
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Os::IBinder)

//
// Accounts
//
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Accounts::IAccount)

//
// View
//
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::View::IView)

//
// Location
//
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::ICountryListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::IGpsStatusListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::IGpsStatusNmeaListener)
DEFINE_OBJECT_HASH_FUNC_FOR(Elastos::Droid::Location::ILocationListener)


#endif // __ELASTOS_DROID_EXT_FRAMEWORKHASH_H__


