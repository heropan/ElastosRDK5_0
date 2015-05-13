
#include "CLocator2Impl.h"

namespace Org {
namespace Xml {
namespace Sax {
namespace Ext {

ECode CLocator2Impl::SetPublicId(
    /* [in] */ const String& publicId)
{
    return LocatorImpl::SetPublicId(publicId);
}

ECode CLocator2Impl::SetSystemId(
    /* [in] */ const String& systemId)
{
    return LocatorImpl::SetSystemId(systemId);
}

ECode CLocator2Impl::SetLineNumber(
    /* [in] */ Int32 lineNumber)
{
    return LocatorImpl::SetLineNumber(lineNumber);
}

ECode CLocator2Impl::SetColumnNumber(
    /* [in] */ Int32 columnNumber)
{
    return LocatorImpl::SetColumnNumber(columnNumber);
}

ECode CLocator2Impl::GetPublicId(
    /* [out] */ String* publicId)
{
    return LocatorImpl::GetPublicId(publicId);
}

ECode CLocator2Impl::GetSystemId(
    /* [out] */ String* systemId)
{
    return LocatorImpl::GetSystemId(systemId);
}

ECode CLocator2Impl::GetLineNumber(
    /* [out] */ Int32* lineNumber)
{
    return LocatorImpl::GetLineNumber(lineNumber);
}

ECode CLocator2Impl::GetColumnNumber(
    /* [out] */ Int32* columnNumber)
{
    return LocatorImpl::GetColumnNumber(columnNumber);
}

ECode CLocator2Impl::SetXMLVersion(
    /* [in] */ const String& xmlVersion)
{
    this->mVersion = xmlVersion;
    return NOERROR;
}

ECode CLocator2Impl::SetEncoding(
    /* [in] */ const String& encoding)
{
    this->mEncoding = encoding;
    return NOERROR;
}

ECode CLocator2Impl::GetXMLVersion(
    /* [out] */ String* xmlVersion)
{
    VALIDATE_NOT_NULL(xmlVersion);

    *xmlVersion = mVersion;;

    return NOERROR;
}

ECode CLocator2Impl::GetEncoding(
    /* [out] */ String* encoding)
{
    VALIDATE_NOT_NULL(encoding);

    *encoding = mEncoding;

    return NOERROR;
}

ECode CLocator2Impl::constructor()
{
    return NOERROR;
}

ECode CLocator2Impl::constructor(
    /* [in] */ ILocator* locator)
{
    LocatorImpl::constructor(locator);

    ILocator2* locator2 = ILocator2::Probe(locator);

    if(locator2 != NULL){
        locator2->GetXMLVersion(&mVersion);
        locator2->GetEncoding(&mEncoding);
    }

    return NOERROR;
}

} // namespace Ext
} // namespace Sax
} // namespace Xml
} // namespace Org
