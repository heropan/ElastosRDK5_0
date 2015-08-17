
#include "provider/CContactsOrganizations.h"
#include "text/TextUtils.h"
#include "net/Uri.h"
#include "R.h"

using Elastos::Core::CString;
using Elastos::Droid::R;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Content::Res::IResources;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsOrganizations::constructor()
{
    return NOERROR;
}

ECode CContactsOrganizations::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://contacts/organizations"), uri);
}

ECode CContactsOrganizations::GetDisplayLabel(
    /* [in] */ IContext* context,
    /* [in] */ Int32 type,
    /* [in] */ ICharSequence* label,
    /* [out] */ ICharSequence** lb)
{
    AutoPtr<ICharSequence> display;
    FAIL_RETURN(CString::New(String(""), (ICharSequence**)&display))

    if (type != IContactsOrganizationColumns::TYPE_CUSTOM) {
        AutoPtr<ArrayOf<ICharSequence*> > labels;
        AutoPtr<IResources> res;
        FAIL_RETURN(context->GetResources((IResources**)&res))
        FAIL_RETURN(res->GetTextArray(Elastos::Droid::R::array::organizationTypes, (ArrayOf<ICharSequence*>**)&labels))

        Int32 size = labels->GetLength();
        if (type - 1 >= 0 && type - 1 < size) {
            display = (*labels)[type - 1];
        }
        else {
            display = (*labels)[IContactsOrganizationColumns::TYPE_WORK - 1];
        }
    }
    else {
        if (!TextUtils::IsEmpty(label)) {
            display = label;
        }
    }
    *lb = display;
    REFCOUNT_ADD(*lb);
    return NOERROR;
}

} // Provider
} // Droid
} // Elastos