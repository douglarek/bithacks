# 03 · 无分支绝对值

**头文件：** [`abs.hpp`](../include/bithacks/abs.hpp)

## 问题

计算 `|v|`，避免 `v < 0 ? -v : v` 的分支（在分支很贵的架构上）。

## 核心恒等式

令 `mask = v < 0 ? -1 : 0`（全 1 或全 0），则：

```text
(v + mask) ^ mask  == |v|
(v ^ mask) - mask  == |v|   // 同操作数的变体
```

直觉：若 `mask == 0`，原样返回；若 `mask == -1`，`v + (-1)` 再按位取反，等价于 `-v`（补码）。

## INT_MIN 陷阱

在 32 位补码中，`|INT_MIN| = 2^31` **不能**放进 `int`。  
因此本教程 API 返回 **`unsigned`**：

```cpp
auto r = bithacks::abs_unsigned(v);
```

## 与标准库

- `std::abs` / `std::labs`：有符号，仍有 `INT_MIN` 问题  
- 生产代码：先确认值域，或使用更宽类型

## 实现变体

| 函数 | 说明 |
|------|------|
| `abs_unsigned` | 可移植 mask（比较） |
| `abs_unsigned_shift` | 算术右移 mask |
| `abs_unsigned_xor_sub` | `(v^mask)-mask` |
| `abs_signed_unsafe` | 有符号结果，勿传入 `min()` |

下一章：[04 · min/max](04-minmax.md)
