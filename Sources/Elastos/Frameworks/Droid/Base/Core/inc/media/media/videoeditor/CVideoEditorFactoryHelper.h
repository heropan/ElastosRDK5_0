
#ifndef __CVIDEOEDITORFACTORYHELPER_H__
#define __CVIDEOEDITORFACTORYHELPER_H__

#include "_CVideoEditorFactoryHelper.h"
#include <ext/frameworkext.h>

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

/**
 * The VideoEditorFactory class must be used to instantiate VideoEditor objects
 * by creating a new project {@link #create(String)} or by loading an
 * existing project {@link #load(String)}.
 * {@hide}
 */
CarClass(CVideoEditorFactoryHelper)
{
public:
    /**
     * This is the factory method for creating a new VideoEditor instance.
     *
     * @param projectPath The path where all VideoEditor internal
     *            files are stored. When a project is deleted the application is
     *            responsible for deleting the path and its contents.
     *
     * @return The VideoEditor instance
     *
     * @throws IOException if path does not exist or if the path can
     *             not be accessed in read/write mode
     */
    /*static*/
    CARAPI Create(
        /* [in] */ const String& projectPath,
        /* [out] */ IVideoEditor** result);

    /**
     * This is the factory method for instantiating a VideoEditor from the
     * internal state previously saved with the
     * {@link VideoEditor#save(String)} method.
     *
     * @param projectPath The path where all VideoEditor internal files
     *            are stored. When a project is deleted the application is
     *            responsible for deleting the path and its contents.
     * @param generatePreview if set to true the
     *      {@link MediaEditor#generatePreview(MediaProcessingProgressListener
     *             listener)}
     *      will be called internally to generate any needed transitions.
     *
     * @return The VideoEditor instance
     *
     * @throws IOException if path does not exist or if the path can
     *             not be accessed in read/write mode or if one of the resource
     *             media files cannot be retrieved
     */
    //static
    CARAPI Load(
        /* [in] */ const String& projectPath,
        /* [in] */ Boolean generatePreview,
        /* [out] */ IVideoEditor** result);
};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __CVIDEOEDITORFACTORYHELPER_H__

