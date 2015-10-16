#include "elastos/droid/sax/CRootElement.h"
#include "elastos/droid/sax/CElement.h"

namespace Elastos {
namespace Droid {
namespace Sax {

UInt32 CRootElement::Handler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CRootElement::Handler::Release()
{
    return ElRefBase::Release();
}

PInterface CRootElement::Handler::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IEntityResolver) {
        return (PInterface)(IEntityResolver*)this;
    }
    else if (riid == EIID_IDTDHandler) {
        return (PInterface)(IDTDHandler*)this;
    }
    else if (riid == EIID_IContentHandler) {
        return (PInterface)(IContentHandler*)this;
    }
    else if (riid == EIID_IErrorHandler) {
        return (PInterface)(IErrorHandler*)this;
    }

    return NULL;
}

ECode CRootElement::Handler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface*)(IEntityResolver*)this) {
        *pIID = EIID_IEntityResolver;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IDTDHandler*)this) {
        *pIID = EIID_IDTDHandler;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IContentHandler*)this) {
        *pIID = EIID_IContentHandler;
        return NOERROR;
    }

    else if (pObject == (IInterface*)(IErrorHandler*)this) {
        *pIID = EIID_IErrorHandler;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode CRootElement::Handler::SetDocumentLocator(
    /* [in] */ ILocator* locator)
{
    mLocator = locator;
    return NOERROR;
}

ECode CRootElement::Handler::StartElement(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [in] */ const String& qName,
    /* [in] */ IAttributes* attributes)
{
    Int32 depth = ++mDepth;

    if (depth == 0) {
        // This is the root element.
        return StartRoot(uri, localName, attributes);
    }

    // Prohibit mixed text and elements.
    if (mBodyBuilder != NULL) {
        //throw new BadXmlException("Encountered mixed content"
        //        + " within text element named " + current + ".",
        //        locator);
        return E_BAD_XML_EXCEPTION;
    }

    // If we're one level below the current element.
    CElement* ccurrent =  (CElement*)mCurrent.Get();
    if (depth == ccurrent->mDepth + 1) {
        // Look for a child to push onto the stack.
        AutoPtr<IChildren> children = ccurrent->mChildren;
        if (children != NULL) {
            AutoPtr<IElement> child;
            FAIL_RETURN(children->Get(uri, localName, (IElement**)&child))
            if (child != NULL) {
                FAIL_RETURN(Start(child, attributes))
            }
        }
    }
    return NOERROR;
}

ECode CRootElement::Handler::StartRoot(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [in] */ IAttributes* attributes)
{
    AutoPtr<IElement> root = (IElement*)this->Probe(EIID_IElement);
    CElement* croot =  (CElement*)root.Get();
    if (croot->mUri.Compare(uri) != 0
            || croot->mLocalName.Compare(localName) != 0) {
        //throw new BadXmlException("Root element name does"
        //        + " not match. Expected: " + root + ", Got: "
        //        + Element.toString(uri, localName), locator);
        return E_BAD_XML_EXCEPTION;
    }

    return Start(root, attributes);
}

ECode CRootElement::Handler::Start(
    /* [in] */ IElement* e,
    /* [in] */ IAttributes* attributes)
{
    // Push element onto the stack.
    mCurrent = e;

    CElement* ce =  (CElement*)e;
    if (ce->mStartElementListener != NULL) {
        FAIL_RETURN(ce->mStartElementListener->Start(attributes))
    }

    if (ce->mEndTextElementListener != NULL) {
        mBodyBuilder = new StringBuilder();
    }

    FAIL_RETURN(e->ResetRequiredChildren())
    ce->mVisited = TRUE;

    return NOERROR;
}

ECode CRootElement::Handler::Characters(
    /* [in] */ const ArrayOf<Char32>& buffer,
    /* [in] */ Int32 start,
    /* [in] */ Int32 length)
{
    if (mBodyBuilder != NULL) {
        mBodyBuilder->AppendChars(buffer, start, length);
    }
    return NOERROR;
}

ECode CRootElement::Handler::EndElement(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [in] */ const String& qName)
{
    AutoPtr<IElement> current = mCurrent;

    CElement* ccurrent =  (CElement*)mCurrent.Get();
    // If we've ended the current element...
    if (mDepth == ccurrent->mDepth) {
        FAIL_RETURN(current->CheckRequiredChildren(mLocator))

        // Invoke end element listener.
        if (ccurrent->mEndElementListener != NULL) {
            FAIL_RETURN(ccurrent->mEndElementListener->End())
        }

        // Invoke end text element listener.
        if (mBodyBuilder != NULL) {
            String body;
            mBodyBuilder->ToString(&body);
            mBodyBuilder = NULL;

            // We can assume that this listener is present.
            FAIL_RETURN(ccurrent->mEndTextElementListener->End(body))
        }

        // Pop element off the stack.
        mCurrent = ccurrent->mParent;
    }

    mDepth--;
    return NOERROR;
}

ECode CRootElement::Handler::ResolveEntity(
    /* [in] */ const String& publicId,
    /* [in] */ const String& systemId,
    /* [out] */ IInputSource** src)
{
    VALIDATE_NOT_NULL(src);

    *src = NULL;

    return NOERROR;
}

ECode CRootElement::Handler::NotationDecl(
    /* [in] */ const String& name,
    /* [in] */ const String& publicId,
    /* [in] */ const String& systemId)
{
    return NOERROR;
}

ECode CRootElement::Handler::UnparsedEntityDecl(
    /* [in] */ const String& name,
    /* [in] */ const String& publicId,
    /* [in] */ const String& systemId,
    /* [in] */ const String& notationName)
{
    return NOERROR;
}

ECode CRootElement::Handler::StartDocument()
{
    return NOERROR;
}

ECode CRootElement::Handler::EndDocument()
{
    return NOERROR;
}

ECode CRootElement::Handler::StartPrefixMapping(
    /* [in] */ const String& prefix,
    /* [in] */ const String& uri)
{
    return NOERROR;
}

ECode CRootElement::Handler::EndPrefixMapping(
    /* [in] */ const String& prefix)
{
    return NOERROR;
}

ECode CRootElement::Handler::Characters(
    /* [out] */ ArrayOf<Char32>* ch,
    /* [in] */ Int32 start,
    /* [in] */ Int32 length)
{
    return NOERROR;
}

ECode CRootElement::Handler::IgnorableWhitespace(
    /* [out] */ ArrayOf<Char32>* ch,
    /* [in] */ Int32 start,
    /* [in] */ Int32 length)
{
    return NOERROR;
}

ECode CRootElement::Handler::ProcessingInstruction(
    /* [in] */ const String& target,
    /* [in] */ const String& data)
{
    return NOERROR;
}

ECode CRootElement::Handler::SkippedEntity(
    /* [in] */ const String& name)
{
    return NOERROR;
}

ECode CRootElement::Handler::Warning(
    /* [in] */ ECode exception)
{
    return NOERROR;
}

ECode CRootElement::Handler::Error(
    /* [in] */ ECode exception)
{
    return NOERROR;
}

ECode CRootElement::Handler::FatalError(
    /* [in] */ ECode exception)
{
    return exception;
}

ECode CRootElement::constructor(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName)
{
    return Element::Init(NULL, uri, localName, 0);
}

ECode CRootElement::constructor(
    /* [in] */ const String& localName)
{
    return constructor(String(""), localName);
}

PInterface CRootElement::Probe(
    /* [in]  */ REIID riid)
{
    return _CRootElement::Probe(riid);
}

ECode CRootElement::GetContentHandler(
    /* [out] */ IContentHandler** handler)
{
    VALIDATE_NOT_NULL(handler);

    *handler = (IContentHandler*)mHandler;
    REFCOUNT_ADD(*handler);

    return NOERROR;
}

ECode CRootElement::GetChild(
    /* [in] */ const String& localName,
    /* [out] */ IElement** result)
{
    return Element::GetChild(localName, result);
}

ECode CRootElement::GetChild(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [out] */ IElement** result)
{
    return Element::GetChild(uri, localName, result);
}

ECode CRootElement::RequireChild(
    /* [in] */ const String& localName,
    /* [out] */ IElement** result)
{
    return Element::RequireChild(localName, result);
}

ECode CRootElement::RequireChild(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [out] */ IElement** result)
{
    return Element::RequireChild(uri, localName, result);
}

ECode CRootElement::SetElementListener(
    /* [in] */ IInterface* elementListener)
{
    return Element::SetElementListener(elementListener);
}

ECode CRootElement::SetTextElementListener(
    /* [in] */ IInterface* elementListener)
{
    return Element::SetTextElementListener(elementListener);
}

ECode CRootElement::SetStartElementListener(
    /* [in] */ IStartElementListener* startElementListener)
{
    return Element::SetStartElementListener(startElementListener);
}

ECode CRootElement::SetEndElementListener(
    /* [in] */ IEndElementListener* endElementListener)
{
    return Element::SetEndElementListener(endElementListener);
}

ECode CRootElement::SetEndTextElementListener(
    /* [in] */ IEndTextElementListener* endTextElementListener)
{
    return Element::SetEndTextElementListener(endTextElementListener);
}

ECode CRootElement::ToString(
    /* [out] */ String* str)
{
    return Element::ToString(str);
}

ECode CRootElement::ResetRequiredChildren()
{
    return Element::ResetRequiredChildren();
}

ECode CRootElement::CheckRequiredChildren(
    /* [in] */ ILocator* locator)
{
    return Element::CheckRequiredChildren(locator);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
