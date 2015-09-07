
#include "content/CContentProviderHelper.h"
#include "content/ContentProvider.h"

namespace Elastos {
namespace Droid {
namespace Content {

CAR_INTERFACE_IMPL(CContentProviderHelper, Singleton, IContentProviderHelper)

CAR_SINGLETON_IMPL(CContentProviderHelper)


ECode CContentProviderHelper::CoerceToLocalContentProvider(
    /* [in] */ IIContentProvider* abstractInterface,
    /* [out] */ IContentProvider** localContentProvider)
{
    return ContentProvider::CoerceToLocalContentProvider(abstractInterface, localContentProvider);
}

}
}
}

