git add 命令可将该文件添加到暂存区。

添加一个或多个文件到暂存区：

```
git add [file1] [file2] ...
```

添加指定目录到暂存区，包括子目录：

```
git add [dir]
```

添加当前目录下的所有文件到暂存区：

```
git add .
```

也可以增加某种后缀名的文件到缓存

```
git add ./*.c
```

将此目录中的所有.c文件添加进入暂存区

后准加-n，表示"演练"，告诉你这条命令将添加哪些文件到暂存区

```
[chenjindou@manjaro mcu_pack]$ git add ./*.md -n
add 'readme.md'
add 'a.md'
add 'test.md'
```



**撤回放入暂存区的文件**

撤回所有放入暂存区的文件

```
git reset HEAD -- .
```

撤销指定目标文件

```
git reset HEAD -- filename
```

撤销指定目标文件

```
git rm --cached filename
```

以上三种方法只是把文件从暂存区中删除了（或者叫撤销了），并不会修改文件内容，也不会删除文件，在本地夹里还能找到。撤销后的文件状态是未被暂存的状态。



删除git中的文件

```
git rm -f filename
```

也能把文件从暂存区删除，但是此命令也同时删除了本地文件，回收站中也找不到了，慎重使用此命令来撤销暂存区的文件。

下面命令表示，a.c在git的管理中，删除a.c(git不再管理这个文件)，但是本地不删除这个文件

```
git rm  --cached  a.c
```

