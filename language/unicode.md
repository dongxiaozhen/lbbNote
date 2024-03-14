## [unicode 字符集和字符编码](https://zhuanlan.zhihu.com/p/165524551)

# unicode 是字符集，相当于一个数据库，每个字都对应一个编码

````
1. 可以在下面的网址里搜索字的编码

 > https://www.unicode.org/cgi-bin/GetUnihanData.pl 
````

# utf-8,utf-16,utf-32 是字符编码， 使用不能的方法来编码(十进制的100 用不能的编码占用的空间不同,数值不同)

## [utf-8](https://www.utf8-chartable.de/unicode-utf8-table.pl)

````
* 0~127         0xxxxxxx
* 128~2047      110xxxxx    10xxxxxx
* 2048~65535    1110xxxx	10xxxxxx	10xxxxxx
* 65536~2097151 11110xxx 	10xxxxxx	10xxxxxx	10xxxxxx

* uft8 兼容ASCII 
* 从左边的1的数量可知道占用几个字节空间
````

## utf-16

````
* 每个字符使用2个或者4个字符编码
````

## utf-32  每个字符都是用4个字节标示

## [归一化 Normalizeation Form（NF）](https://unicode.org/reports/tr15/)

类别

- C 表示 组合(Composition)

  **组合(Composition),** 组合是尽量在Unicode的字符数据库中找到一个尽量短码点来表示上面结构的出的码点，


- D 表示 解构(Decomposition)

  **解构(Decomposition)**, 指的是从Unicode字符数据库当中去查找更细的部分，尽量分成更多的码点，来拆解这个字符编码


- K 表示 兼容(Compatibility)

  如果字符是标准等价，那么他们一定是兼容等价，反之就不一定了。兼容性等效是代表相同抽象字符（或抽象字符序列）的字符或字符序列之间的较弱等效类型，但可能具有不同的视觉外观或行为


### 种类

- NFD：规范化分解

- NFKD：兼容性分解

- NFC：规范化分解后再规范化组合

- NFKC：兼容性分解后再规范化组合

[链接](https://unicode.org/reports/tr15/)

码点有七种基本类型 (categories), 而每个分类下还有子分类 :

- **Graphic** : 分为字母 (L, letter), 标记 (M, combining mark), 数组 (N, number), 标点 (P, punctuation), 符号 (S, symbol), 空格 (Zs, space separator) 六类.
- **Format** : 一些不可见但对相邻字符起作用的字符, 如零宽连字  (zero width joiner, ZWJ), 零宽不连字 (zero width  
   non joiner, ZWNJ) 等, 包括行和段分隔符. 有 Cf (format), Zl (line separators), Zp   
  (paragraph separators) 三类.
- **Control** (Cc) : 控制字符, 没有明确作用, 常用于格式化字符, 如异体字选择器 (variation selector, VS) 等.
- **Private-use** (Co) : 私人使用区, 未在规范中指定, 合作用户之间可根据自定义协定决定其用途.

### 零宽度字符

- 零宽度空格符 (zero-width space) U+200B : 用于较长单词的换行分隔
- 零宽度非断空格符 (zero width no-break space) U+FEFF : 用于阻止特定位置的换行分隔
- 零宽度连字符 (zero-width joiner) U+200D : 用于阿拉伯文与印度语系等文字中，使不会发生连字的字符间产生连字效果
- 零宽度断字符 (zero-width non-joiner) U+200C : 用于阿拉伯文，德文，印度语系等文字中，阻止会发生连字的字符间的连字效果
- 左至右符 (left-to-right mark) U+200E : 用于在混合文字方向的多种语言文本中，规定排版文字书写方向为左至右
- 右至左符 (right-to-left mark) U+200F : 用于在混合文字方向的多种语言文本中，规定排版文字书写方向为右至左

### [unicode character categories](https://www.compart.com/en/unicode/category)

