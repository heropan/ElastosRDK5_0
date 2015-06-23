
#ifndef __PARCELFILEDESCRIPTOR_H__
#define __PARCELFILEDESCRIPTOR_H__

#include "Elastos.Droid.Core_server.h"
#include <elastos/Core/Object.h>

using Elastos::Core::Object;
using Elastos::Core::LockObject;
using Elastos::IO::IFile;
using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Os {

class ParcelFileDescriptor
{
public:
    /**
     * An InputStream you can create on a ParcelFileDescriptor, which will
     * take care of calling {@link ParcelFileDescriptor#close
     * ParcelFileDescriptor.close()} for you when the stream is closed.
     */
    class AutoCloseInputStream //, public FileInputStream
    {
    public:
        AutoCloseInputStream();

        CARAPI Close();

        CARAPI GetFD(
            /* [out] */ IFileDescriptor** fd);

        CARAPI Available(
            /* [out] */ Int32* number);

        CARAPI Mark(
            /* [in] */ Int32 readLimit);

        CARAPI IsMarkSupported(
            /* [out] */ Boolean* supported);

        CARAPI Read(
            /* [out] */ Int32* value);

        CARAPI ReadBytes(
            /* [out] */ ArrayOf<Byte>* buffer,
            /* [out] */ Int32* number);

        CARAPI ReadBytes(
            /* [out] */ ArrayOf<Byte>* buffer,
            /* [in] */ Int32 offset,
            /* [in] */ Int32 length,
            /* [out] */ Int32* number);

        CARAPI Reset();

        CARAPI Skip(
            /* [in] */ Int64 byteCount,
            /* [out] */ Int64* number);

    protected:
        CARAPI Init(
            /* [in] */ IParcelFileDescriptor* fd);

        CARAPI_(AutoPtr<IInterface>) GetLock();

    protected:
        AutoPtr<LockObject> mLock;
    private:
        AutoPtr<IParcelFileDescriptor> mPfd;
        AutoPtr<IFileDescriptor> mFd;
        Boolean mShouldClose;
    };

    /**
     * An OutputStream you can create on a ParcelFileDescriptor, which will
     * take care of calling {@link ParcelFileDescriptor#close
     * ParcelFileDescriptor.close()} for you when the stream is closed.
     */
    class AutoCloseOutputStream //, public FileOutputStream
    {
    public:
        AutoCloseOutputStream();

        CARAPI Close();

        CARAPI Flush();

        CARAPI GetFD(
            /* [out] */ IFileDescriptor** fd);

        CARAPI Write(
            /* [in] */ Int32 oneByte);

        CARAPI WriteBytes(
            /* [in] */ const ArrayOf<Byte>& buffer);

        CARAPI WriteBytes(
            /* [in] */ const ArrayOf<Byte>& buffer,
            /* [in] */ Int32 offset,
            /* [in] */ Int32 count);

        CARAPI CheckError(
            /* [out] */ Boolean* hasError);

    protected:
        CARAPI Init(
            /* [in] */ IParcelFileDescriptor* fd);

        CARAPI_(AutoPtr<IInterface>) GetLock();

    protected:
        AutoPtr<LockObject> mLock;
    private:
        AutoPtr<IParcelFileDescriptor> mPfd;
        AutoPtr<IFileDescriptor> mFd;
        Boolean mShouldClose;

        /** File access mode */
        Int32 mMode;
    };

public:
    ParcelFileDescriptor();

    ~ParcelFileDescriptor();

    ParcelFileDescriptor(
        /* [in] */ IParcelFileDescriptor* descriptor);

    ParcelFileDescriptor(
        /* [in] */ IFileDescriptor* descriptor);

    /**
     * Create a new ParcelFileDescriptor accessing a given file.
     *
     * @param file The file to be opened.
     * @param mode The desired access mode, must be one of
     * {@link #MODE_READ_ONLY}, {@link #MODE_WRITE_ONLY}, or
     * {@link #MODE_READ_WRITE}; may also be any combination of
     * {@link #MODE_CREATE}, {@link #MODE_TRUNCATE},
     * {@link #MODE_WORLD_READABLE}, and {@link #MODE_WORLD_WRITEABLE}.
     *
     * @return Returns a new ParcelFileDescriptor pointing to the given
     * file.
     *
     * @throws FileNotFoundException Throws FileNotFoundException if the given
     * file does not exist or can not be opened with the requested mode.
     */
    static CARAPI Open(
        /* [in] */ IFile* file,
        /* [in] */ Int32 mode,
        /* [out] */ IParcelFileDescriptor** descriptor);

    /**
     * Take ownership of a raw native fd in to a new ParcelFileDescriptor.
     * The returned ParcelFileDescriptor now owns the given fd, and will be
     * responsible for closing it.  You must not close the fd yourself.
     *
     * @param fd The native fd that the ParcelFileDescriptor should adopt.
     *
     * @return Returns a new ParcelFileDescriptor holding a FileDescriptor
     * for the given fd.
     */
    static CARAPI AdoptFd(
        /* [in] */ Int32 fd,
        /* [out] */ IParcelFileDescriptor** descriptor);

    /**
     * Create two ParcelFileDescriptors structured as a data pipe.  The first
     * ParcelFileDescriptor in the returned array is the read side; the second
     * is the write side.
     */
    static CARAPI CreatePipe(
        /* [out, callee] */ ArrayOf<IParcelFileDescriptor*>** pfds);

    /**
    * Create a new ParcelFileDescriptor that is a dup of an existing
    * FileDescriptor.  This obeys standard POSIX semantics, where the
    * new file descriptor shared state such as file position with the
    * original file descriptor.
    */
    static CARAPI Dup(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ IParcelFileDescriptor** descriptor);

    virtual CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor** des);

    virtual CARAPI GetStatSize(
        /* [out] */ Int64* len);

    virtual CARAPI SeekTo(
        /* [in] */ Int64 startOffset,
        /* [out] */ Int64* toOffset);

    virtual CARAPI GetFd(
        /* [out] */ Int32* fd);

    /**
     * Close the ParcelFileDescriptor. This implementation closes the underlying
     * OS resources allocated to represent this stream.
     *
     * @throws IOException
     *             If an error occurs attempting to close this ParcelFileDescriptor.
     */
    virtual CARAPI Close();

    virtual CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

protected:
    CARAPI Init(
        /* [in] */ IParcelFileDescriptor* descriptor);

    CARAPI Init(
        /* [in] */ IFileDescriptor* descriptor);

private:
    static CARAPI CreatePipeNative(
        /* [in] */ ArrayOf<IFileDescriptor*>* outFds);

    CARAPI_(Int32) NativeGetFd();

    // Extracts the file descriptor from the specified socket and returns it un
    static CARAPI_(AutoPtr<IFileDescriptor>) GetFileDescriptorFromFdNoDup(
        /* [in] */ Int32 fd);

private:
    AutoPtr<IFileDescriptor> mFileDescriptor;
    volatile Boolean mClosed;

    /**
     * Wrapped {@link ParcelFileDescriptor}, if any. Used to avoid
     * double-closing {@link #mFileDescriptor}.
     */
    AutoPtr<IParcelFileDescriptor> mWrapped;

    // private final CloseGuard mGuard = CloseGuard.get();
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__PARCELFILEDESCRIPTOR_H__
