#include "util/FloatProperty.h"

using Elastos::Core::ECLSID_CFloat;
using Elastos::Core::IFloat;

namespace Elastos {
namespace Droid {
namespace Utility {

CAR_INTERFACE_IMPL_2(FloatProperty, IFloatProperty, IProperty)

FloatProperty::FloatProperty(
    /* [in] */ const String& name)
{
    mName = name;
    mClsId = ECLSID_CFloat;
}

ECode FloatProperty::IsReadOnly(
    /* [out] */ Boolean* readOnly)
{
    *readOnly = FALSE;
    return NOERROR;
}

ECode FloatProperty::Set(
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* value)
{
    if (IFloat::Probe(value) == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Float v;
    IFloat::Probe(value)->GetValue(&v);
    return SetValue(obj, v);
}

ECode FloatProperty::GetName(
    /* [out] */ String* name)
{
    *name = mName;
    return NOERROR;
}

ECode FloatProperty::GetType(
    /* [out] */ ClassID* id)
{
    *id = mClsId;
    return NOERROR;
}

ECode FloatProperty::IsWriteOnly(
    /* [out] */ Boolean* writeOnly)
{
    *writeOnly = FALSE;
    return NOERROR;
}

ECode FloatProperty::ForbiddenOperate(
    /* [out] */ Boolean* forbidden)
{
    *forbidden = FALSE;
    return NOERROR;
}

} // namespace Utility
} // namepsace Droid
} // namespace Elastos
