# 14 · 尾随零（最低置位位置）

**头文件：** [`trailing_zeros.hpp`](../include/bithacks/trailing_zeros.hpp)

`countr_zero(v)` = 最低位 1 下面有多少个 0；`v==0` 时返回位宽。

## 方法

| 方法 | 要点 |
|------|------|
| 线性 | 循环看最低位 |
| 并行 | 隔离最低 1 后用掩码减宽度 |
| 二分 | 低半全 0 则跳半 |
| float | 隔离后转 float 读指数 |
| mod 37 表 | `isolate % 37` 查表 |
| De Bruijn | `isolate * magic >> k` 查表 |

## 隔离最低置位

```cpp
iso = v & -v;  // 对 unsigned 用 two's complement wrap
```

这是 ctz、下一排列、树状数组 lowbit 的共同积木。

## 标准库

```cpp
std::countr_zero(v);
```

下一章：[15 · 上取整到 2 的幂](15-round-power2.md)
