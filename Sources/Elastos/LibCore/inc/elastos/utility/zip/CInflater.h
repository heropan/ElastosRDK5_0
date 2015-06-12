
#ifndef __ELASTOS_UTILITY_CINFLATER_H__
#define __ELASTOS_UTILITY_CINFLATER_H__

#include "_Elastos_Utility_Zip_CInflater.h"
#include "Zip.h"
#include "Object.h"

using Elastos::Core::Object;
using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Utility {
namespace Zip {

CarClass(CInflater)
    , public Object
    , public IInflater
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CInflater();

    ~CInflater();

    /**
     * Releases resources associated with this {@code Inflater}. Any unused
     * input or output is discarded. This method should be called explicitly in
     * order to free native resources as soon as possible. After {@code end()} is
     * called, other methods will typically throw {@code IllegalStateException}.
     */
    // synchronized
    CARAPI End();

    /**
     * Indicates if the {@code Inflater} has inflated the entire deflated
     * stream. If deflated bytes remain and {@code needsInput()} returns {@code
     * true} this method will return {@code false}. This method should be
     * called after all deflated input is supplied to the {@code Inflater}.
     *
     * @return {@code true} if all input has been inflated, {@code false}
     *         otherwise.
     */
    // synchronized
    CARAPI Finished(
        /* [out] */ Boolean* finished);

    /**
     * Returns the {@link Adler32} checksum of the bytes inflated so far, or the
     * checksum of the preset dictionary if {@link #needsDictionary} returns true.
     */
    // synchronized
    CARAPI GetAdler(
        /* [out] */ Int32* checksum);

    /**
     * Returns the total number of bytes read by the {@code Inflater}. This
     * method is the same as {@link #getTotalIn} except that it returns a
     * {@code long} value instead of an integer.
     */
    // synchronized
    CARAPI GetBytesRead(
        /* [out] */ Int64* number);

    /**
     * Returns a the total number of bytes written by this {@code Inflater}. This
     * method is the same as {@code getTotalOut} except it returns a
     * {@code long} value instead of an integer.
     */
    // synchronized
    CARAPI GetBytesWritten(
        /* [out] */ Int64* number);

    /**
     * Returns the number of bytes of current input remaining to be read by this
     * inflater.
     */
    // synchronized
    CARAPI GetRemaining(
        /* [out] */ Int32* number);

    /**
     * Returns the total number of bytes of input read by this {@code Inflater}. This
     * method is limited to 32 bits; use {@link #getBytesRead} instead.
     */
    // synchronized
    CARAPI GetTotalIn(
        /* [out] */ Int32* number);

    /**
     * Returns the total number of bytes written to the output buffer by this {@code
     * Inflater}. The method is limited to 32 bits; use {@link #getBytesWritten} instead.
     */
    // synchronized
    CARAPI GetTotalOut(
        /* [out] */ Int32* number);

    /**
     * Inflates bytes from the current input and stores them in {@code buf}.
     *
     * @param buf
     *            the buffer where decompressed data bytes are written.
     * @return the number of bytes inflated.
     * @throws DataFormatException
     *             if the underlying stream is corrupted or was not compressed
     *             using a {@code Deflater}.
     */
    CARAPI Inflate(
        /* [in] */ ArrayOf<Byte>* buf,
        /* [out] */ Int32* number);

    /**
     * Inflates up to {@code byteCount} bytes from the current input and stores them in
     * {@code buf} starting at {@code offset}.
     *
     * @throws DataFormatException
     *             if the underlying stream is corrupted or was not compressed
     *             using a {@code Deflater}.
     * @return the number of bytes inflated.
     */
    // synchronized
    CARAPI Inflate(
        /* [in] */ ArrayOf<Byte>* buf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* number);

    /**
     * Returns true if the input bytes were compressed with a preset
     * dictionary. This method should be called if the first call to {@link #inflate} returns 0,
     * to determine whether a dictionary is required. If so, {@link #setDictionary}
     * should be called with the appropriate dictionary before calling {@code
     * inflate} again. Use {@link #getAdler} to determine which dictionary is required.
     */
    // synchronized
    CARAPI NeedsDictionary(
        /* [out] */ Boolean* result);

    /**
     * Returns true if {@link #setInput} must be called before inflation can continue.
     */
    // synchronized
    CARAPI NeedsInput(
        /* [out] */ Boolean* result);

    /**
     * Resets this {@code Inflater}. Should be called prior to inflating a new
     * set of data.
     */
    // synchronized
    CARAPI Reset();

    /**
     * Sets the preset dictionary to be used for inflation to {@code dictionary}.
     * See {@link #needsDictionary} for details.
     */
    CARAPI SetDictionary(
        /* [in] */ ArrayOf<Byte>* buf);

    /**
     * Sets the preset dictionary to be used for inflation to a subsequence of {@code dictionary}
     * starting at {@code offset} and continuing for {@code byteCount} bytes. See {@link
     * #needsDictionary} for details.
     */
    // synchronized
    CARAPI SetDictionary(
        /* [in] */ ArrayOf<Byte>* buf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount);

    /**
     * Sets the current input to to be decompressed. This method should only be
     * called if {@link #needsInput} returns {@code true}.
     */
    CARAPI SetInput(
        /* [in] */ ArrayOf<Byte>* buf);

    /**
     * Sets the current input to to be decompressed. This method should only be
     * called if {@link #needsInput} returns {@code true}.
     */
    // synchronized
    CARAPI SetInput(
        /* [in] */ ArrayOf<Byte>* buf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount);

    /**
     * This constructor creates an inflater that expects a header from the input
     * stream. Use {@code Inflater(boolean)} if the input comes without a ZLIB
     * header.
     */
    CARAPI constructor();

    /**
     * This constructor allows to create an inflater that expects no header from
     * the input stream.
     *
     * @param noHeader
     *            {@code true} indicates that no ZLIB header comes with the
     *            input.
     */
    CARAPI constructor(
        /* [in] */ Boolean noHeader);

    // synchronized
    /* package */ CARAPI_(Int32) SetFileInput(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int64 offset,
        /* [in] */ Int32 byteCount);

private:
    CARAPI CreateStream(
        /* [in] */ Boolean noHeader);

    CARAPI_(void) EndImplLocked(
        /* [in] */ NativeZipStream* stream);

    CARAPI_(Int32) GetAdlerImplLocked(
        /* [in] */ NativeZipStream* stream);

    CARAPI_(Int64) GetTotalInImplLocked(
        /* [in] */ NativeZipStream* stream);

    CARAPI_(Int64) GetTotalOutImplLocked(
        /* [in] */ NativeZipStream* stream);

    CARAPI InflateImplLocked(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount,
        /* [out] */ ArrayOf<Byte>* buf,
        /* [in] */ NativeZipStream* stream,
        /* [out] */ Int32* result);

    CARAPI ResetImplLocked(
        /* [in] */ NativeZipStream* stream);

    CARAPI_(void) SetDictionaryImplLocked(
        /* [in] */ ArrayOf<Byte>* buf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount,
        /* [in] */ NativeZipStream* stream);

    CARAPI_(void) SetInputImplLocked(
        /* [in] */ ArrayOf<Byte>* buf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount,
        /* [in] */ NativeZipStream* stream);

    // BEGIN android-only
    CARAPI_(Int32) SetFileInputImplLocked(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int64 offset,
        /* [in] */ Int32 byteCount,
        /* [in] */ NativeZipStream* stream);

    CARAPI CheckOpen();

private:
    /* package */ Int32 mInLength;

    /* package */ Int32 mInRead; // Set by inflateImpl.
    Boolean mFinished; // Set by the inflateImpl native
    Boolean mNeedsDictionary; // Set by the inflateImpl native

    NativeZipStream* mStreamHandle;

    //CloseGuard guard = CloseGuard.get();

    static Object sLock;
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CINFLATER_H__
