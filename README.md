# xaway

An X11 away-from-keyboard command launcher daemon

## Usage
```
Usage: xaway [OPTIONS] idle_script [active_script]
Set up a daemon watching for X11 inactivity, executing commands when user is
inactive/back.

Parameter 'idle_script' is mandatory. This is the script to execute when idle
timer occurs.

Parameter 'active_script' is the script to execute when going out of idle.

Options:
  -h, --help          Print this help and exit.
  -t, --timeout=N     Set idle timer to N (in seconds, default: 60).
  -i, --interval=N    Set inactivity checks interval to N (in milliseconds,
                      default: 500).
```

## Example

A typical example use-case is when sided with razer-daemon. One may choose to
script driver settings in order to change LED effect when the user is away from
keyboard, before setting them back when user is back here.

## Notes

Be sure the author is aware of xidle. Of course, he wasn't while quickly writing
this. Additionnally, at first glance, xidle doesn't seem to feature out-of-idle
program execution.
