
#ifndef __CFILTERINPUTSTREAM_H__
#define __CFILTERINPUTSTREAM_H__

#include "_CFilterInputStream.h"
#include "FilterInputStream.h"

namespace Elastos {
namespace IO {

CarClass(CFilterInputStream)
    , public FilterInputStream
{
public:
    CAR_OBJECT_DECL()
};

} // namespace IO
} // namespace Elastos

#endif // __CFILTERINPUTSTREAM_H__
