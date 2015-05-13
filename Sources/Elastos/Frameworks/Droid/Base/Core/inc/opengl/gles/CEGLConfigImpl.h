#ifndef __CEGLCONFIGIMPL_H_
#define __CEGLCONFIGIMPL_H_

#include "_CEGLConfigImpl.h"

namespace Elastos {
namespace Droid {
namespace Opengl {
namespace Gles {

CarClass(CEGLConfigImpl)
{
public:
    CARAPI constructor(
        /* [in] */ Int32 config);

    CARAPI_(Int32) Get();

    CARAPI_(Int32) GetEGLConfig();

private:
    Int32 mEGLConfig;
};

} // namespace Gles
} // namespace Opengl
} // namespace Droid
} // namespace Elastos

#endif