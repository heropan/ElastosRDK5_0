
#ifndef __CSUBJECTKEYIDENTIFIERHELPER_H__
#define __CSUBJECTKEYIDENTIFIERHELPER_H__

#include "_CSubjectKeyIdentifierHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CSubjectKeyIdentifierHelper)
{
public:
    CARAPI Decode(
        /* [in] */ ArrayOf<Byte> * pEncoding,
        /* [out] */ Org::Apache::Harmony::Security::X509::ISubjectKeyIdentifier ** ppIdentifier);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CSUBJECTKEYIDENTIFIERHELPER_H__
