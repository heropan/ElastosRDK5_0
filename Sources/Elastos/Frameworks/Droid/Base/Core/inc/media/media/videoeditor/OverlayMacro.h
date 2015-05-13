
#ifndef __OVERLAYMACRO_H__
#define __OVERLAYMACRO_H__

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

#define IOVERLAY_METHODS_DECL()                                      \
    CARAPI GetId(                                                    \
        /* [out] */ String* result);                                 \
                                                                     \
    CARAPI GetDuration(                                              \
        /* [out] */ Int64* result);                                  \
                                                                     \
    CARAPI SetDuration(                                              \
        /* [in] */ Int64 durationMs);                                \
                                                                     \
    CARAPI GetStartTime(                                             \
        /* [out] */ Int64* result);                                  \
                                                                     \
    CARAPI SetStartTime(                                             \
        /* [in] */ Int64 startTimeMs);                               \
                                                                     \
    CARAPI SetStartTimeAndDuration(                                  \
        /* [in] */ Int64 startTimeMs,                                \
        /* [in] */ Int64 durationMs);                                \
                                                                     \
    CARAPI GetMediaItem(                                             \
        /* [out] */ IMediaItem** result);                            \
                                                                     \
    CARAPI SetUserAttribute(                                         \
        /* [in] */ const String& name,                               \
        /* [in] */ const String& value);                             \
                                                                     \
    CARAPI GetUserAttributes(                                        \
        /* [out] */ IObjectStringMap** result);                      \
                                                                     \
    CARAPI Equals(                                                   \
        /* [in] */ IInterface* object,                               \
        /* [out] */ Boolean* result);                                \
                                                                     \
    CARAPI GetHashCode(                                              \
        /* [out] */ Int32* result);

#define IOVERLAY_METHODS_IMPL(className, superClass)                                    \
    ECode className::GetId(                                                             \
    /* [out] */ String* result)                                                         \
    {                                                                                   \
        return superClass::GetId(result);                                               \
    }                                                                                   \
                                                                                        \
    ECode className::GetDuration(                                                       \
        /* [out] */ Int64* result)                                                      \
    {                                                                                   \
        return superClass::GetDuration(result);                                         \
    }                                                                                   \
                                                                                        \
    ECode className::SetDuration(                                                       \
        /* [in] */ Int64 durationMs)                                                    \
    {                                                                                   \
        return superClass::SetDuration(durationMs);                                     \
    }                                                                                   \
                                                                                        \
    ECode className::GetStartTime(                                                      \
        /* [out] */ Int64* result)                                                      \
    {                                                                                   \
        return superClass::GetStartTime(result);                                        \
    }                                                                                   \
                                                                                        \
    ECode className::SetStartTime(                                                      \
        /* [in] */ Int64 startTimeMs)                                                   \
    {                                                                                   \
        return superClass::SetStartTime(startTimeMs);                                   \
    }                                                                                   \
                                                                                        \
    ECode className::SetStartTimeAndDuration(                                           \
        /* [in] */ Int64 startTimeMs,                                                   \
        /* [in] */ Int64 durationMs)                                                    \
    {                                                                                   \
        return superClass::SetStartTimeAndDuration(startTimeMs, durationMs);            \
    }                                                                                   \
                                                                                        \
    ECode className::GetMediaItem(                                                      \
        /* [out] */ IMediaItem** result)                                                \
    {                                                                                   \
        return superClass::GetMediaItem(result);                                        \
    }                                                                                   \
                                                                                        \
    ECode className::SetUserAttribute(                                                  \
        /* [in] */ const String& name,                                                  \
        /* [in] */ const String& value)                                                 \
    {                                                                                   \
        return superClass::SetUserAttribute(name, value);                               \
    }                                                                                   \
                                                                                        \
    ECode className::GetUserAttributes(                                                 \
        /* [out] */ IObjectStringMap** result)                                          \
    {                                                                                   \
        return superClass::GetUserAttributes(result);                                   \
    }                                                                                   \
                                                                                        \
    ECode className::Equals(                                                            \
        /* [in] */ IInterface* object,                                                  \
        /* [out] */ Boolean* result)                                                    \
    {                                                                                   \
        return superClass::Equals(object, result);                                      \
    }                                                                                   \
                                                                                        \
    ECode className::GetHashCode(                                                       \
        /* [out] */ Int32* result)                                                      \
    {                                                                                   \
        return superClass::GetHashCode(result);                                         \
    }

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __OVERLAYMACRO_H__