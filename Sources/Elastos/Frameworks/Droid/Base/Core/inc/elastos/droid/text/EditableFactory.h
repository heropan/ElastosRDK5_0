
#ifndef __ELASTOS_DROID_TEXT_EDITABLEFACTORY_H__
#define __ELASTOS_DROID_TEXT_EDITABLEFACTORY_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Text {

class EditableFactory
    : public Object
    , public IEditableFactory
{
public:
    CAR_INTERFACE_DECL()

    EditableFactory();

    virtual ~EditableFactory();

    /**
     * Returns a new SpannedStringBuilder from the specified
     * CharSequence.  You can override this to provide
     * a different kind of Spanned.
     */
    CARAPI NewEditable(
        /* [in] */ ICharSequence* source,
        /* [out] */ IEditable** editable);

    /**
     * Returns the standard Editable Factory.
     */
    static CARAPI GetInstance(
        /* [out] */ IEditableFactory** fact);

private:
    static AutoPtr<IEditableFactory> sInstance;
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_EDITABLEFACTORY_H__
