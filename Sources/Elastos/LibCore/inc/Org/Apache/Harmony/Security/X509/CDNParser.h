
#ifndef __CDNPARSER_H__
#define __CDNPARSER_H__

#include "_CDNParser.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CDNParser)
{
public:
    CARAPI Parse(
        /* [out] */ Elastos::Utility::IList ** ppList);

    CARAPI constructor(
        /* [in] */ const String& dn);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CDNPARSER_H__
