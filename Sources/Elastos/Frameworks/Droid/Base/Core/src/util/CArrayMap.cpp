#include "util/CArrayMap.h"

using Elastos::Utility::EIID_IMap;

namespace Elastos {
namespace Droid {
namespace Utility {

AutoPtr<IArrayMap> InitEMPTY()
{
    AutoPtr<CArrayMap> map;
    // = new ArrayMap(true);
    return (IArrayMap*)map.Get();
}

const AutoPtr<IArrayMap> CArrayMap::EMPTY = InitEMPTY();
const Boolean CArrayMap::DEBUG = FALSE;
const String CArrayMap::TAG("ArrayMap");
const Int32 CArrayMap::BASE_SIZE = 4;
const Int32 CArrayMap::CACHE_SIZE = 10;

AutoPtr<ArrayOf<IInterface*> > CArrayMap::mBaseCache;
Int32 CArrayMap::mBaseCacheSize = 0;
AutoPtr<ArrayOf<IInterface*> > CArrayMap::mTwiceBaseCache;
Int32 CArrayMap::mTwiceBaseCacheSize = 0;

const AutoPtr<ArrayOf<Int32> > EMPTY_IMMUTABLE_INTS = ArrayOf<Int32>::Alloc(0);


CAR_INTERFACE_IMPL_2(CArrayMap, Object, IArrayMap, IMap)

CAR_OBJECT_IMPL(CArrayMap)


} // Utility
} // Droid
} // Elastos