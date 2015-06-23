#ifndef __AMRINPUTSTREAM_H__
#define __AMRINPUTSTREAM_H__

#include <ext/frameworkext.h>
#include <elastos/io/InputStream.h>

using Elastos::IO::InputStream;
using Elastos::IO::IInputStream;

namespace Elastos {
namespace Droid {
namespace Media {

class AmrInputStream : public InputStream
{
public:
    AmrInputStream();

    virtual ~AmrInputStream();

    //@Override
    virtual CARAPI Read(
        /* [out] */ Int32* value);

    virtual CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    virtual CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int32* number);

    virtual CARAPI Close();

protected:
    ECode Init(
        /* [in] */ IInputStream* inputStream);

private:
    //
    // AudioRecord JNI interface
    //
    static CARAPI GsmAmrEncoderNew(
        /* [out] */ Int32* gae);

    static CARAPI GsmAmrEncoderInitialize(
        /* [in] */ Int32 gae);

    static CARAPI GsmAmrEncoderEncode(
        /* [in] */ Int32 gae,
        /* [in] */ ArrayOf<Byte>* pcm,
        /* [in] */ Int32 pcmOffset,
        /* [in] */ ArrayOf<Byte>* amr,
        /* [in] */ Int32 amrOffset,
        /* [out] */ Int32* number);

    static CARAPI_(void) GsmAmrEncoderCleanup(
        /* [in] */ Int32 gae);

    static CARAPI_(void) GsmAmrEncoderDelete(
        /* [in] */ Int32 gae);

private:
    static String TAG;

    // pcm input stream
    AutoPtr<IInputStream> mInputStream;

    // native handle
    Int32 mGae;

    // result amr stream
    AutoPtr<ArrayOf<Byte> > mBuf;
    Int32 mBufIn;
    Int32 mBufOut;

    // helper for bytewise read()
    AutoPtr<ArrayOf<Byte> > mOneByte;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CAMRINPUTSTREAM_H__
