
# 使用的Ubantu 20.04 LTS

## 安装工具链
$ sudo apt-get install git build-essential gdb-multiarch qemu-system-misc gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu 
## 移除掉qemu的新版本安装旧版本
$ sudo apt-get remove qemu-system-misc

\$ wget https://download.qemu.org/qemu-5.1.0.tar.xz

\$ tar xf qemu-5.1.0.tar.xz

\$ cd qemu-5.1.0

\$ ./configure --disable-kvm --disable-werror --prefix=/usr/local --target-list="riscv64-softmmu"

\$ make

\$ sudo make install

## 克隆xv6实验仓库
\$ git clone git://g.csail.mit.edu/xv6-labs-2020

$ cd xv6-labs-2020

$ git checkout util

# 进行编译
$ make qemu
## 编译成功并进入xv6操作系统
$ xv6 kernel is booting