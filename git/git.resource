git log --pretty=raw 查看commitID信息
	tree 代表目录树
	
git cat-file -p treeID 显示目录下的内容
	tree代表目录树，递归遍历即可
	blob 代表文件，blobID的前两位代表.git/object/下的目录，后38位代表文件名，

.git/refs/head目录下的保存分支信息,命令git branch也可以显示

.git/logs/refs/head 目录下保存分支的历史记录, 命令git reflog show branchname 也可以显示
 
