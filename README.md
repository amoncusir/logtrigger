# logtrigger #

Reads the logs (using the ubus service) and execute scripts using trigger configurations.

## Usage:

`logtrigger -h`
```text
Usage: logtrigger [options]
Options:
    [-s] <socket>                Socket to read
     -t  <type> <value> <path>   Match configuration

Match Configuration:
    Types:
      0 - all: match with all log events
      1 - regex: Regular expression for match
    Usage:
      All: ... -t all '' '/mi/abs/script.sh'
      All: ... -t regex '^cool ?regex$' '/mi/abs/script.sh'
```

## Why?

Low resource program for OpenWRT OS for low hardware specs and realtime execution.

An easy way to run scripts triggered by logs.

## Example of usage

### Notify when HTTP package is send to host

Using NFTables:
```text
chain input {
    ip daddr 192.168.1.128 tcp dport { 80, 443 } log level alert prefix NOTIFY 
}
```

Then you can react from this event, like:

**Script: http_logs.sh**
```shell
echo "Http package" >> /var/logs/http_package.log
```

**And add trigger:**
```text
$ logtrigger -t regex '*.NOTIFY*.' /usr/share/logtrigger/http_logs.sh
```
