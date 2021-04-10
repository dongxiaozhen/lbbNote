# unicode 是字符集，相当于一个数据库，每个字都对应一个编码

    1. 可以在下面的网址里搜索字的编码

     > https://www.unicode.org/cgi-bin/GetUnihanData.pl 

# utf-8,utf-16,utf-32 是字符编码， 使用不能的方法来编码(十进制的100 用不能的编码占用的空间不同,数值不同)

## utf-8   
    
    * 0~127         0xxxxxxx
    * 128~2047      110xxxxx    10xxxxxx
    * 2048~65535    1110xxxx	10xxxxxx	10xxxxxx
    * 65536~2097151 11110xxx 	10xxxxxx	10xxxxxx	10xxxxxx

    * uft8 兼容ASCII 
    * 从左边的1的数量可知道占用几个字节空间


## utf-16

    * 每个字符使用2个或者4个字符编码


## utf-32  每个字符都是用4个字节标示
