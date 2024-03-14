以下划线开头的标识符是有特殊意义的。以单下划线开头 _foo 的代表不能直接访问的类属性，需通过类提供的接口进行访问，不能用 from xxx import * 而导入；  
以双下划线开头的 __foo 代表类的私有成员；以双下划线开头和结尾的 **foo** 代表 Python 里特殊方法专用的标识，如 **init**() 代表类的构造函数。

Python 可以使用引号( ' )、双引号( " )、三引号( ''' 或 """ ) 来表示字符串，引号的开始与结束必须的相同类型的。  
其中三引号可以由多行组成，编写多行文本的快捷语法，常用于文档字符串，在文件的特定地点，被当做注释。  
python 中多行注释使用三个单引号(''')或三个双引号(""")。

像if、while、def和class这样的复合语句，首行以关键字开始，以冒号( : )结束，该行之后的一行或多行代码构成代码组。  
print 默认输出是换行的，如果要实现不换行需要在变量末尾加上逗号：

每个变量在使用前都必须赋值，变量赋值以后该变量才会被创建。

Numbers（数字）  
int（有符号整型）  
long（长整型[也可以代表八进制和十六进制]） 长整型也可以使用小写 l，但是还是建议您使用大写 L，避免与数字 1 混淆。Python使用 L 来显示长整型。  
float（浮点型）  
complex（复数）  
String（字符串）  
Unicode 字符串 引号前小写的"u"表示这里创建的是一个 Unicode 字符串  
List（列表）  
Tuple（元组） 元组用"()"标识,只读列表  
Dictionary（字典）字典用"{ }"标识  
键必须不可变，所以可以用数字，字符串或元组充当，所以用列表就不行，如下实例：

转换函数  
int(x [,base])将x转换为一个整数  
long(x [,base] )将x转换为一个长整数  
float(x)将x转换到一个浮点数  
complex(real [,imag])创建一个复数  
str(x)将对象 x 转换为字符串  
repr(x)将对象 x 转换为表达式字符串  
eval(str)用来计算在字符串中的有效Python表达式,并返回一个对象  
tuple(s)将序列 s 转换为一个元组  
list(s)将序列 s 转换为一个列表  
set(s)转换为可变集合  
dict(d)创建一个字典。d 必须是一个序列 (key,value)元组。  
frozenset(s)转换为不可变集合  
chr(x)将一个整数转换为一个字符  
unichr(x)将一个整数转换为Unicode字符  
ord(x)将一个字符转换为它的整数值  
hex(x)将一个整数转换为一个十六进制字符串  
oct(x)将一个整数转换为一个八进制字符串

运算  
//	取整除 - 返回商的整数部分

````
比较运算 != 不等于
         <> 不等于

辑运算符 and or not
身份运算符 is    两个标识符是不是引用自一个对象
           is not
           
if elif else
while … else
for … else 
try....except...else
try-finally 
raise语句自己触发异常
````

函数 len() 返回列表的长度，即元素的个数。 range返回一个序列的数。

python 使用 lambda 来创建匿名函数。  
lambda只是一个表达式，函数体比def简单很多。  
lambda的主体是一个表达式，而不是一个代码块。仅仅能在lambda表达式中封装有限的逻辑进去。  
lambda函数拥有自己的命名空间，且不能访问自有参数列表之外或全局命名空间里的参数。  
虽然lambda函数看起来只能写一行，却不等同于C或C++的内联函数，后者的目的是调用小函数时不占用栈内存从而增加运行效率。

包就是文件夹，但该文件夹下必须存在 **init**.py 文件, 该文件的内容可以为空。**int**.py用于标识当前文件夹是一个包。

类  
第一种方法__init__()方法是一种特殊的方法，被称为类的构造函数或初始化方法，当创建了这个类的实例时就会调用该方法  
析构函数 **del** ，__del__在对象销毁的时候被调用，当对象不再被使用时，_*del__方法运行：  
self 代表类的实例，self 在定义类的方法时是必须有的，虽然在调用时不必传入相应的参数。*  
***str*** *( self ) 用于将值转化为适于人阅读的形式， 简单的调用方法 : str(obj)*  
*cmp* ( self, x )对象比较，简单的调用方法 : cmp(obj, x)  
**add**(self,other):运算符重载

**dict** : 类的属性（包含一个字典，由类的数据属性组成）  
**doc** :类的文档字符串  
**name**: 类名  
**module**: 类定义所在的模块（类的全名是'**main**.className'，如果类位于一个导入模块mymod中，那么className.**module** 等于 mymod）  
**bases** : 类的所有父类构成元素（包含了一个由所有父类组成的元组）

### 成员访问

