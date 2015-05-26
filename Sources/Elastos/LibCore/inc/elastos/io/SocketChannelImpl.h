#ifndef __ELASTOS_IO_SOCKETCHANNELIMPL_H__
#define __ELASTOS_IO_SOCKETCHANNELIMPL_H__

#include "SocketChannel.h"
#include "CFileDescriptor.h"
#include "CInetSocketAddress.h"
#include "Elastos.CoreLibrary_server.h"
#include "Socket.h"

using Elastos::IO::IFileDescriptor;
using Elastos::IO::IFileDescriptorChannel;
using Elastos::IO::CFileDescriptor;
using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;
using Elastos::IO::CFileDescriptor;
using Elastos::IO::Channels::SocketChannel;
using Elastos::Net::CInetSocketAddress;
using Elastos::Net::IInetAddress;
using Elastos::Net::IInetSocketAddress;

namespace Elastos {
namespace IO {

class SocketChannelInputStream;
class SocketChannelOutputStream;
class SocketAdapter;

class SocketChannelImpl
    : public SocketChannel
    , public IFileDescriptorChannel
{
public:
    /*
     * Constructor for creating a connected socket channel.
     */
    SocketChannelImpl(
        /* [in] */ ISelectorProvider* provider);

    /*
     * Constructor for creating an optionally connected socket channel.
     */
    SocketChannelImpl(
        /* [in] */ ISelectorProvider* provider,
        /* [in] */ Boolean connect);

    /*
     * Constructor for use by Pipe.SinkChannel and Pipe.SourceChannel.
     */
    SocketChannelImpl(
        /* [in] */ ISelectorProvider* provider,
        /* [in] */ IFileDescriptor* existingFd);

    /*
     * Getting the internal Socket If we have not the socket, we create a new
     * one.
     */
    CARAPI GetSocket(
        /* [out] */ ISocket** socket);

    CARAPI IsConnected(
        /* [out] */ Boolean* ret);

    CARAPI IsConnectionPending(
        /* [out] */ Boolean* ret);

    CARAPI Connect(
        /* [in] */ ISocketAddress* addr,
        /* [out] */ Boolean* ret);

    CARAPI FinishConnect(
        /* [out] */ Boolean* ret);

    CARAPI ReadByteBuffer(
        /* [in] */ IByteBuffer* dst,
        /* [out] */ Int32* ret);

    CARAPI ReadBytesBufferEx(
        /* [in] */ ArrayOf<IByteBuffer*>& target,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 len,
        /* [out] */ Int32* ret);

    CARAPI WriteByteBuffer(
        /* [in] */ IByteBuffer* src);

    CARAPI WriteBytesBufferEx(
        /* [in] */ ArrayOf<IByteBuffer*>& source,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 len,
        /* [out] */ Int32* ret);

    /*
     * Get local address.
     */
    CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** addr);

    /*
     * Get the fd.
     */
    CARAPI GetFD(
        /* [out] */ IFileDescriptor** dstcriptor);

protected:
    void SetConnected();

    void SetBound(
        /* [in] */ Boolean flag);

    void FinishAccept();

    /*
     * Shared by this class and DatagramChannelImpl, to do the address transfer
     * and check.
     */
    static CARAPI ValidateAddress(
        /* [in] */ ISocketAddress* addr,
        /* [out] */ IInetSocketAddress** sockAddr);

    /*
     * Do really closing action here.
     */
    CARAPI ImplCloseSelectableChannel();

    CARAPI ImplConfigureBlocking(
        /* [in] */ Boolean block);

private:
    CARAPI InitLocalAddressAndPort();

    CARAPI ReadImpl(
        /* [in] */ IByteBuffer* dst,
        /* [out] */ Int32* ret);

    CARAPI WriteImpl(
        /* [in] */ IByteBuffer* src,
        /* [out] */ Int32* ret);

    /*
     * Status check, open and "connected", when read and write.
     */
    CARAPI CheckOpenConnected();

    /*
     * Status check, open and "unconnected", before connection.
     */
    CARAPI CheckUnconnected();

private:
    const static Int32 SOCKET_STATUS_UNINITITLIZED = -1;

    // Status before connect.
    const static Int32 SOCKET_STATUS_UNCONNECTED;

    // Status connection pending.
    const static Int32 SOCKET_STATUS_PENDING;

    // Status after connection success.
    const static Int32 SOCKET_STATUS_CONNECTED;

    // Status closed.
    const static Int32 SOCKET_STATUS_CLOSED;

    CFileDescriptor* mFileDescriptor;

    // Our internal Socket.
    SocketAdapter* mSocket;

    // The address to be connected.
    CInetSocketAddress* mConnectAddress;

    IInetAddress* mLocalAddress;

    Int32 mLocalPort;

    Int32 mStatus;

    volatile Boolean mIsBound;
    Object mReadLock;
    Object mWriteLock;
};

/*
 * Adapter classes for internal socket.
 */
class SocketAdapter : public Socket
{
public:
    virtual ~SocketAdapter()
    {

    }

    SocketChannel* GetChannel()
    {
        return mChannel;
    }

    Boolean IsBound()
    {
        return mChannel->mIsBound;
    }

