# libhenlein: two file cross-platform C timer

[![Build Status](https://travis-ci.org/raedwulf/henlein.svg?branch=master)](https://travis-ci.org/raedwulf/henlein)

## Example

```C
uint64_t start = henlein_now();
/* do stuff */
uint64_t stop = henlein_now();
uint64_t diff = henlein_diff(stop, start);
printf("%llu nanoseconds elapsed\n", diff);
```

## License

henlein is licensed under the MIT license.
