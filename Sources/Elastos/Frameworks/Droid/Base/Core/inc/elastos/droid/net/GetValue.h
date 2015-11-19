#include "elastos/droid/ext/frameworkext.h"

template <typename T_OBJ, typename T_REV>
T_REV GetValue(T_OBJ* obj, ECode (T_OBJ::*func)(T_REV*))
{
    T_REV rev;
    (obj->*func)(&rev);
    return rev;
}

template <typename T_OBJ, typename T_REV, typename T_PARA1>
T_REV GetValue(T_OBJ* obj, ECode (T_OBJ::*func)(T_PARA1 para1, T_REV*), T_PARA1 para1)
{
    T_REV rev;
    (obj->*func)(para1, &rev);
    return rev;
}

template <typename T_OBJ, typename T_REV>
T_REV GetValue(AutoPtr<T_OBJ> obj, ECode (T_OBJ::*func)(T_REV*))
{
    T_REV rev;
    (obj->*func)(&rev);
    return rev;
}

template <typename TYPE, typename T_REV, typename T_PARA1)
AutoPtr<T_REV> GetValue(ECode (TYPE::*func)(T_PARA1 para1, T_REV**), T_PARA1 para1)
{
    AutoPtr<T_REV> rev;
    (TYPE::*func)(para1, (T_REV**)&rev);
    return  rev;
}

template <typename T_OBJ, typename T_REV>
AutoPtr<T_REV> GetValue(AutoPtr<T_OBJ> obj, ECode (T_OBJ::*func)(T_REV**))
{
    AutoPtr<T_REV> rev;
    (obj->*func)((T_REV**)&rev);
    return rev;
}
