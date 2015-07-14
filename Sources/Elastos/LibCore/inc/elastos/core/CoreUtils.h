#ifndef __ELASTOS_CORE_COREUTILS_H__
#define __ELASTOS_CORE_COREUTILS_H__

#include <elastos/core/Object.h>

namespace Elastos {
namespace Core {

class CoreUtils
{
public:
    static CARAPI_(AutoPtr<IByte>) ConvertByte(
        /* [in] */ Byte value);

    static CARAPI_(AutoPtr<IChar32>) ConvertChar32(
        /* [in] */ Char32 value);

    static CARAPI_(AutoPtr<IBoolean>) Convert(
        /* [in] */ Boolean value);

    static CARAPI_(AutoPtr<IInteger16>) Convert(
        /* [in] */ Int16 value);

    static CARAPI_(AutoPtr<IInteger32>) Convert(
        /* [in] */ Int32 value);

    static CARAPI_(AutoPtr<IInteger64>) Convert(
        /* [in] */ Int64 value);

    static CARAPI_(AutoPtr<IFloat>) Convert(
        /* [in] */ Float value);

    static CARAPI_(AutoPtr<IDouble>) Convert(
        /* [in] */ Double value);

    static CARAPI_(AutoPtr<ICharSequence>) Convert(
        /* [in] */ const String & value);

    static CARAPI_(AutoPtr<ICharSequence>) Convert(
        /* [in] */ const char* str);

private:
    CoreUtils();
    ~CoreUtils();
};

} // namespace Core
} // namespace Elastos

using Elastos::Core::CoreUtils;

#endif // __ELASTOS_CORE_COREUTILS_H__