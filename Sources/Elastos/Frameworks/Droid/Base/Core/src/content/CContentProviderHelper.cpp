
#include "content/CContentProviderHelper.h"

namespace Elastos {
namespace Droid {
namespace Content {

ECode CContentProviderHelper::CoerceToLocalContentProvider(
    /* [in] */ IIContentProvider* abstractInterface,
    /* [out] */ IContentProvider** localContentProvider)
{
    return ContentProvider::CoerceToLocalContentProvider(abstractInterface, localContentProvider);
}

}
}
}

