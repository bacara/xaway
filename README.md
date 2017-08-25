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

## Notes

Make sure the author is aware of xidle. Well, he wasn't, while quickly writing
this.