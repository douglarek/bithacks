# 06 · 符号扩展

**头文件：** [`sign_extend.hpp`](../include/bithacks/sign_extend.hpp)

## 问题

一个只占 `b` 位的补码小数，扩展成完整 `int`：例如 4 位 `0b1000` → `-8`。

## 方法 1：掩码异或减法（可移植）

```cpp
m = 1 << (b - 1);           // 符号位掩码
x = x & ((1 << b) - 1);     // 清高位
r = (x ^ m) - m;
```

若符号位为 0，`x^m` 只是加上 `m`，再减 `m` 还原；若符号位为 1，异或清掉符号位后减去 `m` 得到负数。

## 方法 2：算术移位

```cpp
s = width - b;
r = (x << s) >> s;  // 依赖算术右移
```

## 方法 3：乘除表（约 3 次运算）

原站用 `multipliers[b] / divisors[b]` 把符号位移到顶再除回来。见 `sign_extend_muldiv`。

## 常量宽度

可用模板参数 `sign_extend_const<Bits>(x)`。原站 bitfield 写法在 C++ 中符号性敏感，本教程改用算术实现以保证语义清晰。

下一章：[07 · 条件位操作](07-conditional.md)
