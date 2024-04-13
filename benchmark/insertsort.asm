
results/results_est/insertsort/insertsort:     file format elf32-littlearm


Disassembly of section .text:

00008000 <add1>:
    🚗8000:	e52db004 	push	{fp}		@ (str fp, [sp, #-4]!)
    8004:	e28db000 	add	fp, sp, #0
    8008:	e24dd00c 	sub	sp, sp, #12
    800c:	e50b0008 	str	r0, [fp, #-8]
    8010:	e50b100c 	str	r1, [fp, #-12]
    8014:	e51b2008 	ldr	r2, [fp, #-8]
    8018:	e51b300c 	ldr	r3, [fp, #-12]
    801c:	e0823003 	add	r3, r2, r3
    8020:	e1a00003 	mov	r0, r3
    8024:	e28bd000 	add	sp, fp, #0
    8028:	e49db004 	pop	{fp}		@ (ldr fp, [sp], #4)
    802c:	e12fff1e 	bx	lr

00008030 <_start>:
    0️⃣8030:	e92d4800 	push	{fp, lr}
    8034:	e28db004 	add	fp, sp, #4
    8038:	e24dd018 	sub	sp, sp, #24
    803c:	e3a03000 	mov	r3, #0
    8040:	e50b3008 	str	r3, [fp, #-8]
    8044:	ea00000f 	b	8088 <_start+0x58>
    1️⃣8048:	e51b3008 	ldr	r3, [fp, #-8]
    804c:	e1a03803 	lsl	r3, r3, #16
    8050:	e1a03823 	lsr	r3, r3, #16
    8054:	e2633b01 	rsb	r3, r3, #1024	@ 0x400
    8058:	e1a03803 	lsl	r3, r3, #16
    805c:	e1a03823 	lsr	r3, r3, #16
    8060:	e1a03803 	lsl	r3, r3, #16
    8064:	e1a02843 	asr	r2, r3, #16
    8068:	e59f1128 	ldr	r1, [pc, #296]	@ 8198 <_start+0x168>
    806c:	e51b3008 	ldr	r3, [fp, #-8]
    8070:	e1a03083 	lsl	r3, r3, #1
    8074:	e0813003 	add	r3, r1, r3
    8078:	e1c320b0 	strh	r2, [r3]
    807c:	e51b3008 	ldr	r3, [fp, #-8]
    8080:	e2833001 	add	r3, r3, #1
    8084:	e50b3008 	str	r3, [fp, #-8]
    2️⃣8088:	e51b3008 	ldr	r3, [fp, #-8]
    808c:	e3530b01 	cmp	r3, #1024	@ 0x400
    8090:	baffffec 	blt	8048 <_start+0x18>
    3️⃣8094:	e3a03000 	mov	r3, #0
    8098:	e50b3008 	str	r3, [fp, #-8]
    809c:	ea00002d 	b	8158 <_start+0x128>
    4️⃣80a0:	e51b3008 	ldr	r3, [fp, #-8]
    80a4:	e50b300c 	str	r3, [fp, #-12]
    80a8:	ea00001a 	b	8118 <_start+0xe8>
    5️⃣80ac:	e59f20e4 	ldr	r2, [pc, #228]	@ 8198 <_start+0x168>
    80b0:	e51b300c 	ldr	r3, [fp, #-12]
    80b4:	e1a03083 	lsl	r3, r3, #1
    80b8:	e0823003 	add	r3, r2, r3
    80bc:	e1d330b0 	ldrh	r3, [r3]
    80c0:	e14b31b6 	strh	r3, [fp, #-22]	@ 0xffffffea
    80c4:	e51b300c 	ldr	r3, [fp, #-12]
    80c8:	e2433001 	sub	r3, r3, #1
    80cc:	e59f20c4 	ldr	r2, [pc, #196]	@ 8198 <_start+0x168>
    80d0:	e1a03083 	lsl	r3, r3, #1
    80d4:	e0823003 	add	r3, r2, r3
    80d8:	e1d320f0 	ldrsh	r2, [r3]
    80dc:	e59f10b4 	ldr	r1, [pc, #180]	@ 8198 <_start+0x168>
    80e0:	e51b300c 	ldr	r3, [fp, #-12]
    80e4:	e1a03083 	lsl	r3, r3, #1
    80e8:	e0813003 	add	r3, r1, r3
    80ec:	e1c320b0 	strh	r2, [r3]
    80f0:	e51b300c 	ldr	r3, [fp, #-12]
    80f4:	e2433001 	sub	r3, r3, #1
    80f8:	e59f2098 	ldr	r2, [pc, #152]	@ 8198 <_start+0x168>
    80fc:	e1a03083 	lsl	r3, r3, #1
    8100:	e0823003 	add	r3, r2, r3
    8104:	e15b21b6 	ldrh	r2, [fp, #-22]	@ 0xffffffea
    8108:	e1c320b0 	strh	r2, [r3]
    810c:	e51b300c 	ldr	r3, [fp, #-12]
    8110:	e2433001 	sub	r3, r3, #1
    8114:	e50b300c 	str	r3, [fp, #-12]
    6️⃣8118:	e59f2078 	ldr	r2, [pc, #120]	@ 8198 <_start+0x168>
    811c:	e51b300c 	ldr	r3, [fp, #-12]
    8120:	e1a03083 	lsl	r3, r3, #1
    8124:	e0823003 	add	r3, r2, r3
    8128:	e1d320f0 	ldrsh	r2, [r3]
    812c:	e51b300c 	ldr	r3, [fp, #-12]
    8130:	e2433001 	sub	r3, r3, #1
    8134:	e59f105c 	ldr	r1, [pc, #92]	@ 8198 <_start+0x168>
    8138:	e1a03083 	lsl	r3, r3, #1
    813c:	e0813003 	add	r3, r1, r3
    8140:	e1d330f0 	ldrsh	r3, [r3]
    8144:	e1520003 	cmp	r2, r3
    8148:	baffffd7 	blt	80ac <_start+0x7c>
    7️⃣814c:	e51b3008 	ldr	r3, [fp, #-8]
    8150:	e2833001 	add	r3, r3, #1
    8154:	e50b3008 	str	r3, [fp, #-8]
    8️⃣8158:	e51b3008 	ldr	r3, [fp, #-8]
    815c:	e3530b01 	cmp	r3, #1024	@ 0x400
    8160:	baffffce 	blt	80a0 <_start+0x70>
    9️⃣8164:	e3a01002 	mov	r1, #2
    8168:	e3a00001 	mov	r0, #1
    816c:	ebffffa3 	bl	8000 <add1>
    🔟8170:	e50b0010 	str	r0, [fp, #-16]
    8174:	e3a01004 	mov	r1, #4
    8178:	e3a00003 	mov	r0, #3
    817c:	eb000006 	bl	819c <add2>
    1️⃣1️⃣8180:	e50b0014 	str	r0, [fp, #-20]	@ 0xffffffec
    8184:	e3a03001 	mov	r3, #1
    8188:	e1a00003 	mov	r0, r3
    818c:	e24bd004 	sub	sp, fp, #4
    8190:	e8bd4800 	pop	{fp, lr}
    8194:	e12fff1e 	bx	lr
    1️⃣2️⃣8198:	000091cc 	.word	0x000091cc

0000819c <add2>:
    819c:	e52db004 	push	{fp}		@ (str fp, [sp, #-4]!)
    81a0:	e28db000 	add	fp, sp, #0
    81a4:	e24dd00c 	sub	sp, sp, #12
    81a8:	e50b0008 	str	r0, [fp, #-8]
    81ac:	e50b100c 	str	r1, [fp, #-12]
    81b0:	e51b2008 	ldr	r2, [fp, #-8]
    81b4:	e51b300c 	ldr	r3, [fp, #-12]
    81b8:	e0823003 	add	r3, r2, r3
    81bc:	e1a00003 	mov	r0, r3
    81c0:	e28bd000 	add	sp, fp, #0
    81c4:	e49db004 	pop	{fp}		@ (ldr fp, [sp], #4)
    81c8:	e12fff1e 	bx	lr
