# ASTRAEON AEP CODING STANDARD

## Module Rules

- One module = one responsibility.
- Public API must be declared in a matching header.
- Implementation must stay in the matching .c file.
- Do not create mixed-purpose files.

## Naming

- Files: astra_name.c / astra_name.h
- Types: AstraName
- Enums: AstraNameValue
- Functions: astra_name_action()

## Memory

- Check every allocation.
- Free what you allocate.
- Prefer fixed buffers with bounds checks.
- Never write past buffer limits.

## Errors

- Return explicit error enum where practical.
- Do not silently ignore failures.
- Log recoverable failures through astra_logger.

## Hardware Access

- App UI must not call HAL directly.
- App UI must not call storage_file_* directly.
- Use astra_uart for serial.
- Use astra_storage for files.
- Use astra_logger for logs.
- Use astra_event for cross-module communication.

## SAFE PATCH

- Inspect existing symbols before import/use.
- Backup before modifying existing files.
- Build verify after every controlled patch.
- Commit after verified checkpoint.
