
#ifndef __CDEFAULTHANDLER2_H__
#define __CDEFAULTHANDLER2_H__

#include "_CDefaultHandler2.h"

namespace Org {
namespace Xml {
namespace Sax {
namespace Ext {

/**
 * This class extends the SAX2 base handler class to support the
 * SAX2 {@link LexicalHandler}, {@link DeclHandler}, and
 * {@link EntityResolver2} extensions.  Except for overriding the
 * original SAX1 {@link DefaultHandler#resolveEntity resolveEntity()}
 * method the added handler methods just return.  Subclassers may
 * override everything on a method-by-method basis.
 *
 * <blockquote>
 * <em>This module, both source code and documentation, is in the
 * Public Domain, and comes with <strong>NO WARRANTY</strong>.</em>
 * </blockquote>
 *
 * <p> <em>Note:</em> this class might yet learn that the
 * <em>ContentHandler.setDocumentLocator()</em> call might be passed a
 * {@link Locator2} object, and that the
 * <em>ContentHandler.startElement()</em> call might be passed a
 * {@link Attributes2} object.
 *
 * @since SAX 2.0 (extensions 1.1 alpha)
 * @author David Brownell
 * @version TBS
 */
CarClass(CDefaultHandler2)
{
public:
    /**
     * Receive notification of a notation declaration.
     *
     * <p>By default, do nothing.  Application writers may override this
     * method in a subclass if they wish to keep track of the notations
     * declared in a document.</p>
     *
     * @param name The notation name.
     * @param publicId The notation public identifier, or null if not
     *                 available.
     * @param systemId The notation system identifier.
     * @exception org.xml.sax.SAXException Any SAX exception, possibly
     *            wrapping another exception.
     * @see org.xml.sax.DTDHandler#notationDecl
     */
    CARAPI NotationDecl(
        /* [in] */ const String& name,
        /* [in] */ const String& publicId,
        /* [in] */ const String& systemId);

    /**
     * Receive notification of an unparsed entity declaration.
     *
     * <p>By default, do nothing.  Application writers may override this
     * method in a subclass to keep track of the unparsed entities
     * declared in a document.</p>
     *
     * @param name The entity name.
     * @param publicId The entity public identifier, or null if not
     *                 available.
     * @param systemId The entity system identifier.
     * @param notationName The name of the associated notation.
     * @exception org.xml.sax.SAXException Any SAX exception, possibly
     *            wrapping another exception.
     * @see org.xml.sax.DTDHandler#unparsedEntityDecl
     */
    CARAPI UnparsedEntityDecl(
        /* [in] */ const String& name,
        /* [in] */ const String& publicId,
        /* [in] */ const String& systemId,
        /* [in] */ const String& notationName);

    /**
     * Receive a Locator object for document events.
     *
     * <p>By default, do nothing.  Application writers may override this
     * method in a subclass if they wish to store the locator for use
     * with other document events.</p>
     *
     * @param locator A locator for all SAX document events.
     * @see org.xml.sax.ContentHandler#setDocumentLocator
     * @see org.xml.sax.Locator
     */
    CARAPI SetDocumentLocator(
        /* [in] */ ILocator* locator);

    /**
     * Receive notification of the beginning of the document.
     *
     * <p>By default, do nothing.  Application writers may override this
     * method in a subclass to take specific actions at the beginning
     * of a document (such as allocating the root node of a tree or
     * creating an output file).</p>
     *
     * @exception org.xml.sax.SAXException Any SAX exception, possibly
     *            wrapping another exception.
     * @see org.xml.sax.ContentHandler#startDocument
     */
    CARAPI StartDocument();

    /**
     * Receive notification of the end of the document.
     *
     * <p>By default, do nothing.  Application writers may override this
     * method in a subclass to take specific actions at the end
     * of a document (such as finalising a tree or closing an output
     * file).</p>
     *
     * @exception org.xml.sax.SAXException Any SAX exception, possibly
     *            wrapping another exception.
     * @see org.xml.sax.ContentHandler#endDocument
     */
    CARAPI EndDocument();

    /**
     * Receive notification of the start of a Namespace mapping.
     *
     * <p>By default, do nothing.  Application writers may override this
     * method in a subclass to take specific actions at the start of
     * each Namespace prefix scope (such as storing the prefix mapping).</p>
     *
     * @param prefix The Namespace prefix being declared.
     * @param uri The Namespace URI mapped to the prefix.
     * @exception org.xml.sax.SAXException Any SAX exception, possibly
     *            wrapping another exception.
     * @see org.xml.sax.ContentHandler#startPrefixMapping
     */
    CARAPI StartPrefixMapping(
        /* [in] */ const String& prefix,
        /* [in] */ const String& uri);

    /**
     * Receive notification of the end of a Namespace mapping.
     *
     * <p>By default, do nothing.  Application writers may override this
     * method in a subclass to take specific actions at the end of
     * each prefix mapping.</p>
     *
     * @param prefix The Namespace prefix being declared.
     * @exception org.xml.sax.SAXException Any SAX exception, possibly
     *            wrapping another exception.
     * @see org.xml.sax.ContentHandler#endPrefixMapping
     */
    CARAPI EndPrefixMapping(
        /* [in] */ const String& prefix);

    /**
     * Receive notification of the start of an element.
     *
     * <p>By default, do nothing.  Application writers may override this
     * method in a subclass to take specific actions at the start of
     * each element (such as allocating a new tree node or writing
     * output to a file).</p>
     *
     * @param uri The Namespace URI, or the empty string if the
     *        element has no Namespace URI or if Namespace
     *        processing is not being performed.
     * @param localName The local name (without prefix), or the
     *        empty string if Namespace processing is not being
     *        performed.
     * @param qName The qualified name (with prefix), or the
     *        empty string if qualified names are not available.
     * @param attributes The attributes attached to the element.  If
     *        there are no attributes, it shall be an empty
     *        Attributes object.
     * @exception org.xml.sax.SAXException Any SAX exception, possibly
     *            wrapping another exception.
     * @see org.xml.sax.ContentHandler#startElement
     */
    CARAPI StartElement(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [in] */ const String& qName,
        /* [in] */ IAttributes* atts);

    /**
     * Receive notification of the end of an element.
     *
     * <p>By default, do nothing.  Application writers may override this
     * method in a subclass to take specific actions at the end of
     * each element (such as finalising a tree node or writing
     * output to a file).</p>
     *
     * @param uri The Namespace URI, or the empty string if the
     *        element has no Namespace URI or if Namespace
     *        processing is not being performed.
     * @param localName The local name (without prefix), or the
     *        empty string if Namespace processing is not being
     *        performed.
     * @param qName The qualified name (with prefix), or the
     *        empty string if qualified names are not available.
     * @exception org.xml.sax.SAXException Any SAX exception, possibly
     *            wrapping another exception.
     * @see org.xml.sax.ContentHandler#endElement
     */
    CARAPI EndElement(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [in] */ const String& qName);

    /**
     * Receive notification of character data inside an element.
     *
     * <p>By default, do nothing.  Application writers may override this
     * method to take specific actions for each chunk of character data
     * (such as adding the data to a node or buffer, or printing it to
     * a file).</p>
     *
     * @param ch The characters.
     * @param start The start position in the character array.
     * @param length The number of characters to use from the
     *               character array.
     * @exception org.xml.sax.SAXException Any SAX exception, possibly
     *            wrapping another exception.
     * @see org.xml.sax.ContentHandler#characters
     */
    CARAPI Characters(
        /* [out] */ ArrayOf<Char32>* ch,
        /* [in] */ Int32 start,
        /* [in] */ Int32 length);

    /**
     * Receive notification of ignorable whitespace in element content.
     *
     * <p>By default, do nothing.  Application writers may override this
     * method to take specific actions for each chunk of ignorable
     * whitespace (such as adding data to a node or buffer, or printing
     * it to a file).</p>
     *
     * @param ch The whitespace characters.
     * @param start The start position in the character array.
     * @param length The number of characters to use from the
     *               character array.
     * @exception org.xml.sax.SAXException Any SAX exception, possibly
     *            wrapping another exception.
     * @see org.xml.sax.ContentHandler#ignorableWhitespace
     */
    CARAPI IgnorableWhitespace(
        /* [out] */ ArrayOf<Char32>* ch,
        /* [in] */ Int32 start,
        /* [in] */ Int32 length);

    /**
     * Receive notification of a processing instruction.
     *
     * <p>By default, do nothing.  Application writers may override this
     * method in a subclass to take specific actions for each
     * processing instruction, such as setting status variables or
     * invoking other methods.</p>
     *
     * @param target The processing instruction target.
     * @param data The processing instruction data, or null if
     *             none is supplied.
     * @exception org.xml.sax.SAXException Any SAX exception, possibly
     *            wrapping another exception.
     * @see org.xml.sax.ContentHandler#processingInstruction
     */
    CARAPI ProcessingInstruction(
        /* [in] */ const String& target,
        /* [in] */ const String& data);

    /**
     * Receive notification of a skipped entity.
     *
     * <p>By default, do nothing.  Application writers may override this
     * method in a subclass to take specific actions for each
     * processing instruction, such as setting status variables or
     * invoking other methods.</p>
     *
     * @param name The name of the skipped entity.
     * @exception org.xml.sax.SAXException Any SAX exception, possibly
     *            wrapping another exception.
     * @see org.xml.sax.ContentHandler#processingInstruction
     */
    CARAPI SkippedEntity(
        /* [in] */ const String& name);



    ////////////////////////////////////////////////////////////////////
    // Default implementation of the ErrorHandler interface.
    ////////////////////////////////////////////////////////////////////



    /**
     * Receive notification of a parser warning.
     *
     * <p>The default implementation does nothing.  Application writers
     * may override this method in a subclass to take specific actions
     * for each warning, such as inserting the message in a log file or
     * printing it to the console.</p>
     *
     * @param e The warning information encoded as an exception.
     * @exception org.xml.sax.SAXException Any SAX exception, possibly
     *            wrapping another exception.
     * @see org.xml.sax.ErrorHandler#warning
     * @see org.xml.sax.SAXParseException
     */
    CARAPI Warning(
        /* [in] */ ECode exception);

    /**
     * Receive notification of a recoverable parser error.
     *
     * <p>The default implementation does nothing.  Application writers
     * may override this method in a subclass to take specific actions
     * for each error, such as inserting the message in a log file or
     * printing it to the console.</p>
     *
     * @param e The warning information encoded as an exception.
     * @exception org.xml.sax.SAXException Any SAX exception, possibly
     *            wrapping another exception.
     * @see org.xml.sax.ErrorHandler#warning
     * @see org.xml.sax.SAXParseException
     */
    CARAPI Error(
        /* [in] */ ECode exception);

    /**
     * Report a fatal XML parsing error.
     *
     * <p>The default implementation throws a SAXParseException.
     * Application writers may override this method in a subclass if
     * they need to take specific actions for each fatal error (such as
     * collecting all of the errors into a single report): in any case,
     * the application must stop all regular processing when this
     * method is invoked, since the document is no longer reliable, and
     * the parser may no longer report parsing events.</p>
     *
     * @param e The error information encoded as an exception.
     * @exception org.xml.sax.SAXException Any SAX exception, possibly
     *            wrapping another exception.
     * @see org.xml.sax.ErrorHandler#fatalError
     * @see org.xml.sax.SAXParseException
     */
    CARAPI FatalError(
        /* [in] */ ECode exception);

    CARAPI StartDTD(
        /* [in] */ const String& name,
        /* [in] */ const String& publicId,
        /* [in] */ const String& systemId);

    CARAPI EndDTD();

    CARAPI StartEntity(
        /* [in] */ const String& name);

    CARAPI EndEntity(
        /* [in] */ const String& name);

    CARAPI StartCDATA();

    CARAPI EndCDATA();

    CARAPI Comment(
        /* [out] */ ArrayOf<Char32>* ch,
        /* [in] */ Int32 start,
        /* [in] */ Int32 length);

    CARAPI ElementDecl(
        /* [in] */ const String& name,
        /* [in] */ const String& model);

    CARAPI AttributeDecl(
        /* [in] */ const String& eName,
        /* [in] */ const String& aName,
        /* [in] */ const String& type,
        /* [in] */ const String& mode,
        /* [in] */ const String& value);

    CARAPI InternalEntityDecl(
        /* [in] */ const String& name,
        /* [in] */ const String& value);

    CARAPI ExternalEntityDecl(
        /* [in] */ const String& name,
        /* [in] */ const String& publicId,
        /* [in] */ const String& systemId);

    /**
     * Invokes
     * {@link EntityResolver2#resolveEntity EntityResolver2.resolveEntity()}
     * with null entity name and base URI.
     * You only need to override that method to use this class.
     *
     * @param publicId The public identifier of the external entity being
     *    referenced (normalized as required by the XML specification), or
     *    null if none was supplied.
     * @param systemId The system identifier of the external entity
     *    being referenced; either a relative or absolute URI.
     *  This is never null when invoked by a SAX2 parser; only declared
     *    entities, and any external subset, are resolved by such parsers.
     *
     * @return An InputSource object describing the new input source.
     *
     * @exception SAXException Any SAX exception, possibly wrapping
     *    another exception.
     * @exception IOException Probably indicating a failure to create
     *    a new InputStream or Reader, or an illegal URL.
     */
    CARAPI ResolveEntity(
        /* [in] */ const String& publicId,
        /* [in] */ const String& systemId,
        /* [out] */ IInputSource** src);

    /**
     * Tells the parser that if no external subset has been declared
     * in the document text, none should be used.
     *
     * @param name Identifies the document root element.  This name comes
     *    from a DOCTYPE declaration (where available) or from the actual
     *    root element. The parameter is ignored.
     * @param baseURI The document's base URI, serving as an additional
     *    hint for selecting the external subset.  This is always an absolute
     *    URI, unless it is null because the XMLReader was given an InputSource
     *    without one. The parameter is ignored.
     *
     * @return null (always).
     *
     * @exception SAXException Any SAX exception, possibly wrapping
     *    another exception.
     * @exception IOException Probably indicating a failure to create
     *    a new InputStream or Reader, or an illegal URL.
     */
    CARAPI GetExternalSubset(
        /* [in] */ const String& name,
        /* [in] */ const String& baseURI,
        /* [out] */ IInputSource** src);

    /**
     * Tells the parser to resolve the systemId against the baseURI
     * and read the entity text from that resulting absolute URI.
     * Note that because the older
     * {@link DefaultHandler#resolveEntity DefaultHandler.resolveEntity()},
     * method is overridden to call this one, this method may sometimes
     * be invoked with null <em>name</em> and <em>baseURI</em>, and
     * with the <em>systemId</em> already absolutized.
     *
     * @param name Identifies the external entity being resolved.
     *    Either "[dtd]" for the external subset, or a name starting
     *    with "%" to indicate a parameter entity, or else the name of
     *    a general entity.  This is never null when invoked by a SAX2
     *    parser.
     * @param publicId The public identifier of the external entity being
     *    referenced (normalized as required by the XML specification), or
     *    null if none was supplied.
     * @param baseURI The URI with respect to which relative systemIDs
     *    are interpreted.  This is always an absolute URI, unless it is
     *    null (likely because the XMLReader was given an InputSource without
     *  one).  This URI is defined by the XML specification to be the one
     *    associated with the "&lt;" starting the relevant declaration.
     * @param systemId The system identifier of the external entity
     *    being referenced; either a relative or absolute URI.
     *  This is never null when invoked by a SAX2 parser; only declared
     *    entities, and any external subset, are resolved by such parsers.
     *
     * @return An InputSource object describing the new input source.
     *
     * @exception SAXException Any SAX exception, possibly wrapping
     *    another exception.
     * @exception IOException Probably indicating a failure to create
     *    a new InputStream or Reader, or an illegal URL.
     */
    CARAPI ResolveEntity(
        /* [in] */ const String& name,
        /* [in] */ const String& publicId,
        /* [in] */ const String& baseURI,
        /* [in] */ const String& systemId,
        /* [out] */ IInputSource** src);

private:
    // TODO: Add your private member variables here.
};

} // namespace Ext
} // namespace Sax
} // namespace Xml
} // namespace Org

#endif // __CDEFAULTHANDLER2_H__
