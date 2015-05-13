#ifndef __UlawEncoderInputStream_H__
#define __UlawEncoderInputStream_H__

#include <elastos/io/InputStream.h>

using Elastos::IO::IInputStream;
using Elastos::IO::InputStream;

namespace Elastos {
namespace Droid {
namespace Speech {
namespace Srec {

/**
 * InputStream which transforms 16 bit pcm data to ulaw data.
 *
 * Not yet ready to be supported, so
 * @hide
 */
class UlawEncoderInputStream
    : public InputStream
{
public:
    static CARAPI_(void) Encode(
        /* [in] */ ArrayOf<Byte>* pcmBuf,
        /* [in] */ Int32 pcmOffset,
        /* [in] */ ArrayOf<Byte>* ulawBuf,
        /* [in] */ Int32 ulawOffset,
        /* [in] */ Int32 length,
        /* [in] */ Int32 max);

    /**
     * Compute the maximum of the absolute value of the pcm samples.
     * The return value can be used to set ulaw encoder scaling.
     * @param pcmBuf array containing 16 bit pcm data.
     * @param offset offset of start of 16 bit pcm data.
     * @param length number of pcm samples (not number of input bytes)
     * @return maximum abs of pcm data values
     */
    static CARAPI_(Int32) MaxAbsPcm(
        /* [in] */ ArrayOf<Byte>* pcmBuf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    /**
     * Create an InputStream which takes 16 bit pcm data and produces ulaw data.
     * @param in InputStream containing 16 bit pcm data.
     * @param max pcm value corresponding to maximum ulaw value.
     */
    UlawEncoderInputStream(
        /* [in] */ IInputStream* in,
        /* [in] */ Int32 max);

    CARAPI_(void) Init(
        /* [in] */ IInputStream* in,
        /* [in] */ Int32 max);

    //@Override
    CARAPI ReadBytesEx(
        /* [out] */ ArrayOf<Byte>* buf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    //@Override
    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buf,
        /* [out] */ Int32* number);

    //@Override
    CARAPI Read(
        /* [out] */ Int32* value);

    //@Override
    CARAPI Close();

    //@Override
    CARAPI Available(
        /* [out] */ Int32* number);// throws IOException

protected:
    UlawEncoderInputStream();

private:
    static const CString TAG;// = "UlawEncoderInputStream";

    static const Int32 MAX_ULAW;// = 8192;
    static const Int32 SCALE_BITS;// = 16;

    AutoPtr<IInputStream> mIn;

    Int32 mMax;// = 0;

    AutoPtr< ArrayOf<Byte> > mBuf;// = ArrayOf<Byte>::Alloc(1024);
    Int32 mBufCount;// = 0; // should be 0 or 1

    AutoPtr< ArrayOf<Byte> > mOneByte;// = ArrayOf<Byte>::Alloc(1);
};

} // namespace Srec
} // namespace Speech
} // namepsace Droid
} // namespace Elastos

#endif // __UlawEncoderInputStream_H__