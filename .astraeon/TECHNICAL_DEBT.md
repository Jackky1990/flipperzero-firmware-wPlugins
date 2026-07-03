# ASTRAEON Technical Debt

## Future Refactors

- Consolidate registry documents into machine-readable YAML/JSON after the
  architecture stabilizes.
- Convert portal/service/device registries into source-backed inventory.
- Create cross-repository compatibility checks when future repositories exist.
- Promote validation recipes from documentation to executable definitions.

## Legacy APIs

- Track legacy or target-specific APIs before exposing them as portable
  ASTRAEON SDK contracts.
- Do not publish public SDK APIs until ownership, errors, sessions, and
  verification are defined.

## Temporary Workarounds

- UART/GPIO hardware validation remains blocked by known-good hardware
  availability.
- Current portal URLs are partially unknown and must not be assumed.
- Current service registry includes concepts that require live infrastructure
  verification before production claims.

## Deprecated Ideas

- Do not treat Probe as only a UART bridge.
- Do not treat RogueMaster as the full ASTRAEON universe.
- Do not allow AI, Vision, or Knowledge to execute hardware actions directly.
