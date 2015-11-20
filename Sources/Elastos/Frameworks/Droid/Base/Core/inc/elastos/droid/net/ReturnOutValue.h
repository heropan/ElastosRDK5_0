#ifndef __ELASTOS_DROID_NET_RETURNOUTVALUE_H__
#define __ELASTOS_DROID_NET_RETURNOUTVALUE_H__

#include "elastos/droid/ext/frameworkext.h"

template <typename T_OBJ, typename T_REV>
T_REV ReturnOutValue(T_OBJ* obj, ECode (T_OBJ::*func)(T_REV*))
{
    T_REV rev;
    (obj->*func)(&rev);
    return rev;
}

template <typename T_OBJ, typename T_REV>
AutoPtr<T_REV> ReturnOutValue(T_OBJ* obj, ECode (T_OBJ::*func)(T_REV**))
{
    AutoPtr<T_REV> rev;
    (obj->*func)((T_REV**)&rev);
    return rev;
}

template <typename T_OBJ, typename T_REV, typename T_PARA1>
AutoPtr<T_REV> ReturnOutValue(T_OBJ* obj, ECode (T_OBJ::*func)(T_PARA1 para1, T_REV**), T_PARA1 para1)
{
    AutoPtr<T_REV> rev;
    (obj->*func)(para1, (T_REV**)&rev);
    return rev;
}

template <typename T_OBJ, typename T_REV, typename T_PARA1>
T_REV ReturnOutValue(T_OBJ* obj, ECode (T_OBJ::*func)(T_PARA1 para1, T_REV*), T_PARA1 para1)
{
    T_REV rev;
    (obj->*func)(para1, &rev);
    return rev;
}

template <typename T_OBJ, typename T_REV, typename T_PARA1>
T_REV ReturnOutValue(T_OBJ* obj, ECode (T_OBJ::*func)(T_PARA1* para1, T_REV*), T_PARA1* para1)
{
    T_REV rev;
    (obj->*func)(para1, &rev);
    return rev;
}

/**
 * no object, such as global function, static function
 * involve this function with type parameter list, for it can't identify the T_REV type
 */

template <typename T_REV, typename T_PARA1>
AutoPtr<T_REV> ReturnOutValue(ECode (*func)(T_PARA1*, T_REV**), T_PARA1* para1)
{
    AutoPtr<T_REV> rev;
    (*func)(para1, (T_REV**)&rev);
    return rev;
}

/**
 * Extended
 */
template <typename T_OBJ, typename T_REV>
T_REV ReturnOutValue(AutoPtr<T_OBJ> obj, ECode (T_OBJ::*func)(T_REV*))
{
    return ReturnOutValue(obj.Get(), func);
}

template <typename T_OBJ, typename T_REV, typename T_PARA1>
T_REV ReturnOutValue(AutoPtr<T_OBJ> obj, ECode (T_OBJ::*func)(T_PARA1* para1, T_REV*), T_PARA1* para1)
{
    return ReturnOutValue(obj.Get(), func, para1);
}

template <typename T_OBJ, typename T_REV, typename T_PARA1>
AutoPtr<T_REV> ReturnOutValue(AutoPtr<T_OBJ> obj, ECode (T_OBJ::*func)(T_PARA1 para1, T_REV**), T_PARA1 para1)
{
    return ReturnOutValue(obj.Get(), func, para1);
}

template <typename T_OBJ, typename T_REV, typename T_PARA1>
T_REV ReturnOutValue(T_OBJ* obj, ECode (T_OBJ::*func)(T_PARA1* para1, T_REV*), AutoPtr<T_PARA1> para1)
{
    return ReturnOutValue(obj, func, para1.Get());
}

template <typename T_REV, typename T_PARA1>
AutoPtr<T_REV> ReturnOutValue(ECode (*func)(T_PARA1*, T_REV**), AutoPtr<T_PARA1> para1)
{
    return ReturnOutValue(func, para1.Get());
}

#ifndef FOR_EACH
#define FOR_EACH(iterator, container) AutoPtr<IIterator> iterator;      \
    container->GetIterator((IIterator**)&iterator);                     \
    while (ReturnOutValue(iterator, iterator->HasNext))
#endif // FOR_EACH

#ifndef RETURN_OUT_VALUE
#define RETN_OUT_VAL(obj, func, arg...) ReturnOutValue(obj, (obj)->func, ##arg)
#endif // RETURN_OUT_VALUE
#endif // __ELASTOS_DROID_NET_RETURNOUTVALUE_H__