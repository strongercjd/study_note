# docker笔记

## 镜像操作

查看镜像

``` bash
e$ sudo docker images
                                                                                                            i Info →   U  In Use
IMAGE                ID             DISK USAGE   CONTENT SIZE   EXTRA
hello-world:latest   d4aaab6242e0       25.9kB         9.52kB    U
ubuntu:latest        c35e29c94501        119MB         31.7MB    U

```

删除镜像

```bash
sudo docker rmi ubuntu:latest
```

删除镜像时，如果有容器在运行

```bash
# 停止容器
sduo ocker stop <容器ID>
# 删除容器
sudo docker rm <容器ID>
# 再删除镜像
sudo docker rmi <镜像ID>
```


打包镜像

```bash
sudo docker save -o docker_ubuntu1604_image.tar ubuntu:16.04
```

导入镜像

``` bash
sudo docker load -i docker_ubuntu1604_image.tar
```



## 容器操作

查看docker运行的容器

``` bash
sudo docker ps
```

基于镜像启动一个容器

``` bash
$ sudo docker run -itd --name my-ubuntu ubuntu:latest
b87a3469bbbe93d927ff5ad26f86443fbf43d723118521a917a0446c54219d65
$ sudo docker ps
CONTAINER ID   IMAGE           COMMAND       CREATED          STATUS         PORTS     NAMES
b87a3469bbbe   ubuntu:latest   "/bin/bash"   10 seconds ago   Up 9 seconds             my-ubuntu
```

启动时挂载一下目录:本机目录`/home/chenjd/code`挂载到容器中的`/home/ubuntu/code`,容器中不存在的目录，会自动创建。
且必须在启动时挂载

``` bash
sudo docker run -itd --name my-ubuntu -v /home/chenjd/code:/home/ubuntu/code ubuntu:latest
```

进入容器终端

``` bash
$ sudo docker exec -it my-ubuntu /bin/bash
root@b87a3469bbbe:/#
```


