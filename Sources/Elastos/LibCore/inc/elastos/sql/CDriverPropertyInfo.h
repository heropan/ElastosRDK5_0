
#ifndef __CDRIVERPROPERTYINFO_H__
#define __CDRIVERPROPERTYINFO_H__

#include "_CDriverPropertyInfo.h"

namespace Elastos {
namespace Sql {

CarClass(CDriverPropertyInfo)
{
public:
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
