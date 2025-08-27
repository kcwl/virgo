# virgo
protocol for aquarius

## What is Virgo?
Virgo is a protocol processing component for [aquarius](https://github.com/kcwl/aquarius). The purpose of Virgo is to provide a universal sulotion for most protocol that as long as simple declarations are used, a certain type of protocol can be supported.

## How many protocols are supported?
Ceurently, Virgo supports the following protocols:
+ TCP
+ HTTP

A universal and extended solution will be provided in the future

## How to use Virgo?
Virgo provides a schema languages to describe a protocol. The schema language is called `Virgo Schema Language` (VSL).We use keyword that called `protocol` to define a protocol.
It likes this:
```
protocol SomeProtocol {};
```

The protocol structure contains three parts: `router`,`header` and `body`; Among them, the `router` is nessary, Virgo will use it to match the protocol. According the different protocols, the `header` and `body` may be omitted. such as: In TCP protocol, the `header` and `body` can be omitted, but in HTTP protocol, the `header` are necessary. It likes this:
```
protocol SomeProtocol 
{
router:
    ...

header:
    ...

body:
    ...
};
```

### Router
The naming of `router` consists of two parts, the one is protocol type and the other is the type of router value. Currently, Virgo supports the following types:
+ tcp-key  // router value is a key of TCP packet
+ http-regex  // router value is a regex of HTTP path

So, define a TCP protocol, it likes this:
```
protocol SomeTCPProtocol
{
router:
    tcp-key: 1000;
};
```
define a HTTP protocol, it likes this:
```
protocol SomeTCPProtocol
{
router:
    http-regex: ^/login$;
};
```

### Header And Body
For protocols, the `header` and `body` are composed of `key + value`, they are ending with `;`.
Such as TCP like this:

```
protocol SomeTCPProtocol
{
router:
    tcp-key: 1000;

header:
    int id;
    int64 timestamp;

body:
    string username;
    string password;
};
```

Such as HTTP like this:
```
protocol SomeHTTPProtocol
{
router:
    http-regex: ^/login$;

header:
    Content-Type: application/json;
    Content-Length: 1024;

body:
    string username;
    string password;
};
```

### Compile Virgo Schema
Virgo provides a command line tool that called `lazytool` to compile VSL into C++ code.
Use the following command to compile VSL:
`lazytool --input test.virgo --cpp_out ./test`
that will be generated a C++ file in `./test` directory.Most usages of `lazytool` can be found in `lazytool --help`.

## Wiki

## Contact us