## Flex & Bison

**Building**

```
$ make wc calc
```

**Example usage**

`wc.l`

```sh
$ ./wc
test
<C-d>
1 1 5
$ echo "test" | wc
1 1 5
```

----------

`calc.{l, y}`

```sh
$ ./calc
(3 * (8 - 2) / |-4|) % 2
 = 0
(3 % 2
error: syntax error
<C-d>
```
