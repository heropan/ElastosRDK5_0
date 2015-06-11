#include "SelectionKey.h"

namespace Elastos {
namespace IO {
namespace Channels {

CAR_INTERFACE_IMPL(SelectionKey, Object, ISelectionKey)

/**
 * Interest set mask bit for socket-accept operations.
 */
int SelectionKey::sOP_ACCEPT = 16;

/**
 * Interest set mask bit for socket-connect operations.
 */
int SelectionKey::sOP_CONNECT = 8;

/**
 * Interesting operation mask bit for read operations.
 */
int SelectionKey::sOP_READ = 1;

/**
 * Interest set mask bit for write operations.
 */
int SelectionKey::sOP_WRITE = 4;

SelectionKey::SelectionKey()
    : mAttachment(NULL)
{
}

SelectionKey::~SelectionKey()
{
}

ECode SelectionKey::Attach(
    /* [in] */ IObject* anObject,
    /* [out] */ IObject** oldObject)
{
    *oldObject = mAttachment;
    mAttachment = anObject;
    return NOERROR;
}

ECode SelectionKey::Attachment(
    /* [out] */ IObject** obj)
{
    *obj = mAttachment;
    return NOERROR;
}

ECode SelectionKey::IsAcceptable(
    /* [out] */ Boolean* acceptable)
{
    Int32 ops;
    ECode ecRet = ReadyOps(&ops);
    if ((ops & sOP_ACCEPT) == sOP_ACCEPT)
        *acceptable = TRUE;
    else
        *acceptable = FALSE;
    return ecRet;
}

ECode SelectionKey::IsConnectable(
    /* [out] */ Boolean* connectable)
{
    Int32 ops;
    ECode ecRet = ReadyOps(&ops);
    *connectable = (ops & sOP_CONNECT) == sOP_CONNECT;
    return ecRet;
}

ECode SelectionKey::IsReadable(
    /* [out] */ Boolean* readable)
{
    Int32 ops;
    ECode ecRet = ReadyOps(&ops);
    if ((ops & sOP_READ) == sOP_READ)
        *readable = TRUE;
    else
        *readable = FALSE;
    return ecRet;
}

ECode SelectionKey::IsWritable(
    /* [out] */ Boolean* writable)
{
    Int32 ops;
    ECode ecRet = ReadyOps(&ops);
    *writable = (ops & sOP_WRITE) == sOP_WRITE;
    return ecRet;
}

} // namespace Channels
} // namespace IO
} // snamespace Elastos
