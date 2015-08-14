
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CREASONFLAGS_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CREASONFLAGS_H__

#include "_CReasonFlags.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CReasonFlags)
{
public:
    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb,
        /* [in] */ const String& prefix);

    CARAPI constructor(
        /* [in] */ ArrayOf<Boolean> * pFlags);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CREASONFLAGS_H__
