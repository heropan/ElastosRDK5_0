
#ifndef __ELASTOS_DROID_GRAPHICS_MOVIE_H__
#define __ELASTOS_DROID_GRAPHICS_MOVIE_H__

#include "Elastos.Droid.Core_server.h"
#include <Elastos.CoreLibrary.h>
#include <elastos/core/Object.h>

using Elastos::Core::Object;
using Elastos::IO::IInputStream;

namespace Elastos {
namespace Droid {
namespace Graphics {

class Movie
    : public Object
    , public IMovie
{
public:
    CAR_INTERFACE_DECL();

    Movie(
        /* [in] */ Int64 nativeMovie);

    virtual ~Movie();

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI IsOpaque(
        /* [out] */ Boolean* isOpaque);

    CARAPI GetDuration(
        /* [out] */ Int32* height);

    CARAPI SetTime(
        /* [in] */ Int32 relativeMilliseconds,
        /* [out] */ Boolean* isSet);

    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    CARAPI Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Float x,
        /* [in] */ Float y);

    static CARAPI DecodeStream(
        /* [in] */ IInputStream* is,
        /* [out] */ IMovie** movie);

    static CARAPI DecodeByteArray(
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ IMovie** movie);

    static CARAPI DecodeFile(
        /* [in] */ const String& pathName,
        /* [out] */ IMovie** movie);

private:
    static CARAPI DecodeTempStream(
        /* [in] */ IInputStream* is,
        /* [out] */IMovie** movie);

    static CARAPI_(void) NativeDestructor(
        /* [in] */ Int64 nativeMovie);

    static CARAPI_(AutoPtr<IMovie>) NativeDecodeAsset(
        /* [in] */ Int64 asset);

    static CARAPI_(AutoPtr<IMovie>) NativeDecodeStream(
        /* [in] */ IInputStream* is);

private:
    Int64 mNativeMovie;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_MOVIE_H__
