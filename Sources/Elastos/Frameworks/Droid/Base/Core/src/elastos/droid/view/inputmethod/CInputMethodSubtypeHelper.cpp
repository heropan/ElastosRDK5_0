
#include "elastos/droid/view/inputmethod/CInputMethodSubtypeHelper.h"
#include <elastos/utility/etl/List.h>


using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {

ECode CInputMethodSubtypeHelper::Sort(
    /* [in] */ IContext* context,
    /* [in] */ Int32 flags,
    /* [in] */ IInputMethodInfo* imi,
    /* [in] */ IObjectContainer* subtypeList,
    /* [out] */ IObjectContainer** sortedSubtypeList)
{
    if (imi == NULL) {
        *sortedSubtypeList = subtypeList;
        REFCOUNT_ADD(*sortedSubtypeList);
        return NOERROR;
    }

    List< AutoPtr<IInputMethodSubtype> > sortedList;
    Int32 N = 0;
    imi->GetSubtypeCount(&N);
    for (Int32 i = 0; i < N; ++i) {
        AutoPtr<IInputMethodSubtype> subtype;
        imi->GetSubtypeAt(i, (IInputMethodSubtype**)&subtype);
        Boolean contained;
        subtypeList->Contains(subtype, &contained);
        if (contained) {
            sortedList.PushBack(subtype);
            subtypeList->Remove(subtype);
        }
    }
    // If subtypes in subtypeList remain, that means these subtypes are not
    // contained in imi, so the remaining subtypes will be appended.
    AutoPtr<IObjectEnumerator> it;
    subtypeList->GetObjectEnumerator((IObjectEnumerator**)&it);
    Boolean succeeded;
    while (it->MoveNext(&succeeded), succeeded) {
        AutoPtr<IInputMethodSubtype> subtype;
        it->Current((IInterface**)&subtype);
        sortedList.PushBack(subtype);
    }

    CObjectContainer::New(sortedSubtypeList);
    List< AutoPtr<IInputMethodSubtype> >::Iterator sit;
    for (sit = sortedList.Begin(); sit != sortedList.End(); ++sit) {
        (*sortedSubtypeList)->Add(*sit);
    }
    return NOERROR;
}

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos
