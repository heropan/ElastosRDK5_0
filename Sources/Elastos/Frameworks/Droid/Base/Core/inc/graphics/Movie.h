
#ifndef __ELASTOS_DROID_GRAPHICS_MOVIE_H__
#define __ELASTOS_DROID_GRAPHICS_MOVIE_H__

#include "Elastos.Droid.Core_server.h"
#include <Elastos.Core.h>

using Elastos::IO::IInputStream;

namespace Elastos {
namespace Droid {
namespace Graphics {

class Movie
    : public ElRefBase
    , public IMovie
{
public:
    Movie(
        /* [in] */ Int32 nativeMovie);

    ~Movie();

    CARAPI_(PInterface) Probe(
    /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

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

    CARAPI DrawEx(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Float x,
        /* [in] */ Float y);

    static CARAPI DecodeStream(
        /* [in] */ IInputStream* is,
        /* [out] */ IMovie** movie);

    static CARAPI DecodeByteArray(
        /* [in] */ const ArrayOf<Byte>& data,
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
        /* [in] */ Int32 nativeMovie);

private:
    Int32 mNativeMovie;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_MOVIE_H__
