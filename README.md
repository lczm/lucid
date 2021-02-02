# Lucid

## Building Lucid
Run the following commands with a shell that has access to `ninja` and your preferred compiler.

### Clone
```
git clone https://github.com/lczm/lucid
cd lucid
git submodule update --init --recursive
```

### Build dependencies
```
python ./bin/build_deps.py
```

### Build Lucid
```
cd build
cmake -GNinja -DDEBUG=1 -DRELEASE=0 -DROOT=. -DCMAKE_BUILD_TYPE=Release ..
ninja lucid
```
