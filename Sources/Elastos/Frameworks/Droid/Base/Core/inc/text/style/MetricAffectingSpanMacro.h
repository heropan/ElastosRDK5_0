#ifndef __METRICAFFECTINGSPANMACRO_H__
#define __METRICAFFECTINGSPANMACRO_H__

#define IMETRICAFFECTINGSPAN_METHODS_DECL()                                      \
    CARAPI UpdateMeasureState(                                                   \
        /* [in] */ ITextPaint* p);                                               \


#define IMETRICAFFECTINGSPAN_METHODS_IMPL(className, superClass, overRideClass)  \
ECode className::UpdateMeasureState(                                             \
    /* [in] */ ITextPaint* p)                                                    \
{                                                                                \
    return superClass::UpdateMeasureState(p);                                    \
}                                                                                \


#endif  //__METRICAFFECTINGSPANMACRO_H__