#ifndef __NUMBER_H__
#define __NUMBER_H__

#include <elastos.h>

namespace Elastos {
namespace Core {

/**
 * The abstract superclass of the classes which represent numeric base types
 * (that is {@link Byte}, {@link Short}, {@link Integer}, {@link Long},
 * {@link Float}, and {@link Double}.
 */
class Number
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    /**
     * Empty default constructor.
     */
    Number() {}

    /**
     * Returns this object's value as a byte. Might involve rounding and/or
     * truncating the value, so it fits into a byte.
     *
     * @return the primitive byte value of this object.
     */
    virtual CARAPI_(Byte) ByteValue()
    { return (Byte)Int32Value(); }

    /**
     * Returns this object's value as a double. Might involve rounding.
     *
     * @return the primitive double value of this object.
     */
    virtual CARAPI_(Double) DoubleValue() = 0;

    /**
     * Returns this object's value as a float. Might involve rounding.
     *
     * @return the primitive float value of this object.
     */
    virtual CARAPI_(Float) FloatValue() = 0;

    /**
     * Returns this object's value as an int. Might involve rounding and/or
     * truncating the value, so it fits into an int.
     *
     * @return the primitive int value of this object.
     */
    virtual CARAPI_(Int32) Int32Value() = 0;

    /**
     * Returns this object's value as a long. Might involve rounding and/or
     * truncating the value, so it fits into a long.
     *
     * @return the primitive long value of this object.
     */
    virtual CARAPI_(Int64) Int64Value() = 0;

    /**
     * Returns this object's value as a short. Might involve rounding and/or
     * truncating the value, so it fits into a short.
     *
     * @return the primitive short value of this object.
     */
    virtual CARAPI_(Int16) Int16Value()
    { return (Int16)Int32Value(); }
};

} // namespace Core
} // namespace Elastos

#endif //__NUMBER_H__
