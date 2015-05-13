
#include "cmdef.h"
#include "CLocatorImpl.h"

namespace Org {
namespace Xml {
namespace Sax {
namespace Helpers {

ECode CLocatorImpl::SetPublicId(
    /* [in] */ const String& publicId)
{
    return LocatorImpl::SetPublicId(publicId);
}

ECode CLocatorImpl::SetSystemId(
    /* [in] */ const String& systemId)
{
    return LocatorImpl::SetSystemId(systemId);
}

ECode CLocatorImpl::SetLineNumber(
    /* [in] */ Int32 lineNumber)
{
    return LocatorImpl::SetLineNumber(lineNumber);
}

ECode CLocatorImpl::SetColumnNumber(
    /* [in] */ Int32 columnNumber)
{
    return LocatorImpl::SetColumnNumber(columnNumber);
}

ECode CLocatorImpl::GetPublicId(
    /* [out] */ String* publicId)
{
    return LocatorImpl::GetPublicId(publicId);
}

ECode CLocatorImpl::GetSystemId(
    /* [out] */ String* systemId)
{
    return LocatorImpl::GetSystemId(systemId);
}

ECode CLocatorImpl::GetLineNumber(
    /* [out] */ Int32* lineNumber)
{
    return LocatorImpl::GetLineNumber(lineNumber);
}

ECode CLocatorImpl::GetColumnNumber(
    /* [out] */ Int32* columnNumber)
{
    return LocatorImpl::GetColumnNumber(columnNumber);
}

ECode CLocatorImpl::constructor()
{
    return LocatorImpl::constructor();
}

ECode CLocatorImpl::constructor(
    /* [in] */ ILocator* locator)
{
    return LocatorImpl::constructor(locator);
}

} // namespace Helpers
} // namespace Sax
} // namespace Xml
} // namespace Org
