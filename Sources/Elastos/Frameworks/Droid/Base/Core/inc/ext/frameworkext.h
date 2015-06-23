
#ifndef __DROIDFRAMEWORKEXT_H__
#define __DROIDFRAMEWORKEXT_H__

#ifndef __ELASTOS_H__
#include <elastos.h>
using namespace Elastos;
#endif

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif

extern "C" const Elastos::ClassID ECLSID_CDummyObject;

#ifndef __ETL_HASH_FUN_H__
#include <elastos/utility/etl/etl_hash_fun.h>
#endif

#ifndef __ETL_FUNCTION_H__
#include <elastos/utility/etl/etl_function.h>
#endif

#ifndef __DROIDFRAMEWORKDEF_H__
#include "ext/frameworkdef.h"
#endif

#ifndef __DROIDFRAMEWORKHASH_H__
#include "ext/frameworkhash.h"
#endif

#endif //__FRAMEWORKEXT_H__
