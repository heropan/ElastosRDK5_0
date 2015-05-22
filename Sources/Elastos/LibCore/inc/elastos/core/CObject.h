#ifndef __COBJECT_H__
#define __COBJECT_H__

#include "_Elastos_Core_CObject.h"
#include "Object.h"

namespace Elastos {
namespace Core {

CarClass(CObject)
    , public Object
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Core
} // namespace Elastos

#endif //__COBJECT_H__