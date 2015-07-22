#ifndef __ELASTOSX_XML_VALIDATION_SCHEMA_H__
#define __ELASTOSX_XML_VALIDATION_SCHEMA_H__

#include "Object.h"

using Elastos::Core::Object;

namespace Elastosx {
namespace Xml {
namespace Validation {

class Schema
    : public Object
    , public ISchema
{
public:
    CAR_INTERFACE_DECL()

protected:
    Schema() {}
};

} // namespace Validation
} // namespace Xml
} // namespace Elastosx
#endif