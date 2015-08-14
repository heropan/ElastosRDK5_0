
#include "TypeConverter.h"

namespace Elastos {
namespace Droid {
namespace Animation {

CAR_INTERFACE_IMPL(TypeConverter, Object, ITypeConverter);

TypeConverter::TypeConverter()
{}

TypeConverter::TypeConverter(
    /* [in] */ IMethodInfo* fromClass,
    /* [in] */ IMethodInfo* toClass)
    : mFromClass(fromClass)
    , mToClass(toClass)
{
}

ECode TypeConverter::constructor(
    /* [in] */ IMethodInfo* fromClass,
    /* [in] */ IMethodInfo* toClass)
{
    mFromClass = fromClass;
    mToClass = toClass;
}

ECode TypeConverter::GetTargetType(
    /* [out] */ IMethodInfo** type)
{
    VALIDATE_NOT_NULL(type);
    *type = mToClass;
    REFCOUNT_ADD(*type);
    return NOERROR;
}

ECode TypeConverter::GetSourceType(
    /* [out] */ IMethodInfo** type)
{
    VALIDATE_NOT_NULL(type);
    *type = mFromClass;
    REFCOUNT_ADD(*type);
    return NOERROR;
}

} // namespace Animation
} // namepsace Droid
} // namespace Elastos

