
#include "content/CIntentFilterHelper.h"

namespace Elastos {
namespace Droid {
namespace Content {

ECode CIntentFilterHelper::Create(
    /* [in] */ const String& action,
    /* [in] */ const String& dataType,
    /* [out] */ IIntentFilter** intentFilter)
{
    return IntentFilter::Create(action, dataType, intentFilter);
}

}
}
}

