#ifndef __SOCKETTAGGER_H__
#define __SOCKETTAGGER_H__

#ifndef __UUNM_Elastos_CoreLibrary_DEFINED__
#define __UUNM_Elastos_CoreLibrary_DEFINED__
#define c_pElastos_CoreLibraryUunm "Elastos.CoreLibrary.eco"
#endif // __UUNM_Elastos_CoreLibrary_DEFINED__

#include "core/Object.h"

using Libcore::IO::ISocketTagger;
using Libcore::IO::EIID_ISocketTagger;
using Elastos::IO::IFileDescriptor;
using Elastos::Net::ISocket;

namespace Libcore {
namespace IO {

EXTERN const _ELASTOS ClassID ECLSID_SocketTagger;

class SocketTagger
    : public Object
    , public ISocketTagger
{
public:
    CAR_OBJECT_DECL()
    CAR_INTERFACE_DECL()

    CARAPI Tag(
        /* [in] */ ISocket* socket);

    CARAPI Untag(
        /* [in] */ ISocket* socket);

    using ISocketTagger::Tag;

    using ISocketTagger::Untag;

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

    static Object sLock;
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