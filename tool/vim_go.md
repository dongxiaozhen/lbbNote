# vim-go 快捷键

ref https://github.com/fatih/vim-go/wiki/Tutorial

## 操作函数

1. vif 选中函数，

2. cif 修改函数, caf

3. dif 删除函数, daf

4. yif 复制函数，yaf
   
   5.  :GoDefStack 多次跳转后，查看跳转栈
   
   6.  `GoDecls` 展示当前文件的函数和结构体定义
   
   7. `GoDeclsDir` 展示当前包下的函数和结构体定义
   
   8. ]] -> jump to next function
      [[ -> jump to previous function
   
   9. `GoReferrers` 调用函数的所有地方
   
   10. `GoCallers` 查看调用该函数的地方
   
   11. `GoCallees` 
   
   12. `GoCallstack`
   
   ------

## 操作结构体

1. gS 成员分行显示

2. gJ 成员和并

3.  成员后面输入json 自动加tag
   
   
   
   ------

## 操作包引用

1.  GoImport path

2. GoImportAs xxx_alias path
   
   ------

## ## snip

```
fn -> fmt.Println()
ff -> fmt.Printf()
ln -> log.Println()
lf -> log.Printf()
errp -> 	if err != nil {
		        panic()
	        }

```

-----

## test

1. GoAlternate 测试文件和函数直接跳转



## identifiers

1. `GoRename`重命名

2. `GoImpl` 实现接口方法 :GoImpl b *B fmt.Stringer

3. `GoImplements` 查看结构实现了哪个接口
