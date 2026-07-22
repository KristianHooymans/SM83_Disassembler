# gb-disasm

A disassembler for the Sharp SM83, the CPU in the original Game Boy. Takes a `.gb` ROM,
verifies the cartridge header, and prints a linear disassembly.

C++23, standard library only.

## Build

```
make
```

Override the compiler if needed: `make CXX=g++-14`.

## Usage

```
./sm tests/cpu_instrs.gb > out.txt
```

A 64 KiB ROM produces around 57,000 lines, so redirect to a file.

## Output

```
CPU_INSTRS
64 KiB
MBC1
HEADER CHECKSUM PASS

0200: LD B, A
0201: LD DE, $C000
0204: LD C, $10
0206: LD A, (HL+)
0207: LD (DE), A
0208: INC E
0209: JR NZ, $0206
```

Immediates print in hex, 16-bit operands are assembled little-endian, and relative jumps
resolve to their target address.

## Coverage

All 245 legal unprefixed opcodes and the full 256-entry `CB` page. The 11 illegal opcodes
(`D3 DB DD E3 E4 EB EC ED F4 FC FD`) print as `DB $XX` and the walk advances one byte, so
unexpected input never desyncs the output or crashes.

## Limitations

- Linear sweep can't tell code from data, so the Nintendo logo and tile data disassemble
  as nonsense. Fixing that means following control flow, i.e. an emulator.
- `ADD SP,e8` and `LD HL,SP+e8` print their signed operand unsigned.
- No labels or symbols; jump targets are bare addresses.

## Credits

Test ROMs by [blargg](https://github.com/retrio/gb-test-roms).
Opcode data from [gbdev/gb-opcodes](https://github.com/gbdev/gb-opcodes).
Docs: [Pan Docs](https://gbdev.io/pandocs), [gbz80(7)](https://rgbds.gbdev.io/docs/gbz80.7).
