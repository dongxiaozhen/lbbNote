```go
// 通用类型定义
 type _type struct {
   size       uintptr
   ptrdata    uintptr // size of memory prefix holding all pointers
   hash       uint32
   tflag      tflag
   align      uint8
   fieldalign uint8
   kind       uint8
   alg        *typeAlg
   gcdata    *byte
   str       nameOff
   ptrToThis typeOff
 } 

//-------------------------------------
// 特定类型的描述，第一个字段都是_type，通过_type的kind字段，读取其他字段来确定一个特定类型描述
type maptype struct {
    typ _type
    key *_type
    elem *_type
    ....
 }

 type interfacetype struct {
    typ _type
    pkgpath name
    mhdr []imethod
 }

 type arraytype struct {
     typ _type
     elem *_type
     slice *_type
     len uintptr
 }
 type chantype struct {
     typ _type
     elem *_type
     dir uintptr
}
 //---------------------------------------

 // 接口
  type iface struct {                                                                                                                                                                                                              
      tab  *itab                                                                                                                                                                                                                   
      data unsafe.Pointer                                                                                                                                                                                                          
  }                                                                                                                                                                                                                                
                                                                                                                                                                                                                                   
//空接口
  type eface struct {                                                                                                                                                                                                              
      _type *_type                                                                                                                                                                                                                 
      data  unsafe.Pointer                                                                                                                                                                                                         
  }

 type itab struct {
   	inter  *interfacetype  // 接口信息,接口的方法记录
   	 _type  *_type          //变量的类型信息
   	 link   *itab           //itab链，全局有itab hash,用interfacetype.typ.hash 和_type 的hash 确认slot
     hash   uint32 // copy of _type.hash. Used for type switches.
     bad    bool   // type does not implement interface
     inhash bool   // has this itab been added to hash?
     unused [2]byte
     fun    [1]uintptr // variable sized
 } 
```
interface转的时候会复制原值。convT2E(转化为interface{}),convT2I(转化为自己定义的结构体)
interface 可以比较，只有type,value都相同时才相等 
	src/runtime/iface.go 带方法的借口 
	reflect.Type == runtime._type

var1,ok:=i1.(type) 接口i1的type是不是type1, 返回的是类型type1的变量
i3:=i1.(i2)   接口i1的value有没有实现i2接口,返回的结果是i2类型的接口

reflect
	逆操作：reflect.ValueOf(a).Interface()
	reflect.Type(a).Kind() 返回structkind
	reflect.ValueOf(a).String() 返回 struct名字

switch rr:=a.(type)   (type)只能在switch中出现，rr是变量，可以操作,相当于遍历a.(type)
