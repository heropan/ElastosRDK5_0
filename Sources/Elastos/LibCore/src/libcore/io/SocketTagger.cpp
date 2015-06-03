
#include "SocketTagger.h"

using Elastos::IO::IFileDescriptor;

namespace Libcore {
namespace IO {

Mutex SocketTagger::sLock;

ECode SocketTaggerInner::Tag(
    /* [in] */ IFileDescriptor* socketDescriptor)
{
    return NOERROR;
}

ECode SocketTaggerInner::Untag(
    /* [in] */ IFileDescriptor* socketDescriptor)
{
    return NOERROR;
}

AutoPtr<ISocketTagger> SocketTagger::sTagger = new SocketTaggerInner();

CAR_INTERFACE_IMPL(SocketTagger, ISocketTagger);

ECode SocketTagger::TagEx(
    /* [in] */ ISocket* socket)
{
    Boolean ret;
    FAIL_RETURN(socket->IsClosed(&ret))
    if (!ret) {
        AutoPtr<IFileDescriptor> descriptor;
        FAIL_RETURN(socket->GetFileDescriptor((IFileDescriptor**)&descriptor))
        return Tag(descriptor);
    }
    return NOERROR;
}

ECode SocketTagger::UntagEx(
    /* [in] */ ISocket* socket)
{
    Boolean ret;
    FAIL_RETURN(socket->IsClosed(&ret))
    if (!ret) {
        AutoPtr<IFileDescriptor> descriptor;
        FAIL_RETURN(socket->GetFileDescriptor((IFileDescriptor**)&descriptor))
        return Untag(descriptor);
    }
    return NOERROR;
}

ECode SocketTagger::Set(
    /* [in] */ ISocketTagger* tagger)
{
    Mutex::Autolock lock(sLock);

    if (tagger == NULL) {
        //throw new NullPointerException("tagger == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    sTagger = tagger;
    return NOERROR;
}

/**
 * Returns this process socket tagger.
 */
ECode SocketTagger::Get(
    /* [out] */ ISocketTagger** tagger)
{

    VALIDATE_NOT_NULL(tagger);

    Mutex::Autolock lock(sLock);
    *tagger = sTagger;
    REFCOUNT_ADD(*tagger);
    return NOERROR;
}

} // namespace IO
} // namespace Libcore