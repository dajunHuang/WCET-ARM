Maximize

68 dSta_0 + 35 d0_14 + 63 d1_2.v0 + 63 d1_2.v1 + 35 d1_1.v0 + 35 d1_1.v1 + 37 d2_9 + 2 d3_4.v0 + 2 d3_4.v1 + 5 d3_6.v0 + 5 d3_6.v1 + 74 d4_8.v0 + 43 d4_8.v1 + 5 d5_6.v00 + 5 d5_6.v01 + 5 d5_6.v10 + 5 d5_6.v11 + 74 d5_5.v00 + 43 d5_5.v01 + 43 d5_5.v10 + 43 d5_5.v11 + 31 d6_7.v0 + 31 d6_7.v1 + 37 d6_3.v0 + 7 d6_3.v1 + 74 d8_5.v0 + 43 d8_5.v1 + 5 d8_6.v0 + 5 d8_6.v1 + 2 d9_10 + 34 d9_12 + 104 d10_13 + 34 d11_12.v0 + 34 d11_12.v1 + 74 d11_11.v0 + 43 d11_11.v1 + 37 d12_3 + 31 d12_7 + 74 d13_11 + 34 d13_12 + 35 d14_1 + 63 d14_2


Subject to


\ === tcfg constraints ===
dSta_0 = 1
b0 - d0_14 = 0
b0 - dSta_0 = 0
b1 - d1_2 - d1_1 = 0
b1 - d1_1 - d14_1 = 0
b2 - d2_9 = 0
b2 - d1_2 - d14_2 = 0
b3 - d3_4 - d3_6 = 0
b3 - d6_3 - d12_3 = 0
b4 - d4_8 = 0
b4 - d3_4 = 0
b5 - d5_6 - d5_5 = 0
b5 - d5_5 - d8_5 = 0
b6 - d6_7 - d6_3 = 0
b6 - d3_6 - d5_6 - d8_6 = 0
b7 - d6_7 - d12_7 = 0
b8 - d8_5 - d8_6 = 0
b8 - d4_8 = 0
b9 - d9_10 - d9_12 = 0
b9 - d2_9 = 0
b10 - d10_13 = 0
b10 - d9_10 = 0
b11 - d11_12 - d11_11 = 0
b11 - d11_11 - d13_11 = 0
b12 - d12_3 - d12_7 = 0
b12 - d9_12 - d11_12 - d13_12 = 0
b13 - d13_11 - d13_12 = 0
b13 - d10_13 = 0
b14 - d14_1 - d14_2 = 0
b14 - d0_14 = 0


\ === context constraints ===

\(a)
d1_2 - d1_2.v0 - d1_2.v1 = 0
d1_1 - d1_1.v0 - d1_1.v1 = 0
d3_4 - d3_4.v0 - d3_4.v1 = 0
d3_6 - d3_6.v0 - d3_6.v1 = 0
d4_8 - d4_8.v0 - d4_8.v1 = 0
d5_6 - d5_6.v00 - d5_6.v01 - d5_6.v10 - d5_6.v11 = 0
d5_5 - d5_5.v00 - d5_5.v01 - d5_5.v10 - d5_5.v11 = 0
d6_7 - d6_7.v0 - d6_7.v1 = 0
d6_3 - d6_3.v0 - d6_3.v1 = 0
d8_5 - d8_5.v0 - d8_5.v1 = 0
d8_6 - d8_6.v0 - d8_6.v1 = 0
d11_12 - d11_12.v0 - d11_12.v1 = 0
d11_11 - d11_11.v0 - d11_11.v1 = 0

\(b)
b1.v0 - d1_2.v0 - d1_1.v0 = 0
b1.v1 - d1_2.v1 - d1_1.v1 = 0
b3.v0 - d3_4.v0 - d3_6.v0 = 0
b3.v1 - d3_4.v1 - d3_6.v1 = 0
b4.v0 - d4_8.v0 = 0
b4.v1 - d4_8.v1 = 0
b5.v00 - d5_6.v00 - d5_5.v00 = 0
b5.v01 - d5_6.v01 - d5_5.v01 = 0
b5.v10 - d5_6.v10 - d5_5.v10 = 0
b5.v11 - d5_6.v11 - d5_5.v11 = 0
b6.v0 - d6_7.v0 - d6_3.v0 = 0
b6.v1 - d6_7.v1 - d6_3.v1 = 0
b8.v0 - d8_5.v0 - d8_6.v0 = 0
b8.v1 - d8_5.v1 - d8_6.v1 = 0
b11.v0 - d11_12.v0 - d11_11.v0 = 0
b11.v1 - d11_12.v1 - d11_11.v1 = 0

