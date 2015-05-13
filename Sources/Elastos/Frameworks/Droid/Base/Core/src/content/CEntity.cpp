
#include "content/CEntity.h"
#include "content/CEntityNamedContentValues.h"

namespace Elastos {
namespace Droid {
namespace Content {

CEntity::CEntity()
{}

CEntity::~CEntity()
{
    mSubValues.Clear();
}

ECode CEntity::GetEntityValues(
    /* [out] */ IContentValues** entityValues)
{
    VALIDATE_NOT_NULL(entityValues)
    *entityValues = mValues;
    INTERFACE_ADDREF(*entityValues);
    return NOERROR;
}

ECode CEntity::GetSubValues(
    /* [out] */ IObjectContainer** subValues)
{
    VALIDATE_NOT_NULL(subValues)
    FAIL_RETURN(CObjectContainer::New(subValues));

    List<AutoPtr<IEntityNamedContentValues> >::Iterator it;
    for (it = mSubValues.Begin(); it != mSubValues.End(); it++) {
        FAIL_RETURN((*subValues)->Add(*it));
    }

    return NOERROR;
}

ECode CEntity::AddSubValue(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values)
{
    AutoPtr<IEntityNamedContentValues> entityValues;
    FAIL_RETURN(CEntityNamedContentValues::New(uri, values, (IEntityNamedContentValues**)&entityValues));
    mSubValues.PushBack(entityValues);
    return NOERROR;
}

ECode CEntity::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<IStringBuilder> sb = new StringBuilder();
    sb->AppendString(String("Entity: "));
    AutoPtr<IContentValues> values;
    FAIL_RETURN(GetEntityValues((IContentValues**)&values));
    sb->AppendObject(values);
    List<AutoPtr<IEntityNamedContentValues> >::Iterator it;
    AutoPtr<IEntityNamedContentValues> entityValues;
    AutoPtr<IUri> uri;
    AutoPtr<IContentValues> contentValues;

    for (it = mSubValues.Begin(); it != mSubValues.End(); it++) {
        sb->AppendString(String("\n  "));
        entityValues = *it;
        uri = NULL;
        FAIL_RETURN(entityValues->GetUri((IUri**)&uri));
        FAIL_RETURN(sb->AppendObject(uri));
        sb->AppendString(String("\n  -> "));
        contentValues = NULL;
        FAIL_RETURN(entityValues->GetValues((IContentValues**)&contentValues));
        FAIL_RETURN(sb->AppendObject(contentValues));
    }

    return sb->ToString(str);
}

ECode CEntity::constructor(
    /* [in] */ IContentValues* values)
{
    mValues = values;
    return NOERROR;
}

}
}
}

