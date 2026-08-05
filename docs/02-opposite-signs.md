# 02 · 检测两个整数是否异号

**头文件：** [`sign.hpp`](../include/bithacks/sign.hpp)

## 问题

`x` 与 `y` 是否一正一负？（通常把 0 视为非负。）

## 技巧

符号位不同 ⇔ `x ^ y` 的符号位为 1：

```cpp
bool f = (x ^ y) < 0;
```

因为异或后，仅当两个符号位不同时结果为负。

## 为什么有用

- 检测乘法会不会因异号产生负结果（在不考虑溢出时）
- 分支预测友好的快速筛选

## 边界

- `0` 与正数：同号（都非负）  
- `0` 与负数：异号  
- 与「数学上 sign(x)*sign(y) < 0」在 0 处一致于「0 的符号取 +」的约定

## 实现

```cpp
template <std::signed_integral T>
constexpr bool opposite_signs(T x, T y) noexcept {
  return (x ^ y) < 0;
}
```

上一章：[01 · 符号](01-sign.md) · 下一章：[03 · 绝对值](03-abs.md)