\(c)
b1 - b1.v0 - b1.v1 = 0
b3 - b3.v0 - b3.v1 = 0
b4 - b4.v0 - b4.v1 = 0
b5 - b5.v00 - b5.v01 - b5.v10 - b5.v11 = 0
b6 - b6.v0 - b6.v1 = 0
b8 - b8.v0 - b8.v1 = 0
b11 - b11.v0 - b11.v1 = 0

\(d)
b1.v0 - d14_1 = 0
b5.v00 - d8_5.v0 = 0
b5.v10 - d8_5.v1 = 0
b3.v0 - d12_3 = 0
b11.v0 - d13_11 = 0

\(e)
b1.v1 - d1_1.v0 - d1_1.v1 = 0
b1.v1 - 1022 d1_1.v0 <= 0
b5.v01 - d5_5.v00 - d5_5.v01 = 0
b5.v01 - 510 d5_5.v00 <= 0
b5.v11 - d5_5.v10 - d5_5.v11 = 0
b5.v11 - 510 d5_5.v10 <= 0
b3.v1 - d6_3.v0 - d6_3.v1 = 0
b3.v1 - 1022 d6_3.v0 <= 0
b11.v1 - d11_11.v0 - d11_11.v1 = 0
b11.v1 - 510 d11_11.v0 <= 0

\(f)
d1_2 - d1_2.v0 - d1_2.v1 = 0
d5_6.v0 - d5_6.v00 - d5_6.v01 = 0
d5_6.v1 - d5_6.v10 - d5_6.v11 = 0
d6_7 - d6_7.v0 - d6_7.v1 = 0
d11_12 - d11_12.v0 - d11_12.v1 = 0

\(g)
b4.v0 - d3_4.v0 = 0
b4.v1 - d3_4.v1 = 0
b6.v0 - d3_6.v0 - d5_6.v0 - d8_6.v0 = 0
b6.v1 - d3_6.v1 - d5_6.v1 - d8_6.v1 = 0
b8.v0 - d4_8.v0 = 0
b8.v1 - d4_8.v1 = 0

\ === header constraints ===
b0 <= 1
b1 - 1023 b0 <= 0
b1 - 1023 d14_1 <= 0
b5 - 511 b3 <= 0
b5 - 511 d8_5 <= 0
b3 - 1023 b0 <= 0
b3 - 1023 d12_3 <= 0
b11 - 511 b0 <= 0
b11 - 511 d13_11 <= 0

Generals

Sta
b0
d0_14
b1
d1_2
d1_1
b2
d2_9
b3
d3_4
d3_6
b4
d4_8
b5
d5_6
d5_5
b6
d6_7
d6_3
b7
b8
d8_5
d8_6
b9
d9_10
d9_12
b10
d10_13
b11
d11_12
d11_11
b12
d12_3
d12_7
b13
d13_11
d13_12
b14
d14_1
d14_2
b1.d0
b3.d0
b3.d1
b5.d0
b5.d1
b5.d2
b5.d3
b5.d4
b5.d5
b8.d0
b8.d1
b8.d2
b8.d3
b8.d4
b8.d5
b9.d0
b9.d1
b11.d0
b11.d1
b11.d2
b11.d3
b11.d4
b11.d5
b13.d0
b13.d1
b13.d2
b13.d3
b13.d4
b13.d5
b14.d0
d0_14
d1_2.v0
d1_2.v1
d1_1.v0
d1_1.v1
d2_9
d3_4.v0
d3_4.v1
d3_6.v0
d3_6.v1
d4_8.v0
d4_8.v1
d5_6.v00
d5_6.v01
d5_6.v10
d5_6.v11
d5_5.v00
d5_5.v01
d5_5.v10
d5_5.v11
d6_7.v0
d6_7.v1
d6_3.v0
d6_3.v1
d8_5.v0
d8_5.v1
d8_6.v0
d8_6.v1
d9_10
d9_12
d10_13
d11_12.v0
d11_12.v1
d11_11.v0
d11_11.v1
d12_3
d12_7
d13_11
d13_12
d14_1
d14_2

End
\ total_cons: 100 	total_vars: 70
