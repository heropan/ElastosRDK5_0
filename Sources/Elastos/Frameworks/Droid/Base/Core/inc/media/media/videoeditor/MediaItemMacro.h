
#ifndef __MEDIAITEMMACRO_H__
#define __MEDIAITEMMACRO_H__

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

#define IMEDIAITEM_METHODS_DECL()                                               \
    CARAPI GetId(                                                               \
        /* [out] */ String* result);                                            \
                                                                                \
    CARAPI GetFilename(                                                         \
        /* [out] */ String* result);                                            \
                                                                                \
    CARAPI SetRenderingMode(                                                    \
        /* [in] */ Int32 renderingMode);                                        \
                                                                                \
    CARAPI GetRenderingMode(                                                    \
        /* [out] */ Int32* result);                                             \
                                                                                \
    CARAPI GetBeginTransition(                                                  \
        /* [out] */ ITransition** result);                                      \
                                                                                \
    CARAPI GetEndTransition(                                                    \
        /* [out] */ ITransition** result);                                      \
                                                                                \
    CARAPI RemoveEffect(                                                        \
        /* [in] */ const String& effectId,                                      \
        /* [out] */ IEffect** result);                                          \
                                                                                \
    CARAPI GetEffect(                                                           \
        /* [in] */ const String& effectId,                                      \
        /* [out] */ IEffect** result);                                          \
                                                                                \
    CARAPI GetAllEffects(                                                       \
        /* [out] */ ArrayOf<IEffect*>** result);                                \
                                                                                \
    CARAPI AddOverlay(                                                          \
        /* [in] */ IOverlay* overlay);                                          \
                                                                                \
    CARAPI RemoveOverlay(                                                       \
        /* [in] */ const String& overlayId,                                     \
        /* [out] */ IOverlay** result);                                         \
                                                                                \
    CARAPI GetOverlay(                                                          \
        /* [in] */ const String& overlayId,                                     \
        /* [out] */ IOverlay** result);                                         \
                                                                                \
    CARAPI GetAllOverlays(                                                      \
        /* [out] */ ArrayOf<IOverlay*>** result);                               \
                                                                                \
    CARAPI GetThumbnailListEx(                                                  \
        /* [in] */ Int32 width,                                                 \
        /* [in] */ Int32 height,                                                \
        /* [in] */ Int64 startMs,                                               \
        /* [in] */ Int64 endMs,                                                 \
        /* [in] */ Int32 thumbnailCount,                                        \
        /* [out,callee] */ ArrayOf<IBitmap*>** result);                         \
                                                                                \
    CARAPI Equals(                                                              \
        /* [in] */ IInterface* object,                                          \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI GetHashCode(                                                         \
        /* [out] */ Int32* result);                                             \
                                                                                \
    CARAPI SetBeginTransition(                                                  \
        /* [in] */ ITransition* transition);                                    \
                                                                                \
    CARAPI SetEndTransition(                                                    \
        /* [in] */ ITransition* transition);                                    \
                                                                                \
    CARAPI SetRegenerateClip(                                                   \
        /* [in] */ Boolean flag);                                               \
                                                                                \
    CARAPI GetRegenerateClip(                                                   \
        /* [out] */ Boolean* result);                                           \
                                                                                \
    CARAPI GetNativeContext(                                                    \
        /* [out] */ MediaArtistNativeHelper** helper);                          \
                                                                                \
    CARAPI InitClipSettings(                                                    \
        /* [in] */ MediaArtistNativeHelper::ClipSettings* clipSettings);        \
                                                                                \
    CARAPI GetClipSettings(                                                     \
        /* [out] */ MediaArtistNativeHelper::ClipSettings** clipSettings);      \
                                                                                \
    CARAPI GenerateBlankFrame(                                                  \
        /* [in] */ MediaArtistNativeHelper::ClipSettings* clipSettings);        \
                                                                                \
    CARAPI InvalidateBlankFrame();


#define IMEDIAITEM_METHODS_IMPL(className, superClass)                                               \
    ECode className::GetId(                                                                          \
        /* [out] */ String* result)                                                                  \
    {                                                                                                \
        return superClass::GetId(result);                                                            \
    }                                                                                                \
                                                                                                     \
    ECode className::GetFilename(                                                                    \
        /* [out] */ String* result)                                                                  \
    {                                                                                                \
        return superClass::GetFilename(result);                                                      \
    }                                                                                                \
                                                                                                     \
    ECode className::SetRenderingMode(                                                               \
        /* [in] */ Int32 renderingMode)                                                              \
    {                                                                                                \
        return superClass::SetRenderingMode(renderingMode);                                          \
    }                                                                                                \
                                                                                                     \
    ECode className::GetRenderingMode(                                                               \
        /* [out] */ Int32* result)                                                                   \
    {                                                                                                \
        return superClass::GetRenderingMode(result);                                                 \
    }                                                                                                \
                                                                                                     \
    ECode className::GetBeginTransition(                                                             \
        /* [out] */ ITransition** result)                                                            \
    {                                                                                                \
        return superClass::GetBeginTransition(result);                                               \
    }                                                                                                \
                                                                                                     \
    ECode className::GetEndTransition(                                                               \
        /* [out] */ ITransition** result)                                                            \
    {                                                                                                \
        return superClass::GetEndTransition(result);                                                 \
    }                                                                                                \
                                                                                                     \
    ECode className::RemoveEffect(                                                                   \
        /* [in] */ const String& effectId,                                                           \
        /* [out] */ IEffect** result)                                                                \
    {                                                                                                \
        return superClass::RemoveEffect(effectId, result);                                           \
    }                                                                                                \
                                                                                                     \
    ECode className::GetEffect(                                                                      \
        /* [in] */ const String& effectId,                                                           \
        /* [out] */ IEffect** result)                                                                \
    {                                                                                                \
        return superClass::GetEffect(effectId, result);                                              \
    }                                                                                                \
                                                                                                     \
    ECode className::GetAllEffects(                                                                  \
        /* [out] */ ArrayOf<IEffect*>** result)                                                      \
    {                                                                                                \
        return superClass::GetAllEffects(result);                                                    \
    }                                                                                                \
                                                                                                     \
    ECode className::AddOverlay(                                                                     \
        /* [in] */ IOverlay* overlay)                                                                \
    {                                                                                                \
        return superClass::AddOverlay(overlay);                                                      \
    }                                                                                                \
                                                                                                     \
    ECode className::RemoveOverlay(                                                                  \
        /* [in] */ const String& overlayId,                                                          \
        /* [out] */ IOverlay** result)                                                               \
    {                                                                                                \
        return superClass::RemoveOverlay(overlayId, result);                                         \
    }                                                                                                \
                                                                                                     \
    ECode className::GetOverlay(                                                                     \
        /* [in] */ const String& overlayId,                                                          \
        /* [out] */ IOverlay** result)                                                               \
    {                                                                                                \
        return superClass::GetOverlay(overlayId, result);                                            \
    }                                                                                                \
                                                                                                     \
    ECode className::GetAllOverlays(                                                                 \
        /* [out] */ ArrayOf<IOverlay*>** result)                                                     \
    {                                                                                                \
        return superClass::GetAllOverlays(result);                                                   \
    }                                                                                                \
                                                                                                     \
    ECode className::GetThumbnailListEx(                                                             \
        /* [in] */ Int32 width,                                                                      \
        /* [in] */ Int32 height,                                                                     \
        /* [in] */ Int64 startMs,                                                                    \
        /* [in] */ Int64 endMs,                                                                      \
        /* [in] */ Int32 thumbnailCount,                                                             \
        /* [out,callee] */ ArrayOf<IBitmap*>** result)                                               \
    {                                                                                                \
        return superClass::GetThumbnailListEx(width, height, startMs, endMs, thumbnailCount, result);\
    }                                                                                                \
                                                                                                     \
    ECode className::Equals(                                                                         \
        /* [in] */ IInterface* object,                                                               \
        /* [out] */ Boolean* result)                                                                 \
    {                                                                                                \
        return superClass::Equals(object, result);                                                   \
    }                                                                                                \
                                                                                                     \
    ECode className::GetHashCode(                                                                    \
        /* [out] */ Int32* result)                                                                   \
    {                                                                                                \
        return superClass::GetHashCode(result);                                                      \
    }                                                                                                \
                                                                                                     \
    ECode className::SetBeginTransition(                                                             \
        /* [in] */ ITransition* transition)                                                          \
    {                                                                                                \
        return superClass::SetBeginTransition(transition);                                           \
    }                                                                                                \
                                                                                                     \
    ECode className::SetEndTransition(                                                               \
        /* [in] */ ITransition* transition)                                                          \
    {                                                                                                \
        return superClass::SetEndTransition(transition);                                             \
    }                                                                                                \
                                                                                                     \
    ECode className::SetRegenerateClip(                                                              \
        /* [in] */ Boolean flag)                                                                     \
    {                                                                                                \
        return superClass::SetRegenerateClip(flag);                                                  \
    }                                                                                                \
                                                                                                     \
    ECode className::GetRegenerateClip(                                                              \
        /* [out] */ Boolean* result)                                                                 \
    {                                                                                                \
        return superClass::GetRegenerateClip(result);                                                \
    }                                                                                                \
                                                                                                     \
    ECode className::GetNativeContext(                                                               \
        /* [out] */ MediaArtistNativeHelper** helper)                                                \
    {                                                                                                \
        return superClass::GetNativeContext(helper);                                                 \
    }                                                                                                \
                                                                                                     \
    ECode className::InitClipSettings(                                                               \
        /* [in] */ MediaArtistNativeHelper::ClipSettings* clipSettings)                              \
    {                                                                                                \
        return superClass::InitClipSettings(clipSettings);                                           \
    }                                                                                                \
                                                                                                     \
    ECode className::GetClipSettings(                                                                \
        /* [out] */ MediaArtistNativeHelper::ClipSettings** clipSettings)                            \
    {                                                                                                \
        return superClass::GetClipSettings(clipSettings);                                            \
    }                                                                                                \
                                                                                                     \
    ECode className::GenerateBlankFrame(                                                             \
        /* [in] */ MediaArtistNativeHelper::ClipSettings* clipSettings)                              \
    {                                                                                                \
        return superClass::GenerateBlankFrame(clipSettings);                                         \
    }                                                                                                \
                                                                                                     \
    ECode className::InvalidateBlankFrame()                                                          \
    {                                                                                                \
        return superClass::InvalidateBlankFrame();                                                   \
    }

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __MEDIAITEMMACRO_H__