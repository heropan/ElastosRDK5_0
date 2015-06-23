
#ifndef __ELASTOS_DROID_NET_CLOCALSOCKETIMPL_H__
#define __ELASTOS_DROID_NET_CLOCALSOCKETIMPL_H__

#include "Elastos.Droid.Core_server.h"
#include <elastos/io/InputStream.h>
#include <elastos/io/OutputStream.h>

using Elastos::Core::Mutex;
using Elastos::IO::InputStream;
using Elastos::IO::IInputStream;
using Elastos::IO::OutputStream;
using Elastos::IO::IOutputStream;
using Elastos::IO::IFlushable;
using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Net {

class LocalSocketImpl : public ElRefBase
{
    friend class CLocalSocket;
    friend class CLocalServerSocket;

private:
    /**
     * An input stream for local sockets. Needed because we may
     * need to read ancillary data.
     */
    class SocketInputStream
        : public ElRefBase
        , public IInputStream
        , public InputStream
    {
    public:
        SocketInputStream(
            /* [in] */ LocalSocketImpl* owner);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [in] */ InterfaceID* pIID);

        CARAPI Available(
            /* [out] */ Int32* result);

        CARAPI Close();

        CARAPI Read(
            /* [out] */ Int32* result);

        CARAPI ReadBytes(
            /* [out] */ ArrayOf<Byte>* b,
            /* [out] */ Int32* result);

        CARAPI ReadBytes(
            /* [out] */ ArrayOf<Byte>* b,
            /* [in] */ Int32 off,
            /* [in] */ Int32 len,
            /* [out] */ Int32* result);

        CARAPI Mark(
            /* [in] */ Int32 readLimit);

        CARAPI IsMarkSupported(
            /* [out] */ Boolean* supported);

        CARAPI Reset();

        CARAPI Skip(
            /* [in] */ Int64 byteCount,
            /* [out] */ Int64* number);

        CARAPI GetLock(
            /* [out] */ IInterface** lockobj);
    public:
        LocalSocketImpl* mOwner;
    };

    /**
     * An output stream for local sockets. Needed because we may
     * need to read ancillary data.
     */
    class SocketOutputStream
        : public ElRefBase
        , public IOutputStream
        , public IFlushable
        , public OutputStream
    {
    public:
        SocketOutputStream(
            /* [in] */ LocalSocketImpl* owner);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [in] */ InterfaceID* pIID);

        CARAPI Close();

        CARAPI Write(
            /* [in] */ Int32 b);

        CARAPI WriteBytes(
            /* [in] */ const ArrayOf<Byte>& b);

        CARAPI WriteBytes(
            /* [in] */ const ArrayOf<Byte>& b,
            /* [in] */ Int32 off,
            /* [in] */ Int32 len);

        CARAPI CheckError(
            /* [out] */ Boolean* hasError);

        CARAPI Flush();

        CARAPI GetLock(
            /* [out] */ IInterface** lockobj);
    public:
        LocalSocketImpl* mOwner;
    };

public:
    LocalSocketImpl();

    LocalSocketImpl(
        /* [in] */ IFileDescriptor* fd);

    ~LocalSocketImpl();

    CARAPI_(String) ToString();

    CARAPI Create (
        /* [in] */ Boolean stream);

    CARAPI Close();

    CARAPI Bind(
        /* [in] */ ILocalSocketAddress* endpoint);

    CARAPI GetOption(
        /* [in] */ Int32 optID,
        /* [out] */ IInterface** value);

    CARAPI SetOption(
        /* [in] */ Int32 optID,
        /* [in] */ IInterface* value);

    CARAPI_(void) SetFileDescriptorsForSend(
        /* [in] */ ArrayOf<IFileDescriptor*>* fds);

    CARAPI_(AutoPtr< ArrayOf<IFileDescriptor*> >) GetAncillaryFileDescriptors();

    CARAPI GetPeerCredentials(
        /* [out] */ ICredentials** result);

    CARAPI GetSockAddress(
        /* [out] */ ILocalSocketAddress** result);

protected:
    CARAPI Connect(
        /* [in] */ ILocalSocketAddress* address,
        /* [in] */ Int32 timeout);

    CARAPI Listen(
        /* [in] */ Int32 backlog);

    CARAPI Accept(
        /* [in] */ LocalSocketImpl* s);

    CARAPI GetInputStream(
        /* [out] */ IInputStream** is);

    CARAPI GetOutputStream(
        /* [out] */ IOutputStream** os);

    CARAPI Available(
        /* [out] */ Int32* size);

    CARAPI ShutdownInput();

    CARAPI ShutdownOutput();

    CARAPI_(AutoPtr<IFileDescriptor>) GetFileDescriptor();

    CARAPI_(Boolean) SupportsUrgentData();

    CARAPI SendUrgentData(
        /* [in] */ Int32 data);

private:
    CARAPI NativeAvailable(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ Int32* size);

    CARAPI NativeClose(
        /* [in] */ IFileDescriptor* fd);

    CARAPI NativeRead(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ Int32* result);

    CARAPI NativeReadba(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ Int32* result);

    CARAPI NativeWriteba(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 off,
        /* [in] */ Int32 len,
        /* [in] */ IFileDescriptor* fd);

    CARAPI NativeWrite(
        /* [in] */ Int32 b,
        /* [in] */ IFileDescriptor* fd);

    CARAPI ConnectLocal(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ const String& name,
        /* [in] */ Int32 namespaceId);

    CARAPI BindLocal(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ const String& name,
        /* [in] */ Int32 namespaceId);

    CARAPI NativeCreate(
        /* [in] */ Boolean stream,
        /* [out] */ IFileDescriptor** fd);

    CARAPI NativeListen(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 backlog);

    CARAPI Shutdown(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Boolean shutdownInput);

    CARAPI NativeGetPeerCredentials(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ ICredentials** credentials);

    CARAPI NativeGetOption(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 optID,
        /* [out] */ Int32* value);

    CARAPI NativeSetOption(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 optID,
        /* [in] */ Int32 boolValue,
        /* [in] */ Int32 intValue);

    CARAPI NativeAccept(
        /* [in] */ IFileDescriptor* fileDescriptor,
        /* [in] */ LocalSocketImpl* s,
        /* [out] */ IFileDescriptor** fd);

/*private*/public:
    AutoPtr<SocketInputStream> mFis;
    AutoPtr<SocketOutputStream> mFos;
    Mutex mReadMonitor;
    Mutex mWriteMonitor;

    /** null if closed or not yet created */
    AutoPtr<IFileDescriptor> mFd;

    // These fields are accessed by native code;
    /** file descriptor array received during a previous read */
    AutoPtr< ArrayOf<IFileDescriptor*> > mInboundFileDescriptors;
    /** file descriptor array that should be written during next write */
    AutoPtr< ArrayOf<IFileDescriptor*> > mOutboundFileDescriptors;

    Mutex mLock;
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_NET_CLOCALSOCKETIMPL_H__
