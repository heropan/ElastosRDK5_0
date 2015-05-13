#ifndef __GLSURFACEVIEWMACRO_H__
#define __GLSURFACEVIEWMACRO_H__

#define IGLSURFACEVIEW_METHODS_DECL()                                             \
    CARAPI SetGLWrapper(                                                        \
        /* [in] */ IGLWrapper* glWrapper);                                      \
                                                                                \
    CARAPI SetDebugFlags(                                                       \
        /* [in] */ Int32 debugFlags);                                           \
                                                                                \
    CARAPI GetDebugFlags(                                                       \
        /* [out] */ Int32* flags);                                              \
                                                                                \
    CARAPI SetPreserveEGLContextOnPause(                                        \
        /* [in] */ Boolean preserveOnPause);                                    \
                                                                                \
    CARAPI GetPreserveEGLContextOnPause(                                        \
        /* [out] */ Boolean* preserveEGLContextOnPause);                        \
                                                                                \
    CARAPI SetRenderer(                                                         \
        /* [in] */ IRenderer* renderer);                                        \
                                                                                \
    CARAPI SetEGLContextFactory(                                                \
        /* [in] */ IEGLContextFactory* factory);                                \
                                                                                \
    CARAPI SetEGLWindowSurfaceFactory(                                          \
        /* [in] */ IEGLWindowSurfaceFactory* factory);                          \
                                                                                \
    CARAPI SetEGLConfigChooser(                                                 \
        /* [in] */ IEGLConfigChooser* configChooser);                           \
                                                                                \
    CARAPI SetEGLConfigChooserEx(                                               \
        /* [in] */ Boolean needDepth);                                          \
                                                                                \
    CARAPI SetEGLConfigChooserEx2(                                              \
        /* [in] */ Int32 redSize,                                               \
        /* [in] */ Int32 greenSize,                                             \
        /* [in] */ Int32 blueSize,                                              \
        /* [in] */ Int32 alphaSize,                                             \
        /* [in] */ Int32 depthSize,                                             \
        /* [in] */ Int32 stencilSize);                                          \
                                                                                \
    CARAPI SetEGLContextClientVersion(                                          \
        /* [in] */ Int32 v);                                                    \
                                                                                \
    CARAPI SetRenderMode(                                                       \
        /* [in] */ Int32 renderMode);                                           \
                                                                                \
    CARAPI GetRenderMode(                                                       \
        /* [out] */ Int32* renderMode);                                         \
                                                                                \
    CARAPI RequestRender();                                                     \
                                                                                \
    CARAPI OnPause();                                                           \
                                                                                \
    CARAPI OnResume();                                                          \
                                                                                \
    CARAPI QueueEvent(                                                          \
        /* [in] */ IRunnable* r);

#define IGLSURFACEVIEW_METHODS_IMPL(className, superClass)                             \
ECode className::SetGLWrapper(                                                         \
    /* [in] */ IGLWrapper* glWrapper)                                                  \
{                                                                                      \
    return superClass::SetGLWrapper(glWrapper);                                        \
}                                                                                      \
                                                                                       \
ECode className::SetDebugFlags(                                                        \
    /* [in] */ Int32 debugFlags)                                                       \
{                                                                                      \
    return superClass::SetDebugFlags(debugFlags);                                      \
}                                                                                      \
                                                                                       \
ECode className::GetDebugFlags(                                                        \
    /* [out] */ Int32* flags)                                                          \
{                                                                                      \
    return superClass::GetDebugFlags(flags);                                           \
}                                                                                      \
                                                                                       \
ECode className::SetPreserveEGLContextOnPause(                                         \
    /* [in] */ Boolean preserveOnPause)                                                \
{                                                                                      \
    return superClass::SetPreserveEGLContextOnPause(preserveOnPause);                  \
}                                                                                      \
                                                                                       \
ECode className::GetPreserveEGLContextOnPause(                                         \
    /* [out] */ Boolean* preserveEGLContextOnPause)                                    \
{                                                                                      \
    return superClass::GetPreserveEGLContextOnPause(preserveEGLContextOnPause);        \
}                                                                                      \
                                                                                       \
ECode className::SetRenderer(                                                          \
    /* [in] */ IRenderer* renderer)                                                    \
{                                                                                      \
    return superClass::SetRenderer(renderer);                                          \
}                                                                                      \
                                                                                       \
ECode className::SetEGLContextFactory(                                                 \
    /* [in] */ IEGLContextFactory* factory)                                            \
{                                                                                      \
    return superClass::SetEGLContextFactory(factory);                                  \
}                                                                                      \
                                                                                       \
