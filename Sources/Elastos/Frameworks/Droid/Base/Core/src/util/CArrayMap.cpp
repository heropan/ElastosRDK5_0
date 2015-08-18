#include "util/CArrayMap.h"

using Elastos::Utility::EIID_IMap;

namespace Elastos {
namespace Droid {
namespace Utility {

static AutoPtr<IArrayMap> InitEMPTY()
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

CAR_INTERFACE_IMPL_2(CArrayMap, Object, IArrayMap, IMap)

CAR_OBJECT_IMPL(CArrayMap)


} // Utility
} // Droid
} // Elastos