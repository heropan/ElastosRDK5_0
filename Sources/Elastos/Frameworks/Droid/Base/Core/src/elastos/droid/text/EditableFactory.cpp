
#include "elastos/droid/text/EditableFactory.h"
#include "elastos/droid/text/CEditableFactory.h"
#include "elastos/droid/text/CSpannableStringBuilder.h"

namespace Elastos {
namespace Droid {
namespace Text {

AutoPtr<IEditableFactory> EditableFactory::sInstance;

CAR_INTERFACE_IMPL(EditableFactory, Object, IEditableFactory)

EditableFactory::EditableFactory()
{}

EditableFactory::~EditableFactory()
{}

EditableFactory::GetInstance(
    /* [out] */ IEditableFactory** fact)
{
    VALIDATE_NOT_NULL(fact)

    if (sInstance == NULL) {
        AutoPtr<CEditableFactory> cef;
        CEditableFactory::NewByFriend((CEditableFactory**)&cef);
        sInstance = (IEditableFactory*)cef.Get();
    }

    *fact = cef;
    REFCOUNT_ADD(*fact);
    return NOERROR;
}

ECode EditableFactory::NewEditable(
    /* [in] */ ICharSequence* source,
    /* [out] */ IEditable** editable)
{
    VALIDATE_NOT_NULL(editable);
    return CSpannableStringBuilder::New(
        source, (ISpannableStringBuilder**)editable);
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
