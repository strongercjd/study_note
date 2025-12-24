
# onlinelog  

手动开log

``` bash
X_TOOL=log xspace_tool -o switch -n main_board.log -p '' -s 1 -i 0
X_TOOL=log xspace_tool -o level -n main_board.log -p '' -l 0
```

# 查死机  

符号未定义的问题

``` bash
# cat /tmp/log/log.txt
deebot: symbol lookup error: /data/usr/lib/node/liberos_node_alert.so: undefined symbol: _Z18AlertSystemProcessP17AlertSystemElem_s13MessageType_e
```

使用c++filt命令

``` bash
gengwenfeng@Ecovacs:~/chenjd$ c++filt _Z18AlertSystemProcessP17AlertSystemElem_s13MessageType_e
AlertSystemProcess(AlertSystemElem_s*, MessageType_e)
```

**关于c++filt命令**  
c++函数AlertSystemProcess(AlertSystemElem_s*, MessageType_e)在linux系统下编译之后会变成如下样子

``` bash
_Z18AlertSystemProcessP17AlertSystemElem_s13MessageType_e
```

在linux命令行使用c++filter

``` bash
$ c++filt _Z18AlertSystemProcessP17AlertSystemElem_s13MessageType_e
AlertSystemProcess(AlertSystemElem_s*, MessageType_e)
```

可以得到函数的原始名称， 展开后续追踪  
方法2：
找/data/log/deebot-bt-*  
找到最新的  

``` bash
ls -alt deebot-bt-*
```

知道最新的log，然后打开，查看signal handler called后面的内容

# 驱动轮速度分析  

驱动轮沈工那里没有进行日志保存，可以使用speed_detecter工具进行速度查看  
拷贝speed_detecter工具到地宝/data/目录下，  
给speed_detecter可执行权限  

``` bash
chmod +x speed_detecter
```

配置环境

``` bash
source /opt/ros/melodic/setup.sh 
export HOME=/data/
```

执行工具，会自动打印

``` bash
./speed_detector
```

需要保存打印的速度

``` bash
./speed_detecter > /tmp/speed.txt
```

# debug环境  

创建文件夹并拷贝文件

``` bash
mkdir /data/autostart
cp /etc/rc.d/deebot.sh /data/autostart/
vi /data/autostart/deebot.sh  
```

> deebot.sh  是启动脚本  

- 删除load的if前三行  
  > lib库搜索路径  
- 增加export LD_LIBRARY_PATH=/data/usr/lib:${LD_LIBRARY_PATH}  
- deebot改到/data/下 

``` bash
cp /etc/conf/dxai_node.json /data/
vi /data/dxai_node.json
```

path路径修改，node节点搜索路径，修改成如下

``` bash
"path" : "/data/usr/lib/node",
```

拷贝节点文件

``` bash 
mkdir -p /data/usr/lib/node
cp  /usr/lib/node/*   /data/usr/lib/node
sync
```
