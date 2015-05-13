
#include "cmdef.h"
#include "provider/CCallLog.h"
#include "net/CUriHelper.h"

using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;

namespace Elastos {
namespace Droid {
namespace Provider {

static AutoPtr<IUri> initCONTENTURI()
{
    AutoPtr<IUri> uri;
    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    helper->Parse(String("content://") + ICallLog::AUTHORITY, (IUri**)&uri);
    return uri;
}

AutoPtr<IUri> CCallLog::CONTENT_URI = initCONTENTURI();

ECode CCallLog::GetCONTENTURI(
        /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    *uri = CONTENT_URI;
    INTERFACE_ADDREF(*uri);
    return NOERROR;
}

} //Provider
} //Droid
} //Elastos