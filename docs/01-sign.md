# 01 · 计算整数的符号

**头文件：** [`sign.hpp`](../include/bithacks/sign.hpp)

## 问题

给定有符号整数 `v`，求：

| 需求 | 结果 |
|------|------|
| 负则 -1，否则 0 | `sign_neg01` |
| 负则 -1，否则 +1（0→+1） | `sign_pm1` |
| -1 / 0 / +1 | `sign_neg_zero_pos` |
| 非负则 1，否则 0 | `sign_nonneg01` |

## 朴素写法

```cpp
int sign = (v > 0) - (v < 0);  // 已经很棒
```

比较在多数 ISA 上会变成条件码；在「必须无分支、或要掩码」时，位写法更直接。

## 位技巧

### -1 或 0

```cpp
sign = -(v < 0);
// 或：把符号位逻辑右移到最低位再取负
using U = unsigned;
sign = -int(U(v) >> (sizeof(int)*CHAR_BIT - 1));
// 非可移植高速：算术右移
sign = v >> (sizeof(int)*CHAR_BIT - 1);
```

### -1 / 0 / +1

```cpp
sign = (v > 0) - (v < 0);                 // 推荐
sign = (v != 0) | (v >> (width - 1));      // 依赖算术右移
```

### 非负指示

```cpp
sign = 1 ^ int(U(v) >> (width - 1));  // v>=0 → 1, v<0 → 0
```

## C++23 实现要点

见 `bithacks::sign_neg01`、`sign_neg_zero_pos` 等。优先比较式可移植版本；`*_shift` 后缀表示依赖算术右移。

## 边界

- `v == 0` 在 `sign_pm1` 中映射为 `+1`（与「严格 sgn」不同）  
- 不要假设所有平台都是算术右移（教学代码已标出）

## 测试思路

对 `{INT_MIN, -1, 0, 1, INT_MAX}` 验证四种语义。

下一章：[02 · 异号检测](02-opposite-signs.md)