    Boolean IsConnected()
    {
        Boolean ret;
        mChannel->IsConnected(&ret);
        return ret;
    }

    ECode GetLocalAddress(IInetAddress** address)
    {
        return mChannel->GetLocalAddress(address);
    }

    ECode Connect(ISocketAddress* remoteAddr, Int32 timeout)
    {
        Boolean bRet;
        ECode ecRet;
        ecRet = mChannel->IsBlocking(&bRet);
        assert(NOERROR == ecRet);
        if (!bRet)
        {
            return E_ILLEGAL_BLOCKING_MODE_EXCEPTION;
        }

        if (!IsConnected())
        {
            return E_ALREADY_CONNECTED_EXCEPTION;
        }

        Socket::ConnectEx(remoteAddr, timeout);
        mChannel->InitLocalAddressAndPort();
        Socket::IsConnected(&bRet);
        if (bRet)
        {
            mChannel->SetConnected();
            Socket::IsBound(&bRet);
            mChannel->mIsBound = bRet;
        }

        return NOERROR;
    }

    ECode Bind(ISocketAddress* localAddr)
    {
        Boolean bRet;
        mChannel->IsConnected(&bRet);
        if (bRet)
            return E_ALREADY_CONNECTED_EXCEPTION;

        if (mChannel->mStatus == SocketChannelImpl::SOCKET_STATUS_PENDING)
            return E_CONNECTION_PENDING_EXCEPTION;

        Socket::Bind (localaAddr);
        mChannel->InitLocalAddressAndPort();
        mChannel->mIsBound = TRUE;
        return NOERROR;
    }

    ECode Close()
    {
        printf("WARNING: synchronized  has not implemented\n");
        Boolean bRet;
        ECode ecRet = mChannel->IsOpen(&bRet);
        assert(NOERROR == ecRet);

        if (bRet)
        {
            ecRet = mChannel->Close();
        }
        else
        {
            ecRet = Socket::Close();
        }

        mChannel->mStatus = SocketChannelImpl::SOCKET_STATUS_CLOSED;
        return ecRet;
    }

    ECode GetInputStream(InputStream** inputStream)
    {
        ECode ecRet = CheckOpenAndConnected();
        if (NOERROR != ecRet)
            return ecRet;

        Boolean bRet;
        IsInputShutdown(&bRet);
        if (bRet)
            return E_SOCKET_EXCEPTION;

        (*inputStream) = new SocketChannelInputStream(mChannel);
        return NOERROR;
    }

    ECode CheckOpenAndConnected()
    {
        Boolean bRet;
        mChannel->IsOpen(&bRet);
        if (!bRet)
            return E_SOCKET_EXCEPTION;

        mChannel->IsConnected(&bRet);
        if (!bRet)
            return E_SOCKET_EXCEPTION;

        return NOERROR;
    }

    CFileDescriptor* GetFileDescriptor()
    {
        printf("ERROR: Socket has not implemented GetFileDescriptor\n");
        assert(0);
        return 0;
    }

protected:
    PlainSocketImpl* SocketImpl()
    {
        return mSocketImpl;
    }

private:
    SocketChannelImpl* mChannel;
    PlainSocketImpl* mSocketImpl;
};


/*
 * This output stream delegates all operations to the associated channel.
 * Throws an IllegalBlockingModeException if the channel is in non-blocking
 * mode when performing write operations.
 */
class SocketChannelOutputStream : public OutputStream
{
public:
    SocketChannelOutputStream(SocketChannel* channel)
            : mChannel(new AutoPtr(channel))
    {
        assert(0 != channel);
    }

    ECode Close()
    {
        return mChannel->Close();
    }

    ECode WriteBytesBufferEx(ArrayOf<IByteBuffer*> & buffer, Int32 offset,
            Int32 byteCount)
    {
        printf("ERROR: Not Implemented Yet\n");
        return E_NOT_IMPLEMENTED;
    }

    ECode Write(Int32 oneByte)
    {
        printf("ERROR: Not Implemented Yet\n");
        return E_NOT_IMPLEMENTED;
    }

protected:

private:
        AutoPtr<SocketChannel> mChannel;
};

/*
 * This input stream delegates all operations to the associated channel.
 * Throws an IllegalBlockingModeException if the channel is in non-blocking
 * mode when performing read operations.
*/
class SocketChannelInputStream : public InputStream
{
public:
    SocketChannelInputStream(SocketChannel* channel)
        : mChannel(new AutoPtr(channel))
    {
        assert(0 != channel);
    }

    ECode Close()
    {
        return mChannel->Close();
    }

    ECode Read(Int32* ret)
    {
        printf("ERROR: Not Implemented Yet\n");
        return E_NOT_IMPLEMENTED;
    }

    ECode ReadBytesBufferEx(ArrayOf<IByteBuffer*> & buffer, Int32 offset, Int32 byteCount, Int32* ret)
    {
        printf("ERROR: Not Implemented Yet\n");
        return E_NOT_IMPLEMENTED;
    }

protected:

private:
    AutoPtr<SocketChannel> mChannel;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_SOCKETCHANNELIMPL_H__
