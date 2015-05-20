
#include "cmdef.h"
#include "CDefaultHandler.h"

namespace Org {
namespace Xml {
namespace Sax {
namespace Helpers {

ECode CDefaultHandler::ResolveEntity(
    /* [in] */ const String& publicId,
    /* [in] */ const String& systemId,
    /* [out] */ IInputSource** src)
{
    VALIDATE_NOT_NULL(src);

    *src = NULL;

    return NOERROR;
}

ECode CDefaultHandler::NotationDecl(
    /* [in] */ const String& name,
    /* [in] */ const String& publicId,
    /* [in] */ const String& systemId)
{
    return NOERROR;
}

ECode CDefaultHandler::UnparsedEntityDecl(
    /* [in] */ const String& name,
    /* [in] */ const String& publicId,
    /* [in] */ const String& systemId,
    /* [in] */ const String& notationName)
{
    return NOERROR;
}

ECode CDefaultHandler::SetDocumentLocator(
    /* [in] */ ILocator* locator)
{
    return NOERROR;
}

ECode CDefaultHandler::StartDocument()
{
    return NOERROR;
}

ECode CDefaultHandler::EndDocument()
{
    return NOERROR;
}

ECode CDefaultHandler::StartPrefixMapping(
    /* [in] */ const String& prefix,
    /* [in] */ const String& uri)
{
    return NOERROR;
}

ECode CDefaultHandler::EndPrefixMapping(
    /* [in] */ const String& prefix)
{
    return NOERROR;
}

ECode CDefaultHandler::StartElement(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [in] */ const String& qName,
    /* [in] */ IAttributes* atts)
{
    return NOERROR;
}

ECode CDefaultHandler::EndElement(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [in] */ const String& qName)
{
    return NOERROR;
}

ECode CDefaultHandler::Characters(
    /* [in] */ ArrayOf<Char32>* ch,
    /* [in] */ Int32 start,
    /* [in] */ Int32 length)
{
    return NOERROR;
}

ECode CDefaultHandler::IgnorableWhitespace(
    /* [out] */ ArrayOf<Char32>* ch,
    /* [in] */ Int32 start,
    /* [in] */ Int32 length)
{
    return NOERROR;
}

ECode CDefaultHandler::ProcessingInstruction(
    /* [in] */ const String& target,
    /* [in] */ const String& data)
{
    return NOERROR;
}

ECode CDefaultHandler::SkippedEntity(
    /* [in] */ const String& name)
{
    return NOERROR;
}

ECode CDefaultHandler::Warning(
    /* [in] */ ECode exception)
{
    return NOERROR;
}

ECode CDefaultHandler::Error(
    /* [in] */ ECode exception)
{
    return NOERROR;
}

ECode CDefaultHandler::FatalError(
    /* [in] */ ECode exception)
{
//    throw e;
    //TODO: return E_SAX_PARSE_EXCEPTION
    return NOERROR;
}

} // namespace Helpers
} // namespace Sax
} // namespace Xml
} // namespace Org
