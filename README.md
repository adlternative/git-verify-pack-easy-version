## git-verify-pack-easy-verison

Which can only use for one blob, tree, commit.

### build
```sh
$ mkdir build
$ cd build
$ cmake ..
$ make -j8
```

### usage
```sh
$ ./a.out <pack-path>|<idx-path>
```

### example
```sh
$ ./a.out ~/ali_git/.git/objects/pack/pack-79d6064a63dcc40cd3eac23c37197bfa901fbe14.idx
objectname=23e9f694cc51e1bc793892e01784977ab0635318
objecttype=blob
objectname=5ede01d6714cac202e7cf18e9298b35d07980fa2
objecttype=commit
objectname=6510de6c3fd28ab98d3ce6723ca2612ffb2931fc
objecttype=tree
```