
#include "ext/frameworkext.h"
#include "graphics/CMovieHelper.h"
#include "graphics/Movie.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CMovieHelper::DecodeFile(
    /* [in] */ const String& pathName,
    /* [out] */ IMovie** movie)
{
    VALIDATE_NOT_NULL(movie);

    return Movie::DecodeFile(pathName, movie);
}

ECode CMovieHelper::DecodeStream(
    /* [in] */ IInputStream* is,
    /* [out] */ IMovie** movie)
{
    VALIDATE_NOT_NULL(movie);

    return Movie::DecodeStream(is, movie);
}

ECode CMovieHelper::DecodeByteArray(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ IMovie** movie)
{
    VALIDATE_NOT_NULL(movie);

    return Movie::DecodeByteArray(data, offset, length, movie);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
