#ifndef __ELASTOS_IO_CHANNELS_SELECTOR_H__
#define __ELASTOS_IO_CHANNELS_SELECTOR_H__

#include <Elastos.Core_server.h>
#include <elastos/Thread.h>
#include <elastos/Mutex.h>

using Elastos::Core::IRunnable;
using Elastos::Core::Threading::Mutex;
using Elastos::Utility::ISet;
using Elastos::IO::Channels::Spi::ISelectorProvider;

namespace Elastos {
namespace IO {
namespace Channels {

class Selector
{
public:
    virtual ~Selector()
    {}

    /**
     * Closes this selector. Ongoing calls to the {@code select} methods of this
     * selector will get interrupted. This interruption behaves as if the
     * {@link #wakeup()} method of this selector is called. After this, all keys
     * that are still valid are invalidated and their channels are unregistered.
     * All resources held by this selector are released.
     * <p>
     * Any further attempt of using this selector after this method has been
     * called (except calling {@link #close()} or {@link #wakeup()}) results in
     * a {@link ClosedSelectorException} being thrown.
     *
     * @throws IOException
     *             if an I/O error occurs.
     */
    virtual CARAPI Close() = 0;

    /**
     * Indicates whether this selector is open.
     *
     * @return {@code true} if this selector is not closed, {@code false}
     *         otherwise.
     */
    virtual CARAPI IsOpen(
        /* [out] */ Boolean* isOpen) = 0;

    /**
     * Gets the set of registered keys. The set is immutable and is not thread-
     * safe.
     *
     * @return the set of registered keys.
     */
    virtual CARAPI Keys(
        /* [out] */ ISet** keySet) = 0;

    /**
     * Gets the provider of this selector.
     *
     * @return the provider of this selector.
     */
    virtual CARAPI Provider(
        /* [out] */ ISelectorProvider** provider) = 0;

    /**
     * Detects if any of the registered channels is ready for I/O operations
     * according to its {@link SelectionKey interest set}. This method does not
     * return until at least one channel is ready, {@link #wakeup()} is
     * invoked or the calling thread is interrupted.
     *
     * @return the number of channels that are ready for operation.
     * @throws IOException
     *             if an I/O error occurs.
     * @throws ClosedSelectorException
     *             if the selector is closed.
     */
    virtual CARAPI Select(
        /* [out] */ Int32* ret) = 0;

    /**
     * Detects if any of the registered channels is ready for I/O operations
     * according to its {@link SelectionKey interest set}. This method does not
     * return until at least one channel is ready, {@link #wakeup()} is invoked,
     * the calling thread is interrupted or the specified {@code timeout}
     * expires.
     *
     * @param timeout
     *            the non-negative timeout in millisecond; 0 will block forever
     *            if no channels get ready.
     * @return the number of channels that are ready for operation.
     * @throws ClosedSelectorException
     *             if the selector is closed.
     * @throws IllegalArgumentException
     *             if the given timeout argument is less than zero.
     * @throws IOException
     *             if an I/O error occurs.
     */
    virtual CARAPI SelectEx(
        /* [in] */ Int64 timeout,
        /* [out] */ Int32* ret) = 0;

    /**
     * Gets the selection keys whose channels are ready for operation. The set
     * is not thread-safe and no keys may be added to it. Removing keys is
     * allowed.
     *
     * @return the selection keys whose channels are ready for operation.
     * @throws ClosedSelectorException
     *             if the selector is closed.
     */
    virtual CARAPI SelectedKeys(
        /* [out] */ ISet** keySet) = 0;

    /**
     * Detects if any of the registered channels is ready for I/O operations
     * according to its {@link SelectionKey interest set}. This operation will
     * return immediately.
     *
     * @return the number of channels that are ready for operation, 0 if none is
     *         ready.
     * @throws IOException
     *             if an I/O error occurrs.
     * @throws ClosedSelectorException
     *             if the selector is closed.
     */
    virtual CARAPI SelectNow(
        /* [out] */ Int32* ret) = 0;

    /**
     * Forces blocked {@code select} operations to return immediately.
     * <p>
     * If no {@code select} operation is blocked when {@code wakeup()} is called
     * then the next {@code select} operation will return immediately. This can
     * be undone by a call to {@code selectNow()}; after calling
     * {@code selectNow()}, a subsequent call of {@code select} can block
     * again.
     *
     * @return this selector.
     * @throws ClosedSelectorException
     *             if the selector is closed.
     */
    virtual CARAPI Wakeup(
        /* [out] */ ISelector** selector) = 0;
};

} // namespace Channels
} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_CHANNELS_SELECTOR_H__
