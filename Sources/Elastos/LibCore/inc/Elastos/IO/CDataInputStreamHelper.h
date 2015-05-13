
#ifndef __CDATAINPUTSTREAMHELPER_H__
#define __CDATAINPUTSTREAMHELPER_H__

#include "_CDataInputStreamHelper.h"

namespace Elastos {
namespace IO {

CarClass(CDataInputStreamHelper)
{
public:
    CARAPI ReadUTF(
        /* [in] */ IDataInput* dataInput,
        /* [out] */ String* str);
};

} // namespace IO
} // namespace Elastos

#endif //__CDATAINPUTSTREAMHELPER_H__
