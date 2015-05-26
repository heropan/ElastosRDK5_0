## 前言
---
为了更好的开发Elastos5.0系统，使得系统结构更加的合理、简洁，我们将对现有的一些规则作出调整，并增加一些新的规则。现将这些变化说明如下。

## 使用Object为基类
---
当编写一个C++类或者CAR类时，请将Object类作为其基类。你可以直接继承Object，或者间接继承Object。

Object类实现了IObject、ISynchronize和IWeakReferenceSource接口，原则上，Object的子类需实现GetClassID方法，并可以按需重载ToString、GetHashCode和Equals方法。

为当前的类实现好IObject接口中的ToString和GetClassID方法对程序调试非常有用。一般的，ToString方法可以返回当前类的类名，而GetClassID可以返回当前类的ClassID。对于CAR类，Object基类的ToString方法可以通过反射获取具体的类名，因此，如无特定的实现，CAR类可以直接继承使用Object基类的ToString方法。对于C++类，请**一定重载**ToString方法，若无有特定的实现，须在其中返回该类的类名。另外，对于C++类，编译器并不会自动生成其ClassID，所以，请**一定使用UUID生成器**（如visual studio的guid生成器或Ubuntu下的<code>uuidgen</code> 命令）生成一个128位的UUID给该C++类，并在其重载的GetClassID方法中返回。

Object是所有C++类或者CAR类的子类，所以，所有CAR类都隐含实现了IObject、ISynchronize和IWeakReferenceSource接口。但是，在car文件中声明CAR类时，不用显式的声明实现这些接口，它们将由CAR编译器自动的加入到CAR类实现的接口中。

相应的示例可以参考Thread的实现（位于Thread.h和Thread.cpp中）。

## 请使用Object的锁功能，不要使用Mutex
---
当前Elastos类库中的Mutex是对pthread_mutex_t的包装，并且被初始化为重入检查锁。重入检查锁不是递归锁，虽然它支持同一个线程多次调用lock而不会锁死，但是不论调用几次lock，只要调用一次unlock就会解锁。所以，它并不适用于递归调用的情况。而我们不用递归锁的原因是其效率比较底下。

现在，基类Object实现了ISynchronize接口，具有锁的功能，支持递归调用，并且执行效率要比递归锁高。C++类和CAR类继承Object以后，本身都具有了锁的功能，可以像Java代码那样去使用它。

基类 Object 实现了宏 synchronized：
``` cpp
#define synchronized(obj)  for(Elastos::Core::Object::AutoLock obj##_lock(obj); obj##_lock; obj##_lock.SetUnlock())
```
通过该宏，我们可以像 Java 关键字 synchronized 那样使用。

## CAR接口支持方法重载
---
之前，CAR接口方法不支持重载，必须给同名的方法加“Ex”、“Ex2”等后缀。这种处理方式除了对编程不友好以外，还有一个比较大的弊端。在运行Java程序时，在兼容层需要根据CAR的方法转调同名的Java方法。对于加了后缀的CAR方法，我们不太方便得到相应的Java方法（人为的去掉后缀在某些情况下可能行不通）。

现在，CAR编译器已能支持方法重载，因此，我们在声明接口中的方法时不需要再添加任何后缀，完全可以与Java中的方法同名。

相应的示例可以参考ISynchronize接口的声明。

## 取消接口间的继承
---
当前，我们是按照Java中类的继承关系来定义Elastos中接口间的继承。比如，IActivity接口继承IContextThemeWrapper接口。

接口继承的好处是编程方便，通过IActivity接口指针就可以调用IContextThemeWrapper接口中的方法，类似Java代码。

但接口继承也有比较大的弊端，一个弊端是若Java代码中，一个类实现了多个接口，那么Elastos中应该继承哪个接口并没有相应的标准。

另一个比较大的弊端是会导致代码的冗余。比如，实现IActivity接口的类可以继承实现IContextThemeWrapper的类，但即使前者没有重新定义IContextThemeWrapper接口中的任何方法，仍然需要将该接口中的所有方法都包一下，转调基类的相应方法。

因此，我们打算取消接口间的继承关系。不过，这会给编程带来一些小麻烦，不能再像以前那样一个接口支持所有方法，而是要Probe出相应的接口再调用。比如，当IActivity接口不再继承IContextThemeWrapper接口后，我们就不能通过IActivity接口调用IContextThemeWrapper里的方法了，而是要从IActivity接口Probe出IContextThemeWrapper接口再调用。

由于取消了接口间的继承，因而，实现某个接口往往伴随着要实现别的接口。比如，实现IActivity的类应该也实现IContextThemeWrapper接口，而实现IContextThemeWrapper接口也应该实现IContextWrapper接口，而实现IContextWrapper接口也应该实现IContext接口。这些要同时实现的接口，在IActivity接口的声明中都无法体现出来。因此，我们需要在其声明的头部加上一定的注释，注明这些要同时实现的接口。注释的形式如下：
``` cpp
/**
  * @Involve
  * interface IContext;
  * interface IContextWrapper;
  * interface IContextThemeWrapper;
  */

interface IActivity {
   ...
}
```

## 让基类继承接口
---
当前，被继承的C++类不直接实现相应的接口，比如，Thread类不实现IThread接口。这样处理，是因为CThread之类本身会通过_CThread继承IThread接口，所以，在CThread类里必须重新定义IThread接口中的所有方法。

现在，我们修正了CAR编译器生成的镜像代码，简化了CAR类的继承结构，已经可以支持让Thread类继承IThread接口，而CThread直接继承Thread即可。这样做也可以减小代码的冗余，让类的继承结构更加的简单、合理。

相应的示例可以参考Thread和CThread的实现。

## 其它
---
### 一些宏
* 宏<code>INTERFACE_ADDREF/ARRAYOF_ADDREF</code>改名为<code>REFCOUNT_ADD</code>；宏<code>INTERFACE_RELEASE/ARRAYOF_RELEASE</code>改名为<code>REFCOUNT_RELEASE</code>，定义在elrefbase.h中

* 所有的非单例 CAR 类都需要使用 <code>CAR_OBJECT_DECL/CAR_OBJECT_IMPL</code>
* 所有的单例 CAR 类都需要使用 <code>CAR_SINGLETON_DECL/CAR_SINGLETON_IMPL</code>
* 所有在头文件中继承 CAR 接口的类（ C++ 类或 非单例 CAR 类）都需要使用 <code>CAR_INTERFACE_DECL/CAR_INTERFACE_IMPL</code>
* 请参考示例了解如何使用这些宏：<code>LibCore/inc/org/xml</code> 以及 <code>LibCore/src/org/xml</code>。

### 夹壁墙类文件名包含命名空间前缀
后台生成的夹壁墙类文件名包含命名空间前缀，如 CBoolean 类对应的夹壁墙类文件名为：<code>_Elastos_Core_CBoolean.h</code>

