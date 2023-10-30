# 新增一个远程仓库

命令格式
``` cpp
git remote add <name> <url>
git push <name> <branch>
```
使用示例
``` cpp
git remote add origin git@gitee.com:holyking/test-1.git
git push origin master
```
# 查看当前关联的远程仓库

``` cpp
git remote -v
```

# 远程仓库冲突
删除远程仓库新并新增一个
``` cpp
git remote rm <name>
git remote add <name> <url>
```
修改远程仓库的地址
``` cpp
git remote set-url <name> <url>
```

# 重命名远程仓库

命令格式
``` cpp
git remote rename <old> <new>
```
使用示例
``` cpp
git remote rename origin liang
```



