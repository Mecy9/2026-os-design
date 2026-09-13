# 同济大学操作系统课程设计

本仓库基于 MIT 6.1810 Fall 2025 的 `xv6-labs-2025`。课程设计选择
“B：xv6 及 Labs 课程项目”，九个实验分别保存在同名分支中，便于按照
MIT 的原始分支结构复现、测试和答辩演示。

## 分支与测试结果

| 分支 | 实验主题 | 最终提交 | `make grade` |
| --- | --- | --- | --- |
| `util` | Unix utilities | `0cd4c72` | 131/131 |
| `syscall` | System calls | `f0381a1` | 45/45 |
| `pgtbl` | Page tables | `08f81db` | 41/41 |
| `traps` | Traps | `84f530e` | 95/95 |
| `cow` | Copy-on-write | `a982660` | 130/130 |
| `net` | Network driver | `74c7453` | 171/171 |
| `lock` | Lock | `fdc9307` | 100/100 |
| `fs` | File system | `8652368` | 100/100 |
| `mmap` | mmap | `a8f365f` | 170/170 |

总计：983/983。以上结果在 Apple Silicon macOS、QEMU 11.1.1 和
`riscv64-unknown-elf-gcc 16.1.0` 环境中重新执行官方评分脚本得到。

## 复现方法

```sh
git switch util                 # 替换为需要检查的实验分支
make clean
make grade
```

进入 xv6 交互环境：

```sh
make qemu
```

退出 QEMU：先按 `Ctrl-a`，松开后再按 `x`。

## 阅读顺序

建议先阅读仓库根目录的原版 `README` 和 MIT 课程主页，再按
`util → syscall → pgtbl → traps → cow → net → lock → fs → mmap`
的顺序切换分支。每个分支只包含该实验相对官方起始分支的修改，
可以使用下面的命令查看实现范围：

```sh
git diff origin/分支名...分支名
```

课程报告、答辩 PPT 和答辩知识手册作为单独提交材料，不把源码压缩包
放入作业系统；作业系统中只提交本仓库的托管链接。

源码托管地址：<https://github.com/Mecy9/2026-os-design>

## 上游与许可

上游课程仓库：`git://g.csail.mit.edu/xv6-labs-2025`。
本项目保留 xv6 原有版权与许可说明，仅用于课程学习与实验复现。
