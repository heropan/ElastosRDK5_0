
#ifndef __CATTRIBUTES2IMPL_H__
#define __CATTRIBUTES2IMPL_H__

#include "_CAttributes2Impl.h"
#include "AttributesImpl.h"

using Org::Xml::Sax::Helpers::AttributesImpl;

namespace Org {
namespace Xml {
namespace Sax {
namespace Ext {

/**
 * SAX2 extension helper for additional Attributes information,
 * implementing the {@link Attributes2} interface.
 *
 * <blockquote>
 * <em>This module, both source code and documentation, is in the
 * Public Domain, and comes with <strong>NO WARRANTY</strong>.</em>
 * </blockquote>
 *
 * <p>This is not part of core-only SAX2 distributions.</p>
 *
 * <p>The <em>specified</em> flag for each attribute will always
 * be true, unless it has been set to false in the copy constructor
 * or using {@link #setSpecified}.
 * Similarly, the <em>declared</em> flag for each attribute will
 * always be false, except for defaulted attributes (<em>specified</em>
 * is false), non-CDATA attributes, or when it is set to true using
 * {@link #setDeclared}.
 * If you change an attribute's type by hand, you may need to modify
 * its <em>declared</em> flag to match.
 * </p>
 *
 * @since SAX 2.0 (extensions 1.1 alpha)
 * @author David Brownell
 * @version TBS
 */
CarClass(CAttributes2Impl), public AttributesImpl
{
public:
    CARAPI GetLength(
        /* [out] */ Int32* length);

    CARAPI GetURI(
        /* [in] */ Int32 index,
        /* [out] */ String* URI);

    CARAPI GetLocalName(
        /* [in] */ Int32 index,
        /* [out] */ String* localName);

    CARAPI GetQName(
        /* [in] */ Int32 index,
        /* [out] */ String* qName);

    CARAPI GetType(
        /* [in] */ Int32 index,
        /* [out] */ String* type);

    CARAPI GetValue(
        /* [in] */ Int32 index,
        /* [out] */ String* value);

    CARAPI GetIndex(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ Int32* index);

    CARAPI GetIndexEx(
        /* [in] */ const String& name,
        /* [out] */ Int32* index);

    CARAPI GetTypeEx(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ String* type);

    CARAPI GetTypeEx2(
        /* [in] */ const String& name,
        /* [out] */ String* type);

    CARAPI GetValueEx(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ String* value);

    CARAPI GetValueEx2(
        /* [in] */ const String& name,
        /* [out] */ String* value);

    CARAPI Clear();

    CARAPI SetAttributes(
        /* [in] */ IAttributes* atts);

    CARAPI AddAttribute(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [in] */ const String& name,
        /* [in] */ const String& type,
        /* [in] */ const String& value);

    CARAPI SetAttribute(
        /* [in] */ Int32 index,
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [in] */ const String& name,
        /* [in] */ const String& type,
        /* [in] */ const String& value);

    CARAPI RemoveAttribute(
        /* [in] */ Int32 index);

    CARAPI SetURI(
        /* [in] */ Int32 index,
        /* [in] */ const String& uri);

    CARAPI SetLocalName(
        /* [in] */ Int32 index,
        /* [in] */ const String& localName);

    CARAPI SetQName(
        /* [in] */ Int32 index,
        /* [in] */ const String& name);

    CARAPI SetType(
        /* [in] */ Int32 index,
        /* [in] */ const String& type);

    CARAPI SetValue(
        /* [in] */ Int32 index,
        /* [in] */ const String& value);

    CARAPI IsDeclared(
        /* [in] */ Int32 index,
        /* [out] */ Boolean * isDeclared);

    CARAPI IsDeclaredEx(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ Boolean * isDeclared);

    CARAPI IsDeclaredEx2(
        /* [in] */ const String& qName,
        /* [out] */ Boolean * isDeclared);

    CARAPI IsSpecified(
        /* [in] */ Int32 index,
        /* [out] */ Boolean* isSpecified);

    CARAPI IsSpecifiedEx(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ Boolean* isSpecified);

    CARAPI IsSpecifiedEx2(
        /* [in] */ const String& qName,
        /* [out] */ Boolean* isSpecified);

    CARAPI SetDeclared(
        /* [in] */ Int32 index,
        /* [in] */ Boolean value);

    CARAPI SetSpecified(
        /* [in] */ Int32 index,
        /* [in] */ Boolean value);

    /**
     * Construct a new, empty Attributes2Impl object.
     */
    CARAPI constructor();

    /**
     * Copy an existing Attributes or Attributes2 object.
     * If the object implements Attributes2, values of the
     * <em>specified</em> and <em>declared</em> flags for each
     * attribute are copied.
     * Otherwise the flag values are defaulted to assume no DTD was used,
     * unless there is evidence to the contrary (such as attributes with
     * type other than CDATA, which must have been <em>declared</em>).
     *
     * <p>This constructor is especially useful inside a
     * {@link org.xml.sax.ContentHandler#startElement startElement} event.</p>
     *
     * @param atts The existing Attributes object.
     */
    CARAPI constructor(
        /* [in] */ IAttributes* atts);

private:
    AutoPtr<ArrayOf<Boolean> > mDeclared;
    AutoPtr<ArrayOf<Boolean> > mSpecified;
};

} // namespace Ext
} // namespace Sax
} // namespace Xml
} // namespace Org

#endif // __CATTRIBUTES2IMPL_H__
