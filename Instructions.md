<h1 align="center">4-Bit CPU Design Instructions</h1>

## Table of Contents

1. [Arithmetic and Logic Components](#part-1--arithmetic-and-logic-components)
2. [Flags](#part-2--flags)
3. [Storage Components](#part-3--storage-components)
4. [Control Components](#part-4--control-components)
5. [CPU](#part-5--cpu)
6. [Mapping](#part-6--mapping)
7. [Real Test](#part-7--real-test)

---

## Part 1 — Arithmetic and Logic Components

### 1. Half Adder

| Item | Details |
| ---- | ------- |
| Inputs | `X` (1-bit), `Y` (1-bit) |
| Outputs | `Sum` (1-bit), `Carry` (1-bit) |
| Built from | 1 XOR gate (`Sum`) + 1 AND gate (`Carry`) |

**Truth table**

| X | Y | Sum | Carry |
| - | - | --- | ----- |
| 0 | 0 | 0 | 0 |
| 0 | 1 | 1 | 0 |
| 1 | 0 | 1 | 0 |
| 1 | 1 | 0 | 1 |

### 2. Full Adder

| Item | Details |
| ---- | ------- |
| Inputs | `X` (1-bit), `Y` (1-bit), `Cin` (1-bit) |
| Outputs | `Sum` (1-bit), `Cout` (1-bit) |

**Truth table**

| X | Y | Cin | Sum | Cout |
| - | - | --- | --- | ---- |
| 0 | 0 | 0 | 0 | 0 |
| 0 | 0 | 1 | 1 | 0 |
| 0 | 1 | 0 | 1 | 0 |
| 0 | 1 | 1 | 0 | 1 |
| 1 | 0 | 0 | 1 | 0 |
| 1 | 0 | 1 | 0 | 1 |
| 1 | 1 | 0 | 0 | 1 |
| 1 | 1 | 1 | 1 | 1 |

### 3. AND Unit

| Item | Details |
| ---- | ------- |
| Inputs | `X` (4-bit), `Y` (4-bit) |
| Outputs | `And Result` (4-bit) |

**Truth table** (per bit; the 4-bit unit applies it to each pair `Xi`, `Yi` for i = 0…3)

| Xi | Yi | And Result i |
| -- | -- | ------------ |
| 0 | 0 | 0 |
| 0 | 1 | 0 |
| 1 | 0 | 0 |
| 1 | 1 | 1 |

### 4. OR Unit

| Item | Details |
| ---- | ------- |
| Inputs | `X` (4-bit), `Y` (4-bit) |
| Outputs | `Or Result` (4-bit) |

**Truth table** (per bit; the 4-bit unit applies it to each pair `Xi`, `Yi` for i = 0…3)

| Xi | Yi | Or Result i |
| -- | -- | ----------- |
| 0 | 0 | 0 |
| 0 | 1 | 1 |
| 1 | 0 | 1 |
| 1 | 1 | 1 |

### 5. XOR Unit

| Item | Details |
| ---- | ------- |
| Inputs | `X` (4-bit), `Y` (4-bit) |
| Outputs | `Xor Result` (4-bit) |

**Truth table** (per bit; the 4-bit unit applies it to each pair `Xi`, `Yi` for i = 0…3)

| Xi | Yi | Xor Result i |
| -- | -- | ------------ |
| 0 | 0 | 0 |
| 0 | 1 | 1 |
| 1 | 0 | 1 |
| 1 | 1 | 0 |

### 6. Add/Sub

| Item | Details |
| ---- | ------- |
| Inputs | `X` (4-bit), `Y` (4-bit), `Controller` (1-bit) |
| Outputs | `SumDiff` (4-bit), `CarryBorrow` (1-bit) |
| Built from | 4 Full Adders (from component 2) + 1 XOR for the last `Cout` with the controller |

---

## Part 2 — Flags

### 7. Zero Flag

| Item | Details |
| ---- | ------- |
| Inputs | `X` (4-bit) |
| Outputs | `ZF` (1-bit) |

**Truth table**

| `X` | `ZF` |
| --- | ---- |
| `0000` | 1 |
| otherwise | 0 |

### 8. Negative Flag

| Item | Details |
| ---- | ------- |
| Inputs | `X` (4-bit) |
| Outputs | `NF` (1-bit) |

**Truth table**

| `X` | `NF` |
| --- | ---- |
| `0xxx` | 0 |
| `1xxx` | 1 |

### 9. Carry Borrow Flag

| Item | Details |
| ---- | ------- |
| Inputs | `X` (1-bit) |
| Outputs | `CBF` (1-bit) |

**Truth table**

| X | CBF |
| - | --- |
| 0 | 0 |
| 1 | 1 |

---

## Part 3 — Storage Components

### 10. 1-bit Register (controlled buffer register with parallel load)

| Item | Details |
| ---- | ------- |
| Inputs | `LOAD` (1-bit), `D` (1-bit), `CLK` (1-bit), `CLR` (1-bit) |
| Outputs | `Q` (1-bit), `Q Not` (1-bit) |
| Built from | D flip-flop |
| Note | `CLR` restarts the whole circuit |

### 11. 4-bit Register (controlled buffer register with parallel load)

| Item | Details |
| ---- | ------- |
| Inputs | `LOAD` (1-bit), `D` (4-bit), `CLK` (1-bit), `CLR` (1-bit) |
| Outputs | `Q` (4-bit) |
| Built from | 4 × 1-bit Registers |
| Note | `CLR` restarts the whole circuit |

### 12. Register Bank

**Inputs**

| Signal | Width | Description |
| ------ | ----- | ----------- |
| `WE` | 1-bit | Write Enable |
| `WA` | 2-bit | Write Address |
| `CLR` | 1-bit | Clear |
| `CLK` | 1-bit | Clock |
| `D` | 4-bit | Data to write |
| `RA` | 2-bit | Selects which register is read through output `A` |
| `RB` | 2-bit | Selects which register is read through output `B` |

**Outputs**

| Signal | Width | Description |
| ------ | ----- | ----------- |
| `R0` | 4-bit | Reading of register 0 |
| `R1` | 4-bit | Reading of register 1 |
| `R2` | 4-bit | Reading of register 2 |
| `R3` | 4-bit | Reading of register 3 |
| `A` | 4-bit | Output A |
| `B` | 4-bit | Output B |

**Built from:** 4 × 4-bit Registers + 2 MUXs (to select which register is read through outputs `A` and `B`).

---

## Part 4 — Control Components

### 13. 4-bit Synchronous Counter

| Item | Details |
| ---- | ------- |
| Inputs | `EN` (Enable Toggle, 1-bit), `CLK` (1-bit), `CLR` (1-bit) |
| Outputs | `Address` (4-bit) |
| Built from | 4 T flip-flops + 4 AND gates |

### 14. Instruction Memory

| Item | Details |
| ---- | ------- |
| Inputs | `Address` (4-bit) |
| Outputs | `Instruction` (9-bit, represented in hexadecimal, e.g. `0C7`) |

### 15. Instruction Decoder

**Inputs**

| Signal | Width |
| ------ | ----- |
| `Instruction` | 9-bit |

**Outputs**

| Signal | Width |
| ------ | ----- |
| `OpCode` | 3-bit |
| `SrcA` | 2-bit |
| `SrcB` | 2-bit |
| `Dest` | 2-bit |
| `S0` | 1-bit |
| `S1` | 1-bit |
| `Controller` | 1-bit |
| `WE` (= `PC_EN`) | 1-bit |
| `LOAD_EN` | 1-bit |
| `Value` | 4-bit |

**Built from:** a MUX that selects which bits are used as `Dest`:

- Bits 1–0 for ALU instructions (e.g. `ADD R0, R1, R2`)
- Bits 5–4 for `LOAD` instructions (e.g. `LOAD R0, 5`)

---

## Part 5 — CPU

### 16. CPU

**Inputs**

| Signal | Width |
| ------ | ----- |
| `CLK` | 1-bit |
| `CLR` | 1-bit |
| `Initial Value` | 4-bit |
| `Manual Mode` | 1-bit |

**Outputs**

| Signal | Width | Notes |
| ------ | ----- | ----- |
| `R0` | 4-bit | |
| `R1` | 4-bit | |
| `R2` | 4-bit | |
| `R3` | 4-bit | |
| `Counter` | 4-bit | |
| `Instruction` | 9-bit | Represented in hexadecimal, e.g. `0C7` |
| `Result` | 4-bit | |
| `CarryBorrow` | 1-bit | |
| `ZF` | 1-bit | |
| `NF` | 1-bit | |
| `CBF` | 1-bit | |

---

## Part 6 — Mapping

### 17.1 ALU Instruction Format

| Field | OpCode | SrcA | SrcB | Dest |
| ----- | ------ | ---- | ---- | ---- |
| Width | 3 | 2 | 2 | 2 |
| Bits | 8–6 | 5–4 | 3–2 | 1–0 |

### 17.2 LOAD Instruction Format

| Field | OpCode | Dest | Immediate (Value) |
| ----- | ------ | ---- | ----------------- |
| Width | 3 | 2 | 4 |
| Bits | 8–6 | 5–4 | 3–0 |

### 17.3 Register Addresses

| Register | Address |
| -------- | ------- |
| R0 | `00` |
| R1 | `01` |
| R2 | `10` |
| R3 | `11` |

### 17.4 OpCodes

| Instruction | OpCode |
| ----------- | ------ |
| ADD | `000` |
| SUB | `001` |
| AND | `010` |
| OR | `011` |
| XOR | `100` |
| LOAD | `101` |

---

## Part 7 — Real Test

### 18. Test Program

| # | Instruction | Binary | Hex |
| -: | ----------- | ------ | --- |
| 1 | `LOAD R0, 6` | `101000110` | `146` |
| 2 | `LOAD R1, 3` | `101010011` | `153` |
| 3 | `ADD R0, R1, R2` | `000000110` | `006` |
| 4 | `SUB R0, R1, R3` | `001000111` | `087` |
| 5 | `AND R0, R1, R2` | `010000110` | `086` |
| 6 | `OR R0, R1, R3` | `011000111` | `0C7` |
| 7 | `XOR R0, R1, R2` | `100000110` | `106` |

### 19. Expected Register Values After Every Pulse

| Pulse | R0 | R1 | R2 | R3 |
| ----: | ------ | ------ | ------ | ------ |
| Initial | `0000` | `0000` | `0000` | `0000` |
| 1 | `0110` | `0000` | `0000` | `0000` |
| 2 | `0110` | `0011` | `0000` | `0000` |
| 3 | `0110` | `0011` | `1001` | `0000` |
| 4 | `0110` | `0011` | `1001` | `0011` |
| 5 | `0110` | `0011` | `0010` | `0011` |
| 6 | `0110` | `0011` | `0010` | `0111` |
| 7 | `0110` | `0011` | `0101` | `0111` |