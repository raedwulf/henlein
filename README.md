# henlein: two file cross-platform C timer

[![Build Status](https://travis-ci.com/raedwulf/henlein.svg?branch=master)](https://travis-ci.com/raedwulf/henlein)

## Example

```C
/* initialise the library */
int rc = henlein_init();
assert(!rc); /* rc should return 0 if successful */
/* now time something */
uint64_t start = henlein_now();
/* do stuff */
uint64_t stop = henlein_now();
uint64_t diff = henlein_diff(stop, start);
printf("%llu nanoseconds elapsed\n", diff);
```

## License

henlein is licensed under the [MIT license](LICENSE).
The contributing authors which collectively hold copyright are listed in [AUTHORS](AUTHORS).

