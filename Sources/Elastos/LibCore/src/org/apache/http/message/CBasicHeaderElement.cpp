
#include "CBasicHeaderElement.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Message {

CAR_OBJECT_IMPL(CBasicHeaderElement)

ECode CBasicHeaderElement::Clone(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj)
    AutoPtr<CHeaderElement> element;
    CBasicHeaderElement::NewByFriend((CHeaderElement**)&element);
    CloneImpl((BasicHeaderElement*)element);
    *obj = element->Probe(EIID_IInterface);
    REFCOUNT_ADD(*obj)
    return NOERROR;
}

ECode CBasicHeaderElement::constructor(
    /* [in] */ cosnt String& name,
    /* [in] */ cosnt String& value,
    /* [in] */ ArrayOf<INameValuePair*>* parameters)
{
    return Init(name, value, parameters);
}

ECode CBasicHeaderElement::constructor(
    /* [in] */ cosnt String& name,
    /* [in] */ cosnt String& value)
{
    return constructor(name, value, NULL);
}

ECode CBasicHeaderElement::constructor()
{
    return NOERROR;
}

} // namespace Message
} // namespace Http
} // namespace Apache
} // namespace Org
