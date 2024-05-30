# Lox Grammar

## operator precedence and associativity

The following table lists the precedence and associativity of Lox operators.

Operators are listed top to bottom, in descending precedence.

| Name       | operators            | associativity   |
| ---        | ---                  | ---             |
| Unary      | `!`, `-`             | right->to->left |
| Factor     | `*`, `/`             | left->to->right |
| Term       | `+`, `-`             | left->to->right |
| Comparison | `<`, `<=`, `>`, `>=` | left->to->right |
| Equality   | `==`, `!=`           | left->to->right |

## rules

expression -> Equality;

equality -> comparison ( ("==" | "!=") comparison )*;

comparison -> term ( ("<" | "<=" | ">" | ">=") term )*;

term -> factor ( ("+" | "-") factor )*;

factor -> unary ( ("*" | "/") unary )*;

unary -> ("!" | "-") unary |
         primary;

primary -> NUMBER |
           STRING |
           "true" |
           "false" |
           "nil" |
           "(" expression ")"