1. @property包装器来包装getter和setter方法，使得对属性的访问既安全又方便

   ```python
   class Person(object):
   
       def __init__(self, name, age):
           self._name = name
           self._age = age
   
       # 访问器 - getter方法
       @property
       def name(self):
           return self._name
   
       # 访问器 - getter方法
       @property
       def age(self):
           return self._age
   
       # 修改器 - setter方法
       @age.setter
       def age(self, age):
           self._age = age
   
       def play(self):
           if self._age <= 16:
               print('%s正在玩飞行棋.' % self._name)
           else:
               print('%s正在玩斗地主.' % self._name)
   
   
   def main():
       person = Person('王大锤', 12)
       person.play()
       person.age = 22
       person.play()
       # person.name = '白元芳'  # AttributeError: can't set attribute
   
   
   if __name__ == '__main__':
       main()
   ```

   2. ### __slots__魔法,限定自定义类型的对象只能绑定某些属性

      ```python
      class Person(object):
      
          # 限定Person对象只能绑定_name, _age和_gender属性
          __slots__ = ('_name', '_age', '_gender')
      
          def __init__(self, name, age):
              self._name = name
              self._age = age
      
          @property
          def name(self):
              return self._name
      
          @property
          def age(self):
              return self._age
      
          @age.setter
          def age(self, age):
              self._age = age
      
          def play(self):
              if self._age <= 16:
                  print('%s正在玩飞行棋.' % self._name)
              else:
                  print('%s正在玩斗地主.' % self._name)
      
      
      def main():
          person = Person('王大锤', 22)
          person.play()
          person._gender = '男'
          # AttributeError: 'Person' object has no attribute '_is_gay'
          # person._is_gay = True
      ```

      3. 静态方法

         ```python
         from math import sqrt
         
         
         class Triangle(object):
         
             def __init__(self, a, b, c):
                 self._a = a
                 self._b = b
                 self._c = c
         
             @staticmethod
             def is_valid(a, b, c):
                 return a + b > c and b + c > a and a + c > b
         
             def perimeter(self):
                 return self._a + self._b + self._c
         
             def area(self):
                 half = self.perimeter() / 2
                 return sqrt(half * (half - self._a) *
                             (half - self._b) * (half - self._c))
         
         
         def main():
             a, b, c = 3, 4, 5
             # 静态方法和类方法都是通过给类发消息来调用的
             if Triangle.is_valid(a, b, c):
                 t = Triangle(a, b, c)
                 print(t.perimeter())
                 # 也可以通过给类发消息来调用对象方法但是要传入接收消息的对象作为参数
                 # print(Triangle.perimeter(t))
                 print(t.area())
                 # print(Triangle.area(t))
             else:
                 print('无法构成三角形.')
         
         
         if __name__ == '__main__':
             main()
         ```

         4. 类方法，第一个参数约定名为cls，它代表的是当前类相关的信息的对象

            ```python
            from time import time, localtime, sleep
            
            
            class Clock(object):
                """数字时钟"""
            
                def __init__(self, hour=0, minute=0, second=0):
                    self._hour = hour
                    self._minute = minute
                    self._second = second
            
                @classmethod
                def now(cls):
                    ctime = localtime(time())
                    return cls(ctime.tm_hour, ctime.tm_min, ctime.tm_sec)
            
                def run(self):
                    """走字"""
                    self._second += 1
                    if self._second == 60:
                        self._second = 0
                        self._minute += 1
                        if self._minute == 60:
                            self._minute = 0
                            self._hour += 1
                            if self._hour == 24:
                                self._hour = 0
            
                def show(self):
                    """显示时间"""
                    return '%02d:%02d:%02d' % \
                           (self._hour, self._minute, self._second)
            
            
            def main():
                # 通过类方法创建对象并获取系统时间
                clock = Clock.now()
                while True:
                    print(clock.show())
                    sleep(1)
                    clock.run()
            
            
            if __name__ == '__main__':
                main()
            ```

            5. ### 类之间的关系,is-a、has-a和use-a关系。

               ````
               1. s-a关系也叫继承或泛化，比如学生和人的关系、手机和电子产品的关系都属于继承关系。
               2. has-a关系通常称之为关联，比如部门和员工的关系，汽车和引擎的关系都属于关联关系；关联关系如果是整体和部分的关联，那么我们称之为聚合关系；如果整体进一步负责了部分的生命周期（整体和部分是不可分割的，同时同在也同时消亡），那么这种就是最强的关联关系，我们称之为合成关系。
               3. use-a关系通常称之为依赖，比如司机有一个驾驶的行为（方法），其中（的参数）使用到了汽车，那么司机和汽车的关系就是依赖关系。
               ````

            6. 继承、多态

               提供继承信息的我们称之为父类，也叫超类或基类；得到继承信息的我们称之为子类，也叫派生类或衍生类。子类除了继承父类提供的属性和方法，还可以定义自己特有的属性和方法，所以子类比父类拥有的更多的能力

               ```python
               class Person(object):
                   """人"""
               
                   def __init__(self, name, age):
                       self._name = name
                       self._age = age
               
                   @property
                   def name(self):
                       return self._name
               
                   @property
                   def age(self):
                       return self._age
               
                   @age.setter
                   def age(self, age):
                       self._age = age
               
                   def play(self):
                       print('%s正在愉快的玩耍.' % self._name)
               
                   def watch_av(self):
                       if self._age >= 18:
                           print('%s正在观看爱情动作片.' % self._name)
                       else:
                           print('%s只能观看《熊出没》.' % self._name)
               
               
               class Student(Person):
                   """学生"""
               
                   def __init__(self, name, age, grade):
                       super().__init__(name, age)
                       self._grade = grade
               
                   @property
                   def grade(self):
                       return self._grade
               
                   @grade.setter
                   def grade(self, grade):
                       self._grade = grade
               
                   def study(self, course):
                       print('%s的%s正在学习%s.' % (self._grade, self._name, course))
               
               
               class Teacher(Person):
                   """老师"""
               
                   def __init__(self, name, age, title):
                       super().__init__(name, age)
                       self._title = title
               
                   @property
                   def title(self):
                       return self._title
               
                   @title.setter
                   def title(self, title):
                       self._title = title
               
                   def teach(self, course):
                       print('%s%s正在讲%s.' % (self._name, self._title, course))
               
               
               def main():
                   stu = Student('王大锤', 15, '初三')
                   stu.study('数学')
                   stu.watch_av()
                   t = Teacher('骆昊', 38, '砖家')
                   t.teach('Python程序设计')
                   t.watch_av()
               
               
               if __name__ == '__main__':
                   main()
               ```

               7. 多态。

                  子类在继承了父类的方法后，可以对父类已有的方法给出新的实现版本，这个动作称之为方法重写（override）









