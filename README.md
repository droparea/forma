# forma

forma is an alternative to printf (almost compatible) especially for embedded systems and highly customizable

## API

### Output: standard output (STDOUT)
```C
unsigned forma_Print ( const char* fmt, ... ) ;
unsigned forma_PrintArgList ( const char* fmt, va_list ap ) ;
```

### Output: write to file descriptor 1
```C
unsigned forma_Write ( const char* fmt, ... ) ;
unsigned forma_WriteArgList ( const char* fmt, va_list ap ) ;
```

### Output: FILE*
```C
unsigned forma_File ( FILE* file, const char* fmt, ... ) ;
unsigned forma_FileArgList ( FILE* file, const char* fmt, va_list ap ) ;
```

### Output: file descriptor
```C
unsigned forma_FileDescriptor ( int fileDescriptor, const char* fmt, ... ) ;
unsigned forma_FileDescriptorArgList ( int fileDescriptor, const char* fmt, va_list ap ) ;
```

### Output: char buffer
```C
unsigned forma_Buffer ( char* buffer, size_t bufferSize, const char* fmt, ... ) ;
unsigned forma_BufferArgList ( char* buffer, size_t bufferSize, const char* fmt, va_list ap ) ;
```

### Output: function
```C
unsigned forma_Output ( forma_OutputChar_t* outputChar, void* outputCharContext, const char* fmt, ... ) ;
unsigned forma_OutputArgList ( forma_OutputChar_t* outputChar, void* outputCharContext, const char* fmt, va_list ap ) ;
```

### Output: forma object
```C
void forma_Object ( forma_object_t* object, const char* fmt, ... ) ;
void forma_ObjectArgList ( forma_object_t* object, const char* fmt, va_list ap ) ;
```

### Counting
```C
unsigned forma_Count ( const char* fmt, ... ) ;
unsigned forma_CountArgList ( const char* fmt, va_list ap ) ;
```
 
## Format

The overall syntax of a conversion specification is:

```
%[flags][width][.precision][length modifier]conversion
```

### Flags

| Flag | Description |
|------|-------------|
| ? | expression value, can be used to build SQL strings<br /> Example:<br />* forma_Print( "%'?E", 0, "hello" ) => "NULL"<br />* forma_Print( "%'?E", 1, "hello" ) => "'hello'" |
| - | align output to left |
| ~ | tilde operator has different effects on conversions:<br />* 'I': IPv4 number will be swapped<br />* 'P', 'B': enables multi part support<br />* 'h', 'H', 'D': adds offset to output<br />* 'c', 'C': repeats char |
| ' | set quote to ' and enables artifical quotes (can be used for SQL strings) |
| " | set quote to " and enables artifical quotes (can be used for SQL strings) |
| = | loads next char in format as filler |
| | | enables char mapping function |
| _ | start pattern definition |
| : | sets column separator to : |
| # | adds prefix to hex number output |

### Length modifiers

| Length modifiers | Description |
|------------------|-------------|
| l                | 32 bit number |
| ll               | 64 bit number |
| z                | bit size of "size_t" |

### Conversions

| Conversion | Description |
|------------|-------------|
| s          | string |
| E          | string: SQL |
| J          | string: JSON |
| h, H       | HEX array |
| D          | HEX dump |
| x, X       | HEX number |
| p          | pointer |
| o          | octet number |
| d, i       | signed number |
| u          | unsigned number |
| c          | char |
| C          | char array |
| B          | base64 |
| P          | base64 with PEM framing |
| I          | IP (at the moment only IPv4) |
| T          | time |
| F          | function |

Floating point is not yet supported

## Configuration

### Flag: pattern support
```C
#define FORMA_SUPPORT_FLAG_PATTERN_ENABLED            1
```

### Flag: tilde support
```C
#define FORMA_SUPPORT_FLAG_TILDE_ENABLED              1
```

### Flag: char mapping support
```C
#define FORMA_SUPPORT_FLAG_MAPCHAR_ENABLED            1
```

### Flag: artificial support
```C
#define FORMA_SUPPORT_FLAG_ARTIFICIAL_QUOTE_ENABLED   1
```

### Width and precision support
```C
#define FORMA_SUPPORT_WIDTH_AND_PRECISION_ENABLED     1
```

### Length modifier support
```C
#define FORMA_SUPPOORT_LENGTH_MODIFIER_ENABLED        1
```

### Conversion: base64 support
```C
#define FORMA_SUPPORT_CONV_BASE64_ENABLED             1
```

### Conversion: char support
```C
#define FORMA_SUPPORT_CONV_CHAR_ENABLED               1
```

### Conversion: float support
```C
#define FORMA_SUPPORT_CONV_FLOAT_ENABLED              1
```

### Conversion: function support
```C
#define FORMA_SUPPORT_CONV_FUNCTION_ENABLED           1
```

### Conversion: HEX support
```C
#define FORMA_SUPPORT_CONV_HEX_ENABLED                1
```

### Conversion: integer support
```C
#define FORMA_SUPPORT_CONV_INT_ENABLED                1
```

### Flag: network support
```C
#define FORMA_SUPPORT_CONV_NET_ENABLED                1
```

### Conversion: string support
```C
#define FORMA_SUPPORT_CONV_STR_ENABLED                1
#define FORMA_SUPPORT_CONV_STR_SQL_ENABLED            1
#define FORMA_SUPPORT_CONV_STR_JSON_ENABLED           1
```

### Conversion: time support
```C
#define FORMA_SUPPORT_CONV_TIME_ENABLED               1
```

### Parameters
```C
#define FORMA_PRINT_BUFFER_SIZE                       128
```

```C
#define FORMA_NEW_LINE                                "\r\n"
```

```C
#define FORMA_TIME_DEFAULT_PATTERN                    "Y-M-D h:m:s"
```

```C
#define FORMA_CONSOLE_USE_WRITE                       0
```