# tpin

This tool visualizes how OpenMP thread are pinned to cores in a multi-socket machine.
It assumes that hardware threads are numbered such that the i-th CPU is on NUMA domains _i mod N_ where _N_ is the number of domains.
This assumption might not hold on your current machine.

## Install

Just run `make`.

## Examples

In a system with 2 sockets and 2 threads per core
```
OMP_NUM_THREADS=10 ./tpin
```
_could_ print:
```
  0 on  27 [--------------] [-------------0]
  1 on  48 [----------1---] [--------------]
  2 on  37 [--------------] [----1---------]
  3 on  54 [-------------1] [--------------]
  4 on  54 [-------------1] [--------------]
  5 on  10 [-----0--------] [--------------]
  6 on   8 [----0---------] [--------------]
  7 on   6 [---0----------] [--------------]
  8 on  35 [--------------] [---1----------]
  9 on  41 [--------------] [------1-------]
           [---111----1--2] [---11-1------1]
```
`0 on  27` means that the OpenMP thread ID 0 has been pinned to CPU27.
The large bars, one per socket, represent where threads are pinned.
In this case thread ID 0 is on the 28th CPU of the system,
which is the last one of the second socket.
Characters `0` and `1` are used to distinguish between the two different
**hardware threads** of the same core.
The last row is the total number of threads that have been pinned to each
physical core.

```
OMP_NUM_THREADS=5 taskset -c 6,8 ./tpin 2 2
```
_could_ print:
```
  0 on   8 [----0---------] [--------------]
  1 on   6 [---0----------] [--------------]
  2 on   8 [----0---------] [--------------]
  3 on   8 [----0---------] [--------------]
  4 on   6 [---0----------] [--------------]
           [---23---------] [--------------]
```

```
OMP_NUM_THREADS=28 OMP_PLACES=0:14:2,1:14:2 ./tpin
```
_will_ print:
```
  0 on   0 [0-------------] [--------------]
  1 on   2 [-0------------] [--------------]
  2 on   4 [--0-----------] [--------------]
  3 on   6 [---0----------] [--------------]
  4 on   8 [----0---------] [--------------]
  5 on  10 [-----0--------] [--------------]
  6 on  12 [------0-------] [--------------]
  7 on  14 [-------0------] [--------------]
  8 on  16 [--------0-----] [--------------]
  9 on  18 [---------0----] [--------------]
 10 on  20 [----------0---] [--------------]
 11 on  22 [-----------0--] [--------------]
 12 on  24 [------------0-] [--------------]
 13 on  26 [-------------0] [--------------]
 14 on   1 [--------------] [0-------------]
 15 on   3 [--------------] [-0------------]
 16 on   5 [--------------] [--0-----------]
 17 on   7 [--------------] [---0----------]
 18 on   9 [--------------] [----0---------]
 19 on  11 [--------------] [-----0--------]
 20 on  13 [--------------] [------0-------]
 21 on  15 [--------------] [-------0------]
 22 on  17 [--------------] [--------0-----]
 23 on  19 [--------------] [---------0----]
 24 on  21 [--------------] [----------0---]
 25 on  23 [--------------] [-----------0--]
 26 on  25 [--------------] [------------0-]
 27 on  27 [--------------] [-------------0]
           [11111111111111] [11111111111111]
```

```
OMP_NUM_THREADS=28 OMP_PLACES=0:28 ./tpin
```
_will_ print:
```
  0 on   0 [0-------------] [--------------]
  1 on   1 [--------------] [0-------------]
  2 on   2 [-0------------] [--------------]
  3 on   3 [--------------] [-0------------]
  4 on   4 [--0-----------] [--------------]
  5 on   5 [--------------] [--0-----------]
  6 on   6 [---0----------] [--------------]
  7 on   7 [--------------] [---0----------]
  8 on   8 [----0---------] [--------------]
  9 on   9 [--------------] [----0---------]
 10 on  10 [-----0--------] [--------------]
 11 on  11 [--------------] [-----0--------]
 12 on  12 [------0-------] [--------------]
 13 on  13 [--------------] [------0-------]
 14 on  14 [-------0------] [--------------]
 15 on  15 [--------------] [-------0------]
 16 on  16 [--------0-----] [--------------]
 17 on  17 [--------------] [--------0-----]
 18 on  18 [---------0----] [--------------]
 19 on  19 [--------------] [---------0----]
 20 on  20 [----------0---] [--------------]
 21 on  21 [--------------] [----------0---]
 22 on  22 [-----------0--] [--------------]
 23 on  23 [--------------] [-----------0--]
 24 on  24 [------------0-] [--------------]
 25 on  25 [--------------] [------------0-]
 26 on  26 [-------------0] [--------------]
 27 on  27 [--------------] [-------------0]
           [11111111111111] [11111111111111]
```
