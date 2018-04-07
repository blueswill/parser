# parser
简单的正规词法解析器

# 语法

## 词法说明

* 格式 <token> <simple regex>

__token__ 中不能出现空白字符和其他特殊符号, 否则会解析异常, 第一个空白字符视为分割符, 之后的所有*非换行*空白字符视为一般字符.

regex 出现的次序作为它们的优先级. 比如 *for* 能够匹配 /for/ 以及 /[a-z]\*/ 两种, 若 /for/ 出现得早, 则最终采用
/for/ 的结果

* simple regex rules

| 符号 | 意义 |
| --- | --- |
| (c) | 用括号来强制优先级 |
| . | 任意一个字符 |
| [abc] | a 或者 b 或者 c |
| [\^abc] | 不是 a/b/c 中的任何一个 |
| [0-9] 或 [\^0-9] | 区间表示, 意义类似 |
| c\* | 任意多个 c 字符 |
| c? | 零到一个 c 字符 |
| c+ | 多于零个的 c 字符 |
| A \| B | A 或者 B |

优先级为: () > [] > * == ? == + > \|

## 语法说明

* 格式

[\<non-terminal token\> ... ]

\<non-terminal token\> [\<token\> ... ]
\<non-terminal token\> [\<token\> ... ]
...
    
其中第一条产生式的 __token__ 为开始符号, 第一行中的所有 __token__ 都是非终结符, 以下产生式中出现的其他 __token__ 都视为终结符.
出现在句首(*忽略空白字符*)的 __|__ 字符视为该条产生式和上条共用一个 __token__, 其他地方的 __|__ 符号无此特殊含义.

对于所有的终结符 __token__, 在词法中必须有相关的条目. 否则视为错误.

# Regular Expression CFG

E T M F K

E E | T

E T

T M T

T M

M M *

M M +

M M ?

M F

F ( E )

F c

F .

F [ K ]

F [ ^ K ]

K K c

K K c - c

K c

K c - c
