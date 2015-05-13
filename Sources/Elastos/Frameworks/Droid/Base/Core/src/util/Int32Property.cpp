#include "util/Int32Property.h"

using Elastos::Core::ECLSID_CInteger32;
using Elastos::Core::IInteger32;

namespace Elastos {
namespace Droid {
namespace Utility {

CAR_INTERFACE_IMPL_2(Int32Property, IInt32Property, IProperty)

Int32Property::Int32Property(
    /* [in] */ const String& name)
{
    mName = name;
    mClsId = ECLSID_CInteger32;
}

ECode Int32Property::IsReadOnly(
    /* [out] */ Boolean* readOnly)
{
    *readOnly = FALSE;
    return NOERROR;
}

ECode Int32Property::Set(
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* value)
{
    IInteger32* iobj = IInteger32::Probe(value);
    if (iobj == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 v;
    iobj->GetValue(&v);
    return SetValue(obj, v);
}

ECode Int32Property::GetName(
    /* [out] */ String* name)
{
    *name = mName;
    return NOERROR;
}

ECode Int32Property::GetType(
    /* [out] */ ClassID* id)
{
    *id = mClsId;
    return NOERROR;
}

ECode Int32Property::IsWriteOnly(
    /* [out] */ Boolean* writeOnly)
{
    *writeOnly = FALSE;
    return NOERROR;
}

ECode Int32Property::ForbiddenOperate(
    /* [out] */ Boolean* forbidden)
{
    *forbidden = FALSE;
    return NOERROR;
}

} // namespace Utility
} // namepsace Droid
} // namespace Elastos
