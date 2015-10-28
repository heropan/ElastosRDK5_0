#ifndef __ELASTOS_DROID_SAX_CCHILDRENT_H__
#define __ELASTOS_DROID_SAX_CCHILDRENT_H__

#include "_Elastos_Droid_Sax_CChildren.h"
#include "elastos/droid/sax/Element.h"

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
        : public Element
    {
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

        CAR_OBJECT_DECL();

        Child();
        virtual ~Child();

    public:
        const Int32 mHash;
        AutoPtr<Child> mNext;
    };

public:
    CChildren();
    virtual ~CChildren();

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
