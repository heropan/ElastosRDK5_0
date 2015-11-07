
#ifndef __ELASTOS_DROID_GRAPHICS_NATIVEPICTURE_H__
#define __ELASTOS_DROID_GRAPHICS_NATIVEPICTURE_H__

#include "SkPicture.h"
#include "SkPictureRecorder.h"
#include "SkRefCnt.h"
#include "SkTemplates.h"

class SkStream;
class SkWStream;

namespace Elastos {
namespace Droid {
namespace Graphics {

class NativeCanvas;

// Skia's SkPicture class has been split into an SkPictureRecorder
// and an SkPicture. AndroidPicture recreates the functionality
// of the old SkPicture interface by flip-flopping between the two
// new classes.
class NativePicture {
public:
    explicit NativePicture(
        /* [in] */ const NativePicture* src = NULL);

    NativeCanvas* beginRecording(
        /* [in] */ int width,
        /* [in] */ int height);

    void endRecording();

    int width() const;

    int height() const;

    static NativePicture* CreateFromStream(
        /* [in] */ SkStream* stream);

    void serialize(
        /* [in] */ SkWStream* stream) const;

    void draw(
        /* [in] */ NativeCanvas* canvas);

private:
    int mWidth;
    int mHeight;
    SkAutoTUnref<const SkPicture> mPicture;
    SkAutoTDelete<SkPictureRecorder> mRecorder;

    // Make a copy of a picture that is in the midst of being recorded. The
    // resulting picture will have balanced saves and restores.
    SkPicture* makePartialCopy() const;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_NATIVEPICTURE_H__