在python中继承中的一些特点：  
1：在继承中基类的构造（**init**()方法）不会被自动调用，它需要在其派生类的构造中亲自专门调用。  
2：在调用基类的方法时，需要加上基类的类名前缀，且需要带上self参数变量。区别在于类中调用普通函数时并不需要带上self参数  
3：Python总是首先查找对应类型的方法，如果它不能在派生类中找到对应的方法，它才开始到基类中逐个查找。（先在本类中查找调用的方法，找不到才去基类中找）。

__private_attrs：两个下划线开头，声明该属性为私有

yield 迭代器中使用，类似return

列表生成器 [x * x for x in range(10)]  
生成器  
(x * x for x in range(10))  // 使用next(xxx) 或者 for x in xxx  
func 里调用yield

凡是可作用于for循环的对象都是Iterable类型；  
凡是可作用于next()函数的对象都是Iterator类型，它们表示一个惰性计算的序列；

map()函数接收两个参数，一个是函数，一个是Iterable，map将传入的函数依次作用到序列的每个元素，并把结果作为新的Iterator返回  
reduce把一个函数作用在一个序列[x1, x2, x3, ...]上，这个函数必须接收两个参数，reduce把结果继续和序列的下一个元素做累积计算  
filter()也接收一个函数和一个序列,把传入的函数依次作用于每个元素，然后根据返回值是True还是False决定保留还是丢弃该元素

匿名函数 lambda x: x * x

装饰器 借助Python的@语法，把decorator置于函数的定义处  
@log  
def now():  
print('2015-3-25')

now = log(now)  
import functools是导入functools模块。模块的概念稍候讲解。现在，只需记住在定义wrapper()的前面加上@functools.wraps(func)即可。

