# byteboard
Byteboard is a free UCI chess engine written in C

## Installation
### Compiling from source

Build using [meson](https://github.com/mesonbuild/meson) and [ninja](https://github.com/ninja-build/ninja)

```bash
meson build/
ninja -C build/
```

Running
```bash
cd build
./bb
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

Please make sure to update tests as appropriate.

### Documentation
Creating documentation using doxygen
```bash
doxygen Doxyfile
```

### Unit tests
We use the [unity](https://github.com/ThrowTheSwitch/Unity.git) framework and [ceedling](https://www.throwtheswitch.org/ceedling)

To run all tests:
```bash
ceedling test:all
```

Following tdd, after creating a branch with new features, the first thing to do is to create unit tests. All tests must be in `test/`.

## License
[MIT](https://choosealicense.com/licenses/mit/)yteboard is a free UCI chess engine written in C
