
#include "cmdef.h"
#include <elastos/StringBuilder.h>
#include "AttributedCharacterIteratorAttribute.h"
#include "CAttributedCharacterIteratorAttribute.h"

using Elastos::Core::StringBuilder;
using Elastos::Text::CAttributedCharacterIteratorAttribute;

namespace Elastos {
namespace Text {

static AutoPtr<IAttributedCharacterIteratorAttribute> sInit(const String& name)
{
    AutoPtr<CAttributedCharacterIteratorAttribute> attribute;
    ASSERT_SUCCEEDED(CAttributedCharacterIteratorAttribute::NewByFriend(
            name, (CAttributedCharacterIteratorAttribute**)&attribute));
    return (IAttributedCharacterIteratorAttribute*)attribute.Get();
}

const AutoPtr<IAttributedCharacterIteratorAttribute> AttributedCharacterIteratorAttribute::INPUT_METHOD_SEGMENT =
        sInit(String("input_method_segment"));
const AutoPtr<IAttributedCharacterIteratorAttribute> AttributedCharacterIteratorAttribute::LANGUAGE =
        sInit(String("language"));
const AutoPtr<IAttributedCharacterIteratorAttribute> AttributedCharacterIteratorAttribute::READING =
        sInit(String("reading"));

ECode AttributedCharacterIteratorAttribute::Equals(
    /* [in] */ IInterface * obj,
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(value)
    *value = FALSE;
    VALIDATE_NOT_NULL(obj)

    IAttributedCharacterIteratorAttribute* o = IAttributedCharacterIteratorAttribute::Probe(obj);
    if (o != NULL) {
        *value = (o == THIS_PROBE(IAttributedCharacterIteratorAttribute));
    }
    return NOERROR;
}

ECode AttributedCharacterIteratorAttribute::GetHashCode(
    /* [out] */ Int32 * value)
{
    // return super.hashCode();
    VALIDATE_NOT_NULL(value)
    *value = 0 ;
    return NOERROR;
}

ECode AttributedCharacterIteratorAttribute::ToString(
    /* [out] */ String * str)
{
    // return getClass().getName() + '(' + getName() + ')';
    VALIDATE_NOT_NULL(str)
    return NOERROR;
}

ECode AttributedCharacterIteratorAttribute::Init(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode AttributedCharacterIteratorAttribute::GetName(
    /* [out] */ String * str)
{
    *str = mName;
    return NOERROR;
}

ECode AttributedCharacterIteratorAttribute::ReadResolve(
    /* [out] */ IInterface ** outface)
{
    /*
     * This class is used like Java enums, where all instances are
     * defined as fields of their own class. To preserve identity
     * equality, resolve to the canonical instance when deserialized.
     */
    /*
            try {
                for (Field field : getClass().getFields()) {
                    if (field.getType() == getClass() && Modifier.isStatic(field.getModifiers())) {
                        Attribute candidate = (Attribute) field.get(null);
                        if (name.equals(candidate.name)) {
                            return candidate;
                        }
                    }
                }
            } catch (IllegalAccessException e) {
            }
            throw new InvalidObjectException("Failed to resolve " + this);
    */
    return NOERROR;
/* former code:

    ClassID clsid;
    GetClassID(&clsid);
    if (clsid != ECLSID_CAttributedCharacterIteratorAttribute) {
    //    throw new InvalidObjectException("cannot resolve subclasses");
        return E_INVALID_OBJECT_EXCEPTION;
    }

    String name;
    INPUT_METHOD_SEGMENT->GetName(&name);
    if (mName.Equals(name)) {
        *resolve = INPUT_METHOD_SEGMENT;
        INTERFACE_ADDREF(*resolve);
        return NOERROR;
    }
    LANGUAGE->GetName(&name);
    if (mName.Equals(name)) {
        *resolve = LANGUAGE;
        INTERFACE_ADDREF(*resolve);
        return NOERROR;
    }
    READING->GetName(&name);
    if (mName.Equals(name)) {
        *resolve = READING;
        INTERFACE_ADDREF(*resolve);
        return NOERROR;
    }
    //throw new InvalidObjectException("Unknown attribute");
    return E_INVALID_OBJECT_EXCEPTION;
*/
}

} // namespace Text
} // namespace Elastos