模块  
内置模块  [https://docs.python.org/zh-cn/3/library/functions.html](https://docs.python.org/zh-cn/3/library/functions.html)  
一个.py文件就称之为一个模块（Module）  
目录来组织模块的方法，称为包（Package）  
每一个包目录下面都会有一个__init__.py的文件，这个文件是必须存在的，否则，Python就把这个目录当成普通目录，而不是一个包。**init**.py可以是空文件，也可以有Python代码，因为__init__.py本身就是一个模块，而它的模块名就是mycompany。  
作用域  
正常的函数和变量名是公开的（public），可以被直接引用  
类似__xxx__这样的变量是特殊变量，可以被直接引用，但是有特殊用途  
类似_xxx和__xxx这样的函数或变量就是非公开的（private），不应该被直接引用

默认情况下，Python解释器会搜索当前目录、所有已安装的内置模块和第三方模块, 搜索路径存放在sys.path

class  
__init__方法的第一个参数永远是self，表示创建的实例本身  
实例的变量名如果以__开头，就变成了一个私有变量（private），只有内部可以访问，外部不能访问

try...except...finally...的错误处理机制

file  
>>> f = open('/Users/michael/test.txt', 'r')  
read()方法可以一次读取文件的全部内容  
close()方法关闭文件。  
readlines()  // for line in f.readlines():

StringIO     在内存中读写st StringIO操作的只能是str，  
BytesIO     如果要操作二进制数据，就需要使用BytesIO

Python有五个标准的数据类型：  
Numbers（数字）  
int（有符号整型）  
bool True, False  
-- long（长整型，也可以代表八进制和十六进制）Python使用 L 来显示长整型。long 类型只存在于 Python2.X 版本中，  
float（浮点型）  
complex（复数） 可以用 a + bj,或者 complex(a,b) 表示， 复数的实部 a 和虚部 b 都是浮点型

````
String（字符串）
	我们可以使用引号 ( ' 或 " ) 来创建字符串,Python 三引号允许一个字符串跨多行，字符串中可以包含换行符、制表符以及其他特殊字符
	u(U)前缀表示字符串的编码方式为unicode。
	f在字符串前面，字符串中引入变量直接{变量名}，推荐使用这款,format
	r在字符串前面,此字符串中的 \, \\, ' , " , \n \b \r ... 等转义符号，就不进行转义了.raw string. 注意：两个\\不能正常处理，需要\\\\
	chr(x)将一个整数转换为一个字符
	unichr(x)将一个整数转换为Unicode字符
	ord(x)将一个字符转换为它的整数值
	hex(x)将一个整数转换为一个十六进制字符串
	oct(x) 将一个整数转换为一个八进制字符串

List（列表）
	list = [ 'runoob', 786 , 2.23, 'john', 70.2 ]
	list(s)将序列 s 转换为一个列表
Tuple（元组）
	tuple = ( 'runoob', 786 , 2.23, 'john', 70.2 )
	元组不能二次赋值，相当于只读列表。
	tuple(s) 将序列 s 转换为一个元组	
Dictionary（字典）
	字典用"{ }"标识。字典由索引(key)和它对应的值value组成
	tinydict = {'name': 'runoob','code':6734, 'dept': 'sales'}
	dict(d) 创建一个字典。d 必须是一个序列 (key,value)元组。
	键必须不可变，所以可以用数字，字符串或元组充当，所以用列表就不行，
	xxx in dict
	dict.items()
	dict.keys()

在 python 中，strings, tuples, 和 numbers 是不可更改的对象，而 list,dict 等则是可以修改的对象。
Python 模块(Module)，是一个 Python 文件，以 .py 结尾，包含了 Python 对象定义和Python语句。
	from…import 语句 让你从模块中导入一个指定的部分到当前命名空间中
	如果要给函数内的全局变量赋值，必须使用 global 语句。
	dir() 函数一个排好序的字符串列表，内容是一个模块里定义过的名字。
Python中的包 包就是文件夹，但该文件夹下必须存在 __init__.py 文件, 该文件的内容可以为空。__init__.py 用于标识当前文件夹是一个包

生成器
yield函数是一个生成器(generator),可用于迭代；在函数中yield类似于return，不同的是，yield返回一个return的值并且记住这个返回值的位置，下次迭代就从记住的这个位置开始，并且下一次迭代时，从上一次迭代遇到的yield后面的代码开始执行。
next()函数可以不断打印yield生成器的值；
send()函数特别之处在于它可以携带参数，并修改上一个表达式的值，同时用法也与next()有很多相同之处；

迭代器
	把一个类作为一个迭代器使用需要在类中实现两个方法 __iter__() 与 __next__() 。
	__iter__() 方法返回一个特殊的迭代器对象， 这个迭代器对象实现了 __next__() 方法并通过 StopIteration 异常标识迭代的完成。

try/except 语句还有一个可选的 else 子句，如果使用这个子句，那么必须放在所有的 except 子句之后。
else 子句将在 try 子句没有发生任何异常的时候执行.
try-finally 语句无论是否发生异常都将执行最后的代码
使用 raise 语句抛出一个指定的异常。
异常类继承自 Exception 类，可以直接继承，或者间接继承，

类：
	__private_attrs：两个下划线开头，声明该属性为私有，不能在类的外部被使用或直接访问。在类内部的方法中使用时 self.__private_attrs。
	在类的内部，使用 def 关键字来定义一个方法，与一般函数定义不同，类方法必须包含参数 self，且为第一个参数，self 代表的是类的实例。

	__init__ : 构造函数，在生成对象时调用
	__del__ : 析构函数，释放对象时使用
	__repr__ : 打印，转换
	__setitem__ : 按照索引赋值
	__getitem__: 按照索引获取值
	__len__: 获得长度
	__cmp__: 比较运算
	__call__: 函数调用
	__add__: 加运算
	__sub__: 减运算
	__mul__: 乘运算
	__truediv__: 除运算
	__mod__: 求余运算
	__pow__: 乘方
````