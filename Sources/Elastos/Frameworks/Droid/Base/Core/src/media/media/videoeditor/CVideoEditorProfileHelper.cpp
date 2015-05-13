#include "CVideoEditorProfileHelper.h"
#include "CVideoEditorProfile.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

ECode CVideoEditorProfileHelper::Get(
    /* [out] */ IVideoEditorProfile** result)
{
    return CVideoEditorProfile::Get(result);
}

ECode CVideoEditorProfileHelper::GetExportProfile(
    /* [in] */ Int32 vidCodec,
    /* [out] */ Int32* result)
{
    return CVideoEditorProfile::GetExportProfile(vidCodec, result);
}

ECode CVideoEditorProfileHelper::GetExportLevel(
    /* [in] */ Int32 vidCodec,
    /* [out] */ Int32* result)
{
    return CVideoEditorProfile::GetExportLevel(vidCodec, result);
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
