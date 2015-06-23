#include "CWaveformData.h"

using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IFile;
using Elastos::IO::CFile;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

CWaveformData::CWaveformData()
    : mFrameDurationMs(0)
    , mFramesCount(0)
{
}

ECode CWaveformData::constructor(
    /* [in] */ const String& audioWaveformFilename)
{
    if (audioWaveformFilename == NULL) {
        //throw new IllegalArgumentException("WaveformData : filename is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IFileInputStream> audioGraphFileReadHandle;

    //try {
    AutoPtr<IFile> audioGraphFileContext;
    CFile::New(audioWaveformFilename,(IFile**)&audioGraphFileContext);

    CFileInputStream::New(audioGraphFileContext,(IFileInputStream**)&audioGraphFileReadHandle);
    /*
     * Read frame duration
     */
    AutoPtr<ArrayOf<Byte> > tempFrameDuration = ArrayOf<Byte>::Alloc(4);

    Int32 num;
    audioGraphFileReadHandle->ReadBytes(tempFrameDuration.Get(), 0, 4, &num);

    Int32 tempFrameDurationMs = 0;
    Int32 tempFramesCounter = 0;
    for (Int32 i = 0; i < 4; i++) {
        tempFrameDurationMs = (tempFrameDurationMs << 8);
        tempFrameDurationMs = (tempFrameDurationMs | ((*tempFrameDuration)[i] & 0xff));
    }
    mFrameDurationMs = tempFrameDurationMs;

    /*
     * Read count
     */
    AutoPtr<ArrayOf<Byte> > tempFramesCount = ArrayOf<Byte>::Alloc(4);

    audioGraphFileReadHandle->ReadBytes(tempFramesCount.Get(), 0, 4, &num);
    for (Int32 i = 0; i < 4; i++) {
        tempFramesCounter = (tempFramesCounter << 8);
        tempFramesCounter = (tempFramesCounter | ((*tempFramesCount)[i] & 0xff));
    }
    mFramesCount = tempFramesCounter;

    /*
     *  Capture the graph values
     */
    mGains = ArrayOf<Int16>::Alloc(mFramesCount);

    for (Int32 i = 0; i < mFramesCount; i++) {
        Int32 data;
        audioGraphFileReadHandle->Read(&data);
        mGains->Set(i, (Int16)data);
    }
    //} finally {
    if (audioGraphFileReadHandle != NULL) {
        audioGraphFileReadHandle->Close();
    }
    //}
    return NOERROR;
}

ECode CWaveformData::GetFrameDuration(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mFrameDurationMs;
    return NOERROR;
}

ECode CWaveformData::GetFramesCount(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mFramesCount;
    return NOERROR;
}

ECode CWaveformData::GetFrameGains(
    /* [out] */ ArrayOf<Int16>** result)
{
    VALIDATE_NOT_NULL(result);

    *result = mGains;
    return NOERROR;
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
