#ifndef __SOCKETTAGGER_H__
#define __SOCKETTAGGER_H__

#include "Elastos.CoreLibrary_server.h"
#include <elastos/Mutex.h>

using Libcore::IO::ISocketTagger;
using Elastos::Core::Mutex;
using Elastos::IO::IFileDescriptor;
using Elastos::Net::ISocket;

namespace Libcore {
namespace IO {

class SocketTagger
    : public ElRefBase
    , public ISocketTagger
{
public:
    CAR_INTERFACE_DECL();

    CARAPI TagEx(
        /* [in] */ ISocket* socket);

    CARAPI UntagEx(
        /* [in] */ ISocket* socket);

    /**
     * Sets this process' socket tagger to {@code tagger}.
     */
    static CARAPI Set(
        /* [in] */ ISocketTagger* tagger);

    /**
     * Returns this process socket tagger.
     */
    static CARAPI Get(
        /* [out] */ ISocketTagger** tagger);

private:
    static AutoPtr<ISocketTagger> sTagger;

    static Mutex sLock;
};

class SocketTaggerInner
    : public SocketTagger
{
public:
    CARAPI Tag(
        /* [in] */ IFileDescriptor* socketDescriptor);

    CARAPI Untag(
        /* [in] */ IFileDescriptor* socketDescriptor);
};

} // namespace IO
} // namespace Libcore

#endif // __SOCKETTAGGER_H__