
#include "CAttributesImpl.h"

namespace Org {
namespace Xml {
namespace Sax {
namespace Helpers {

ECode CAttributesImpl::GetLength(
    /* [out] */ Int32* length)
{
    return AttributesImpl::GetLength(length);
}

ECode CAttributesImpl::GetURI(
    /* [in] */ Int32 index,
    /* [out] */ String* URI)
{
    return AttributesImpl::GetURI(index, URI);
}

ECode CAttributesImpl::GetLocalName(
    /* [in] */ Int32 index,
    /* [out] */ String* localName)
{
    return AttributesImpl::GetLocalName(index, localName);
}

ECode CAttributesImpl::GetQName(
    /* [in] */ Int32 index,
    /* [out] */ String* qName)
{
    return AttributesImpl::GetQName(index, qName);
}

ECode CAttributesImpl::GetType(
    /* [in] */ Int32 index,
    /* [out] */ String* type)
{
    return GetType(index, type);
}

ECode CAttributesImpl::GetValue(
    /* [in] */ Int32 index,
    /* [out] */ String* value)
{
    return AttributesImpl::GetValue(index, value);
}

ECode CAttributesImpl::GetIndex(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [out] */ Int32* index)
{
    return AttributesImpl::GetIndex(uri, localName, index);
}

ECode CAttributesImpl::GetIndexEx(
    /* [in] */ const String& qName,
    /* [out] */ Int32* index)
{
    return AttributesImpl::GetIndexEx(qName, index);
}

ECode CAttributesImpl::GetTypeEx(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [out] */ String* type)
{
    return AttributesImpl::GetTypeEx(uri, localName, type);
}

ECode CAttributesImpl::GetTypeEx2(
    /* [in] */ const String& qName,
    /* [out] */ String* type)
{
    return CAttributesImpl::GetTypeEx2(qName, type);
}

ECode CAttributesImpl::GetValueEx(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [out] */ String* value)
{
    return AttributesImpl::GetValueEx(uri, localName, value);
}

ECode CAttributesImpl::GetValueEx2(
    /* [in] */ const String& qName,
    /* [out] */ String* value)
{
    return AttributesImpl::GetValueEx2(qName, value);
}

ECode CAttributesImpl::Clear()
{
    return AttributesImpl::Clear();
}

ECode CAttributesImpl::SetAttributes(
    /* [in] */ IAttributes* atts)
{
    return AttributesImpl::SetAttributes(atts);
}

ECode CAttributesImpl::AddAttribute(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [in] */ const String& qName,
    /* [in] */ const String& type,
    /* [in] */ const String& value)
{
    return AttributesImpl::AddAttribute(uri, localName, qName, type, value);
}

ECode CAttributesImpl::SetAttribute(
    /* [in] */ Int32 index,
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [in] */ const String& qName,
    /* [in] */ const String& type,
    /* [in] */ const String& value)
{
    return AttributesImpl::SetAttribute(index, uri, localName, qName, type, value);
}

ECode CAttributesImpl::RemoveAttribute(
    /* [in] */ Int32 index)
{
    return AttributesImpl::RemoveAttribute(index);
}

ECode CAttributesImpl::SetURI(
    /* [in] */ Int32 index,
    /* [in] */ const String& uri)
{
    return AttributesImpl::SetURI(index, uri);
}

ECode CAttributesImpl::SetLocalName(
    /* [in] */ Int32 index,
    /* [in] */ const String& localName)
{
    return AttributesImpl::SetLocalName(index, localName);
}

ECode CAttributesImpl::SetQName(
    /* [in] */ Int32 index,
    /* [in] */ const String& qName)
{
    return SetQName(index, qName);
}

ECode CAttributesImpl::SetType(
    /* [in] */ Int32 index,
    /* [in] */ const String& type)
{
    return AttributesImpl::SetType(index, type);
}

ECode CAttributesImpl::SetValue(
    /* [in] */ Int32 index,
    /* [in] */ const String& value)
{
    return AttributesImpl::SetValue(index, value);
}

ECode CAttributesImpl::constructor()
{
    return AttributesImpl::constructor();
}

ECode CAttributesImpl::constructor(
    /* [in] */ IAttributes* atts)
{
    return AttributesImpl::constructor(atts);
}

} // namespace Helpers
} // namespace Sax
} // namespace Xml
} // namespace Org
