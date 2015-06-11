
#ifndef __CDRIVERPROPERTYINFO_H__
#define __CDRIVERPROPERTYINFO_H__

#include "_Elastos_Sql_CDriverPropertyInfo.h"
#include "Object.h"

using Elastos::Core::Object;

namespace Elastos {
namespace Sql {

CarClass(CDriverPropertyInfo)
    , public Object
    , public IDriverPropertyInfo
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ const String& value);

public:
    AutoPtr<ArrayOf<String> > choices;

    String description;

    String name;

    Boolean required;

    String value;
};

} // namespace Sql
} // namespace Elastos

#endif // __CDRIVERPROPERTYINFO_H__
