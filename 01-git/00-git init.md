在执行完成 git init 命令后，Git 仓库会生成一个 .git 目录，该目录包含了资源的所有元数据，其他的项目目录保持不变。

```
git init
```

这个会在当前仓库初始化git，如果想在制定目录初始化git仓库，后面需要追加文件夹路径

```
git init gittest
```

会在当前目录下创建gittest目录，并且在gittest目录中创建git仓库