# OPL Parameters

| NRPN | Hex    | Parameter                              | Range |
| ---- | ------ | -------------------------------------- | ----- |
| 0    | 0x0000 | 4-op mode                              | 0-1   |
| 1    | 0x0001 | Feedback                               | 0-7   |
| 2    | 0x0002 | Connection                             | 0-3   |
| 16   | 0x0010 | Operator 1 enable amplitude modulation | 0-1   |
| 17   | 0x0011 | Operator 1 enable vibrato              | 0-1   |
| 18   | 0x0012 | Operator 1 envelope type               | 0-1   |
| 19   | 0x0013 | Operator 1 key scale rate              | 0-1   |
| 20   | 0x0014 | Operator 1 frequency multiplier        | 0-15  |
| 21   | 0x0015 | Operator 1 key scale level             | 0-3   |
| 22   | 0x0016 | Operator 1 total level                 | 0-63  |
| 23   | 0x0017 | Operator 1 attack rate                 | 0-15  |
| 24   | 0x0018 | Operator 1 decay rate                  | 0-15  |
| 25   | 0x0019 | Operator 1 sustain level               | 0-15  |
| 26   | 0x001a | Operator 1 release rate                | 0-15  |
| 27   | 0x001b | Operator 1 wave shape                  | 0-7   |
| 32   | 0x0020 | Operator 2 enable amplitude modulation | 0-1   |
| 33   | 0x0021 | Operator 2 enable vibrato              | 0-1   |
| 34   | 0x0022 | Operator 2 envelope type               | 0-1   |
| 35   | 0x0023 | Operator 2 key scale rate              | 0-1   |
| 36   | 0x0024 | Operator 2 frequency multiplier        | 0-15  |
| 37   | 0x0025 | Operator 2 key scale level             | 0-3   |
| 38   | 0x0026 | Operator 2 total level                 | 0-63  |
| 39   | 0x0027 | Operator 2 attack rate                 | 0-15  |
| 40   | 0x0028 | Operator 2 decay rate                  | 0-15  |
| 41   | 0x0029 | Operator 2 sustain level               | 0-15  |
| 42   | 0x002a | Operator 2 release rate                | 0-15  |
| 43   | 0x002b | Operator 2 wave shape                  | 0-7   |
| 48   | 0x0030 | Operator 3 enable amplitude modulation | 0-1   |
| 49   | 0x0031 | Operator 3 enable vibrato              | 0-1   |
| 50   | 0x0032 | Operator 3 envelope type               | 0-1   |
| 51   | 0x0033 | Operator 3 key scale rate              | 0-1   |
| 52   | 0x0034 | Operator 3 frequency multiplier        | 0-15  |
| 53   | 0x0035 | Operator 3 key scale level             | 0-3   |
| 54   | 0x0036 | Operator 3 total level                 | 0-63  |
| 55   | 0x0037 | Operator 3 attack rate                 | 0-15  |
| 56   | 0x0038 | Operator 3 decay rate                  | 0-15  |
| 57   | 0x0039 | Operator 3 sustain level               | 0-15  |
| 58   | 0x003a | Operator 3 release rate                | 0-15  |
| 59   | 0x003b | Operator 3 wave shape                  | 0-7   |
| 64   | 0x0040 | Operator 4 enable amplitude modulation | 0-1   |
| 65   | 0x0041 | Operator 4 enable vibrato              | 0-1   |
| 66   | 0x0042 | Operator 4 envelope type               | 0-1   |
| 67   | 0x0043 | Operator 4 key scale rate              | 0-1   |
| 68   | 0x0044 | Operator 4 frequency multiplier        | 0-15  |
| 69   | 0x0045 | Operator 4 key scale level             | 0-3   |
| 70   | 0x0046 | Operator 4 total level                 | 0-63  |
| 71   | 0x0047 | Operator 4 attack rate                 | 0-15  |
| 72   | 0x0048 | Operator 4 decay rate                  | 0-15  |
| 73   | 0x0049 | Operator 4 sustain level               | 0-15  |
| 74   | 0x004a | Operator 4 release rate                | 0-15  |
| 75   | 0x004b | Operator 4 wave shape                  | 0-7   |

# Universal Parameters

| NRPN  | Hex    | Parameter               | Range   |
| ----- | ------ | ----------------------- | ------- |
| 8192  | 0x2000 | Polyphony mode          | 0-4     |
| 8193  | 0x2001 | Fixed note number       | 0-127   |
| 8194  | 0x2002 | Pitch offset            | 0-16383 |
| 8195  | 0x2003 | Glide duration (ms)     | 0-16383 |
| 8196  | 0x2004 | Velocity depth          | 0-127   |
| 12288 | 0x3000 | Map 1 source            | 0-255   |
| 12289 | 0x3001 | Map 1 destination       | 0-16383 |
| 12290 | 0x3002 | Map 1 adjustment amount | 0-16383 |
| 12304 | 0x3010 | Map 2 source            | 0-255   |
| 12305 | 0x3011 | Map 2 destination       | 0-16383 |
| 12306 | 0x3012 | Map 2 adjustment amount | 0-16383 |
| 12320 | 0x3020 | Map 3 source            | 0-255   |
| 12321 | 0x3021 | Map 3 destination       | 0-16383 |
| 12322 | 0x3022 | Map 3 adjustment amount | 0-16383 |
| 12336 | 0x3030 | Map 4 source            | 0-255   |
| 12337 | 0x3031 | Map 4 destination       | 0-16383 |
| 12338 | 0x3032 | Map 4 adjustment amount | 0-16383 |
| 12544 | 0x3100 | LFO 1 wave              | 0-4     |
| 12545 | 0x3101 | LFO 1 period (ms)       | 0-16383 |
| 12546 | 0x3102 | LFO 1 sync              | 0-1     |
| 12547 | 0x3103 | LFO 1 one shot          | 0-1     |
| 12560 | 0x3110 | LFO 2 wave              | 0-4     |
| 12561 | 0x3111 | LFO 2 period (ms)       | 0-16383 |
| 12562 | 0x3112 | LFO 2 sync              | 0-1     |
| 12563 | 0x3113 | LFO 2 one shot          | 0-1     |
