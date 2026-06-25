# MessageChunk and Sequence Header Fixtures

These fixtures validate OPC UA UASC message chunking and sequence headers.

## OPC References
- OPC-10000-6 6.7.2 (Message Chunk Header)
- OPC-10000-6 6.7.2.4 (Sequence Header)

## Generation Script

The following Python sidecar generates `msg_chunk.bin`:

```python
import struct

# MSG F chunk
# MessageHeader (12 bytes)
#   MessageType (3 bytes): b'MSG'
#   ChunkType (1 byte): b'F'
#   MessageSize (4 bytes): 24
#   SecureChannelId (4 bytes): 1
# SymmetricSecurityHeader (4 bytes)
#   TokenId (4 bytes): 1
# SequenceHeader (8 bytes)
#   SequenceNumber (4 bytes): 1
#   RequestId (4 bytes): 1

msg_size = 12 + 4 + 8
msg_chunk = struct.pack(
    "<3scIIIII",
    b"MSG", b"F",
    msg_size,
    1,  # SecureChannelId
    1,  # TokenId
    1,  # SequenceNumber
    1   # RequestId
)

with open("msg_chunk.bin", "wb") as f:
    f.write(msg_chunk)
```
