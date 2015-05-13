
#ifndef __TRANSITIONMACRO_H__
#define __TRANSITIONMACRO_H__

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

#define ITRANSITION_METHODS_DECL()                                   \
    CARAPI GetId(                                                    \
        /* [out] */ String* result);                                 \
                                                                     \
    CARAPI GetAfterMediaItem(                                        \
        /* [out] */ IMediaItem** result);                            \
                                                                     \
    CARAPI GetBeforeMediaItem(                                       \
        /* [out] */ IMediaItem** result);                            \
                                                                     \
    CARAPI SetDuration(                                              \
        /* [in] */ Int64 durationMs);                                \
                                                                     \
    CARAPI GetDuration(                                              \
        /* [out] */ Int64* result);                                  \
                                                                     \
    CARAPI GetMaximumDuration(                                       \
        /* [out] */ Int64* result);                                  \
                                                                     \
    CARAPI GetBehavior(                                              \
        /* [out] */ Int32* result);                                  \
                                                                     \
    CARAPI Equals(                                                   \
        /* [in] */ IInterface* object,                               \
        /* [out] */ Boolean* result);                                \
                                                                     \
    CARAPI GetHashCode(                                              \
        /* [out] */ Int32* result);

#define ITRANSITION_METHODS_IMPL(className, superClass)                                 \
    ECode className::GetId(                                                             \
    /* [out] */ String* result)                                                         \
    {                                                                                   \
        return superClass::GetId(result);                                               \
    }                                                                                   \
                                                                                        \
    ECode className::GetAfterMediaItem(                                                 \
        /* [out] */ IMediaItem** result)                                                \
    {                                                                                   \
        return superClass::GetAfterMediaItem(result);                                   \
    }                                                                                   \
                                                                                        \
    ECode className::GetBeforeMediaItem(                                                \
        /* [out] */ IMediaItem** result)                                                \
    {                                                                                   \
        return superClass::GetBeforeMediaItem(result);                                  \
    }                                                                                   \
                                                                                        \
    ECode className::SetDuration(                                                       \
        /* [in] */ Int64 durationMs)                                                    \
    {                                                                                   \
        return superClass::SetDuration(durationMs);                                     \
    }                                                                                   \
                                                                                        \
    ECode className::GetDuration(                                                       \
        /* [out] */ Int64* result)                                                      \
    {                                                                                   \
        return superClass::GetDuration(result);                                         \
    }                                                                                   \
                                                                                        \
    ECode className::GetMaximumDuration(                                                \
        /* [out] */ Int64* result)                                                      \
    {                                                                                   \
        return superClass::GetMaximumDuration(result);                                  \
    }                                                                                   \
                                                                                        \
    ECode className::GetBehavior(                                                       \
        /* [out] */ Int32* result)                                                      \
    {                                                                                   \
        return superClass::GetBehavior(result);                                         \
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

#endif // __TRANSITIONMACRO_H__