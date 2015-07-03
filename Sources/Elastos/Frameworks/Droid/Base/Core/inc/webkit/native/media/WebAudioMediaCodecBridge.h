
#ifndef __WEBAUDIOMEDIACODECBRIDGE_H__
#define __WEBAUDIOMEDIACODECBRIDGE_H__

// import android.content.Context;
// import android.media.MediaCodec;
// import android.media.MediaCodec.BufferInfo;
// import android.media.MediaExtractor;
// import android.media.MediaFormat;
// import android.os.ParcelFileDescriptor;
// import android.util.Log;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

// import java.io.File;
// import java.nio.ByteBuffer;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

//@JNINamespace("media")
class WebAudioMediaCodecBridge
{
public:
    static const String LOG_TAG;
    // TODO(rtoy): What is the correct timeout value for reading
    // from a file in memory?
    static const Int64 TIMEOUT_MICROSECONDS = 500;

private:
    //@CalledByNative
    static CARAPI_(String) CreateTempFile(
        /* [in] */ IContext* ctx);

    //@CalledByNative
    static CARAPI_(Boolean) DecodeAudioFile(
        /* [in] */ IContext* ctx,
        /* [in] */ Int64 nativeMediaCodecBridge,
        /* [in] */ Int32 inputFD,
        /* [in] */ Int64 dataSize);

    static CARAPI_(void) NativeOnChunkDecoded(
        /* [in] */ Int64 nativeWebAudioMediaCodecBridge,
        /* [in] */ IByteBuffer* buf,
        /* [in] */ Int32 size,
        /* [in] */ Int32 inputChannelCount,
        /* [in] */ Int32 outputChannelCount);

    static CARAPI_(void) NativeInitializeDestination(
        /* [in] */ Int64 nativeWebAudioMediaCodecBridge,
        /* [in] */ Int32 inputChannelCount,
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int64 durationMicroseconds);
};

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__WEBAUDIOMEDIACODECBRIDGE_H__
