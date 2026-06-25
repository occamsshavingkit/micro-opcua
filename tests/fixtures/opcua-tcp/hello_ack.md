# Hello / Acknowledge Fixtures

These fixtures validate OPC UA TCP Connection Protocol messages.

## OPC References
- OPC-10000-6 7.1.2.3 (Hello Message)
- OPC-10000-6 7.1.2.4 (Acknowledge Message)

## Generation Script

The following Python sidecar generates `hello.bin` and `ack.bin`:

```python
import struct

# HELLO (HELF)
endpoint = b"opc.tcp://localhost:4840"
msg_size = 8 + 4 + 4 + 4 + 4 + 4 + 4 + len(endpoint)

hello_data = struct.pack(
    "<3scIIIIIIi",
    b"HEL", b"F",
    msg_size,
    0,      # Version
    65536,  # RcvBuf
    65536,  # SndBuf
    2097152,# MaxMsg
    64,     # MaxChunk
    len(endpoint)
) + endpoint

with open("hello.bin", "wb") as f:
    f.write(hello_data)

# ACKNOWLEDGE (ACKF)
ack_size = 8 + 4 + 4 + 4 + 4 + 4
ack_data = struct.pack(
    "<3scIIIIII",
    b"ACK", b"F",
    ack_size,
    0,      # Version
    65536,  # RcvBuf
    65536,  # SndBuf
    2097152,# MaxMsg
    64      # MaxChunk
)

with open("ack.bin", "wb") as f:
    f.write(ack_data)
```
