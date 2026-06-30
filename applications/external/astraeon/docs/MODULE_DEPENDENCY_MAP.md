# MODULE DEPENDENCY MAP

## Rule

UI must not call hardware directly.
Hardware must not update UI directly.
All cross-module activity must pass through Event or Protocol layer.

## Dependency Flow

companion -> astra_console
companion -> astra_event
companion -> astra_protocol
companion -> astra_logger

astra_console -> astra_event
astra_protocol -> astra_packet
astra_protocol -> astra_event
astra_uart -> astra_protocol
astra_logger -> astra_storage
astra_storage -> Furi Storage API
astra_node01 -> astra_runtime_transport
astra_node01 -> astra_event

## Forbidden

- companion -> furi_hal_serial
- companion -> storage_file_open
- UI -> HAL directly
- hardware callback -> UI directly
