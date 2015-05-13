
#ifndef __CQNAMEHELPER_H__
#define __CQNAMEHELPER_H__

#include "_CQNameHelper.h"

namespace Elastosx {
namespace Xml {
namespace Namespace {

CarClass(CQNameHelper)
{
public:
    CARAPI ValueOf(
        /* [in] */ const String& qNameAsString,
        /* [out] */ Elastosx::Xml::Namespace::IQName ** ppName);

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __CQNAMEHELPER_H__
