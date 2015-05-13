
#ifndef __CLOGGERHELPER_H__
#define __CLOGGERHELPER_H__

#include "_CLoggerHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CLoggerHelper)
{
public:
    CARAPI GetStream(
        /* [in] */ const String& name,
        /* [out] */ Org::Apache::Harmony::Xnet::Provider::Jsse::ILoggerStream ** ppStream);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CLOGGERHELPER_H__
