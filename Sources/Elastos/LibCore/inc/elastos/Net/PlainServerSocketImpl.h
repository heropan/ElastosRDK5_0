#ifndef __PLAINSERVERSOCKETIMPL_H__
#define __PLAINSERVERSOCKETIMPL_H__

#include "PlainSocketImpl.h"

namespace Elastos {
namespace Net {

class PlainServerSocketImpl : public PlainSocketImpl
{
public:
    Init();

    Init(
        /* [in] */ IFileDescriptor *fd);

    CARAPI Create(
        /* [in] */ Boolean isStreaming);
};

} // namespace Net
} // namespace Elastos

#endif //__PLAINSERVERSOCKETIMPL_H__
