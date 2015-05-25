
#ifndef  ___XMLPULL_V1_CDUMMY_H__
#define  ___XMLPULL_V1_CDUMMY_H__

#include "_Org_Xmlpull_V1_CDummy.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Org {
namespace Xmlpull {
namespace V1 {


/**
 * Dummy class
 */
CarClass(CDummy)
    , public Object
{
public:
    CAR_OBJECT_DECL()
};


} //namespace V1
} //namespace Xmlpull
} //namespace Org

#endif //__CKXMLPARSER_H__
