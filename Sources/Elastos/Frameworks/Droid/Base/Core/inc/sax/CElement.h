#ifndef __ELASTOS_DROID_SAX_CELEMENT_H__
#define __ELASTOS_DROID_SAX_CELEMENT_H__

#include "_CElement.h"
#include "sax/Element.h"

namespace Elastos {
namespace Droid {
namespace Sax {

/**
 * An XML element. Provides access to child elements and hooks to listen
 * for events related to this element.
 *
 * @see RootElement
 */
CarClass(CElement) , public Element
{
public:
    CARAPI constructor(
        /* [in] */ IElement* parent,
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [in] */ Int32 depth);

    CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

    /**
     * Gets the child element with the given name. Uses an empty string as the
     * namespace.
     */
    CARAPI GetChild(
        /* [in] */ const String&  localName,
        /* [out] */ IElement** result);

    /**
     * Gets the child element with the given name.
     */
    CARAPI GetChild(
        /* [in] */ const String&  uri,
        /* [in] */ const String&  localName,
        /* [out] */ IElement** result);

    /**
     * Gets the child element with the given name. Uses an empty string as the
     * namespace. We will throw a {@link org.xml.sax.SAXException} at parsing
     * time if the specified child is missing. This helps you ensure that your
     * listeners are called.
     */
    CARAPI RequireChild(
        /* [in] */ const String&  localName,
        /* [out] */ IElement** result);

    /**
     * Gets the child element with the given name. We will throw a
     * {@link org.xml.sax.SAXException} at parsing time if the specified child
     * is missing. This helps you ensure that your listeners are called.
     */
    CARAPI RequireChild(
        /* [in] */ const String&  uri,
        /* [in] */ const String&  localName,
        /* [out] */ IElement** result);

    /**
     * Sets start and end element listeners at the same time.
     */
    CARAPI SetElementListener(
        /* [in] */ IInterface* elementListener);

    /**
     * Sets start and end text element listeners at the same time.
     */
    CARAPI SetTextElementListener(
        /* [in] */ IInterface* elementListener);

    /**
     * Sets a listener for the start of this element.
     */
    CARAPI SetStartElementListener(
        /* [in] */ IStartElementListener* startElementListener);

    /**
     * Sets a listener for the end of this element.
     */
    CARAPI SetEndElementListener(
        /* [in] */ IEndElementListener* endElementListener);

    /**
     * Sets a listener for the end of this text element.
     */
    CARAPI SetEndTextElementListener(
        /* [in] */ IEndTextElementListener* endTextElementListener);

    CARAPI ToString(
        /* [out] */ String* str);

    /**
     * Clears flags on required children.
     */
    CARAPI ResetRequiredChildren();

    /**
     * Throws an exception if a required child was not present.
     */
    CARAPI CheckRequiredChildren(
        /* [in] */ ILocator* locator);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SAX_CELEMENT_H__
