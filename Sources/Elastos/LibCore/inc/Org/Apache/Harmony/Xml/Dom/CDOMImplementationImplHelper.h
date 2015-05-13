
#ifndef __CDOMIMPLEMENTATIONIMPLHELPER_H__
#define __CDOMIMPLEMENTATIONIMPLHELPER_H__

#include "_CDOMImplementationImplHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xml {
namespace Dom {

CarClass(CDOMImplementationImplHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ Org::W3c::Dom::IDOMImplementation ** ppInstance);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CDOMIMPLEMENTATIONIMPLHELPER_H__
