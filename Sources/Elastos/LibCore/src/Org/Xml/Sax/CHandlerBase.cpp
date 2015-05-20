
#include "cmdef.h"
#include "CHandlerBase.h"

namespace Org {
namespace Xml {
namespace Sax {

ECode CHandlerBase::ResolveEntity(
    /* [in] */ const String& publicId,
    /* [in] */ const String& systemId,
    /* [out] */ IInputSource** src)
{
    VALIDATE_NOT_NULL(src);

    *src = NULL;

    return NOERROR;
}

ECode CHandlerBase::NotationDecl(
    /* [in] */ const String& name,
    /* [in] */ const String& publicId,
    /* [in] */ const String& systemId)
{
    return NOERROR;
}

ECode CHandlerBase::UnparsedEntityDecl(
    /* [in] */ const String& name,
    /* [in] */ const String& publicId,
    /* [in] */ const String& systemId,
    /* [in] */ const String& notationName)
{
    return NOERROR;
}

ECode CHandlerBase::SetDocumentLocator(
    /* [in] */ ILocator* locator)
{
    return NOERROR;
}

ECode CHandlerBase::StartDocument()
{
    return NOERROR;
}

ECode CHandlerBase::EndDocument()
{
    return NOERROR;
}

ECode CHandlerBase::StartElement(
    /* [in] */ const String& name,
    /* [in] */ IAttributeList* atts)
{
    return NOERROR;
}

ECode CHandlerBase::EndElement(
    /* [in] */ const String& name)
{
    return NOERROR;
}

ECode CHandlerBase::Characters(
    /* [in] */ ArrayOf<Char32>* ch,
    /* [in] */ Int32 start,
    /* [in] */ Int32 length)
{
    return NOERROR;
}

ECode CHandlerBase::IgnorableWhitespace(
    /* [in] */ ArrayOf<Char32>* ch,
    /* [in] */ Int32 start,
    /* [in] */ Int32 length)
{
    return NOERROR;
}

ECode CHandlerBase::ProcessingInstruction(
    /* [in] */ const String& target,
    /* [in] */ const String& data)
{
    return NOERROR;
}

ECode CHandlerBase::Warning(
    /* [in] */ ECode exception)
{
    return NOERROR;
}

ECode CHandlerBase::Error(
    /* [in] */ ECode exception)
{
    return NOERROR;
}

ECode CHandlerBase::FatalError(
    /* [in] */ ECode exception)
{
    //    throw e;
    //TODO: return E_SAX_PARSE_EXCEPTION
    return NOERROR;
}

} // namespace Sax
} // namespace Xml
} // namespace Org
