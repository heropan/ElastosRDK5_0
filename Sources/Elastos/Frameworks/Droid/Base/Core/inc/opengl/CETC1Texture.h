
#ifndef __CETC1TEXTURE_H__
#define __CETC1TEXTURE_H__

#include "_CETC1Texture.h"

using Elastos::IO::IByteBuffer;

namespace Elastos {
namespace Droid {
namespace Opengl {

CarClass(CETC1Texture)
{
public:
    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI GetData(
        /* [out] */ Elastos::IO::IByteBuffer** data);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Elastos::IO::IByteBuffer* data);

private:
    Int32 mWidth;
    Int32 mHeight;
    AutoPtr<IByteBuffer> mData;
};

} // namespace Opengl
} // namespace Droid
} // namespace Elastos

#endif // __CETC1TEXTURE_H__
