
#ifndef __ELASTOS_DROID_MEDIA_CMEDIAFILE_H__
#define __ELASTOS_DROID_MEDIA_CMEDIAFILE_H__

#include "_CMediaFile.h"
#include "ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMediaFile)
{
public:
    CMediaFile();

    ~CMediaFile();

    static CARAPI_(void) AddFileType(
        /* [in] */ const String& extension,
        /* [in] */ Int32 fileType,
        /* [in] */ const String& mimeType);

    static CARAPI_(void) AddFileType(
        /* [in] */ const String& extension,
        /* [in] */ Int32 fileType,
        /* [in] */ const String& mimeType,
        /* [in] */ Int32 mtpFormatCode);

    CARAPI IsAudioFileType(
        /* [in] */ Int32 fileType,
        /* [out] */ Boolean* result);

    CARAPI IsVideoFileType(
        /* [in] */ Int32 fileType,
        /* [out] */ Boolean* result);

    CARAPI IsImageFileType(
        /* [in] */ Int32 fileType,
        /* [out] */ Boolean* result);

    CARAPI IsPlayListFileType(
        /* [in] */ Int32 fileType,
        /* [out] */ Boolean* result);

    CARAPI IsDrmFileType(
        /* [in] */ Int32 fileType,
        /* [out] */ Boolean* result);

    CARAPI GetFileType(
        /* [in] */ const String& path,
        /* [out] */ IMediaFileType** result);

    CARAPI IsMimeTypeMedia(
        /* [in] */ const String& mimeType,
        /* [out] */ Boolean* result);

    // generates a title based on file name
    CARAPI GetFileTitle(
        /* [in] */ const String& path,
        /* [out] */ String* result);

    CARAPI GetFileTypeForMimeType(
        /* [in] */ const String& mimeType,
        /* [out] */ Int32* result);

    CARAPI GetMimeTypeForFile(
        /* [in] */ const String& path,
        /* [out] */ String* result);

    CARAPI GetFormatCode(
        /* [in] */ const String& fileName,
        /* [in] */ const String& mimeType,
        /* [out] */ Int32* result);

    CARAPI GetMimeTypeForFormatCode(
        /* [in] */ Int32 formatCode,
        /* [out] */ String* result);

private:
    static CARAPI_(Boolean) IsWMAEnabled();

    static CARAPI_(Boolean) IsWMVEnabled();

    static CARAPI_(Boolean) InitStatic();

    static CARAPI_(Boolean) InitHashMap();

private:
    // Audio file types
    static const Int32 FIRST_AUDIO_FILE_TYPE; // = IMediaFile::FILE_TYPE_MP3;
    static const Int32 LAST_AUDIO_FILE_TYPE; // = IMediaFile::FILE_TYPE_FLAC;

    // MIDI file types
    static const Int32 FIRST_MIDI_FILE_TYPE; // = IMediaFile::FILE_TYPE_MID;
    static const Int32 LAST_MIDI_FILE_TYPE; // = IMediaFile::FILE_TYPE_IMY;

    // Video file types
    static const Int32 FIRST_VIDEO_FILE_TYPE; // = IMediaFile::FILE_TYPE_MP4;
    static const Int32 LAST_VIDEO_FILE_TYPE; // = IMediaFile::FILE_TYPE_WEBM;

    // More video file types
    static const Int32 FIRST_VIDEO_FILE_TYPE2; // = IMediaFile::FILE_TYPE_MP2PS;
    static const Int32 LAST_VIDEO_FILE_TYPE2; // = IMediaFile::FILE_TYPE_CEDARV;

    // More audio file types
    static const Int32 FILE_TYPE_CEDARA; // = 300;

    // Image file types
    static const Int32 FIRST_IMAGE_FILE_TYPE; // = IMediaFile::FILE_TYPE_JPEG;
    static const Int32 LAST_IMAGE_FILE_TYPE; // = IMediaFile::FILE_TYPE_WEBP;

    // Playlist file types
    static const Int32 FIRST_PLAYLIST_FILE_TYPE; // = IMediaFile::FILE_TYPE_M3U;
    static const Int32 LAST_PLAYLIST_FILE_TYPE; // = IMediaFile::FILE_TYPE_HTTPLIVE;

    // Drm file types
    static const Int32 FIRST_DRM_FILE_TYPE; // = IMediaFile::FILE_TYPE_FL;
    static const Int32 LAST_DRM_FILE_TYPE; // = IMediaFile::FILE_TYPE_FL;

    static HashMap<String, AutoPtr<IMediaFileType> > sFileTypeMap;
    static HashMap<String, Int32> sMimeTypeMap;
    static HashMap<String, Int32> sFileTypeToFormatMap;
    static HashMap<String, Int32> sMimeTypeToFormatMap;
    static HashMap<Int32, String> sFormatToMimeTypeMap;

    static Boolean mInit;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CMEDIAFILE_H__
