#ifndef __Annotation_H__
#define __Annotation_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Text {

class Annotation {
public:
    Annotation(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    Annotation(
        /* [in] */ IParcel* src);

    CARAPI_(Int32) GetSpanTypeId();

    CARAPI_(Int32) DescribeContents();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI_(String) GetKey();

    CARAPI_(String) GetValue();

protected:
    void Init(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    void Init(
        /* [in] */ IParcel* src);

    Annotation();

private:
    String mKey;
    String mValue;
};


} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__Annotation_H__
