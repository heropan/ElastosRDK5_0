#ifndef __ELASTOS_IO_CHANNELS_ELASTOS_IO_DATAGRAMCHANNEL_H__
#define __ELASTOS_IO_CHANNELS_ELASTOS_IO_DATAGRAMCHANNEL_H__

#include "Elastos.CoreLibrary.IO.h"
#include "ByteBuffer.h"
#include "SelectorProvider.h"
#include "AbstractSelectableChannel.h"

using Elastos::IO::Channels::IByteChannel;
using Elastos::IO::Channels::Spi::SelectorProvider;
using Elastos::IO::Channels::Spi::AbstractSelectableChannel;
using Elastos::Net::IDatagramSocket;
using Elastos::Net::ISocketAddress;

namespace Elastos {
namespace IO {
namespace Channels {

class DatagramChannel
    : public AbstractSelectableChannel
    , public IDatagramChannel
    , public IByteChannel
    , public IScatteringByteChannel
    , public IGatheringByteChannel
{
public:
    CAR_INTERFACE_DECL()

    /**
     * Creates an opened and not-connected datagram channel.
     * <p>
     * This channel is created by calling the <code>openDatagramChannel</code>
     * method of the default {@link SelectorProvider} instance.
     *
     * @return the new channel which is open but not connected.
     * @throws IOException
     *             if some I/O error occurs.
     */
    static CARAPI Open(
        /* [out] */ IDatagramChannel** channel);

    /**
     * Gets the valid operations of this channel. Datagram channels support read
     * and write operations, so this method returns (
     * <code>SelectionKey.OP_READ</code> | <code>SelectionKey.OP_WRITE</code> ).
     *
     * @see java.nio.channels.SelectableChannel#validOps()
     * @return valid operations in bit-set.
     */
    CARAPI GetValidOps(
        /* [out] */ Int32* value);

    /**
     * Returns the related datagram socket of this channel, which does not
     * define additional public methods to those defined by
     * {@link DatagramSocket}.
     *
     * @return the related DatagramSocket instance.
     */
    // remove it temporily
    virtual CARAPI GetSocket(
        /* [out] */ IDatagramSocket** socket) = 0;

    /**
     * Returns whether this channel's socket is connected or not.
     *
     * @return <code>true</code> if this channel's socket is connected;
     *         <code>false</code> otherwise.
     */
    virtual CARAPI IsConnected(
        /* [out] */ Boolean* isConnected) = 0;

    /**
     * Connects the socket of this channel to a remote address, which is the
     * only communication peer for getting and sending datagrams after being
     * connected.
     * <p>
     * This method can be called at any time without affecting the read and
     * write operations being processed at the time the method is called. The
     * connection status does not change until the channel is disconnected or
     * closed.
     * <p>
     * This method executes the same security checks as the connect method of
     * the {@link DatagramSocket} class.
     *
     * @param address
     *            the address to be connected to.
     * @return this channel.
     * @throws ClosedChannelException
     *             if the channel is already closed.
     * @throws AsynchronousCloseException
     *             if the channel is closed by another thread while this method
     *             is in operation.
     * @throws ClosedByInterruptException
     *             if another thread interrupts the calling thread while the
     *             operation is in progress. The calling thread will have the
     *             interrupt state set and the channel will be closed.
     * @throws IOException
     *             if some other I/O error occurs.
     */
    virtual CARAPI Connect(
        /* [in] */ ISocketAddress* address,
        /* [out] */ IDatagramChannel** channel) = 0;
    /**
     * Disconnects the socket of this channel, which has been connected before
     * in order to send and receive datagrams.
     * <p>
     * This method can be called at any time without affecting the read and
     * write operations being underway. It does not have any effect if the
     * socket is not connected or the channel is closed.
     *
     * @return this channel.
     * @throws IOException
     *             some other I/O error occurs.
     */
    virtual CARAPI Disconnect() = 0;

    /**
     * Gets a datagram from this channel.
     * <p>
     * This method transfers a datagram from the channel into the target byte
     * buffer. If this channel is in blocking mode, it waits for the datagram
     * and returns its address when it is available. If this channel is in
     * non-blocking mode and no datagram is available, it returns {@code null}
     * immediately. The transfer starts at the current position of the buffer,
     * and if there is not enough space remaining in the buffer to store the
     * datagram then the part of the datagram that does not fit is discarded.
     * <p>
     * This method can be called at any time and it will block if there is
     * another thread that has started a read operation on the channel.
     * <p>
     * This method executes the same security checks as the receive method of
     * the {@link DatagramSocket} class.
     *
     * @param target
     *            the byte buffer to store the received datagram.
     * @return the address of the datagram if the transfer is performed, or null
     *         if the channel is in non-blocking mode and no datagram is
     *         available.
     * @throws ClosedChannelException
     *             if the channel is already closed.
     * @throws AsynchronousCloseException
     *             if the channel is closed by another thread while this method
     *             is in operation.
     * @throws ClosedByInterruptException
     *             if another thread interrupts the calling thread while the
     *             operation is in progress. The calling thread will have the
     *             interrupt state set and the channel will be closed.
     * @throws IOException
     *             some other I/O error occurs.
     */

    // remove it temporarily
    virtual CARAPI Receive(
        /* [in] */ IByteBuffer* target,
        /* [out] */ ISocketAddress** address) = 0;

    /**
     * Sends a datagram through this channel. The datagram consists of the
     * remaining bytes in {@code source}.
     * <p>
     * If this channel is in blocking mode then the datagram is sent as soon as
     * there is enough space in the underlying output buffer. If this channel is
     * in non-blocking mode then the datagram is only sent if there is enough
     * space in the underlying output buffer at that moment. The transfer action
     * is just like a regular write operation.
     * <p>
     * This method can be called at any time and it will block if another thread
     * has started a send operation on this channel.
     * <p>
     * This method executes the same security checks as the send method of the
     * {@link DatagramSocket} class.
     *
     * @param source
     *            the byte buffer with the datagram to be sent.
     * @param address
     *            the destination address for the datagram.
     * @return the number of bytes sent. This is the number of bytes remaining
     *         in {@code source} or zero if the channel is in non-blocking mode
     *         and there is not enough space for the datagram in the underlying
     *         output buffer.
     * @throws ClosedChannelException
     *             if the channel is already closed.
     * @throws AsynchronousCloseException
     *             if the channel is closed by another thread while this method
     *             is in operation.
     * @throws ClosedByInterruptException
     *             if another thread interrupts the calling thread while the
     *             operation is in progress. The calling thread will have the
     *             interrupt state set and the channel will be closed.
     * @throws IOException
     *             some other I/O error occurs.
     */
    virtual CARAPI Send(
        /* [in] */ IByteBuffer* source,
        /* [in] */ ISocketAddress* address,
        /* [out] */ Int32* number) = 0;
    /**
     * Reads a datagram from this channel into the byte buffer.
     * <p>
     * The precondition for calling this method is that the channel is connected
     * and the incoming datagram is from the connected address. If the buffer is
     * not big enough to store the datagram, the part of the datagram that does
     * not fit in the buffer is discarded. Otherwise, this method has the same
     * behavior as the {@code read} method in the {@link ReadableByteChannel}
     * interface.
     *
     * @see java.nio.channels.ReadableByteChannel#read(java.nio.ByteBuffer)
     * @param target
     *            the byte buffer to store the received datagram.
     * @return a non-negative number as the number of bytes read, or -1 as the
     *         read operation reaches the end of stream.
     * @throws NotYetConnectedException
     *             if the channel is not connected yet.
     * @throws ClosedChannelException
     *             if the channel is already closed.
     * @throws AsynchronousCloseException
     *             if the channel is closed by another thread while this method
     *             is in operation.
     * @throws ClosedByInterruptException
     *             if another thread interrupts the calling thread while the
     *             operation is in progress. The calling thread will have the
     *             interrupt state set and the channel will be closed.
     * @throws IOException
     *             some other I/O error occurs.
     */
    virtual CARAPI Read(
        /* [in] */ IByteBuffer* target,
        /* [out] */ Int32* nRead) = 0;

    /**
     * Reads a datagram from this channel into an array of byte buffers.
     * <p>
     * The precondition for calling this method is that the channel is connected
     * and the incoming datagram is from the connected address. If the buffers
     * do not have enough remaining space to store the datagram, the part of the
     * datagram that does not fit in the buffers is discarded. Otherwise, this
     * method has the same behavior as the {@code read} method in the
     * {@link ScatteringByteChannel} interface.
     *
     * @see java.nio.channels.ScatteringByteChannel#read(java.nio.ByteBuffer[],
     *      int, int)
     * @param targets
     *            the byte buffers to store the received datagram.
     * @param offset
     *            a non-negative offset in the array of buffers, pointing to the
     *            starting buffer to store the bytes transferred, must not be
     *            bigger than {@code targets.length}.
     * @param length
     *            a non-negative length to indicate the maximum number of
     *            buffers to be filled, must not be bigger than
     *            {@code targets.length - offset}.
     * @return a non-negative number as the number of bytes read, or -1 if the
     *         read operation reaches the end of stream.
     * @throws NotYetConnectedException
     *             if the channel is not connected yet.
     * @throws ClosedChannelException
     *             if the channel is already closed.
     * @throws AsynchronousCloseException
     *             if the channel is closed by another thread while this method
     *             is in operation.
     * @throws ClosedByInterruptException
     *             if another thread interrupts the calling thread while the
     *             operation is in progress. The calling thread will have the
     *             interrupt state set and the channel will be closed.
     * @throws IOException
     *             some other I/O error occurs.
     */
    virtual CARAPI Read(
        /* [in] */ ArrayOf<IByteBuffer*>* targets,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Int64* nRead) = 0;

    /**
     * Reads a datagram from this channel into an array of byte buffers.
     * <p>
     * The precondition for calling this method is that the channel is connected
     * and the incoming datagram is from the connected address. If the buffers
     * do not have enough remaining space to store the datagram, the part of the
     * datagram that does not fit in the buffers is discarded. Otherwise, this
     * method has the same behavior as the {@code read} method in the
     * {@link ScatteringByteChannel} interface.
     *
     * @see java.nio.channels.ScatteringByteChannel#read(java.nio.ByteBuffer[])
     * @param targets
     *            the byte buffers to store the received datagram.
     * @return a non-negative number as the number of bytes read, or -1 if the
     *         read operation reaches the end of stream.
     * @throws NotYetConnectedException
     *             if the channel is not connected yet.
     * @throws ClosedChannelException
     *             if the channel is already closed.
     * @throws AsynchronousCloseException
     *             if the channel is closed by another thread while this method
     *             is in operation.
     * @throws ClosedByInterruptException
     *             if another thread interrupts the calling thread while the
     *             operation is in progress. The calling thread will have the
     *             interrupt state set and the channel will be closed.
     * @throws IOException
     *             some other I/O error occurs.
     */
    CARAPI Read(
        /* [in] */ ArrayOf<IByteBuffer*>* targets,
        /* [out] */ Int64* nRead);

    /**
     * Writes a datagram from the byte buffer to this channel.
     * <p>
     * The precondition of calling this method is that the channel is connected
     * and the datagram is sent to the connected address. Otherwise, this method
     * has the same behavior as the {@code write} method in the
     * {@link WritableByteChannel} interface.
     *
     * @see java.nio.channels.WritableByteChannel#write(java.nio.ByteBuffer)
     * @param source
     *            the byte buffer as the source of the datagram.
     * @return a non-negative number of bytes written.
     * @throws NotYetConnectedException
     *             if the channel is not connected yet.
     * @throws ClosedChannelException
     *             if the channel is already closed.
     * @throws AsynchronousCloseException
     *             if the channel is closed by another thread while this method
     *             is in operation.
     * @throws ClosedByInterruptException
     *             if another thread interrupts the calling thread while the
     *             operation is in progress. The calling thread will have the
     *             interrupt state set and the channel will be closed.
     * @throws IOException
     *             some other I/O error occurs.
     */
    virtual CARAPI Write(
        /* [in] */ IByteBuffer *source,
        /* [out] */ Int32* nWrite) = 0;

    /**
     * Writes a datagram from the byte buffers to this channel.
     * <p>
     * The precondition of calling this method is that the channel is connected
     * and the datagram is sent to the connected address. Otherwise, this method
     * has the same behavior as the {@code write} method in the
     * {@link GatheringByteChannel} interface.
     *
     * @see java.nio.channels.GatheringByteChannel#write(java.nio.ByteBuffer[],
     *      int, int)
     * @param sources
     *            the byte buffers as the source of the datagram.
     * @param offset
     *            a non-negative offset in the array of buffers, pointing to the
     *            starting buffer to be retrieved, must be no larger than
     *            {@code sources.length}.
     * @param length
     *            a non-negative length to indicate the maximum number of
     *            buffers to be submitted, must be no bigger than
     *            {@code sources.length - offset}.
     * @return the number of bytes written. If this method is called, it returns
     *         the number of bytes that where remaining in the byte buffers. If
     *         the channel is in non-blocking mode and there was not enough
     *         space for the datagram in the buffer, it may return zero.
     * @throws NotYetConnectedException
     *             if the channel is not connected yet.
     * @throws ClosedChannelException
     *             if the channel is already closed.
     * @throws AsynchronousCloseException
     *             if the channel is closed by another thread while this method
     *             is in operation.
     * @throws ClosedByInterruptException
     *             if another thread interrupts the calling thread while the
     *             operation is in progress. The calling thread will have the
     *             interrupt state set and the channel will be closed.
     * @throws IOException
     *             some other I/O error occurs.
     */
    virtual CARAPI Write(
        /* [in] */ ArrayOf<IByteBuffer*>* sources,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int64* nWrite) = 0;

    /**
     * Writes a datagram from the byte buffers to this channel.
     * <p>
     * The precondition of calling this method is that the channel is connected
     * and the datagram is sent to the connected address. Otherwise, this method
     * has the same behavior as the write method in the
     * {@link GatheringByteChannel} interface.
     *
     * @see java.nio.channels.GatheringByteChannel#write(java.nio.ByteBuffer[])
     * @param sources
     *            the byte buffers as the source of the datagram.
     * @return the number of bytes written. If this method is called, it returns
     *         the number of bytes that where remaining in the byte buffer. If
     *         the channel is in non-blocking mode and there was not enough
     *         space for the datagram in the buffer, it may return zero.
     * @throws NotYetConnectedException
     *             if the channel is not connected yet.
     * @throws ClosedChannelException
     *             if the channel is already closed.
     * @throws AsynchronousCloseException
     *             if the channel is closed by another thread while this method
     *             is in operation.
     * @throws ClosedByInterruptException
     *             if another thread interrupts the calling thread while the
     *             operation is in progress. The calling thread will have the
     *             interrupt state set and the channel will be closed.
     * @throws IOException
     *             some other I/O error occurs.
     */
    CARAPI Write(
        /* [in] */ ArrayOf<IByteBuffer*>* sources,
        /* [out] */ Int64* nWrite);

protected:
    DatagramChannel();

    CARAPI constructor(
        /* [in] */ ISelectorProvider* selectorProvider);

    ~DatagramChannel();
};

} // namespace Channels
} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_CHANNELS_ELASTOS_IO_DATAGRAMCHANNEL_H__
