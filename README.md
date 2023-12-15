# Group Project

## Getting Started

This project is intended only to run on UNIX systems. \
It has been compiled and tested on MacOS 14.3.

### Prerequisites

- [G++](https://gcc.gnu.org/)
- [Make](https://www.gnu.org/software/make/)
- [GoogleTest](https://github.com/google/googletest)
- [Doxygen](http://www.doxygen.nl/) (Optional)

### Building

```bash
make all
```

## Operation

### Running Server

```bash
./out/server
```

### Running Client

```bash
./out/client
```

To demonstrate functionality, the ideal setup is to run the server and at least two clients concurrently.

## Documentation

Documentation is provided via Doxygen.

### Building

```bash
doxygen Doxyfile
```

### Viewing

```bash
open out/doxygen/html/index.html
```

## Testing

### Building

```bash
make client_test common_test server_test
```

### Running

```bash
./out/test_server
./out/test_client
./out/test_common
```

