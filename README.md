# DAQ-Middleware DAQコンポーネント間レイテンシの測定

## ディレクトリ構成

```
/home/daq/Latency/LatencySender
/home/daq/Latency/LatencyLogger
/home/daq/Latency/latency-sender-logger.xml
```

## config.xml

latency-sender-logger.xml中のパラメータ

```
<params>
    <param pid="bufsize_kb">128</param>
</params>
```

でLatencySenderCompが送るデータサイズを決める。

## LatencySender

bufsize_kb*1024のデータを後段LatencyLoggerに送る。
送るデータのデータフォーマットは以下のとおり:

```
63                        0
+-------------------------+
|     bufsize_kb          | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
```

struct timespecは送る直前にclock_gettime()で時刻情報を
埋め込む。

## LatencyLogger

LatencyLoggerでは受け取ったデータの24バイト名（0オリジン)
に受け取った時刻をstruct timespecで埋める。

```
63                        0
+-------------------------+
|     bufsize_kb          | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
```

合計40バイトをディスクに記録する。
記録されるデータは次のようになる:

```
63                        0
+-------------------------+
|     bufsize_kb          | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
|     bufsize_kb          | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
|     bufsize_kb          | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
| struct timespec tv_sec  | (8バイト)
+-------------------------+
| struct timespec tv_nsec | (8バイト)
+-------------------------+
(以下送った回数分続く)
```
