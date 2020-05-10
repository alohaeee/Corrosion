// Integer

123;                               // type i32
123i32;                            // type i32
123u32;                            // type u32
123_u32;                           // type u32
let a: u64 = 123;                  // type u64

0xff;                              // type i32
0xff_u8;                           // type u8

0o70;                              // type i32
0o70_i16;                          // type i16

0b1111_1111_1001_0000;             // type i32
0b1111_1111_1001_0000i64;          // type i64
0b________1;                       // type i32

0usize;                            // type usize

// invalid suffixes

0invalidSuffix;

// uses numbers of the wrong base

123AFB43;
0b0102;
0o0581;

// integers too big for their type (they overflow)

128_i8;
256_u8;

// bin, hex, and octal literals must have at least one digit

0b_;
0b____;

