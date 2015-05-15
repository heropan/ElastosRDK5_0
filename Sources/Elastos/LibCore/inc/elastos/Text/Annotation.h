#ifndef __ANNOTATION_H__
#define __ANNOTATION_H__

#include "cmdef.h"
#include "Elastos.Core_server.h"

namespace Elastos {
namespace Text {

class Annotation
{
public:

    CARAPI Init(
        /* [in] */ IInterface* attribute);

    virtual CARAPI GetValue(
        /* [out] */ IInterface** value);

    virtual CARAPI ToString(
        /* [out] */ String * str);

private:
    AutoPtr<IInterface> mValue;
};

} // namespace Text
} // namespace Elastos

#endif //__ANNOTATION_H__