ECode className::SetEGLWindowSurfaceFactory(                                           \
    /* [in] */ IEGLWindowSurfaceFactory* factory)                                      \
{                                                                                      \
    return superClass::SetEGLWindowSurfaceFactory(factory);                            \
}                                                                                      \
                                                                                       \
ECode className::SetEGLConfigChooser(                                                  \
    /* [in] */ IEGLConfigChooser* configChooser)                                       \
{                                                                                      \
    return superClass::SetEGLConfigChooser(configChooser);                             \
}                                                                                      \
                                                                                       \
ECode className::SetEGLConfigChooserEx(                                                \
    /* [in] */ Boolean needDepth)                                                      \
{                                                                                      \
    return superClass::SetEGLConfigChooserEx(needDepth);                               \
}                                                                                      \
                                                                                       \
ECode className::SetEGLConfigChooserEx2(                                               \
    /* [in] */ Int32 redSize,                                                          \
    /* [in] */ Int32 greenSize,                                                        \
    /* [in] */ Int32 blueSize,                                                         \
    /* [in] */ Int32 alphaSize,                                                        \
    /* [in] */ Int32 depthSize,                                                        \
    /* [in] */ Int32 stencilSize)                                                      \
{                                                                                      \
    return superClass::SetEGLConfigChooserEx2(                                         \
        redSize, greenSize, blueSize, alphaSize, depthSize, stencilSize);              \
}                                                                                      \
                                                                                       \
ECode className::SetEGLContextClientVersion(                                           \
    /* [in] */ Int32 v)                                                                \
{                                                                                      \
    return superClass::SetEGLContextClientVersion(v);                                  \
}                                                                                      \
                                                                                       \
ECode className::SetRenderMode(                                                        \
    /* [in] */ Int32 renderMode)                                                       \
{                                                                                      \
    return superClass::SetRenderMode(renderMode);                                      \
}                                                                                      \
                                                                                       \
ECode className::GetRenderMode(                                                        \
    /* [out] */ Int32* renderMode)                                                     \
{                                                                                      \
    return superClass::GetRenderMode(renderMode);                                      \
}                                                                                      \
                                                                                       \
ECode className::RequestRender()                                                       \
{                                                                                      \
    return superClass::RequestRender();                                                \
}                                                                                      \
                                                                                       \
ECode className::OnPause()                                                             \
{                                                                                      \
    return superClass::OnPause();                                                      \
}                                                                                      \
                                                                                       \
ECode className::OnResume()                                                            \
{                                                                                      \
    return superClass::OnResume();                                                     \
}                                                                                      \
                                                                                       \
ECode className::QueueEvent(                                                           \
    /* [in] */ IRunnable* r)                                                           \
{                                                                                      \
    return superClass::QueueEvent(r);                                                  \
}

#define ISURFACEHOLDERCALLBACK_METHODS_DECL()                                          \
    CARAPI SurfaceCreated(                                                             \
        /* [in] */ ISurfaceHolder* holder);                                            \
                                                                                       \
    CARAPI SurfaceChanged(                                                             \
        /* [in] */ ISurfaceHolder* holder,                                             \
        /* [in] */ Int32 format,                                                       \
        /* [in] */ Int32 width,                                                        \
        /* [in] */ Int32 height);                                                      \
                                                                                       \
    CARAPI SurfaceDestroyed(                                                           \
        /* [in] */ ISurfaceHolder* holder);

#define ISURFACEHOLDERCALLBACK_METHODS_IMPL(className, superClass)                     \
ECode className::SurfaceCreated(                                                       \
    /* [in] */ ISurfaceHolder* holder)                                                 \
{                                                                                      \
    return superClass::SurfaceCreated(holder);                                         \
}                                                                                      \
                                                                                       \
ECode className::SurfaceChanged(                                                       \
        /* [in] */ ISurfaceHolder* holder,                                             \
        /* [in] */ Int32 format,                                                       \
        /* [in] */ Int32 width,                                                        \
        /* [in] */ Int32 height)                                                       \
{                                                                                      \
    return superClass::SurfaceChanged(holder, format, width, height);                  \
}                                                                                      \
                                                                                       \
ECode className::SurfaceDestroyed(                                                     \
        /* [in] */ ISurfaceHolder* holder)                                             \
{                                                                                      \
    return superClass::SurfaceDestroyed(holder);                                       \
}
#endif //__GLSURFACEVIEWMACRO_H__
