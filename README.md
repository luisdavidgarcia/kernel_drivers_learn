# Getting Started

I already had Fedora installed, so Linux completed.

However, I just found out about KVM and QEMU.Best way to learn about it is here directly: [Virtulization](https://developer.fedoraproject.org/tools/virtualization/about.html)

I then use virtual-manager to load an ubuntu server ISO, which I got ssh working 
with just because it might be useful to know both: [Ubuntu Server](https://ubuntu.com/download/server)

Now just ssh'ing and using as my sandbox

Yet it is important to perform an update and get the linux headers/libraries,
the uname basically here will gives us the version

```sh
sudo apt update && upgrade -y
sudo apt install -y build-essential linux-headers-$(uname -r) kmod
```
