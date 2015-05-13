#include "sax/CChildren.h"
#include "sax/CElement.h"

namespace Elastos {
namespace Droid {
namespace Sax {

CAR_INTERFACE_IMPL(CChildren::Child, IElement);

ECode CChildren::Child::GetChild(
    /* [in] */ const String& localName,
    /* [out] */ IElement** result)
{
    return Element::GetChild(localName, result);
}

ECode CChildren::Child::GetChildEx(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [out] */ IElement** result)
{
    return Element::GetChildEx(uri, localName, result);
}

ECode CChildren::Child::RequireChild(
    /* [in] */ const String& localName,
    /* [out] */ IElement** result)
{
    return Element::RequireChild(localName, result);
}

ECode CChildren::Child::RequireChildEx(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [out] */ IElement** result)
{
    return Element::RequireChildEx(uri, localName, result);
}

ECode CChildren::Child::SetElementListener(
    /* [in] */ IInterface* elementListener)
{
    return Element::SetElementListener(elementListener);
}

ECode CChildren::Child::SetTextElementListener(
    /* [in] */ IInterface* elementListener)
{
    return Element::SetTextElementListener(elementListener);
}

ECode CChildren::Child::SetStartElementListener(
    /* [in] */ IStartElementListener* startElementListener)
{
    return Element::SetStartElementListener(startElementListener);
}

ECode CChildren::Child::SetEndElementListener(
    /* [in] */ IEndElementListener* endElementListener)
{
    return Element::SetEndElementListener(endElementListener);
}

ECode CChildren::Child::SetEndTextElementListener(
    /* [in] */ IEndTextElementListener* endTextElementListener)
{
    return Element::SetEndTextElementListener(endTextElementListener);
}

ECode CChildren::Child::ToString(
    /* [out] */ String* str)
{
    return Element::ToString(str);
}

ECode CChildren::Child::ResetRequiredChildren()
{
    return Element::ResetRequiredChildren();
}

ECode CChildren::Child::CheckRequiredChildren(
    /* [in] */ ILocator* locator)
{
    return Element::CheckRequiredChildren(locator);
}

CChildren::CChildren()
{
    mChildren = ArrayOf<AutoPtr<Child> >::Alloc(16);
}

ECode CChildren::GetOrCreate(
    /* [in] */ IElement* parent,
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [out] */ IElement** result)
{
    VALIDATE_NOT_NULL(result);

    Int32 hash = uri.GetHashCode() * 31 + localName.GetHashCode();
    Int32 index = hash & 15;

    AutoPtr<Child> current = (*mChildren)[index];
    CElement* cparent = (CElement*)parent;
    if (current == NULL) {
        // We have no children in this bucket yet.
        current = new Child(parent, uri, localName, cparent->mDepth + 1, hash);
        mChildren->Set(index, current);
        *result = (IElement*)current;
        INTERFACE_ADDREF(*result);
        return NOERROR;
    }
    else {
        // Search this bucket.
        AutoPtr<Child> previous;
        do {
            if (current->mHash == hash
                    && current->mUri.Compare(uri) == 0
                    && current->mLocalName.Compare(localName) == 0) {
                // We already have a child with that name.
                *result = (IElement*)current;
                INTERFACE_ADDREF(*result);
                return NOERROR;
            }

            previous = current;
            current = current->mNext;
        } while (current != NULL);

        // Add a new child to the bucket.
        current = new Child(parent, uri, localName, cparent->mDepth + 1, hash);
        previous->mNext = current;
        *result = current;
        INTERFACE_ADDREF(*result);
        return NOERROR;
    }
    return NOERROR;
}

ECode CChildren::Get(
    /* [in] */ const String& uri,
    /* [in] */ const String& localName,
    /* [out] */ IElement** result)
{
    VALIDATE_NOT_NULL(result);

    Int32 hash = uri.GetHashCode() * 31 + localName.GetHashCode();
    Int32 index = hash & 15;

    AutoPtr<Child> current = (*mChildren)[index];
    if (current == NULL) {
        *result = NULL;
        return NOERROR;
    }
    else {
        do {
            if (current->mHash == hash
                    && current->mUri.Compare(uri) == 0
                    && current->mLocalName.Compare(localName) == 0) {
                *result = current;
                INTERFACE_ADDREF(*result);
                return NOERROR;
            }
            current = current->mNext;
        } while (current != NULL);

        *result = NULL;
        return NOERROR;
    }
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
