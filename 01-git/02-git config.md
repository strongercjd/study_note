git config命令用于git的配置

显示当前git的配置信息

```
git config --list
```

其他操作

```
//查
git config --global --list

git config --global user.name
git config --global user.email

//增
git config  --global --add user.name xxx
git config  --global --add user.email xxx

//删
git config  --global --unset user.name xxx
git config  --global --unset user.email xxx

//改
git config --global user.name xxx
git config --global user.email xxx
```

