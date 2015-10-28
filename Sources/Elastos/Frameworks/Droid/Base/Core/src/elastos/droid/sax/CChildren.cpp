#include "elastos/droid/sax/CChildren.h"
#include "elastos/droid/sax/CElement.h"

namespace Elastos {
namespace Droid {
namespace Sax {

CAR_OBJECT_IMPL(CChildren)

CChildren::CChildren()
{
    mChildren = ArrayOf<AutoPtr<Child> >::Alloc(16);
}

CChildren::~CChildren()
{
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
        REFCOUNT_ADD(*result);
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
                REFCOUNT_ADD(*result);
                return NOERROR;
            }

            previous = current;
            current = current->mNext;
        } while (current != NULL);

        // Add a new child to the bucket.
        current = new Child(parent, uri, localName, cparent->mDepth + 1, hash);
        previous->mNext = current;
        *result = current;
        REFCOUNT_ADD(*result);
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
                REFCOUNT_ADD(*result);
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
