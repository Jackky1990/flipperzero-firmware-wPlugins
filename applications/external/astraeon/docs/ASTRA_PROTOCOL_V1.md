# ASTRA PROTOCOL V1

## Transport

Initial transport: UART / USB Serial using JSONL.
Each message is one JSON object followed by newline.

## Message Types

- PING
- PONG
- STATUS
- EVENT
- COMMAND
- ACK
- ERROR
- HEARTBEAT
- MISSION_START
- MISSION_COMPLETE
- SYNC_REQUEST
- SYNC_RESPONSE

## Error Codes

- ERR_PROTOCOL
- ERR_TIMEOUT
- ERR_STORAGE
- ERR_POLICY
- ERR_PERMISSION
- ERR_DEVICE
- ERR_UNKNOWN

## Safety

Unknown access signals are log-only.
Replay requires explicit authorized policy.
BadUSB commands must target owned devices only.
