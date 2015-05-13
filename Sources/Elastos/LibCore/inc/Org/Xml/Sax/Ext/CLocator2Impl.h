
#ifndef __CLOCATOR2IMPL_H__
#define __CLOCATOR2IMPL_H__

#include "_CLocator2Impl.h"
#include "LocatorImpl.h"
#include "CLocatorImpl.h"
#include "CLocator2Impl.h"

using Org::Xml::Sax::Helpers::LocatorImpl;

namespace Org {
namespace Xml {
namespace Sax {
namespace Ext {

CarClass(CLocator2Impl), public LocatorImpl
{
public:
    CARAPI SetPublicId(
        /* [in] */ const String& publicId);

    CARAPI SetSystemId(
        /* [in] */ const String& systemId);

    CARAPI SetLineNumber(
        /* [in] */ Int32 lineNumber);

    CARAPI SetColumnNumber(
        /* [in] */ Int32 columnNumber);

    CARAPI GetPublicId(
        /* [out] */ String* publicId);

    CARAPI GetSystemId(
        /* [out] */ String* systemId);

    CARAPI GetLineNumber(
        /* [out] */ Int32* lineNumber);

    CARAPI GetColumnNumber(
        /* [out] */ Int32* columnNumber);

    /**
     * Assigns the current value of the version property.
     *
     * @param version the new "version" value
     * @see #getXMLVersion
     */
    CARAPI SetXMLVersion(
        /* [in] */ const String& xmlVersion);

    /**
     * Assigns the current value of the encoding property.
     *
     * @param encoding the new "encoding" value
     * @see #getEncoding
     */
    CARAPI SetEncoding(
        /* [in] */ const String& encoding);

    /**
     * Returns the current value of the version property.
     *
     * @return the current value of the version property.
     *
     * @see #setXMLVersion
     */
    CARAPI GetXMLVersion(
        /* [out] */ String* xmlVersion);

    /**
     * Returns the current value of the encoding property.
     *
     * @return the current value of the encoding property.
     *
     * @see #setEncoding
     */
    CARAPI GetEncoding(
        /* [out] */ String* encoding);

    /**
     * Construct a new, empty Locator2Impl object.
     * This will not normally be useful, since the main purpose
     * of this class is to make a snapshot of an existing Locator.
     */
    CARAPI constructor();

    /**
     * Copy an existing Locator or Locator2 object.
     * If the object implements Locator2, values of the
     * <em>encoding</em> and <em>version</em>strings are copied,
     * otherwise they set to <em>null</em>.
     *
     * @param locator The existing Locator object.
     */
    CARAPI constructor(
        /* [in] */ ILocator* locator);

private:
    String mEncoding;
    String mVersion;
};

} // namespace Ext
} // namespace Sax
} // namespace Xml
} // namespace Org

#endif // __CLOCATOR2IMPL_H__
