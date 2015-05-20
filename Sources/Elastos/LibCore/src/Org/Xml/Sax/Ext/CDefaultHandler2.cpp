
#include "cmdef.h"
#include "CDefaultHandler2.h"

namespace Org {
namespace Xml {
namespace Sax {
namespace Ext {

ECode CDefaultHandler2::NotationDecl(
    /* [in] */ const String& name,
    /* [in] */ const String& publicId,
    /* [in] */ const String& systemId)
{
    return NOERROR;
}

ECode CDefaultHandler2::UnparsedEntityDecl(
    /* [in] */ const String& name,
    /* [in] */ const String& publicId,
    /* [in] */ const String& systemId,
    /* [in] */ const String& notationName)
{
    return NOERROR;
}

ECode CDefaultHandler2::SetDocumentLocator(
    /* [in] */ ILocator* locator)
{
    return NOERROR;
}

ECode CDefaultHandler2::StartDocument()
{
    return NOERROR;
}

ECode CDefaultHandler2::EndDocument()
{
    return NOERROR;
}

ECode CDefaultHandler2::StartPrefixMapping(
    /* [in] */ const String& prefix,
    /* [in] */ const String& uri)
{
    return NOERROR;
}

ECode CDefaultHandler2::EndPrefixMapping(
    /* [in] */ const String& prefix)
{
    return NOERROR;
}

ECode CDefaultHandler2::StartElement(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [in] */ const String& qName,
    /* [in] */ IAttributes* atts)
{
    return NOERROR;
}

ECode CDefaultHandler2::EndElement(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [in] */ const String& qName)
{
    return NOERROR;
}

ECode CDefaultHandler2::Characters(
    /* [in] */ ArrayOf<Char32>* ch,
    /* [in] */ Int32 start,
    /* [in] */ Int32 length)
{
    return NOERROR;
}

ECode CDefaultHandler2::IgnorableWhitespace(
    /* [in] */ ArrayOf<Char32>* ch,
    /* [in] */ Int32 start,
    /* [in] */ Int32 length)
{
    return NOERROR;
}

ECode CDefaultHandler2::ProcessingInstruction(
    /* [in] */ const String& target,
    /* [in] */ const String& data)
{
    return NOERROR;
}

ECode CDefaultHandler2::SkippedEntity(
    /* [in] */ const String& name)
{
    return NOERROR;
}

ECode CDefaultHandler2::Warning(
    /* [in] */ ECode exception)
{
    return NOERROR;
}

ECode CDefaultHandler2::Error(
    /* [in] */ ECode exception)
{
    return NOERROR;
}

ECode CDefaultHandler2::FatalError(
    /* [in] */ ECode exception)
{
    //    throw e;
    //TODO: return E_SAX_PARSE_EXCEPTION
    return NOERROR;
}

ECode CDefaultHandler2::StartDTD(
    /* [in] */ const String& name,
    /* [in] */ const String& publicId,
    /* [in] */ const String& systemId)
{
    return NOERROR;
}

ECode CDefaultHandler2::EndDTD()
{
    return NOERROR;
}

ECode CDefaultHandler2::StartEntity(
    /* [in] */ const String& name)
{
    return NOERROR;
}

ECode CDefaultHandler2::EndEntity(
    /* [in] */ const String& name)
{
    return NOERROR;
}

ECode CDefaultHandler2::StartCDATA()
{
    return NOERROR;
}

ECode CDefaultHandler2::EndCDATA()
{
    return NOERROR;
}

ECode CDefaultHandler2::Comment(
    /* [out] */ ArrayOf<Char32>* ch,
    /* [in] */ Int32 start,
    /* [in] */ Int32 length)
{
    return NOERROR;
}

ECode CDefaultHandler2::ElementDecl(
    /* [in] */ const String& name,
    /* [in] */ const String& model)
{
    return NOERROR;
}

ECode CDefaultHandler2::AttributeDecl(
    /* [in] */ const String& eName,
    /* [in] */ const String& aName,
    /* [in] */ const String& type,
    /* [in] */ const String& mode,
    /* [in] */ const String& value)
{
    return NOERROR;
}

ECode CDefaultHandler2::InternalEntityDecl(
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    return NOERROR;
}

ECode CDefaultHandler2::ExternalEntityDecl(
    /* [in] */ const String& name,
    /* [in] */ const String& publicId,
    /* [in] */ const String& systemId)
{
    return NOERROR;
}

ECode CDefaultHandler2::ResolveEntity(
    /* [in] */ const String& publicId,
    /* [in] */ const String& systemId,
    /* [out] */ IInputSource** src)
{
    return ResolveEntity(String(NULL), publicId, String(NULL), systemId, src);
}

ECode CDefaultHandler2::GetExternalSubset(
    /* [in] */ const String& name,
    /* [in] */ const String& baseURI,
    /* [out] */ IInputSource** src)
{
    VALIDATE_NOT_NULL(src);

    *src = NULL;

    return NOERROR;
}

ECode CDefaultHandler2::ResolveEntity(
    /* [in] */ const String& name,
    /* [in] */ const String& publicId,
    /* [in] */ const String& baseURI,
    /* [in] */ const String& systemId,
    /* [out] */ IInputSource** src)
{
    VALIDATE_NOT_NULL(src);

    *src = NULL;

    return NOERROR;
}

} // namespace Ext
} // namespace Sax
} // namespace Xml
} // namespace Org
