#ifndef __ELASTOS_DROID_SAX_CCHILDRENT_H__
#define __ELASTOS_DROID_SAX_CCHILDRENT_H__

#include "_CChildren.h"
#include "sax/Element.h"

namespace Elastos {
namespace Droid {
namespace Sax {

/**
 * Contains element children. Using this class instead of HashMap results in
 * measurably better performance.
 */
CarClass(CChildren)
{
public:
    class Child
        : public ElRefBase
        , public IElement
        , public Element {
    public:
        Child(
            /* [in] */ IElement* parent,
            /* [in] */ const String& uri,
            /* [in] */ const String& localName,
            /* [in] */ Int32 depth,
            /* [in] */ Int32 hash)
            : mHash(hash)
        {
            Element::Init(parent, uri, localName, depth);
        }

        CAR_INTERFACE_DECL();

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

    public:
        const Int32 mHash;
        AutoPtr<Child> mNext;
    };

public:
    CChildren();

    /**
     * Looks up a child by name and creates a new one if necessary.
     */
    CARAPI GetOrCreate(
        /* [in] */ IElement* parent,
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ IElement** result);

    /**
     * Looks up a child by name.
     */
    CARAPI Get(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [out] */ IElement** result);

public:
    AutoPtr<ArrayOf<AutoPtr<Child> > > mChildren;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SAX_CCHILDRENT_H__
