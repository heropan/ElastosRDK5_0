
#ifndef __CRAWHEADERSHELPER_H__
#define __CRAWHEADERSHELPER_H__

#include "_CRawHeadersHelper.h"

using Elastos::Utility::IMap;

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CRawHeadersHelper)
{
public:
    CARAPI FromMultimap(
        /* [in] */ IMap* map,
        /* [out] */ IRawHeaders** rh);
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CRAWHEADERSHELPER_H__
