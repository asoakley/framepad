# Phase 2 --- USB CDC and Protocol

## Objective

Establish reliable bidirectional USB CDC communication between the Pico
and a host serial terminal/test program. Do not yet depend on the full
WPF application.

## Requirements

-   Use TinyUSB CDC through the Pico SDK integration already present or
    a compatible, pinned dependency.
-   Enumerate as a USB CDC virtual COM port.
-   Define a versioned framed protocol.
-   Handle partial reads, multiple frames per read, malformed frames,
    and bounded buffers.
-   USB callbacks only collect bytes or enqueue data. Parsing and
    application actions happen in the main loop.
-   Never assume CDC read/write calls map one-to-one to application
    messages.

## Initial frame format

Use a documented little-endian frame:

  ------------------------------------------------------------------------
  Field                                         Size Description
  --------------------- ---------------------------- ---------------------
  Magic                                      4 bytes Fixed identifying
                                                     bytes, define in
                                                     protocol document

  Protocol version                            1 byte Initial value 1

  Message type                                1 byte Enumerated message ID

  Sequence                                   2 bytes Little-endian
                                                     request/response
                                                     correlation

  Payload length                             4 bytes Little-endian byte
                                                     count

  Payload                                   variable JSON UTF-8 for
                                                     control messages;
                                                     binary for file data

  CRC32                                      4 bytes Define exact coverage
                                                     and CRC variant
  ------------------------------------------------------------------------

The exact magic value, CRC variant, maximum payload, and message IDs
must be written into `protocol/PROTOCOL.md` before implementation.
Choose and document conservative limits suitable for the Pico. Reject
oversized payloads before copying them into buffers.

CRC coverage must be unambiguous. A recommended rule is CRC32 over the
header fields after magic through the final payload byte, excluding the
CRC field. Use the same standard CRC32 variant and test vectors on both
sides.

## Initial message set

  Message       Direction   Purpose
  ------------- ----------- -----------------------------------
  HELLO         Pico → PC   Device announces itself
  HELLO_ACK     PC → Pico   Host acknowledges
  DEVICE_INFO   Pico → PC   Firmware/protocol/capability info
  STATUS        Either      Connection and feature status
  MACRO_PRESS   Pico → PC   Macro button ID pressed
  ACK           Either      Acknowledge request
  ERROR         Either      Error code and optional detail
  PING/PONG     Either      Optional liveness check

Define required payload fields and error behavior in the protocol
specification.

## Handshake

1.  Pico sends HELLO when CDC is ready.
2.  Host validates protocol version and replies HELLO_ACK.
3.  Pico sends DEVICE_INFO.
4.  Both sides enter connected state.
5.  On disconnect or timeout, Pico returns to standalone mode.

Avoid a tight reconnect or heartbeat loop. Use bounded retry intervals.

## Tasks

1.  Pin and document TinyUSB/Pico SDK integration.
2.  Implement USB CDC initialization and polling/task service.
3.  Implement frame encoder and incremental decoder.
4.  Add CRC32 implementation and shared test vectors.
5.  Implement message dispatch in main-loop context.
6.  Implement HELLO/HELLO_ACK/DEVICE_INFO.
7.  Implement MACRO_PRESS generated from a physical/touch macro button.
8.  Build a host-side serial test harness or documented terminal test.
9.  Add protocol tests for fragmentation, concatenated frames, bad CRC,
    unknown type, and oversized payload.

## Acceptance criteria

-   Pico reliably enumerates as CDC.
-   Host and Pico complete handshake.
-   A macro press is received with the correct ID.
-   Malformed data does not crash or wedge firmware.
-   USB disconnect returns the device to standalone behavior.
-   Protocol document and test vectors are checked in.
