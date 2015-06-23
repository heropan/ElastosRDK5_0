#include "sax/CElement.h"

namespace Elastos {
namespace Droid {
namespace Sax {

ECode CElement::constructor(
    /* [in] */ IElement* parent,
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [in] */ Int32 depth)
{
    return Element::Init(parent, uri, localName, depth);
}

PInterface CElement::Probe(
    /* [in]  */ REIID riid)
{
    return _CElement::Probe(riid);
}

ECode CElement::GetChild(
    /* [in] */ const String& localName,
    /* [out] */ IElement** result)
{
    return Element::GetChild(localName, result);
}

ECode CElement::GetChild(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [out] */ IElement** result)
{
    return Element::GetChild(uri, localName, result);
}

ECode CElement::RequireChild(
    /* [in] */ const String& localName,
    /* [out] */ IElement** result)
{
    return Element::RequireChild(localName, result);
}

ECode CElement::RequireChild(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [out] */ IElement** result)
{
    return Element::RequireChild(uri, localName, result);
}

ECode CElement::SetElementListener(
    /* [in] */ IInterface* elementListener)
{
    return Element::SetElementListener(elementListener);
}

ECode CElement::SetTextElementListener(
    /* [in] */ IInterface* elementListener)
{
    return Element::SetTextElementListener(elementListener);
}

ECode CElement::SetStartElementListener(
    /* [in] */ IStartElementListener* startElementListener)
{
    return Element::SetStartElementListener(startElementListener);
}

ECode CElement::SetEndElementListener(
    /* [in] */ IEndElementListener* endElementListener)
{
    return Element::SetEndElementListener(endElementListener);
}

ECode CElement::SetEndTextElementListener(
    /* [in] */ IEndTextElementListener* endTextElementListener)
{
    return Element::SetEndTextElementListener(endTextElementListener);
}

ECode CElement::ToString(
    /* [out] */ String* str)
{
    return Element::ToString(str);
}

ECode CElement::ResetRequiredChildren()
{
    return Element::ResetRequiredChildren();
}

ECode CElement::CheckRequiredChildren(
    /* [in] */ ILocator* locator)
{
    return Element::CheckRequiredChildren(locator);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
