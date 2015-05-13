#ifndef __CMEDIAPLAYER_SUBINFO_H__
#define __CMEDIAPLAYER_SUBINFO_H__

#include "_CMediaPlayerSubInfo.h"

namespace Elastos {
namespace Droid {
namespace Media {


CarClass(CMediaPlayerSubInfo)
{
public:
    CARAPI constructor(
        /* [in] */ ArrayOf<Byte>* name,
        /* [in] */ const String& charset,
        /* [in] */ Int32 type);

    CARAPI SetName(
        /* [in] */ ArrayOf<Byte>* name);

    CARAPI GetName(
        /* [out, callee] */ ArrayOf<Byte>** name);

    CARAPI SetCharset(
        /* [in] */ const String& charset);

    CARAPI GetCharset(
        /* [out] */ String* charset);

    CARAPI SetType(
        /* [in] */ Int32 type);

    CARAPI GetType(
        /* [out] */ Int32* type);

public:
    AutoPtr<ArrayOf<Byte> > mName;
    String mCharset;
    Int32 mType;    // text or bitmap
};


} // namespace media
} // namepsace Droid
} // namespace Elastos

#endif //__CMEDIAPLAYER_SUBINFO_H__
