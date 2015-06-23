#include "CVideoEditorFactoryHelper.h"
#include "CVideoEditorImpl.h"

using Elastos::IO::IFile;
using Elastos::IO::CFile;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

ECode CVideoEditorFactoryHelper::Create(
    /* [in] */ const String& projectPath,
    /* [out] */ IVideoEditor** result)
{
    /*
     *  If the project path does not exist create it
     */
    AutoPtr<IFile> dir;
    CFile::New(projectPath, (IFile**)&dir);
    Boolean b;
    dir->Exists(&b);
    if (!b) {
        dir->Mkdirs(&b);
        if (!b) {
            // throw new FileNotFoundException("Cannot create project path: " + projectPath);
            return E_FILE_NOT_FOUND_EXCEPTION;
        }
        else {
            /*
             * Create the file which hides the media files
             * from the media scanner
             */
            AutoPtr<IFile> file;
            CFile::New(dir, String(".nomedia"), (IFile**)&file);
            file->CreateNewFile(&b);
            if (!b) {
                // throw new FileNotFoundException("Cannot create file .nomedia");
                return E_FILE_NOT_FOUND_EXCEPTION;
            }
        }
    }

    AutoPtr<IVideoEditor> videoEditor;
    CVideoEditorImpl::New(projectPath, (IVideoEditor**)&videoEditor);
    *result = videoEditor;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CVideoEditorFactoryHelper::Load(
    /* [in] */ const String& projectPath,
    /* [in] */ Boolean generatePreview,
    /* [out] */ IVideoEditor** result)
{
    AutoPtr<IVideoEditor> videoEditor;
    CVideoEditorImpl::New(projectPath, (IVideoEditor**)&videoEditor);
    if (generatePreview) {
        videoEditor->GeneratePreview(NULL);
    }
    *result = videoEditor;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
