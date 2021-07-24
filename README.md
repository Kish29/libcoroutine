基于C++11标准的协程库，根据腾讯libco进行改造
> 故该框架仅支持c++11及以上
>
> 其目的就是简化协程的lambda的编写，提升开发幸福感

思路借鉴腾讯[libco](https://github.com/Tencent/libco):

CPU架构支持apple silicon(arm64):
> 本人用Mac mini M1版进行的开发，已在x86_64的ArchLinux和aarch64的fedora上面编译通过