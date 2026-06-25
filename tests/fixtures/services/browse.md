# Browse Services Fixtures

These fixtures validate OPC UA UASC Browse requests.

## OPC References
- OPC-10000-4 5.9.2.2 (Browse)
- OPC-10000-4 7.29 (ReferenceDescription)

## Generation Script

The following Python sidecar generates `browse_req.bin`:

```python
import struct

# Browse Request Body
body_browse = struct.pack("<BBH", 0x01, 0x00, 527)

msg_size_browse = 12 + 4 + 8 + len(body_browse)
msg_chunk_browse = struct.pack(
    "<3scIIIII",
    b"MSG", b"F",
    msg_size_browse,
    1,  # SecureChannelId
    1,  # TokenId
    6,  # SequenceNumber
    6   # RequestId
) + body_browse

with open("browse_req.bin", "wb") as f:
    f.write(msg_chunk_browse)
```
