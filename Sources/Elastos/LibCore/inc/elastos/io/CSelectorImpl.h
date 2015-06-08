#ifndef __ELASTOS_IO_CSELECTORIMPL_H__
#define __ELASTOS_IO_CSELECTORIMPL_H__

#include "_Elastos_IO_CSelectorImpl.h"
#include "AbstractSelector.h"
#include "SelectionKeyImpl.h"
#include <elastos/core/Thread.h>
#include <elastos/core/Object.h>

using Elastos::Core::Object;
using Elastos::Core::IRunnable;
using Elastos::Utility::ISet;
using Elastos::Utility::IUnsafeArrayList;
using Elastos::IO::Channels::Spi::ISelectorProvider;
using Elastos::IO::Channels::Spi::AbstractSelector;
using Elastos::IO::Channels::Spi::IAbstractSelector;
using Elastos::IO::Channels::ISelectionKey;
using Elastos::IO::Channels::ISelector;

namespace Elastos {
namespace IO {

CarClass(CSelectorImpl)
    , public Object
    , public AbstractSelector
    , public IAbstractSelector
    , public ISelector
{
    // private static class UnaddableSet<E> implements Set<E> {

    //     private final Set<E> set;

    //     UnaddableSet(Set<E> set) {
    //         this.set = set;
    //     }

    //     @Override
    //     public boolean equals(Object object) {
    //         return set.equals(object);
    //     }

    //     @Override
    //     public int hashCode() {
    //         return set.hashCode();
    //     }

    //     public boolean add(E object) {
    //         throw new UnsupportedOperationException();
    //     }

    //     public boolean addAll(Collection<? extends E> c) {
    //         throw new UnsupportedOperationException();
    //     }

    //     public void clear() {
    //         set.clear();
    //     }

    //     public boolean contains(Object object) {
    //         return set.contains(object);
    //     }

    //     public boolean containsAll(Collection<?> c) {
    //         return set.containsAll(c);
    //     }

    //     public boolean isEmpty() {
    //         return set.isEmpty();
    //     }

    //     public Iterator<E> iterator() {
    //         return set.iterator();
    //     }

    //     public boolean remove(Object object) {
    //         return set.remove(object);
    //     }

    //     public boolean removeAll(Collection<?> c) {
    //         return set.removeAll(c);
    //     }

    //     public boolean retainAll(Collection<?> c) {
    //         return set.retainAll(c);
    //     }

    //     public int size() {
    //         return set.size();
    //     }

    //     public Object[] toArray() {
    //         return set.toArray();
    //     }

    //     public <T> T[] toArray(T[] a) {
    //         return set.toArray(a);
    //     }
    // };

public:
    CAR_INTERFACE_DECL()

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ ISelectorProvider* selectorProvider);

    CARAPI Cancel(
        /* [in] */ ISelectionKey* key);

    CARAPI Register(
        /* [in] */  AbstractSelectableChannel* channel,
        /* [in] */  Int32 operations,
        /* [in] */  IObject* obj,
        /* [out] */ ISelectionKey** returnKey);

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
    CARAPI Close();

    /**
     * Indicates whether this selector is open.
     *
     * @return {@code true} if this selector is not closed, {@code false}
     *         otherwise.
     */
    CARAPI IsOpen(
        /* [out] */ Boolean* isOpen);

     /**
     * Gets the set of registered keys. The set is immutable and is not thread-
     * safe.
     *
     * @return the set of registered keys.
     */
    CARAPI GetKeys(
        /* [out] */ ISet** keySet);

     /**
     * Gets the provider of this selector.
     *
     * @return the provider of this selector.
     */
    CARAPI GetProvider(
        /* [out] */ ISelectorProvider** provider);

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
    CARAPI Select(
        /* [out] */ Int32* ret);

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
    CARAPI Select(
        /* [in] */ Int64 timeout,
        /* [out] */ Int32*ret);


     /**
     * Gets the selection keys whose channels are ready for operation. The set
     * is not thread-safe and no keys may be added to it. Removing keys is
     * allowed.
     *
     * @return the selection keys whose channels are ready for operation.
     * @throws ClosedSelectorException
     *             if the selector is closed.
     */
    CARAPI SelectedKeys(
        /* [out] */ ISet** keySet);

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
    CARAPI SelectNow(
        /* [out] */ Int32* ret);

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
    CARAPI Wakeup(
        /* [out] */ ISelector** outsel);

protected:
    CARAPI ImplCloseSelector();

private:
    CARAPI CheckClosed();

    CARAPI SelectInternal(
        /* [in] */ Int64 timeout,
        /* [out] */ Int32* ret);

    void SetPollFd(
        /* [in] */ Int32 i,
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 events,
        /* [in] */ IInterface* object);

    void PreparePollFds();

    void EnsurePollFdsCapacity();

    CARAPI_(Int32) ProcessPollFds();

    Int32 DoCancel();

public:
    AutoPtr<IInterface> mKeysLock;

private:
    AutoPtr<ISet> mMutableKeys;

    /**
     * The unmodifiable set of keys as exposed to the user. This object is used
     * for synchronization.
     */
    AutoPtr<ISet> mUnmodifiableKeys;

    AutoPtr<ISet> mMutableSelectedKeys;

    /**
     * The unmodifiable set of selectable keys as seen by the user. This object
     * is used for synchronization.
     */
    AutoPtr<ISet> mSelectedKeys;

    AutoPtr<IFileDescriptor> mWakeupIn;
    AutoPtr<IFileDescriptor> mWakeupOut;

    AutoPtr<IUnsafeArrayList> mPollFds;

    // Used for emulating synchronized keyword
    Object mLock;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_CSELECTORIMPL_H__
