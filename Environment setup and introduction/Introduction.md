# 目录结构介绍
![Alt text](image.png)
kernel: 系统调用函数（内核）

mkfs: 用于创建文件系统映像的程序，初始化一个文件系统

user: 用户层函数（lab作业就放这里面）

grade-lab-util: lab作业测试打分文件

每次lab作业完成后，将起编译选项加入makefile里面，之后./grade-lab-util+函数名即可测试