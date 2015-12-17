
#ifndef __ELASTOS_DROID_MEDIA_CMEDIACODECINFO_H__
#define __ELASTOS_DROID_MEDIA_CMEDIACODECINFO_H__

#include "_Elastos_Droid_Media_CMediaCodecInfo.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/utility/Range.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Utility::IRational;
using Elastos::Droid::Utility::Range;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMediaCodecInfo)
    , public Object
    , public IMediaCodecInfo
{
private:
    class Feature
        : public Object
    {
    public:
        Feature(
            /* [in] */ const String& name,
            /* [in] */ Int32 value,
            /* [in] */ Boolean def)
            : mName(name)
            , mValue(value)
            , mDefault(def)
        {}

    public:
        String mName;
        Int32 mValue;
        Boolean mDefault;
    };

public:
    CMediaCodecInfo();

    virtual ~CMediaCodecInfo();

    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ Boolean isEncoder,
        /* [in] */ ArrayOf<IMediaCodecInfoCodecCapabilities*>* caps);

    /**
     * Retrieve the codec name.
     */
    CARAPI GetName(
        /* [out] */ String* result);

    /**
     * Query if the codec is an encoder.
     */
    CARAPI IsEncoder(
        /* [out] */ Boolean* result);

    /**
     * Query the media types supported by the codec.
     */
    CARAPI GetSupportedTypes(
        /* [out, callee] */ ArrayOf<String>** result);

    CARAPI GetCapabilitiesForType(
        /* [in] */ const String& type,
        /* [out] */ ICodecCapabilities** result);

    CARAPI MakeRegular(
        /* [out] */ IMediaCodecInfo** result);

private:
    static CARAPI CheckPowerOfTwo(
        /* [in] */ Int32 value,
        /* [in] */ const String& message,
        /* [out] */ Int32* result);

    static CARAPI_(void) Init();

private:
    // COMMON CONSTANTS
    static AutoPtr<Range<IInteger32> > POSITIVE_INTEGERS;// = Range.create(1, Integer.MAX_VALUE);
    static AutoPtr<Range<IInteger64> > POSITIVE_LONGS;// = Range.create(1l, Long.MAX_VALUE);
    static AutoPtr<Range<IRational> > POSITIVE_RATIONALS;// = Range.create(new Rational(1, Integer.MAX_VALUE), new Rational(Integer.MAX_VALUE, 1));
    static AutoPtr<Range<IInteger32> > SIZE_RANGE;// = Range.create(1, 32768);
    static AutoPtr<Range<IInteger32> > FRAME_RATE_RANGE;// = Range.create(0, 960);

    // found stuff that is not supported by framework (=> this should not happen)
    static const Int32 ERROR_UNRECOGNIZED; // = (1 << 0);
    // found profile/level for which we don't have capability estimates
    static const Int32 ERROR_UNSUPPORTED; // = (1 << 1);
    // have not found any profile/level for which we don't have capability estimate
    static const Int32 ERROR_NONE_SUPPORTED; // = (1 << 2);

    Boolean mIsEncoder;
    String mName;
    AutoPtr<IMap> mCaps;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_MEDIA_CMEDIACODECINFO_H__
