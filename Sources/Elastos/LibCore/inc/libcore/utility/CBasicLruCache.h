#ifndef __CBASICLRUCACHE_H__
#define __CBASICLRUCACHE_H__

#include "_Libcore_Utility_CBasicLruCache.h"
#include "BasicLruCache.h"

namespace Libcore {
namespace Utility {

CarClass(CBasicLruCache)
    , public BasicLruCache
{
public:
    CAR_OBJECT_DECL()
};

}
}

#endif