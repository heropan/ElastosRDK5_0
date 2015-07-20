
#ifndef __TEMPORARYBUFFER_H__
#define __TEMPORARYBUFFER_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

class TemporaryBuffer
{
public:
    static CARAPI_(AutoPtr< ArrayOf<Char32> >) Obtain(
       /* [in] */ Int32 len);

    static CARAPI_(void) Recycle(
       /*  [in] */ ArrayOf<Char32>* temp);

private:
    static AutoPtr< ArrayOf<Char32> > sTemp;
    static Object sClassLock;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __CTEMPORARYBUFFER_H__
