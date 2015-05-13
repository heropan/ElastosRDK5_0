
#ifndef __UTILITY_SLOGGER_H__
#define __UTILITY_SLOGGER_H__

#include <elastos.h>

#ifndef __SLOGGER_MACRO__
#define __SLOGGER_MACRO__
#define SLOGGERD(tag, ...)  Elastos::Utility::Logging::Slogger::D(tag, __VA_ARGS__);
#define SLOGGERE(tag, ...)  Elastos::Utility::Logging::Slogger::E(tag, __VA_ARGS__);
#define SLOGGERI(tag, ...)  Elastos::Utility::Logging::Slogger::I(tag, __VA_ARGS__);
#define SLOGGERV(tag, ...)  Elastos::Utility::Logging::Slogger::V(tag, __VA_ARGS__);
#define SLOGGERW(tag, ...)  Elastos::Utility::Logging::Slogger::W(tag, __VA_ARGS__);
#endif //__SLOGGER_MACRO__

namespace Elastos {
namespace Utility {
namespace Logging {

class Slogger
{
public:
    /*!
     * @brief Send a DEBUG log message.
     * @param tag
     * @param msg
     */
    static CARAPI D(CString tag, const char *fmt, ...);

    /*!
     * @brief Send an ERROR log message.
     * @param tag
     * @param msg
     */
    static CARAPI E(CString tag, const char *fmt, ...);

    /*!
     * @brief Send an INFO log message.
     * @param tag
     * @param msg
     */
    static CARAPI I(CString tag, const char *fmt, ...);

    /*!
     * @brief Send an VERBOSE log message.
     * @param tag
     * @param msg
     */
    static CARAPI V(CString tag, const char *fmt, ...);

    /*!
     * @brief Send a WARN log message.
     * @param tag
     * @param msg
     */
    static CARAPI W(CString tag, const char *fmt, ...);

    /*!
     * @brief Low-level logging call.
     * @param priority
     * @param tag
     * @param msg
     */
    static CARAPI Println(Int32 priority, CString tag, const char *fmt, ...);
};

} // Logging
} // namespace Utility
} // namespace Elastos

#endif //__UTILITY_SLOGGER_H__
