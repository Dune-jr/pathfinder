
if_then_else_2seq.elf:     file format elf32-littlearm


Disassembly of section .init:

00008000 <_init>:
    8000:	e1a0c00d 	mov	ip, sp
    8004:	e92ddff8 	push	{r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}
    8008:	e24cb004 	sub	fp, ip, #4
    800c:	e24bd028 	sub	sp, fp, #40	; 0x28
    8010:	e89d6ff0 	ldm	sp, {r4, r5, r6, r7, r8, r9, sl, fp, sp, lr}
    8014:	e12fff1e 	bx	lr

Disassembly of section .text:

00008018 <deregister_tm_clones>:
    8018:	e92d4008 	push	{r3, lr}
    801c:	e59f002c 	ldr	r0, [pc, #44]	; 8050 <deregister_tm_clones+0x38>
    8020:	e59f302c 	ldr	r3, [pc, #44]	; 8054 <deregister_tm_clones+0x3c>
    8024:	e0603003 	rsb	r3, r0, r3
    8028:	e3530006 	cmp	r3, #6
    802c:	8a000001 	bhi	8038 <deregister_tm_clones+0x20>
    8030:	e8bd4008 	pop	{r3, lr}
    8034:	e12fff1e 	bx	lr
    8038:	e59f3018 	ldr	r3, [pc, #24]	; 8058 <deregister_tm_clones+0x40>
    803c:	e3530000 	cmp	r3, #0
    8040:	0afffffa 	beq	8030 <deregister_tm_clones+0x18>
    8044:	e1a0e00f 	mov	lr, pc
    8048:	e12fff13 	bx	r3
    804c:	eafffff7 	b	8030 <deregister_tm_clones+0x18>
    8050:	0001aec0 	.word	0x0001aec0
    8054:	0001aec3 	.word	0x0001aec3
    8058:	00000000 	.word	0x00000000

0000805c <register_tm_clones>:
    805c:	e92d4008 	push	{r3, lr}
    8060:	e59f0034 	ldr	r0, [pc, #52]	; 809c <register_tm_clones+0x40>
    8064:	e59f3034 	ldr	r3, [pc, #52]	; 80a0 <register_tm_clones+0x44>
    8068:	e0603003 	rsb	r3, r0, r3
    806c:	e1a03143 	asr	r3, r3, #2
    8070:	e0833fa3 	add	r3, r3, r3, lsr #31
    8074:	e1b010c3 	asrs	r1, r3, #1
    8078:	1a000001 	bne	8084 <register_tm_clones+0x28>
    807c:	e8bd4008 	pop	{r3, lr}
    8080:	e12fff1e 	bx	lr
    8084:	e59f2018 	ldr	r2, [pc, #24]	; 80a4 <register_tm_clones+0x48>
    8088:	e3520000 	cmp	r2, #0
    808c:	0afffffa 	beq	807c <register_tm_clones+0x20>
    8090:	e1a0e00f 	mov	lr, pc
    8094:	e12fff12 	bx	r2
    8098:	eafffff7 	b	807c <register_tm_clones+0x20>
    809c:	0001aec0 	.word	0x0001aec0
    80a0:	0001aec0 	.word	0x0001aec0
    80a4:	00000000 	.word	0x00000000

000080a8 <__do_global_dtors_aux>:
    80a8:	e92d4010 	push	{r4, lr}
    80ac:	e59f402c 	ldr	r4, [pc, #44]	; 80e0 <__do_global_dtors_aux+0x38>
    80b0:	e5d43000 	ldrb	r3, [r4]
    80b4:	e3530000 	cmp	r3, #0
    80b8:	1a000006 	bne	80d8 <__do_global_dtors_aux+0x30>
    80bc:	ebffffd5 	bl	8018 <deregister_tm_clones>
    80c0:	e59f301c 	ldr	r3, [pc, #28]	; 80e4 <__do_global_dtors_aux+0x3c>
    80c4:	e3530000 	cmp	r3, #0
    80c8:	159f0018 	ldrne	r0, [pc, #24]	; 80e8 <__do_global_dtors_aux+0x40>
    80cc:	11a00000 	movne	r0, r0
    80d0:	e3a03001 	mov	r3, #1
    80d4:	e5c43000 	strb	r3, [r4]
    80d8:	e8bd4010 	pop	{r4, lr}
    80dc:	e12fff1e 	bx	lr
    80e0:	0001aec0 	.word	0x0001aec0
    80e4:	00000000 	.word	0x00000000
    80e8:	000124d8 	.word	0x000124d8

000080ec <frame_dummy>:
    80ec:	e92d4008 	push	{r3, lr}
    80f0:	e59f3034 	ldr	r3, [pc, #52]	; 812c <frame_dummy+0x40>
    80f4:	e3530000 	cmp	r3, #0
    80f8:	159f0030 	ldrne	r0, [pc, #48]	; 8130 <frame_dummy+0x44>
    80fc:	159f1030 	ldrne	r1, [pc, #48]	; 8134 <frame_dummy+0x48>
    8100:	11a00000 	movne	r0, r0
    8104:	e59f002c 	ldr	r0, [pc, #44]	; 8138 <frame_dummy+0x4c>
    8108:	e5903000 	ldr	r3, [r0]
    810c:	e3530000 	cmp	r3, #0
    8110:	0a000003 	beq	8124 <frame_dummy+0x38>
    8114:	e59f3020 	ldr	r3, [pc, #32]	; 813c <frame_dummy+0x50>
    8118:	e3530000 	cmp	r3, #0
    811c:	11a0e00f 	movne	lr, pc
    8120:	112fff13 	bxne	r3
    8124:	e8bd4008 	pop	{r3, lr}
    8128:	eaffffcb 	b	805c <register_tm_clones>
    812c:	00000000 	.word	0x00000000
    8130:	000124d8 	.word	0x000124d8
    8134:	0001aec4 	.word	0x0001aec4
    8138:	0001a4e4 	.word	0x0001a4e4
    813c:	00000000 	.word	0x00000000

00008140 <_mainCRTStartup>:
    8140:	e3b00016 	movs	r0, #22
    8144:	e28f1e11 	add	r1, pc, #272	; 0x110
    8148:	ef123456 	svc	0x00123456
    814c:	e59f0108 	ldr	r0, [pc, #264]	; 825c <_mainCRTStartup+0x11c>
    8150:	e5901000 	ldr	r1, [r0]
    8154:	e3510000 	cmp	r1, #0
    8158:	1a000001 	bne	8164 <_mainCRTStartup+0x24>
    815c:	e59f1110 	ldr	r1, [pc, #272]	; 8274 <_mainCRTStartup+0x134>
    8160:	e5801000 	str	r1, [r0]
    8164:	e5901008 	ldr	r1, [r0, #8]
    8168:	e590200c 	ldr	r2, [r0, #12]
    816c:	e3510000 	cmp	r1, #0
    8170:	0a000000 	beq	8178 <_mainCRTStartup+0x38>
    8174:	e1a0d001 	mov	sp, r1
    8178:	e3520000 	cmp	r2, #0
    817c:	0a000000 	beq	8184 <_mainCRTStartup+0x44>
    8180:	e282ac01 	add	sl, r2, #256	; 0x100
    8184:	e3b01000 	movs	r1, #0
    8188:	e1a0b001 	mov	fp, r1
    818c:	e1a07001 	mov	r7, r1
    8190:	e59f00c8 	ldr	r0, [pc, #200]	; 8260 <_mainCRTStartup+0x120>
    8194:	e59f20c8 	ldr	r2, [pc, #200]	; 8264 <_mainCRTStartup+0x124>
    8198:	e0522000 	subs	r2, r2, r0
    819c:	eb0000a9 	bl	8448 <memset>
    81a0:	eb000131 	bl	866c <initialise_monitor_handles>
    81a4:	e3b00015 	movs	r0, #21
    81a8:	e28f10bc 	add	r1, pc, #188	; 0xbc
    81ac:	ef123456 	svc	0x00123456
    81b0:	e59f10b4 	ldr	r1, [pc, #180]	; 826c <_mainCRTStartup+0x12c>
    81b4:	e3b00000 	movs	r0, #0
    81b8:	e92d0001 	push	{r0}
    81bc:	e4d13001 	ldrb	r3, [r1], #1
    81c0:	e3530000 	cmp	r3, #0
    81c4:	0a000011 	beq	8210 <_mainCRTStartup+0xd0>
    81c8:	e3530020 	cmp	r3, #32
    81cc:	0afffffa 	beq	81bc <_mainCRTStartup+0x7c>
    81d0:	e3530022 	cmp	r3, #34	; 0x22
    81d4:	13530027 	cmpne	r3, #39	; 0x27
    81d8:	01a02003 	moveq	r2, r3
    81dc:	13a02020 	movne	r2, #32
    81e0:	12411001 	subne	r1, r1, #1
    81e4:	e92d0002 	push	{r1}
    81e8:	e2900001 	adds	r0, r0, #1
    81ec:	e4d13001 	ldrb	r3, [r1], #1
    81f0:	e3530000 	cmp	r3, #0
    81f4:	0a000005 	beq	8210 <_mainCRTStartup+0xd0>
    81f8:	e1520003 	cmp	r2, r3
    81fc:	1afffffa 	bne	81ec <_mainCRTStartup+0xac>
    8200:	e3b02000 	movs	r2, #0
    8204:	e2513001 	subs	r3, r1, #1
    8208:	e5c32000 	strb	r2, [r3]
    820c:	eaffffea 	b	81bc <_mainCRTStartup+0x7c>
    8210:	e1a0100d 	mov	r1, sp
    8214:	e08d2100 	add	r2, sp, r0, lsl #2
    8218:	e1a0300d 	mov	r3, sp
    821c:	e1520003 	cmp	r2, r3
    8220:	85124004 	ldrhi	r4, [r2, #-4]
    8224:	85935000 	ldrhi	r5, [r3]
    8228:	85225004 	strhi	r5, [r2, #-4]!
    822c:	84834004 	strhi	r4, [r3], #4
    8230:	8afffff9 	bhi	821c <_mainCRTStartup+0xdc>
    8234:	e3cdd007 	bic	sp, sp, #7
    8238:	e1b04000 	movs	r4, r0
    823c:	e1b05001 	movs	r5, r1
    8240:	e59f0020 	ldr	r0, [pc, #32]	; 8268 <_mainCRTStartup+0x128>
    8244:	eb000037 	bl	8328 <atexit>
    8248:	eb00005c 	bl	83c0 <__libc_init_array>
    824c:	e1b00004 	movs	r0, r4
    8250:	e1b01005 	movs	r1, r5
    8254:	eb00001f 	bl	82d8 <main>
    8258:	eb00003a 	bl	8348 <exit>
    825c:	0001a4ec 	.word	0x0001a4ec
    8260:	0001aec0 	.word	0x0001aec0
    8264:	0001afd0 	.word	0x0001afd0
    8268:	0000837c 	.word	0x0000837c
    826c:	0001a4fc 	.word	0x0001a4fc
    8270:	000000ff 	.word	0x000000ff
    8274:	0001afd0 	.word	0x0001afd0

00008278 <f1>:
    8278:	e92d4008 	push	{r3, lr}
    827c:	e59f0008 	ldr	r0, [pc, #8]	; 828c <f1+0x14>
    8280:	eb0000bb 	bl	8574 <printf>
    8284:	e8bd4008 	pop	{r3, lr}
    8288:	e12fff1e 	bx	lr
    828c:	000122d0 	.word	0x000122d0

00008290 <f2>:
    8290:	e92d4008 	push	{r3, lr}
    8294:	e59f0008 	ldr	r0, [pc, #8]	; 82a4 <f2+0x14>
    8298:	eb0000b5 	bl	8574 <printf>
    829c:	e8bd4008 	pop	{r3, lr}
    82a0:	e12fff1e 	bx	lr
    82a4:	000122d8 	.word	0x000122d8

000082a8 <f3>:
    82a8:	e92d4008 	push	{r3, lr}
    82ac:	e59f0008 	ldr	r0, [pc, #8]	; 82bc <f3+0x14>
    82b0:	eb0000af 	bl	8574 <printf>
    82b4:	e8bd4008 	pop	{r3, lr}
    82b8:	e12fff1e 	bx	lr
    82bc:	000122e0 	.word	0x000122e0

000082c0 <f4>:
    82c0:	e92d4008 	push	{r3, lr}
    82c4:	e59f0008 	ldr	r0, [pc, #8]	; 82d4 <f4+0x14>
    82c8:	eb0000a9 	bl	8574 <printf>
    82cc:	e8bd4008 	pop	{r3, lr}
    82d0:	e12fff1e 	bx	lr
    82d4:	000122e8 	.word	0x000122e8

000082d8 <main>:
    82d8:	e92d4010 	push	{r4, lr}
    82dc:	e1a04000 	mov	r4, r0
    82e0:	e350000a 	cmp	r0, #10
    82e4:	ca000001 	bgt	82f0 <main+0x18>
    82e8:	ebffffe2 	bl	8278 <f1>
    82ec:	ea000000 	b	82f4 <main+0x1c>
    82f0:	ebffffe6 	bl	8290 <f2>
    82f4:	e3540000 	cmp	r4, #0
    82f8:	ca000001 	bgt	8304 <main+0x2c>
    82fc:	ebffffe9 	bl	82a8 <f3>
    8300:	ea000000 	b	8308 <main+0x30>
    8304:	ebffffed 	bl	82c0 <f4>
    8308:	e59f0014 	ldr	r0, [pc, #20]	; 8324 <main+0x4c>
    830c:	e3a01000 	mov	r1, #0
    8310:	e1a02001 	mov	r2, r1
    8314:	eb000096 	bl	8574 <printf>
    8318:	e3a00000 	mov	r0, #0
    831c:	e8bd4010 	pop	{r4, lr}
    8320:	e12fff1e 	bx	lr
    8324:	000122f0 	.word	0x000122f0

00008328 <atexit>:
    8328:	e1a01000 	mov	r1, r0
    832c:	e3a00000 	mov	r0, #0
    8330:	e92d4008 	push	{r3, lr}
    8334:	e1a02000 	mov	r2, r0
    8338:	e1a03000 	mov	r3, r0
    833c:	eb000b18 	bl	afa4 <__register_exitproc>
    8340:	e8bd4008 	pop	{r3, lr}
    8344:	e12fff1e 	bx	lr

00008348 <exit>:
    8348:	e92d4008 	push	{r3, lr}
    834c:	e3a01000 	mov	r1, #0
    8350:	e1a04000 	mov	r4, r0
    8354:	eb000b4f 	bl	b098 <__call_exitprocs>
    8358:	e59f3018 	ldr	r3, [pc, #24]	; 8378 <exit+0x30>
    835c:	e5930000 	ldr	r0, [r3]
    8360:	e590303c 	ldr	r3, [r0, #60]	; 0x3c
    8364:	e3530000 	cmp	r3, #0
    8368:	11a0e00f 	movne	lr, pc
    836c:	112fff13 	bxne	r3
    8370:	e1a00004 	mov	r0, r4
    8374:	eb000218 	bl	8bdc <_exit>
    8378:	000122f8 	.word	0x000122f8

0000837c <__libc_fini_array>:
    837c:	e92d4038 	push	{r3, r4, r5, lr}
    8380:	e59f5030 	ldr	r5, [pc, #48]	; 83b8 <__libc_fini_array+0x3c>
    8384:	e59f4030 	ldr	r4, [pc, #48]	; 83bc <__libc_fini_array+0x40>
    8388:	e0654004 	rsb	r4, r5, r4
    838c:	e1b04144 	asrs	r4, r4, #2
    8390:	10855104 	addne	r5, r5, r4, lsl #2
    8394:	0a000004 	beq	83ac <__libc_fini_array+0x30>
    8398:	e5353004 	ldr	r3, [r5, #-4]!
    839c:	e1a0e00f 	mov	lr, pc
    83a0:	e12fff13 	bx	r3
    83a4:	e2544001 	subs	r4, r4, #1
    83a8:	1afffffa 	bne	8398 <__libc_fini_array+0x1c>
    83ac:	eb0027c1 	bl	122b8 <_fini>
    83b0:	e8bd4038 	pop	{r3, r4, r5, lr}
    83b4:	e12fff1e 	bx	lr
    83b8:	0001a4e0 	.word	0x0001a4e0
    83bc:	0001a4e4 	.word	0x0001a4e4

000083c0 <__libc_init_array>:
    83c0:	e92d4070 	push	{r4, r5, r6, lr}
    83c4:	e59f506c 	ldr	r5, [pc, #108]	; 8438 <__libc_init_array+0x78>
    83c8:	e59f606c 	ldr	r6, [pc, #108]	; 843c <__libc_init_array+0x7c>
    83cc:	e0656006 	rsb	r6, r5, r6
    83d0:	e1b06146 	asrs	r6, r6, #2
    83d4:	12455004 	subne	r5, r5, #4
    83d8:	13a04000 	movne	r4, #0
    83dc:	0a000005 	beq	83f8 <__libc_init_array+0x38>
    83e0:	e5b53004 	ldr	r3, [r5, #4]!
    83e4:	e2844001 	add	r4, r4, #1
    83e8:	e1a0e00f 	mov	lr, pc
    83ec:	e12fff13 	bx	r3
    83f0:	e1560004 	cmp	r6, r4
    83f4:	1afffff9 	bne	83e0 <__libc_init_array+0x20>
    83f8:	e59f5040 	ldr	r5, [pc, #64]	; 8440 <__libc_init_array+0x80>
    83fc:	e59f6040 	ldr	r6, [pc, #64]	; 8444 <__libc_init_array+0x84>
    8400:	e0656006 	rsb	r6, r5, r6
    8404:	ebfffefd 	bl	8000 <_init>
    8408:	e1b06146 	asrs	r6, r6, #2
    840c:	12455004 	subne	r5, r5, #4
    8410:	13a04000 	movne	r4, #0
    8414:	0a000005 	beq	8430 <__libc_init_array+0x70>
    8418:	e5b53004 	ldr	r3, [r5, #4]!
    841c:	e2844001 	add	r4, r4, #1
    8420:	e1a0e00f 	mov	lr, pc
    8424:	e12fff13 	bx	r3
    8428:	e1560004 	cmp	r6, r4
    842c:	1afffff9 	bne	8418 <__libc_init_array+0x58>
    8430:	e8bd4070 	pop	{r4, r5, r6, lr}
    8434:	e12fff1e 	bx	lr
    8438:	0001a4dc 	.word	0x0001a4dc
    843c:	0001a4dc 	.word	0x0001a4dc
    8440:	0001a4dc 	.word	0x0001a4dc
    8444:	0001a4e0 	.word	0x0001a4e0

00008448 <memset>:
    8448:	e3100003 	tst	r0, #3
    844c:	e92d0030 	push	{r4, r5}
    8450:	e1a03000 	mov	r3, r0
    8454:	0a000037 	beq	8538 <memset+0xf0>
    8458:	e3520000 	cmp	r2, #0
    845c:	e2422001 	sub	r2, r2, #1
    8460:	0a000032 	beq	8530 <memset+0xe8>
    8464:	e20140ff 	and	r4, r1, #255	; 0xff
    8468:	ea000002 	b	8478 <memset+0x30>
    846c:	e3520000 	cmp	r2, #0
    8470:	e2422001 	sub	r2, r2, #1
    8474:	0a00002d 	beq	8530 <memset+0xe8>
    8478:	e4c34001 	strb	r4, [r3], #1
    847c:	e3130003 	tst	r3, #3
    8480:	e1a0c003 	mov	ip, r3
    8484:	1afffff8 	bne	846c <memset+0x24>
    8488:	e3520003 	cmp	r2, #3
    848c:	9a000020 	bls	8514 <memset+0xcc>
    8490:	e20140ff 	and	r4, r1, #255	; 0xff
    8494:	e352000f 	cmp	r2, #15
    8498:	e1844404 	orr	r4, r4, r4, lsl #8
    849c:	e1844804 	orr	r4, r4, r4, lsl #16
    84a0:	81a0300c 	movhi	r3, ip
    84a4:	81a05002 	movhi	r5, r2
    84a8:	9a00000e 	bls	84e8 <memset+0xa0>
    84ac:	e2455010 	sub	r5, r5, #16
    84b0:	e355000f 	cmp	r5, #15
    84b4:	e5834000 	str	r4, [r3]
    84b8:	e5834004 	str	r4, [r3, #4]
    84bc:	e5834008 	str	r4, [r3, #8]
    84c0:	e583400c 	str	r4, [r3, #12]
    84c4:	e2833010 	add	r3, r3, #16
    84c8:	8afffff7 	bhi	84ac <memset+0x64>
    84cc:	e2423010 	sub	r3, r2, #16
    84d0:	e3c3300f 	bic	r3, r3, #15
    84d4:	e202200f 	and	r2, r2, #15
    84d8:	e2833010 	add	r3, r3, #16
    84dc:	e3520003 	cmp	r2, #3
    84e0:	e08cc003 	add	ip, ip, r3
    84e4:	9a00000a 	bls	8514 <memset+0xcc>
    84e8:	e1a0500c 	mov	r5, ip
    84ec:	e1a03002 	mov	r3, r2
    84f0:	e2433004 	sub	r3, r3, #4
    84f4:	e3530003 	cmp	r3, #3
    84f8:	e4854004 	str	r4, [r5], #4
    84fc:	8afffffb 	bhi	84f0 <memset+0xa8>
    8500:	e2423004 	sub	r3, r2, #4
    8504:	e3c33003 	bic	r3, r3, #3
    8508:	e2833004 	add	r3, r3, #4
    850c:	e2022003 	and	r2, r2, #3
    8510:	e08cc003 	add	ip, ip, r3
    8514:	e3520000 	cmp	r2, #0
    8518:	120110ff 	andne	r1, r1, #255	; 0xff
    851c:	108c2002 	addne	r2, ip, r2
    8520:	0a000002 	beq	8530 <memset+0xe8>
    8524:	e4cc1001 	strb	r1, [ip], #1
    8528:	e15c0002 	cmp	ip, r2
    852c:	1afffffc 	bne	8524 <memset+0xdc>
    8530:	e8bd0030 	pop	{r4, r5}
    8534:	e12fff1e 	bx	lr
    8538:	e1a0c000 	mov	ip, r0
    853c:	eaffffd1 	b	8488 <memset+0x40>

00008540 <_printf_r>:
    8540:	e92d000e 	push	{r1, r2, r3}
    8544:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
    8548:	e24dd008 	sub	sp, sp, #8
    854c:	e28dc010 	add	ip, sp, #16
    8550:	e5901008 	ldr	r1, [r0, #8]
    8554:	e59d200c 	ldr	r2, [sp, #12]
    8558:	e1a0300c 	mov	r3, ip
    855c:	e58dc004 	str	ip, [sp, #4]
    8560:	eb00026d 	bl	8f1c <_vfprintf_r>
    8564:	e28dd008 	add	sp, sp, #8
    8568:	e49de004 	pop	{lr}		; (ldr lr, [sp], #4)
    856c:	e28dd00c 	add	sp, sp, #12
    8570:	e12fff1e 	bx	lr

00008574 <printf>:
    8574:	e92d000f 	push	{r0, r1, r2, r3}
    8578:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
    857c:	e59f302c 	ldr	r3, [pc, #44]	; 85b0 <printf+0x3c>
    8580:	e24dd00c 	sub	sp, sp, #12
    8584:	e5930000 	ldr	r0, [r3]
    8588:	e28dc014 	add	ip, sp, #20
    858c:	e5901008 	ldr	r1, [r0, #8]
    8590:	e59d2010 	ldr	r2, [sp, #16]
    8594:	e1a0300c 	mov	r3, ip
    8598:	e58dc004 	str	ip, [sp, #4]
    859c:	eb00025e 	bl	8f1c <_vfprintf_r>
    85a0:	e28dd00c 	add	sp, sp, #12
    85a4:	e49de004 	pop	{lr}		; (ldr lr, [sp], #4)
    85a8:	e28dd010 	add	sp, sp, #16
    85ac:	e12fff1e 	bx	lr
    85b0:	0001a600 	.word	0x0001a600

000085b4 <error>:
    85b4:	e92d40f8 	push	{r3, r4, r5, r6, r7, lr}
    85b8:	e1a04000 	mov	r4, r0
    85bc:	eb001133 	bl	ca90 <__errno>
    85c0:	e3a07000 	mov	r7, #0
    85c4:	e1a05000 	mov	r5, r0
    85c8:	e3a06013 	mov	r6, #19
    85cc:	e1a00006 	mov	r0, r6
    85d0:	e1a01007 	mov	r1, r7
    85d4:	ef123456 	svc	0x00123456
    85d8:	e1a06000 	mov	r6, r0
    85dc:	e5856000 	str	r6, [r5]
    85e0:	e1a00004 	mov	r0, r4
    85e4:	e8bd40f8 	pop	{r3, r4, r5, r6, r7, lr}
    85e8:	e12fff1e 	bx	lr

000085ec <wrap.part.0>:
    85ec:	e3e00000 	mvn	r0, #0
    85f0:	eaffffef 	b	85b4 <error>

000085f4 <remap_handle>:
    85f4:	e59f3068 	ldr	r3, [pc, #104]	; 8664 <remap_handle+0x70>
    85f8:	e5933000 	ldr	r3, [r3]
    85fc:	e3530000 	cmp	r3, #0
    8600:	e92d4010 	push	{r4, lr}
    8604:	e1a04000 	mov	r4, r0
    8608:	0a000002 	beq	8618 <remap_handle+0x24>
    860c:	e5932038 	ldr	r2, [r3, #56]	; 0x38
    8610:	e3520000 	cmp	r2, #0
    8614:	0a00000c 	beq	864c <remap_handle+0x58>
    8618:	e3540000 	cmp	r4, #0
    861c:	0a000007 	beq	8640 <remap_handle+0x4c>
    8620:	e3540001 	cmp	r4, #1
    8624:	0a00000b 	beq	8658 <remap_handle+0x64>
    8628:	e3540002 	cmp	r4, #2
    862c:	059f3034 	ldreq	r3, [pc, #52]	; 8668 <remap_handle+0x74>
    8630:	05930008 	ldreq	r0, [r3, #8]
    8634:	12440020 	subne	r0, r4, #32
    8638:	e8bd4010 	pop	{r4, lr}
    863c:	e12fff1e 	bx	lr
    8640:	e59f3020 	ldr	r3, [pc, #32]	; 8668 <remap_handle+0x74>
    8644:	e5930000 	ldr	r0, [r3]
    8648:	eafffffa 	b	8638 <remap_handle+0x44>
    864c:	e1a00003 	mov	r0, r3
    8650:	eb001202 	bl	ce60 <__sinit>
    8654:	eaffffef 	b	8618 <remap_handle+0x24>
    8658:	e59f3008 	ldr	r3, [pc, #8]	; 8668 <remap_handle+0x74>
    865c:	e5930004 	ldr	r0, [r3, #4]
    8660:	eafffff4 	b	8638 <remap_handle+0x44>
    8664:	0001a600 	.word	0x0001a600
    8668:	0001aedc 	.word	0x0001aedc

0000866c <initialise_monitor_handles>:
    866c:	e92d47f0 	push	{r4, r5, r6, r7, r8, r9, sl, lr}
    8670:	e59fa0a0 	ldr	sl, [pc, #160]	; 8718 <initialise_monitor_handles+0xac>
    8674:	e24dd010 	sub	sp, sp, #16
    8678:	e3a08003 	mov	r8, #3
    867c:	e3a06000 	mov	r6, #0
    8680:	e58da004 	str	sl, [sp, #4]
    8684:	e3a05001 	mov	r5, #1
    8688:	e58d800c 	str	r8, [sp, #12]
    868c:	e28d7004 	add	r7, sp, #4
    8690:	e58d6008 	str	r6, [sp, #8]
    8694:	e1a00005 	mov	r0, r5
    8698:	e1a01007 	mov	r1, r7
    869c:	ef123456 	svc	0x00123456
    86a0:	e1a09000 	mov	r9, r0
    86a4:	e59f4070 	ldr	r4, [pc, #112]	; 871c <initialise_monitor_handles+0xb0>
    86a8:	e3a03004 	mov	r3, #4
    86ac:	e58da004 	str	sl, [sp, #4]
    86b0:	e5849000 	str	r9, [r4]
    86b4:	e58d800c 	str	r8, [sp, #12]
    86b8:	e58d3008 	str	r3, [sp, #8]
    86bc:	e1a00005 	mov	r0, r5
    86c0:	e1a01007 	mov	r1, r7
    86c4:	ef123456 	svc	0x00123456
    86c8:	e1a05000 	mov	r5, r0
    86cc:	e5845008 	str	r5, [r4, #8]
    86d0:	e5845004 	str	r5, [r4, #4]
    86d4:	e1a03006 	mov	r3, r6
    86d8:	e284000c 	add	r0, r4, #12
    86dc:	e3e01000 	mvn	r1, #0
    86e0:	e7801003 	str	r1, [r0, r3]
    86e4:	e2833008 	add	r3, r3, #8
    86e8:	e35300a0 	cmp	r3, #160	; 0xa0
    86ec:	e59f2028 	ldr	r2, [pc, #40]	; 871c <initialise_monitor_handles+0xb0>
    86f0:	1afffffa 	bne	86e0 <initialise_monitor_handles+0x74>
    86f4:	e5921000 	ldr	r1, [r2]
    86f8:	e3a03000 	mov	r3, #0
    86fc:	e582100c 	str	r1, [r2, #12]
    8700:	e5823010 	str	r3, [r2, #16]
    8704:	e5825014 	str	r5, [r2, #20]
    8708:	e5823018 	str	r3, [r2, #24]
    870c:	e28dd010 	add	sp, sp, #16
    8710:	e8bd47f0 	pop	{r4, r5, r6, r7, r8, r9, sl, lr}
    8714:	e12fff1e 	bx	lr
    8718:	00012300 	.word	0x00012300
    871c:	0001aedc 	.word	0x0001aedc

00008720 <_swiread>:
    8720:	e92d4030 	push	{r4, r5, lr}
    8724:	e24dd014 	sub	sp, sp, #20
    8728:	e1a04001 	mov	r4, r1
    872c:	e1a05002 	mov	r5, r2
    8730:	ebffffaf 	bl	85f4 <remap_handle>
    8734:	e98d0031 	stmib	sp, {r0, r4, r5}
    8738:	e28d5004 	add	r5, sp, #4
    873c:	e3a04006 	mov	r4, #6
    8740:	e1a00004 	mov	r0, r4
    8744:	e1a01005 	mov	r1, r5
    8748:	ef123456 	svc	0x00123456
    874c:	e1a04000 	mov	r4, r0
    8750:	e1a00004 	mov	r0, r4
    8754:	e28dd014 	add	sp, sp, #20
    8758:	e8bd4030 	pop	{r4, r5, lr}
    875c:	e12fff1e 	bx	lr

00008760 <_read>:
    8760:	e92d41e8 	push	{r3, r5, r6, r7, r8, lr}
    8764:	e1a07000 	mov	r7, r0
    8768:	e1a08001 	mov	r8, r1
    876c:	e1a06002 	mov	r6, r2
    8770:	ebffff9f 	bl	85f4 <remap_handle>
    8774:	e59fe080 	ldr	lr, [pc, #128]	; 87fc <_read+0x9c>
    8778:	e3a03000 	mov	r3, #0
    877c:	e79ec183 	ldr	ip, [lr, r3, lsl #3]
    8780:	e150000c 	cmp	r0, ip
    8784:	e1a05183 	lsl	r5, r3, #3
    8788:	e2833001 	add	r3, r3, #1
    878c:	0a000009 	beq	87b8 <_read+0x58>
    8790:	e3530014 	cmp	r3, #20
    8794:	1afffff8 	bne	877c <_read+0x1c>
    8798:	e1a00007 	mov	r0, r7
    879c:	e1a01008 	mov	r1, r8
    87a0:	e1a02006 	mov	r2, r6
    87a4:	ebffffdd 	bl	8720 <_swiread>
    87a8:	e3500000 	cmp	r0, #0
    87ac:	ba00000f 	blt	87f0 <_read+0x90>
    87b0:	e0600006 	rsb	r0, r0, r6
    87b4:	ea00000b 	b	87e8 <_read+0x88>
    87b8:	e1a00007 	mov	r0, r7
    87bc:	e1a01008 	mov	r1, r8
    87c0:	e1a02006 	mov	r2, r6
    87c4:	ebffffd5 	bl	8720 <_swiread>
    87c8:	e3500000 	cmp	r0, #0
    87cc:	ba000007 	blt	87f0 <_read+0x90>
    87d0:	e59f3028 	ldr	r3, [pc, #40]	; 8800 <_read+0xa0>
    87d4:	e0835005 	add	r5, r3, r5
    87d8:	e5953010 	ldr	r3, [r5, #16]
    87dc:	e0600006 	rsb	r0, r0, r6
    87e0:	e0833000 	add	r3, r3, r0
    87e4:	e5853010 	str	r3, [r5, #16]
    87e8:	e8bd41e8 	pop	{r3, r5, r6, r7, r8, lr}
    87ec:	e12fff1e 	bx	lr
    87f0:	e3e00000 	mvn	r0, #0
    87f4:	e8bd41e8 	pop	{r3, r5, r6, r7, r8, lr}
    87f8:	eaffff6d 	b	85b4 <error>
    87fc:	0001aee8 	.word	0x0001aee8
    8800:	0001aedc 	.word	0x0001aedc

00008804 <_swilseek>:
    8804:	e92d47f0 	push	{r4, r5, r6, r7, r8, r9, sl, lr}
    8808:	e24dd008 	sub	sp, sp, #8
    880c:	e1a08000 	mov	r8, r0
    8810:	e1a06001 	mov	r6, r1
    8814:	e1a0a002 	mov	sl, r2
    8818:	ebffff75 	bl	85f4 <remap_handle>
    881c:	e59f70dc 	ldr	r7, [pc, #220]	; 8900 <_swilseek+0xfc>
    8820:	e3a04000 	mov	r4, #0
    8824:	e287500c 	add	r5, r7, #12
    8828:	e7953184 	ldr	r3, [r5, r4, lsl #3]
    882c:	e1500003 	cmp	r0, r3
    8830:	e1a03184 	lsl	r3, r4, #3
    8834:	0a000006 	beq	8854 <_swilseek+0x50>
    8838:	e2844001 	add	r4, r4, #1
    883c:	e3540014 	cmp	r4, #20
    8840:	1afffff8 	bne	8828 <_swilseek+0x24>
    8844:	e35a0001 	cmp	sl, #1
    8848:	13a09000 	movne	r9, #0
    884c:	1a000003 	bne	8860 <_swilseek+0x5c>
    8850:	ea000015 	b	88ac <_swilseek+0xa8>
    8854:	e35a0001 	cmp	sl, #1
    8858:	13a09001 	movne	r9, #1
    885c:	0a000020 	beq	88e4 <_swilseek+0xe0>
    8860:	e35a0002 	cmp	sl, #2
    8864:	11a0500d 	movne	r5, sp
    8868:	0a000014 	beq	88c0 <_swilseek+0xbc>
    886c:	e1a00008 	mov	r0, r8
    8870:	ebffff5f 	bl	85f4 <remap_handle>
    8874:	e3a0800a 	mov	r8, #10
    8878:	e88d0041 	stm	sp, {r0, r6}
    887c:	e1a00008 	mov	r0, r8
    8880:	e1a01005 	mov	r1, r5
    8884:	ef123456 	svc	0x00123456
    8888:	e1a05000 	mov	r5, r0
    888c:	e2755001 	rsbs	r5, r5, #1
    8890:	33a05000 	movcc	r5, #0
    8894:	e1150009 	tst	r5, r9
    8898:	10874184 	addne	r4, r7, r4, lsl #3
    889c:	15846010 	strne	r6, [r4, #16]
    88a0:	1a000002 	bne	88b0 <_swilseek+0xac>
    88a4:	e3550000 	cmp	r5, #0
    88a8:	1a000000 	bne	88b0 <_swilseek+0xac>
    88ac:	e3e06000 	mvn	r6, #0
    88b0:	e1a00006 	mov	r0, r6
    88b4:	e28dd008 	add	sp, sp, #8
    88b8:	e8bd47f0 	pop	{r4, r5, r6, r7, r8, r9, sl, lr}
    88bc:	e12fff1e 	bx	lr
    88c0:	e28d5008 	add	r5, sp, #8
    88c4:	e5250008 	str	r0, [r5, #-8]!
    88c8:	e3a0a00c 	mov	sl, #12
    88cc:	e1a0000a 	mov	r0, sl
    88d0:	e1a01005 	mov	r1, r5
    88d4:	ef123456 	svc	0x00123456
    88d8:	e1a0a000 	mov	sl, r0
    88dc:	e086600a 	add	r6, r6, sl
    88e0:	eaffffe1 	b	886c <_swilseek+0x68>
    88e4:	e59f2014 	ldr	r2, [pc, #20]	; 8900 <_swilseek+0xfc>
    88e8:	e0823003 	add	r3, r2, r3
    88ec:	e5933010 	ldr	r3, [r3, #16]
    88f0:	e1a0900a 	mov	r9, sl
    88f4:	e0866003 	add	r6, r6, r3
    88f8:	e1a0500d 	mov	r5, sp
    88fc:	eaffffda 	b	886c <_swilseek+0x68>
    8900:	0001aedc 	.word	0x0001aedc

00008904 <_lseek>:
    8904:	e92d4008 	push	{r3, lr}
    8908:	ebffffbd 	bl	8804 <_swilseek>
    890c:	e3700001 	cmn	r0, #1
    8910:	0a000001 	beq	891c <_lseek+0x18>
    8914:	e8bd4008 	pop	{r3, lr}
    8918:	e12fff1e 	bx	lr
    891c:	e8bd4008 	pop	{r3, lr}
    8920:	eaffff31 	b	85ec <wrap.part.0>

00008924 <_swiwrite>:
    8924:	e92d4030 	push	{r4, r5, lr}
    8928:	e24dd014 	sub	sp, sp, #20
    892c:	e1a04001 	mov	r4, r1
    8930:	e1a05002 	mov	r5, r2
    8934:	ebffff2e 	bl	85f4 <remap_handle>
    8938:	e98d0031 	stmib	sp, {r0, r4, r5}
    893c:	e28d5004 	add	r5, sp, #4
    8940:	e3a04005 	mov	r4, #5
    8944:	e1a00004 	mov	r0, r4
    8948:	e1a01005 	mov	r1, r5
    894c:	ef123456 	svc	0x00123456
    8950:	e1a04000 	mov	r4, r0
    8954:	e1a00004 	mov	r0, r4
    8958:	e28dd014 	add	sp, sp, #20
    895c:	e8bd4030 	pop	{r4, r5, lr}
    8960:	e12fff1e 	bx	lr

00008964 <_write>:
    8964:	e92d41e8 	push	{r3, r5, r6, r7, r8, lr}
    8968:	e1a07000 	mov	r7, r0
    896c:	e1a08001 	mov	r8, r1
    8970:	e1a06002 	mov	r6, r2
    8974:	ebffff1e 	bl	85f4 <remap_handle>
    8978:	e59fe088 	ldr	lr, [pc, #136]	; 8a08 <_write+0xa4>
    897c:	e3a03000 	mov	r3, #0
    8980:	e79ec183 	ldr	ip, [lr, r3, lsl #3]
    8984:	e150000c 	cmp	r0, ip
    8988:	e1a05183 	lsl	r5, r3, #3
    898c:	e2833001 	add	r3, r3, #1
    8990:	0a00000a 	beq	89c0 <_write+0x5c>
    8994:	e3530014 	cmp	r3, #20
    8998:	1afffff8 	bne	8980 <_write+0x1c>
    899c:	e1a00007 	mov	r0, r7
    89a0:	e1a01008 	mov	r1, r8
    89a4:	e1a02006 	mov	r2, r6
    89a8:	ebffffdd 	bl	8924 <_swiwrite>
    89ac:	e3700001 	cmn	r0, #1
    89b0:	11500006 	cmpne	r0, r6
    89b4:	0a000010 	beq	89fc <_write+0x98>
    89b8:	e0600006 	rsb	r0, r0, r6
    89bc:	ea00000c 	b	89f4 <_write+0x90>
    89c0:	e1a00007 	mov	r0, r7
    89c4:	e1a01008 	mov	r1, r8
    89c8:	e1a02006 	mov	r2, r6
    89cc:	ebffffd4 	bl	8924 <_swiwrite>
    89d0:	e3700001 	cmn	r0, #1
    89d4:	11500006 	cmpne	r0, r6
    89d8:	0a000007 	beq	89fc <_write+0x98>
    89dc:	e59f3028 	ldr	r3, [pc, #40]	; 8a0c <_write+0xa8>
    89e0:	e0835005 	add	r5, r3, r5
    89e4:	e5953010 	ldr	r3, [r5, #16]
    89e8:	e0600006 	rsb	r0, r0, r6
    89ec:	e0833000 	add	r3, r3, r0
    89f0:	e5853010 	str	r3, [r5, #16]
    89f4:	e8bd41e8 	pop	{r3, r5, r6, r7, r8, lr}
    89f8:	e12fff1e 	bx	lr
    89fc:	e3e00000 	mvn	r0, #0
    8a00:	e8bd41e8 	pop	{r3, r5, r6, r7, r8, lr}
    8a04:	eafffeea 	b	85b4 <error>
    8a08:	0001aee8 	.word	0x0001aee8
    8a0c:	0001aedc 	.word	0x0001aedc

00008a10 <_swiopen>:
    8a10:	e92d4070 	push	{r4, r5, r6, lr}
    8a14:	e59fc0bc 	ldr	ip, [pc, #188]	; 8ad8 <_swiopen+0xc8>
    8a18:	e24dd010 	sub	sp, sp, #16
    8a1c:	e1a06000 	mov	r6, r0
    8a20:	e3a03000 	mov	r3, #0
    8a24:	e79c2183 	ldr	r2, [ip, r3, lsl #3]
    8a28:	e3720001 	cmn	r2, #1
    8a2c:	e1a04183 	lsl	r4, r3, #3
    8a30:	e2833001 	add	r3, r3, #1
    8a34:	0a000005 	beq	8a50 <_swiopen+0x40>
    8a38:	e3530014 	cmp	r3, #20
    8a3c:	1afffff8 	bne	8a24 <_swiopen+0x14>
    8a40:	e3e00000 	mvn	r0, #0
    8a44:	e28dd010 	add	sp, sp, #16
    8a48:	e8bd4070 	pop	{r4, r5, r6, lr}
    8a4c:	e12fff1e 	bx	lr
    8a50:	e1a05821 	lsr	r5, r1, #16
    8a54:	e2055001 	and	r5, r5, #1
    8a58:	e3110002 	tst	r1, #2
    8a5c:	13855002 	orrne	r5, r5, #2
    8a60:	e3110c02 	tst	r1, #512	; 0x200
    8a64:	13855004 	orrne	r5, r5, #4
    8a68:	e3110b01 	tst	r1, #1024	; 0x400
    8a6c:	13855004 	orrne	r5, r5, #4
    8a70:	e3110008 	tst	r1, #8
    8a74:	13c55004 	bicne	r5, r5, #4
    8a78:	e1a00006 	mov	r0, r6
    8a7c:	13855008 	orrne	r5, r5, #8
    8a80:	e58d6004 	str	r6, [sp, #4]
    8a84:	eb001993 	bl	f0d8 <strlen>
    8a88:	e58d5008 	str	r5, [sp, #8]
    8a8c:	e58d000c 	str	r0, [sp, #12]
    8a90:	e28d6004 	add	r6, sp, #4
    8a94:	e3a05001 	mov	r5, #1
    8a98:	e1a00005 	mov	r0, r5
    8a9c:	e1a01006 	mov	r1, r6
    8aa0:	ef123456 	svc	0x00123456
    8aa4:	e1a05000 	mov	r5, r0
    8aa8:	e3550000 	cmp	r5, #0
    8aac:	ba000006 	blt	8acc <_swiopen+0xbc>
    8ab0:	e59f3024 	ldr	r3, [pc, #36]	; 8adc <_swiopen+0xcc>
    8ab4:	e0834004 	add	r4, r3, r4
    8ab8:	e3a03000 	mov	r3, #0
    8abc:	e584500c 	str	r5, [r4, #12]
    8ac0:	e5843010 	str	r3, [r4, #16]
    8ac4:	e2850020 	add	r0, r5, #32
    8ac8:	eaffffdd 	b	8a44 <_swiopen+0x34>
    8acc:	e1a00005 	mov	r0, r5
    8ad0:	ebfffeb7 	bl	85b4 <error>
    8ad4:	eaffffda 	b	8a44 <_swiopen+0x34>
    8ad8:	0001aee8 	.word	0x0001aee8
    8adc:	0001aedc 	.word	0x0001aedc

00008ae0 <_open>:
    8ae0:	e92d000e 	push	{r1, r2, r3}
    8ae4:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
    8ae8:	e59d1004 	ldr	r1, [sp, #4]
    8aec:	ebffffc7 	bl	8a10 <_swiopen>
    8af0:	e3700001 	cmn	r0, #1
    8af4:	0a000002 	beq	8b04 <_open+0x24>
    8af8:	e49de004 	pop	{lr}		; (ldr lr, [sp], #4)
    8afc:	e28dd00c 	add	sp, sp, #12
    8b00:	e12fff1e 	bx	lr
    8b04:	ebfffeb8 	bl	85ec <wrap.part.0>
    8b08:	eafffffa 	b	8af8 <_open+0x18>

00008b0c <_swiclose>:
    8b0c:	e92d4030 	push	{r4, r5, lr}
    8b10:	e24dd00c 	sub	sp, sp, #12
    8b14:	ebfffeb6 	bl	85f4 <remap_handle>
    8b18:	e59f105c 	ldr	r1, [pc, #92]	; 8b7c <_swiclose+0x70>
    8b1c:	e58d0004 	str	r0, [sp, #4]
    8b20:	e3a03000 	mov	r3, #0
    8b24:	e7912183 	ldr	r2, [r1, r3, lsl #3]
    8b28:	e1500002 	cmp	r0, r2
    8b2c:	e1a02183 	lsl	r2, r3, #3
    8b30:	e2833001 	add	r3, r3, #1
    8b34:	0a00000b 	beq	8b68 <_swiclose+0x5c>
    8b38:	e3530014 	cmp	r3, #20
    8b3c:	1afffff8 	bne	8b24 <_swiclose+0x18>
    8b40:	e28d5004 	add	r5, sp, #4
    8b44:	e3a04002 	mov	r4, #2
    8b48:	e1a00004 	mov	r0, r4
    8b4c:	e1a01005 	mov	r1, r5
    8b50:	ef123456 	svc	0x00123456
    8b54:	e1a04000 	mov	r4, r0
    8b58:	e1a00004 	mov	r0, r4
    8b5c:	e28dd00c 	add	sp, sp, #12
    8b60:	e8bd4030 	pop	{r4, r5, lr}
    8b64:	e12fff1e 	bx	lr
    8b68:	e59f3010 	ldr	r3, [pc, #16]	; 8b80 <_swiclose+0x74>
    8b6c:	e0832002 	add	r2, r3, r2
    8b70:	e3e03000 	mvn	r3, #0
    8b74:	e582300c 	str	r3, [r2, #12]
    8b78:	eafffff0 	b	8b40 <_swiclose+0x34>
    8b7c:	0001aee8 	.word	0x0001aee8
    8b80:	0001aedc 	.word	0x0001aedc

00008b84 <_close>:
    8b84:	e92d4008 	push	{r3, lr}
    8b88:	ebffffdf 	bl	8b0c <_swiclose>
    8b8c:	e3700001 	cmn	r0, #1
    8b90:	0a000001 	beq	8b9c <_close+0x18>
    8b94:	e8bd4008 	pop	{r3, lr}
    8b98:	e12fff1e 	bx	lr
    8b9c:	e8bd4008 	pop	{r3, lr}
    8ba0:	eafffe91 	b	85ec <wrap.part.0>

00008ba4 <_kill>:
    8ba4:	e3510006 	cmp	r1, #6
    8ba8:	e92d4030 	push	{r4, r5, lr}
    8bac:	059f5020 	ldreq	r5, [pc, #32]	; 8bd4 <_kill+0x30>
    8bb0:	159f5020 	ldrne	r5, [pc, #32]	; 8bd8 <_kill+0x34>
    8bb4:	e3a04018 	mov	r4, #24
    8bb8:	e1a00004 	mov	r0, r4
    8bbc:	e1a01005 	mov	r1, r5
    8bc0:	ef123456 	svc	0x00123456
    8bc4:	e1a04000 	mov	r4, r0
    8bc8:	e1a00004 	mov	r0, r4
    8bcc:	e8bd4030 	pop	{r4, r5, lr}
    8bd0:	e12fff1e 	bx	lr
    8bd4:	00020023 	.word	0x00020023
    8bd8:	00020026 	.word	0x00020026

00008bdc <_exit>:
    8bdc:	e52de004 	push	{lr}		; (str lr, [sp, #-4]!)
    8be0:	e59f5018 	ldr	r5, [pc, #24]	; 8c00 <_exit+0x24>
    8be4:	e3a04018 	mov	r4, #24
    8be8:	e1a00004 	mov	r0, r4
    8bec:	e1a01005 	mov	r1, r5
    8bf0:	ef123456 	svc	0x00123456
    8bf4:	e1a04000 	mov	r4, r0
    8bf8:	e49de004 	pop	{lr}		; (ldr lr, [sp], #4)
    8bfc:	e12fff1e 	bx	lr
    8c00:	00020026 	.word	0x00020026

00008c04 <_getpid>:
    8c04:	e3a00001 	mov	r0, #1
    8c08:	e12fff1e 	bx	lr

00008c0c <_sbrk>:
    8c0c:	e59f2044 	ldr	r2, [pc, #68]	; 8c58 <_sbrk+0x4c>
    8c10:	e92d4008 	push	{r3, lr}
    8c14:	e59230ac 	ldr	r3, [r2, #172]	; 0xac
    8c18:	e3530000 	cmp	r3, #0
    8c1c:	059f3038 	ldreq	r3, [pc, #56]	; 8c5c <_sbrk+0x50>
    8c20:	e1a0100d 	mov	r1, sp
    8c24:	e0830000 	add	r0, r3, r0
    8c28:	058230ac 	streq	r3, [r2, #172]	; 0xac
    8c2c:	e1500001 	cmp	r0, r1
    8c30:	958200ac 	strls	r0, [r2, #172]	; 0xac
    8c34:	8a000002 	bhi	8c44 <_sbrk+0x38>
    8c38:	e1a00003 	mov	r0, r3
    8c3c:	e8bd4008 	pop	{r3, lr}
    8c40:	e12fff1e 	bx	lr
    8c44:	eb000f91 	bl	ca90 <__errno>
    8c48:	e3a0300c 	mov	r3, #12
    8c4c:	e5803000 	str	r3, [r0]
    8c50:	e3e03000 	mvn	r3, #0
    8c54:	eafffff7 	b	8c38 <_sbrk+0x2c>
    8c58:	0001aedc 	.word	0x0001aedc
    8c5c:	0001afd0 	.word	0x0001afd0

00008c60 <_fstat>:
    8c60:	e92d4010 	push	{r4, lr}
    8c64:	e1a00001 	mov	r0, r1
    8c68:	e1a04001 	mov	r4, r1
    8c6c:	e3a0203c 	mov	r2, #60	; 0x3c
    8c70:	e3a01000 	mov	r1, #0
    8c74:	ebfffdf3 	bl	8448 <memset>
    8c78:	e3a03a02 	mov	r3, #8192	; 0x2000
    8c7c:	e5843004 	str	r3, [r4, #4]
    8c80:	e3a03b01 	mov	r3, #1024	; 0x400
    8c84:	e584302c 	str	r3, [r4, #44]	; 0x2c
    8c88:	e3a00000 	mov	r0, #0
    8c8c:	e8bd4010 	pop	{r4, lr}
    8c90:	e12fff1e 	bx	lr

00008c94 <_stat>:
    8c94:	e92d4038 	push	{r3, r4, r5, lr}
    8c98:	e1a04001 	mov	r4, r1
    8c9c:	e3a01000 	mov	r1, #0
    8ca0:	ebffff8e 	bl	8ae0 <_open>
    8ca4:	e2505000 	subs	r5, r0, #0
    8ca8:	ba00000c 	blt	8ce0 <_stat+0x4c>
    8cac:	e1a00004 	mov	r0, r4
    8cb0:	e3a01000 	mov	r1, #0
    8cb4:	e3a0203c 	mov	r2, #60	; 0x3c
    8cb8:	ebfffde2 	bl	8448 <memset>
    8cbc:	e3a03c81 	mov	r3, #33024	; 0x8100
    8cc0:	e5843004 	str	r3, [r4, #4]
    8cc4:	e3a03b01 	mov	r3, #1024	; 0x400
    8cc8:	e584302c 	str	r3, [r4, #44]	; 0x2c
    8ccc:	e1a00005 	mov	r0, r5
    8cd0:	ebffff8d 	bl	8b0c <_swiclose>
    8cd4:	e3a00000 	mov	r0, #0
    8cd8:	e8bd4038 	pop	{r3, r4, r5, lr}
    8cdc:	e12fff1e 	bx	lr
    8ce0:	e3e00000 	mvn	r0, #0
    8ce4:	eafffffb 	b	8cd8 <_stat+0x44>

00008ce8 <_link>:
    8ce8:	e3e00000 	mvn	r0, #0
    8cec:	e12fff1e 	bx	lr

00008cf0 <_unlink>:
    8cf0:	e92d4030 	push	{r4, r5, lr}
    8cf4:	e24dd00c 	sub	sp, sp, #12
    8cf8:	e58d0000 	str	r0, [sp]
    8cfc:	eb0018f5 	bl	f0d8 <strlen>
    8d00:	e3a0400e 	mov	r4, #14
    8d04:	e58d0004 	str	r0, [sp, #4]
    8d08:	e1a0500d 	mov	r5, sp
    8d0c:	e1a00004 	mov	r0, r4
    8d10:	e1a01005 	mov	r1, r5
    8d14:	ef123456 	svc	0x00123456
    8d18:	e1a04000 	mov	r4, r0
    8d1c:	e3740001 	cmn	r4, #1
    8d20:	0a000005 	beq	8d3c <_unlink+0x4c>
    8d24:	e3540000 	cmp	r4, #0
    8d28:	13e00000 	mvnne	r0, #0
    8d2c:	03a00000 	moveq	r0, #0
    8d30:	e28dd00c 	add	sp, sp, #12
    8d34:	e8bd4030 	pop	{r4, r5, lr}
    8d38:	e12fff1e 	bx	lr
    8d3c:	ebfffe2a 	bl	85ec <wrap.part.0>
    8d40:	e1a04000 	mov	r4, r0
    8d44:	eafffff6 	b	8d24 <_unlink+0x34>

00008d48 <_raise>:
    8d48:	e12fff1e 	bx	lr

00008d4c <_gettimeofday>:
    8d4c:	e92d40f0 	push	{r4, r5, r6, r7, lr}
    8d50:	e2505000 	subs	r5, r0, #0
    8d54:	e1a04001 	mov	r4, r1
    8d58:	0a000007 	beq	8d7c <_gettimeofday+0x30>
    8d5c:	e3a06000 	mov	r6, #0
    8d60:	e3a07011 	mov	r7, #17
    8d64:	e1a00007 	mov	r0, r7
    8d68:	e1a01006 	mov	r1, r6
    8d6c:	ef123456 	svc	0x00123456
    8d70:	e1a07000 	mov	r7, r0
    8d74:	e5857000 	str	r7, [r5]
    8d78:	e5856004 	str	r6, [r5, #4]
    8d7c:	e3540000 	cmp	r4, #0
    8d80:	13a03000 	movne	r3, #0
    8d84:	15843000 	strne	r3, [r4]
    8d88:	15843004 	strne	r3, [r4, #4]
    8d8c:	e3a00000 	mov	r0, #0
    8d90:	e8bd40f0 	pop	{r4, r5, r6, r7, lr}
    8d94:	e12fff1e 	bx	lr

00008d98 <_times>:
    8d98:	e92d4070 	push	{r4, r5, r6, lr}
    8d9c:	e1a04000 	mov	r4, r0
    8da0:	e3a05000 	mov	r5, #0
    8da4:	e3a06010 	mov	r6, #16
    8da8:	e1a00006 	mov	r0, r6
    8dac:	e1a01005 	mov	r1, r5
    8db0:	ef123456 	svc	0x00123456
    8db4:	e1a06000 	mov	r6, r0
    8db8:	e1540005 	cmp	r4, r5
    8dbc:	15846000 	strne	r6, [r4]
    8dc0:	15845004 	strne	r5, [r4, #4]
    8dc4:	15845008 	strne	r5, [r4, #8]
    8dc8:	1584500c 	strne	r5, [r4, #12]
    8dcc:	e1a00006 	mov	r0, r6
    8dd0:	e8bd4070 	pop	{r4, r5, r6, lr}
    8dd4:	e12fff1e 	bx	lr

00008dd8 <_isatty>:
    8dd8:	e92d4030 	push	{r4, r5, lr}
    8ddc:	e24dd00c 	sub	sp, sp, #12
    8de0:	ebfffe03 	bl	85f4 <remap_handle>
    8de4:	e28d4008 	add	r4, sp, #8
    8de8:	e5240004 	str	r0, [r4, #-4]!
    8dec:	e3a05009 	mov	r5, #9
    8df0:	e1a00005 	mov	r0, r5
    8df4:	e1a01004 	mov	r1, r4
    8df8:	ef123456 	svc	0x00123456
    8dfc:	e1a04000 	mov	r4, r0
    8e00:	e3740001 	cmn	r4, #1
    8e04:	0a000003 	beq	8e18 <_isatty+0x40>
    8e08:	e1a00004 	mov	r0, r4
    8e0c:	e28dd00c 	add	sp, sp, #12
    8e10:	e8bd4030 	pop	{r4, r5, lr}
    8e14:	e12fff1e 	bx	lr
    8e18:	ebfffdf3 	bl	85ec <wrap.part.0>
    8e1c:	e1a04000 	mov	r4, r0
    8e20:	eafffff8 	b	8e08 <_isatty+0x30>

00008e24 <_system>:
    8e24:	e92d4030 	push	{r4, r5, lr}
    8e28:	e2503000 	subs	r3, r0, #0
    8e2c:	e24dd00c 	sub	sp, sp, #12
    8e30:	03a04001 	moveq	r4, #1
    8e34:	0a000016 	beq	8e94 <_system+0x70>
    8e38:	e58d3000 	str	r3, [sp]
    8e3c:	eb0018a5 	bl	f0d8 <strlen>
    8e40:	e3a04012 	mov	r4, #18
    8e44:	e58d0004 	str	r0, [sp, #4]
    8e48:	e1a0500d 	mov	r5, sp
    8e4c:	e1a00004 	mov	r0, r4
    8e50:	e1a01005 	mov	r1, r5
    8e54:	ef123456 	svc	0x00123456
    8e58:	e1a04000 	mov	r4, r0
    8e5c:	e3740001 	cmn	r4, #1
    8e60:	0a00000f 	beq	8ea4 <_system+0x80>
    8e64:	e35400ff 	cmp	r4, #255	; 0xff
    8e68:	8a000009 	bhi	8e94 <_system+0x70>
    8e6c:	e3540000 	cmp	r4, #0
    8e70:	0a000007 	beq	8e94 <_system+0x70>
    8e74:	e1a03004 	mov	r3, r4
    8e78:	e1b03083 	lsls	r3, r3, #1
    8e7c:	e1a02443 	asr	r2, r3, #8
    8e80:	e20220ff 	and	r2, r2, #255	; 0xff
    8e84:	0a000001 	beq	8e90 <_system+0x6c>
    8e88:	e1520004 	cmp	r2, r4
    8e8c:	1afffff9 	bne	8e78 <_system+0x54>
    8e90:	e1a04003 	mov	r4, r3
    8e94:	e1a00004 	mov	r0, r4
    8e98:	e28dd00c 	add	sp, sp, #12
    8e9c:	e8bd4030 	pop	{r4, r5, lr}
    8ea0:	e12fff1e 	bx	lr
    8ea4:	ebfffdd0 	bl	85ec <wrap.part.0>
    8ea8:	e1a04000 	mov	r4, r0
    8eac:	eaffffec 	b	8e64 <_system+0x40>

00008eb0 <_rename>:
    8eb0:	e92d4030 	push	{r4, r5, lr}
    8eb4:	e24dd014 	sub	sp, sp, #20
    8eb8:	e1a04001 	mov	r4, r1
    8ebc:	e58d0000 	str	r0, [sp]
    8ec0:	eb001884 	bl	f0d8 <strlen>
    8ec4:	e58d0004 	str	r0, [sp, #4]
    8ec8:	e1a00004 	mov	r0, r4
    8ecc:	e58d4008 	str	r4, [sp, #8]
    8ed0:	eb001880 	bl	f0d8 <strlen>
    8ed4:	e3a0400f 	mov	r4, #15
    8ed8:	e58d000c 	str	r0, [sp, #12]
    8edc:	e1a0500d 	mov	r5, sp
    8ee0:	e1a00004 	mov	r0, r4
    8ee4:	e1a01005 	mov	r1, r5
    8ee8:	ef123456 	svc	0x00123456
    8eec:	e1a04000 	mov	r4, r0
    8ef0:	e3740001 	cmn	r4, #1
    8ef4:	0a000005 	beq	8f10 <_rename+0x60>
    8ef8:	e3540000 	cmp	r4, #0
    8efc:	13e00000 	mvnne	r0, #0
    8f00:	03a00000 	moveq	r0, #0
    8f04:	e28dd014 	add	sp, sp, #20
    8f08:	e8bd4030 	pop	{r4, r5, lr}
    8f0c:	e12fff1e 	bx	lr
    8f10:	ebfffdb5 	bl	85ec <wrap.part.0>
    8f14:	e1a04000 	mov	r4, r0
    8f18:	eafffff6 	b	8ef8 <_rename+0x48>

00008f1c <_vfprintf_r>:
    8f1c:	e92d4ff0 	push	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
    8f20:	e24ddd15 	sub	sp, sp, #1344	; 0x540
    8f24:	e24dd004 	sub	sp, sp, #4
    8f28:	e1a09001 	mov	r9, r1
    8f2c:	e1a04002 	mov	r4, r2
    8f30:	e58d3038 	str	r3, [sp, #56]	; 0x38
    8f34:	e58d002c 	str	r0, [sp, #44]	; 0x2c
    8f38:	eb00117f 	bl	d53c <_localeconv_r>
    8f3c:	e5900000 	ldr	r0, [r0]
    8f40:	e58d0058 	str	r0, [sp, #88]	; 0x58
    8f44:	eb001863 	bl	f0d8 <strlen>
    8f48:	e59d702c 	ldr	r7, [sp, #44]	; 0x2c
    8f4c:	e3a0b000 	mov	fp, #0
    8f50:	e3a0c000 	mov	ip, #0
    8f54:	e3570000 	cmp	r7, #0
    8f58:	e58d0064 	str	r0, [sp, #100]	; 0x64
    8f5c:	e58db068 	str	fp, [sp, #104]	; 0x68
    8f60:	e58dc06c 	str	ip, [sp, #108]	; 0x6c
    8f64:	0a000002 	beq	8f74 <_vfprintf_r+0x58>
    8f68:	e5973038 	ldr	r3, [r7, #56]	; 0x38
    8f6c:	e3530000 	cmp	r3, #0
    8f70:	0a0000ff 	beq	9374 <_vfprintf_r+0x458>
    8f74:	e1d980bc 	ldrh	r8, [r9, #12]
    8f78:	e3180a02 	tst	r8, #8192	; 0x2000
    8f7c:	05993064 	ldreq	r3, [r9, #100]	; 0x64
    8f80:	03888a02 	orreq	r8, r8, #8192	; 0x2000
    8f84:	03c33a02 	biceq	r3, r3, #8192	; 0x2000
    8f88:	01c980bc 	strheq	r8, [r9, #12]
    8f8c:	05893064 	streq	r3, [r9, #100]	; 0x64
    8f90:	e3180008 	tst	r8, #8
    8f94:	0a0000a8 	beq	923c <_vfprintf_r+0x320>
    8f98:	e5993010 	ldr	r3, [r9, #16]
    8f9c:	e3530000 	cmp	r3, #0
    8fa0:	0a0000a5 	beq	923c <_vfprintf_r+0x320>
    8fa4:	e208301a 	and	r3, r8, #26
    8fa8:	e353000a 	cmp	r3, #10
    8fac:	0a0000c2 	beq	92bc <_vfprintf_r+0x3a0>
    8fb0:	e28d70d8 	add	r7, sp, #216	; 0xd8
    8fb4:	e28db0d7 	add	fp, sp, #215	; 0xd7
    8fb8:	e3a03000 	mov	r3, #0
    8fbc:	e28dcd05 	add	ip, sp, #320	; 0x140
    8fc0:	e58d7054 	str	r7, [sp, #84]	; 0x54
    8fc4:	e06b7007 	rsb	r7, fp, r7
    8fc8:	e58d3044 	str	r3, [sp, #68]	; 0x44
    8fcc:	e58db05c 	str	fp, [sp, #92]	; 0x5c
    8fd0:	e58dc0a4 	str	ip, [sp, #164]	; 0xa4
    8fd4:	e58d30ac 	str	r3, [sp, #172]	; 0xac
    8fd8:	e58d30a8 	str	r3, [sp, #168]	; 0xa8
    8fdc:	e58d3074 	str	r3, [sp, #116]	; 0x74
    8fe0:	e58d3070 	str	r3, [sp, #112]	; 0x70
    8fe4:	e58d3034 	str	r3, [sp, #52]	; 0x34
    8fe8:	e58d707c 	str	r7, [sp, #124]	; 0x7c
    8fec:	e58d901c 	str	r9, [sp, #28]
    8ff0:	e58d4028 	str	r4, [sp, #40]	; 0x28
    8ff4:	e1a0a00c 	mov	sl, ip
    8ff8:	e1a04003 	mov	r4, r3
    8ffc:	e59dc028 	ldr	ip, [sp, #40]	; 0x28
    9000:	e5dc3000 	ldrb	r3, [ip]
    9004:	e3530025 	cmp	r3, #37	; 0x25
    9008:	13530000 	cmpne	r3, #0
    900c:	0a0000db 	beq	9380 <_vfprintf_r+0x464>
    9010:	e28c3001 	add	r3, ip, #1
    9014:	e1a06003 	mov	r6, r3
    9018:	e5d33000 	ldrb	r3, [r3]
    901c:	e3530025 	cmp	r3, #37	; 0x25
    9020:	13530000 	cmpne	r3, #0
    9024:	e2863001 	add	r3, r6, #1
    9028:	1afffff9 	bne	9014 <_vfprintf_r+0xf8>
    902c:	e59d4028 	ldr	r4, [sp, #40]	; 0x28
    9030:	e0567004 	subs	r7, r6, r4
    9034:	0a00000e 	beq	9074 <_vfprintf_r+0x158>
    9038:	e28d40a8 	add	r4, sp, #168	; 0xa8
    903c:	e8940030 	ldm	r4, {r4, r5}
    9040:	e59dc028 	ldr	ip, [sp, #40]	; 0x28
    9044:	e2844001 	add	r4, r4, #1
    9048:	e0855007 	add	r5, r5, r7
    904c:	e3540007 	cmp	r4, #7
    9050:	e58ac000 	str	ip, [sl]
    9054:	e58a7004 	str	r7, [sl, #4]
    9058:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    905c:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    9060:	d28aa008 	addle	sl, sl, #8
    9064:	ca000081 	bgt	9270 <_vfprintf_r+0x354>
    9068:	e59d4034 	ldr	r4, [sp, #52]	; 0x34
    906c:	e0844007 	add	r4, r4, r7
    9070:	e58d4034 	str	r4, [sp, #52]	; 0x34
    9074:	e5d63000 	ldrb	r3, [r6]
    9078:	e3530000 	cmp	r3, #0
    907c:	0a000627 	beq	a920 <_vfprintf_r+0x1a04>
    9080:	e3a03000 	mov	r3, #0
    9084:	e2866001 	add	r6, r6, #1
    9088:	e58d3030 	str	r3, [sp, #48]	; 0x30
    908c:	e58d6028 	str	r6, [sp, #40]	; 0x28
    9090:	e5cd3083 	strb	r3, [sp, #131]	; 0x83
    9094:	e58d3018 	str	r3, [sp, #24]
    9098:	e3e04000 	mvn	r4, #0
    909c:	e3a01020 	mov	r1, #32
    90a0:	e3a0202b 	mov	r2, #43	; 0x2b
    90a4:	e1a00006 	mov	r0, r6
    90a8:	e4d07001 	ldrb	r7, [r0], #1
    90ac:	e2473020 	sub	r3, r7, #32
    90b0:	e3530058 	cmp	r3, #88	; 0x58
    90b4:	979ff103 	ldrls	pc, [pc, r3, lsl #2]
    90b8:	ea0001ff 	b	98bc <_vfprintf_r+0x9a0>
    90bc:	000098f8 	.word	0x000098f8
    90c0:	000098bc 	.word	0x000098bc
    90c4:	000098bc 	.word	0x000098bc
    90c8:	00009908 	.word	0x00009908
    90cc:	000098bc 	.word	0x000098bc
    90d0:	000098bc 	.word	0x000098bc
    90d4:	000098bc 	.word	0x000098bc
    90d8:	000098bc 	.word	0x000098bc
    90dc:	000098bc 	.word	0x000098bc
    90e0:	000098bc 	.word	0x000098bc
    90e4:	00009918 	.word	0x00009918
    90e8:	00009388 	.word	0x00009388
    90ec:	000098bc 	.word	0x000098bc
    90f0:	0000922c 	.word	0x0000922c
    90f4:	00009390 	.word	0x00009390
    90f8:	000098bc 	.word	0x000098bc
    90fc:	000093d8 	.word	0x000093d8
    9100:	000093e8 	.word	0x000093e8
    9104:	000093e8 	.word	0x000093e8
    9108:	000093e8 	.word	0x000093e8
    910c:	000093e8 	.word	0x000093e8
    9110:	000093e8 	.word	0x000093e8
    9114:	000093e8 	.word	0x000093e8
    9118:	000093e8 	.word	0x000093e8
    911c:	000093e8 	.word	0x000093e8
    9120:	000093e8 	.word	0x000093e8
    9124:	000098bc 	.word	0x000098bc
    9128:	000098bc 	.word	0x000098bc
    912c:	000098bc 	.word	0x000098bc
    9130:	000098bc 	.word	0x000098bc
    9134:	000098bc 	.word	0x000098bc
    9138:	000098bc 	.word	0x000098bc
    913c:	000098bc 	.word	0x000098bc
    9140:	000098bc 	.word	0x000098bc
    9144:	000098bc 	.word	0x000098bc
    9148:	000098bc 	.word	0x000098bc
    914c:	00009418 	.word	0x00009418
    9150:	00009620 	.word	0x00009620
    9154:	000098bc 	.word	0x000098bc
    9158:	00009620 	.word	0x00009620
    915c:	000098bc 	.word	0x000098bc
    9160:	000098bc 	.word	0x000098bc
    9164:	000098bc 	.word	0x000098bc
    9168:	000098bc 	.word	0x000098bc
    916c:	000096d4 	.word	0x000096d4
    9170:	000098bc 	.word	0x000098bc
    9174:	000098bc 	.word	0x000098bc
    9178:	000096e4 	.word	0x000096e4
    917c:	000098bc 	.word	0x000098bc
    9180:	000098bc 	.word	0x000098bc
    9184:	000098bc 	.word	0x000098bc
    9188:	000098bc 	.word	0x000098bc
    918c:	000098bc 	.word	0x000098bc
    9190:	00009728 	.word	0x00009728
    9194:	000098bc 	.word	0x000098bc
    9198:	000098bc 	.word	0x000098bc
    919c:	00009764 	.word	0x00009764
    91a0:	000098bc 	.word	0x000098bc
    91a4:	000098bc 	.word	0x000098bc
    91a8:	000098bc 	.word	0x000098bc
    91ac:	000098bc 	.word	0x000098bc
    91b0:	000098bc 	.word	0x000098bc
    91b4:	000098bc 	.word	0x000098bc
    91b8:	000098bc 	.word	0x000098bc
    91bc:	000098bc 	.word	0x000098bc
    91c0:	000098bc 	.word	0x000098bc
    91c4:	000098bc 	.word	0x000098bc
    91c8:	00009a28 	.word	0x00009a28
    91cc:	000099dc 	.word	0x000099dc
    91d0:	00009620 	.word	0x00009620
    91d4:	00009620 	.word	0x00009620
    91d8:	00009620 	.word	0x00009620
    91dc:	00009a18 	.word	0x00009a18
    91e0:	000099dc 	.word	0x000099dc
    91e4:	000098bc 	.word	0x000098bc
    91e8:	000098bc 	.word	0x000098bc
    91ec:	00009844 	.word	0x00009844
    91f0:	000098bc 	.word	0x000098bc
    91f4:	00009854 	.word	0x00009854
    91f8:	00009880 	.word	0x00009880
    91fc:	00009a64 	.word	0x00009a64
    9200:	000097a8 	.word	0x000097a8
    9204:	000098bc 	.word	0x000098bc
    9208:	000097b8 	.word	0x000097b8
    920c:	000098bc 	.word	0x000098bc
    9210:	0000993c 	.word	0x0000993c
    9214:	000098bc 	.word	0x000098bc
    9218:	000098bc 	.word	0x000098bc
    921c:	00009964 	.word	0x00009964
    9220:	e26bb000 	rsb	fp, fp, #0
    9224:	e58db030 	str	fp, [sp, #48]	; 0x30
    9228:	e58d3038 	str	r3, [sp, #56]	; 0x38
    922c:	e59d7018 	ldr	r7, [sp, #24]
    9230:	e3877004 	orr	r7, r7, #4
    9234:	e58d7018 	str	r7, [sp, #24]
    9238:	eaffff9a 	b	90a8 <_vfprintf_r+0x18c>
    923c:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    9240:	e1a01009 	mov	r1, r9
    9244:	eb0006ff 	bl	ae48 <__swsetup_r>
    9248:	e3500000 	cmp	r0, #0
    924c:	01d980bc 	ldrheq	r8, [r9, #12]
    9250:	0affff53 	beq	8fa4 <_vfprintf_r+0x88>
    9254:	e3e0c000 	mvn	ip, #0
    9258:	e58dc034 	str	ip, [sp, #52]	; 0x34
    925c:	e59d0034 	ldr	r0, [sp, #52]	; 0x34
    9260:	e28ddf51 	add	sp, sp, #324	; 0x144
    9264:	e28ddb01 	add	sp, sp, #1024	; 0x400
    9268:	e8bd4ff0 	pop	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
    926c:	e12fff1e 	bx	lr
    9270:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    9274:	e59d101c 	ldr	r1, [sp, #28]
    9278:	e28d20a4 	add	r2, sp, #164	; 0xa4
    927c:	eb0017ad 	bl	f138 <__sprint_r>
    9280:	e3500000 	cmp	r0, #0
    9284:	1a000007 	bne	92a8 <_vfprintf_r+0x38c>
    9288:	e28dad05 	add	sl, sp, #320	; 0x140
    928c:	eaffff75 	b	9068 <_vfprintf_r+0x14c>
    9290:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    9294:	e59d101c 	ldr	r1, [sp, #28]
    9298:	e28d20a4 	add	r2, sp, #164	; 0xa4
    929c:	eb0017a5 	bl	f138 <__sprint_r>
    92a0:	e3500000 	cmp	r0, #0
    92a4:	0a0002ae 	beq	9d64 <_vfprintf_r+0xe48>
    92a8:	e59d901c 	ldr	r9, [sp, #28]
    92ac:	e1d930bc 	ldrh	r3, [r9, #12]
    92b0:	e3130040 	tst	r3, #64	; 0x40
    92b4:	1affffe6 	bne	9254 <_vfprintf_r+0x338>
    92b8:	eaffffe7 	b	925c <_vfprintf_r+0x340>
    92bc:	e1d9c0be 	ldrh	ip, [r9, #14]
    92c0:	e31c0902 	tst	ip, #32768	; 0x8000
    92c4:	1affff39 	bne	8fb0 <_vfprintf_r+0x94>
    92c8:	e1a02004 	mov	r2, r4
    92cc:	e59f4fb8 	ldr	r4, [pc, #4024]	; a28c <_vfprintf_r+0x1370>
    92d0:	e28dbd15 	add	fp, sp, #1344	; 0x540
    92d4:	e3c88002 	bic	r8, r8, #2
    92d8:	e18b80b4 	strh	r8, [fp, r4]
    92dc:	e5997064 	ldr	r7, [r9, #100]	; 0x64
    92e0:	e599601c 	ldr	r6, [r9, #28]
    92e4:	e599a024 	ldr	sl, [r9, #36]	; 0x24
    92e8:	e2844002 	add	r4, r4, #2
    92ec:	e3a0eb01 	mov	lr, #1024	; 0x400
    92f0:	e28d5d05 	add	r5, sp, #320	; 0x140
    92f4:	e18bc0b4 	strh	ip, [fp, r4]
    92f8:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    92fc:	e3a0c000 	mov	ip, #0
    9300:	e28d10d8 	add	r1, sp, #216	; 0xd8
    9304:	e59d3038 	ldr	r3, [sp, #56]	; 0x38
    9308:	e58d713c 	str	r7, [sp, #316]	; 0x13c
    930c:	e58d60f4 	str	r6, [sp, #244]	; 0xf4
    9310:	e58da0fc 	str	sl, [sp, #252]	; 0xfc
    9314:	e58d50d8 	str	r5, [sp, #216]	; 0xd8
    9318:	e58d50e8 	str	r5, [sp, #232]	; 0xe8
    931c:	e58de0e0 	str	lr, [sp, #224]	; 0xe0
    9320:	e58de0ec 	str	lr, [sp, #236]	; 0xec
    9324:	e58dc0f0 	str	ip, [sp, #240]	; 0xf0
    9328:	ebfffefb 	bl	8f1c <_vfprintf_r>
    932c:	e3500000 	cmp	r0, #0
    9330:	e58d0034 	str	r0, [sp, #52]	; 0x34
    9334:	ba000006 	blt	9354 <_vfprintf_r+0x438>
    9338:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    933c:	e28d10d8 	add	r1, sp, #216	; 0xd8
    9340:	eb000e62 	bl	ccd0 <_fflush_r>
    9344:	e59dc034 	ldr	ip, [sp, #52]	; 0x34
    9348:	e3500000 	cmp	r0, #0
    934c:	13e0c000 	mvnne	ip, #0
    9350:	e58dc034 	str	ip, [sp, #52]	; 0x34
    9354:	e59f3f30 	ldr	r3, [pc, #3888]	; a28c <_vfprintf_r+0x1370>
    9358:	e28d0d15 	add	r0, sp, #1344	; 0x540
    935c:	e19030b3 	ldrh	r3, [r0, r3]
    9360:	e3130040 	tst	r3, #64	; 0x40
    9364:	11d930bc 	ldrhne	r3, [r9, #12]
    9368:	13833040 	orrne	r3, r3, #64	; 0x40
    936c:	11c930bc 	strhne	r3, [r9, #12]
    9370:	eaffffb9 	b	925c <_vfprintf_r+0x340>
    9374:	e1a00007 	mov	r0, r7
    9378:	eb000eb8 	bl	ce60 <__sinit>
    937c:	eafffefc 	b	8f74 <_vfprintf_r+0x58>
    9380:	e59d6028 	ldr	r6, [sp, #40]	; 0x28
    9384:	eaffff3a 	b	9074 <_vfprintf_r+0x158>
    9388:	e5cd2083 	strb	r2, [sp, #131]	; 0x83
    938c:	eaffff45 	b	90a8 <_vfprintf_r+0x18c>
    9390:	e4d07001 	ldrb	r7, [r0], #1
    9394:	e357002a 	cmp	r7, #42	; 0x2a
    9398:	0a000686 	beq	adb8 <_vfprintf_r+0x1e9c>
    939c:	e2473030 	sub	r3, r7, #48	; 0x30
    93a0:	e3530009 	cmp	r3, #9
    93a4:	83a04000 	movhi	r4, #0
    93a8:	8affff3f 	bhi	90ac <_vfprintf_r+0x190>
    93ac:	e1a0c000 	mov	ip, r0
    93b0:	e3a04000 	mov	r4, #0
    93b4:	e4dc7001 	ldrb	r7, [ip], #1
    93b8:	e0844104 	add	r4, r4, r4, lsl #2
    93bc:	e0834084 	add	r4, r3, r4, lsl #1
    93c0:	e2473030 	sub	r3, r7, #48	; 0x30
    93c4:	e3530009 	cmp	r3, #9
    93c8:	e1a0000c 	mov	r0, ip
    93cc:	9afffff8 	bls	93b4 <_vfprintf_r+0x498>
    93d0:	e1844fc4 	orr	r4, r4, r4, asr #31
    93d4:	eaffff34 	b	90ac <_vfprintf_r+0x190>
    93d8:	e59dc018 	ldr	ip, [sp, #24]
    93dc:	e38cc080 	orr	ip, ip, #128	; 0x80
    93e0:	e58dc018 	str	ip, [sp, #24]
    93e4:	eaffff2f 	b	90a8 <_vfprintf_r+0x18c>
    93e8:	e1a0c000 	mov	ip, r0
    93ec:	e3a03000 	mov	r3, #0
    93f0:	e2470030 	sub	r0, r7, #48	; 0x30
    93f4:	e4dc7001 	ldrb	r7, [ip], #1
    93f8:	e2475030 	sub	r5, r7, #48	; 0x30
    93fc:	e0833103 	add	r3, r3, r3, lsl #2
    9400:	e3550009 	cmp	r5, #9
    9404:	e0803083 	add	r3, r0, r3, lsl #1
    9408:	e1a0000c 	mov	r0, ip
    940c:	9afffff7 	bls	93f0 <_vfprintf_r+0x4d4>
    9410:	e58d3030 	str	r3, [sp, #48]	; 0x30
    9414:	eaffff24 	b	90ac <_vfprintf_r+0x190>
    9418:	e59db018 	ldr	fp, [sp, #24]
    941c:	e38bb010 	orr	fp, fp, #16
    9420:	e58db018 	str	fp, [sp, #24]
    9424:	e59dc018 	ldr	ip, [sp, #24]
    9428:	e31c0010 	tst	ip, #16
    942c:	e58d0028 	str	r0, [sp, #40]	; 0x28
    9430:	0a00016d 	beq	99ec <_vfprintf_r+0xad0>
    9434:	e59db038 	ldr	fp, [sp, #56]	; 0x38
    9438:	e59b3000 	ldr	r3, [fp]
    943c:	e28bb004 	add	fp, fp, #4
    9440:	e58db038 	str	fp, [sp, #56]	; 0x38
    9444:	e3530000 	cmp	r3, #0
    9448:	ba000476 	blt	a628 <_vfprintf_r+0x170c>
    944c:	03a02000 	moveq	r2, #0
    9450:	13a02001 	movne	r2, #1
    9454:	e3a01001 	mov	r1, #1
    9458:	e3540000 	cmp	r4, #0
    945c:	a59dc018 	ldrge	ip, [sp, #24]
    9460:	a3ccc080 	bicge	ip, ip, #128	; 0x80
    9464:	a58dc018 	strge	ip, [sp, #24]
    9468:	e3540000 	cmp	r4, #0
    946c:	13822001 	orrne	r2, r2, #1
    9470:	e21220ff 	ands	r2, r2, #255	; 0xff
    9474:	0a000289 	beq	9ea0 <_vfprintf_r+0xf84>
    9478:	e3510001 	cmp	r1, #1
    947c:	0a00036c 	beq	a234 <_vfprintf_r+0x1318>
    9480:	e3510002 	cmp	r1, #2
    9484:	e59d205c 	ldr	r2, [sp, #92]	; 0x5c
    9488:	0a00035c 	beq	a200 <_vfprintf_r+0x12e4>
    948c:	e2031007 	and	r1, r3, #7
    9490:	e2811030 	add	r1, r1, #48	; 0x30
    9494:	e1b031a3 	lsrs	r3, r3, #3
    9498:	e1a00002 	mov	r0, r2
    949c:	e5c21000 	strb	r1, [r2]
    94a0:	e2422001 	sub	r2, r2, #1
    94a4:	1afffff8 	bne	948c <_vfprintf_r+0x570>
    94a8:	e59db018 	ldr	fp, [sp, #24]
    94ac:	e31b0001 	tst	fp, #1
    94b0:	e58d004c 	str	r0, [sp, #76]	; 0x4c
    94b4:	1a000452 	bne	a604 <_vfprintf_r+0x16e8>
    94b8:	e59dc054 	ldr	ip, [sp, #84]	; 0x54
    94bc:	e060c00c 	rsb	ip, r0, ip
    94c0:	e58dc03c 	str	ip, [sp, #60]	; 0x3c
    94c4:	e59dc03c 	ldr	ip, [sp, #60]	; 0x3c
    94c8:	e58d4040 	str	r4, [sp, #64]	; 0x40
    94cc:	e15c0004 	cmp	ip, r4
    94d0:	b1a0c004 	movlt	ip, r4
    94d4:	e3a04000 	mov	r4, #0
    94d8:	e58dc020 	str	ip, [sp, #32]
    94dc:	e5dd3083 	ldrb	r3, [sp, #131]	; 0x83
    94e0:	e58d4060 	str	r4, [sp, #96]	; 0x60
    94e4:	e3530000 	cmp	r3, #0
    94e8:	0a000002 	beq	94f8 <_vfprintf_r+0x5dc>
    94ec:	e59db020 	ldr	fp, [sp, #32]
    94f0:	e28bb001 	add	fp, fp, #1
    94f4:	e58db020 	str	fp, [sp, #32]
    94f8:	e59dc018 	ldr	ip, [sp, #24]
    94fc:	e21cc002 	ands	ip, ip, #2
    9500:	159d4020 	ldrne	r4, [sp, #32]
    9504:	e59db018 	ldr	fp, [sp, #24]
    9508:	12844002 	addne	r4, r4, #2
    950c:	158d4020 	strne	r4, [sp, #32]
    9510:	e21bb084 	ands	fp, fp, #132	; 0x84
    9514:	e58dc048 	str	ip, [sp, #72]	; 0x48
    9518:	e58db050 	str	fp, [sp, #80]	; 0x50
    951c:	1a000163 	bne	9ab0 <_vfprintf_r+0xb94>
    9520:	e59dc030 	ldr	ip, [sp, #48]	; 0x30
    9524:	e59d4020 	ldr	r4, [sp, #32]
    9528:	e064600c 	rsb	r6, r4, ip
    952c:	e3560000 	cmp	r6, #0
    9530:	da00015e 	ble	9ab0 <_vfprintf_r+0xb94>
    9534:	e28d40a8 	add	r4, sp, #168	; 0xa8
    9538:	e3560010 	cmp	r6, #16
    953c:	e8940030 	ldm	r4, {r4, r5}
    9540:	e59f9d48 	ldr	r9, [pc, #3400]	; a290 <_vfprintf_r+0x1374>
    9544:	da000023 	ble	95d8 <_vfprintf_r+0x6bc>
    9548:	e28d80a4 	add	r8, sp, #164	; 0xa4
    954c:	e1a03009 	mov	r3, r9
    9550:	e58d7078 	str	r7, [sp, #120]	; 0x78
    9554:	e1a09006 	mov	r9, r6
    9558:	e1a07008 	mov	r7, r8
    955c:	e3a0b010 	mov	fp, #16
    9560:	e59d802c 	ldr	r8, [sp, #44]	; 0x2c
    9564:	e1a06003 	mov	r6, r3
    9568:	ea000002 	b	9578 <_vfprintf_r+0x65c>
    956c:	e2499010 	sub	r9, r9, #16
    9570:	e3590010 	cmp	r9, #16
    9574:	da000013 	ble	95c8 <_vfprintf_r+0x6ac>
    9578:	e2844001 	add	r4, r4, #1
    957c:	e3540007 	cmp	r4, #7
    9580:	e2855010 	add	r5, r5, #16
    9584:	e88a0840 	stm	sl, {r6, fp}
    9588:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    958c:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    9590:	d28aa008 	addle	sl, sl, #8
    9594:	dafffff4 	ble	956c <_vfprintf_r+0x650>
    9598:	e1a00008 	mov	r0, r8
    959c:	e59d101c 	ldr	r1, [sp, #28]
    95a0:	e1a02007 	mov	r2, r7
    95a4:	eb0016e3 	bl	f138 <__sprint_r>
    95a8:	e3500000 	cmp	r0, #0
    95ac:	1affff3d 	bne	92a8 <_vfprintf_r+0x38c>
    95b0:	e2499010 	sub	r9, r9, #16
    95b4:	e28d40a8 	add	r4, sp, #168	; 0xa8
    95b8:	e3590010 	cmp	r9, #16
    95bc:	e8940030 	ldm	r4, {r4, r5}
    95c0:	e28dad05 	add	sl, sp, #320	; 0x140
    95c4:	caffffeb 	bgt	9578 <_vfprintf_r+0x65c>
    95c8:	e1a03006 	mov	r3, r6
    95cc:	e59d7078 	ldr	r7, [sp, #120]	; 0x78
    95d0:	e1a06009 	mov	r6, r9
    95d4:	e1a09003 	mov	r9, r3
    95d8:	e2844001 	add	r4, r4, #1
    95dc:	e0855006 	add	r5, r5, r6
    95e0:	e3540007 	cmp	r4, #7
    95e4:	e58a9000 	str	r9, [sl]
    95e8:	e58a6004 	str	r6, [sl, #4]
    95ec:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    95f0:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    95f4:	d28aa008 	addle	sl, sl, #8
    95f8:	da00012d 	ble	9ab4 <_vfprintf_r+0xb98>
    95fc:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    9600:	e59d101c 	ldr	r1, [sp, #28]
    9604:	e28d20a4 	add	r2, sp, #164	; 0xa4
    9608:	eb0016ca 	bl	f138 <__sprint_r>
    960c:	e3500000 	cmp	r0, #0
    9610:	1affff24 	bne	92a8 <_vfprintf_r+0x38c>
    9614:	e59d50ac 	ldr	r5, [sp, #172]	; 0xac
    9618:	e28dad05 	add	sl, sp, #320	; 0x140
    961c:	ea000124 	b	9ab4 <_vfprintf_r+0xb98>
    9620:	e59db018 	ldr	fp, [sp, #24]
    9624:	e31b0008 	tst	fp, #8
    9628:	159dc038 	ldrne	ip, [sp, #56]	; 0x38
    962c:	059db038 	ldreq	fp, [sp, #56]	; 0x38
    9630:	e58d0028 	str	r0, [sp, #40]	; 0x28
    9634:	128c1007 	addne	r1, ip, #7
    9638:	028b1007 	addeq	r1, fp, #7
    963c:	e3c11007 	bic	r1, r1, #7
    9640:	e5918000 	ldr	r8, [r1]
    9644:	e5916004 	ldr	r6, [r1, #4]
    9648:	e2811008 	add	r1, r1, #8
    964c:	e58d1038 	str	r1, [sp, #56]	; 0x38
    9650:	e1a00008 	mov	r0, r8
    9654:	e1a01006 	mov	r1, r6
    9658:	e58d0068 	str	r0, [sp, #104]	; 0x68
    965c:	e58d106c 	str	r1, [sp, #108]	; 0x6c
    9660:	e1a01006 	mov	r1, r6
    9664:	e1a00008 	mov	r0, r8
    9668:	eb0015bc 	bl	ed60 <__fpclassifyd>
    966c:	e3500001 	cmp	r0, #1
    9670:	e1a01006 	mov	r1, r6
    9674:	e1a00008 	mov	r0, r8
    9678:	1a0003f1 	bne	a644 <_vfprintf_r+0x1728>
    967c:	e3a03000 	mov	r3, #0
    9680:	e3a02000 	mov	r2, #0
    9684:	eb0022dc 	bl	121fc <__aeabi_dcmplt>
    9688:	e3500000 	cmp	r0, #0
    968c:	13a0302d 	movne	r3, #45	; 0x2d
    9690:	e59dc018 	ldr	ip, [sp, #24]
    9694:	e59f2bf8 	ldr	r2, [pc, #3064]	; a294 <_vfprintf_r+0x1378>
    9698:	e59f1bf8 	ldr	r1, [pc, #3064]	; a298 <_vfprintf_r+0x137c>
    969c:	05dd3083 	ldrbeq	r3, [sp, #131]	; 0x83
    96a0:	15cd3083 	strbne	r3, [sp, #131]	; 0x83
    96a4:	e3570047 	cmp	r7, #71	; 0x47
    96a8:	e3a04003 	mov	r4, #3
    96ac:	e3a0b000 	mov	fp, #0
    96b0:	d1a01002 	movle	r1, r2
    96b4:	e3ccc080 	bic	ip, ip, #128	; 0x80
    96b8:	e58d4020 	str	r4, [sp, #32]
    96bc:	e58db040 	str	fp, [sp, #64]	; 0x40
    96c0:	e58d104c 	str	r1, [sp, #76]	; 0x4c
    96c4:	e58dc018 	str	ip, [sp, #24]
    96c8:	e58d403c 	str	r4, [sp, #60]	; 0x3c
    96cc:	e58db060 	str	fp, [sp, #96]	; 0x60
    96d0:	eaffff83 	b	94e4 <_vfprintf_r+0x5c8>
    96d4:	e59d7018 	ldr	r7, [sp, #24]
    96d8:	e3877008 	orr	r7, r7, #8
    96dc:	e58d7018 	str	r7, [sp, #24]
    96e0:	eafffe70 	b	90a8 <_vfprintf_r+0x18c>
    96e4:	e59dc018 	ldr	ip, [sp, #24]
    96e8:	e38cc010 	orr	ip, ip, #16
    96ec:	e58dc018 	str	ip, [sp, #24]
    96f0:	e59db018 	ldr	fp, [sp, #24]
    96f4:	e21b2010 	ands	r2, fp, #16
    96f8:	e58d0028 	str	r0, [sp, #40]	; 0x28
    96fc:	0a000063 	beq	9890 <_vfprintf_r+0x974>
    9700:	e59dc038 	ldr	ip, [sp, #56]	; 0x38
    9704:	e59c3000 	ldr	r3, [ip]
    9708:	e3a01000 	mov	r1, #0
    970c:	e28cc004 	add	ip, ip, #4
    9710:	e0532001 	subs	r2, r3, r1
    9714:	e58dc038 	str	ip, [sp, #56]	; 0x38
    9718:	13a02001 	movne	r2, #1
    971c:	e3a00000 	mov	r0, #0
    9720:	e5cd0083 	strb	r0, [sp, #131]	; 0x83
    9724:	eaffff4b 	b	9458 <_vfprintf_r+0x53c>
    9728:	e59db018 	ldr	fp, [sp, #24]
    972c:	e38bb010 	orr	fp, fp, #16
    9730:	e58db018 	str	fp, [sp, #24]
    9734:	e59dc018 	ldr	ip, [sp, #24]
    9738:	e31c0010 	tst	ip, #16
    973c:	e58d0028 	str	r0, [sp, #40]	; 0x28
    9740:	0a000081 	beq	994c <_vfprintf_r+0xa30>
    9744:	e59db038 	ldr	fp, [sp, #56]	; 0x38
    9748:	e59b3000 	ldr	r3, [fp]
    974c:	e2932000 	adds	r2, r3, #0
    9750:	e28bb004 	add	fp, fp, #4
    9754:	e3a01001 	mov	r1, #1
    9758:	13a02001 	movne	r2, #1
    975c:	e58db038 	str	fp, [sp, #56]	; 0x38
    9760:	eaffffed 	b	971c <_vfprintf_r+0x800>
    9764:	e59db018 	ldr	fp, [sp, #24]
    9768:	e59fcb2c 	ldr	ip, [pc, #2860]	; a29c <_vfprintf_r+0x1380>
    976c:	e31b0010 	tst	fp, #16
    9770:	e58d0028 	str	r0, [sp, #40]	; 0x28
    9774:	e58dc074 	str	ip, [sp, #116]	; 0x74
    9778:	0a00007f 	beq	997c <_vfprintf_r+0xa60>
    977c:	e59dc038 	ldr	ip, [sp, #56]	; 0x38
    9780:	e59c3000 	ldr	r3, [ip]
    9784:	e28cc004 	add	ip, ip, #4
    9788:	e58dc038 	str	ip, [sp, #56]	; 0x38
    978c:	e59dc018 	ldr	ip, [sp, #24]
    9790:	e31c0001 	tst	ip, #1
    9794:	1a000085 	bne	99b0 <_vfprintf_r+0xa94>
    9798:	e2932000 	adds	r2, r3, #0
    979c:	13a02001 	movne	r2, #1
    97a0:	e3a01002 	mov	r1, #2
    97a4:	eaffffdc 	b	971c <_vfprintf_r+0x800>
    97a8:	e59d7018 	ldr	r7, [sp, #24]
    97ac:	e3877010 	orr	r7, r7, #16
    97b0:	e58d7018 	str	r7, [sp, #24]
    97b4:	eafffe3b 	b	90a8 <_vfprintf_r+0x18c>
    97b8:	e3a05000 	mov	r5, #0
    97bc:	e5cd5083 	strb	r5, [sp, #131]	; 0x83
    97c0:	e59dc038 	ldr	ip, [sp, #56]	; 0x38
    97c4:	e59cc000 	ldr	ip, [ip]
    97c8:	e59db038 	ldr	fp, [sp, #56]	; 0x38
    97cc:	e15c0005 	cmp	ip, r5
    97d0:	e58d0028 	str	r0, [sp, #40]	; 0x28
    97d4:	e58dc04c 	str	ip, [sp, #76]	; 0x4c
    97d8:	e28b6004 	add	r6, fp, #4
    97dc:	0a0004bd 	beq	aad8 <_vfprintf_r+0x1bbc>
    97e0:	e3540000 	cmp	r4, #0
    97e4:	e59d004c 	ldr	r0, [sp, #76]	; 0x4c
    97e8:	ba00049b 	blt	aa5c <_vfprintf_r+0x1b40>
    97ec:	e1a01005 	mov	r1, r5
    97f0:	e1a02004 	mov	r2, r4
    97f4:	eb00117b 	bl	dde8 <memchr>
    97f8:	e3500000 	cmp	r0, #0
    97fc:	0a00050b 	beq	ac30 <_vfprintf_r+0x1d14>
    9800:	e59dc04c 	ldr	ip, [sp, #76]	; 0x4c
    9804:	e06c0000 	rsb	r0, ip, r0
    9808:	e58d003c 	str	r0, [sp, #60]	; 0x3c
    980c:	e1500004 	cmp	r0, r4
    9810:	d59dc03c 	ldrle	ip, [sp, #60]	; 0x3c
    9814:	c1c4bfc4 	bicgt	fp, r4, r4, asr #31
    9818:	d1cccfcc 	bicle	ip, ip, ip, asr #31
    981c:	e58d5040 	str	r5, [sp, #64]	; 0x40
    9820:	c58db020 	strgt	fp, [sp, #32]
    9824:	c5dd3083 	ldrbgt	r3, [sp, #131]	; 0x83
    9828:	c58d6038 	strgt	r6, [sp, #56]	; 0x38
    982c:	c58d403c 	strgt	r4, [sp, #60]	; 0x3c
    9830:	d58dc020 	strle	ip, [sp, #32]
    9834:	d5dd3083 	ldrble	r3, [sp, #131]	; 0x83
    9838:	d58d6038 	strle	r6, [sp, #56]	; 0x38
    983c:	e58d5060 	str	r5, [sp, #96]	; 0x60
    9840:	eaffff27 	b	94e4 <_vfprintf_r+0x5c8>
    9844:	e59dc018 	ldr	ip, [sp, #24]
    9848:	e38cc010 	orr	ip, ip, #16
    984c:	e58dc018 	str	ip, [sp, #24]
    9850:	eafffe14 	b	90a8 <_vfprintf_r+0x18c>
    9854:	e59dc018 	ldr	ip, [sp, #24]
    9858:	e31c0010 	tst	ip, #16
    985c:	e58d0028 	str	r0, [sp, #40]	; 0x28
    9860:	0a0003e1 	beq	a7ec <_vfprintf_r+0x18d0>
    9864:	e59d4038 	ldr	r4, [sp, #56]	; 0x38
    9868:	e59d7034 	ldr	r7, [sp, #52]	; 0x34
    986c:	e5943000 	ldr	r3, [r4]
    9870:	e2844004 	add	r4, r4, #4
    9874:	e58d4038 	str	r4, [sp, #56]	; 0x38
    9878:	e5837000 	str	r7, [r3]
    987c:	eafffdde 	b	8ffc <_vfprintf_r+0xe0>
    9880:	e59db018 	ldr	fp, [sp, #24]
    9884:	e21b2010 	ands	r2, fp, #16
    9888:	e58d0028 	str	r0, [sp, #40]	; 0x28
    988c:	1affff9b 	bne	9700 <_vfprintf_r+0x7e4>
    9890:	e59db018 	ldr	fp, [sp, #24]
    9894:	e21b1040 	ands	r1, fp, #64	; 0x40
    9898:	0a0003c4 	beq	a7b0 <_vfprintf_r+0x1894>
    989c:	e59dc038 	ldr	ip, [sp, #56]	; 0x38
    98a0:	e1dc30b0 	ldrh	r3, [ip]
    98a4:	e1a01002 	mov	r1, r2
    98a8:	e28cc004 	add	ip, ip, #4
    98ac:	e2932000 	adds	r2, r3, #0
    98b0:	13a02001 	movne	r2, #1
    98b4:	e58dc038 	str	ip, [sp, #56]	; 0x38
    98b8:	eaffff97 	b	971c <_vfprintf_r+0x800>
    98bc:	e3570000 	cmp	r7, #0
    98c0:	e58d0028 	str	r0, [sp, #40]	; 0x28
    98c4:	0a000415 	beq	a920 <_vfprintf_r+0x1a04>
    98c8:	e3a0b001 	mov	fp, #1
    98cc:	e3a03000 	mov	r3, #0
    98d0:	e28dc0b0 	add	ip, sp, #176	; 0xb0
    98d4:	e58db020 	str	fp, [sp, #32]
    98d8:	e5cd70b0 	strb	r7, [sp, #176]	; 0xb0
    98dc:	e5cd3083 	strb	r3, [sp, #131]	; 0x83
    98e0:	e58db03c 	str	fp, [sp, #60]	; 0x3c
    98e4:	e58dc04c 	str	ip, [sp, #76]	; 0x4c
    98e8:	e3a04000 	mov	r4, #0
    98ec:	e58d4040 	str	r4, [sp, #64]	; 0x40
    98f0:	e58d4060 	str	r4, [sp, #96]	; 0x60
    98f4:	eafffeff 	b	94f8 <_vfprintf_r+0x5dc>
    98f8:	e5dd3083 	ldrb	r3, [sp, #131]	; 0x83
    98fc:	e3530000 	cmp	r3, #0
    9900:	05cd1083 	strbeq	r1, [sp, #131]	; 0x83
    9904:	eafffde7 	b	90a8 <_vfprintf_r+0x18c>
    9908:	e59d7018 	ldr	r7, [sp, #24]
    990c:	e3877001 	orr	r7, r7, #1
    9910:	e58d7018 	str	r7, [sp, #24]
    9914:	eafffde3 	b	90a8 <_vfprintf_r+0x18c>
    9918:	e59db038 	ldr	fp, [sp, #56]	; 0x38
    991c:	e59bb000 	ldr	fp, [fp]
    9920:	e59dc038 	ldr	ip, [sp, #56]	; 0x38
    9924:	e35b0000 	cmp	fp, #0
    9928:	e58db030 	str	fp, [sp, #48]	; 0x30
    992c:	e28c3004 	add	r3, ip, #4
    9930:	bafffe3a 	blt	9220 <_vfprintf_r+0x304>
    9934:	e58d3038 	str	r3, [sp, #56]	; 0x38
    9938:	eafffdda 	b	90a8 <_vfprintf_r+0x18c>
    993c:	e59dc018 	ldr	ip, [sp, #24]
    9940:	e31c0010 	tst	ip, #16
    9944:	e58d0028 	str	r0, [sp, #40]	; 0x28
    9948:	1affff7d 	bne	9744 <_vfprintf_r+0x828>
    994c:	e59dc018 	ldr	ip, [sp, #24]
    9950:	e31c0040 	tst	ip, #64	; 0x40
    9954:	0a00039c 	beq	a7cc <_vfprintf_r+0x18b0>
    9958:	e59db038 	ldr	fp, [sp, #56]	; 0x38
    995c:	e1db30b0 	ldrh	r3, [fp]
    9960:	eaffff79 	b	974c <_vfprintf_r+0x830>
    9964:	e59db018 	ldr	fp, [sp, #24]
    9968:	e59fc930 	ldr	ip, [pc, #2352]	; a2a0 <_vfprintf_r+0x1384>
    996c:	e31b0010 	tst	fp, #16
    9970:	e58d0028 	str	r0, [sp, #40]	; 0x28
    9974:	e58dc074 	str	ip, [sp, #116]	; 0x74
    9978:	1affff7f 	bne	977c <_vfprintf_r+0x860>
    997c:	e59db018 	ldr	fp, [sp, #24]
    9980:	e31b0040 	tst	fp, #64	; 0x40
    9984:	159dc038 	ldrne	ip, [sp, #56]	; 0x38
    9988:	059db038 	ldreq	fp, [sp, #56]	; 0x38
    998c:	11dc30b0 	ldrhne	r3, [ip]
    9990:	128cc004 	addne	ip, ip, #4
    9994:	158dc038 	strne	ip, [sp, #56]	; 0x38
    9998:	e59dc018 	ldr	ip, [sp, #24]
    999c:	059b3000 	ldreq	r3, [fp]
    99a0:	028bb004 	addeq	fp, fp, #4
    99a4:	058db038 	streq	fp, [sp, #56]	; 0x38
    99a8:	e31c0001 	tst	ip, #1
    99ac:	0affff79 	beq	9798 <_vfprintf_r+0x87c>
    99b0:	e3530000 	cmp	r3, #0
    99b4:	159db018 	ldrne	fp, [sp, #24]
    99b8:	13a02030 	movne	r2, #48	; 0x30
    99bc:	138bb002 	orrne	fp, fp, #2
    99c0:	15cd2084 	strbne	r2, [sp, #132]	; 0x84
    99c4:	15cd7085 	strbne	r7, [sp, #133]	; 0x85
    99c8:	158db018 	strne	fp, [sp, #24]
    99cc:	13a02001 	movne	r2, #1
    99d0:	01a02003 	moveq	r2, r3
    99d4:	e3a01002 	mov	r1, #2
    99d8:	eaffff4f 	b	971c <_vfprintf_r+0x800>
    99dc:	e59dc018 	ldr	ip, [sp, #24]
    99e0:	e31c0010 	tst	ip, #16
    99e4:	e58d0028 	str	r0, [sp, #40]	; 0x28
    99e8:	1afffe91 	bne	9434 <_vfprintf_r+0x518>
    99ec:	e59dc018 	ldr	ip, [sp, #24]
    99f0:	e31c0040 	tst	ip, #64	; 0x40
    99f4:	159db038 	ldrne	fp, [sp, #56]	; 0x38
    99f8:	059dc038 	ldreq	ip, [sp, #56]	; 0x38
    99fc:	11db30f0 	ldrshne	r3, [fp]
    9a00:	059c3000 	ldreq	r3, [ip]
    9a04:	128bb004 	addne	fp, fp, #4
    9a08:	028cc004 	addeq	ip, ip, #4
    9a0c:	158db038 	strne	fp, [sp, #56]	; 0x38
    9a10:	058dc038 	streq	ip, [sp, #56]	; 0x38
    9a14:	eafffe8a 	b	9444 <_vfprintf_r+0x528>
    9a18:	e59db018 	ldr	fp, [sp, #24]
    9a1c:	e38bb040 	orr	fp, fp, #64	; 0x40
    9a20:	e58db018 	str	fp, [sp, #24]
    9a24:	eafffd9f 	b	90a8 <_vfprintf_r+0x18c>
    9a28:	e59d3038 	ldr	r3, [sp, #56]	; 0x38
    9a2c:	e59dc038 	ldr	ip, [sp, #56]	; 0x38
    9a30:	e5933000 	ldr	r3, [r3]
    9a34:	e3a0b001 	mov	fp, #1
    9a38:	e28cc004 	add	ip, ip, #4
    9a3c:	e5cd30b0 	strb	r3, [sp, #176]	; 0xb0
    9a40:	e28d40b0 	add	r4, sp, #176	; 0xb0
    9a44:	e3a03000 	mov	r3, #0
    9a48:	e58d0028 	str	r0, [sp, #40]	; 0x28
    9a4c:	e58db020 	str	fp, [sp, #32]
    9a50:	e58dc038 	str	ip, [sp, #56]	; 0x38
    9a54:	e5cd3083 	strb	r3, [sp, #131]	; 0x83
    9a58:	e58db03c 	str	fp, [sp, #60]	; 0x3c
    9a5c:	e58d404c 	str	r4, [sp, #76]	; 0x4c
    9a60:	eaffffa0 	b	98e8 <_vfprintf_r+0x9cc>
    9a64:	e59d3038 	ldr	r3, [sp, #56]	; 0x38
    9a68:	e59dc038 	ldr	ip, [sp, #56]	; 0x38
    9a6c:	e5933000 	ldr	r3, [r3]
    9a70:	e28cc004 	add	ip, ip, #4
    9a74:	e59db018 	ldr	fp, [sp, #24]
    9a78:	e3a02030 	mov	r2, #48	; 0x30
    9a7c:	e58dc038 	str	ip, [sp, #56]	; 0x38
    9a80:	e59fc818 	ldr	ip, [pc, #2072]	; a2a0 <_vfprintf_r+0x1384>
    9a84:	e3a07078 	mov	r7, #120	; 0x78
    9a88:	e38bb002 	orr	fp, fp, #2
    9a8c:	e5cd2084 	strb	r2, [sp, #132]	; 0x84
    9a90:	e2932000 	adds	r2, r3, #0
    9a94:	e58d0028 	str	r0, [sp, #40]	; 0x28
    9a98:	e58db018 	str	fp, [sp, #24]
    9a9c:	e5cd7085 	strb	r7, [sp, #133]	; 0x85
    9aa0:	13a02001 	movne	r2, #1
    9aa4:	e58dc074 	str	ip, [sp, #116]	; 0x74
    9aa8:	e3a01002 	mov	r1, #2
    9aac:	eaffff1a 	b	971c <_vfprintf_r+0x800>
    9ab0:	e59d50ac 	ldr	r5, [sp, #172]	; 0xac
    9ab4:	e5dd3083 	ldrb	r3, [sp, #131]	; 0x83
    9ab8:	e3530000 	cmp	r3, #0
    9abc:	0a00000c 	beq	9af4 <_vfprintf_r+0xbd8>
    9ac0:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    9ac4:	e28d3083 	add	r3, sp, #131	; 0x83
    9ac8:	e2844001 	add	r4, r4, #1
    9acc:	e58a3000 	str	r3, [sl]
    9ad0:	e2855001 	add	r5, r5, #1
    9ad4:	e3540007 	cmp	r4, #7
    9ad8:	e3a03001 	mov	r3, #1
    9adc:	e58a3004 	str	r3, [sl, #4]
    9ae0:	e28db090 	add	fp, sp, #144	; 0x90
    9ae4:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    9ae8:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    9aec:	d28aa008 	addle	sl, sl, #8
    9af0:	ca00017a 	bgt	a0e0 <_vfprintf_r+0x11c4>
    9af4:	e59d3048 	ldr	r3, [sp, #72]	; 0x48
    9af8:	e3530000 	cmp	r3, #0
    9afc:	0a00000c 	beq	9b34 <_vfprintf_r+0xc18>
    9b00:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    9b04:	e28d3084 	add	r3, sp, #132	; 0x84
    9b08:	e2844001 	add	r4, r4, #1
    9b0c:	e58a3000 	str	r3, [sl]
    9b10:	e2855002 	add	r5, r5, #2
    9b14:	e3540007 	cmp	r4, #7
    9b18:	e3a03002 	mov	r3, #2
    9b1c:	e58a3004 	str	r3, [sl, #4]
    9b20:	e28db090 	add	fp, sp, #144	; 0x90
    9b24:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    9b28:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    9b2c:	d28aa008 	addle	sl, sl, #8
    9b30:	ca000161 	bgt	a0bc <_vfprintf_r+0x11a0>
    9b34:	e59d3050 	ldr	r3, [sp, #80]	; 0x50
    9b38:	e3530080 	cmp	r3, #128	; 0x80
    9b3c:	0a0000e3 	beq	9ed0 <_vfprintf_r+0xfb4>
    9b40:	e59d4040 	ldr	r4, [sp, #64]	; 0x40
    9b44:	e59db03c 	ldr	fp, [sp, #60]	; 0x3c
    9b48:	e06b9004 	rsb	r9, fp, r4
    9b4c:	e3590000 	cmp	r9, #0
    9b50:	da000031 	ble	9c1c <_vfprintf_r+0xd00>
    9b54:	e3590010 	cmp	r9, #16
    9b58:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    9b5c:	e59f6748 	ldr	r6, [pc, #1864]	; a2ac <_vfprintf_r+0x1390>
    9b60:	da00001d 	ble	9bdc <_vfprintf_r+0xcc0>
    9b64:	e28d80a4 	add	r8, sp, #164	; 0xa4
    9b68:	e58d7040 	str	r7, [sp, #64]	; 0x40
    9b6c:	e3a0b010 	mov	fp, #16
    9b70:	e1a07008 	mov	r7, r8
    9b74:	e59d802c 	ldr	r8, [sp, #44]	; 0x2c
    9b78:	ea000002 	b	9b88 <_vfprintf_r+0xc6c>
    9b7c:	e2499010 	sub	r9, r9, #16
    9b80:	e3590010 	cmp	r9, #16
    9b84:	da000013 	ble	9bd8 <_vfprintf_r+0xcbc>
    9b88:	e2844001 	add	r4, r4, #1
    9b8c:	e3540007 	cmp	r4, #7
    9b90:	e2855010 	add	r5, r5, #16
    9b94:	e88a0840 	stm	sl, {r6, fp}
    9b98:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    9b9c:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    9ba0:	d28aa008 	addle	sl, sl, #8
    9ba4:	dafffff4 	ble	9b7c <_vfprintf_r+0xc60>
    9ba8:	e1a00008 	mov	r0, r8
    9bac:	e59d101c 	ldr	r1, [sp, #28]
    9bb0:	e1a02007 	mov	r2, r7
    9bb4:	eb00155f 	bl	f138 <__sprint_r>
    9bb8:	e3500000 	cmp	r0, #0
    9bbc:	1afffdb9 	bne	92a8 <_vfprintf_r+0x38c>
    9bc0:	e2499010 	sub	r9, r9, #16
    9bc4:	e28d40a8 	add	r4, sp, #168	; 0xa8
    9bc8:	e3590010 	cmp	r9, #16
    9bcc:	e8940030 	ldm	r4, {r4, r5}
    9bd0:	e28dad05 	add	sl, sp, #320	; 0x140
    9bd4:	caffffeb 	bgt	9b88 <_vfprintf_r+0xc6c>
    9bd8:	e59d7040 	ldr	r7, [sp, #64]	; 0x40
    9bdc:	e2844001 	add	r4, r4, #1
    9be0:	e0855009 	add	r5, r5, r9
    9be4:	e3540007 	cmp	r4, #7
    9be8:	e88a0240 	stm	sl, {r6, r9}
    9bec:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    9bf0:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    9bf4:	d28aa008 	addle	sl, sl, #8
    9bf8:	da000007 	ble	9c1c <_vfprintf_r+0xd00>
    9bfc:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    9c00:	e59d101c 	ldr	r1, [sp, #28]
    9c04:	e28d20a4 	add	r2, sp, #164	; 0xa4
    9c08:	eb00154a 	bl	f138 <__sprint_r>
    9c0c:	e3500000 	cmp	r0, #0
    9c10:	1afffda4 	bne	92a8 <_vfprintf_r+0x38c>
    9c14:	e59d50ac 	ldr	r5, [sp, #172]	; 0xac
    9c18:	e28dad05 	add	sl, sp, #320	; 0x140
    9c1c:	e59d4018 	ldr	r4, [sp, #24]
    9c20:	e3140c01 	tst	r4, #256	; 0x100
    9c24:	1a000052 	bne	9d74 <_vfprintf_r+0xe58>
    9c28:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    9c2c:	e59d703c 	ldr	r7, [sp, #60]	; 0x3c
    9c30:	e2844001 	add	r4, r4, #1
    9c34:	e59dc04c 	ldr	ip, [sp, #76]	; 0x4c
    9c38:	e0855007 	add	r5, r5, r7
    9c3c:	e3540007 	cmp	r4, #7
    9c40:	e58ac000 	str	ip, [sl]
    9c44:	e58a7004 	str	r7, [sl, #4]
    9c48:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    9c4c:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    9c50:	ca00010c 	bgt	a088 <_vfprintf_r+0x116c>
    9c54:	e28aa008 	add	sl, sl, #8
    9c58:	e59d4018 	ldr	r4, [sp, #24]
    9c5c:	e3140004 	tst	r4, #4
    9c60:	0a000036 	beq	9d40 <_vfprintf_r+0xe24>
    9c64:	e59db030 	ldr	fp, [sp, #48]	; 0x30
    9c68:	e59dc020 	ldr	ip, [sp, #32]
    9c6c:	e06c700b 	rsb	r7, ip, fp
    9c70:	e3570000 	cmp	r7, #0
    9c74:	da000031 	ble	9d40 <_vfprintf_r+0xe24>
    9c78:	e3570010 	cmp	r7, #16
    9c7c:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    9c80:	e59f9608 	ldr	r9, [pc, #1544]	; a290 <_vfprintf_r+0x1374>
    9c84:	da00001e 	ble	9d04 <_vfprintf_r+0xde8>
    9c88:	e28d60a4 	add	r6, sp, #164	; 0xa4
    9c8c:	e1a03006 	mov	r3, r6
    9c90:	e3a08010 	mov	r8, #16
    9c94:	e1a06009 	mov	r6, r9
    9c98:	e59db02c 	ldr	fp, [sp, #44]	; 0x2c
    9c9c:	e1a09003 	mov	r9, r3
    9ca0:	ea000002 	b	9cb0 <_vfprintf_r+0xd94>
    9ca4:	e2477010 	sub	r7, r7, #16
    9ca8:	e3570010 	cmp	r7, #16
    9cac:	da000013 	ble	9d00 <_vfprintf_r+0xde4>
    9cb0:	e2844001 	add	r4, r4, #1
    9cb4:	e3540007 	cmp	r4, #7
    9cb8:	e2855010 	add	r5, r5, #16
    9cbc:	e88a0140 	stm	sl, {r6, r8}
    9cc0:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    9cc4:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    9cc8:	d28aa008 	addle	sl, sl, #8
    9ccc:	dafffff4 	ble	9ca4 <_vfprintf_r+0xd88>
    9cd0:	e1a0000b 	mov	r0, fp
    9cd4:	e59d101c 	ldr	r1, [sp, #28]
    9cd8:	e1a02009 	mov	r2, r9
    9cdc:	eb001515 	bl	f138 <__sprint_r>
    9ce0:	e3500000 	cmp	r0, #0
    9ce4:	1afffd6f 	bne	92a8 <_vfprintf_r+0x38c>
    9ce8:	e2477010 	sub	r7, r7, #16
    9cec:	e28d40a8 	add	r4, sp, #168	; 0xa8
    9cf0:	e3570010 	cmp	r7, #16
    9cf4:	e8940030 	ldm	r4, {r4, r5}
    9cf8:	e28dad05 	add	sl, sp, #320	; 0x140
    9cfc:	caffffeb 	bgt	9cb0 <_vfprintf_r+0xd94>
    9d00:	e1a09006 	mov	r9, r6
    9d04:	e2844001 	add	r4, r4, #1
    9d08:	e0875005 	add	r5, r7, r5
    9d0c:	e3540007 	cmp	r4, #7
    9d10:	e58a9000 	str	r9, [sl]
    9d14:	e58a7004 	str	r7, [sl, #4]
    9d18:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    9d1c:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    9d20:	da000006 	ble	9d40 <_vfprintf_r+0xe24>
    9d24:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    9d28:	e59d101c 	ldr	r1, [sp, #28]
    9d2c:	e28d20a4 	add	r2, sp, #164	; 0xa4
    9d30:	eb001500 	bl	f138 <__sprint_r>
    9d34:	e3500000 	cmp	r0, #0
    9d38:	1afffd5a 	bne	92a8 <_vfprintf_r+0x38c>
    9d3c:	e59d50ac 	ldr	r5, [sp, #172]	; 0xac
    9d40:	e59d4034 	ldr	r4, [sp, #52]	; 0x34
    9d44:	e59d7020 	ldr	r7, [sp, #32]
    9d48:	e59db030 	ldr	fp, [sp, #48]	; 0x30
    9d4c:	e157000b 	cmp	r7, fp
    9d50:	a0844007 	addge	r4, r4, r7
    9d54:	b084400b 	addlt	r4, r4, fp
    9d58:	e3550000 	cmp	r5, #0
    9d5c:	e58d4034 	str	r4, [sp, #52]	; 0x34
    9d60:	1afffd4a 	bne	9290 <_vfprintf_r+0x374>
    9d64:	e3a03000 	mov	r3, #0
    9d68:	e58d30a8 	str	r3, [sp, #168]	; 0xa8
    9d6c:	e28dad05 	add	sl, sp, #320	; 0x140
    9d70:	eafffca1 	b	8ffc <_vfprintf_r+0xe0>
    9d74:	e3570065 	cmp	r7, #101	; 0x65
    9d78:	da00008c 	ble	9fb0 <_vfprintf_r+0x1094>
    9d7c:	e28d1068 	add	r1, sp, #104	; 0x68
    9d80:	e8910003 	ldm	r1, {r0, r1}
    9d84:	e3a02000 	mov	r2, #0
    9d88:	e3a03000 	mov	r3, #0
    9d8c:	eb002114 	bl	121e4 <__aeabi_dcmpeq>
    9d90:	e3500000 	cmp	r0, #0
    9d94:	0a0000da 	beq	a104 <_vfprintf_r+0x11e8>
    9d98:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    9d9c:	e59f3504 	ldr	r3, [pc, #1284]	; a2a8 <_vfprintf_r+0x138c>
    9da0:	e2844001 	add	r4, r4, #1
    9da4:	e58a3000 	str	r3, [sl]
    9da8:	e2855001 	add	r5, r5, #1
    9dac:	e3540007 	cmp	r4, #7
    9db0:	e3a03001 	mov	r3, #1
    9db4:	e58a3004 	str	r3, [sl, #4]
    9db8:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    9dbc:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    9dc0:	d28aa008 	addle	sl, sl, #8
    9dc4:	ca000292 	bgt	a814 <_vfprintf_r+0x18f8>
    9dc8:	e59d3088 	ldr	r3, [sp, #136]	; 0x88
    9dcc:	e59d7044 	ldr	r7, [sp, #68]	; 0x44
    9dd0:	e1530007 	cmp	r3, r7
    9dd4:	ba000002 	blt	9de4 <_vfprintf_r+0xec8>
    9dd8:	e59db018 	ldr	fp, [sp, #24]
    9ddc:	e31b0001 	tst	fp, #1
    9de0:	0affff9c 	beq	9c58 <_vfprintf_r+0xd3c>
    9de4:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    9de8:	e59dc064 	ldr	ip, [sp, #100]	; 0x64
    9dec:	e2844001 	add	r4, r4, #1
    9df0:	e59d7058 	ldr	r7, [sp, #88]	; 0x58
    9df4:	e085500c 	add	r5, r5, ip
    9df8:	e3540007 	cmp	r4, #7
    9dfc:	e88a1080 	stm	sl, {r7, ip}
    9e00:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    9e04:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    9e08:	d28aa008 	addle	sl, sl, #8
    9e0c:	ca0002f4 	bgt	a9e4 <_vfprintf_r+0x1ac8>
    9e10:	e59dc044 	ldr	ip, [sp, #68]	; 0x44
    9e14:	e24c8001 	sub	r8, ip, #1
    9e18:	e3580000 	cmp	r8, #0
    9e1c:	daffff8d 	ble	9c58 <_vfprintf_r+0xd3c>
    9e20:	e3580010 	cmp	r8, #16
    9e24:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    9e28:	e59f647c 	ldr	r6, [pc, #1148]	; a2ac <_vfprintf_r+0x1390>
    9e2c:	da000139 	ble	a318 <_vfprintf_r+0x13fc>
    9e30:	e28d70a4 	add	r7, sp, #164	; 0xa4
    9e34:	e1a03007 	mov	r3, r7
    9e38:	e3a09010 	mov	r9, #16
    9e3c:	e1a07006 	mov	r7, r6
    9e40:	e59db02c 	ldr	fp, [sp, #44]	; 0x2c
    9e44:	e1a06003 	mov	r6, r3
    9e48:	ea000002 	b	9e58 <_vfprintf_r+0xf3c>
    9e4c:	e2488010 	sub	r8, r8, #16
    9e50:	e3580010 	cmp	r8, #16
    9e54:	da00012e 	ble	a314 <_vfprintf_r+0x13f8>
    9e58:	e2844001 	add	r4, r4, #1
    9e5c:	e3540007 	cmp	r4, #7
    9e60:	e2855010 	add	r5, r5, #16
    9e64:	e88a0280 	stm	sl, {r7, r9}
    9e68:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    9e6c:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    9e70:	d28aa008 	addle	sl, sl, #8
    9e74:	dafffff4 	ble	9e4c <_vfprintf_r+0xf30>
    9e78:	e1a0000b 	mov	r0, fp
    9e7c:	e59d101c 	ldr	r1, [sp, #28]
    9e80:	e1a02006 	mov	r2, r6
    9e84:	eb0014ab 	bl	f138 <__sprint_r>
    9e88:	e3500000 	cmp	r0, #0
    9e8c:	1afffd05 	bne	92a8 <_vfprintf_r+0x38c>
    9e90:	e28d40a8 	add	r4, sp, #168	; 0xa8
    9e94:	e8940030 	ldm	r4, {r4, r5}
    9e98:	e28dad05 	add	sl, sp, #320	; 0x140
    9e9c:	eaffffea 	b	9e4c <_vfprintf_r+0xf30>
    9ea0:	e3510000 	cmp	r1, #0
    9ea4:	1a000080 	bne	a0ac <_vfprintf_r+0x1190>
    9ea8:	e59db018 	ldr	fp, [sp, #24]
    9eac:	e31b0001 	tst	fp, #1
    9eb0:	0a0000ce 	beq	a1f0 <_vfprintf_r+0x12d4>
    9eb4:	e59dc07c 	ldr	ip, [sp, #124]	; 0x7c
    9eb8:	e59db05c 	ldr	fp, [sp, #92]	; 0x5c
    9ebc:	e3a03030 	mov	r3, #48	; 0x30
    9ec0:	e5cd30d7 	strb	r3, [sp, #215]	; 0xd7
    9ec4:	e58dc03c 	str	ip, [sp, #60]	; 0x3c
    9ec8:	e58db04c 	str	fp, [sp, #76]	; 0x4c
    9ecc:	eafffd7c 	b	94c4 <_vfprintf_r+0x5a8>
    9ed0:	e59d4030 	ldr	r4, [sp, #48]	; 0x30
    9ed4:	e59db020 	ldr	fp, [sp, #32]
    9ed8:	e06b9004 	rsb	r9, fp, r4
    9edc:	e3590000 	cmp	r9, #0
    9ee0:	daffff16 	ble	9b40 <_vfprintf_r+0xc24>
    9ee4:	e3590010 	cmp	r9, #16
    9ee8:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    9eec:	e59f63b8 	ldr	r6, [pc, #952]	; a2ac <_vfprintf_r+0x1390>
    9ef0:	da00001d 	ble	9f6c <_vfprintf_r+0x1050>
    9ef4:	e28d80a4 	add	r8, sp, #164	; 0xa4
    9ef8:	e58d7048 	str	r7, [sp, #72]	; 0x48
    9efc:	e3a0b010 	mov	fp, #16
    9f00:	e1a07008 	mov	r7, r8
    9f04:	e59d802c 	ldr	r8, [sp, #44]	; 0x2c
    9f08:	ea000002 	b	9f18 <_vfprintf_r+0xffc>
    9f0c:	e2499010 	sub	r9, r9, #16
    9f10:	e3590010 	cmp	r9, #16
    9f14:	da000013 	ble	9f68 <_vfprintf_r+0x104c>
    9f18:	e2844001 	add	r4, r4, #1
    9f1c:	e3540007 	cmp	r4, #7
    9f20:	e2855010 	add	r5, r5, #16
    9f24:	e88a0840 	stm	sl, {r6, fp}
    9f28:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    9f2c:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    9f30:	d28aa008 	addle	sl, sl, #8
    9f34:	dafffff4 	ble	9f0c <_vfprintf_r+0xff0>
    9f38:	e1a00008 	mov	r0, r8
    9f3c:	e59d101c 	ldr	r1, [sp, #28]
    9f40:	e1a02007 	mov	r2, r7
    9f44:	eb00147b 	bl	f138 <__sprint_r>
    9f48:	e3500000 	cmp	r0, #0
    9f4c:	1afffcd5 	bne	92a8 <_vfprintf_r+0x38c>
    9f50:	e2499010 	sub	r9, r9, #16
    9f54:	e28d40a8 	add	r4, sp, #168	; 0xa8
    9f58:	e3590010 	cmp	r9, #16
    9f5c:	e8940030 	ldm	r4, {r4, r5}
    9f60:	e28dad05 	add	sl, sp, #320	; 0x140
    9f64:	caffffeb 	bgt	9f18 <_vfprintf_r+0xffc>
    9f68:	e59d7048 	ldr	r7, [sp, #72]	; 0x48
    9f6c:	e2844001 	add	r4, r4, #1
    9f70:	e0855009 	add	r5, r5, r9
    9f74:	e3540007 	cmp	r4, #7
    9f78:	e88a0240 	stm	sl, {r6, r9}
    9f7c:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    9f80:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    9f84:	d28aa008 	addle	sl, sl, #8
    9f88:	dafffeec 	ble	9b40 <_vfprintf_r+0xc24>
    9f8c:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    9f90:	e59d101c 	ldr	r1, [sp, #28]
    9f94:	e28d20a4 	add	r2, sp, #164	; 0xa4
    9f98:	eb001466 	bl	f138 <__sprint_r>
    9f9c:	e3500000 	cmp	r0, #0
    9fa0:	1afffcc0 	bne	92a8 <_vfprintf_r+0x38c>
    9fa4:	e59d50ac 	ldr	r5, [sp, #172]	; 0xac
    9fa8:	e28dad05 	add	sl, sp, #320	; 0x140
    9fac:	eafffee3 	b	9b40 <_vfprintf_r+0xc24>
    9fb0:	e59d4044 	ldr	r4, [sp, #68]	; 0x44
    9fb4:	e3540001 	cmp	r4, #1
    9fb8:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    9fbc:	e2855001 	add	r5, r5, #1
    9fc0:	e2844001 	add	r4, r4, #1
    9fc4:	da000144 	ble	a4dc <_vfprintf_r+0x15c0>
    9fc8:	e59dc04c 	ldr	ip, [sp, #76]	; 0x4c
    9fcc:	e3540007 	cmp	r4, #7
    9fd0:	e3a03001 	mov	r3, #1
    9fd4:	e58ac000 	str	ip, [sl]
    9fd8:	e58a3004 	str	r3, [sl, #4]
    9fdc:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    9fe0:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    9fe4:	d28aa008 	addle	sl, sl, #8
    9fe8:	ca000158 	bgt	a550 <_vfprintf_r+0x1634>
    9fec:	e59d7064 	ldr	r7, [sp, #100]	; 0x64
    9ff0:	e2844001 	add	r4, r4, #1
    9ff4:	e59db058 	ldr	fp, [sp, #88]	; 0x58
    9ff8:	e0855007 	add	r5, r5, r7
    9ffc:	e3540007 	cmp	r4, #7
    a000:	e58ab000 	str	fp, [sl]
    a004:	e58a7004 	str	r7, [sl, #4]
    a008:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a00c:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a010:	d28aa008 	addle	sl, sl, #8
    a014:	ca000143 	bgt	a528 <_vfprintf_r+0x160c>
    a018:	e28d1068 	add	r1, sp, #104	; 0x68
    a01c:	e8910003 	ldm	r1, {r0, r1}
    a020:	e3a02000 	mov	r2, #0
    a024:	e3a03000 	mov	r3, #0
    a028:	eb00206d 	bl	121e4 <__aeabi_dcmpeq>
    a02c:	e3500000 	cmp	r0, #0
    a030:	e59d7044 	ldr	r7, [sp, #68]	; 0x44
    a034:	1a000087 	bne	a258 <_vfprintf_r+0x133c>
    a038:	e59dc04c 	ldr	ip, [sp, #76]	; 0x4c
    a03c:	e2473001 	sub	r3, r7, #1
    a040:	e2844001 	add	r4, r4, #1
    a044:	e28c2001 	add	r2, ip, #1
    a048:	e0855003 	add	r5, r5, r3
    a04c:	e3540007 	cmp	r4, #7
    a050:	e88a000c 	stm	sl, {r2, r3}
    a054:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a058:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a05c:	ca00014d 	bgt	a598 <_vfprintf_r+0x167c>
    a060:	e28aa008 	add	sl, sl, #8
    a064:	e59dc070 	ldr	ip, [sp, #112]	; 0x70
    a068:	e2844001 	add	r4, r4, #1
    a06c:	e085500c 	add	r5, r5, ip
    a070:	e28d3094 	add	r3, sp, #148	; 0x94
    a074:	e3540007 	cmp	r4, #7
    a078:	e88a1008 	stm	sl, {r3, ip}
    a07c:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a080:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a084:	dafffef2 	ble	9c54 <_vfprintf_r+0xd38>
    a088:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    a08c:	e59d101c 	ldr	r1, [sp, #28]
    a090:	e28d20a4 	add	r2, sp, #164	; 0xa4
    a094:	eb001427 	bl	f138 <__sprint_r>
    a098:	e3500000 	cmp	r0, #0
    a09c:	1afffc81 	bne	92a8 <_vfprintf_r+0x38c>
    a0a0:	e28dad05 	add	sl, sp, #320	; 0x140
    a0a4:	e59d50ac 	ldr	r5, [sp, #172]	; 0xac
    a0a8:	eafffeea 	b	9c58 <_vfprintf_r+0xd3c>
    a0ac:	e59dc054 	ldr	ip, [sp, #84]	; 0x54
    a0b0:	e58d203c 	str	r2, [sp, #60]	; 0x3c
    a0b4:	e58dc04c 	str	ip, [sp, #76]	; 0x4c
    a0b8:	eafffd01 	b	94c4 <_vfprintf_r+0x5a8>
    a0bc:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    a0c0:	e59d101c 	ldr	r1, [sp, #28]
    a0c4:	e28d20a4 	add	r2, sp, #164	; 0xa4
    a0c8:	eb00141a 	bl	f138 <__sprint_r>
    a0cc:	e3500000 	cmp	r0, #0
    a0d0:	1afffc74 	bne	92a8 <_vfprintf_r+0x38c>
    a0d4:	e59d50ac 	ldr	r5, [sp, #172]	; 0xac
    a0d8:	e28dad05 	add	sl, sp, #320	; 0x140
    a0dc:	eafffe94 	b	9b34 <_vfprintf_r+0xc18>
    a0e0:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    a0e4:	e59d101c 	ldr	r1, [sp, #28]
    a0e8:	e28d20a4 	add	r2, sp, #164	; 0xa4
    a0ec:	eb001411 	bl	f138 <__sprint_r>
    a0f0:	e3500000 	cmp	r0, #0
    a0f4:	1afffc6b 	bne	92a8 <_vfprintf_r+0x38c>
    a0f8:	e59d50ac 	ldr	r5, [sp, #172]	; 0xac
    a0fc:	e28dad05 	add	sl, sp, #320	; 0x140
    a100:	eafffe7b 	b	9af4 <_vfprintf_r+0xbd8>
    a104:	e59d7088 	ldr	r7, [sp, #136]	; 0x88
    a108:	e3570000 	cmp	r7, #0
    a10c:	da0001ca 	ble	a83c <_vfprintf_r+0x1920>
    a110:	e59d6044 	ldr	r6, [sp, #68]	; 0x44
    a114:	e59d4060 	ldr	r4, [sp, #96]	; 0x60
    a118:	e59d704c 	ldr	r7, [sp, #76]	; 0x4c
    a11c:	e1560004 	cmp	r6, r4
    a120:	a1a06004 	movge	r6, r4
    a124:	e59dc044 	ldr	ip, [sp, #68]	; 0x44
    a128:	e3560000 	cmp	r6, #0
    a12c:	e087b00c 	add	fp, r7, ip
    a130:	da000009 	ble	a15c <_vfprintf_r+0x1240>
    a134:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    a138:	e2844001 	add	r4, r4, #1
    a13c:	e0855006 	add	r5, r5, r6
    a140:	e3540007 	cmp	r4, #7
    a144:	e58a7000 	str	r7, [sl]
    a148:	e58a6004 	str	r6, [sl, #4]
    a14c:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a150:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a154:	d28aa008 	addle	sl, sl, #8
    a158:	ca000248 	bgt	aa80 <_vfprintf_r+0x1b64>
    a15c:	e59d4060 	ldr	r4, [sp, #96]	; 0x60
    a160:	e1c66fc6 	bic	r6, r6, r6, asr #31
    a164:	e0668004 	rsb	r8, r6, r4
    a168:	e3580000 	cmp	r8, #0
    a16c:	da000083 	ble	a380 <_vfprintf_r+0x1464>
    a170:	e3580010 	cmp	r8, #16
    a174:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    a178:	e59f612c 	ldr	r6, [pc, #300]	; a2ac <_vfprintf_r+0x1390>
    a17c:	da00006f 	ble	a340 <_vfprintf_r+0x1424>
    a180:	e28d70a4 	add	r7, sp, #164	; 0xa4
    a184:	e58db03c 	str	fp, [sp, #60]	; 0x3c
    a188:	e3a09010 	mov	r9, #16
    a18c:	e1a0b008 	mov	fp, r8
    a190:	e1a08007 	mov	r8, r7
    a194:	e59d702c 	ldr	r7, [sp, #44]	; 0x2c
    a198:	ea000002 	b	a1a8 <_vfprintf_r+0x128c>
    a19c:	e24bb010 	sub	fp, fp, #16
    a1a0:	e35b0010 	cmp	fp, #16
    a1a4:	da000063 	ble	a338 <_vfprintf_r+0x141c>
    a1a8:	e2844001 	add	r4, r4, #1
    a1ac:	e3540007 	cmp	r4, #7
    a1b0:	e2855010 	add	r5, r5, #16
    a1b4:	e88a0240 	stm	sl, {r6, r9}
    a1b8:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a1bc:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a1c0:	d28aa008 	addle	sl, sl, #8
    a1c4:	dafffff4 	ble	a19c <_vfprintf_r+0x1280>
    a1c8:	e1a00007 	mov	r0, r7
    a1cc:	e59d101c 	ldr	r1, [sp, #28]
    a1d0:	e1a02008 	mov	r2, r8
    a1d4:	eb0013d7 	bl	f138 <__sprint_r>
    a1d8:	e3500000 	cmp	r0, #0
    a1dc:	1afffc31 	bne	92a8 <_vfprintf_r+0x38c>
    a1e0:	e28d40a8 	add	r4, sp, #168	; 0xa8
    a1e4:	e8940030 	ldm	r4, {r4, r5}
    a1e8:	e28dad05 	add	sl, sp, #320	; 0x140
    a1ec:	eaffffea 	b	a19c <_vfprintf_r+0x1280>
    a1f0:	e59db054 	ldr	fp, [sp, #84]	; 0x54
    a1f4:	e58d103c 	str	r1, [sp, #60]	; 0x3c
    a1f8:	e58db04c 	str	fp, [sp, #76]	; 0x4c
    a1fc:	eafffcb0 	b	94c4 <_vfprintf_r+0x5a8>
    a200:	e59dc074 	ldr	ip, [sp, #116]	; 0x74
    a204:	e203100f 	and	r1, r3, #15
    a208:	e7dc0001 	ldrb	r0, [ip, r1]
    a20c:	e1b03223 	lsrs	r3, r3, #4
    a210:	e1a01002 	mov	r1, r2
    a214:	e5c20000 	strb	r0, [r2]
    a218:	e2422001 	sub	r2, r2, #1
    a21c:	1afffff8 	bne	a204 <_vfprintf_r+0x12e8>
    a220:	e59dc054 	ldr	ip, [sp, #84]	; 0x54
    a224:	e061c00c 	rsb	ip, r1, ip
    a228:	e58d104c 	str	r1, [sp, #76]	; 0x4c
    a22c:	e58dc03c 	str	ip, [sp, #60]	; 0x3c
    a230:	eafffca3 	b	94c4 <_vfprintf_r+0x5a8>
    a234:	e3530009 	cmp	r3, #9
    a238:	8a0000e0 	bhi	a5c0 <_vfprintf_r+0x16a4>
    a23c:	e59db07c 	ldr	fp, [sp, #124]	; 0x7c
    a240:	e59dc05c 	ldr	ip, [sp, #92]	; 0x5c
    a244:	e2833030 	add	r3, r3, #48	; 0x30
    a248:	e5cd30d7 	strb	r3, [sp, #215]	; 0xd7
    a24c:	e58db03c 	str	fp, [sp, #60]	; 0x3c
    a250:	e58dc04c 	str	ip, [sp, #76]	; 0x4c
    a254:	eafffc9a 	b	94c4 <_vfprintf_r+0x5a8>
    a258:	e2478001 	sub	r8, r7, #1
    a25c:	e3580000 	cmp	r8, #0
    a260:	daffff7f 	ble	a064 <_vfprintf_r+0x1148>
    a264:	e3580010 	cmp	r8, #16
    a268:	e59f603c 	ldr	r6, [pc, #60]	; a2ac <_vfprintf_r+0x1390>
    a26c:	da0000c2 	ble	a57c <_vfprintf_r+0x1660>
    a270:	e28d70a4 	add	r7, sp, #164	; 0xa4
    a274:	e1a03007 	mov	r3, r7
    a278:	e3a09010 	mov	r9, #16
    a27c:	e1a07006 	mov	r7, r6
    a280:	e59db02c 	ldr	fp, [sp, #44]	; 0x2c
    a284:	e1a06003 	mov	r6, r3
    a288:	ea00000f 	b	a2cc <_vfprintf_r+0x13b0>
    a28c:	fffffba4 	.word	0xfffffba4
    a290:	00012304 	.word	0x00012304
    a294:	00012324 	.word	0x00012324
    a298:	00012328 	.word	0x00012328
    a29c:	00012334 	.word	0x00012334
    a2a0:	00012348 	.word	0x00012348
    a2a4:	cccccccd 	.word	0xcccccccd
    a2a8:	00012364 	.word	0x00012364
    a2ac:	00012314 	.word	0x00012314
    a2b0:	0001232c 	.word	0x0001232c
    a2b4:	00012330 	.word	0x00012330
    a2b8:	0001235c 	.word	0x0001235c
    a2bc:	66666667 	.word	0x66666667
    a2c0:	e2488010 	sub	r8, r8, #16
    a2c4:	e3580010 	cmp	r8, #16
    a2c8:	da0000aa 	ble	a578 <_vfprintf_r+0x165c>
    a2cc:	e2844001 	add	r4, r4, #1
    a2d0:	e3540007 	cmp	r4, #7
    a2d4:	e2855010 	add	r5, r5, #16
    a2d8:	e88a0280 	stm	sl, {r7, r9}
    a2dc:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a2e0:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a2e4:	d28aa008 	addle	sl, sl, #8
    a2e8:	dafffff4 	ble	a2c0 <_vfprintf_r+0x13a4>
    a2ec:	e1a0000b 	mov	r0, fp
    a2f0:	e59d101c 	ldr	r1, [sp, #28]
    a2f4:	e1a02006 	mov	r2, r6
    a2f8:	eb00138e 	bl	f138 <__sprint_r>
    a2fc:	e3500000 	cmp	r0, #0
    a300:	1afffbe8 	bne	92a8 <_vfprintf_r+0x38c>
    a304:	e28d40a8 	add	r4, sp, #168	; 0xa8
    a308:	e8940030 	ldm	r4, {r4, r5}
    a30c:	e28dad05 	add	sl, sp, #320	; 0x140
    a310:	eaffffea 	b	a2c0 <_vfprintf_r+0x13a4>
    a314:	e1a06007 	mov	r6, r7
    a318:	e2844001 	add	r4, r4, #1
    a31c:	e0855008 	add	r5, r5, r8
    a320:	e3540007 	cmp	r4, #7
    a324:	e88a0140 	stm	sl, {r6, r8}
    a328:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a32c:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a330:	dafffe47 	ble	9c54 <_vfprintf_r+0xd38>
    a334:	eaffff53 	b	a088 <_vfprintf_r+0x116c>
    a338:	e1a0800b 	mov	r8, fp
    a33c:	e59db03c 	ldr	fp, [sp, #60]	; 0x3c
    a340:	e2844001 	add	r4, r4, #1
    a344:	e0855008 	add	r5, r5, r8
    a348:	e3540007 	cmp	r4, #7
    a34c:	e88a0140 	stm	sl, {r6, r8}
    a350:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a354:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a358:	d28aa008 	addle	sl, sl, #8
    a35c:	da000007 	ble	a380 <_vfprintf_r+0x1464>
    a360:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    a364:	e59d101c 	ldr	r1, [sp, #28]
    a368:	e28d20a4 	add	r2, sp, #164	; 0xa4
    a36c:	eb001371 	bl	f138 <__sprint_r>
    a370:	e3500000 	cmp	r0, #0
    a374:	1afffbcb 	bne	92a8 <_vfprintf_r+0x38c>
    a378:	e59d50ac 	ldr	r5, [sp, #172]	; 0xac
    a37c:	e28dad05 	add	sl, sp, #320	; 0x140
    a380:	e59d4044 	ldr	r4, [sp, #68]	; 0x44
    a384:	e59d3088 	ldr	r3, [sp, #136]	; 0x88
    a388:	e59dc04c 	ldr	ip, [sp, #76]	; 0x4c
    a38c:	e1530004 	cmp	r3, r4
    a390:	e59d4060 	ldr	r4, [sp, #96]	; 0x60
    a394:	e08c7004 	add	r7, ip, r4
    a398:	ba000038 	blt	a480 <_vfprintf_r+0x1564>
    a39c:	e59dc018 	ldr	ip, [sp, #24]
    a3a0:	e31c0001 	tst	ip, #1
    a3a4:	1a000035 	bne	a480 <_vfprintf_r+0x1564>
    a3a8:	e59d4044 	ldr	r4, [sp, #68]	; 0x44
    a3ac:	e067600b 	rsb	r6, r7, fp
    a3b0:	e0633004 	rsb	r3, r3, r4
    a3b4:	e1530006 	cmp	r3, r6
    a3b8:	b1a06003 	movlt	r6, r3
    a3bc:	a1a06006 	movge	r6, r6
    a3c0:	e3560000 	cmp	r6, #0
    a3c4:	da000009 	ble	a3f0 <_vfprintf_r+0x14d4>
    a3c8:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    a3cc:	e2844001 	add	r4, r4, #1
    a3d0:	e0855006 	add	r5, r5, r6
    a3d4:	e3540007 	cmp	r4, #7
    a3d8:	e58a7000 	str	r7, [sl]
    a3dc:	e58a6004 	str	r6, [sl, #4]
    a3e0:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a3e4:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a3e8:	d28aa008 	addle	sl, sl, #8
    a3ec:	ca0001ac 	bgt	aaa4 <_vfprintf_r+0x1b88>
    a3f0:	e1c66fc6 	bic	r6, r6, r6, asr #31
    a3f4:	e0668003 	rsb	r8, r6, r3
    a3f8:	e3580000 	cmp	r8, #0
    a3fc:	dafffe15 	ble	9c58 <_vfprintf_r+0xd3c>
    a400:	e3580010 	cmp	r8, #16
    a404:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    a408:	e51f6164 	ldr	r6, [pc, #-356]	; a2ac <_vfprintf_r+0x1390>
    a40c:	daffffc1 	ble	a318 <_vfprintf_r+0x13fc>
    a410:	e28d70a4 	add	r7, sp, #164	; 0xa4
    a414:	e1a03007 	mov	r3, r7
    a418:	e3a09010 	mov	r9, #16
    a41c:	e1a07006 	mov	r7, r6
    a420:	e59db02c 	ldr	fp, [sp, #44]	; 0x2c
    a424:	e1a06003 	mov	r6, r3
    a428:	ea000002 	b	a438 <_vfprintf_r+0x151c>
    a42c:	e2488010 	sub	r8, r8, #16
    a430:	e3580010 	cmp	r8, #16
    a434:	daffffb6 	ble	a314 <_vfprintf_r+0x13f8>
    a438:	e2844001 	add	r4, r4, #1
    a43c:	e3540007 	cmp	r4, #7
    a440:	e2855010 	add	r5, r5, #16
    a444:	e88a0280 	stm	sl, {r7, r9}
    a448:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a44c:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a450:	d28aa008 	addle	sl, sl, #8
    a454:	dafffff4 	ble	a42c <_vfprintf_r+0x1510>
    a458:	e1a0000b 	mov	r0, fp
    a45c:	e59d101c 	ldr	r1, [sp, #28]
    a460:	e1a02006 	mov	r2, r6
    a464:	eb001333 	bl	f138 <__sprint_r>
    a468:	e3500000 	cmp	r0, #0
    a46c:	1afffb8d 	bne	92a8 <_vfprintf_r+0x38c>
    a470:	e28d40a8 	add	r4, sp, #168	; 0xa8
    a474:	e8940030 	ldm	r4, {r4, r5}
    a478:	e28dad05 	add	sl, sp, #320	; 0x140
    a47c:	eaffffea 	b	a42c <_vfprintf_r+0x1510>
    a480:	e59dc064 	ldr	ip, [sp, #100]	; 0x64
    a484:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    a488:	e085500c 	add	r5, r5, ip
    a48c:	e59dc058 	ldr	ip, [sp, #88]	; 0x58
    a490:	e2844001 	add	r4, r4, #1
    a494:	e58ac000 	str	ip, [sl]
    a498:	e59dc064 	ldr	ip, [sp, #100]	; 0x64
    a49c:	e3540007 	cmp	r4, #7
    a4a0:	e58ac004 	str	ip, [sl, #4]
    a4a4:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a4a8:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a4ac:	d28aa008 	addle	sl, sl, #8
    a4b0:	daffffbc 	ble	a3a8 <_vfprintf_r+0x148c>
    a4b4:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    a4b8:	e59d101c 	ldr	r1, [sp, #28]
    a4bc:	e28d20a4 	add	r2, sp, #164	; 0xa4
    a4c0:	eb00131c 	bl	f138 <__sprint_r>
    a4c4:	e3500000 	cmp	r0, #0
    a4c8:	1afffb76 	bne	92a8 <_vfprintf_r+0x38c>
    a4cc:	e59d3088 	ldr	r3, [sp, #136]	; 0x88
    a4d0:	e59d50ac 	ldr	r5, [sp, #172]	; 0xac
    a4d4:	e28dad05 	add	sl, sp, #320	; 0x140
    a4d8:	eaffffb2 	b	a3a8 <_vfprintf_r+0x148c>
    a4dc:	e59d7018 	ldr	r7, [sp, #24]
    a4e0:	e3170001 	tst	r7, #1
    a4e4:	1afffeb7 	bne	9fc8 <_vfprintf_r+0x10ac>
    a4e8:	e59d704c 	ldr	r7, [sp, #76]	; 0x4c
    a4ec:	e3a03001 	mov	r3, #1
    a4f0:	e3540007 	cmp	r4, #7
    a4f4:	e58a7000 	str	r7, [sl]
    a4f8:	e58a3004 	str	r3, [sl, #4]
    a4fc:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a500:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a504:	dafffed5 	ble	a060 <_vfprintf_r+0x1144>
    a508:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    a50c:	e59d101c 	ldr	r1, [sp, #28]
    a510:	e28d20a4 	add	r2, sp, #164	; 0xa4
    a514:	eb001307 	bl	f138 <__sprint_r>
    a518:	e3500000 	cmp	r0, #0
    a51c:	e28db0b0 	add	fp, sp, #176	; 0xb0
    a520:	0a000022 	beq	a5b0 <_vfprintf_r+0x1694>
    a524:	eafffb5f 	b	92a8 <_vfprintf_r+0x38c>
    a528:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    a52c:	e59d101c 	ldr	r1, [sp, #28]
    a530:	e28d20a4 	add	r2, sp, #164	; 0xa4
    a534:	eb0012ff 	bl	f138 <__sprint_r>
    a538:	e3500000 	cmp	r0, #0
    a53c:	1afffb59 	bne	92a8 <_vfprintf_r+0x38c>
    a540:	e28d40a8 	add	r4, sp, #168	; 0xa8
    a544:	e8940030 	ldm	r4, {r4, r5}
    a548:	e28dad05 	add	sl, sp, #320	; 0x140
    a54c:	eafffeb1 	b	a018 <_vfprintf_r+0x10fc>
    a550:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    a554:	e59d101c 	ldr	r1, [sp, #28]
    a558:	e28d20a4 	add	r2, sp, #164	; 0xa4
    a55c:	eb0012f5 	bl	f138 <__sprint_r>
    a560:	e3500000 	cmp	r0, #0
    a564:	1afffb4f 	bne	92a8 <_vfprintf_r+0x38c>
    a568:	e28d40a8 	add	r4, sp, #168	; 0xa8
    a56c:	e8940030 	ldm	r4, {r4, r5}
    a570:	e28dad05 	add	sl, sp, #320	; 0x140
    a574:	eafffe9c 	b	9fec <_vfprintf_r+0x10d0>
    a578:	e1a06007 	mov	r6, r7
    a57c:	e2844001 	add	r4, r4, #1
    a580:	e0855008 	add	r5, r5, r8
    a584:	e3540007 	cmp	r4, #7
    a588:	e88a0140 	stm	sl, {r6, r8}
    a58c:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a590:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a594:	dafffeb1 	ble	a060 <_vfprintf_r+0x1144>
    a598:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    a59c:	e59d101c 	ldr	r1, [sp, #28]
    a5a0:	e28d20a4 	add	r2, sp, #164	; 0xa4
    a5a4:	eb0012e3 	bl	f138 <__sprint_r>
    a5a8:	e3500000 	cmp	r0, #0
    a5ac:	1afffb3d 	bne	92a8 <_vfprintf_r+0x38c>
    a5b0:	e28d40a8 	add	r4, sp, #168	; 0xa8
    a5b4:	e28dad05 	add	sl, sp, #320	; 0x140
    a5b8:	e8940030 	ldm	r4, {r4, r5}
    a5bc:	eafffea8 	b	a064 <_vfprintf_r+0x1148>
    a5c0:	e51f0324 	ldr	r0, [pc, #-804]	; a2a4 <_vfprintf_r+0x1388>
    a5c4:	e59d105c 	ldr	r1, [sp, #92]	; 0x5c
    a5c8:	e082b390 	umull	fp, r2, r0, r3
    a5cc:	e1a021a2 	lsr	r2, r2, #3
    a5d0:	e082c102 	add	ip, r2, r2, lsl #2
    a5d4:	e043308c 	sub	r3, r3, ip, lsl #1
    a5d8:	e1a0c001 	mov	ip, r1
    a5dc:	e2831030 	add	r1, r3, #48	; 0x30
    a5e0:	e2523000 	subs	r3, r2, #0
    a5e4:	e5cc1000 	strb	r1, [ip]
    a5e8:	e24c1001 	sub	r1, ip, #1
    a5ec:	1afffff5 	bne	a5c8 <_vfprintf_r+0x16ac>
    a5f0:	e59db054 	ldr	fp, [sp, #84]	; 0x54
    a5f4:	e58dc04c 	str	ip, [sp, #76]	; 0x4c
    a5f8:	e06cc00b 	rsb	ip, ip, fp
    a5fc:	e58dc03c 	str	ip, [sp, #60]	; 0x3c
    a600:	eafffbaf 	b	94c4 <_vfprintf_r+0x5a8>
    a604:	e3510030 	cmp	r1, #48	; 0x30
    a608:	158d204c 	strne	r2, [sp, #76]	; 0x4c
    a60c:	e59d3054 	ldr	r3, [sp, #84]	; 0x54
    a610:	e59dc04c 	ldr	ip, [sp, #76]	; 0x4c
    a614:	13a02030 	movne	r2, #48	; 0x30
    a618:	e06c3003 	rsb	r3, ip, r3
    a61c:	15402001 	strbne	r2, [r0, #-1]
    a620:	e58d303c 	str	r3, [sp, #60]	; 0x3c
    a624:	eafffba6 	b	94c4 <_vfprintf_r+0x5a8>
    a628:	e3a0202d 	mov	r2, #45	; 0x2d
    a62c:	e2633000 	rsb	r3, r3, #0
    a630:	e5cd2083 	strb	r2, [sp, #131]	; 0x83
    a634:	e2932000 	adds	r2, r3, #0
    a638:	13a02001 	movne	r2, #1
    a63c:	e3a01001 	mov	r1, #1
    a640:	eafffb84 	b	9458 <_vfprintf_r+0x53c>
    a644:	eb0011c5 	bl	ed60 <__fpclassifyd>
    a648:	e3500000 	cmp	r0, #0
    a64c:	0a0000c3 	beq	a960 <_vfprintf_r+0x1a44>
    a650:	e3740001 	cmn	r4, #1
    a654:	e3c79020 	bic	r9, r7, #32
    a658:	03a04006 	moveq	r4, #6
    a65c:	0a000003 	beq	a670 <_vfprintf_r+0x1754>
    a660:	e3590047 	cmp	r9, #71	; 0x47
    a664:	1a000001 	bne	a670 <_vfprintf_r+0x1754>
    a668:	e3540000 	cmp	r4, #0
    a66c:	03a04001 	moveq	r4, #1
    a670:	e3560000 	cmp	r6, #0
    a674:	b2866102 	addlt	r6, r6, #-2147483648	; 0x80000000
    a678:	b3a0b02d 	movlt	fp, #45	; 0x2d
    a67c:	a3a0b000 	movge	fp, #0
    a680:	e2593046 	subs	r3, r9, #70	; 0x46
    a684:	e2735000 	rsbs	r5, r3, #0
    a688:	e59dc018 	ldr	ip, [sp, #24]
    a68c:	e0b55003 	adcs	r5, r5, r3
    a690:	e38ccc01 	orr	ip, ip, #256	; 0x100
    a694:	e3550000 	cmp	r5, #0
    a698:	e58dc040 	str	ip, [sp, #64]	; 0x40
    a69c:	1a000116 	bne	aafc <_vfprintf_r+0x1be0>
    a6a0:	e3590045 	cmp	r9, #69	; 0x45
    a6a4:	1a000174 	bne	ac7c <_vfprintf_r+0x1d60>
    a6a8:	e28d3090 	add	r3, sp, #144	; 0x90
    a6ac:	e58d8020 	str	r8, [sp, #32]
    a6b0:	e2845001 	add	r5, r4, #1
    a6b4:	e28d1088 	add	r1, sp, #136	; 0x88
    a6b8:	e28d208c 	add	r2, sp, #140	; 0x8c
    a6bc:	e58d6024 	str	r6, [sp, #36]	; 0x24
    a6c0:	e3a00002 	mov	r0, #2
    a6c4:	e88d0021 	stm	sp, {r0, r5}
    a6c8:	e58d200c 	str	r2, [sp, #12]
    a6cc:	e58d3010 	str	r3, [sp, #16]
    a6d0:	e58d1008 	str	r1, [sp, #8]
    a6d4:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    a6d8:	e28d3020 	add	r3, sp, #32
    a6dc:	e893000c 	ldm	r3, {r2, r3}
    a6e0:	eb000344 	bl	b3f8 <_dtoa_r>
    a6e4:	e58d004c 	str	r0, [sp, #76]	; 0x4c
    a6e8:	e0805005 	add	r5, r0, r5
    a6ec:	e1a00008 	mov	r0, r8
    a6f0:	e1a01006 	mov	r1, r6
    a6f4:	e3a02000 	mov	r2, #0
    a6f8:	e3a03000 	mov	r3, #0
    a6fc:	eb001eb8 	bl	121e4 <__aeabi_dcmpeq>
    a700:	e3500000 	cmp	r0, #0
    a704:	1a000008 	bne	a72c <_vfprintf_r+0x1810>
    a708:	e59d3090 	ldr	r3, [sp, #144]	; 0x90
    a70c:	e1550003 	cmp	r5, r3
    a710:	91a05003 	movls	r5, r3
    a714:	9a000004 	bls	a72c <_vfprintf_r+0x1810>
    a718:	e3a02030 	mov	r2, #48	; 0x30
    a71c:	e4c32001 	strb	r2, [r3], #1
    a720:	e1530005 	cmp	r3, r5
    a724:	e58d3090 	str	r3, [sp, #144]	; 0x90
    a728:	1afffffb 	bne	a71c <_vfprintf_r+0x1800>
    a72c:	e59dc04c 	ldr	ip, [sp, #76]	; 0x4c
    a730:	e3590047 	cmp	r9, #71	; 0x47
    a734:	e06c5005 	rsb	r5, ip, r5
    a738:	e58d5044 	str	r5, [sp, #68]	; 0x44
    a73c:	0a000114 	beq	ab94 <_vfprintf_r+0x1c78>
    a740:	e3570065 	cmp	r7, #101	; 0x65
    a744:	da0001b0 	ble	ae0c <_vfprintf_r+0x1ef0>
    a748:	e3570066 	cmp	r7, #102	; 0x66
    a74c:	e59d3088 	ldr	r3, [sp, #136]	; 0x88
    a750:	0a000155 	beq	acac <_vfprintf_r+0x1d90>
    a754:	e59d7044 	ldr	r7, [sp, #68]	; 0x44
    a758:	e1530007 	cmp	r3, r7
    a75c:	ba000148 	blt	ac84 <_vfprintf_r+0x1d68>
    a760:	e59dc018 	ldr	ip, [sp, #24]
    a764:	e31c0001 	tst	ip, #1
    a768:	12837001 	addne	r7, r3, #1
    a76c:	11c7cfc7 	bicne	ip, r7, r7, asr #31
    a770:	01c34fc3 	biceq	r4, r3, r3, asr #31
    a774:	058d4020 	streq	r4, [sp, #32]
    a778:	058d303c 	streq	r3, [sp, #60]	; 0x3c
    a77c:	158dc020 	strne	ip, [sp, #32]
    a780:	158d703c 	strne	r7, [sp, #60]	; 0x3c
    a784:	e3a07067 	mov	r7, #103	; 0x67
    a788:	e58d3060 	str	r3, [sp, #96]	; 0x60
    a78c:	e35b0000 	cmp	fp, #0
    a790:	0a0000f7 	beq	ab74 <_vfprintf_r+0x1c58>
    a794:	e59d4040 	ldr	r4, [sp, #64]	; 0x40
    a798:	e3a0302d 	mov	r3, #45	; 0x2d
    a79c:	e3a0b000 	mov	fp, #0
    a7a0:	e5cd3083 	strb	r3, [sp, #131]	; 0x83
    a7a4:	e58d4018 	str	r4, [sp, #24]
    a7a8:	e58db040 	str	fp, [sp, #64]	; 0x40
    a7ac:	eafffb4e 	b	94ec <_vfprintf_r+0x5d0>
    a7b0:	e59db038 	ldr	fp, [sp, #56]	; 0x38
    a7b4:	e59b3000 	ldr	r3, [fp]
    a7b8:	e28bb004 	add	fp, fp, #4
    a7bc:	e2932000 	adds	r2, r3, #0
    a7c0:	13a02001 	movne	r2, #1
    a7c4:	e58db038 	str	fp, [sp, #56]	; 0x38
    a7c8:	eafffbd3 	b	971c <_vfprintf_r+0x800>
    a7cc:	e59dc038 	ldr	ip, [sp, #56]	; 0x38
    a7d0:	e59c3000 	ldr	r3, [ip]
    a7d4:	e28cc004 	add	ip, ip, #4
    a7d8:	e2932000 	adds	r2, r3, #0
    a7dc:	e3a01001 	mov	r1, #1
    a7e0:	13a02001 	movne	r2, #1
    a7e4:	e58dc038 	str	ip, [sp, #56]	; 0x38
    a7e8:	eafffbcb 	b	971c <_vfprintf_r+0x800>
    a7ec:	e59db018 	ldr	fp, [sp, #24]
    a7f0:	e31b0040 	tst	fp, #64	; 0x40
    a7f4:	0a000052 	beq	a944 <_vfprintf_r+0x1a28>
    a7f8:	e59dc038 	ldr	ip, [sp, #56]	; 0x38
    a7fc:	e59d4034 	ldr	r4, [sp, #52]	; 0x34
    a800:	e59c3000 	ldr	r3, [ip]
    a804:	e28cc004 	add	ip, ip, #4
    a808:	e58dc038 	str	ip, [sp, #56]	; 0x38
    a80c:	e1c340b0 	strh	r4, [r3]
    a810:	eafff9f9 	b	8ffc <_vfprintf_r+0xe0>
    a814:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    a818:	e59d101c 	ldr	r1, [sp, #28]
    a81c:	e28d20a4 	add	r2, sp, #164	; 0xa4
    a820:	eb001244 	bl	f138 <__sprint_r>
    a824:	e3500000 	cmp	r0, #0
    a828:	e28d40b0 	add	r4, sp, #176	; 0xb0
    a82c:	1afffa9d 	bne	92a8 <_vfprintf_r+0x38c>
    a830:	e59d50ac 	ldr	r5, [sp, #172]	; 0xac
    a834:	e28dad05 	add	sl, sp, #320	; 0x140
    a838:	eafffd62 	b	9dc8 <_vfprintf_r+0xeac>
    a83c:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    a840:	e51f35a0 	ldr	r3, [pc, #-1440]	; a2a8 <_vfprintf_r+0x138c>
    a844:	e2844001 	add	r4, r4, #1
    a848:	e58a3000 	str	r3, [sl]
    a84c:	e2855001 	add	r5, r5, #1
    a850:	e3540007 	cmp	r4, #7
    a854:	e3a03001 	mov	r3, #1
    a858:	e58a3004 	str	r3, [sl, #4]
    a85c:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a860:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a864:	d28aa008 	addle	sl, sl, #8
    a868:	ca000052 	bgt	a9b8 <_vfprintf_r+0x1a9c>
    a86c:	e3570000 	cmp	r7, #0
    a870:	0a000049 	beq	a99c <_vfprintf_r+0x1a80>
    a874:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    a878:	e59db064 	ldr	fp, [sp, #100]	; 0x64
    a87c:	e2844001 	add	r4, r4, #1
    a880:	e59dc058 	ldr	ip, [sp, #88]	; 0x58
    a884:	e085500b 	add	r5, r5, fp
    a888:	e3540007 	cmp	r4, #7
    a88c:	e58ac000 	str	ip, [sl]
    a890:	e58ab004 	str	fp, [sl, #4]
    a894:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a898:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a89c:	d28aa008 	addle	sl, sl, #8
    a8a0:	ca0000ea 	bgt	ac50 <_vfprintf_r+0x1d34>
    a8a4:	e2677000 	rsb	r7, r7, #0
    a8a8:	e3570000 	cmp	r7, #0
    a8ac:	da000067 	ble	aa50 <_vfprintf_r+0x1b34>
    a8b0:	e3570010 	cmp	r7, #16
    a8b4:	e51f6610 	ldr	r6, [pc, #-1552]	; a2ac <_vfprintf_r+0x1390>
    a8b8:	da000053 	ble	aa0c <_vfprintf_r+0x1af0>
    a8bc:	e3a08010 	mov	r8, #16
    a8c0:	e28d90a4 	add	r9, sp, #164	; 0xa4
    a8c4:	e59db02c 	ldr	fp, [sp, #44]	; 0x2c
    a8c8:	ea000002 	b	a8d8 <_vfprintf_r+0x19bc>
    a8cc:	e2477010 	sub	r7, r7, #16
    a8d0:	e3570010 	cmp	r7, #16
    a8d4:	da00004c 	ble	aa0c <_vfprintf_r+0x1af0>
    a8d8:	e2844001 	add	r4, r4, #1
    a8dc:	e3540007 	cmp	r4, #7
    a8e0:	e2855010 	add	r5, r5, #16
    a8e4:	e88a0140 	stm	sl, {r6, r8}
    a8e8:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    a8ec:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    a8f0:	d28aa008 	addle	sl, sl, #8
    a8f4:	dafffff4 	ble	a8cc <_vfprintf_r+0x19b0>
    a8f8:	e1a0000b 	mov	r0, fp
    a8fc:	e59d101c 	ldr	r1, [sp, #28]
    a900:	e1a02009 	mov	r2, r9
    a904:	eb00120b 	bl	f138 <__sprint_r>
    a908:	e3500000 	cmp	r0, #0
    a90c:	1afffa65 	bne	92a8 <_vfprintf_r+0x38c>
    a910:	e28d40a8 	add	r4, sp, #168	; 0xa8
    a914:	e8940030 	ldm	r4, {r4, r5}
    a918:	e28dad05 	add	sl, sp, #320	; 0x140
    a91c:	eaffffea 	b	a8cc <_vfprintf_r+0x19b0>
    a920:	e59d30ac 	ldr	r3, [sp, #172]	; 0xac
    a924:	e3530000 	cmp	r3, #0
    a928:	e59d901c 	ldr	r9, [sp, #28]
    a92c:	0afffa5e 	beq	92ac <_vfprintf_r+0x390>
    a930:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    a934:	e1a01009 	mov	r1, r9
    a938:	e28d20a4 	add	r2, sp, #164	; 0xa4
    a93c:	eb0011fd 	bl	f138 <__sprint_r>
    a940:	eafffa59 	b	92ac <_vfprintf_r+0x390>
    a944:	e59d7038 	ldr	r7, [sp, #56]	; 0x38
    a948:	e59db034 	ldr	fp, [sp, #52]	; 0x34
    a94c:	e5973000 	ldr	r3, [r7]
    a950:	e2877004 	add	r7, r7, #4
    a954:	e58d7038 	str	r7, [sp, #56]	; 0x38
    a958:	e583b000 	str	fp, [r3]
    a95c:	eafff9a6 	b	8ffc <_vfprintf_r+0xe0>
    a960:	e51f36b8 	ldr	r3, [pc, #-1720]	; a2b0 <_vfprintf_r+0x1394>
    a964:	e59db018 	ldr	fp, [sp, #24]
    a968:	e51f26bc 	ldr	r2, [pc, #-1724]	; a2b4 <_vfprintf_r+0x1398>
    a96c:	e3570047 	cmp	r7, #71	; 0x47
    a970:	e3a04003 	mov	r4, #3
    a974:	d1a02003 	movle	r2, r3
    a978:	e3cbb080 	bic	fp, fp, #128	; 0x80
    a97c:	e58d4020 	str	r4, [sp, #32]
    a980:	e58d0040 	str	r0, [sp, #64]	; 0x40
    a984:	e58d204c 	str	r2, [sp, #76]	; 0x4c
    a988:	e58db018 	str	fp, [sp, #24]
    a98c:	e5dd3083 	ldrb	r3, [sp, #131]	; 0x83
    a990:	e58d403c 	str	r4, [sp, #60]	; 0x3c
    a994:	e58d0060 	str	r0, [sp, #96]	; 0x60
    a998:	eafffad1 	b	94e4 <_vfprintf_r+0x5c8>
    a99c:	e59db044 	ldr	fp, [sp, #68]	; 0x44
    a9a0:	e35b0000 	cmp	fp, #0
    a9a4:	1affffb2 	bne	a874 <_vfprintf_r+0x1958>
    a9a8:	e59dc018 	ldr	ip, [sp, #24]
    a9ac:	e31c0001 	tst	ip, #1
    a9b0:	0afffca8 	beq	9c58 <_vfprintf_r+0xd3c>
    a9b4:	eaffffae 	b	a874 <_vfprintf_r+0x1958>
    a9b8:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    a9bc:	e59d101c 	ldr	r1, [sp, #28]
    a9c0:	e28d20a4 	add	r2, sp, #164	; 0xa4
    a9c4:	eb0011db 	bl	f138 <__sprint_r>
    a9c8:	e3500000 	cmp	r0, #0
    a9cc:	e28d40b0 	add	r4, sp, #176	; 0xb0
    a9d0:	1afffa34 	bne	92a8 <_vfprintf_r+0x38c>
    a9d4:	e59d7088 	ldr	r7, [sp, #136]	; 0x88
    a9d8:	e59d50ac 	ldr	r5, [sp, #172]	; 0xac
    a9dc:	e28dad05 	add	sl, sp, #320	; 0x140
    a9e0:	eaffffa1 	b	a86c <_vfprintf_r+0x1950>
    a9e4:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    a9e8:	e59d101c 	ldr	r1, [sp, #28]
    a9ec:	e28d20a4 	add	r2, sp, #164	; 0xa4
    a9f0:	eb0011d0 	bl	f138 <__sprint_r>
    a9f4:	e3500000 	cmp	r0, #0
    a9f8:	e28db0b0 	add	fp, sp, #176	; 0xb0
    a9fc:	1afffa29 	bne	92a8 <_vfprintf_r+0x38c>
    aa00:	e59d50ac 	ldr	r5, [sp, #172]	; 0xac
    aa04:	e28dad05 	add	sl, sp, #320	; 0x140
    aa08:	eafffd00 	b	9e10 <_vfprintf_r+0xef4>
    aa0c:	e2844001 	add	r4, r4, #1
    aa10:	e0855007 	add	r5, r5, r7
    aa14:	e3540007 	cmp	r4, #7
    aa18:	e88a00c0 	stm	sl, {r6, r7}
    aa1c:	e58d50ac 	str	r5, [sp, #172]	; 0xac
    aa20:	e58d40a8 	str	r4, [sp, #168]	; 0xa8
    aa24:	d28aa008 	addle	sl, sl, #8
    aa28:	da000008 	ble	aa50 <_vfprintf_r+0x1b34>
    aa2c:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    aa30:	e59d101c 	ldr	r1, [sp, #28]
    aa34:	e28d20a4 	add	r2, sp, #164	; 0xa4
    aa38:	eb0011be 	bl	f138 <__sprint_r>
    aa3c:	e3500000 	cmp	r0, #0
    aa40:	1afffa18 	bne	92a8 <_vfprintf_r+0x38c>
    aa44:	e28d40a8 	add	r4, sp, #168	; 0xa8
    aa48:	e8940030 	ldm	r4, {r4, r5}
    aa4c:	e28dad05 	add	sl, sp, #320	; 0x140
    aa50:	e2844001 	add	r4, r4, #1
    aa54:	e59d7044 	ldr	r7, [sp, #68]	; 0x44
    aa58:	eafffc75 	b	9c34 <_vfprintf_r+0xd18>
    aa5c:	eb00119d 	bl	f0d8 <strlen>
    aa60:	e1c04fc0 	bic	r4, r0, r0, asr #31
    aa64:	e58d003c 	str	r0, [sp, #60]	; 0x3c
    aa68:	e58d5040 	str	r5, [sp, #64]	; 0x40
    aa6c:	e58d4020 	str	r4, [sp, #32]
    aa70:	e5dd3083 	ldrb	r3, [sp, #131]	; 0x83
    aa74:	e58d6038 	str	r6, [sp, #56]	; 0x38
    aa78:	e58d5060 	str	r5, [sp, #96]	; 0x60
    aa7c:	eafffa98 	b	94e4 <_vfprintf_r+0x5c8>
    aa80:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    aa84:	e59d101c 	ldr	r1, [sp, #28]
    aa88:	e28d20a4 	add	r2, sp, #164	; 0xa4
    aa8c:	eb0011a9 	bl	f138 <__sprint_r>
    aa90:	e3500000 	cmp	r0, #0
    aa94:	1afffa03 	bne	92a8 <_vfprintf_r+0x38c>
    aa98:	e59d50ac 	ldr	r5, [sp, #172]	; 0xac
    aa9c:	e28dad05 	add	sl, sp, #320	; 0x140
    aaa0:	eafffdad 	b	a15c <_vfprintf_r+0x1240>
    aaa4:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    aaa8:	e59d101c 	ldr	r1, [sp, #28]
    aaac:	e28d20a4 	add	r2, sp, #164	; 0xa4
    aab0:	eb0011a0 	bl	f138 <__sprint_r>
    aab4:	e3500000 	cmp	r0, #0
    aab8:	e28d70b0 	add	r7, sp, #176	; 0xb0
    aabc:	1afff9f9 	bne	92a8 <_vfprintf_r+0x38c>
    aac0:	e59d3088 	ldr	r3, [sp, #136]	; 0x88
    aac4:	e59db044 	ldr	fp, [sp, #68]	; 0x44
    aac8:	e59d50ac 	ldr	r5, [sp, #172]	; 0xac
    aacc:	e063300b 	rsb	r3, r3, fp
    aad0:	e28dad05 	add	sl, sp, #320	; 0x140
    aad4:	eafffe45 	b	a3f0 <_vfprintf_r+0x14d4>
    aad8:	e3540006 	cmp	r4, #6
    aadc:	23a04006 	movcs	r4, #6
    aae0:	e1c4cfc4 	bic	ip, r4, r4, asr #31
    aae4:	e58d403c 	str	r4, [sp, #60]	; 0x3c
    aae8:	e51f4838 	ldr	r4, [pc, #-2104]	; a2b8 <_vfprintf_r+0x139c>
    aaec:	e58dc020 	str	ip, [sp, #32]
    aaf0:	e58d6038 	str	r6, [sp, #56]	; 0x38
    aaf4:	e58d404c 	str	r4, [sp, #76]	; 0x4c
    aaf8:	eafffb7a 	b	98e8 <_vfprintf_r+0x9cc>
    aafc:	e3a02003 	mov	r2, #3
    ab00:	e28d3090 	add	r3, sp, #144	; 0x90
    ab04:	e58d8020 	str	r8, [sp, #32]
    ab08:	e28d0088 	add	r0, sp, #136	; 0x88
    ab0c:	e28d108c 	add	r1, sp, #140	; 0x8c
    ab10:	e58d6024 	str	r6, [sp, #36]	; 0x24
    ab14:	e88d0014 	stm	sp, {r2, r4}
    ab18:	e58d0008 	str	r0, [sp, #8]
    ab1c:	e58d3010 	str	r3, [sp, #16]
    ab20:	e58d100c 	str	r1, [sp, #12]
    ab24:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    ab28:	e28d3020 	add	r3, sp, #32
    ab2c:	e893000c 	ldm	r3, {r2, r3}
    ab30:	eb000230 	bl	b3f8 <_dtoa_r>
    ab34:	e3590047 	cmp	r9, #71	; 0x47
    ab38:	e58d004c 	str	r0, [sp, #76]	; 0x4c
    ab3c:	1a000002 	bne	ab4c <_vfprintf_r+0x1c30>
    ab40:	e59dc018 	ldr	ip, [sp, #24]
    ab44:	e31c0001 	tst	ip, #1
    ab48:	0a00000e 	beq	ab88 <_vfprintf_r+0x1c6c>
    ab4c:	e59dc04c 	ldr	ip, [sp, #76]	; 0x4c
    ab50:	e3550000 	cmp	r5, #0
    ab54:	e08c5004 	add	r5, ip, r4
    ab58:	0afffee3 	beq	a6ec <_vfprintf_r+0x17d0>
    ab5c:	e5dc3000 	ldrb	r3, [ip]
    ab60:	e3530030 	cmp	r3, #48	; 0x30
    ab64:	0a000080 	beq	ad6c <_vfprintf_r+0x1e50>
    ab68:	e59d3088 	ldr	r3, [sp, #136]	; 0x88
    ab6c:	e0855003 	add	r5, r5, r3
    ab70:	eafffedd 	b	a6ec <_vfprintf_r+0x17d0>
    ab74:	e59d4040 	ldr	r4, [sp, #64]	; 0x40
    ab78:	e5dd3083 	ldrb	r3, [sp, #131]	; 0x83
    ab7c:	e58d4018 	str	r4, [sp, #24]
    ab80:	e58db040 	str	fp, [sp, #64]	; 0x40
    ab84:	eafffa56 	b	94e4 <_vfprintf_r+0x5c8>
    ab88:	e59d3090 	ldr	r3, [sp, #144]	; 0x90
    ab8c:	e0603003 	rsb	r3, r0, r3
    ab90:	e58d3044 	str	r3, [sp, #68]	; 0x44
    ab94:	e59d0088 	ldr	r0, [sp, #136]	; 0x88
    ab98:	e3700003 	cmn	r0, #3
    ab9c:	e1a03000 	mov	r3, r0
    aba0:	ba000001 	blt	abac <_vfprintf_r+0x1c90>
    aba4:	e1540000 	cmp	r4, r0
    aba8:	aafffee9 	bge	a754 <_vfprintf_r+0x1838>
    abac:	e2477002 	sub	r7, r7, #2
    abb0:	e2400001 	sub	r0, r0, #1
    abb4:	e3500000 	cmp	r0, #0
    abb8:	e58d0088 	str	r0, [sp, #136]	; 0x88
    abbc:	b2600000 	rsblt	r0, r0, #0
    abc0:	b3a0302d 	movlt	r3, #45	; 0x2d
    abc4:	a3a0302b 	movge	r3, #43	; 0x2b
    abc8:	e3500009 	cmp	r0, #9
    abcc:	e5cd7094 	strb	r7, [sp, #148]	; 0x94
    abd0:	e5cd3095 	strb	r3, [sp, #149]	; 0x95
    abd4:	ca00003f 	bgt	acd8 <_vfprintf_r+0x1dbc>
    abd8:	e2800030 	add	r0, r0, #48	; 0x30
    abdc:	e3a02030 	mov	r2, #48	; 0x30
    abe0:	e28d30a0 	add	r3, sp, #160	; 0xa0
    abe4:	e5cd0097 	strb	r0, [sp, #151]	; 0x97
    abe8:	e5cd2096 	strb	r2, [sp, #150]	; 0x96
    abec:	e2432008 	sub	r2, r3, #8
    abf0:	e59dc044 	ldr	ip, [sp, #68]	; 0x44
    abf4:	e243300c 	sub	r3, r3, #12
    abf8:	e0633002 	rsb	r3, r3, r2
    abfc:	e58d3070 	str	r3, [sp, #112]	; 0x70
    ac00:	e35c0001 	cmp	ip, #1
    ac04:	e08c3003 	add	r3, ip, r3
    ac08:	e58d303c 	str	r3, [sp, #60]	; 0x3c
    ac0c:	da000071 	ble	add8 <_vfprintf_r+0x1ebc>
    ac10:	e59d303c 	ldr	r3, [sp, #60]	; 0x3c
    ac14:	e2833001 	add	r3, r3, #1
    ac18:	e1c34fc3 	bic	r4, r3, r3, asr #31
    ac1c:	e3a0c000 	mov	ip, #0
    ac20:	e58d303c 	str	r3, [sp, #60]	; 0x3c
    ac24:	e58d4020 	str	r4, [sp, #32]
    ac28:	e58dc060 	str	ip, [sp, #96]	; 0x60
    ac2c:	eafffed6 	b	a78c <_vfprintf_r+0x1870>
    ac30:	e1c4bfc4 	bic	fp, r4, r4, asr #31
    ac34:	e58d0040 	str	r0, [sp, #64]	; 0x40
    ac38:	e58db020 	str	fp, [sp, #32]
    ac3c:	e5dd3083 	ldrb	r3, [sp, #131]	; 0x83
    ac40:	e58d6038 	str	r6, [sp, #56]	; 0x38
    ac44:	e58d403c 	str	r4, [sp, #60]	; 0x3c
    ac48:	e58d0060 	str	r0, [sp, #96]	; 0x60
    ac4c:	eafffa24 	b	94e4 <_vfprintf_r+0x5c8>
    ac50:	e59d002c 	ldr	r0, [sp, #44]	; 0x2c
    ac54:	e59d101c 	ldr	r1, [sp, #28]
    ac58:	e28d20a4 	add	r2, sp, #164	; 0xa4
    ac5c:	eb001135 	bl	f138 <__sprint_r>
    ac60:	e3500000 	cmp	r0, #0
    ac64:	1afff98f 	bne	92a8 <_vfprintf_r+0x38c>
    ac68:	e28d40a8 	add	r4, sp, #168	; 0xa8
    ac6c:	e59d7088 	ldr	r7, [sp, #136]	; 0x88
    ac70:	e8940030 	ldm	r4, {r4, r5}
    ac74:	e28dad05 	add	sl, sp, #320	; 0x140
    ac78:	eaffff09 	b	a8a4 <_vfprintf_r+0x1988>
    ac7c:	e3a02002 	mov	r2, #2
    ac80:	eaffff9e 	b	ab00 <_vfprintf_r+0x1be4>
    ac84:	e3530000 	cmp	r3, #0
    ac88:	e59d4044 	ldr	r4, [sp, #68]	; 0x44
    ac8c:	d2632002 	rsble	r2, r3, #2
    ac90:	c3a02001 	movgt	r2, #1
    ac94:	e0822004 	add	r2, r2, r4
    ac98:	e1c27fc2 	bic	r7, r2, r2, asr #31
    ac9c:	e58d7020 	str	r7, [sp, #32]
    aca0:	e58d203c 	str	r2, [sp, #60]	; 0x3c
    aca4:	e3a07067 	mov	r7, #103	; 0x67
    aca8:	eafffeb6 	b	a788 <_vfprintf_r+0x186c>
    acac:	e3530000 	cmp	r3, #0
    acb0:	da000037 	ble	ad94 <_vfprintf_r+0x1e78>
    acb4:	e3540000 	cmp	r4, #0
    acb8:	1a000025 	bne	ad54 <_vfprintf_r+0x1e38>
    acbc:	e59dc018 	ldr	ip, [sp, #24]
    acc0:	e31c0001 	tst	ip, #1
    acc4:	1a000022 	bne	ad54 <_vfprintf_r+0x1e38>
    acc8:	e1c34fc3 	bic	r4, r3, r3, asr #31
    accc:	e58d4020 	str	r4, [sp, #32]
    acd0:	e58d303c 	str	r3, [sp, #60]	; 0x3c
    acd4:	eafffeab 	b	a788 <_vfprintf_r+0x186c>
    acd8:	e28d30a0 	add	r3, sp, #160	; 0xa0
    acdc:	e2832002 	add	r2, r3, #2
    ace0:	e51f1a2c 	ldr	r1, [pc, #-2604]	; a2bc <_vfprintf_r+0x13a0>
    ace4:	e0cc4091 	smull	r4, ip, r1, r0
    ace8:	e1a01fc0 	asr	r1, r0, #31
    acec:	e061114c 	rsb	r1, r1, ip, asr #2
    acf0:	e081c101 	add	ip, r1, r1, lsl #2
    acf4:	e040c08c 	sub	ip, r0, ip, lsl #1
    acf8:	e1a00001 	mov	r0, r1
    acfc:	e3500009 	cmp	r0, #9
    ad00:	e1a01002 	mov	r1, r2
    ad04:	e28c2030 	add	r2, ip, #48	; 0x30
    ad08:	e5c12000 	strb	r2, [r1]
    ad0c:	e2412001 	sub	r2, r1, #1
    ad10:	cafffff2 	bgt	ace0 <_vfprintf_r+0x1dc4>
    ad14:	e2800030 	add	r0, r0, #48	; 0x30
    ad18:	e2834003 	add	r4, r3, #3
    ad1c:	e20000ff 	and	r0, r0, #255	; 0xff
    ad20:	e1540002 	cmp	r4, r2
    ad24:	e5410001 	strb	r0, [r1, #-1]
    ad28:	9a000039 	bls	ae14 <_vfprintf_r+0x1ef8>
    ad2c:	e243c00b 	sub	ip, r3, #11
    ad30:	e1a02001 	mov	r2, r1
    ad34:	ea000000 	b	ad3c <_vfprintf_r+0x1e20>
    ad38:	e4d20001 	ldrb	r0, [r2], #1
    ad3c:	e1520004 	cmp	r2, r4
    ad40:	e5ec0001 	strb	r0, [ip, #1]!
    ad44:	1afffffb 	bne	ad38 <_vfprintf_r+0x1e1c>
    ad48:	e0612083 	rsb	r2, r1, r3, lsl #1
    ad4c:	e2422006 	sub	r2, r2, #6
    ad50:	eaffffa6 	b	abf0 <_vfprintf_r+0x1cd4>
    ad54:	e2844001 	add	r4, r4, #1
    ad58:	e0834004 	add	r4, r3, r4
    ad5c:	e1c4cfc4 	bic	ip, r4, r4, asr #31
    ad60:	e58d403c 	str	r4, [sp, #60]	; 0x3c
    ad64:	e58dc020 	str	ip, [sp, #32]
    ad68:	eafffe86 	b	a788 <_vfprintf_r+0x186c>
    ad6c:	e3a03000 	mov	r3, #0
    ad70:	e1a00008 	mov	r0, r8
    ad74:	e1a01006 	mov	r1, r6
    ad78:	e3a02000 	mov	r2, #0
    ad7c:	eb001d18 	bl	121e4 <__aeabi_dcmpeq>
    ad80:	e3500000 	cmp	r0, #0
    ad84:	02643001 	rsbeq	r3, r4, #1
    ad88:	058d3088 	streq	r3, [sp, #136]	; 0x88
    ad8c:	0affff76 	beq	ab6c <_vfprintf_r+0x1c50>
    ad90:	eaffff74 	b	ab68 <_vfprintf_r+0x1c4c>
    ad94:	e3540000 	cmp	r4, #0
    ad98:	1a000016 	bne	adf8 <_vfprintf_r+0x1edc>
    ad9c:	e59dc018 	ldr	ip, [sp, #24]
    ada0:	e31c0001 	tst	ip, #1
    ada4:	1a000013 	bne	adf8 <_vfprintf_r+0x1edc>
    ada8:	e3a0c001 	mov	ip, #1
    adac:	e58dc020 	str	ip, [sp, #32]
    adb0:	e58dc03c 	str	ip, [sp, #60]	; 0x3c
    adb4:	eafffe73 	b	a788 <_vfprintf_r+0x186c>
    adb8:	e59db038 	ldr	fp, [sp, #56]	; 0x38
    adbc:	e59b4000 	ldr	r4, [fp]
    adc0:	e3540000 	cmp	r4, #0
    adc4:	e28b3004 	add	r3, fp, #4
    adc8:	aafffad9 	bge	9934 <_vfprintf_r+0xa18>
    adcc:	e58d3038 	str	r3, [sp, #56]	; 0x38
    add0:	e3e04000 	mvn	r4, #0
    add4:	eafff8b3 	b	90a8 <_vfprintf_r+0x18c>
    add8:	e59d4018 	ldr	r4, [sp, #24]
    addc:	e2143001 	ands	r3, r4, #1
    ade0:	1affff8a 	bne	ac10 <_vfprintf_r+0x1cf4>
    ade4:	e59dc03c 	ldr	ip, [sp, #60]	; 0x3c
    ade8:	e1cccfcc 	bic	ip, ip, ip, asr #31
    adec:	e58dc020 	str	ip, [sp, #32]
    adf0:	e58d3060 	str	r3, [sp, #96]	; 0x60
    adf4:	eafffe64 	b	a78c <_vfprintf_r+0x1870>
    adf8:	e2844002 	add	r4, r4, #2
    adfc:	e58d403c 	str	r4, [sp, #60]	; 0x3c
    ae00:	e1c44fc4 	bic	r4, r4, r4, asr #31
    ae04:	e58d4020 	str	r4, [sp, #32]
    ae08:	eafffe5e 	b	a788 <_vfprintf_r+0x186c>
    ae0c:	e59d0088 	ldr	r0, [sp, #136]	; 0x88
    ae10:	eaffff66 	b	abb0 <_vfprintf_r+0x1c94>
    ae14:	e243200a 	sub	r2, r3, #10
    ae18:	eaffff74 	b	abf0 <_vfprintf_r+0x1cd4>

0000ae1c <vfprintf>:
    ae1c:	e52d4004 	push	{r4}		; (str r4, [sp, #-4]!)
    ae20:	e1a03002 	mov	r3, r2
    ae24:	e59f2018 	ldr	r2, [pc, #24]	; ae44 <vfprintf+0x28>
    ae28:	e1a04000 	mov	r4, r0
    ae2c:	e1a0c001 	mov	ip, r1
    ae30:	e5920000 	ldr	r0, [r2]
    ae34:	e1a01004 	mov	r1, r4
    ae38:	e1a0200c 	mov	r2, ip
    ae3c:	e8bd0010 	pop	{r4}
    ae40:	eafff835 	b	8f1c <_vfprintf_r>
    ae44:	0001a600 	.word	0x0001a600

0000ae48 <__swsetup_r>:
    ae48:	e92d4038 	push	{r3, r4, r5, lr}
    ae4c:	e59f314c 	ldr	r3, [pc, #332]	; afa0 <__swsetup_r+0x158>
    ae50:	e5933000 	ldr	r3, [r3]
    ae54:	e3530000 	cmp	r3, #0
    ae58:	e1a05000 	mov	r5, r0
    ae5c:	e1a04001 	mov	r4, r1
    ae60:	0a000002 	beq	ae70 <__swsetup_r+0x28>
    ae64:	e5932038 	ldr	r2, [r3, #56]	; 0x38
    ae68:	e3520000 	cmp	r2, #0
    ae6c:	0a000031 	beq	af38 <__swsetup_r+0xf0>
    ae70:	e1d430bc 	ldrh	r3, [r4, #12]
    ae74:	e3130008 	tst	r3, #8
    ae78:	e1a02003 	mov	r2, r3
    ae7c:	0a00000c 	beq	aeb4 <__swsetup_r+0x6c>
    ae80:	e5942010 	ldr	r2, [r4, #16]
    ae84:	e3520000 	cmp	r2, #0
    ae88:	0a000012 	beq	aed8 <__swsetup_r+0x90>
    ae8c:	e2131001 	ands	r1, r3, #1
    ae90:	1a000019 	bne	aefc <__swsetup_r+0xb4>
    ae94:	e3130002 	tst	r3, #2
    ae98:	05941014 	ldreq	r1, [r4, #20]
    ae9c:	e3520000 	cmp	r2, #0
    aea0:	e5841008 	str	r1, [r4, #8]
    aea4:	13a00000 	movne	r0, #0
    aea8:	0a00001b 	beq	af1c <__swsetup_r+0xd4>
    aeac:	e8bd4038 	pop	{r3, r4, r5, lr}
    aeb0:	e12fff1e 	bx	lr
    aeb4:	e3130010 	tst	r3, #16
    aeb8:	0a000032 	beq	af88 <__swsetup_r+0x140>
    aebc:	e3120004 	tst	r2, #4
    aec0:	1a00001f 	bne	af44 <__swsetup_r+0xfc>
    aec4:	e5942010 	ldr	r2, [r4, #16]
    aec8:	e3833008 	orr	r3, r3, #8
    aecc:	e3520000 	cmp	r2, #0
    aed0:	e1c430bc 	strh	r3, [r4, #12]
    aed4:	1affffec 	bne	ae8c <__swsetup_r+0x44>
    aed8:	e2031d0a 	and	r1, r3, #640	; 0x280
    aedc:	e3510c02 	cmp	r1, #512	; 0x200
    aee0:	0affffe9 	beq	ae8c <__swsetup_r+0x44>
    aee4:	e1a00005 	mov	r0, r5
    aee8:	e1a01004 	mov	r1, r4
    aeec:	eb00099f 	bl	d570 <__smakebuf_r>
    aef0:	e1d430bc 	ldrh	r3, [r4, #12]
    aef4:	e5942010 	ldr	r2, [r4, #16]
    aef8:	eaffffe3 	b	ae8c <__swsetup_r+0x44>
    aefc:	e5943014 	ldr	r3, [r4, #20]
    af00:	e3a01000 	mov	r1, #0
    af04:	e2633000 	rsb	r3, r3, #0
    af08:	e3520000 	cmp	r2, #0
    af0c:	e5841008 	str	r1, [r4, #8]
    af10:	e5843018 	str	r3, [r4, #24]
    af14:	13a00000 	movne	r0, #0
    af18:	1affffe3 	bne	aeac <__swsetup_r+0x64>
    af1c:	e1d430bc 	ldrh	r3, [r4, #12]
    af20:	e3130080 	tst	r3, #128	; 0x80
    af24:	13833040 	orrne	r3, r3, #64	; 0x40
    af28:	11c430bc 	strhne	r3, [r4, #12]
    af2c:	13e00000 	mvnne	r0, #0
    af30:	01a00002 	moveq	r0, r2
    af34:	eaffffdc 	b	aeac <__swsetup_r+0x64>
    af38:	e1a00003 	mov	r0, r3
    af3c:	eb0007c7 	bl	ce60 <__sinit>
    af40:	eaffffca 	b	ae70 <__swsetup_r+0x28>
    af44:	e5941030 	ldr	r1, [r4, #48]	; 0x30
    af48:	e3510000 	cmp	r1, #0
    af4c:	0a000007 	beq	af70 <__swsetup_r+0x128>
    af50:	e2842040 	add	r2, r4, #64	; 0x40
    af54:	e1510002 	cmp	r1, r2
    af58:	0a000002 	beq	af68 <__swsetup_r+0x120>
    af5c:	e1a00005 	mov	r0, r5
    af60:	eb00086f 	bl	d124 <_free_r>
    af64:	e1d430bc 	ldrh	r3, [r4, #12]
    af68:	e3a02000 	mov	r2, #0
    af6c:	e5842030 	str	r2, [r4, #48]	; 0x30
    af70:	e5942010 	ldr	r2, [r4, #16]
    af74:	e3a01000 	mov	r1, #0
    af78:	e3c33024 	bic	r3, r3, #36	; 0x24
    af7c:	e5841004 	str	r1, [r4, #4]
    af80:	e5842000 	str	r2, [r4]
    af84:	eaffffcf 	b	aec8 <__swsetup_r+0x80>
    af88:	e3822040 	orr	r2, r2, #64	; 0x40
    af8c:	e3a03009 	mov	r3, #9
    af90:	e5853000 	str	r3, [r5]
    af94:	e3e00000 	mvn	r0, #0
    af98:	e1c420bc 	strh	r2, [r4, #12]
    af9c:	eaffffc2 	b	aeac <__swsetup_r+0x64>
    afa0:	0001a600 	.word	0x0001a600

0000afa4 <__register_exitproc>:
    afa4:	e59fc0e4 	ldr	ip, [pc, #228]	; b090 <__register_exitproc+0xec>
    afa8:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
    afac:	e59c4000 	ldr	r4, [ip]
    afb0:	e594c148 	ldr	ip, [r4, #328]	; 0x148
    afb4:	e35c0000 	cmp	ip, #0
    afb8:	0284cf53 	addeq	ip, r4, #332	; 0x14c
    afbc:	e59c5004 	ldr	r5, [ip, #4]
    afc0:	0584c148 	streq	ip, [r4, #328]	; 0x148
    afc4:	e355001f 	cmp	r5, #31
    afc8:	e24dd010 	sub	sp, sp, #16
    afcc:	e1a06000 	mov	r6, r0
    afd0:	da000015 	ble	b02c <__register_exitproc+0x88>
    afd4:	e59f00b8 	ldr	r0, [pc, #184]	; b094 <__register_exitproc+0xf0>
    afd8:	e3500000 	cmp	r0, #0
    afdc:	1a000001 	bne	afe8 <__register_exitproc+0x44>
    afe0:	e3e00000 	mvn	r0, #0
    afe4:	ea000018 	b	b04c <__register_exitproc+0xa8>
    afe8:	e3a00e19 	mov	r0, #400	; 0x190
    afec:	e58d100c 	str	r1, [sp, #12]
    aff0:	e58d2008 	str	r2, [sp, #8]
    aff4:	e58d3004 	str	r3, [sp, #4]
    aff8:	e1a00000 	nop			; (mov r0, r0)
    affc:	e250c000 	subs	ip, r0, #0
    b000:	e59d100c 	ldr	r1, [sp, #12]
    b004:	e59d2008 	ldr	r2, [sp, #8]
    b008:	e59d3004 	ldr	r3, [sp, #4]
    b00c:	0afffff3 	beq	afe0 <__register_exitproc+0x3c>
    b010:	e5945148 	ldr	r5, [r4, #328]	; 0x148
    b014:	e3a00000 	mov	r0, #0
    b018:	e58c0004 	str	r0, [ip, #4]
    b01c:	e58c5000 	str	r5, [ip]
    b020:	e584c148 	str	ip, [r4, #328]	; 0x148
    b024:	e58c0188 	str	r0, [ip, #392]	; 0x188
    b028:	e58c018c 	str	r0, [ip, #396]	; 0x18c
    b02c:	e3560000 	cmp	r6, #0
    b030:	e59c4004 	ldr	r4, [ip, #4]
    b034:	1a000007 	bne	b058 <__register_exitproc+0xb4>
    b038:	e2843002 	add	r3, r4, #2
    b03c:	e2844001 	add	r4, r4, #1
    b040:	e78c1103 	str	r1, [ip, r3, lsl #2]
    b044:	e58c4004 	str	r4, [ip, #4]
    b048:	e3a00000 	mov	r0, #0
    b04c:	e28dd010 	add	sp, sp, #16
    b050:	e8bd41f0 	pop	{r4, r5, r6, r7, r8, lr}
    b054:	e12fff1e 	bx	lr
    b058:	e3a00001 	mov	r0, #1
    b05c:	e1a00410 	lsl	r0, r0, r4
    b060:	e08c8104 	add	r8, ip, r4, lsl #2
    b064:	e3560002 	cmp	r6, #2
    b068:	e59c7188 	ldr	r7, [ip, #392]	; 0x188
    b06c:	e5883108 	str	r3, [r8, #264]	; 0x108
    b070:	059c318c 	ldreq	r3, [ip, #396]	; 0x18c
    b074:	e1877000 	orr	r7, r7, r0
    b078:	01830000 	orreq	r0, r3, r0
    b07c:	e1a05008 	mov	r5, r8
    b080:	e5882088 	str	r2, [r8, #136]	; 0x88
    b084:	e58c7188 	str	r7, [ip, #392]	; 0x188
    b088:	058c018c 	streq	r0, [ip, #396]	; 0x18c
    b08c:	eaffffe9 	b	b038 <__register_exitproc+0x94>
    b090:	000122f8 	.word	0x000122f8
    b094:	00000000 	.word	0x00000000

0000b098 <__call_exitprocs>:
    b098:	e92d4ff0 	push	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
    b09c:	e59f3168 	ldr	r3, [pc, #360]	; b20c <__call_exitprocs+0x174>
    b0a0:	e5933000 	ldr	r3, [r3]
    b0a4:	e24dd014 	sub	sp, sp, #20
    b0a8:	e58d3004 	str	r3, [sp, #4]
    b0ac:	e2833f52 	add	r3, r3, #328	; 0x148
    b0b0:	e58d0008 	str	r0, [sp, #8]
    b0b4:	e58d300c 	str	r3, [sp, #12]
    b0b8:	e1a06001 	mov	r6, r1
    b0bc:	e3a0a001 	mov	sl, #1
    b0c0:	e59d3004 	ldr	r3, [sp, #4]
    b0c4:	e5937148 	ldr	r7, [r3, #328]	; 0x148
    b0c8:	e3570000 	cmp	r7, #0
    b0cc:	e59db00c 	ldr	fp, [sp, #12]
    b0d0:	0a000033 	beq	b1a4 <__call_exitprocs+0x10c>
    b0d4:	e5975004 	ldr	r5, [r7, #4]
    b0d8:	e2554001 	subs	r4, r5, #1
    b0dc:	4a00002d 	bmi	b198 <__call_exitprocs+0x100>
    b0e0:	e2878088 	add	r8, r7, #136	; 0x88
    b0e4:	e285501f 	add	r5, r5, #31
    b0e8:	e0885105 	add	r5, r8, r5, lsl #2
    b0ec:	ea000006 	b	b10c <__call_exitprocs+0x74>
    b0f0:	e5953000 	ldr	r3, [r5]
    b0f4:	e1530006 	cmp	r3, r6
    b0f8:	0a000005 	beq	b114 <__call_exitprocs+0x7c>
    b0fc:	e2444001 	sub	r4, r4, #1
    b100:	e3740001 	cmn	r4, #1
    b104:	e2455004 	sub	r5, r5, #4
    b108:	0a000022 	beq	b198 <__call_exitprocs+0x100>
    b10c:	e3560000 	cmp	r6, #0
    b110:	1afffff6 	bne	b0f0 <__call_exitprocs+0x58>
    b114:	e5973004 	ldr	r3, [r7, #4]
    b118:	e0682005 	rsb	r2, r8, r5
    b11c:	e2433001 	sub	r3, r3, #1
    b120:	e0872002 	add	r2, r7, r2
    b124:	e1530004 	cmp	r3, r4
    b128:	e5123078 	ldr	r3, [r2, #-120]	; 0x78
    b12c:	13a01000 	movne	r1, #0
    b130:	05874004 	streq	r4, [r7, #4]
    b134:	15021078 	strne	r1, [r2, #-120]	; 0x78
    b138:	e3530000 	cmp	r3, #0
    b13c:	0affffee 	beq	b0fc <__call_exitprocs+0x64>
    b140:	e1a0241a 	lsl	r2, sl, r4
    b144:	e5971188 	ldr	r1, [r7, #392]	; 0x188
    b148:	e1120001 	tst	r2, r1
    b14c:	e5979004 	ldr	r9, [r7, #4]
    b150:	0a000016 	beq	b1b0 <__call_exitprocs+0x118>
    b154:	e597118c 	ldr	r1, [r7, #396]	; 0x18c
    b158:	e1120001 	tst	r2, r1
    b15c:	1a000016 	bne	b1bc <__call_exitprocs+0x124>
    b160:	e59d0008 	ldr	r0, [sp, #8]
    b164:	e5151080 	ldr	r1, [r5, #-128]	; 0x80
    b168:	e1a0e00f 	mov	lr, pc
    b16c:	e12fff13 	bx	r3
    b170:	e5973004 	ldr	r3, [r7, #4]
    b174:	e1530009 	cmp	r3, r9
    b178:	1affffd0 	bne	b0c0 <__call_exitprocs+0x28>
    b17c:	e59b3000 	ldr	r3, [fp]
    b180:	e1530007 	cmp	r3, r7
    b184:	1affffcd 	bne	b0c0 <__call_exitprocs+0x28>
    b188:	e2444001 	sub	r4, r4, #1
    b18c:	e3740001 	cmn	r4, #1
    b190:	e2455004 	sub	r5, r5, #4
    b194:	1affffdc 	bne	b10c <__call_exitprocs+0x74>
    b198:	e59f3070 	ldr	r3, [pc, #112]	; b210 <__call_exitprocs+0x178>
    b19c:	e3530000 	cmp	r3, #0
    b1a0:	1a000009 	bne	b1cc <__call_exitprocs+0x134>
    b1a4:	e28dd014 	add	sp, sp, #20
    b1a8:	e8bd4ff0 	pop	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
    b1ac:	e12fff1e 	bx	lr
    b1b0:	e1a0e00f 	mov	lr, pc
    b1b4:	e12fff13 	bx	r3
    b1b8:	eaffffec 	b	b170 <__call_exitprocs+0xd8>
    b1bc:	e5150080 	ldr	r0, [r5, #-128]	; 0x80
    b1c0:	e1a0e00f 	mov	lr, pc
    b1c4:	e12fff13 	bx	r3
    b1c8:	eaffffe8 	b	b170 <__call_exitprocs+0xd8>
    b1cc:	e5973004 	ldr	r3, [r7, #4]
    b1d0:	e3530000 	cmp	r3, #0
    b1d4:	e5973000 	ldr	r3, [r7]
    b1d8:	1a000008 	bne	b200 <__call_exitprocs+0x168>
    b1dc:	e3530000 	cmp	r3, #0
    b1e0:	0a000006 	beq	b200 <__call_exitprocs+0x168>
    b1e4:	e1a00007 	mov	r0, r7
    b1e8:	e58b3000 	str	r3, [fp]
    b1ec:	e1a00000 	nop			; (mov r0, r0)
    b1f0:	e59b7000 	ldr	r7, [fp]
    b1f4:	e3570000 	cmp	r7, #0
    b1f8:	1affffb5 	bne	b0d4 <__call_exitprocs+0x3c>
    b1fc:	eaffffe8 	b	b1a4 <__call_exitprocs+0x10c>
    b200:	e1a0b007 	mov	fp, r7
    b204:	e1a07003 	mov	r7, r3
    b208:	eafffff9 	b	b1f4 <__call_exitprocs+0x15c>
    b20c:	000122f8 	.word	0x000122f8
	...

0000b218 <quorem>:
    b218:	e92d4ff8 	push	{r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
    b21c:	e5903010 	ldr	r3, [r0, #16]
    b220:	e5916010 	ldr	r6, [r1, #16]
    b224:	e1560003 	cmp	r6, r3
    b228:	e1a0a000 	mov	sl, r0
    b22c:	e1a05001 	mov	r5, r1
    b230:	c3a00000 	movgt	r0, #0
    b234:	ca00006d 	bgt	b3f0 <quorem+0x1d8>
    b238:	e2863003 	add	r3, r6, #3
    b23c:	e1a03103 	lsl	r3, r3, #2
    b240:	e0819003 	add	r9, r1, r3
    b244:	e5991004 	ldr	r1, [r9, #4]
    b248:	e08a3003 	add	r3, sl, r3
    b24c:	e5930004 	ldr	r0, [r3, #4]
    b250:	e2811001 	add	r1, r1, #1
    b254:	eb00193b 	bl	11748 <__aeabi_uidiv>
    b258:	e2507000 	subs	r7, r0, #0
    b25c:	e2466001 	sub	r6, r6, #1
    b260:	e2899004 	add	r9, r9, #4
    b264:	e2854014 	add	r4, r5, #20
    b268:	e28a8014 	add	r8, sl, #20
    b26c:	0a000030 	beq	b334 <quorem+0x11c>
    b270:	e3a0c000 	mov	ip, #0
    b274:	e1a02008 	mov	r2, r8
    b278:	e1a03004 	mov	r3, r4
    b27c:	e1a0000c 	mov	r0, ip
    b280:	e493e004 	ldr	lr, [r3], #4
    b284:	e1a0b80e 	lsl	fp, lr, #16
    b288:	e1a0b82b 	lsr	fp, fp, #16
    b28c:	e1a0e82e 	lsr	lr, lr, #16
    b290:	e02bcb97 	mla	fp, r7, fp, ip
    b294:	e00c0e97 	mul	ip, r7, lr
    b298:	e592e000 	ldr	lr, [r2]
    b29c:	e08cc82b 	add	ip, ip, fp, lsr #16
    b2a0:	e1a0180e 	lsl	r1, lr, #16
    b2a4:	e0801821 	add	r1, r0, r1, lsr #16
    b2a8:	e1a0b80b 	lsl	fp, fp, #16
    b2ac:	e1a0080c 	lsl	r0, ip, #16
    b2b0:	e041182b 	sub	r1, r1, fp, lsr #16
    b2b4:	e1a00820 	lsr	r0, r0, #16
    b2b8:	e060082e 	rsb	r0, r0, lr, lsr #16
    b2bc:	e1a0b801 	lsl	fp, r1, #16
    b2c0:	e0800841 	add	r0, r0, r1, asr #16
    b2c4:	e1a0182b 	lsr	r1, fp, #16
    b2c8:	e1811800 	orr	r1, r1, r0, lsl #16
    b2cc:	e1590003 	cmp	r9, r3
    b2d0:	e1a0c82c 	lsr	ip, ip, #16
    b2d4:	e1a00840 	asr	r0, r0, #16
    b2d8:	e4821004 	str	r1, [r2], #4
    b2dc:	2affffe7 	bcs	b280 <quorem+0x68>
    b2e0:	e2863004 	add	r3, r6, #4
    b2e4:	e08a3103 	add	r3, sl, r3, lsl #2
    b2e8:	e5932004 	ldr	r2, [r3, #4]
    b2ec:	e3520000 	cmp	r2, #0
    b2f0:	1a00000f 	bne	b334 <quorem+0x11c>
    b2f4:	e1580003 	cmp	r8, r3
    b2f8:	e2832004 	add	r2, r3, #4
    b2fc:	2a00000b 	bcs	b330 <quorem+0x118>
    b300:	e5122004 	ldr	r2, [r2, #-4]
    b304:	e3520000 	cmp	r2, #0
    b308:	1a000008 	bne	b330 <quorem+0x118>
    b30c:	e2433004 	sub	r3, r3, #4
    b310:	ea000003 	b	b324 <quorem+0x10c>
    b314:	e5932000 	ldr	r2, [r3]
    b318:	e3520000 	cmp	r2, #0
    b31c:	e2433004 	sub	r3, r3, #4
    b320:	1a000002 	bne	b330 <quorem+0x118>
    b324:	e1580003 	cmp	r8, r3
    b328:	e2466001 	sub	r6, r6, #1
    b32c:	3afffff8 	bcc	b314 <quorem+0xfc>
    b330:	e58a6010 	str	r6, [sl, #16]
    b334:	e1a0000a 	mov	r0, sl
    b338:	e1a01005 	mov	r1, r5
    b33c:	eb000d05 	bl	e758 <__mcmp>
    b340:	e3500000 	cmp	r0, #0
    b344:	ba000028 	blt	b3ec <quorem+0x1d4>
    b348:	e2877001 	add	r7, r7, #1
    b34c:	e1a03008 	mov	r3, r8
    b350:	e3a02000 	mov	r2, #0
    b354:	e4945004 	ldr	r5, [r4], #4
    b358:	e593c000 	ldr	ip, [r3]
    b35c:	e1a01805 	lsl	r1, r5, #16
    b360:	e1a0080c 	lsl	r0, ip, #16
    b364:	e1a01821 	lsr	r1, r1, #16
    b368:	e0611820 	rsb	r1, r1, r0, lsr #16
    b36c:	e0811002 	add	r1, r1, r2
    b370:	e1a02825 	lsr	r2, r5, #16
    b374:	e062282c 	rsb	r2, r2, ip, lsr #16
    b378:	e1a00801 	lsl	r0, r1, #16
    b37c:	e0822841 	add	r2, r2, r1, asr #16
    b380:	e1a01820 	lsr	r1, r0, #16
    b384:	e1811802 	orr	r1, r1, r2, lsl #16
    b388:	e1590004 	cmp	r9, r4
    b38c:	e1a02842 	asr	r2, r2, #16
    b390:	e4831004 	str	r1, [r3], #4
    b394:	2affffee 	bcs	b354 <quorem+0x13c>
    b398:	e2863004 	add	r3, r6, #4
    b39c:	e08a3103 	add	r3, sl, r3, lsl #2
    b3a0:	e5932004 	ldr	r2, [r3, #4]
    b3a4:	e3520000 	cmp	r2, #0
    b3a8:	1a00000f 	bne	b3ec <quorem+0x1d4>
    b3ac:	e1580003 	cmp	r8, r3
    b3b0:	e2832004 	add	r2, r3, #4
    b3b4:	2a00000b 	bcs	b3e8 <quorem+0x1d0>
    b3b8:	e5122004 	ldr	r2, [r2, #-4]
    b3bc:	e3520000 	cmp	r2, #0
    b3c0:	1a000008 	bne	b3e8 <quorem+0x1d0>
    b3c4:	e2433004 	sub	r3, r3, #4
    b3c8:	ea000003 	b	b3dc <quorem+0x1c4>
    b3cc:	e5932000 	ldr	r2, [r3]
    b3d0:	e3520000 	cmp	r2, #0
    b3d4:	e2433004 	sub	r3, r3, #4
    b3d8:	1a000002 	bne	b3e8 <quorem+0x1d0>
    b3dc:	e1580003 	cmp	r8, r3
    b3e0:	e2466001 	sub	r6, r6, #1
    b3e4:	3afffff8 	bcc	b3cc <quorem+0x1b4>
    b3e8:	e58a6010 	str	r6, [sl, #16]
    b3ec:	e1a00007 	mov	r0, r7
    b3f0:	e8bd4ff8 	pop	{r3, r4, r5, r6, r7, r8, r9, sl, fp, lr}
    b3f4:	e12fff1e 	bx	lr

0000b3f8 <_dtoa_r>:
    b3f8:	e92d4ff0 	push	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
    b3fc:	e5901040 	ldr	r1, [r0, #64]	; 0x40
    b400:	e24dd07c 	sub	sp, sp, #124	; 0x7c
    b404:	e3510000 	cmp	r1, #0
    b408:	e1a06000 	mov	r6, r0
    b40c:	e59d40ac 	ldr	r4, [sp, #172]	; 0xac
    b410:	e58d2010 	str	r2, [sp, #16]
    b414:	e58d3014 	str	r3, [sp, #20]
    b418:	0a000007 	beq	b43c <_dtoa_r+0x44>
    b41c:	e5903044 	ldr	r3, [r0, #68]	; 0x44
    b420:	e3a02001 	mov	r2, #1
    b424:	e1a02312 	lsl	r2, r2, r3
    b428:	e5813004 	str	r3, [r1, #4]
    b42c:	e5812008 	str	r2, [r1, #8]
    b430:	eb000b15 	bl	e08c <_Bfree>
    b434:	e3a03000 	mov	r3, #0
    b438:	e5863040 	str	r3, [r6, #64]	; 0x40
    b43c:	e59da014 	ldr	sl, [sp, #20]
    b440:	e35a0000 	cmp	sl, #0
    b444:	b3a03001 	movlt	r3, #1
    b448:	a3a03000 	movge	r3, #0
    b44c:	b5843000 	strlt	r3, [r4]
    b450:	a5843000 	strge	r3, [r4]
    b454:	e59f3560 	ldr	r3, [pc, #1376]	; b9bc <_dtoa_r+0x5c4>
    b458:	b3caa102 	biclt	sl, sl, #-2147483648	; 0x80000000
    b45c:	e59f2558 	ldr	r2, [pc, #1368]	; b9bc <_dtoa_r+0x5c4>
    b460:	b1a0100a 	movlt	r1, sl
    b464:	e00a3003 	and	r3, sl, r3
    b468:	b58d1014 	strlt	r1, [sp, #20]
    b46c:	e1530002 	cmp	r3, r2
    b470:	0a000016 	beq	b4d0 <_dtoa_r+0xd8>
    b474:	e28d5010 	add	r5, sp, #16
    b478:	e8950030 	ldm	r5, {r4, r5}
    b47c:	e3a02000 	mov	r2, #0
    b480:	e1a00004 	mov	r0, r4
    b484:	e1a01005 	mov	r1, r5
    b488:	e3a03000 	mov	r3, #0
    b48c:	eb001b54 	bl	121e4 <__aeabi_dcmpeq>
    b490:	e3500000 	cmp	r0, #0
    b494:	0a00001e 	beq	b514 <_dtoa_r+0x11c>
    b498:	e59d40b0 	ldr	r4, [sp, #176]	; 0xb0
    b49c:	e59dc0a8 	ldr	ip, [sp, #168]	; 0xa8
    b4a0:	e3a03001 	mov	r3, #1
    b4a4:	e3540000 	cmp	r4, #0
    b4a8:	e58c3000 	str	r3, [ip]
    b4ac:	0a000096 	beq	b70c <_dtoa_r+0x314>
    b4b0:	e59f44d8 	ldr	r4, [pc, #1240]	; b990 <_dtoa_r+0x598>
    b4b4:	e59dc0b0 	ldr	ip, [sp, #176]	; 0xb0
    b4b8:	e58c4000 	str	r4, [ip]
    b4bc:	e2444001 	sub	r4, r4, #1
    b4c0:	e1a00004 	mov	r0, r4
    b4c4:	e28dd07c 	add	sp, sp, #124	; 0x7c
    b4c8:	e8bd4ff0 	pop	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
    b4cc:	e12fff1e 	bx	lr
    b4d0:	e59d2010 	ldr	r2, [sp, #16]
    b4d4:	e59f34b8 	ldr	r3, [pc, #1208]	; b994 <_dtoa_r+0x59c>
    b4d8:	e59d40a8 	ldr	r4, [sp, #168]	; 0xa8
    b4dc:	e3520000 	cmp	r2, #0
    b4e0:	e5843000 	str	r3, [r4]
    b4e4:	0a000075 	beq	b6c0 <_dtoa_r+0x2c8>
    b4e8:	e59f44a8 	ldr	r4, [pc, #1192]	; b998 <_dtoa_r+0x5a0>
    b4ec:	e59dc0b0 	ldr	ip, [sp, #176]	; 0xb0
    b4f0:	e35c0000 	cmp	ip, #0
    b4f4:	0afffff1 	beq	b4c0 <_dtoa_r+0xc8>
    b4f8:	e5d43003 	ldrb	r3, [r4, #3]
    b4fc:	e59dc0b0 	ldr	ip, [sp, #176]	; 0xb0
    b500:	e3530000 	cmp	r3, #0
    b504:	12843008 	addne	r3, r4, #8
    b508:	02843003 	addeq	r3, r4, #3
    b50c:	e58c3000 	str	r3, [ip]
    b510:	eaffffea 	b	b4c0 <_dtoa_r+0xc8>
    b514:	e28d3074 	add	r3, sp, #116	; 0x74
    b518:	e58d3000 	str	r3, [sp]
    b51c:	e28d3070 	add	r3, sp, #112	; 0x70
    b520:	e58d3004 	str	r3, [sp, #4]
    b524:	e1a00006 	mov	r0, r6
    b528:	e1a02004 	mov	r2, r4
    b52c:	e1a03005 	mov	r3, r5
    b530:	eb000d40 	bl	ea38 <__d2b>
    b534:	e1a0708a 	lsl	r7, sl, #1
    b538:	e1b07aa7 	lsrs	r7, r7, #21
    b53c:	e58d0038 	str	r0, [sp, #56]	; 0x38
    b540:	1a000066 	bne	b6e0 <_dtoa_r+0x2e8>
    b544:	e59d8070 	ldr	r8, [sp, #112]	; 0x70
    b548:	e59d7074 	ldr	r7, [sp, #116]	; 0x74
    b54c:	e3e03e41 	mvn	r3, #1040	; 0x410
    b550:	e0887007 	add	r7, r8, r7
    b554:	e1570003 	cmp	r7, r3
    b558:	ba0001cf 	blt	bc9c <_dtoa_r+0x8a4>
    b55c:	e2870e41 	add	r0, r7, #1040	; 0x410
    b560:	e59dc010 	ldr	ip, [sp, #16]
    b564:	e2800002 	add	r0, r0, #2
    b568:	e1a0003c 	lsr	r0, ip, r0
    b56c:	e283301f 	add	r3, r3, #31
    b570:	e0673003 	rsb	r3, r7, r3
    b574:	e180031a 	orr	r0, r0, sl, lsl r3
    b578:	eb00197f 	bl	11b7c <__aeabi_ui2d>
    b57c:	e3a04001 	mov	r4, #1
    b580:	e58d4060 	str	r4, [sp, #96]	; 0x60
    b584:	e241161f 	sub	r1, r1, #32505856	; 0x1f00000
    b588:	e2477001 	sub	r7, r7, #1
    b58c:	e3a02000 	mov	r2, #0
    b590:	e59f3404 	ldr	r3, [pc, #1028]	; b99c <_dtoa_r+0x5a4>
    b594:	eb0018b3 	bl	11868 <__aeabi_dsub>
    b598:	e28f3ff6 	add	r3, pc, #984	; 0x3d8
    b59c:	e893000c 	ldm	r3, {r2, r3}
    b5a0:	eb0019b5 	bl	11c7c <__aeabi_dmul>
    b5a4:	e28f3ff5 	add	r3, pc, #980	; 0x3d4
    b5a8:	e893000c 	ldm	r3, {r2, r3}
    b5ac:	eb0018ae 	bl	1186c <__adddf3>
    b5b0:	e1a04000 	mov	r4, r0
    b5b4:	e1a00007 	mov	r0, r7
    b5b8:	e1a05001 	mov	r5, r1
    b5bc:	eb001977 	bl	11ba0 <__aeabi_i2d>
    b5c0:	e28f3d0f 	add	r3, pc, #960	; 0x3c0
    b5c4:	e893000c 	ldm	r3, {r2, r3}
    b5c8:	eb0019ab 	bl	11c7c <__aeabi_dmul>
    b5cc:	e1a02000 	mov	r2, r0
    b5d0:	e1a03001 	mov	r3, r1
    b5d4:	e1a00004 	mov	r0, r4
    b5d8:	e1a01005 	mov	r1, r5
    b5dc:	eb0018a2 	bl	1186c <__adddf3>
    b5e0:	e1a04000 	mov	r4, r0
    b5e4:	e1a05001 	mov	r5, r1
    b5e8:	eb001b1b 	bl	1225c <__aeabi_d2iz>
    b5ec:	e1a01005 	mov	r1, r5
    b5f0:	e1a09000 	mov	r9, r0
    b5f4:	e3a02000 	mov	r2, #0
    b5f8:	e1a00004 	mov	r0, r4
    b5fc:	e3a03000 	mov	r3, #0
    b600:	eb001afd 	bl	121fc <__aeabi_dcmplt>
    b604:	e3500000 	cmp	r0, #0
    b608:	1a00019b 	bne	bc7c <_dtoa_r+0x884>
    b60c:	e3590016 	cmp	r9, #22
    b610:	83a04001 	movhi	r4, #1
    b614:	858d404c 	strhi	r4, [sp, #76]	; 0x4c
    b618:	8a00000a 	bhi	b648 <_dtoa_r+0x250>
    b61c:	e59f3390 	ldr	r3, [pc, #912]	; b9b4 <_dtoa_r+0x5bc>
    b620:	e0833189 	add	r3, r3, r9, lsl #3
    b624:	e8930003 	ldm	r3, {r0, r1}
    b628:	e28d3010 	add	r3, sp, #16
    b62c:	e893000c 	ldm	r3, {r2, r3}
    b630:	eb001b03 	bl	12244 <__aeabi_dcmpgt>
    b634:	e3500000 	cmp	r0, #0
    b638:	13a0c000 	movne	ip, #0
    b63c:	158dc04c 	strne	ip, [sp, #76]	; 0x4c
    b640:	058d004c 	streq	r0, [sp, #76]	; 0x4c
    b644:	12499001 	subne	r9, r9, #1
    b648:	e0677008 	rsb	r7, r7, r8
    b64c:	e257b001 	subs	fp, r7, #1
    b650:	426bb000 	rsbmi	fp, fp, #0
    b654:	53a0c000 	movpl	ip, #0
    b658:	458db028 	strmi	fp, [sp, #40]	; 0x28
    b65c:	558dc028 	strpl	ip, [sp, #40]	; 0x28
    b660:	43a0b000 	movmi	fp, #0
    b664:	e3590000 	cmp	r9, #0
    b668:	ba00017b 	blt	bc5c <_dtoa_r+0x864>
    b66c:	e3a04000 	mov	r4, #0
    b670:	e58d9048 	str	r9, [sp, #72]	; 0x48
    b674:	e58d403c 	str	r4, [sp, #60]	; 0x3c
    b678:	e08bb009 	add	fp, fp, r9
    b67c:	e59d40a0 	ldr	r4, [sp, #160]	; 0xa0
    b680:	e3540009 	cmp	r4, #9
    b684:	8a000022 	bhi	b714 <_dtoa_r+0x31c>
    b688:	e3540005 	cmp	r4, #5
    b68c:	c2444004 	subgt	r4, r4, #4
    b690:	c58d40a0 	strgt	r4, [sp, #160]	; 0xa0
    b694:	e59dc0a0 	ldr	ip, [sp, #160]	; 0xa0
    b698:	e24c3002 	sub	r3, ip, #2
    b69c:	c3a04000 	movgt	r4, #0
    b6a0:	d3a04001 	movle	r4, #1
    b6a4:	e3530003 	cmp	r3, #3
    b6a8:	979ff103 	ldrls	pc, [pc, r3, lsl #2]
    b6ac:	ea00001a 	b	b71c <_dtoa_r+0x324>
    b6b0:	0000bd2c 	.word	0x0000bd2c
    b6b4:	0000c0c0 	.word	0x0000c0c0
    b6b8:	0000c0b4 	.word	0x0000c0b4
    b6bc:	0000c080 	.word	0x0000c080
    b6c0:	e3ca44ff 	bic	r4, sl, #-16777216	; 0xff000000
    b6c4:	e3c4460f 	bic	r4, r4, #15728640	; 0xf00000
    b6c8:	e59f32c8 	ldr	r3, [pc, #712]	; b998 <_dtoa_r+0x5a0>
    b6cc:	e59f22cc 	ldr	r2, [pc, #716]	; b9a0 <_dtoa_r+0x5a8>
    b6d0:	e3540000 	cmp	r4, #0
    b6d4:	01a04002 	moveq	r4, r2
    b6d8:	11a04003 	movne	r4, r3
    b6dc:	eaffff82 	b	b4ec <_dtoa_r+0xf4>
    b6e0:	e3c534ff 	bic	r3, r5, #-16777216	; 0xff000000
    b6e4:	e3c3360f 	bic	r3, r3, #15728640	; 0xf00000
    b6e8:	e1a00004 	mov	r0, r4
    b6ec:	e38335ff 	orr	r3, r3, #1069547520	; 0x3fc00000
    b6f0:	e2477fff 	sub	r7, r7, #1020	; 0x3fc
    b6f4:	e3a04000 	mov	r4, #0
    b6f8:	e3831603 	orr	r1, r3, #3145728	; 0x300000
    b6fc:	e2477003 	sub	r7, r7, #3
    b700:	e59d8070 	ldr	r8, [sp, #112]	; 0x70
    b704:	e58d4060 	str	r4, [sp, #96]	; 0x60
    b708:	eaffff9f 	b	b58c <_dtoa_r+0x194>
    b70c:	e59f4290 	ldr	r4, [pc, #656]	; b9a4 <_dtoa_r+0x5ac>
    b710:	eaffff6a 	b	b4c0 <_dtoa_r+0xc8>
    b714:	e3a04000 	mov	r4, #0
    b718:	e58d40a0 	str	r4, [sp, #160]	; 0xa0
    b71c:	e3a04000 	mov	r4, #0
    b720:	e5864044 	str	r4, [r6, #68]	; 0x44
    b724:	e1a00006 	mov	r0, r6
    b728:	e1a01004 	mov	r1, r4
    b72c:	eb000a33 	bl	e000 <_Balloc>
    b730:	e3e0c000 	mvn	ip, #0
    b734:	e58dc044 	str	ip, [sp, #68]	; 0x44
    b738:	e58dc02c 	str	ip, [sp, #44]	; 0x2c
    b73c:	e3a0c001 	mov	ip, #1
    b740:	e58d0024 	str	r0, [sp, #36]	; 0x24
    b744:	e5860040 	str	r0, [r6, #64]	; 0x40
    b748:	e58dc040 	str	ip, [sp, #64]	; 0x40
    b74c:	e58d40a4 	str	r4, [sp, #164]	; 0xa4
    b750:	e59d3074 	ldr	r3, [sp, #116]	; 0x74
    b754:	e3530000 	cmp	r3, #0
    b758:	ba0000ab 	blt	ba0c <_dtoa_r+0x614>
    b75c:	e359000e 	cmp	r9, #14
    b760:	ca0000a9 	bgt	ba0c <_dtoa_r+0x614>
    b764:	e59d40a4 	ldr	r4, [sp, #164]	; 0xa4
    b768:	e59dc02c 	ldr	ip, [sp, #44]	; 0x2c
    b76c:	e59f3240 	ldr	r3, [pc, #576]	; b9b4 <_dtoa_r+0x5bc>
    b770:	e1a02fa4 	lsr	r2, r4, #31
    b774:	e35c0000 	cmp	ip, #0
    b778:	c3a02000 	movgt	r2, #0
    b77c:	d2022001 	andle	r2, r2, #1
    b780:	e0833189 	add	r3, r3, r9, lsl #3
    b784:	e3520000 	cmp	r2, #0
    b788:	e8930006 	ldm	r3, {r1, r2}
    b78c:	e58d1018 	str	r1, [sp, #24]
    b790:	e58d201c 	str	r2, [sp, #28]
    b794:	1a00033d 	bne	c490 <_dtoa_r+0x1098>
    b798:	e28d5010 	add	r5, sp, #16
    b79c:	e8950030 	ldm	r5, {r4, r5}
    b7a0:	e1a03002 	mov	r3, r2
    b7a4:	e1a00004 	mov	r0, r4
    b7a8:	e1a02001 	mov	r2, r1
    b7ac:	e1a01005 	mov	r1, r5
    b7b0:	eb0019d5 	bl	11f0c <__aeabi_ddiv>
    b7b4:	eb001aa8 	bl	1225c <__aeabi_d2iz>
    b7b8:	e1a07000 	mov	r7, r0
    b7bc:	eb0018f7 	bl	11ba0 <__aeabi_i2d>
    b7c0:	e28d3018 	add	r3, sp, #24
    b7c4:	e893000c 	ldm	r3, {r2, r3}
    b7c8:	eb00192b 	bl	11c7c <__aeabi_dmul>
    b7cc:	e1a03001 	mov	r3, r1
    b7d0:	e1a02000 	mov	r2, r0
    b7d4:	e1a01005 	mov	r1, r5
    b7d8:	e1a00004 	mov	r0, r4
    b7dc:	eb001821 	bl	11868 <__aeabi_dsub>
    b7e0:	e59dc024 	ldr	ip, [sp, #36]	; 0x24
    b7e4:	e2873030 	add	r3, r7, #48	; 0x30
    b7e8:	e4cc3001 	strb	r3, [ip], #1
    b7ec:	e58dc030 	str	ip, [sp, #48]	; 0x30
    b7f0:	e59dc02c 	ldr	ip, [sp, #44]	; 0x2c
    b7f4:	e35c0001 	cmp	ip, #1
    b7f8:	e1a04000 	mov	r4, r0
    b7fc:	e1a05001 	mov	r5, r1
    b800:	e59dc030 	ldr	ip, [sp, #48]	; 0x30
    b804:	0a000039 	beq	b8f0 <_dtoa_r+0x4f8>
    b808:	e3a02000 	mov	r2, #0
    b80c:	e59f31ac 	ldr	r3, [pc, #428]	; b9c0 <_dtoa_r+0x5c8>
    b810:	eb001919 	bl	11c7c <__aeabi_dmul>
    b814:	e3a02000 	mov	r2, #0
    b818:	e3a03000 	mov	r3, #0
    b81c:	e1a04000 	mov	r4, r0
    b820:	e1a05001 	mov	r5, r1
    b824:	eb001a6e 	bl	121e4 <__aeabi_dcmpeq>
    b828:	e3500000 	cmp	r0, #0
    b82c:	1a000477 	bne	ca10 <_dtoa_r+0x1618>
    b830:	e59dc024 	ldr	ip, [sp, #36]	; 0x24
    b834:	e59d102c 	ldr	r1, [sp, #44]	; 0x2c
    b838:	e08c8001 	add	r8, ip, r1
    b83c:	e1a0b009 	mov	fp, r9
    b840:	e58d8010 	str	r8, [sp, #16]
    b844:	e28ca002 	add	sl, ip, #2
    b848:	e28d9018 	add	r9, sp, #24
    b84c:	e8990300 	ldm	r9, {r8, r9}
    b850:	e58d6020 	str	r6, [sp, #32]
    b854:	ea000008 	b	b87c <_dtoa_r+0x484>
    b858:	eb001907 	bl	11c7c <__aeabi_dmul>
    b85c:	e3a02000 	mov	r2, #0
    b860:	e3a03000 	mov	r3, #0
    b864:	e1a04000 	mov	r4, r0
    b868:	e1a05001 	mov	r5, r1
    b86c:	eb001a5c 	bl	121e4 <__aeabi_dcmpeq>
    b870:	e3500000 	cmp	r0, #0
    b874:	e28aa001 	add	sl, sl, #1
    b878:	1a0003a5 	bne	c714 <_dtoa_r+0x131c>
    b87c:	e1a02008 	mov	r2, r8
    b880:	e1a03009 	mov	r3, r9
    b884:	e1a00004 	mov	r0, r4
    b888:	e1a01005 	mov	r1, r5
    b88c:	eb00199e 	bl	11f0c <__aeabi_ddiv>
    b890:	eb001a71 	bl	1225c <__aeabi_d2iz>
    b894:	e1a07000 	mov	r7, r0
    b898:	eb0018c0 	bl	11ba0 <__aeabi_i2d>
    b89c:	e1a02008 	mov	r2, r8
    b8a0:	e1a03009 	mov	r3, r9
    b8a4:	eb0018f4 	bl	11c7c <__aeabi_dmul>
    b8a8:	e1a02000 	mov	r2, r0
    b8ac:	e1a03001 	mov	r3, r1
    b8b0:	e1a00004 	mov	r0, r4
    b8b4:	e1a01005 	mov	r1, r5
    b8b8:	eb0017ea 	bl	11868 <__aeabi_dsub>
    b8bc:	e59dc010 	ldr	ip, [sp, #16]
    b8c0:	e15a000c 	cmp	sl, ip
    b8c4:	e287c030 	add	ip, r7, #48	; 0x30
    b8c8:	e1a04000 	mov	r4, r0
    b8cc:	e1a05001 	mov	r5, r1
    b8d0:	e3a02000 	mov	r2, #0
    b8d4:	e59f30e4 	ldr	r3, [pc, #228]	; b9c0 <_dtoa_r+0x5c8>
    b8d8:	e54ac001 	strb	ip, [sl, #-1]
    b8dc:	e1a0600a 	mov	r6, sl
    b8e0:	1affffdc 	bne	b858 <_dtoa_r+0x460>
    b8e4:	e59d6020 	ldr	r6, [sp, #32]
    b8e8:	e1a0900b 	mov	r9, fp
    b8ec:	e1a0c00a 	mov	ip, sl
    b8f0:	e1a02004 	mov	r2, r4
    b8f4:	e1a03005 	mov	r3, r5
    b8f8:	e1a00004 	mov	r0, r4
    b8fc:	e1a01005 	mov	r1, r5
    b900:	e58dc00c 	str	ip, [sp, #12]
    b904:	eb0017d8 	bl	1186c <__adddf3>
    b908:	e1a04000 	mov	r4, r0
    b90c:	e1a05001 	mov	r5, r1
    b910:	e1a02004 	mov	r2, r4
    b914:	e28d1018 	add	r1, sp, #24
    b918:	e8910003 	ldm	r1, {r0, r1}
    b91c:	e1a03005 	mov	r3, r5
    b920:	eb001a35 	bl	121fc <__aeabi_dcmplt>
    b924:	e3500000 	cmp	r0, #0
    b928:	e59dc00c 	ldr	ip, [sp, #12]
    b92c:	1a00000a 	bne	b95c <_dtoa_r+0x564>
    b930:	e28d1018 	add	r1, sp, #24
    b934:	e8910003 	ldm	r1, {r0, r1}
    b938:	e1a02004 	mov	r2, r4
    b93c:	e1a03005 	mov	r3, r5
    b940:	e58dc00c 	str	ip, [sp, #12]
    b944:	eb001a26 	bl	121e4 <__aeabi_dcmpeq>
    b948:	e3500000 	cmp	r0, #0
    b94c:	e59dc00c 	ldr	ip, [sp, #12]
    b950:	0a000372 	beq	c720 <_dtoa_r+0x1328>
    b954:	e3170001 	tst	r7, #1
    b958:	0a000370 	beq	c720 <_dtoa_r+0x1328>
    b95c:	e55c8001 	ldrb	r8, [ip, #-1]
    b960:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    b964:	e58d9064 	str	r9, [sp, #100]	; 0x64
    b968:	e1a0900c 	mov	r9, ip
    b96c:	e59d1030 	ldr	r1, [sp, #48]	; 0x30
    b970:	ea000017 	b	b9d4 <_dtoa_r+0x5dc>
    b974:	e1a00000 	nop			; (mov r0, r0)
    b978:	636f4361 	.word	0x636f4361
    b97c:	3fd287a7 	.word	0x3fd287a7
    b980:	8b60c8b3 	.word	0x8b60c8b3
    b984:	3fc68a28 	.word	0x3fc68a28
    b988:	509f79fb 	.word	0x509f79fb
    b98c:	3fd34413 	.word	0x3fd34413
    b990:	00012365 	.word	0x00012365
    b994:	0000270f 	.word	0x0000270f
    b998:	00012374 	.word	0x00012374
    b99c:	3ff80000 	.word	0x3ff80000
    b9a0:	00012368 	.word	0x00012368
    b9a4:	00012364 	.word	0x00012364
    b9a8:	fffffbee 	.word	0xfffffbee
    b9ac:	3ff00000 	.word	0x3ff00000
    b9b0:	00012488 	.word	0x00012488
    b9b4:	00012398 	.word	0x00012398
    b9b8:	40140000 	.word	0x40140000
    b9bc:	7ff00000 	.word	0x7ff00000
    b9c0:	40240000 	.word	0x40240000
    b9c4:	401c0000 	.word	0x401c0000
    b9c8:	3fe00000 	.word	0x3fe00000
    b9cc:	e5598002 	ldrb	r8, [r9, #-2]
    b9d0:	e1a09003 	mov	r9, r3
    b9d4:	e3580039 	cmp	r8, #57	; 0x39
    b9d8:	e1a02009 	mov	r2, r9
    b9dc:	e2493001 	sub	r3, r9, #1
    b9e0:	1a0003b6 	bne	c8c0 <_dtoa_r+0x14c8>
    b9e4:	e1590001 	cmp	r9, r1
    b9e8:	1afffff7 	bne	b9cc <_dtoa_r+0x5d4>
    b9ec:	e59dc064 	ldr	ip, [sp, #100]	; 0x64
    b9f0:	e58d2024 	str	r2, [sp, #36]	; 0x24
    b9f4:	e3a02030 	mov	r2, #48	; 0x30
    b9f8:	e5c32000 	strb	r2, [r3]
    b9fc:	e28c9001 	add	r9, ip, #1
    ba00:	e3a02031 	mov	r2, #49	; 0x31
    ba04:	e5c32000 	strb	r2, [r3]
    ba08:	ea000084 	b	bc20 <_dtoa_r+0x828>
    ba0c:	e59d4040 	ldr	r4, [sp, #64]	; 0x40
    ba10:	e3540000 	cmp	r4, #0
    ba14:	1a0000a5 	bne	bcb0 <_dtoa_r+0x8b8>
    ba18:	e59d503c 	ldr	r5, [sp, #60]	; 0x3c
    ba1c:	e59d4028 	ldr	r4, [sp, #40]	; 0x28
    ba20:	e59da040 	ldr	sl, [sp, #64]	; 0x40
    ba24:	e35b0000 	cmp	fp, #0
    ba28:	c3540000 	cmpgt	r4, #0
    ba2c:	da000007 	ble	ba50 <_dtoa_r+0x658>
    ba30:	e59dc028 	ldr	ip, [sp, #40]	; 0x28
    ba34:	e15b0004 	cmp	fp, r4
    ba38:	b1a0300b 	movlt	r3, fp
    ba3c:	a1a03004 	movge	r3, r4
    ba40:	e063c00c 	rsb	ip, r3, ip
    ba44:	e58dc028 	str	ip, [sp, #40]	; 0x28
    ba48:	e0634004 	rsb	r4, r3, r4
    ba4c:	e063b00b 	rsb	fp, r3, fp
    ba50:	e59dc03c 	ldr	ip, [sp, #60]	; 0x3c
    ba54:	e35c0000 	cmp	ip, #0
    ba58:	da000015 	ble	bab4 <_dtoa_r+0x6bc>
    ba5c:	e59dc040 	ldr	ip, [sp, #64]	; 0x40
    ba60:	e35c0000 	cmp	ip, #0
    ba64:	0a000324 	beq	c6fc <_dtoa_r+0x1304>
    ba68:	e3550000 	cmp	r5, #0
    ba6c:	da00000d 	ble	baa8 <_dtoa_r+0x6b0>
    ba70:	e1a0100a 	mov	r1, sl
    ba74:	e1a02005 	mov	r2, r5
    ba78:	e1a00006 	mov	r0, r6
    ba7c:	eb000ab7 	bl	e560 <__pow5mult>
    ba80:	e1a0a000 	mov	sl, r0
    ba84:	e1a0100a 	mov	r1, sl
    ba88:	e59d2038 	ldr	r2, [sp, #56]	; 0x38
    ba8c:	e1a00006 	mov	r0, r6
    ba90:	eb000a39 	bl	e37c <__multiply>
    ba94:	e59d1038 	ldr	r1, [sp, #56]	; 0x38
    ba98:	e1a07000 	mov	r7, r0
    ba9c:	e1a00006 	mov	r0, r6
    baa0:	eb000979 	bl	e08c <_Bfree>
    baa4:	e58d7038 	str	r7, [sp, #56]	; 0x38
    baa8:	e59dc03c 	ldr	ip, [sp, #60]	; 0x3c
    baac:	e05c2005 	subs	r2, ip, r5
    bab0:	1a000356 	bne	c810 <_dtoa_r+0x1418>
    bab4:	e1a00006 	mov	r0, r6
    bab8:	e3a01001 	mov	r1, #1
    babc:	eb000a25 	bl	e358 <__i2b>
    bac0:	e59dc048 	ldr	ip, [sp, #72]	; 0x48
    bac4:	e35c0000 	cmp	ip, #0
    bac8:	e1a07000 	mov	r7, r0
    bacc:	da000004 	ble	bae4 <_dtoa_r+0x6ec>
    bad0:	e1a01007 	mov	r1, r7
    bad4:	e1a00006 	mov	r0, r6
    bad8:	e1a0200c 	mov	r2, ip
    badc:	eb000a9f 	bl	e560 <__pow5mult>
    bae0:	e1a07000 	mov	r7, r0
    bae4:	e59dc0a0 	ldr	ip, [sp, #160]	; 0xa0
    bae8:	e35c0001 	cmp	ip, #1
    baec:	da000277 	ble	c4d0 <_dtoa_r+0x10d8>
    baf0:	e3a05000 	mov	r5, #0
    baf4:	e59dc048 	ldr	ip, [sp, #72]	; 0x48
    baf8:	e35c0000 	cmp	ip, #0
    bafc:	03a00001 	moveq	r0, #1
    bb00:	1a000240 	bne	c408 <_dtoa_r+0x1010>
    bb04:	e080300b 	add	r3, r0, fp
    bb08:	e213301f 	ands	r3, r3, #31
    bb0c:	0a0001ce 	beq	c24c <_dtoa_r+0xe54>
    bb10:	e2632020 	rsb	r2, r3, #32
    bb14:	e3520004 	cmp	r2, #4
    bb18:	da0003d8 	ble	ca80 <_dtoa_r+0x1688>
    bb1c:	e263301c 	rsb	r3, r3, #28
    bb20:	e59dc028 	ldr	ip, [sp, #40]	; 0x28
    bb24:	e08cc003 	add	ip, ip, r3
    bb28:	e58dc028 	str	ip, [sp, #40]	; 0x28
    bb2c:	e0844003 	add	r4, r4, r3
    bb30:	e08bb003 	add	fp, fp, r3
    bb34:	e59dc028 	ldr	ip, [sp, #40]	; 0x28
    bb38:	e35c0000 	cmp	ip, #0
    bb3c:	da000004 	ble	bb54 <_dtoa_r+0x75c>
    bb40:	e1a00006 	mov	r0, r6
    bb44:	e59d1038 	ldr	r1, [sp, #56]	; 0x38
    bb48:	e1a0200c 	mov	r2, ip
    bb4c:	eb000ac0 	bl	e654 <__lshift>
    bb50:	e58d0038 	str	r0, [sp, #56]	; 0x38
    bb54:	e35b0000 	cmp	fp, #0
    bb58:	da000004 	ble	bb70 <_dtoa_r+0x778>
    bb5c:	e1a01007 	mov	r1, r7
    bb60:	e1a00006 	mov	r0, r6
    bb64:	e1a0200b 	mov	r2, fp
    bb68:	eb000ab9 	bl	e654 <__lshift>
    bb6c:	e1a07000 	mov	r7, r0
    bb70:	e59dc04c 	ldr	ip, [sp, #76]	; 0x4c
    bb74:	e35c0000 	cmp	ip, #0
    bb78:	1a000228 	bne	c420 <_dtoa_r+0x1028>
    bb7c:	e59dc0a0 	ldr	ip, [sp, #160]	; 0xa0
    bb80:	e59d102c 	ldr	r1, [sp, #44]	; 0x2c
    bb84:	e35c0002 	cmp	ip, #2
    bb88:	d3a03000 	movle	r3, #0
    bb8c:	c3a03001 	movgt	r3, #1
    bb90:	e3510000 	cmp	r1, #0
    bb94:	c3a03000 	movgt	r3, #0
    bb98:	e3530000 	cmp	r3, #0
    bb9c:	0a00016d 	beq	c158 <_dtoa_r+0xd60>
    bba0:	e59d402c 	ldr	r4, [sp, #44]	; 0x2c
    bba4:	e3540000 	cmp	r4, #0
    bba8:	1a000165 	bne	c144 <_dtoa_r+0xd4c>
    bbac:	e1a01007 	mov	r1, r7
    bbb0:	e3a02005 	mov	r2, #5
    bbb4:	e1a03004 	mov	r3, r4
    bbb8:	e1a00006 	mov	r0, r6
    bbbc:	eb000939 	bl	e0a8 <__multadd>
    bbc0:	e1a07000 	mov	r7, r0
    bbc4:	e1a01007 	mov	r1, r7
    bbc8:	e59d0038 	ldr	r0, [sp, #56]	; 0x38
    bbcc:	eb000ae1 	bl	e758 <__mcmp>
    bbd0:	e3500000 	cmp	r0, #0
    bbd4:	da00015a 	ble	c144 <_dtoa_r+0xd4c>
    bbd8:	e59d3024 	ldr	r3, [sp, #36]	; 0x24
    bbdc:	e3a02031 	mov	r2, #49	; 0x31
    bbe0:	e4c32001 	strb	r2, [r3], #1
    bbe4:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    bbe8:	e58d3024 	str	r3, [sp, #36]	; 0x24
    bbec:	e2899001 	add	r9, r9, #1
    bbf0:	e3a05000 	mov	r5, #0
    bbf4:	e1a00006 	mov	r0, r6
    bbf8:	e1a01007 	mov	r1, r7
    bbfc:	eb000922 	bl	e08c <_Bfree>
    bc00:	e35a0000 	cmp	sl, #0
    bc04:	0a000005 	beq	bc20 <_dtoa_r+0x828>
    bc08:	e155000a 	cmp	r5, sl
    bc0c:	13550000 	cmpne	r5, #0
    bc10:	1a000195 	bne	c26c <_dtoa_r+0xe74>
    bc14:	e1a00006 	mov	r0, r6
    bc18:	e1a0100a 	mov	r1, sl
    bc1c:	eb00091a 	bl	e08c <_Bfree>
    bc20:	e1a00006 	mov	r0, r6
    bc24:	e59d1038 	ldr	r1, [sp, #56]	; 0x38
    bc28:	eb000917 	bl	e08c <_Bfree>
    bc2c:	e59dc0b0 	ldr	ip, [sp, #176]	; 0xb0
    bc30:	e35c0000 	cmp	ip, #0
    bc34:	e59dc024 	ldr	ip, [sp, #36]	; 0x24
    bc38:	e3a02000 	mov	r2, #0
    bc3c:	e5cc2000 	strb	r2, [ip]
    bc40:	e59dc0a8 	ldr	ip, [sp, #168]	; 0xa8
    bc44:	e2893001 	add	r3, r9, #1
    bc48:	e58c3000 	str	r3, [ip]
    bc4c:	159dc024 	ldrne	ip, [sp, #36]	; 0x24
    bc50:	159d10b0 	ldrne	r1, [sp, #176]	; 0xb0
    bc54:	1581c000 	strne	ip, [r1]
    bc58:	eafffe18 	b	b4c0 <_dtoa_r+0xc8>
    bc5c:	e59dc028 	ldr	ip, [sp, #40]	; 0x28
    bc60:	e069c00c 	rsb	ip, r9, ip
    bc64:	e58dc028 	str	ip, [sp, #40]	; 0x28
    bc68:	e2694000 	rsb	r4, r9, #0
    bc6c:	e3a0c000 	mov	ip, #0
    bc70:	e58d403c 	str	r4, [sp, #60]	; 0x3c
    bc74:	e58dc048 	str	ip, [sp, #72]	; 0x48
    bc78:	eafffe7f 	b	b67c <_dtoa_r+0x284>
    bc7c:	e1a00009 	mov	r0, r9
    bc80:	eb0017c6 	bl	11ba0 <__aeabi_i2d>
    bc84:	e1a02004 	mov	r2, r4
    bc88:	e1a03005 	mov	r3, r5
    bc8c:	eb001954 	bl	121e4 <__aeabi_dcmpeq>
    bc90:	e3500000 	cmp	r0, #0
    bc94:	02499001 	subeq	r9, r9, #1
    bc98:	eafffe5b 	b	b60c <_dtoa_r+0x214>
    bc9c:	e51f02fc 	ldr	r0, [pc, #-764]	; b9a8 <_dtoa_r+0x5b0>
    bca0:	e59d1010 	ldr	r1, [sp, #16]
    bca4:	e0670000 	rsb	r0, r7, r0
    bca8:	e1a00011 	lsl	r0, r1, r0
    bcac:	eafffe31 	b	b578 <_dtoa_r+0x180>
    bcb0:	e59dc0a0 	ldr	ip, [sp, #160]	; 0xa0
    bcb4:	e35c0001 	cmp	ip, #1
    bcb8:	da0002d9 	ble	c824 <_dtoa_r+0x142c>
    bcbc:	e59dc02c 	ldr	ip, [sp, #44]	; 0x2c
    bcc0:	e59d403c 	ldr	r4, [sp, #60]	; 0x3c
    bcc4:	e24c5001 	sub	r5, ip, #1
    bcc8:	e1540005 	cmp	r4, r5
    bccc:	b59dc03c 	ldrlt	ip, [sp, #60]	; 0x3c
    bcd0:	b59d4048 	ldrlt	r4, [sp, #72]	; 0x48
    bcd4:	b06c3005 	rsblt	r3, ip, r5
    bcd8:	e59dc02c 	ldr	ip, [sp, #44]	; 0x2c
    bcdc:	b0844003 	addlt	r4, r4, r3
    bce0:	a0655004 	rsbge	r5, r5, r4
    bce4:	b58d4048 	strlt	r4, [sp, #72]	; 0x48
    bce8:	b58d503c 	strlt	r5, [sp, #60]	; 0x3c
    bcec:	b3a05000 	movlt	r5, #0
    bcf0:	e35c0000 	cmp	ip, #0
    bcf4:	b59d1028 	ldrlt	r1, [sp, #40]	; 0x28
    bcf8:	a59d4028 	ldrge	r4, [sp, #40]	; 0x28
    bcfc:	a59d302c 	ldrge	r3, [sp, #44]	; 0x2c
    bd00:	b06c4001 	rsblt	r4, ip, r1
    bd04:	b3a03000 	movlt	r3, #0
    bd08:	e59dc028 	ldr	ip, [sp, #40]	; 0x28
    bd0c:	e1a00006 	mov	r0, r6
    bd10:	e08cc003 	add	ip, ip, r3
    bd14:	e3a01001 	mov	r1, #1
    bd18:	e58dc028 	str	ip, [sp, #40]	; 0x28
    bd1c:	e08bb003 	add	fp, fp, r3
    bd20:	eb00098c 	bl	e358 <__i2b>
    bd24:	e1a0a000 	mov	sl, r0
    bd28:	eaffff3d 	b	ba24 <_dtoa_r+0x62c>
    bd2c:	e3a0c000 	mov	ip, #0
    bd30:	e58dc040 	str	ip, [sp, #64]	; 0x40
    bd34:	e59d10a4 	ldr	r1, [sp, #164]	; 0xa4
    bd38:	e3510000 	cmp	r1, #0
    bd3c:	da000261 	ble	c6c8 <_dtoa_r+0x12d0>
    bd40:	e58d1044 	str	r1, [sp, #68]	; 0x44
    bd44:	e58d102c 	str	r1, [sp, #44]	; 0x2c
    bd48:	e1a08001 	mov	r8, r1
    bd4c:	e351000e 	cmp	r1, #14
    bd50:	83a04000 	movhi	r4, #0
    bd54:	92044001 	andls	r4, r4, #1
    bd58:	e3a01000 	mov	r1, #0
    bd5c:	e3580017 	cmp	r8, #23
    bd60:	e5861044 	str	r1, [r6, #68]	; 0x44
    bd64:	9a000008 	bls	bd8c <_dtoa_r+0x994>
    bd68:	e3a02001 	mov	r2, #1
    bd6c:	e3a03004 	mov	r3, #4
    bd70:	e1a03083 	lsl	r3, r3, #1
    bd74:	e2830014 	add	r0, r3, #20
    bd78:	e1500008 	cmp	r0, r8
    bd7c:	e1a01002 	mov	r1, r2
    bd80:	e2822001 	add	r2, r2, #1
    bd84:	9afffff9 	bls	bd70 <_dtoa_r+0x978>
    bd88:	e5861044 	str	r1, [r6, #68]	; 0x44
    bd8c:	e1a00006 	mov	r0, r6
    bd90:	eb00089a 	bl	e000 <_Balloc>
    bd94:	e3540000 	cmp	r4, #0
    bd98:	e58d0024 	str	r0, [sp, #36]	; 0x24
    bd9c:	e5860040 	str	r0, [r6, #64]	; 0x40
    bda0:	0afffe6a 	beq	b750 <_dtoa_r+0x358>
    bda4:	e3590000 	cmp	r9, #0
    bda8:	da000133 	ble	c27c <_dtoa_r+0xe84>
    bdac:	e51f3400 	ldr	r3, [pc, #-1024]	; b9b4 <_dtoa_r+0x5bc>
    bdb0:	e209200f 	and	r2, r9, #15
    bdb4:	e1a07249 	asr	r7, r9, #4
    bdb8:	e0833182 	add	r3, r3, r2, lsl #3
    bdbc:	e3170010 	tst	r7, #16
    bdc0:	e8930030 	ldm	r3, {r4, r5}
    bdc4:	0a000122 	beq	c254 <_dtoa_r+0xe5c>
    bdc8:	e51f3420 	ldr	r3, [pc, #-1056]	; b9b0 <_dtoa_r+0x5b8>
    bdcc:	e28d1010 	add	r1, sp, #16
    bdd0:	e8910003 	ldm	r1, {r0, r1}
    bdd4:	e2833020 	add	r3, r3, #32
    bdd8:	e893000c 	ldm	r3, {r2, r3}
    bddc:	eb00184a 	bl	11f0c <__aeabi_ddiv>
    bde0:	e58d0030 	str	r0, [sp, #48]	; 0x30
    bde4:	e58d1034 	str	r1, [sp, #52]	; 0x34
    bde8:	e207700f 	and	r7, r7, #15
    bdec:	e3a08003 	mov	r8, #3
    bdf0:	e3570000 	cmp	r7, #0
    bdf4:	0a00000c 	beq	be2c <_dtoa_r+0xa34>
    bdf8:	e51fa450 	ldr	sl, [pc, #-1104]	; b9b0 <_dtoa_r+0x5b8>
    bdfc:	e1a00004 	mov	r0, r4
    be00:	e1a01005 	mov	r1, r5
    be04:	e3170001 	tst	r7, #1
    be08:	0a000002 	beq	be18 <_dtoa_r+0xa20>
    be0c:	e89a000c 	ldm	sl, {r2, r3}
    be10:	eb001799 	bl	11c7c <__aeabi_dmul>
    be14:	e2888001 	add	r8, r8, #1
    be18:	e1b070c7 	asrs	r7, r7, #1
    be1c:	e28aa008 	add	sl, sl, #8
    be20:	1afffff7 	bne	be04 <_dtoa_r+0xa0c>
    be24:	e1a04000 	mov	r4, r0
    be28:	e1a05001 	mov	r5, r1
    be2c:	e28d1030 	add	r1, sp, #48	; 0x30
    be30:	e8910003 	ldm	r1, {r0, r1}
    be34:	e1a02004 	mov	r2, r4
    be38:	e1a03005 	mov	r3, r5
    be3c:	eb001832 	bl	11f0c <__aeabi_ddiv>
    be40:	e58d0030 	str	r0, [sp, #48]	; 0x30
    be44:	e58d1034 	str	r1, [sp, #52]	; 0x34
    be48:	e59d404c 	ldr	r4, [sp, #76]	; 0x4c
    be4c:	e3540000 	cmp	r4, #0
    be50:	0a000006 	beq	be70 <_dtoa_r+0xa78>
    be54:	e28d1030 	add	r1, sp, #48	; 0x30
    be58:	e8910003 	ldm	r1, {r0, r1}
    be5c:	e3a02000 	mov	r2, #0
    be60:	e51f34bc 	ldr	r3, [pc, #-1212]	; b9ac <_dtoa_r+0x5b4>
    be64:	eb0018e4 	bl	121fc <__aeabi_dcmplt>
    be68:	e3500000 	cmp	r0, #0
    be6c:	1a000234 	bne	c744 <_dtoa_r+0x134c>
    be70:	e1a00008 	mov	r0, r8
    be74:	eb001749 	bl	11ba0 <__aeabi_i2d>
    be78:	e28d3030 	add	r3, sp, #48	; 0x30
    be7c:	e893000c 	ldm	r3, {r2, r3}
    be80:	eb00177d 	bl	11c7c <__aeabi_dmul>
    be84:	e3a02000 	mov	r2, #0
    be88:	e51f34cc 	ldr	r3, [pc, #-1228]	; b9c4 <_dtoa_r+0x5cc>
    be8c:	eb001676 	bl	1186c <__adddf3>
    be90:	e59dc02c 	ldr	ip, [sp, #44]	; 0x2c
    be94:	e35c0000 	cmp	ip, #0
    be98:	e1a04000 	mov	r4, r0
    be9c:	e241550d 	sub	r5, r1, #54525952	; 0x3400000
    bea0:	0a000093 	beq	c0f4 <_dtoa_r+0xcfc>
    bea4:	e58d9064 	str	r9, [sp, #100]	; 0x64
    bea8:	e59da02c 	ldr	sl, [sp, #44]	; 0x2c
    beac:	e59dc040 	ldr	ip, [sp, #64]	; 0x40
    beb0:	e35c0000 	cmp	ip, #0
    beb4:	0a00010e 	beq	c2f4 <_dtoa_r+0xefc>
    beb8:	e51f350c 	ldr	r3, [pc, #-1292]	; b9b4 <_dtoa_r+0x5bc>
    bebc:	e083318a 	add	r3, r3, sl, lsl #3
    bec0:	e913000c 	ldmdb	r3, {r2, r3}
    bec4:	e3a00000 	mov	r0, #0
    bec8:	e51f1508 	ldr	r1, [pc, #-1288]	; b9c8 <_dtoa_r+0x5d0>
    becc:	eb00180e 	bl	11f0c <__aeabi_ddiv>
    bed0:	e1a02004 	mov	r2, r4
    bed4:	e1a03005 	mov	r3, r5
    bed8:	eb001662 	bl	11868 <__aeabi_dsub>
    bedc:	e58d0050 	str	r0, [sp, #80]	; 0x50
    bee0:	e58d1054 	str	r1, [sp, #84]	; 0x54
    bee4:	e28d1030 	add	r1, sp, #48	; 0x30
    bee8:	e8910003 	ldm	r1, {r0, r1}
    beec:	eb0018da 	bl	1225c <__aeabi_d2iz>
    bef0:	e1a07000 	mov	r7, r0
    bef4:	eb001729 	bl	11ba0 <__aeabi_i2d>
    bef8:	e1a02000 	mov	r2, r0
    befc:	e1a03001 	mov	r3, r1
    bf00:	e28d1030 	add	r1, sp, #48	; 0x30
    bf04:	e8910003 	ldm	r1, {r0, r1}
    bf08:	eb001656 	bl	11868 <__aeabi_dsub>
    bf0c:	e2877030 	add	r7, r7, #48	; 0x30
    bf10:	e59dc024 	ldr	ip, [sp, #36]	; 0x24
    bf14:	e20780ff 	and	r8, r7, #255	; 0xff
    bf18:	e4cc8001 	strb	r8, [ip], #1
    bf1c:	e1a04000 	mov	r4, r0
    bf20:	e1a05001 	mov	r5, r1
    bf24:	e1a02004 	mov	r2, r4
    bf28:	e28d1050 	add	r1, sp, #80	; 0x50
    bf2c:	e8910003 	ldm	r1, {r0, r1}
    bf30:	e1a03005 	mov	r3, r5
    bf34:	e58dc030 	str	ip, [sp, #48]	; 0x30
    bf38:	eb0018c1 	bl	12244 <__aeabi_dcmpgt>
    bf3c:	e3500000 	cmp	r0, #0
    bf40:	e59dc030 	ldr	ip, [sp, #48]	; 0x30
    bf44:	1a0002c8 	bne	ca6c <_dtoa_r+0x1674>
    bf48:	e1a02004 	mov	r2, r4
    bf4c:	e1a03005 	mov	r3, r5
    bf50:	e3a00000 	mov	r0, #0
    bf54:	e51f15b0 	ldr	r1, [pc, #-1456]	; b9ac <_dtoa_r+0x5b4>
    bf58:	e58dc00c 	str	ip, [sp, #12]
    bf5c:	eb001641 	bl	11868 <__aeabi_dsub>
    bf60:	e1a02000 	mov	r2, r0
    bf64:	e1a03001 	mov	r3, r1
    bf68:	e28d1050 	add	r1, sp, #80	; 0x50
    bf6c:	e8910003 	ldm	r1, {r0, r1}
    bf70:	eb0018b3 	bl	12244 <__aeabi_dcmpgt>
    bf74:	e3500000 	cmp	r0, #0
    bf78:	e59dc00c 	ldr	ip, [sp, #12]
    bf7c:	1a00024d 	bne	c8b8 <_dtoa_r+0x14c0>
    bf80:	e35a0001 	cmp	sl, #1
    bf84:	dafffdf1 	ble	b750 <_dtoa_r+0x358>
    bf88:	e59dc024 	ldr	ip, [sp, #36]	; 0x24
    bf8c:	e59d7030 	ldr	r7, [sp, #48]	; 0x30
    bf90:	e08ca00a 	add	sl, ip, sl
    bf94:	e58d9068 	str	r9, [sp, #104]	; 0x68
    bf98:	e58d6058 	str	r6, [sp, #88]	; 0x58
    bf9c:	e1a0900a 	mov	r9, sl
    bfa0:	e58db06c 	str	fp, [sp, #108]	; 0x6c
    bfa4:	e1a0a007 	mov	sl, r7
    bfa8:	e28d7050 	add	r7, sp, #80	; 0x50
    bfac:	e89700c0 	ldm	r7, {r6, r7}
    bfb0:	ea000007 	b	bfd4 <_dtoa_r+0xbdc>
    bfb4:	eb00162b 	bl	11868 <__aeabi_dsub>
    bfb8:	e1a02006 	mov	r2, r6
    bfbc:	e1a03007 	mov	r3, r7
    bfc0:	eb00188d 	bl	121fc <__aeabi_dcmplt>
    bfc4:	e3500000 	cmp	r0, #0
    bfc8:	1a000238 	bne	c8b0 <_dtoa_r+0x14b8>
    bfcc:	e15a0009 	cmp	sl, r9
    bfd0:	0a00023f 	beq	c8d4 <_dtoa_r+0x14dc>
    bfd4:	e3a02000 	mov	r2, #0
    bfd8:	e51f3620 	ldr	r3, [pc, #-1568]	; b9c0 <_dtoa_r+0x5c8>
    bfdc:	e1a00006 	mov	r0, r6
    bfe0:	e1a01007 	mov	r1, r7
    bfe4:	eb001724 	bl	11c7c <__aeabi_dmul>
    bfe8:	e3a02000 	mov	r2, #0
    bfec:	e51f3634 	ldr	r3, [pc, #-1588]	; b9c0 <_dtoa_r+0x5c8>
    bff0:	e1a06000 	mov	r6, r0
    bff4:	e1a07001 	mov	r7, r1
    bff8:	e1a00004 	mov	r0, r4
    bffc:	e1a01005 	mov	r1, r5
    c000:	eb00171d 	bl	11c7c <__aeabi_dmul>
    c004:	e1a05001 	mov	r5, r1
    c008:	e1a04000 	mov	r4, r0
    c00c:	eb001892 	bl	1225c <__aeabi_d2iz>
    c010:	e1a08000 	mov	r8, r0
    c014:	eb0016e1 	bl	11ba0 <__aeabi_i2d>
    c018:	e1a02000 	mov	r2, r0
    c01c:	e1a03001 	mov	r3, r1
    c020:	e1a00004 	mov	r0, r4
    c024:	e1a01005 	mov	r1, r5
    c028:	eb00160e 	bl	11868 <__aeabi_dsub>
    c02c:	e2888030 	add	r8, r8, #48	; 0x30
    c030:	e20880ff 	and	r8, r8, #255	; 0xff
    c034:	e4ca8001 	strb	r8, [sl], #1
    c038:	e1a02006 	mov	r2, r6
    c03c:	e1a03007 	mov	r3, r7
    c040:	e1a05001 	mov	r5, r1
    c044:	e1a04000 	mov	r4, r0
    c048:	eb00186b 	bl	121fc <__aeabi_dcmplt>
    c04c:	e3500000 	cmp	r0, #0
    c050:	e1a02004 	mov	r2, r4
    c054:	e1a03005 	mov	r3, r5
    c058:	e3a00000 	mov	r0, #0
    c05c:	e51f16b8 	ldr	r1, [pc, #-1720]	; b9ac <_dtoa_r+0x5b4>
    c060:	e1a0b00a 	mov	fp, sl
    c064:	0affffd2 	beq	bfb4 <_dtoa_r+0xbbc>
    c068:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    c06c:	e1a0700a 	mov	r7, sl
    c070:	e59d6058 	ldr	r6, [sp, #88]	; 0x58
    c074:	e59d9064 	ldr	r9, [sp, #100]	; 0x64
    c078:	e58da024 	str	sl, [sp, #36]	; 0x24
    c07c:	eafffee7 	b	bc20 <_dtoa_r+0x828>
    c080:	e3a0c001 	mov	ip, #1
    c084:	e58dc040 	str	ip, [sp, #64]	; 0x40
    c088:	e59dc0a4 	ldr	ip, [sp, #164]	; 0xa4
    c08c:	e08cc009 	add	ip, ip, r9
    c090:	e28c8001 	add	r8, ip, #1
    c094:	e3580000 	cmp	r8, #0
    c098:	e58dc044 	str	ip, [sp, #68]	; 0x44
    c09c:	da000191 	ble	c6e8 <_dtoa_r+0x12f0>
    c0a0:	e358000e 	cmp	r8, #14
    c0a4:	83a04000 	movhi	r4, #0
    c0a8:	92044001 	andls	r4, r4, #1
    c0ac:	e58d802c 	str	r8, [sp, #44]	; 0x2c
    c0b0:	eaffff28 	b	bd58 <_dtoa_r+0x960>
    c0b4:	e3a0c001 	mov	ip, #1
    c0b8:	e58dc040 	str	ip, [sp, #64]	; 0x40
    c0bc:	eaffff1c 	b	bd34 <_dtoa_r+0x93c>
    c0c0:	e3a0c000 	mov	ip, #0
    c0c4:	e58dc040 	str	ip, [sp, #64]	; 0x40
    c0c8:	eaffffee 	b	c088 <_dtoa_r+0xc90>
    c0cc:	e1a00008 	mov	r0, r8
    c0d0:	eb0016b2 	bl	11ba0 <__aeabi_i2d>
    c0d4:	e28d3030 	add	r3, sp, #48	; 0x30
    c0d8:	e893000c 	ldm	r3, {r2, r3}
    c0dc:	eb0016e6 	bl	11c7c <__aeabi_dmul>
    c0e0:	e3a02000 	mov	r2, #0
    c0e4:	e51f3728 	ldr	r3, [pc, #-1832]	; b9c4 <_dtoa_r+0x5cc>
    c0e8:	eb0015df 	bl	1186c <__adddf3>
    c0ec:	e1a04000 	mov	r4, r0
    c0f0:	e241550d 	sub	r5, r1, #54525952	; 0x3400000
    c0f4:	e3a02000 	mov	r2, #0
    c0f8:	e51f3748 	ldr	r3, [pc, #-1864]	; b9b8 <_dtoa_r+0x5c0>
    c0fc:	e28d1030 	add	r1, sp, #48	; 0x30
    c100:	e8910003 	ldm	r1, {r0, r1}
    c104:	eb0015d7 	bl	11868 <__aeabi_dsub>
    c108:	e1a02004 	mov	r2, r4
    c10c:	e1a03005 	mov	r3, r5
    c110:	e1a07000 	mov	r7, r0
    c114:	e1a08001 	mov	r8, r1
    c118:	eb001849 	bl	12244 <__aeabi_dcmpgt>
    c11c:	e250a000 	subs	sl, r0, #0
    c120:	1a000070 	bne	c2e8 <_dtoa_r+0xef0>
    c124:	e1a02004 	mov	r2, r4
    c128:	e2853102 	add	r3, r5, #-2147483648	; 0x80000000
    c12c:	e1a00007 	mov	r0, r7
    c130:	e1a01008 	mov	r1, r8
    c134:	eb001830 	bl	121fc <__aeabi_dcmplt>
    c138:	e3500000 	cmp	r0, #0
    c13c:	0afffd83 	beq	b750 <_dtoa_r+0x358>
    c140:	e1a0700a 	mov	r7, sl
    c144:	e59dc0a4 	ldr	ip, [sp, #164]	; 0xa4
    c148:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    c14c:	e1e0900c 	mvn	r9, ip
    c150:	e3a05000 	mov	r5, #0
    c154:	eafffea6 	b	bbf4 <_dtoa_r+0x7fc>
    c158:	e59dc040 	ldr	ip, [sp, #64]	; 0x40
    c15c:	e35c0000 	cmp	ip, #0
    c160:	1a0000ec 	bne	c518 <_dtoa_r+0x1120>
    c164:	e3a04000 	mov	r4, #0
    c168:	e59d5038 	ldr	r5, [sp, #56]	; 0x38
    c16c:	e59d802c 	ldr	r8, [sp, #44]	; 0x2c
    c170:	e59db024 	ldr	fp, [sp, #36]	; 0x24
    c174:	ea000005 	b	c190 <_dtoa_r+0xd98>
    c178:	e1a01005 	mov	r1, r5
    c17c:	e1a00006 	mov	r0, r6
    c180:	e3a0200a 	mov	r2, #10
    c184:	e3a03000 	mov	r3, #0
    c188:	eb0007c6 	bl	e0a8 <__multadd>
    c18c:	e1a05000 	mov	r5, r0
    c190:	e1a01007 	mov	r1, r7
    c194:	e1a00005 	mov	r0, r5
    c198:	ebfffc1e 	bl	b218 <quorem>
    c19c:	e2800030 	add	r0, r0, #48	; 0x30
    c1a0:	e7cb0004 	strb	r0, [fp, r4]
    c1a4:	e2844001 	add	r4, r4, #1
    c1a8:	e1540008 	cmp	r4, r8
    c1ac:	bafffff1 	blt	c178 <_dtoa_r+0xd80>
    c1b0:	e59dc024 	ldr	ip, [sp, #36]	; 0x24
    c1b4:	e59d402c 	ldr	r4, [sp, #44]	; 0x2c
    c1b8:	e58d5038 	str	r5, [sp, #56]	; 0x38
    c1bc:	e1a0b000 	mov	fp, r0
    c1c0:	e3540001 	cmp	r4, #1
    c1c4:	a08cc004 	addge	ip, ip, r4
    c1c8:	b28cc001 	addlt	ip, ip, #1
    c1cc:	e3a05000 	mov	r5, #0
    c1d0:	e3a02001 	mov	r2, #1
    c1d4:	e59d1038 	ldr	r1, [sp, #56]	; 0x38
    c1d8:	e1a00006 	mov	r0, r6
    c1dc:	e58dc00c 	str	ip, [sp, #12]
    c1e0:	eb00091b 	bl	e654 <__lshift>
    c1e4:	e1a01007 	mov	r1, r7
    c1e8:	e58d0038 	str	r0, [sp, #56]	; 0x38
    c1ec:	eb000959 	bl	e758 <__mcmp>
    c1f0:	e3500000 	cmp	r0, #0
    c1f4:	e59dc00c 	ldr	ip, [sp, #12]
    c1f8:	da0001b9 	ble	c8e4 <_dtoa_r+0x14ec>
    c1fc:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    c200:	e2844001 	add	r4, r4, #1
    c204:	e55c3001 	ldrb	r3, [ip, #-1]
    c208:	e58d4030 	str	r4, [sp, #48]	; 0x30
    c20c:	e1a0b00c 	mov	fp, ip
    c210:	e59d1030 	ldr	r1, [sp, #48]	; 0x30
    c214:	ea000001 	b	c220 <_dtoa_r+0xe28>
    c218:	e55b3002 	ldrb	r3, [fp, #-2]
    c21c:	e1a0b002 	mov	fp, r2
    c220:	e3530039 	cmp	r3, #57	; 0x39
    c224:	e24b2001 	sub	r2, fp, #1
    c228:	1a0001cc 	bne	c960 <_dtoa_r+0x1568>
    c22c:	e15b0001 	cmp	fp, r1
    c230:	1afffff8 	bne	c218 <_dtoa_r+0xe20>
    c234:	e3a03031 	mov	r3, #49	; 0x31
    c238:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    c23c:	e2899001 	add	r9, r9, #1
    c240:	e5c23000 	strb	r3, [r2]
    c244:	e58db024 	str	fp, [sp, #36]	; 0x24
    c248:	eafffe69 	b	bbf4 <_dtoa_r+0x7fc>
    c24c:	e3a0301c 	mov	r3, #28
    c250:	eafffe32 	b	bb20 <_dtoa_r+0x728>
    c254:	e28d3010 	add	r3, sp, #16
    c258:	e893000c 	ldm	r3, {r2, r3}
    c25c:	e3a08002 	mov	r8, #2
    c260:	e58d2030 	str	r2, [sp, #48]	; 0x30
    c264:	e58d3034 	str	r3, [sp, #52]	; 0x34
    c268:	eafffee0 	b	bdf0 <_dtoa_r+0x9f8>
    c26c:	e1a00006 	mov	r0, r6
    c270:	e1a01005 	mov	r1, r5
    c274:	eb000784 	bl	e08c <_Bfree>
    c278:	eafffe65 	b	bc14 <_dtoa_r+0x81c>
    c27c:	e2694000 	rsb	r4, r9, #0
    c280:	e3540000 	cmp	r4, #0
    c284:	0a000128 	beq	c72c <_dtoa_r+0x1334>
    c288:	e51f38dc 	ldr	r3, [pc, #-2268]	; b9b4 <_dtoa_r+0x5bc>
    c28c:	e204200f 	and	r2, r4, #15
    c290:	e0833182 	add	r3, r3, r2, lsl #3
    c294:	e28d1010 	add	r1, sp, #16
    c298:	e8910003 	ldm	r1, {r0, r1}
    c29c:	e893000c 	ldm	r3, {r2, r3}
    c2a0:	eb001675 	bl	11c7c <__aeabi_dmul>
    c2a4:	e1b04244 	asrs	r4, r4, #4
    c2a8:	e58d0030 	str	r0, [sp, #48]	; 0x30
    c2ac:	e58d1034 	str	r1, [sp, #52]	; 0x34
    c2b0:	0a0001d4 	beq	ca08 <_dtoa_r+0x1610>
    c2b4:	e51f590c 	ldr	r5, [pc, #-2316]	; b9b0 <_dtoa_r+0x5b8>
    c2b8:	e3a08002 	mov	r8, #2
    c2bc:	e3140001 	tst	r4, #1
    c2c0:	0a000002 	beq	c2d0 <_dtoa_r+0xed8>
    c2c4:	e895000c 	ldm	r5, {r2, r3}
    c2c8:	eb00166b 	bl	11c7c <__aeabi_dmul>
    c2cc:	e2888001 	add	r8, r8, #1
    c2d0:	e1b040c4 	asrs	r4, r4, #1
    c2d4:	e2855008 	add	r5, r5, #8
    c2d8:	1afffff7 	bne	c2bc <_dtoa_r+0xec4>
    c2dc:	e58d0030 	str	r0, [sp, #48]	; 0x30
    c2e0:	e58d1034 	str	r1, [sp, #52]	; 0x34
    c2e4:	eafffed7 	b	be48 <_dtoa_r+0xa50>
    c2e8:	e3a07000 	mov	r7, #0
    c2ec:	e1a0a007 	mov	sl, r7
    c2f0:	eafffe38 	b	bbd8 <_dtoa_r+0x7e0>
    c2f4:	e51f1948 	ldr	r1, [pc, #-2376]	; b9b4 <_dtoa_r+0x5bc>
    c2f8:	e24ac001 	sub	ip, sl, #1
    c2fc:	e081118c 	add	r1, r1, ip, lsl #3
    c300:	e1a02004 	mov	r2, r4
    c304:	e1a03005 	mov	r3, r5
    c308:	e8910003 	ldm	r1, {r0, r1}
    c30c:	e58dc050 	str	ip, [sp, #80]	; 0x50
    c310:	eb001659 	bl	11c7c <__aeabi_dmul>
    c314:	e58d0058 	str	r0, [sp, #88]	; 0x58
    c318:	e58d105c 	str	r1, [sp, #92]	; 0x5c
    c31c:	e28d1030 	add	r1, sp, #48	; 0x30
    c320:	e8910003 	ldm	r1, {r0, r1}
    c324:	eb0017cc 	bl	1225c <__aeabi_d2iz>
    c328:	e1a07000 	mov	r7, r0
    c32c:	eb00161b 	bl	11ba0 <__aeabi_i2d>
    c330:	e1a02000 	mov	r2, r0
    c334:	e1a03001 	mov	r3, r1
    c338:	e28d1030 	add	r1, sp, #48	; 0x30
    c33c:	e8910003 	ldm	r1, {r0, r1}
    c340:	eb001548 	bl	11868 <__aeabi_dsub>
    c344:	e59dc024 	ldr	ip, [sp, #36]	; 0x24
    c348:	e2877030 	add	r7, r7, #48	; 0x30
    c34c:	e4cc7001 	strb	r7, [ip], #1
    c350:	e35a0001 	cmp	sl, #1
    c354:	e1a04000 	mov	r4, r0
    c358:	e1a05001 	mov	r5, r1
    c35c:	e58dc030 	str	ip, [sp, #48]	; 0x30
    c360:	0a000019 	beq	c3cc <_dtoa_r+0xfd4>
    c364:	e59dc024 	ldr	ip, [sp, #36]	; 0x24
    c368:	e24c3001 	sub	r3, ip, #1
    c36c:	e1a0700c 	mov	r7, ip
    c370:	e083a00a 	add	sl, r3, sl
    c374:	e3a02000 	mov	r2, #0
    c378:	e51f39c0 	ldr	r3, [pc, #-2496]	; b9c0 <_dtoa_r+0x5c8>
    c37c:	eb00163e 	bl	11c7c <__aeabi_dmul>
    c380:	e1a05001 	mov	r5, r1
    c384:	e1a04000 	mov	r4, r0
    c388:	eb0017b3 	bl	1225c <__aeabi_d2iz>
    c38c:	e1a08000 	mov	r8, r0
    c390:	eb001602 	bl	11ba0 <__aeabi_i2d>
    c394:	e2888030 	add	r8, r8, #48	; 0x30
    c398:	e1a02000 	mov	r2, r0
    c39c:	e1a03001 	mov	r3, r1
    c3a0:	e1a00004 	mov	r0, r4
    c3a4:	e1a01005 	mov	r1, r5
    c3a8:	eb00152e 	bl	11868 <__aeabi_dsub>
    c3ac:	e5e78001 	strb	r8, [r7, #1]!
    c3b0:	e157000a 	cmp	r7, sl
    c3b4:	1affffee 	bne	c374 <_dtoa_r+0xf7c>
    c3b8:	e1a05001 	mov	r5, r1
    c3bc:	e59d2050 	ldr	r2, [sp, #80]	; 0x50
    c3c0:	e59d1030 	ldr	r1, [sp, #48]	; 0x30
    c3c4:	e1a04000 	mov	r4, r0
    c3c8:	e081c002 	add	ip, r1, r2
    c3cc:	e3a02000 	mov	r2, #0
    c3d0:	e51f3a10 	ldr	r3, [pc, #-2576]	; b9c8 <_dtoa_r+0x5d0>
    c3d4:	e28d1058 	add	r1, sp, #88	; 0x58
    c3d8:	e8910003 	ldm	r1, {r0, r1}
    c3dc:	e58dc00c 	str	ip, [sp, #12]
    c3e0:	eb001521 	bl	1186c <__adddf3>
    c3e4:	e1a02004 	mov	r2, r4
    c3e8:	e1a03005 	mov	r3, r5
    c3ec:	eb001782 	bl	121fc <__aeabi_dcmplt>
    c3f0:	e3500000 	cmp	r0, #0
    c3f4:	e59dc00c 	ldr	ip, [sp, #12]
    c3f8:	0a0000ee 	beq	c7b8 <_dtoa_r+0x13c0>
    c3fc:	e55c8001 	ldrb	r8, [ip, #-1]
    c400:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    c404:	eafffd57 	b	b968 <_dtoa_r+0x570>
    c408:	e5973010 	ldr	r3, [r7, #16]
    c40c:	e0873103 	add	r3, r7, r3, lsl #2
    c410:	e5930010 	ldr	r0, [r3, #16]
    c414:	eb000796 	bl	e274 <__hi0bits>
    c418:	e2600020 	rsb	r0, r0, #32
    c41c:	eafffdb8 	b	bb04 <_dtoa_r+0x70c>
    c420:	e59d0038 	ldr	r0, [sp, #56]	; 0x38
    c424:	e1a01007 	mov	r1, r7
    c428:	eb0008ca 	bl	e758 <__mcmp>
    c42c:	e3500000 	cmp	r0, #0
    c430:	aafffdd1 	bge	bb7c <_dtoa_r+0x784>
    c434:	e1a00006 	mov	r0, r6
    c438:	e59d1038 	ldr	r1, [sp, #56]	; 0x38
    c43c:	e3a0200a 	mov	r2, #10
    c440:	e3a03000 	mov	r3, #0
    c444:	eb000717 	bl	e0a8 <__multadd>
    c448:	e59dc040 	ldr	ip, [sp, #64]	; 0x40
    c44c:	e35c0000 	cmp	ip, #0
    c450:	e58d0038 	str	r0, [sp, #56]	; 0x38
    c454:	e2499001 	sub	r9, r9, #1
    c458:	1a000170 	bne	ca20 <_dtoa_r+0x1628>
    c45c:	e59d4044 	ldr	r4, [sp, #68]	; 0x44
    c460:	e59dc0a0 	ldr	ip, [sp, #160]	; 0xa0
    c464:	e3540000 	cmp	r4, #0
    c468:	c3a03000 	movgt	r3, #0
    c46c:	d3a03001 	movle	r3, #1
    c470:	e35c0002 	cmp	ip, #2
    c474:	d3a03000 	movle	r3, #0
    c478:	e3530000 	cmp	r3, #0
    c47c:	058d402c 	streq	r4, [sp, #44]	; 0x2c
    c480:	0affff37 	beq	c164 <_dtoa_r+0xd6c>
    c484:	e59dc044 	ldr	ip, [sp, #68]	; 0x44
    c488:	e58dc02c 	str	ip, [sp, #44]	; 0x2c
    c48c:	eafffdc3 	b	bba0 <_dtoa_r+0x7a8>
    c490:	e59d402c 	ldr	r4, [sp, #44]	; 0x2c
    c494:	e3540000 	cmp	r4, #0
    c498:	1a00011d 	bne	c914 <_dtoa_r+0x151c>
    c49c:	e3a02000 	mov	r2, #0
    c4a0:	e51f3af0 	ldr	r3, [pc, #-2800]	; b9b8 <_dtoa_r+0x5c0>
    c4a4:	e28d1018 	add	r1, sp, #24
    c4a8:	e8910003 	ldm	r1, {r0, r1}
    c4ac:	eb0015f2 	bl	11c7c <__aeabi_dmul>
    c4b0:	e28d3010 	add	r3, sp, #16
    c4b4:	e893000c 	ldm	r3, {r2, r3}
    c4b8:	eb00175b 	bl	1222c <__aeabi_dcmpge>
    c4bc:	e59d702c 	ldr	r7, [sp, #44]	; 0x2c
    c4c0:	e3500000 	cmp	r0, #0
    c4c4:	e1a0a007 	mov	sl, r7
    c4c8:	1affff1d 	bne	c144 <_dtoa_r+0xd4c>
    c4cc:	eafffdc1 	b	bbd8 <_dtoa_r+0x7e0>
    c4d0:	e59d1010 	ldr	r1, [sp, #16]
    c4d4:	e3510000 	cmp	r1, #0
    c4d8:	1afffd84 	bne	baf0 <_dtoa_r+0x6f8>
    c4dc:	e59d3014 	ldr	r3, [sp, #20]
    c4e0:	e3c324ff 	bic	r2, r3, #-16777216	; 0xff000000
    c4e4:	e3c2260f 	bic	r2, r2, #15728640	; 0xf00000
    c4e8:	e3520000 	cmp	r2, #0
    c4ec:	1a000120 	bne	c974 <_dtoa_r+0x157c>
    c4f0:	e51f5b3c 	ldr	r5, [pc, #-2876]	; b9bc <_dtoa_r+0x5c4>
    c4f4:	e0035005 	and	r5, r3, r5
    c4f8:	e3550000 	cmp	r5, #0
    c4fc:	0afffd7c 	beq	baf4 <_dtoa_r+0x6fc>
    c500:	e59dc028 	ldr	ip, [sp, #40]	; 0x28
    c504:	e28cc001 	add	ip, ip, #1
    c508:	e58dc028 	str	ip, [sp, #40]	; 0x28
    c50c:	e28bb001 	add	fp, fp, #1
    c510:	e3a05001 	mov	r5, #1
    c514:	eafffd76 	b	baf4 <_dtoa_r+0x6fc>
    c518:	e3540000 	cmp	r4, #0
    c51c:	da000004 	ble	c534 <_dtoa_r+0x113c>
    c520:	e1a0100a 	mov	r1, sl
    c524:	e1a00006 	mov	r0, r6
    c528:	e1a02004 	mov	r2, r4
    c52c:	eb000848 	bl	e654 <__lshift>
    c530:	e1a0a000 	mov	sl, r0
    c534:	e3550000 	cmp	r5, #0
    c538:	01a0800a 	moveq	r8, sl
    c53c:	1a0000f7 	bne	c920 <_dtoa_r+0x1528>
    c540:	e59d102c 	ldr	r1, [sp, #44]	; 0x2c
    c544:	e59dc024 	ldr	ip, [sp, #36]	; 0x24
    c548:	e08cc001 	add	ip, ip, r1
    c54c:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    c550:	e58dc02c 	str	ip, [sp, #44]	; 0x2c
    c554:	e59dc010 	ldr	ip, [sp, #16]
    c558:	e2844001 	add	r4, r4, #1
    c55c:	e20cc001 	and	ip, ip, #1
    c560:	e58d903c 	str	r9, [sp, #60]	; 0x3c
    c564:	e58d4030 	str	r4, [sp, #48]	; 0x30
    c568:	e1a09007 	mov	r9, r7
    c56c:	e1a0500a 	mov	r5, sl
    c570:	e58dc020 	str	ip, [sp, #32]
    c574:	e59d7038 	ldr	r7, [sp, #56]	; 0x38
    c578:	ea00002e 	b	c638 <_dtoa_r+0x1240>
    c57c:	e1a00006 	mov	r0, r6
    c580:	e1a01003 	mov	r1, r3
    c584:	e58d200c 	str	r2, [sp, #12]
    c588:	eb0006bf 	bl	e08c <_Bfree>
    c58c:	e59d200c 	ldr	r2, [sp, #12]
    c590:	e59dc0a0 	ldr	ip, [sp, #160]	; 0xa0
    c594:	e192c00c 	orrs	ip, r2, ip
    c598:	1a000002 	bne	c5a8 <_dtoa_r+0x11b0>
    c59c:	e59dc020 	ldr	ip, [sp, #32]
    c5a0:	e35c0000 	cmp	ip, #0
    c5a4:	0a00010a 	beq	c9d4 <_dtoa_r+0x15dc>
    c5a8:	e35a0000 	cmp	sl, #0
    c5ac:	ba0000a6 	blt	c84c <_dtoa_r+0x1454>
    c5b0:	e59dc0a0 	ldr	ip, [sp, #160]	; 0xa0
    c5b4:	e19ac00c 	orrs	ip, sl, ip
    c5b8:	1a000002 	bne	c5c8 <_dtoa_r+0x11d0>
    c5bc:	e59dc020 	ldr	ip, [sp, #32]
    c5c0:	e35c0000 	cmp	ip, #0
    c5c4:	0a0000a0 	beq	c84c <_dtoa_r+0x1454>
    c5c8:	e3520000 	cmp	r2, #0
    c5cc:	ca0000ea 	bgt	c97c <_dtoa_r+0x1584>
    c5d0:	e59dc02c 	ldr	ip, [sp, #44]	; 0x2c
    c5d4:	e154000c 	cmp	r4, ip
    c5d8:	e544b001 	strb	fp, [r4, #-1]
    c5dc:	e1a0c004 	mov	ip, r4
    c5e0:	0a0000f1 	beq	c9ac <_dtoa_r+0x15b4>
    c5e4:	e1a01007 	mov	r1, r7
    c5e8:	e1a00006 	mov	r0, r6
    c5ec:	e3a0200a 	mov	r2, #10
    c5f0:	e3a03000 	mov	r3, #0
    c5f4:	eb0006ab 	bl	e0a8 <__multadd>
    c5f8:	e1550008 	cmp	r5, r8
    c5fc:	e1a07000 	mov	r7, r0
    c600:	0a000028 	beq	c6a8 <_dtoa_r+0x12b0>
    c604:	e1a01005 	mov	r1, r5
    c608:	e3a0200a 	mov	r2, #10
    c60c:	e3a03000 	mov	r3, #0
    c610:	e1a00006 	mov	r0, r6
    c614:	eb0006a3 	bl	e0a8 <__multadd>
    c618:	e1a01008 	mov	r1, r8
    c61c:	e1a05000 	mov	r5, r0
    c620:	e3a0200a 	mov	r2, #10
    c624:	e1a00006 	mov	r0, r6
    c628:	e3a03000 	mov	r3, #0
    c62c:	eb00069d 	bl	e0a8 <__multadd>
    c630:	e1a08000 	mov	r8, r0
    c634:	e2844001 	add	r4, r4, #1
    c638:	e1a01009 	mov	r1, r9
    c63c:	e1a00007 	mov	r0, r7
    c640:	ebfffaf4 	bl	b218 <quorem>
    c644:	e1a01005 	mov	r1, r5
    c648:	e58d0010 	str	r0, [sp, #16]
    c64c:	e1a00007 	mov	r0, r7
    c650:	eb000840 	bl	e758 <__mcmp>
    c654:	e1a02008 	mov	r2, r8
    c658:	e1a0a000 	mov	sl, r0
    c65c:	e1a01009 	mov	r1, r9
    c660:	e1a00006 	mov	r0, r6
    c664:	eb000854 	bl	e7bc <__mdiff>
    c668:	e244c001 	sub	ip, r4, #1
    c66c:	e590200c 	ldr	r2, [r0, #12]
    c670:	e58dc028 	str	ip, [sp, #40]	; 0x28
    c674:	e59dc010 	ldr	ip, [sp, #16]
    c678:	e3520000 	cmp	r2, #0
    c67c:	e1a03000 	mov	r3, r0
    c680:	e28cb030 	add	fp, ip, #48	; 0x30
    c684:	13a02001 	movne	r2, #1
    c688:	1affffbb 	bne	c57c <_dtoa_r+0x1184>
    c68c:	e1a01003 	mov	r1, r3
    c690:	e1a00007 	mov	r0, r7
    c694:	e58d300c 	str	r3, [sp, #12]
    c698:	eb00082e 	bl	e758 <__mcmp>
    c69c:	e59d300c 	ldr	r3, [sp, #12]
    c6a0:	e1a02000 	mov	r2, r0
    c6a4:	eaffffb4 	b	c57c <_dtoa_r+0x1184>
    c6a8:	e1a01005 	mov	r1, r5
    c6ac:	e1a00006 	mov	r0, r6
    c6b0:	e3a0200a 	mov	r2, #10
    c6b4:	e3a03000 	mov	r3, #0
    c6b8:	eb00067a 	bl	e0a8 <__multadd>
    c6bc:	e1a05000 	mov	r5, r0
    c6c0:	e1a08000 	mov	r8, r0
    c6c4:	eaffffda 	b	c634 <_dtoa_r+0x123c>
    c6c8:	e3a03001 	mov	r3, #1
    c6cc:	e58d3044 	str	r3, [sp, #68]	; 0x44
    c6d0:	e58d302c 	str	r3, [sp, #44]	; 0x2c
    c6d4:	e58d30a4 	str	r3, [sp, #164]	; 0xa4
    c6d8:	e3a01000 	mov	r1, #0
    c6dc:	e5861044 	str	r1, [r6, #68]	; 0x44
    c6e0:	e0034004 	and	r4, r3, r4
    c6e4:	eafffda8 	b	bd8c <_dtoa_r+0x994>
    c6e8:	e358000e 	cmp	r8, #14
    c6ec:	83a03000 	movhi	r3, #0
    c6f0:	93a03001 	movls	r3, #1
    c6f4:	e58d802c 	str	r8, [sp, #44]	; 0x2c
    c6f8:	eafffff6 	b	c6d8 <_dtoa_r+0x12e0>
    c6fc:	e28d1038 	add	r1, sp, #56	; 0x38
    c700:	e1a00006 	mov	r0, r6
    c704:	e8910006 	ldm	r1, {r1, r2}
    c708:	eb000794 	bl	e560 <__pow5mult>
    c70c:	e58d0038 	str	r0, [sp, #56]	; 0x38
    c710:	eafffce7 	b	bab4 <_dtoa_r+0x6bc>
    c714:	e1a0c006 	mov	ip, r6
    c718:	e59d6020 	ldr	r6, [sp, #32]
    c71c:	e1a0900b 	mov	r9, fp
    c720:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    c724:	e58dc024 	str	ip, [sp, #36]	; 0x24
    c728:	eafffd3c 	b	bc20 <_dtoa_r+0x828>
    c72c:	e28d4010 	add	r4, sp, #16
    c730:	e8940018 	ldm	r4, {r3, r4}
    c734:	e3a08002 	mov	r8, #2
    c738:	e58d3030 	str	r3, [sp, #48]	; 0x30
    c73c:	e58d4034 	str	r4, [sp, #52]	; 0x34
    c740:	eafffdc0 	b	be48 <_dtoa_r+0xa50>
    c744:	e59dc02c 	ldr	ip, [sp, #44]	; 0x2c
    c748:	e35c0000 	cmp	ip, #0
    c74c:	0afffe5e 	beq	c0cc <_dtoa_r+0xcd4>
    c750:	e59d4044 	ldr	r4, [sp, #68]	; 0x44
    c754:	e3540000 	cmp	r4, #0
    c758:	dafffbfc 	ble	b750 <_dtoa_r+0x358>
    c75c:	e3a02000 	mov	r2, #0
    c760:	e51f3da8 	ldr	r3, [pc, #-3496]	; b9c0 <_dtoa_r+0x5c8>
    c764:	e28d1030 	add	r1, sp, #48	; 0x30
    c768:	e8910003 	ldm	r1, {r0, r1}
    c76c:	eb001542 	bl	11c7c <__aeabi_dmul>
    c770:	e58d0030 	str	r0, [sp, #48]	; 0x30
    c774:	e58d1034 	str	r1, [sp, #52]	; 0x34
    c778:	e2880001 	add	r0, r8, #1
    c77c:	eb001507 	bl	11ba0 <__aeabi_i2d>
    c780:	e1a02000 	mov	r2, r0
    c784:	e1a03001 	mov	r3, r1
    c788:	e28d1030 	add	r1, sp, #48	; 0x30
    c78c:	e8910003 	ldm	r1, {r0, r1}
    c790:	eb001539 	bl	11c7c <__aeabi_dmul>
    c794:	e3a02000 	mov	r2, #0
    c798:	e51f3ddc 	ldr	r3, [pc, #-3548]	; b9c4 <_dtoa_r+0x5cc>
    c79c:	eb001432 	bl	1186c <__adddf3>
    c7a0:	e249c001 	sub	ip, r9, #1
    c7a4:	e1a04000 	mov	r4, r0
    c7a8:	e58dc064 	str	ip, [sp, #100]	; 0x64
    c7ac:	e241550d 	sub	r5, r1, #54525952	; 0x3400000
    c7b0:	e59da044 	ldr	sl, [sp, #68]	; 0x44
    c7b4:	eafffdbc 	b	beac <_dtoa_r+0xab4>
    c7b8:	e28d3058 	add	r3, sp, #88	; 0x58
    c7bc:	e893000c 	ldm	r3, {r2, r3}
    c7c0:	e3a00000 	mov	r0, #0
    c7c4:	e51f1e04 	ldr	r1, [pc, #-3588]	; b9c8 <_dtoa_r+0x5d0>
    c7c8:	e58dc00c 	str	ip, [sp, #12]
    c7cc:	eb001425 	bl	11868 <__aeabi_dsub>
    c7d0:	e1a02004 	mov	r2, r4
    c7d4:	e1a03005 	mov	r3, r5
    c7d8:	eb001699 	bl	12244 <__aeabi_dcmpgt>
    c7dc:	e3500000 	cmp	r0, #0
    c7e0:	e59dc00c 	ldr	ip, [sp, #12]
    c7e4:	0afffbd9 	beq	b750 <_dtoa_r+0x358>
    c7e8:	e1a0900c 	mov	r9, ip
    c7ec:	e5592001 	ldrb	r2, [r9, #-1]
    c7f0:	e3520030 	cmp	r2, #48	; 0x30
    c7f4:	e1a03009 	mov	r3, r9
    c7f8:	e2499001 	sub	r9, r9, #1
    c7fc:	0afffffa 	beq	c7ec <_dtoa_r+0x13f4>
    c800:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    c804:	e59d9064 	ldr	r9, [sp, #100]	; 0x64
    c808:	e58d3024 	str	r3, [sp, #36]	; 0x24
    c80c:	eafffd03 	b	bc20 <_dtoa_r+0x828>
    c810:	e1a00006 	mov	r0, r6
    c814:	e59d1038 	ldr	r1, [sp, #56]	; 0x38
    c818:	eb000750 	bl	e560 <__pow5mult>
    c81c:	e58d0038 	str	r0, [sp, #56]	; 0x38
    c820:	eafffca3 	b	bab4 <_dtoa_r+0x6bc>
    c824:	e59d4060 	ldr	r4, [sp, #96]	; 0x60
    c828:	e3540000 	cmp	r4, #0
    c82c:	059d3070 	ldreq	r3, [sp, #112]	; 0x70
    c830:	12833e43 	addne	r3, r3, #1072	; 0x430
    c834:	12833003 	addne	r3, r3, #3
    c838:	159d503c 	ldrne	r5, [sp, #60]	; 0x3c
    c83c:	059d503c 	ldreq	r5, [sp, #60]	; 0x3c
    c840:	02633036 	rsbeq	r3, r3, #54	; 0x36
    c844:	e59d4028 	ldr	r4, [sp, #40]	; 0x28
    c848:	eafffd2e 	b	bd08 <_dtoa_r+0x910>
    c84c:	e3520000 	cmp	r2, #0
    c850:	e58d7038 	str	r7, [sp, #56]	; 0x38
    c854:	e1a0400b 	mov	r4, fp
    c858:	e1a07009 	mov	r7, r9
    c85c:	e59d903c 	ldr	r9, [sp, #60]	; 0x3c
    c860:	da00000c 	ble	c898 <_dtoa_r+0x14a0>
    c864:	e3a02001 	mov	r2, #1
    c868:	e59d1038 	ldr	r1, [sp, #56]	; 0x38
    c86c:	e1a00006 	mov	r0, r6
    c870:	eb000777 	bl	e654 <__lshift>
    c874:	e1a01007 	mov	r1, r7
    c878:	e58d0038 	str	r0, [sp, #56]	; 0x38
    c87c:	eb0007b5 	bl	e758 <__mcmp>
    c880:	e3500000 	cmp	r0, #0
    c884:	da00005b 	ble	c9f8 <_dtoa_r+0x1600>
    c888:	e59d3010 	ldr	r3, [sp, #16]
    c88c:	e3540039 	cmp	r4, #57	; 0x39
    c890:	e283b031 	add	fp, r3, #49	; 0x31
    c894:	0a000049 	beq	c9c0 <_dtoa_r+0x15c8>
    c898:	e59d3028 	ldr	r3, [sp, #40]	; 0x28
    c89c:	e4c3b001 	strb	fp, [r3], #1
    c8a0:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    c8a4:	e1a0a008 	mov	sl, r8
    c8a8:	e58d3024 	str	r3, [sp, #36]	; 0x24
    c8ac:	eafffcd0 	b	bbf4 <_dtoa_r+0x7fc>
    c8b0:	e59d6058 	ldr	r6, [sp, #88]	; 0x58
    c8b4:	e1a0c00b 	mov	ip, fp
    c8b8:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    c8bc:	eafffc29 	b	b968 <_dtoa_r+0x570>
    c8c0:	e2882001 	add	r2, r8, #1
    c8c4:	e58d9024 	str	r9, [sp, #36]	; 0x24
    c8c8:	e20220ff 	and	r2, r2, #255	; 0xff
    c8cc:	e59d9064 	ldr	r9, [sp, #100]	; 0x64
    c8d0:	eafffc4b 	b	ba04 <_dtoa_r+0x60c>
    c8d4:	e28d9068 	add	r9, sp, #104	; 0x68
    c8d8:	e8990a00 	ldm	r9, {r9, fp}
    c8dc:	e59d6058 	ldr	r6, [sp, #88]	; 0x58
    c8e0:	eafffb9a 	b	b750 <_dtoa_r+0x358>
    c8e4:	1a000001 	bne	c8f0 <_dtoa_r+0x14f8>
    c8e8:	e31b0001 	tst	fp, #1
    c8ec:	1afffe42 	bne	c1fc <_dtoa_r+0xe04>
    c8f0:	e1a0b00c 	mov	fp, ip
    c8f4:	e55b2001 	ldrb	r2, [fp, #-1]
    c8f8:	e3520030 	cmp	r2, #48	; 0x30
    c8fc:	e1a0300b 	mov	r3, fp
    c900:	e24bb001 	sub	fp, fp, #1
    c904:	0afffffa 	beq	c8f4 <_dtoa_r+0x14fc>
    c908:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    c90c:	e58d3024 	str	r3, [sp, #36]	; 0x24
    c910:	eafffcb7 	b	bbf4 <_dtoa_r+0x7fc>
    c914:	e3a07000 	mov	r7, #0
    c918:	e1a0a007 	mov	sl, r7
    c91c:	eafffe08 	b	c144 <_dtoa_r+0xd4c>
    c920:	e59a1004 	ldr	r1, [sl, #4]
    c924:	e1a00006 	mov	r0, r6
    c928:	eb0005b4 	bl	e000 <_Balloc>
    c92c:	e59a2010 	ldr	r2, [sl, #16]
    c930:	e2822002 	add	r2, r2, #2
    c934:	e1a04000 	mov	r4, r0
    c938:	e1a02102 	lsl	r2, r2, #2
    c93c:	e28a100c 	add	r1, sl, #12
    c940:	e280000c 	add	r0, r0, #12
    c944:	eb00056e 	bl	df04 <memcpy>
    c948:	e1a00006 	mov	r0, r6
    c94c:	e1a01004 	mov	r1, r4
    c950:	e3a02001 	mov	r2, #1
    c954:	eb00073e 	bl	e654 <__lshift>
    c958:	e1a08000 	mov	r8, r0
    c95c:	eafffef7 	b	c540 <_dtoa_r+0x1148>
    c960:	e2833001 	add	r3, r3, #1
    c964:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    c968:	e5c23000 	strb	r3, [r2]
    c96c:	e58db024 	str	fp, [sp, #36]	; 0x24
    c970:	eafffc9f 	b	bbf4 <_dtoa_r+0x7fc>
    c974:	e59d5010 	ldr	r5, [sp, #16]
    c978:	eafffc5d 	b	baf4 <_dtoa_r+0x6fc>
    c97c:	e35b0039 	cmp	fp, #57	; 0x39
    c980:	e58d7038 	str	r7, [sp, #56]	; 0x38
    c984:	e1a07009 	mov	r7, r9
    c988:	e59d903c 	ldr	r9, [sp, #60]	; 0x3c
    c98c:	0a00000b 	beq	c9c0 <_dtoa_r+0x15c8>
    c990:	e59d2028 	ldr	r2, [sp, #40]	; 0x28
    c994:	e28b3001 	add	r3, fp, #1
    c998:	e4c23001 	strb	r3, [r2], #1
    c99c:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    c9a0:	e1a0a008 	mov	sl, r8
    c9a4:	e58d2024 	str	r2, [sp, #36]	; 0x24
    c9a8:	eafffc91 	b	bbf4 <_dtoa_r+0x7fc>
    c9ac:	e58d7038 	str	r7, [sp, #56]	; 0x38
    c9b0:	e1a0a008 	mov	sl, r8
    c9b4:	e1a07009 	mov	r7, r9
    c9b8:	e59d903c 	ldr	r9, [sp, #60]	; 0x3c
    c9bc:	eafffe03 	b	c1d0 <_dtoa_r+0xdd8>
    c9c0:	e59dc028 	ldr	ip, [sp, #40]	; 0x28
    c9c4:	e3a03039 	mov	r3, #57	; 0x39
    c9c8:	e4cc3001 	strb	r3, [ip], #1
    c9cc:	e1a0a008 	mov	sl, r8
    c9d0:	eafffe0d 	b	c20c <_dtoa_r+0xe14>
    c9d4:	e35b0039 	cmp	fp, #57	; 0x39
    c9d8:	e58d7038 	str	r7, [sp, #56]	; 0x38
    c9dc:	e1a07009 	mov	r7, r9
    c9e0:	e59d903c 	ldr	r9, [sp, #60]	; 0x3c
    c9e4:	0afffff5 	beq	c9c0 <_dtoa_r+0x15c8>
    c9e8:	e35a0000 	cmp	sl, #0
    c9ec:	c59d3010 	ldrgt	r3, [sp, #16]
    c9f0:	c283b031 	addgt	fp, r3, #49	; 0x31
    c9f4:	eaffffa7 	b	c898 <_dtoa_r+0x14a0>
    c9f8:	1affffa6 	bne	c898 <_dtoa_r+0x14a0>
    c9fc:	e31b0001 	tst	fp, #1
    ca00:	0affffa4 	beq	c898 <_dtoa_r+0x14a0>
    ca04:	eaffff9f 	b	c888 <_dtoa_r+0x1490>
    ca08:	e3a08002 	mov	r8, #2
    ca0c:	eafffd0d 	b	be48 <_dtoa_r+0xa50>
    ca10:	e59dc030 	ldr	ip, [sp, #48]	; 0x30
    ca14:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    ca18:	e58dc024 	str	ip, [sp, #36]	; 0x24
    ca1c:	eafffc7f 	b	bc20 <_dtoa_r+0x828>
    ca20:	e3a03000 	mov	r3, #0
    ca24:	e1a0100a 	mov	r1, sl
    ca28:	e1a00006 	mov	r0, r6
    ca2c:	e3a0200a 	mov	r2, #10
    ca30:	eb00059c 	bl	e0a8 <__multadd>
    ca34:	e59dc044 	ldr	ip, [sp, #68]	; 0x44
    ca38:	e59d10a0 	ldr	r1, [sp, #160]	; 0xa0
    ca3c:	e35c0000 	cmp	ip, #0
    ca40:	c3a03000 	movgt	r3, #0
    ca44:	d3a03001 	movle	r3, #1
    ca48:	e3510002 	cmp	r1, #2
    ca4c:	d3a03000 	movle	r3, #0
    ca50:	e3530000 	cmp	r3, #0
    ca54:	e1a0a000 	mov	sl, r0
    ca58:	058dc02c 	streq	ip, [sp, #44]	; 0x2c
    ca5c:	0afffead 	beq	c518 <_dtoa_r+0x1120>
    ca60:	e59d4044 	ldr	r4, [sp, #68]	; 0x44
    ca64:	e58d402c 	str	r4, [sp, #44]	; 0x2c
    ca68:	eafffc4c 	b	bba0 <_dtoa_r+0x7a8>
    ca6c:	e59dc030 	ldr	ip, [sp, #48]	; 0x30
    ca70:	e59d4024 	ldr	r4, [sp, #36]	; 0x24
    ca74:	e59d9064 	ldr	r9, [sp, #100]	; 0x64
    ca78:	e58dc024 	str	ip, [sp, #36]	; 0x24
    ca7c:	eafffc67 	b	bc20 <_dtoa_r+0x828>
    ca80:	1263303c 	rsbne	r3, r3, #60	; 0x3c
    ca84:	0afffc2a 	beq	bb34 <_dtoa_r+0x73c>
    ca88:	eafffc24 	b	bb20 <_dtoa_r+0x728>
    ca8c:	e1a00000 	nop			; (mov r0, r0)

0000ca90 <__errno>:
    ca90:	e59f3004 	ldr	r3, [pc, #4]	; ca9c <__errno+0xc>
    ca94:	e5930000 	ldr	r0, [r3]
    ca98:	e12fff1e 	bx	lr
    ca9c:	0001a600 	.word	0x0001a600

0000caa0 <__sflush_r>:
    caa0:	e1d120bc 	ldrh	r2, [r1, #12]
    caa4:	e3120008 	tst	r2, #8
    caa8:	e92d40f8 	push	{r3, r4, r5, r6, r7, lr}
    caac:	e1a05001 	mov	r5, r1
    cab0:	e1a07000 	mov	r7, r0
    cab4:	1a00002f 	bne	cb78 <__sflush_r+0xd8>
    cab8:	e5911004 	ldr	r1, [r1, #4]
    cabc:	e3823b02 	orr	r3, r2, #2048	; 0x800
    cac0:	e3510000 	cmp	r1, #0
    cac4:	e1c530bc 	strh	r3, [r5, #12]
    cac8:	da00004a 	ble	cbf8 <__sflush_r+0x158>
    cacc:	e595c028 	ldr	ip, [r5, #40]	; 0x28
    cad0:	e35c0000 	cmp	ip, #0
    cad4:	0a00004a 	beq	cc04 <__sflush_r+0x164>
    cad8:	e3a01000 	mov	r1, #0
    cadc:	e2126a01 	ands	r6, r2, #4096	; 0x1000
    cae0:	e5974000 	ldr	r4, [r7]
    cae4:	e5871000 	str	r1, [r7]
    cae8:	0a000047 	beq	cc0c <__sflush_r+0x16c>
    caec:	e5952050 	ldr	r2, [r5, #80]	; 0x50
    caf0:	e3130004 	tst	r3, #4
    caf4:	0a000005 	beq	cb10 <__sflush_r+0x70>
    caf8:	e5951030 	ldr	r1, [r5, #48]	; 0x30
    cafc:	e5953004 	ldr	r3, [r5, #4]
    cb00:	e3510000 	cmp	r1, #0
    cb04:	e0632002 	rsb	r2, r3, r2
    cb08:	1595303c 	ldrne	r3, [r5, #60]	; 0x3c
    cb0c:	10632002 	rsbne	r2, r3, r2
    cb10:	e1a00007 	mov	r0, r7
    cb14:	e595101c 	ldr	r1, [r5, #28]
    cb18:	e3a03000 	mov	r3, #0
    cb1c:	e1a0e00f 	mov	lr, pc
    cb20:	e12fff1c 	bx	ip
    cb24:	e3700001 	cmn	r0, #1
    cb28:	0a000043 	beq	cc3c <__sflush_r+0x19c>
    cb2c:	e1d530bc 	ldrh	r3, [r5, #12]
    cb30:	e5952010 	ldr	r2, [r5, #16]
    cb34:	e3c31b02 	bic	r1, r3, #2048	; 0x800
    cb38:	e3130a01 	tst	r3, #4096	; 0x1000
    cb3c:	e3a03000 	mov	r3, #0
    cb40:	e1c510bc 	strh	r1, [r5, #12]
    cb44:	e885000c 	stm	r5, {r2, r3}
    cb48:	1a000045 	bne	cc64 <__sflush_r+0x1c4>
    cb4c:	e5951030 	ldr	r1, [r5, #48]	; 0x30
    cb50:	e3510000 	cmp	r1, #0
    cb54:	e5874000 	str	r4, [r7]
    cb58:	0a000029 	beq	cc04 <__sflush_r+0x164>
    cb5c:	e2853040 	add	r3, r5, #64	; 0x40
    cb60:	e1510003 	cmp	r1, r3
    cb64:	11a00007 	movne	r0, r7
    cb68:	1b00016d 	blne	d124 <_free_r>
    cb6c:	e3a00000 	mov	r0, #0
    cb70:	e5850030 	str	r0, [r5, #48]	; 0x30
    cb74:	ea00001d 	b	cbf0 <__sflush_r+0x150>
    cb78:	e5916010 	ldr	r6, [r1, #16]
    cb7c:	e3560000 	cmp	r6, #0
    cb80:	0a00001f 	beq	cc04 <__sflush_r+0x164>
    cb84:	e5914000 	ldr	r4, [r1]
    cb88:	e3120003 	tst	r2, #3
    cb8c:	05913014 	ldreq	r3, [r1, #20]
    cb90:	e0664004 	rsb	r4, r6, r4
    cb94:	13a03000 	movne	r3, #0
    cb98:	e3540000 	cmp	r4, #0
    cb9c:	e5816000 	str	r6, [r1]
    cba0:	e5813008 	str	r3, [r1, #8]
    cba4:	ca000003 	bgt	cbb8 <__sflush_r+0x118>
    cba8:	ea000015 	b	cc04 <__sflush_r+0x164>
    cbac:	e3540000 	cmp	r4, #0
    cbb0:	e0866000 	add	r6, r6, r0
    cbb4:	da000012 	ble	cc04 <__sflush_r+0x164>
    cbb8:	e1a03004 	mov	r3, r4
    cbbc:	e1a02006 	mov	r2, r6
    cbc0:	e1a00007 	mov	r0, r7
    cbc4:	e595101c 	ldr	r1, [r5, #28]
    cbc8:	e595c024 	ldr	ip, [r5, #36]	; 0x24
    cbcc:	e1a0e00f 	mov	lr, pc
    cbd0:	e12fff1c 	bx	ip
    cbd4:	e3500000 	cmp	r0, #0
    cbd8:	e0604004 	rsb	r4, r0, r4
    cbdc:	cafffff2 	bgt	cbac <__sflush_r+0x10c>
    cbe0:	e1d530bc 	ldrh	r3, [r5, #12]
    cbe4:	e3833040 	orr	r3, r3, #64	; 0x40
    cbe8:	e1c530bc 	strh	r3, [r5, #12]
    cbec:	e3e00000 	mvn	r0, #0
    cbf0:	e8bd40f8 	pop	{r3, r4, r5, r6, r7, lr}
    cbf4:	e12fff1e 	bx	lr
    cbf8:	e595103c 	ldr	r1, [r5, #60]	; 0x3c
    cbfc:	e3510000 	cmp	r1, #0
    cc00:	caffffb1 	bgt	cacc <__sflush_r+0x2c>
    cc04:	e3a00000 	mov	r0, #0
    cc08:	eafffff8 	b	cbf0 <__sflush_r+0x150>
    cc0c:	e1a02006 	mov	r2, r6
    cc10:	e1a00007 	mov	r0, r7
    cc14:	e595101c 	ldr	r1, [r5, #28]
    cc18:	e3a03001 	mov	r3, #1
    cc1c:	e1a0e00f 	mov	lr, pc
    cc20:	e12fff1c 	bx	ip
    cc24:	e3700001 	cmn	r0, #1
    cc28:	e1a02000 	mov	r2, r0
    cc2c:	0a00001a 	beq	cc9c <__sflush_r+0x1fc>
    cc30:	e1d530bc 	ldrh	r3, [r5, #12]
    cc34:	e595c028 	ldr	ip, [r5, #40]	; 0x28
    cc38:	eaffffac 	b	caf0 <__sflush_r+0x50>
    cc3c:	e5973000 	ldr	r3, [r7]
    cc40:	e3530000 	cmp	r3, #0
    cc44:	1a000008 	bne	cc6c <__sflush_r+0x1cc>
    cc48:	e1d520bc 	ldrh	r2, [r5, #12]
    cc4c:	e5951010 	ldr	r1, [r5, #16]
    cc50:	e3c2cb02 	bic	ip, r2, #2048	; 0x800
    cc54:	e3120a01 	tst	r2, #4096	; 0x1000
    cc58:	e1c5c0bc 	strh	ip, [r5, #12]
    cc5c:	e885000a 	stm	r5, {r1, r3}
    cc60:	0affffb9 	beq	cb4c <__sflush_r+0xac>
    cc64:	e5850050 	str	r0, [r5, #80]	; 0x50
    cc68:	eaffffb7 	b	cb4c <__sflush_r+0xac>
    cc6c:	e353001d 	cmp	r3, #29
    cc70:	0a000001 	beq	cc7c <__sflush_r+0x1dc>
    cc74:	e3530016 	cmp	r3, #22
    cc78:	1a000010 	bne	ccc0 <__sflush_r+0x220>
    cc7c:	e1d520bc 	ldrh	r2, [r5, #12]
    cc80:	e5953010 	ldr	r3, [r5, #16]
    cc84:	e3c22b02 	bic	r2, r2, #2048	; 0x800
    cc88:	e1c520bc 	strh	r2, [r5, #12]
    cc8c:	e3a02000 	mov	r2, #0
    cc90:	e5852004 	str	r2, [r5, #4]
    cc94:	e5853000 	str	r3, [r5]
    cc98:	eaffffab 	b	cb4c <__sflush_r+0xac>
    cc9c:	e5973000 	ldr	r3, [r7]
    cca0:	e3530000 	cmp	r3, #0
    cca4:	0affffe1 	beq	cc30 <__sflush_r+0x190>
    cca8:	e353001d 	cmp	r3, #29
    ccac:	13530016 	cmpne	r3, #22
    ccb0:	05874000 	streq	r4, [r7]
    ccb4:	01a00006 	moveq	r0, r6
    ccb8:	0affffcc 	beq	cbf0 <__sflush_r+0x150>
    ccbc:	eaffffc7 	b	cbe0 <__sflush_r+0x140>
    ccc0:	e1d530bc 	ldrh	r3, [r5, #12]
    ccc4:	e3833040 	orr	r3, r3, #64	; 0x40
    ccc8:	e1c530bc 	strh	r3, [r5, #12]
    cccc:	eaffffc7 	b	cbf0 <__sflush_r+0x150>

0000ccd0 <_fflush_r>:
    ccd0:	e92d4010 	push	{r4, lr}
    ccd4:	e2504000 	subs	r4, r0, #0
    ccd8:	e24dd008 	sub	sp, sp, #8
    ccdc:	0a000002 	beq	ccec <_fflush_r+0x1c>
    cce0:	e5943038 	ldr	r3, [r4, #56]	; 0x38
    cce4:	e3530000 	cmp	r3, #0
    cce8:	0a000009 	beq	cd14 <_fflush_r+0x44>
    ccec:	e1d100fc 	ldrsh	r0, [r1, #12]
    ccf0:	e3500000 	cmp	r0, #0
    ccf4:	1a000002 	bne	cd04 <_fflush_r+0x34>
    ccf8:	e28dd008 	add	sp, sp, #8
    ccfc:	e8bd4010 	pop	{r4, lr}
    cd00:	e12fff1e 	bx	lr
    cd04:	e1a00004 	mov	r0, r4
    cd08:	e28dd008 	add	sp, sp, #8
    cd0c:	e8bd4010 	pop	{r4, lr}
    cd10:	eaffff62 	b	caa0 <__sflush_r>
    cd14:	e58d1004 	str	r1, [sp, #4]
    cd18:	eb000050 	bl	ce60 <__sinit>
    cd1c:	e59d1004 	ldr	r1, [sp, #4]
    cd20:	eafffff1 	b	ccec <_fflush_r+0x1c>

0000cd24 <fflush>:
    cd24:	e2501000 	subs	r1, r0, #0
    cd28:	e92d4008 	push	{r3, lr}
    cd2c:	0a000003 	beq	cd40 <fflush+0x1c>
    cd30:	e59f3020 	ldr	r3, [pc, #32]	; cd58 <fflush+0x34>
    cd34:	e5930000 	ldr	r0, [r3]
    cd38:	e8bd4008 	pop	{r3, lr}
    cd3c:	eaffffe3 	b	ccd0 <_fflush_r>
    cd40:	e59f3014 	ldr	r3, [pc, #20]	; cd5c <fflush+0x38>
    cd44:	e59f1014 	ldr	r1, [pc, #20]	; cd60 <fflush+0x3c>
    cd48:	e5930000 	ldr	r0, [r3]
    cd4c:	eb0001b0 	bl	d414 <_fwalk_reent>
    cd50:	e8bd4008 	pop	{r3, lr}
    cd54:	e12fff1e 	bx	lr
    cd58:	0001a600 	.word	0x0001a600
    cd5c:	000122f8 	.word	0x000122f8
    cd60:	0000ccd0 	.word	0x0000ccd0

0000cd64 <__fp_lock>:
    cd64:	e3a00000 	mov	r0, #0
    cd68:	e12fff1e 	bx	lr

0000cd6c <__fp_unlock>:
    cd6c:	e3a00000 	mov	r0, #0
    cd70:	e12fff1e 	bx	lr

0000cd74 <_cleanup_r>:
    cd74:	e92d4008 	push	{r3, lr}
    cd78:	e59f1008 	ldr	r1, [pc, #8]	; cd88 <_cleanup_r+0x14>
    cd7c:	eb000183 	bl	d390 <_fwalk>
    cd80:	e8bd4008 	pop	{r3, lr}
    cd84:	e12fff1e 	bx	lr
    cd88:	000104ac 	.word	0x000104ac

0000cd8c <std.isra.0>:
    cd8c:	e3a03000 	mov	r3, #0
    cd90:	e92d4010 	push	{r4, lr}
    cd94:	e1a04000 	mov	r4, r0
    cd98:	e1c010bc 	strh	r1, [r0, #12]
    cd9c:	e1c020be 	strh	r2, [r0, #14]
    cda0:	e5803000 	str	r3, [r0]
    cda4:	e5803004 	str	r3, [r0, #4]
    cda8:	e5803008 	str	r3, [r0, #8]
    cdac:	e5803064 	str	r3, [r0, #100]	; 0x64
    cdb0:	e5803010 	str	r3, [r0, #16]
    cdb4:	e5803014 	str	r3, [r0, #20]
    cdb8:	e5803018 	str	r3, [r0, #24]
    cdbc:	e1a01003 	mov	r1, r3
    cdc0:	e280005c 	add	r0, r0, #92	; 0x5c
    cdc4:	e3a02008 	mov	r2, #8
    cdc8:	ebffed9e 	bl	8448 <memset>
    cdcc:	e59f3024 	ldr	r3, [pc, #36]	; cdf8 <std.isra.0+0x6c>
    cdd0:	e5843020 	str	r3, [r4, #32]
    cdd4:	e59f3020 	ldr	r3, [pc, #32]	; cdfc <std.isra.0+0x70>
    cdd8:	e5843024 	str	r3, [r4, #36]	; 0x24
    cddc:	e59f301c 	ldr	r3, [pc, #28]	; ce00 <std.isra.0+0x74>
    cde0:	e5843028 	str	r3, [r4, #40]	; 0x28
    cde4:	e59f3018 	ldr	r3, [pc, #24]	; ce04 <std.isra.0+0x78>
    cde8:	e584401c 	str	r4, [r4, #28]
    cdec:	e584302c 	str	r3, [r4, #44]	; 0x2c
    cdf0:	e8bd4010 	pop	{r4, lr}
    cdf4:	e12fff1e 	bx	lr
    cdf8:	0000edcc 	.word	0x0000edcc
    cdfc:	0000ee08 	.word	0x0000ee08
    ce00:	0000ee60 	.word	0x0000ee60
    ce04:	0000ee94 	.word	0x0000ee94

0000ce08 <__sfmoreglue>:
    ce08:	e92d4070 	push	{r4, r5, r6, lr}
    ce0c:	e0816081 	add	r6, r1, r1, lsl #1
    ce10:	e0816106 	add	r6, r1, r6, lsl #2
    ce14:	e1a06186 	lsl	r6, r6, #3
    ce18:	e1a05001 	mov	r5, r1
    ce1c:	e286100c 	add	r1, r6, #12
    ce20:	eb000229 	bl	d6cc <_malloc_r>
    ce24:	e2504000 	subs	r4, r0, #0
    ce28:	0a000005 	beq	ce44 <__sfmoreglue+0x3c>
    ce2c:	e284000c 	add	r0, r4, #12
    ce30:	e3a01000 	mov	r1, #0
    ce34:	e8840022 	stm	r4, {r1, r5}
    ce38:	e5840008 	str	r0, [r4, #8]
    ce3c:	e1a02006 	mov	r2, r6
    ce40:	ebffed80 	bl	8448 <memset>
    ce44:	e1a00004 	mov	r0, r4
    ce48:	e8bd4070 	pop	{r4, r5, r6, lr}
    ce4c:	e12fff1e 	bx	lr

0000ce50 <_cleanup>:
    ce50:	e59f3004 	ldr	r3, [pc, #4]	; ce5c <_cleanup+0xc>
    ce54:	e5930000 	ldr	r0, [r3]
    ce58:	eaffffc5 	b	cd74 <_cleanup_r>
    ce5c:	000122f8 	.word	0x000122f8

0000ce60 <__sinit>:
    ce60:	e92d4038 	push	{r3, r4, r5, lr}
    ce64:	e5903038 	ldr	r3, [r0, #56]	; 0x38
    ce68:	e3530000 	cmp	r3, #0
    ce6c:	e1a04000 	mov	r4, r0
    ce70:	0a000001 	beq	ce7c <__sinit+0x1c>
    ce74:	e8bd4038 	pop	{r3, r4, r5, lr}
    ce78:	e12fff1e 	bx	lr
    ce7c:	e59fe050 	ldr	lr, [pc, #80]	; ced4 <__sinit+0x74>
    ce80:	e280cfbb 	add	ip, r0, #748	; 0x2ec
    ce84:	e3a05001 	mov	r5, #1
    ce88:	e1a02003 	mov	r2, r3
    ce8c:	e58432e0 	str	r3, [r4, #736]	; 0x2e0
    ce90:	e3a03003 	mov	r3, #3
    ce94:	e584e03c 	str	lr, [r4, #60]	; 0x3c
    ce98:	e58432e4 	str	r3, [r4, #740]	; 0x2e4
    ce9c:	e584c2e8 	str	ip, [r4, #744]	; 0x2e8
    cea0:	e5900004 	ldr	r0, [r0, #4]
    cea4:	e3a01004 	mov	r1, #4
    cea8:	e5845038 	str	r5, [r4, #56]	; 0x38
    ceac:	ebffffb6 	bl	cd8c <std.isra.0>
    ceb0:	e5940008 	ldr	r0, [r4, #8]
    ceb4:	e1a02005 	mov	r2, r5
    ceb8:	e3a01009 	mov	r1, #9
    cebc:	ebffffb2 	bl	cd8c <std.isra.0>
    cec0:	e594000c 	ldr	r0, [r4, #12]
    cec4:	e3a01012 	mov	r1, #18
    cec8:	e3a02002 	mov	r2, #2
    cecc:	e8bd4038 	pop	{r3, r4, r5, lr}
    ced0:	eaffffad 	b	cd8c <std.isra.0>
    ced4:	0000cd74 	.word	0x0000cd74

0000ced8 <__sfp>:
    ced8:	e59f30ec 	ldr	r3, [pc, #236]	; cfcc <__sfp+0xf4>
    cedc:	e92d4070 	push	{r4, r5, r6, lr}
    cee0:	e5935000 	ldr	r5, [r3]
    cee4:	e5953038 	ldr	r3, [r5, #56]	; 0x38
    cee8:	e3530000 	cmp	r3, #0
    ceec:	e1a06000 	mov	r6, r0
    cef0:	0a000028 	beq	cf98 <__sfp+0xc0>
    cef4:	e2855e2e 	add	r5, r5, #736	; 0x2e0
    cef8:	e5953004 	ldr	r3, [r5, #4]
    cefc:	e2533001 	subs	r3, r3, #1
    cf00:	e5954008 	ldr	r4, [r5, #8]
    cf04:	5a000004 	bpl	cf1c <__sfp+0x44>
    cf08:	ea00001d 	b	cf84 <__sfp+0xac>
    cf0c:	e2433001 	sub	r3, r3, #1
    cf10:	e3730001 	cmn	r3, #1
    cf14:	e2844068 	add	r4, r4, #104	; 0x68
    cf18:	0a000019 	beq	cf84 <__sfp+0xac>
    cf1c:	e1d420fc 	ldrsh	r2, [r4, #12]
    cf20:	e3520000 	cmp	r2, #0
    cf24:	1afffff8 	bne	cf0c <__sfp+0x34>
    cf28:	e3e03000 	mvn	r3, #0
    cf2c:	e3a05000 	mov	r5, #0
    cf30:	e1c430be 	strh	r3, [r4, #14]
    cf34:	e3a03001 	mov	r3, #1
    cf38:	e1c430bc 	strh	r3, [r4, #12]
    cf3c:	e5845064 	str	r5, [r4, #100]	; 0x64
    cf40:	e5845000 	str	r5, [r4]
    cf44:	e5845008 	str	r5, [r4, #8]
    cf48:	e5845004 	str	r5, [r4, #4]
    cf4c:	e5845010 	str	r5, [r4, #16]
    cf50:	e5845014 	str	r5, [r4, #20]
    cf54:	e5845018 	str	r5, [r4, #24]
    cf58:	e284005c 	add	r0, r4, #92	; 0x5c
    cf5c:	e1a01005 	mov	r1, r5
    cf60:	e3a02008 	mov	r2, #8
    cf64:	ebffed37 	bl	8448 <memset>
    cf68:	e5845030 	str	r5, [r4, #48]	; 0x30
    cf6c:	e5845034 	str	r5, [r4, #52]	; 0x34
    cf70:	e5845044 	str	r5, [r4, #68]	; 0x44
    cf74:	e5845048 	str	r5, [r4, #72]	; 0x48
    cf78:	e1a00004 	mov	r0, r4
    cf7c:	e8bd4070 	pop	{r4, r5, r6, lr}
    cf80:	e12fff1e 	bx	lr
    cf84:	e5950000 	ldr	r0, [r5]
    cf88:	e3500000 	cmp	r0, #0
    cf8c:	0a000004 	beq	cfa4 <__sfp+0xcc>
    cf90:	e1a05000 	mov	r5, r0
    cf94:	eaffffd7 	b	cef8 <__sfp+0x20>
    cf98:	e1a00005 	mov	r0, r5
    cf9c:	ebffffaf 	bl	ce60 <__sinit>
    cfa0:	eaffffd3 	b	cef4 <__sfp+0x1c>
    cfa4:	e1a00006 	mov	r0, r6
    cfa8:	e3a01004 	mov	r1, #4
    cfac:	ebffff95 	bl	ce08 <__sfmoreglue>
    cfb0:	e3500000 	cmp	r0, #0
    cfb4:	e5850000 	str	r0, [r5]
    cfb8:	1afffff4 	bne	cf90 <__sfp+0xb8>
    cfbc:	e3a0300c 	mov	r3, #12
    cfc0:	e5863000 	str	r3, [r6]
    cfc4:	e1a04000 	mov	r4, r0
    cfc8:	eaffffea 	b	cf78 <__sfp+0xa0>
    cfcc:	000122f8 	.word	0x000122f8

0000cfd0 <__sfp_lock_acquire>:
    cfd0:	e12fff1e 	bx	lr

0000cfd4 <__sfp_lock_release>:
    cfd4:	e12fff1e 	bx	lr

0000cfd8 <__sinit_lock_acquire>:
    cfd8:	e12fff1e 	bx	lr

0000cfdc <__sinit_lock_release>:
    cfdc:	e12fff1e 	bx	lr

0000cfe0 <__fp_lock_all>:
    cfe0:	e92d4008 	push	{r3, lr}
    cfe4:	e59f3010 	ldr	r3, [pc, #16]	; cffc <__fp_lock_all+0x1c>
    cfe8:	e59f1010 	ldr	r1, [pc, #16]	; d000 <__fp_lock_all+0x20>
    cfec:	e5930000 	ldr	r0, [r3]
    cff0:	eb0000e6 	bl	d390 <_fwalk>
    cff4:	e8bd4008 	pop	{r3, lr}
    cff8:	e12fff1e 	bx	lr
    cffc:	0001a600 	.word	0x0001a600
    d000:	0000cd64 	.word	0x0000cd64

0000d004 <__fp_unlock_all>:
    d004:	e92d4008 	push	{r3, lr}
    d008:	e59f3010 	ldr	r3, [pc, #16]	; d020 <__fp_unlock_all+0x1c>
    d00c:	e59f1010 	ldr	r1, [pc, #16]	; d024 <__fp_unlock_all+0x20>
    d010:	e5930000 	ldr	r0, [r3]
    d014:	eb0000dd 	bl	d390 <_fwalk>
    d018:	e8bd4008 	pop	{r3, lr}
    d01c:	e12fff1e 	bx	lr
    d020:	0001a600 	.word	0x0001a600
    d024:	0000cd6c 	.word	0x0000cd6c

0000d028 <_malloc_trim_r>:
    d028:	e92d40f8 	push	{r3, r4, r5, r6, r7, lr}
    d02c:	e59f50e4 	ldr	r5, [pc, #228]	; d118 <_malloc_trim_r+0xf0>
    d030:	e1a07001 	mov	r7, r1
    d034:	e1a04000 	mov	r4, r0
    d038:	eb0003ee 	bl	dff8 <__malloc_lock>
    d03c:	e5953008 	ldr	r3, [r5, #8]
    d040:	e5936004 	ldr	r6, [r3, #4]
    d044:	e3c66003 	bic	r6, r6, #3
    d048:	e2863efe 	add	r3, r6, #4064	; 0xfe0
    d04c:	e283300f 	add	r3, r3, #15
    d050:	e0677003 	rsb	r7, r7, r3
    d054:	e1a07627 	lsr	r7, r7, #12
    d058:	e2477001 	sub	r7, r7, #1
    d05c:	e1a07607 	lsl	r7, r7, #12
    d060:	e3570a01 	cmp	r7, #4096	; 0x1000
    d064:	ba000006 	blt	d084 <_malloc_trim_r+0x5c>
    d068:	e1a00004 	mov	r0, r4
    d06c:	e3a01000 	mov	r1, #0
    d070:	eb00072a 	bl	ed20 <_sbrk_r>
    d074:	e5953008 	ldr	r3, [r5, #8]
    d078:	e0833006 	add	r3, r3, r6
    d07c:	e1500003 	cmp	r0, r3
    d080:	0a000004 	beq	d098 <_malloc_trim_r+0x70>
    d084:	e1a00004 	mov	r0, r4
    d088:	eb0003db 	bl	dffc <__malloc_unlock>
    d08c:	e3a00000 	mov	r0, #0
    d090:	e8bd40f8 	pop	{r3, r4, r5, r6, r7, lr}
    d094:	e12fff1e 	bx	lr
    d098:	e1a00004 	mov	r0, r4
    d09c:	e2671000 	rsb	r1, r7, #0
    d0a0:	eb00071e 	bl	ed20 <_sbrk_r>
    d0a4:	e3700001 	cmn	r0, #1
    d0a8:	0a00000b 	beq	d0dc <_malloc_trim_r+0xb4>
    d0ac:	e59f3068 	ldr	r3, [pc, #104]	; d11c <_malloc_trim_r+0xf4>
    d0b0:	e5952008 	ldr	r2, [r5, #8]
    d0b4:	e5931000 	ldr	r1, [r3]
    d0b8:	e0676006 	rsb	r6, r7, r6
    d0bc:	e3866001 	orr	r6, r6, #1
    d0c0:	e0677001 	rsb	r7, r7, r1
    d0c4:	e1a00004 	mov	r0, r4
    d0c8:	e5826004 	str	r6, [r2, #4]
    d0cc:	e5837000 	str	r7, [r3]
    d0d0:	eb0003c9 	bl	dffc <__malloc_unlock>
    d0d4:	e3a00001 	mov	r0, #1
    d0d8:	eaffffec 	b	d090 <_malloc_trim_r+0x68>
    d0dc:	e1a00004 	mov	r0, r4
    d0e0:	e3a01000 	mov	r1, #0
    d0e4:	eb00070d 	bl	ed20 <_sbrk_r>
    d0e8:	e5953008 	ldr	r3, [r5, #8]
    d0ec:	e0632000 	rsb	r2, r3, r0
    d0f0:	e352000f 	cmp	r2, #15
    d0f4:	daffffe2 	ble	d084 <_malloc_trim_r+0x5c>
    d0f8:	e59f1020 	ldr	r1, [pc, #32]	; d120 <_malloc_trim_r+0xf8>
    d0fc:	e5911000 	ldr	r1, [r1]
    d100:	e0610000 	rsb	r0, r1, r0
    d104:	e59f1010 	ldr	r1, [pc, #16]	; d11c <_malloc_trim_r+0xf4>
    d108:	e3822001 	orr	r2, r2, #1
    d10c:	e5810000 	str	r0, [r1]
    d110:	e5832004 	str	r2, [r3, #4]
    d114:	eaffffda 	b	d084 <_malloc_trim_r+0x5c>
    d118:	0001aaac 	.word	0x0001aaac
    d11c:	0001af9c 	.word	0x0001af9c
    d120:	0001aeb4 	.word	0x0001aeb4

0000d124 <_free_r>:
    d124:	e92d40f8 	push	{r3, r4, r5, r6, r7, lr}
    d128:	e2514000 	subs	r4, r1, #0
    d12c:	e1a06000 	mov	r6, r0
    d130:	0a000046 	beq	d250 <_free_r+0x12c>
    d134:	eb0003af 	bl	dff8 <__malloc_lock>
    d138:	e514e004 	ldr	lr, [r4, #-4]
    d13c:	e59f1238 	ldr	r1, [pc, #568]	; d37c <_free_r+0x258>
    d140:	e244c008 	sub	ip, r4, #8
    d144:	e3ce3001 	bic	r3, lr, #1
    d148:	e08c2003 	add	r2, ip, r3
    d14c:	e5910008 	ldr	r0, [r1, #8]
    d150:	e5925004 	ldr	r5, [r2, #4]
    d154:	e1500002 	cmp	r0, r2
    d158:	e3c55003 	bic	r5, r5, #3
    d15c:	0a00004a 	beq	d28c <_free_r+0x168>
    d160:	e21ee001 	ands	lr, lr, #1
    d164:	e5825004 	str	r5, [r2, #4]
    d168:	13a0e000 	movne	lr, #0
    d16c:	1a000009 	bne	d198 <_free_r+0x74>
    d170:	e5144008 	ldr	r4, [r4, #-8]
    d174:	e064c00c 	rsb	ip, r4, ip
    d178:	e59c0008 	ldr	r0, [ip, #8]
    d17c:	e2817008 	add	r7, r1, #8
    d180:	e1500007 	cmp	r0, r7
    d184:	e0833004 	add	r3, r3, r4
    d188:	159c400c 	ldrne	r4, [ip, #12]
    d18c:	1580400c 	strne	r4, [r0, #12]
    d190:	15840008 	strne	r0, [r4, #8]
    d194:	03a0e001 	moveq	lr, #1
    d198:	e0820005 	add	r0, r2, r5
    d19c:	e5900004 	ldr	r0, [r0, #4]
    d1a0:	e3100001 	tst	r0, #1
    d1a4:	1a000009 	bne	d1d0 <_free_r+0xac>
    d1a8:	e35e0000 	cmp	lr, #0
    d1ac:	e5920008 	ldr	r0, [r2, #8]
    d1b0:	e0833005 	add	r3, r3, r5
    d1b4:	1a000002 	bne	d1c4 <_free_r+0xa0>
    d1b8:	e59f41c0 	ldr	r4, [pc, #448]	; d380 <_free_r+0x25c>
    d1bc:	e1500004 	cmp	r0, r4
    d1c0:	0a000047 	beq	d2e4 <_free_r+0x1c0>
    d1c4:	e592200c 	ldr	r2, [r2, #12]
    d1c8:	e580200c 	str	r2, [r0, #12]
    d1cc:	e5820008 	str	r0, [r2, #8]
    d1d0:	e3832001 	orr	r2, r3, #1
    d1d4:	e35e0000 	cmp	lr, #0
    d1d8:	e58c2004 	str	r2, [ip, #4]
    d1dc:	e78c3003 	str	r3, [ip, r3]
    d1e0:	1a000018 	bne	d248 <_free_r+0x124>
    d1e4:	e3530c02 	cmp	r3, #512	; 0x200
    d1e8:	3a00001a 	bcc	d258 <_free_r+0x134>
    d1ec:	e1a024a3 	lsr	r2, r3, #9
    d1f0:	e3520004 	cmp	r2, #4
    d1f4:	8a000042 	bhi	d304 <_free_r+0x1e0>
    d1f8:	e1a0e323 	lsr	lr, r3, #6
    d1fc:	e28ee038 	add	lr, lr, #56	; 0x38
    d200:	e1a0008e 	lsl	r0, lr, #1
    d204:	e0810100 	add	r0, r1, r0, lsl #2
    d208:	e5902008 	ldr	r2, [r0, #8]
    d20c:	e1520000 	cmp	r2, r0
    d210:	e59f1164 	ldr	r1, [pc, #356]	; d37c <_free_r+0x258>
    d214:	0a000044 	beq	d32c <_free_r+0x208>
    d218:	e5921004 	ldr	r1, [r2, #4]
    d21c:	e3c11003 	bic	r1, r1, #3
    d220:	e1530001 	cmp	r3, r1
    d224:	2a000002 	bcs	d234 <_free_r+0x110>
    d228:	e5922008 	ldr	r2, [r2, #8]
    d22c:	e1500002 	cmp	r0, r2
    d230:	1afffff8 	bne	d218 <_free_r+0xf4>
    d234:	e592300c 	ldr	r3, [r2, #12]
    d238:	e58c300c 	str	r3, [ip, #12]
    d23c:	e58c2008 	str	r2, [ip, #8]
    d240:	e582c00c 	str	ip, [r2, #12]
    d244:	e583c008 	str	ip, [r3, #8]
    d248:	e1a00006 	mov	r0, r6
    d24c:	eb00036a 	bl	dffc <__malloc_unlock>
    d250:	e8bd40f8 	pop	{r3, r4, r5, r6, r7, lr}
    d254:	e12fff1e 	bx	lr
    d258:	e5912004 	ldr	r2, [r1, #4]
    d25c:	e1a031a3 	lsr	r3, r3, #3
    d260:	e1a00143 	asr	r0, r3, #2
    d264:	e3a0e001 	mov	lr, #1
    d268:	e182001e 	orr	r0, r2, lr, lsl r0
    d26c:	e0813183 	add	r3, r1, r3, lsl #3
    d270:	e5932008 	ldr	r2, [r3, #8]
    d274:	e58c300c 	str	r3, [ip, #12]
    d278:	e58c2008 	str	r2, [ip, #8]
    d27c:	e5810004 	str	r0, [r1, #4]
    d280:	e583c008 	str	ip, [r3, #8]
    d284:	e582c00c 	str	ip, [r2, #12]
    d288:	eaffffee 	b	d248 <_free_r+0x124>
    d28c:	e31e0001 	tst	lr, #1
    d290:	e0853003 	add	r3, r5, r3
    d294:	1a000006 	bne	d2b4 <_free_r+0x190>
    d298:	e5142008 	ldr	r2, [r4, #-8]
    d29c:	e062c00c 	rsb	ip, r2, ip
    d2a0:	e59c000c 	ldr	r0, [ip, #12]
    d2a4:	e59ce008 	ldr	lr, [ip, #8]
    d2a8:	e58e000c 	str	r0, [lr, #12]
    d2ac:	e580e008 	str	lr, [r0, #8]
    d2b0:	e0833002 	add	r3, r3, r2
    d2b4:	e59f20c8 	ldr	r2, [pc, #200]	; d384 <_free_r+0x260>
    d2b8:	e5920000 	ldr	r0, [r2]
    d2bc:	e3832001 	orr	r2, r3, #1
    d2c0:	e1530000 	cmp	r3, r0
    d2c4:	e58c2004 	str	r2, [ip, #4]
    d2c8:	e581c008 	str	ip, [r1, #8]
    d2cc:	3affffdd 	bcc	d248 <_free_r+0x124>
    d2d0:	e59f30b0 	ldr	r3, [pc, #176]	; d388 <_free_r+0x264>
    d2d4:	e1a00006 	mov	r0, r6
    d2d8:	e5931000 	ldr	r1, [r3]
    d2dc:	ebffff51 	bl	d028 <_malloc_trim_r>
    d2e0:	eaffffd8 	b	d248 <_free_r+0x124>
    d2e4:	e3832001 	orr	r2, r3, #1
    d2e8:	e581c014 	str	ip, [r1, #20]
    d2ec:	e581c010 	str	ip, [r1, #16]
    d2f0:	e58c000c 	str	r0, [ip, #12]
    d2f4:	e58c0008 	str	r0, [ip, #8]
    d2f8:	e58c2004 	str	r2, [ip, #4]
    d2fc:	e78c3003 	str	r3, [ip, r3]
    d300:	eaffffd0 	b	d248 <_free_r+0x124>
    d304:	e3520014 	cmp	r2, #20
    d308:	9282e05b 	addls	lr, r2, #91	; 0x5b
    d30c:	91a0008e 	lslls	r0, lr, #1
    d310:	9affffbb 	bls	d204 <_free_r+0xe0>
    d314:	e3520054 	cmp	r2, #84	; 0x54
    d318:	8a00000a 	bhi	d348 <_free_r+0x224>
    d31c:	e1a0e623 	lsr	lr, r3, #12
    d320:	e28ee06e 	add	lr, lr, #110	; 0x6e
    d324:	e1a0008e 	lsl	r0, lr, #1
    d328:	eaffffb5 	b	d204 <_free_r+0xe0>
    d32c:	e5913004 	ldr	r3, [r1, #4]
    d330:	e1a0e14e 	asr	lr, lr, #2
    d334:	e3a00001 	mov	r0, #1
    d338:	e1830e10 	orr	r0, r3, r0, lsl lr
    d33c:	e1a03002 	mov	r3, r2
    d340:	e5810004 	str	r0, [r1, #4]
    d344:	eaffffbb 	b	d238 <_free_r+0x114>
    d348:	e3520f55 	cmp	r2, #340	; 0x154
    d34c:	91a0e7a3 	lsrls	lr, r3, #15
    d350:	928ee077 	addls	lr, lr, #119	; 0x77
    d354:	91a0008e 	lslls	r0, lr, #1
    d358:	9affffa9 	bls	d204 <_free_r+0xe0>
    d35c:	e59f0028 	ldr	r0, [pc, #40]	; d38c <_free_r+0x268>
    d360:	e1520000 	cmp	r2, r0
    d364:	91a0e923 	lsrls	lr, r3, #18
    d368:	928ee07c 	addls	lr, lr, #124	; 0x7c
    d36c:	91a0008e 	lslls	r0, lr, #1
    d370:	83a000fc 	movhi	r0, #252	; 0xfc
    d374:	83a0e07e 	movhi	lr, #126	; 0x7e
    d378:	eaffffa1 	b	d204 <_free_r+0xe0>
    d37c:	0001aaac 	.word	0x0001aaac
    d380:	0001aab4 	.word	0x0001aab4
    d384:	0001aeb8 	.word	0x0001aeb8
    d388:	0001af98 	.word	0x0001af98
    d38c:	00000554 	.word	0x00000554

0000d390 <_fwalk>:
    d390:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
    d394:	e2906e2e 	adds	r6, r0, #736	; 0x2e0
    d398:	e1a08001 	mov	r8, r1
    d39c:	0a00001a 	beq	d40c <_fwalk+0x7c>
    d3a0:	e3a07000 	mov	r7, #0
    d3a4:	e5965004 	ldr	r5, [r6, #4]
    d3a8:	e2555001 	subs	r5, r5, #1
    d3ac:	e5964008 	ldr	r4, [r6, #8]
    d3b0:	4a00000f 	bmi	d3f4 <_fwalk+0x64>
    d3b4:	e1d430bc 	ldrh	r3, [r4, #12]
    d3b8:	e3530000 	cmp	r3, #0
    d3bc:	e2455001 	sub	r5, r5, #1
    d3c0:	0a000008 	beq	d3e8 <_fwalk+0x58>
    d3c4:	e3530001 	cmp	r3, #1
    d3c8:	9a000006 	bls	d3e8 <_fwalk+0x58>
    d3cc:	e1d430fe 	ldrsh	r3, [r4, #14]
    d3d0:	e3730001 	cmn	r3, #1
    d3d4:	e1a00004 	mov	r0, r4
    d3d8:	0a000002 	beq	d3e8 <_fwalk+0x58>
    d3dc:	e1a0e00f 	mov	lr, pc
    d3e0:	e12fff18 	bx	r8
    d3e4:	e1877000 	orr	r7, r7, r0
    d3e8:	e3750001 	cmn	r5, #1
    d3ec:	e2844068 	add	r4, r4, #104	; 0x68
    d3f0:	1affffef 	bne	d3b4 <_fwalk+0x24>
    d3f4:	e5966000 	ldr	r6, [r6]
    d3f8:	e3560000 	cmp	r6, #0
    d3fc:	1affffe8 	bne	d3a4 <_fwalk+0x14>
    d400:	e1a00007 	mov	r0, r7
    d404:	e8bd41f0 	pop	{r4, r5, r6, r7, r8, lr}
    d408:	e12fff1e 	bx	lr
    d40c:	e1a07006 	mov	r7, r6
    d410:	eafffffa 	b	d400 <_fwalk+0x70>

0000d414 <_fwalk_reent>:
    d414:	e92d45f8 	push	{r3, r4, r5, r6, r7, r8, sl, lr}
    d418:	e2906e2e 	adds	r6, r0, #736	; 0x2e0
    d41c:	e1a08000 	mov	r8, r0
    d420:	e1a0a001 	mov	sl, r1
    d424:	0a00001b 	beq	d498 <_fwalk_reent+0x84>
    d428:	e3a07000 	mov	r7, #0
    d42c:	e5965004 	ldr	r5, [r6, #4]
    d430:	e2555001 	subs	r5, r5, #1
    d434:	e5964008 	ldr	r4, [r6, #8]
    d438:	4a000010 	bmi	d480 <_fwalk_reent+0x6c>
    d43c:	e1d430bc 	ldrh	r3, [r4, #12]
    d440:	e3530000 	cmp	r3, #0
    d444:	e2455001 	sub	r5, r5, #1
    d448:	0a000009 	beq	d474 <_fwalk_reent+0x60>
    d44c:	e3530001 	cmp	r3, #1
    d450:	9a000007 	bls	d474 <_fwalk_reent+0x60>
    d454:	e1d430fe 	ldrsh	r3, [r4, #14]
    d458:	e3730001 	cmn	r3, #1
    d45c:	e1a01004 	mov	r1, r4
    d460:	e1a00008 	mov	r0, r8
    d464:	0a000002 	beq	d474 <_fwalk_reent+0x60>
    d468:	e1a0e00f 	mov	lr, pc
    d46c:	e12fff1a 	bx	sl
    d470:	e1877000 	orr	r7, r7, r0
    d474:	e3750001 	cmn	r5, #1
    d478:	e2844068 	add	r4, r4, #104	; 0x68
    d47c:	1affffee 	bne	d43c <_fwalk_reent+0x28>
    d480:	e5966000 	ldr	r6, [r6]
    d484:	e3560000 	cmp	r6, #0
    d488:	1affffe7 	bne	d42c <_fwalk_reent+0x18>
    d48c:	e1a00007 	mov	r0, r7
    d490:	e8bd45f8 	pop	{r3, r4, r5, r6, r7, r8, sl, lr}
    d494:	e12fff1e 	bx	lr
    d498:	e1a07006 	mov	r7, r6
    d49c:	eafffffa 	b	d48c <_fwalk_reent+0x78>

0000d4a0 <_setlocale_r>:
    d4a0:	e92d4010 	push	{r4, lr}
    d4a4:	e2524000 	subs	r4, r2, #0
    d4a8:	0a000004 	beq	d4c0 <_setlocale_r+0x20>
    d4ac:	e1a00004 	mov	r0, r4
    d4b0:	e59f1048 	ldr	r1, [pc, #72]	; d500 <_setlocale_r+0x60>
    d4b4:	eb00067b 	bl	eea8 <strcmp>
    d4b8:	e3500000 	cmp	r0, #0
    d4bc:	1a000002 	bne	d4cc <_setlocale_r+0x2c>
    d4c0:	e59f003c 	ldr	r0, [pc, #60]	; d504 <_setlocale_r+0x64>
    d4c4:	e8bd4010 	pop	{r4, lr}
    d4c8:	e12fff1e 	bx	lr
    d4cc:	e1a00004 	mov	r0, r4
    d4d0:	e59f102c 	ldr	r1, [pc, #44]	; d504 <_setlocale_r+0x64>
    d4d4:	eb000673 	bl	eea8 <strcmp>
    d4d8:	e3500000 	cmp	r0, #0
    d4dc:	0afffff7 	beq	d4c0 <_setlocale_r+0x20>
    d4e0:	e1a00004 	mov	r0, r4
    d4e4:	e59f101c 	ldr	r1, [pc, #28]	; d508 <_setlocale_r+0x68>
    d4e8:	eb00066e 	bl	eea8 <strcmp>
    d4ec:	e59f3010 	ldr	r3, [pc, #16]	; d504 <_setlocale_r+0x64>
    d4f0:	e3500000 	cmp	r0, #0
    d4f4:	01a00003 	moveq	r0, r3
    d4f8:	13a00000 	movne	r0, #0
    d4fc:	eafffff0 	b	d4c4 <_setlocale_r+0x24>
    d500:	00012378 	.word	0x00012378
    d504:	000122fc 	.word	0x000122fc
    d508:	00012344 	.word	0x00012344

0000d50c <__locale_charset>:
    d50c:	e59f0000 	ldr	r0, [pc]	; d514 <__locale_charset+0x8>
    d510:	e12fff1e 	bx	lr
    d514:	0001aa30 	.word	0x0001aa30

0000d518 <__locale_mb_cur_max>:
    d518:	e59f3004 	ldr	r3, [pc, #4]	; d524 <__locale_mb_cur_max+0xc>
    d51c:	e5930020 	ldr	r0, [r3, #32]
    d520:	e12fff1e 	bx	lr
    d524:	0001aa30 	.word	0x0001aa30

0000d528 <__locale_msgcharset>:
    d528:	e59f0000 	ldr	r0, [pc]	; d530 <__locale_msgcharset+0x8>
    d52c:	e12fff1e 	bx	lr
    d530:	0001aa54 	.word	0x0001aa54

0000d534 <__locale_cjk_lang>:
    d534:	e3a00000 	mov	r0, #0
    d538:	e12fff1e 	bx	lr

0000d53c <_localeconv_r>:
    d53c:	e59f0000 	ldr	r0, [pc]	; d544 <_localeconv_r+0x8>
    d540:	e12fff1e 	bx	lr
    d544:	0001aa74 	.word	0x0001aa74

0000d548 <setlocale>:
    d548:	e1a02001 	mov	r2, r1
    d54c:	e59f100c 	ldr	r1, [pc, #12]	; d560 <setlocale+0x18>
    d550:	e1a03000 	mov	r3, r0
    d554:	e5910000 	ldr	r0, [r1]
    d558:	e1a01003 	mov	r1, r3
    d55c:	eaffffcf 	b	d4a0 <_setlocale_r>
    d560:	0001a600 	.word	0x0001a600

0000d564 <localeconv>:
    d564:	e59f0000 	ldr	r0, [pc]	; d56c <localeconv+0x8>
    d568:	e12fff1e 	bx	lr
    d56c:	0001aa74 	.word	0x0001aa74

0000d570 <__smakebuf_r>:
    d570:	e92d40f0 	push	{r4, r5, r6, r7, lr}
    d574:	e1d130bc 	ldrh	r3, [r1, #12]
    d578:	e3130002 	tst	r3, #2
    d57c:	e24dd044 	sub	sp, sp, #68	; 0x44
    d580:	e1a04001 	mov	r4, r1
    d584:	e1a05000 	mov	r5, r0
    d588:	1a00002b 	bne	d63c <__smakebuf_r+0xcc>
    d58c:	e1d110fe 	ldrsh	r1, [r1, #14]
    d590:	e3510000 	cmp	r1, #0
    d594:	ba000010 	blt	d5dc <__smakebuf_r+0x6c>
    d598:	e28d2004 	add	r2, sp, #4
    d59c:	eb000c2e 	bl	1065c <_fstat_r>
    d5a0:	e3500000 	cmp	r0, #0
    d5a4:	ba00000b 	blt	d5d8 <__smakebuf_r+0x68>
    d5a8:	e59d3008 	ldr	r3, [sp, #8]
    d5ac:	e2033a0f 	and	r3, r3, #61440	; 0xf000
    d5b0:	e2532a02 	subs	r2, r3, #8192	; 0x2000
    d5b4:	e2727000 	rsbs	r7, r2, #0
    d5b8:	e0b77002 	adcs	r7, r7, r2
    d5bc:	e3530902 	cmp	r3, #32768	; 0x8000
    d5c0:	0a000023 	beq	d654 <__smakebuf_r+0xe4>
    d5c4:	e1d430bc 	ldrh	r3, [r4, #12]
    d5c8:	e3833b02 	orr	r3, r3, #2048	; 0x800
    d5cc:	e1c430bc 	strh	r3, [r4, #12]
    d5d0:	e3a06b01 	mov	r6, #1024	; 0x400
    d5d4:	ea000006 	b	d5f4 <__smakebuf_r+0x84>
    d5d8:	e1d430bc 	ldrh	r3, [r4, #12]
    d5dc:	e3130080 	tst	r3, #128	; 0x80
    d5e0:	e3833b02 	orr	r3, r3, #2048	; 0x800
    d5e4:	e1c430bc 	strh	r3, [r4, #12]
    d5e8:	03a06b01 	moveq	r6, #1024	; 0x400
    d5ec:	13a06040 	movne	r6, #64	; 0x40
    d5f0:	e3a07000 	mov	r7, #0
    d5f4:	e1a00005 	mov	r0, r5
    d5f8:	e1a01006 	mov	r1, r6
    d5fc:	eb000032 	bl	d6cc <_malloc_r>
    d600:	e3500000 	cmp	r0, #0
    d604:	e1d430bc 	ldrh	r3, [r4, #12]
    d608:	0a000023 	beq	d69c <__smakebuf_r+0x12c>
    d60c:	e59f20b0 	ldr	r2, [pc, #176]	; d6c4 <__smakebuf_r+0x154>
    d610:	e3833080 	orr	r3, r3, #128	; 0x80
    d614:	e3570000 	cmp	r7, #0
    d618:	e585203c 	str	r2, [r5, #60]	; 0x3c
    d61c:	e1c430bc 	strh	r3, [r4, #12]
    d620:	e5840000 	str	r0, [r4]
    d624:	e5840010 	str	r0, [r4, #16]
    d628:	e5846014 	str	r6, [r4, #20]
    d62c:	1a000012 	bne	d67c <__smakebuf_r+0x10c>
    d630:	e28dd044 	add	sp, sp, #68	; 0x44
    d634:	e8bd40f0 	pop	{r4, r5, r6, r7, lr}
    d638:	e12fff1e 	bx	lr
    d63c:	e2813043 	add	r3, r1, #67	; 0x43
    d640:	e5813000 	str	r3, [r1]
    d644:	e5813010 	str	r3, [r1, #16]
    d648:	e3a03001 	mov	r3, #1
    d64c:	e5813014 	str	r3, [r1, #20]
    d650:	eafffff6 	b	d630 <__smakebuf_r+0xc0>
    d654:	e5942028 	ldr	r2, [r4, #40]	; 0x28
    d658:	e59f3068 	ldr	r3, [pc, #104]	; d6c8 <__smakebuf_r+0x158>
    d65c:	e1520003 	cmp	r2, r3
    d660:	1affffd7 	bne	d5c4 <__smakebuf_r+0x54>
    d664:	e1d430bc 	ldrh	r3, [r4, #12]
    d668:	e3a06b01 	mov	r6, #1024	; 0x400
    d66c:	e1833006 	orr	r3, r3, r6
    d670:	e1c430bc 	strh	r3, [r4, #12]
    d674:	e584604c 	str	r6, [r4, #76]	; 0x4c
    d678:	eaffffdd 	b	d5f4 <__smakebuf_r+0x84>
    d67c:	e1a00005 	mov	r0, r5
    d680:	e1d410fe 	ldrsh	r1, [r4, #14]
    d684:	eb000d34 	bl	10b5c <_isatty_r>
    d688:	e3500000 	cmp	r0, #0
    d68c:	11d430bc 	ldrhne	r3, [r4, #12]
    d690:	13833001 	orrne	r3, r3, #1
    d694:	11c430bc 	strhne	r3, [r4, #12]
    d698:	eaffffe4 	b	d630 <__smakebuf_r+0xc0>
    d69c:	e3130c02 	tst	r3, #512	; 0x200
    d6a0:	1affffe2 	bne	d630 <__smakebuf_r+0xc0>
    d6a4:	e3833002 	orr	r3, r3, #2
    d6a8:	e2842043 	add	r2, r4, #67	; 0x43
    d6ac:	e1c430bc 	strh	r3, [r4, #12]
    d6b0:	e3a03001 	mov	r3, #1
    d6b4:	e5842000 	str	r2, [r4]
    d6b8:	e5842010 	str	r2, [r4, #16]
    d6bc:	e5843014 	str	r3, [r4, #20]
    d6c0:	eaffffda 	b	d630 <__smakebuf_r+0xc0>
    d6c4:	0000cd74 	.word	0x0000cd74
    d6c8:	0000ee60 	.word	0x0000ee60

0000d6cc <_malloc_r>:
    d6cc:	e92d4ff0 	push	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
    d6d0:	e281500b 	add	r5, r1, #11
    d6d4:	e3550016 	cmp	r5, #22
    d6d8:	83c55007 	bichi	r5, r5, #7
    d6dc:	81a03fa5 	lsrhi	r3, r5, #31
    d6e0:	93a03000 	movls	r3, #0
    d6e4:	93a05010 	movls	r5, #16
    d6e8:	e1550001 	cmp	r5, r1
    d6ec:	21a01003 	movcs	r1, r3
    d6f0:	33831001 	orrcc	r1, r3, #1
    d6f4:	e3510000 	cmp	r1, #0
    d6f8:	13a0300c 	movne	r3, #12
    d6fc:	e24dd00c 	sub	sp, sp, #12
    d700:	e1a06000 	mov	r6, r0
    d704:	15803000 	strne	r3, [r0]
    d708:	13a04000 	movne	r4, #0
    d70c:	1a000015 	bne	d768 <_malloc_r+0x9c>
    d710:	eb000238 	bl	dff8 <__malloc_lock>
    d714:	e3550f7e 	cmp	r5, #504	; 0x1f8
    d718:	2a000016 	bcs	d778 <_malloc_r+0xac>
    d71c:	e59f76b8 	ldr	r7, [pc, #1720]	; dddc <_malloc_r+0x710>
    d720:	e1a0e1a5 	lsr	lr, r5, #3
    d724:	e087318e 	add	r3, r7, lr, lsl #3
    d728:	e593400c 	ldr	r4, [r3, #12]
    d72c:	e1540003 	cmp	r4, r3
    d730:	0a00014b 	beq	dc64 <_malloc_r+0x598>
    d734:	e5943004 	ldr	r3, [r4, #4]
    d738:	e3c33003 	bic	r3, r3, #3
    d73c:	e0843003 	add	r3, r4, r3
    d740:	e5930004 	ldr	r0, [r3, #4]
    d744:	e2841008 	add	r1, r4, #8
    d748:	e8910006 	ldm	r1, {r1, r2}
    d74c:	e3800001 	orr	r0, r0, #1
    d750:	e5830004 	str	r0, [r3, #4]
    d754:	e581200c 	str	r2, [r1, #12]
    d758:	e5821008 	str	r1, [r2, #8]
    d75c:	e1a00006 	mov	r0, r6
    d760:	eb000225 	bl	dffc <__malloc_unlock>
    d764:	e2844008 	add	r4, r4, #8
    d768:	e1a00004 	mov	r0, r4
    d76c:	e28dd00c 	add	sp, sp, #12
    d770:	e8bd4ff0 	pop	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
    d774:	e12fff1e 	bx	lr
    d778:	e1b0e4a5 	lsrs	lr, r5, #9
    d77c:	01a0e1a5 	lsreq	lr, r5, #3
    d780:	01a0108e 	lsleq	r1, lr, #1
    d784:	1a000061 	bne	d910 <_malloc_r+0x244>
    d788:	e59f764c 	ldr	r7, [pc, #1612]	; dddc <_malloc_r+0x710>
    d78c:	e0871101 	add	r1, r7, r1, lsl #2
    d790:	e591400c 	ldr	r4, [r1, #12]
    d794:	e1510004 	cmp	r1, r4
    d798:	1a000005 	bne	d7b4 <_malloc_r+0xe8>
    d79c:	ea00000a 	b	d7cc <_malloc_r+0x100>
    d7a0:	e3530000 	cmp	r3, #0
    d7a4:	aa0000d7 	bge	db08 <_malloc_r+0x43c>
    d7a8:	e594400c 	ldr	r4, [r4, #12]
    d7ac:	e1510004 	cmp	r1, r4
    d7b0:	0a000005 	beq	d7cc <_malloc_r+0x100>
    d7b4:	e5942004 	ldr	r2, [r4, #4]
    d7b8:	e3c22003 	bic	r2, r2, #3
    d7bc:	e0653002 	rsb	r3, r5, r2
    d7c0:	e353000f 	cmp	r3, #15
    d7c4:	dafffff5 	ble	d7a0 <_malloc_r+0xd4>
    d7c8:	e24ee001 	sub	lr, lr, #1
    d7cc:	e28ee001 	add	lr, lr, #1
    d7d0:	e59f3604 	ldr	r3, [pc, #1540]	; dddc <_malloc_r+0x710>
    d7d4:	e5974010 	ldr	r4, [r7, #16]
    d7d8:	e2838008 	add	r8, r3, #8
    d7dc:	e1540008 	cmp	r4, r8
    d7e0:	05931004 	ldreq	r1, [r3, #4]
    d7e4:	0a000016 	beq	d844 <_malloc_r+0x178>
    d7e8:	e5942004 	ldr	r2, [r4, #4]
    d7ec:	e3c22003 	bic	r2, r2, #3
    d7f0:	e0651002 	rsb	r1, r5, r2
    d7f4:	e351000f 	cmp	r1, #15
    d7f8:	ca0000f7 	bgt	dbdc <_malloc_r+0x510>
    d7fc:	e3510000 	cmp	r1, #0
    d800:	e5838014 	str	r8, [r3, #20]
    d804:	e5838010 	str	r8, [r3, #16]
    d808:	aa000046 	bge	d928 <_malloc_r+0x25c>
    d80c:	e3520c02 	cmp	r2, #512	; 0x200
    d810:	2a0000d2 	bcs	db60 <_malloc_r+0x494>
    d814:	e5930004 	ldr	r0, [r3, #4]
    d818:	e1a021a2 	lsr	r2, r2, #3
    d81c:	e1a01142 	asr	r1, r2, #2
    d820:	e3a0c001 	mov	ip, #1
    d824:	e180111c 	orr	r1, r0, ip, lsl r1
    d828:	e0832182 	add	r2, r3, r2, lsl #3
    d82c:	e5920008 	ldr	r0, [r2, #8]
    d830:	e584200c 	str	r2, [r4, #12]
    d834:	e5840008 	str	r0, [r4, #8]
    d838:	e5831004 	str	r1, [r3, #4]
    d83c:	e5824008 	str	r4, [r2, #8]
    d840:	e580400c 	str	r4, [r0, #12]
    d844:	e1a0314e 	asr	r3, lr, #2
    d848:	e3a00001 	mov	r0, #1
    d84c:	e1a00310 	lsl	r0, r0, r3
    d850:	e1500001 	cmp	r0, r1
    d854:	8a00003b 	bhi	d948 <_malloc_r+0x27c>
    d858:	e1110000 	tst	r1, r0
    d85c:	1a000008 	bne	d884 <_malloc_r+0x1b8>
    d860:	e1a00080 	lsl	r0, r0, #1
    d864:	e3cee003 	bic	lr, lr, #3
    d868:	e1110000 	tst	r1, r0
    d86c:	e28ee004 	add	lr, lr, #4
    d870:	1a000003 	bne	d884 <_malloc_r+0x1b8>
    d874:	e1a00080 	lsl	r0, r0, #1
    d878:	e1110000 	tst	r1, r0
    d87c:	e28ee004 	add	lr, lr, #4
    d880:	0afffffb 	beq	d874 <_malloc_r+0x1a8>
    d884:	e087a18e 	add	sl, r7, lr, lsl #3
    d888:	e1a0c00a 	mov	ip, sl
    d88c:	e1a0400e 	mov	r4, lr
    d890:	e59c300c 	ldr	r3, [ip, #12]
    d894:	e15c0003 	cmp	ip, r3
    d898:	1a000005 	bne	d8b4 <_malloc_r+0x1e8>
    d89c:	ea0000db 	b	dc10 <_malloc_r+0x544>
    d8a0:	e3520000 	cmp	r2, #0
    d8a4:	aa0000f4 	bge	dc7c <_malloc_r+0x5b0>
    d8a8:	e593300c 	ldr	r3, [r3, #12]
    d8ac:	e15c0003 	cmp	ip, r3
    d8b0:	0a0000d6 	beq	dc10 <_malloc_r+0x544>
    d8b4:	e5931004 	ldr	r1, [r3, #4]
    d8b8:	e3c11003 	bic	r1, r1, #3
    d8bc:	e0652001 	rsb	r2, r5, r1
    d8c0:	e352000f 	cmp	r2, #15
    d8c4:	dafffff5 	ble	d8a0 <_malloc_r+0x1d4>
    d8c8:	e1a04003 	mov	r4, r3
    d8cc:	e593000c 	ldr	r0, [r3, #12]
    d8d0:	e5b4c008 	ldr	ip, [r4, #8]!
    d8d4:	e0831005 	add	r1, r3, r5
    d8d8:	e382e001 	orr	lr, r2, #1
    d8dc:	e3855001 	orr	r5, r5, #1
    d8e0:	e58c000c 	str	r0, [ip, #12]
    d8e4:	e580c008 	str	ip, [r0, #8]
    d8e8:	e5835004 	str	r5, [r3, #4]
    d8ec:	e5871014 	str	r1, [r7, #20]
    d8f0:	e5871010 	str	r1, [r7, #16]
    d8f4:	e581800c 	str	r8, [r1, #12]
    d8f8:	e5818008 	str	r8, [r1, #8]
    d8fc:	e581e004 	str	lr, [r1, #4]
    d900:	e7812002 	str	r2, [r1, r2]
    d904:	e1a00006 	mov	r0, r6
    d908:	eb0001bb 	bl	dffc <__malloc_unlock>
    d90c:	eaffff95 	b	d768 <_malloc_r+0x9c>
    d910:	e35e0004 	cmp	lr, #4
    d914:	8a000087 	bhi	db38 <_malloc_r+0x46c>
    d918:	e1a0e325 	lsr	lr, r5, #6
    d91c:	e28ee038 	add	lr, lr, #56	; 0x38
    d920:	e1a0108e 	lsl	r1, lr, #1
    d924:	eaffff97 	b	d788 <_malloc_r+0xbc>
    d928:	e0842002 	add	r2, r4, r2
    d92c:	e5923004 	ldr	r3, [r2, #4]
    d930:	e3833001 	orr	r3, r3, #1
    d934:	e5823004 	str	r3, [r2, #4]
    d938:	e1a00006 	mov	r0, r6
    d93c:	eb0001ae 	bl	dffc <__malloc_unlock>
    d940:	e2844008 	add	r4, r4, #8
    d944:	eaffff87 	b	d768 <_malloc_r+0x9c>
    d948:	e5974008 	ldr	r4, [r7, #8]
    d94c:	e594a004 	ldr	sl, [r4, #4]
    d950:	e3caa003 	bic	sl, sl, #3
    d954:	e155000a 	cmp	r5, sl
    d958:	8a000002 	bhi	d968 <_malloc_r+0x29c>
    d95c:	e065300a 	rsb	r3, r5, sl
    d960:	e353000f 	cmp	r3, #15
    d964:	ca00005d 	bgt	dae0 <_malloc_r+0x414>
    d968:	e59f9470 	ldr	r9, [pc, #1136]	; dde0 <_malloc_r+0x714>
    d96c:	e5973408 	ldr	r3, [r7, #1032]	; 0x408
    d970:	e599b000 	ldr	fp, [r9]
    d974:	e3730001 	cmn	r3, #1
    d978:	e085b00b 	add	fp, r5, fp
    d97c:	128bba01 	addne	fp, fp, #4096	; 0x1000
    d980:	128bb00f 	addne	fp, fp, #15
    d984:	13cbbeff 	bicne	fp, fp, #4080	; 0xff0
    d988:	028bb010 	addeq	fp, fp, #16
    d98c:	13cbb00f 	bicne	fp, fp, #15
    d990:	e084200a 	add	r2, r4, sl
    d994:	e1a00006 	mov	r0, r6
    d998:	e1a0100b 	mov	r1, fp
    d99c:	e58d2004 	str	r2, [sp, #4]
    d9a0:	eb0004de 	bl	ed20 <_sbrk_r>
    d9a4:	e3700001 	cmn	r0, #1
    d9a8:	e1a08000 	mov	r8, r0
    d9ac:	e59d2004 	ldr	r2, [sp, #4]
    d9b0:	0a0000e2 	beq	dd40 <_malloc_r+0x674>
    d9b4:	e1520000 	cmp	r2, r0
    d9b8:	8a0000bf 	bhi	dcbc <_malloc_r+0x5f0>
    d9bc:	e5993004 	ldr	r3, [r9, #4]
    d9c0:	e1520008 	cmp	r2, r8
    d9c4:	e08b3003 	add	r3, fp, r3
    d9c8:	e5893004 	str	r3, [r9, #4]
    d9cc:	0a0000df 	beq	dd50 <_malloc_r+0x684>
    d9d0:	e5971408 	ldr	r1, [r7, #1032]	; 0x408
    d9d4:	e3710001 	cmn	r1, #1
    d9d8:	10622008 	rsbne	r2, r2, r8
    d9dc:	059f33f8 	ldreq	r3, [pc, #1016]	; dddc <_malloc_r+0x710>
    d9e0:	10833002 	addne	r3, r3, r2
    d9e4:	05838408 	streq	r8, [r3, #1032]	; 0x408
    d9e8:	15893004 	strne	r3, [r9, #4]
    d9ec:	e2183007 	ands	r3, r8, #7
    d9f0:	12632008 	rsbne	r2, r3, #8
    d9f4:	10888002 	addne	r8, r8, r2
    d9f8:	12633a01 	rsbne	r3, r3, #4096	; 0x1000
    d9fc:	12832008 	addne	r2, r3, #8
    da00:	e088300b 	add	r3, r8, fp
    da04:	03a02a01 	moveq	r2, #4096	; 0x1000
    da08:	e1a03a03 	lsl	r3, r3, #20
    da0c:	e042ba23 	sub	fp, r2, r3, lsr #20
    da10:	e1a0100b 	mov	r1, fp
    da14:	e1a00006 	mov	r0, r6
    da18:	eb0004c0 	bl	ed20 <_sbrk_r>
    da1c:	e3700001 	cmn	r0, #1
    da20:	10682000 	rsbne	r2, r8, r0
    da24:	e5993004 	ldr	r3, [r9, #4]
    da28:	108b2002 	addne	r2, fp, r2
    da2c:	03a0b000 	moveq	fp, #0
    da30:	13822001 	orrne	r2, r2, #1
    da34:	03a02001 	moveq	r2, #1
    da38:	e08b3003 	add	r3, fp, r3
    da3c:	e1540007 	cmp	r4, r7
    da40:	e5893004 	str	r3, [r9, #4]
    da44:	e5878008 	str	r8, [r7, #8]
    da48:	e5882004 	str	r2, [r8, #4]
    da4c:	0a00000d 	beq	da88 <_malloc_r+0x3bc>
    da50:	e35a000f 	cmp	sl, #15
    da54:	9a0000ac 	bls	dd0c <_malloc_r+0x640>
    da58:	e594c004 	ldr	ip, [r4, #4]
    da5c:	e24a200c 	sub	r2, sl, #12
    da60:	e3c22007 	bic	r2, r2, #7
    da64:	e20cc001 	and	ip, ip, #1
    da68:	e0841002 	add	r1, r4, r2
    da6c:	e3a00005 	mov	r0, #5
    da70:	e182c00c 	orr	ip, r2, ip
    da74:	e352000f 	cmp	r2, #15
    da78:	e584c004 	str	ip, [r4, #4]
    da7c:	e5810004 	str	r0, [r1, #4]
    da80:	e5810008 	str	r0, [r1, #8]
    da84:	8a0000b8 	bhi	dd6c <_malloc_r+0x6a0>
    da88:	e599202c 	ldr	r2, [r9, #44]	; 0x2c
    da8c:	e1530002 	cmp	r3, r2
    da90:	859f2348 	ldrhi	r2, [pc, #840]	; dde0 <_malloc_r+0x714>
    da94:	8582302c 	strhi	r3, [r2, #44]	; 0x2c
    da98:	e5992030 	ldr	r2, [r9, #48]	; 0x30
    da9c:	e5974008 	ldr	r4, [r7, #8]
    daa0:	e1530002 	cmp	r3, r2
    daa4:	95943004 	ldrls	r3, [r4, #4]
    daa8:	85942004 	ldrhi	r2, [r4, #4]
    daac:	859f132c 	ldrhi	r1, [pc, #812]	; dde0 <_malloc_r+0x714>
    dab0:	93c33003 	bicls	r3, r3, #3
    dab4:	85813030 	strhi	r3, [r1, #48]	; 0x30
    dab8:	83c23003 	bichi	r3, r2, #3
    dabc:	e1550003 	cmp	r5, r3
    dac0:	e0653003 	rsb	r3, r5, r3
    dac4:	8a000001 	bhi	dad0 <_malloc_r+0x404>
    dac8:	e353000f 	cmp	r3, #15
    dacc:	ca000003 	bgt	dae0 <_malloc_r+0x414>
    dad0:	e1a00006 	mov	r0, r6
    dad4:	eb000148 	bl	dffc <__malloc_unlock>
    dad8:	e3a04000 	mov	r4, #0
    dadc:	eaffff21 	b	d768 <_malloc_r+0x9c>
    dae0:	e0842005 	add	r2, r4, r5
    dae4:	e3833001 	orr	r3, r3, #1
    dae8:	e3855001 	orr	r5, r5, #1
    daec:	e5845004 	str	r5, [r4, #4]
    daf0:	e1a00006 	mov	r0, r6
    daf4:	e5823004 	str	r3, [r2, #4]
    daf8:	e5872008 	str	r2, [r7, #8]
    dafc:	e2844008 	add	r4, r4, #8
    db00:	eb00013d 	bl	dffc <__malloc_unlock>
    db04:	eaffff17 	b	d768 <_malloc_r+0x9c>
    db08:	e0842002 	add	r2, r4, r2
    db0c:	e5920004 	ldr	r0, [r2, #4]
    db10:	e2841008 	add	r1, r4, #8
    db14:	e891000a 	ldm	r1, {r1, r3}
    db18:	e3800001 	orr	r0, r0, #1
    db1c:	e5820004 	str	r0, [r2, #4]
    db20:	e581300c 	str	r3, [r1, #12]
    db24:	e5831008 	str	r1, [r3, #8]
    db28:	e1a00006 	mov	r0, r6
    db2c:	eb000132 	bl	dffc <__malloc_unlock>
    db30:	e2844008 	add	r4, r4, #8
    db34:	eaffff0b 	b	d768 <_malloc_r+0x9c>
    db38:	e35e0014 	cmp	lr, #20
    db3c:	928ee05b 	addls	lr, lr, #91	; 0x5b
    db40:	91a0108e 	lslls	r1, lr, #1
    db44:	9affff0f 	bls	d788 <_malloc_r+0xbc>
    db48:	e35e0054 	cmp	lr, #84	; 0x54
    db4c:	8a000061 	bhi	dcd8 <_malloc_r+0x60c>
    db50:	e1a0e625 	lsr	lr, r5, #12
    db54:	e28ee06e 	add	lr, lr, #110	; 0x6e
    db58:	e1a0108e 	lsl	r1, lr, #1
    db5c:	eaffff09 	b	d788 <_malloc_r+0xbc>
    db60:	e1a034a2 	lsr	r3, r2, #9
    db64:	e3530004 	cmp	r3, #4
    db68:	9a00004f 	bls	dcac <_malloc_r+0x5e0>
    db6c:	e3530014 	cmp	r3, #20
    db70:	9283a05b 	addls	sl, r3, #91	; 0x5b
    db74:	91a0008a 	lslls	r0, sl, #1
    db78:	9a000004 	bls	db90 <_malloc_r+0x4c4>
    db7c:	e3530054 	cmp	r3, #84	; 0x54
    db80:	8a00007f 	bhi	dd84 <_malloc_r+0x6b8>
    db84:	e1a0a622 	lsr	sl, r2, #12
    db88:	e28aa06e 	add	sl, sl, #110	; 0x6e
    db8c:	e1a0008a 	lsl	r0, sl, #1
    db90:	e0870100 	add	r0, r7, r0, lsl #2
    db94:	e5903008 	ldr	r3, [r0, #8]
    db98:	e1530000 	cmp	r3, r0
    db9c:	e59fc238 	ldr	ip, [pc, #568]	; dddc <_malloc_r+0x710>
    dba0:	0a000052 	beq	dcf0 <_malloc_r+0x624>
    dba4:	e5931004 	ldr	r1, [r3, #4]
    dba8:	e3c11003 	bic	r1, r1, #3
    dbac:	e1520001 	cmp	r2, r1
    dbb0:	2a000002 	bcs	dbc0 <_malloc_r+0x4f4>
    dbb4:	e5933008 	ldr	r3, [r3, #8]
    dbb8:	e1500003 	cmp	r0, r3
    dbbc:	1afffff8 	bne	dba4 <_malloc_r+0x4d8>
    dbc0:	e593200c 	ldr	r2, [r3, #12]
    dbc4:	e5971004 	ldr	r1, [r7, #4]
    dbc8:	e584200c 	str	r2, [r4, #12]
    dbcc:	e5843008 	str	r3, [r4, #8]
    dbd0:	e583400c 	str	r4, [r3, #12]
    dbd4:	e5824008 	str	r4, [r2, #8]
    dbd8:	eaffff19 	b	d844 <_malloc_r+0x178>
    dbdc:	e0842005 	add	r2, r4, r5
    dbe0:	e3810001 	orr	r0, r1, #1
    dbe4:	e3855001 	orr	r5, r5, #1
    dbe8:	e5845004 	str	r5, [r4, #4]
    dbec:	e5832014 	str	r2, [r3, #20]
    dbf0:	e5832010 	str	r2, [r3, #16]
    dbf4:	e582800c 	str	r8, [r2, #12]
    dbf8:	e9820101 	stmib	r2, {r0, r8}
    dbfc:	e7821001 	str	r1, [r2, r1]
    dc00:	e1a00006 	mov	r0, r6
    dc04:	eb0000fc 	bl	dffc <__malloc_unlock>
    dc08:	e2844008 	add	r4, r4, #8
    dc0c:	eafffed5 	b	d768 <_malloc_r+0x9c>
    dc10:	e2844001 	add	r4, r4, #1
    dc14:	e3140003 	tst	r4, #3
    dc18:	e28cc008 	add	ip, ip, #8
    dc1c:	1affff1b 	bne	d890 <_malloc_r+0x1c4>
    dc20:	e31e0003 	tst	lr, #3
    dc24:	e24a3008 	sub	r3, sl, #8
    dc28:	0a000062 	beq	ddb8 <_malloc_r+0x6ec>
    dc2c:	e59aa000 	ldr	sl, [sl]
    dc30:	e15a0003 	cmp	sl, r3
    dc34:	e24ee001 	sub	lr, lr, #1
    dc38:	0afffff8 	beq	dc20 <_malloc_r+0x554>
    dc3c:	e5973004 	ldr	r3, [r7, #4]
    dc40:	e1a00080 	lsl	r0, r0, #1
    dc44:	e1500003 	cmp	r0, r3
    dc48:	8affff3e 	bhi	d948 <_malloc_r+0x27c>
    dc4c:	e3500000 	cmp	r0, #0
    dc50:	0affff3c 	beq	d948 <_malloc_r+0x27c>
    dc54:	e1130000 	tst	r3, r0
    dc58:	0a00005a 	beq	ddc8 <_malloc_r+0x6fc>
    dc5c:	e1a0e004 	mov	lr, r4
    dc60:	eaffff07 	b	d884 <_malloc_r+0x1b8>
    dc64:	e2843008 	add	r3, r4, #8
    dc68:	e5944014 	ldr	r4, [r4, #20]
    dc6c:	e1530004 	cmp	r3, r4
    dc70:	028ee002 	addeq	lr, lr, #2
    dc74:	0afffed5 	beq	d7d0 <_malloc_r+0x104>
    dc78:	eafffead 	b	d734 <_malloc_r+0x68>
    dc7c:	e0831001 	add	r1, r3, r1
    dc80:	e5910004 	ldr	r0, [r1, #4]
    dc84:	e1a04003 	mov	r4, r3
    dc88:	e5b42008 	ldr	r2, [r4, #8]!
    dc8c:	e593300c 	ldr	r3, [r3, #12]
    dc90:	e3800001 	orr	r0, r0, #1
    dc94:	e5810004 	str	r0, [r1, #4]
    dc98:	e582300c 	str	r3, [r2, #12]
    dc9c:	e5832008 	str	r2, [r3, #8]
    dca0:	e1a00006 	mov	r0, r6
    dca4:	eb0000d4 	bl	dffc <__malloc_unlock>
    dca8:	eafffeae 	b	d768 <_malloc_r+0x9c>
    dcac:	e1a0a322 	lsr	sl, r2, #6
    dcb0:	e28aa038 	add	sl, sl, #56	; 0x38
    dcb4:	e1a0008a 	lsl	r0, sl, #1
    dcb8:	eaffffb4 	b	db90 <_malloc_r+0x4c4>
    dcbc:	e1540007 	cmp	r4, r7
    dcc0:	0affff3d 	beq	d9bc <_malloc_r+0x2f0>
    dcc4:	e59f3110 	ldr	r3, [pc, #272]	; dddc <_malloc_r+0x710>
    dcc8:	e5934008 	ldr	r4, [r3, #8]
    dccc:	e5943004 	ldr	r3, [r4, #4]
    dcd0:	e3c33003 	bic	r3, r3, #3
    dcd4:	eaffff78 	b	dabc <_malloc_r+0x3f0>
    dcd8:	e35e0f55 	cmp	lr, #340	; 0x154
    dcdc:	8a00000f 	bhi	dd20 <_malloc_r+0x654>
    dce0:	e1a0e7a5 	lsr	lr, r5, #15
    dce4:	e28ee077 	add	lr, lr, #119	; 0x77
    dce8:	e1a0108e 	lsl	r1, lr, #1
    dcec:	eafffea5 	b	d788 <_malloc_r+0xbc>
    dcf0:	e59c2004 	ldr	r2, [ip, #4]
    dcf4:	e1a0a14a 	asr	sl, sl, #2
    dcf8:	e3a01001 	mov	r1, #1
    dcfc:	e1821a11 	orr	r1, r2, r1, lsl sl
    dd00:	e1a02003 	mov	r2, r3
    dd04:	e58c1004 	str	r1, [ip, #4]
    dd08:	eaffffae 	b	dbc8 <_malloc_r+0x4fc>
    dd0c:	e3a03001 	mov	r3, #1
    dd10:	e5883004 	str	r3, [r8, #4]
    dd14:	e1a04008 	mov	r4, r8
    dd18:	e3a03000 	mov	r3, #0
    dd1c:	eaffff66 	b	dabc <_malloc_r+0x3f0>
    dd20:	e59f30bc 	ldr	r3, [pc, #188]	; dde4 <_malloc_r+0x718>
    dd24:	e15e0003 	cmp	lr, r3
    dd28:	91a0e925 	lsrls	lr, r5, #18
    dd2c:	928ee07c 	addls	lr, lr, #124	; 0x7c
    dd30:	91a0108e 	lslls	r1, lr, #1
    dd34:	83a010fc 	movhi	r1, #252	; 0xfc
    dd38:	83a0e07e 	movhi	lr, #126	; 0x7e
    dd3c:	eafffe91 	b	d788 <_malloc_r+0xbc>
    dd40:	e5974008 	ldr	r4, [r7, #8]
    dd44:	e5943004 	ldr	r3, [r4, #4]
    dd48:	e3c33003 	bic	r3, r3, #3
    dd4c:	eaffff5a 	b	dabc <_malloc_r+0x3f0>
    dd50:	e1b01a02 	lsls	r1, r2, #20
    dd54:	1affff1d 	bne	d9d0 <_malloc_r+0x304>
    dd58:	e5972008 	ldr	r2, [r7, #8]
    dd5c:	e08b100a 	add	r1, fp, sl
    dd60:	e3811001 	orr	r1, r1, #1
    dd64:	e5821004 	str	r1, [r2, #4]
    dd68:	eaffff46 	b	da88 <_malloc_r+0x3bc>
    dd6c:	e1a00006 	mov	r0, r6
    dd70:	e2841008 	add	r1, r4, #8
    dd74:	ebfffcea 	bl	d124 <_free_r>
    dd78:	e59f3060 	ldr	r3, [pc, #96]	; dde0 <_malloc_r+0x714>
    dd7c:	e5933004 	ldr	r3, [r3, #4]
    dd80:	eaffff40 	b	da88 <_malloc_r+0x3bc>
    dd84:	e3530f55 	cmp	r3, #340	; 0x154
    dd88:	91a0a7a2 	lsrls	sl, r2, #15
    dd8c:	928aa077 	addls	sl, sl, #119	; 0x77
    dd90:	91a0008a 	lslls	r0, sl, #1
    dd94:	9affff7d 	bls	db90 <_malloc_r+0x4c4>
    dd98:	e59f1044 	ldr	r1, [pc, #68]	; dde4 <_malloc_r+0x718>
    dd9c:	e1530001 	cmp	r3, r1
    dda0:	91a0a922 	lsrls	sl, r2, #18
    dda4:	928aa07c 	addls	sl, sl, #124	; 0x7c
    dda8:	91a0008a 	lslls	r0, sl, #1
    ddac:	83a000fc 	movhi	r0, #252	; 0xfc
    ddb0:	83a0a07e 	movhi	sl, #126	; 0x7e
    ddb4:	eaffff75 	b	db90 <_malloc_r+0x4c4>
    ddb8:	e5973004 	ldr	r3, [r7, #4]
    ddbc:	e1c33000 	bic	r3, r3, r0
    ddc0:	e5873004 	str	r3, [r7, #4]
    ddc4:	eaffff9d 	b	dc40 <_malloc_r+0x574>
    ddc8:	e1a00080 	lsl	r0, r0, #1
    ddcc:	e1130000 	tst	r3, r0
    ddd0:	e2844004 	add	r4, r4, #4
    ddd4:	1affffa0 	bne	dc5c <_malloc_r+0x590>
    ddd8:	eafffffa 	b	ddc8 <_malloc_r+0x6fc>
    dddc:	0001aaac 	.word	0x0001aaac
    dde0:	0001af98 	.word	0x0001af98
    dde4:	00000554 	.word	0x00000554

0000dde8 <memchr>:
    dde8:	e3100003 	tst	r0, #3
    ddec:	e92d0070 	push	{r4, r5, r6}
    ddf0:	e20110ff 	and	r1, r1, #255	; 0xff
    ddf4:	0a00003c 	beq	deec <memchr+0x104>
    ddf8:	e3520000 	cmp	r2, #0
    ddfc:	e242c001 	sub	ip, r2, #1
    de00:	0a00003b 	beq	def4 <memchr+0x10c>
    de04:	e5d03000 	ldrb	r3, [r0]
    de08:	e1530001 	cmp	r3, r1
    de0c:	0a00001f 	beq	de90 <memchr+0xa8>
    de10:	e2803001 	add	r3, r0, #1
    de14:	ea000006 	b	de34 <memchr+0x4c>
    de18:	e35c0000 	cmp	ip, #0
    de1c:	0a00001d 	beq	de98 <memchr+0xb0>
    de20:	e5d02000 	ldrb	r2, [r0]
    de24:	e1520001 	cmp	r2, r1
    de28:	e2833001 	add	r3, r3, #1
    de2c:	e24cc001 	sub	ip, ip, #1
    de30:	0a000016 	beq	de90 <memchr+0xa8>
    de34:	e3130003 	tst	r3, #3
    de38:	e1a00003 	mov	r0, r3
    de3c:	1afffff5 	bne	de18 <memchr+0x30>
    de40:	e35c0003 	cmp	ip, #3
    de44:	8a000015 	bhi	dea0 <memchr+0xb8>
    de48:	e35c0000 	cmp	ip, #0
    de4c:	e24c4001 	sub	r4, ip, #1
    de50:	0a000010 	beq	de98 <memchr+0xb0>
    de54:	e5d03000 	ldrb	r3, [r0]
    de58:	e1530001 	cmp	r3, r1
    de5c:	0a00000b 	beq	de90 <memchr+0xa8>
    de60:	e2802001 	add	r2, r0, #1
    de64:	e3a03000 	mov	r3, #0
    de68:	ea000004 	b	de80 <memchr+0x98>
    de6c:	e5d0c000 	ldrb	ip, [r0]
    de70:	e15c0001 	cmp	ip, r1
    de74:	e2822001 	add	r2, r2, #1
    de78:	e2833001 	add	r3, r3, #1
    de7c:	0a000003 	beq	de90 <memchr+0xa8>
    de80:	e1540003 	cmp	r4, r3
    de84:	e1a00002 	mov	r0, r2
    de88:	1afffff7 	bne	de6c <memchr+0x84>
    de8c:	e3a00000 	mov	r0, #0
    de90:	e8bd0070 	pop	{r4, r5, r6}
    de94:	e12fff1e 	bx	lr
    de98:	e1a0000c 	mov	r0, ip
    de9c:	eafffffb 	b	de90 <memchr+0xa8>
    dea0:	e1816401 	orr	r6, r1, r1, lsl #8
    dea4:	e1866806 	orr	r6, r6, r6, lsl #16
    dea8:	e5903000 	ldr	r3, [r0]
    deac:	e59f4048 	ldr	r4, [pc, #72]	; defc <memchr+0x114>
    deb0:	e0263003 	eor	r3, r6, r3
    deb4:	e0834004 	add	r4, r3, r4
    deb8:	e59f2040 	ldr	r2, [pc, #64]	; df00 <memchr+0x118>
    debc:	e1c44003 	bic	r4, r4, r3
    dec0:	e0042002 	and	r2, r4, r2
    dec4:	e3520000 	cmp	r2, #0
    dec8:	e1a05000 	mov	r5, r0
    decc:	e2800004 	add	r0, r0, #4
    ded0:	1a000003 	bne	dee4 <memchr+0xfc>
    ded4:	e24cc004 	sub	ip, ip, #4
    ded8:	e35c0003 	cmp	ip, #3
    dedc:	8afffff1 	bhi	dea8 <memchr+0xc0>
    dee0:	eaffffd8 	b	de48 <memchr+0x60>
    dee4:	e1a00005 	mov	r0, r5
    dee8:	eaffffd6 	b	de48 <memchr+0x60>
    deec:	e1a0c002 	mov	ip, r2
    def0:	eaffffd2 	b	de40 <memchr+0x58>
    def4:	e1a00002 	mov	r0, r2
    def8:	eaffffe4 	b	de90 <memchr+0xa8>
    defc:	fefefeff 	.word	0xfefefeff
    df00:	80808080 	.word	0x80808080

0000df04 <memcpy>:
    df04:	e352000f 	cmp	r2, #15
    df08:	e92d00f0 	push	{r4, r5, r6, r7}
    df0c:	9a00002a 	bls	dfbc <memcpy+0xb8>
    df10:	e1813000 	orr	r3, r1, r0
    df14:	e3130003 	tst	r3, #3
    df18:	1a000032 	bne	dfe8 <memcpy+0xe4>
    df1c:	e1a0c001 	mov	ip, r1
    df20:	e1a03000 	mov	r3, r0
    df24:	e1a04002 	mov	r4, r2
    df28:	e59c5000 	ldr	r5, [ip]
    df2c:	e5835000 	str	r5, [r3]
    df30:	e59c5004 	ldr	r5, [ip, #4]
    df34:	e5835004 	str	r5, [r3, #4]
    df38:	e59c5008 	ldr	r5, [ip, #8]
    df3c:	e5835008 	str	r5, [r3, #8]
    df40:	e2444010 	sub	r4, r4, #16
    df44:	e59c500c 	ldr	r5, [ip, #12]
    df48:	e354000f 	cmp	r4, #15
    df4c:	e583500c 	str	r5, [r3, #12]
    df50:	e28cc010 	add	ip, ip, #16
    df54:	e2833010 	add	r3, r3, #16
    df58:	8afffff2 	bhi	df28 <memcpy+0x24>
    df5c:	e2424010 	sub	r4, r2, #16
    df60:	e3c4400f 	bic	r4, r4, #15
    df64:	e202c00f 	and	ip, r2, #15
    df68:	e2844010 	add	r4, r4, #16
    df6c:	e35c0003 	cmp	ip, #3
    df70:	e0803004 	add	r3, r0, r4
    df74:	e0811004 	add	r1, r1, r4
    df78:	9a00001c 	bls	dff0 <memcpy+0xec>
    df7c:	e24cc004 	sub	ip, ip, #4
    df80:	e1a0712c 	lsr	r7, ip, #2
    df84:	e0816107 	add	r6, r1, r7, lsl #2
    df88:	e1a0c001 	mov	ip, r1
    df8c:	e2866004 	add	r6, r6, #4
    df90:	e1a04003 	mov	r4, r3
    df94:	e49c5004 	ldr	r5, [ip], #4
    df98:	e15c0006 	cmp	ip, r6
    df9c:	e4845004 	str	r5, [r4], #4
    dfa0:	1afffffb 	bne	df94 <memcpy+0x90>
    dfa4:	e287c001 	add	ip, r7, #1
    dfa8:	e1a0c10c 	lsl	ip, ip, #2
    dfac:	e2022003 	and	r2, r2, #3
    dfb0:	e081100c 	add	r1, r1, ip
    dfb4:	e083300c 	add	r3, r3, ip
    dfb8:	ea000000 	b	dfc0 <memcpy+0xbc>
    dfbc:	e1a03000 	mov	r3, r0
    dfc0:	e3520000 	cmp	r2, #0
    dfc4:	0a000005 	beq	dfe0 <memcpy+0xdc>
    dfc8:	e2411001 	sub	r1, r1, #1
    dfcc:	e0832002 	add	r2, r3, r2
    dfd0:	e5f1c001 	ldrb	ip, [r1, #1]!
    dfd4:	e4c3c001 	strb	ip, [r3], #1
    dfd8:	e1530002 	cmp	r3, r2
    dfdc:	1afffffb 	bne	dfd0 <memcpy+0xcc>
    dfe0:	e8bd00f0 	pop	{r4, r5, r6, r7}
    dfe4:	e12fff1e 	bx	lr
    dfe8:	e1a03000 	mov	r3, r0
    dfec:	eafffff5 	b	dfc8 <memcpy+0xc4>
    dff0:	e1a0200c 	mov	r2, ip
    dff4:	eafffff1 	b	dfc0 <memcpy+0xbc>

0000dff8 <__malloc_lock>:
    dff8:	e12fff1e 	bx	lr

0000dffc <__malloc_unlock>:
    dffc:	e12fff1e 	bx	lr

0000e000 <_Balloc>:
    e000:	e590304c 	ldr	r3, [r0, #76]	; 0x4c
    e004:	e3530000 	cmp	r3, #0
    e008:	e92d4070 	push	{r4, r5, r6, lr}
    e00c:	e1a05000 	mov	r5, r0
    e010:	e1a04001 	mov	r4, r1
    e014:	0a000009 	beq	e040 <_Balloc+0x40>
    e018:	e7930104 	ldr	r0, [r3, r4, lsl #2]
    e01c:	e3500000 	cmp	r0, #0
    e020:	0a00000f 	beq	e064 <_Balloc+0x64>
    e024:	e5902000 	ldr	r2, [r0]
    e028:	e7832104 	str	r2, [r3, r4, lsl #2]
    e02c:	e3a03000 	mov	r3, #0
    e030:	e5803010 	str	r3, [r0, #16]
    e034:	e580300c 	str	r3, [r0, #12]
    e038:	e8bd4070 	pop	{r4, r5, r6, lr}
    e03c:	e12fff1e 	bx	lr
    e040:	e3a01004 	mov	r1, #4
    e044:	e3a02021 	mov	r2, #33	; 0x21
    e048:	eb0008a8 	bl	102f0 <_calloc_r>
    e04c:	e3500000 	cmp	r0, #0
    e050:	e1a03000 	mov	r3, r0
    e054:	e585004c 	str	r0, [r5, #76]	; 0x4c
    e058:	1affffee 	bne	e018 <_Balloc+0x18>
    e05c:	e3a00000 	mov	r0, #0
    e060:	eafffff4 	b	e038 <_Balloc+0x38>
    e064:	e3a01001 	mov	r1, #1
    e068:	e1a06411 	lsl	r6, r1, r4
    e06c:	e2862005 	add	r2, r6, #5
    e070:	e1a00005 	mov	r0, r5
    e074:	e1a02102 	lsl	r2, r2, #2
    e078:	eb00089c 	bl	102f0 <_calloc_r>
    e07c:	e3500000 	cmp	r0, #0
    e080:	0afffff5 	beq	e05c <_Balloc+0x5c>
    e084:	e9800050 	stmib	r0, {r4, r6}
    e088:	eaffffe7 	b	e02c <_Balloc+0x2c>

0000e08c <_Bfree>:
    e08c:	e3510000 	cmp	r1, #0
    e090:	1590304c 	ldrne	r3, [r0, #76]	; 0x4c
    e094:	15912004 	ldrne	r2, [r1, #4]
    e098:	17930102 	ldrne	r0, [r3, r2, lsl #2]
    e09c:	15810000 	strne	r0, [r1]
    e0a0:	17831102 	strne	r1, [r3, r2, lsl #2]
    e0a4:	e12fff1e 	bx	lr

0000e0a8 <__multadd>:
    e0a8:	e92d45f0 	push	{r4, r5, r6, r7, r8, sl, lr}
    e0ac:	e5917010 	ldr	r7, [r1, #16]
    e0b0:	e24dd00c 	sub	sp, sp, #12
    e0b4:	e1a08001 	mov	r8, r1
    e0b8:	e1a0a000 	mov	sl, r0
    e0bc:	e2814014 	add	r4, r1, #20
    e0c0:	e3a0c000 	mov	ip, #0
    e0c4:	e5946000 	ldr	r6, [r4]
    e0c8:	e1a05806 	lsl	r5, r6, #16
    e0cc:	e1a05825 	lsr	r5, r5, #16
    e0d0:	e0253592 	mla	r5, r2, r5, r3
    e0d4:	e1a03826 	lsr	r3, r6, #16
    e0d8:	e0030392 	mul	r3, r2, r3
    e0dc:	e1a01805 	lsl	r1, r5, #16
    e0e0:	e0833825 	add	r3, r3, r5, lsr #16
    e0e4:	e1a01821 	lsr	r1, r1, #16
    e0e8:	e28cc001 	add	ip, ip, #1
    e0ec:	e0811803 	add	r1, r1, r3, lsl #16
    e0f0:	e157000c 	cmp	r7, ip
    e0f4:	e1a03823 	lsr	r3, r3, #16
    e0f8:	e4841004 	str	r1, [r4], #4
    e0fc:	cafffff0 	bgt	e0c4 <__multadd+0x1c>
    e100:	e3530000 	cmp	r3, #0
    e104:	0a000006 	beq	e124 <__multadd+0x7c>
    e108:	e5982008 	ldr	r2, [r8, #8]
    e10c:	e1570002 	cmp	r7, r2
    e110:	aa000007 	bge	e134 <__multadd+0x8c>
    e114:	e0882107 	add	r2, r8, r7, lsl #2
    e118:	e2877001 	add	r7, r7, #1
    e11c:	e5823014 	str	r3, [r2, #20]
    e120:	e5887010 	str	r7, [r8, #16]
    e124:	e1a00008 	mov	r0, r8
    e128:	e28dd00c 	add	sp, sp, #12
    e12c:	e8bd45f0 	pop	{r4, r5, r6, r7, r8, sl, lr}
    e130:	e12fff1e 	bx	lr
    e134:	e5981004 	ldr	r1, [r8, #4]
    e138:	e1a0000a 	mov	r0, sl
    e13c:	e2811001 	add	r1, r1, #1
    e140:	e58d3004 	str	r3, [sp, #4]
    e144:	ebffffad 	bl	e000 <_Balloc>
    e148:	e5982010 	ldr	r2, [r8, #16]
    e14c:	e2822002 	add	r2, r2, #2
    e150:	e288100c 	add	r1, r8, #12
    e154:	e1a04000 	mov	r4, r0
    e158:	e1a02102 	lsl	r2, r2, #2
    e15c:	e280000c 	add	r0, r0, #12
    e160:	ebffff67 	bl	df04 <memcpy>
    e164:	e5981004 	ldr	r1, [r8, #4]
    e168:	e59a204c 	ldr	r2, [sl, #76]	; 0x4c
    e16c:	e7920101 	ldr	r0, [r2, r1, lsl #2]
    e170:	e59d3004 	ldr	r3, [sp, #4]
    e174:	e5880000 	str	r0, [r8]
    e178:	e7828101 	str	r8, [r2, r1, lsl #2]
    e17c:	e1a08004 	mov	r8, r4
    e180:	eaffffe3 	b	e114 <__multadd+0x6c>

0000e184 <__s2b>:
    e184:	e92d45f8 	push	{r3, r4, r5, r6, r7, r8, sl, lr}
    e188:	e283c008 	add	ip, r3, #8
    e18c:	e1a07003 	mov	r7, r3
    e190:	e59f30d8 	ldr	r3, [pc, #216]	; e270 <__s2b+0xec>
    e194:	e0c3e39c 	smull	lr, r3, ip, r3
    e198:	e1a0cfcc 	asr	ip, ip, #31
    e19c:	e06c30c3 	rsb	r3, ip, r3, asr #1
    e1a0:	e3530001 	cmp	r3, #1
    e1a4:	e1a05000 	mov	r5, r0
    e1a8:	e1a04001 	mov	r4, r1
    e1ac:	e1a08002 	mov	r8, r2
    e1b0:	da00002c 	ble	e268 <__s2b+0xe4>
    e1b4:	e3a0c001 	mov	ip, #1
    e1b8:	e3a01000 	mov	r1, #0
    e1bc:	e1a0c08c 	lsl	ip, ip, #1
    e1c0:	e153000c 	cmp	r3, ip
    e1c4:	e2811001 	add	r1, r1, #1
    e1c8:	cafffffb 	bgt	e1bc <__s2b+0x38>
    e1cc:	e1a00005 	mov	r0, r5
    e1d0:	ebffff8a 	bl	e000 <_Balloc>
    e1d4:	e59d3020 	ldr	r3, [sp, #32]
    e1d8:	e3580009 	cmp	r8, #9
    e1dc:	e5803014 	str	r3, [r0, #20]
    e1e0:	e3a03001 	mov	r3, #1
    e1e4:	e1a01000 	mov	r1, r0
    e1e8:	e5803010 	str	r3, [r0, #16]
    e1ec:	d284400a 	addle	r4, r4, #10
    e1f0:	d3a08009 	movle	r8, #9
    e1f4:	da00000c 	ble	e22c <__s2b+0xa8>
    e1f8:	e284a009 	add	sl, r4, #9
    e1fc:	e1a0600a 	mov	r6, sl
    e200:	e0844008 	add	r4, r4, r8
    e204:	e4d63001 	ldrb	r3, [r6], #1
    e208:	e1a00005 	mov	r0, r5
    e20c:	e3a0200a 	mov	r2, #10
    e210:	e2433030 	sub	r3, r3, #48	; 0x30
    e214:	ebffffa3 	bl	e0a8 <__multadd>
    e218:	e1560004 	cmp	r6, r4
    e21c:	e1a01000 	mov	r1, r0
    e220:	1afffff7 	bne	e204 <__s2b+0x80>
    e224:	e08a4008 	add	r4, sl, r8
    e228:	e2444008 	sub	r4, r4, #8
    e22c:	e1570008 	cmp	r7, r8
    e230:	da000009 	ble	e25c <__s2b+0xd8>
    e234:	e0687007 	rsb	r7, r8, r7
    e238:	e0847007 	add	r7, r4, r7
    e23c:	e4d43001 	ldrb	r3, [r4], #1
    e240:	e1a00005 	mov	r0, r5
    e244:	e3a0200a 	mov	r2, #10
    e248:	e2433030 	sub	r3, r3, #48	; 0x30
    e24c:	ebffff95 	bl	e0a8 <__multadd>
    e250:	e1540007 	cmp	r4, r7
    e254:	e1a01000 	mov	r1, r0
    e258:	1afffff7 	bne	e23c <__s2b+0xb8>
    e25c:	e1a00001 	mov	r0, r1
    e260:	e8bd45f8 	pop	{r3, r4, r5, r6, r7, r8, sl, lr}
    e264:	e12fff1e 	bx	lr
    e268:	e3a01000 	mov	r1, #0
    e26c:	eaffffd6 	b	e1cc <__s2b+0x48>
    e270:	38e38e39 	.word	0x38e38e39

0000e274 <__hi0bits>:
    e274:	e1b02820 	lsrs	r2, r0, #16
    e278:	e1a03000 	mov	r3, r0
    e27c:	01a03800 	lsleq	r3, r0, #16
    e280:	13a00000 	movne	r0, #0
    e284:	03a00010 	moveq	r0, #16
    e288:	e31304ff 	tst	r3, #-16777216	; 0xff000000
    e28c:	01a03403 	lsleq	r3, r3, #8
    e290:	02800008 	addeq	r0, r0, #8
    e294:	e313020f 	tst	r3, #-268435456	; 0xf0000000
    e298:	01a03203 	lsleq	r3, r3, #4
    e29c:	02800004 	addeq	r0, r0, #4
    e2a0:	e3130103 	tst	r3, #-1073741824	; 0xc0000000
    e2a4:	01a03103 	lsleq	r3, r3, #2
    e2a8:	02800002 	addeq	r0, r0, #2
    e2ac:	e3530000 	cmp	r3, #0
    e2b0:	b12fff1e 	bxlt	lr
    e2b4:	e3130101 	tst	r3, #1073741824	; 0x40000000
    e2b8:	12800001 	addne	r0, r0, #1
    e2bc:	03a00020 	moveq	r0, #32
    e2c0:	e12fff1e 	bx	lr

0000e2c4 <__lo0bits>:
    e2c4:	e5903000 	ldr	r3, [r0]
    e2c8:	e1a02000 	mov	r2, r0
    e2cc:	e2130007 	ands	r0, r3, #7
    e2d0:	0a000009 	beq	e2fc <__lo0bits+0x38>
    e2d4:	e3130001 	tst	r3, #1
    e2d8:	1a00001c 	bne	e350 <__lo0bits+0x8c>
    e2dc:	e3130002 	tst	r3, #2
    e2e0:	11a030a3 	lsrne	r3, r3, #1
    e2e4:	01a03123 	lsreq	r3, r3, #2
    e2e8:	13a00001 	movne	r0, #1
    e2ec:	15823000 	strne	r3, [r2]
    e2f0:	05823000 	streq	r3, [r2]
    e2f4:	03a00002 	moveq	r0, #2
    e2f8:	e12fff1e 	bx	lr
    e2fc:	e1b01803 	lsls	r1, r3, #16
    e300:	01a03823 	lsreq	r3, r3, #16
    e304:	03a00010 	moveq	r0, #16
    e308:	e31300ff 	tst	r3, #255	; 0xff
    e30c:	01a03423 	lsreq	r3, r3, #8
    e310:	02800008 	addeq	r0, r0, #8
    e314:	e313000f 	tst	r3, #15
    e318:	01a03223 	lsreq	r3, r3, #4
    e31c:	02800004 	addeq	r0, r0, #4
    e320:	e3130003 	tst	r3, #3
    e324:	01a03123 	lsreq	r3, r3, #2
    e328:	02800002 	addeq	r0, r0, #2
    e32c:	e3130001 	tst	r3, #1
    e330:	1a000004 	bne	e348 <__lo0bits+0x84>
    e334:	e1b030a3 	lsrs	r3, r3, #1
    e338:	1a000001 	bne	e344 <__lo0bits+0x80>
    e33c:	e3a00020 	mov	r0, #32
    e340:	e12fff1e 	bx	lr
    e344:	e2800001 	add	r0, r0, #1
    e348:	e5823000 	str	r3, [r2]
    e34c:	e12fff1e 	bx	lr
    e350:	e3a00000 	mov	r0, #0
    e354:	e12fff1e 	bx	lr

0000e358 <__i2b>:
    e358:	e92d4010 	push	{r4, lr}
    e35c:	e1a04001 	mov	r4, r1
    e360:	e3a01001 	mov	r1, #1
    e364:	ebffff25 	bl	e000 <_Balloc>
    e368:	e3a02001 	mov	r2, #1
    e36c:	e5804014 	str	r4, [r0, #20]
    e370:	e5802010 	str	r2, [r0, #16]
    e374:	e8bd4010 	pop	{r4, lr}
    e378:	e12fff1e 	bx	lr

0000e37c <__multiply>:
    e37c:	e92d4ff0 	push	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
    e380:	e5918010 	ldr	r8, [r1, #16]
    e384:	e592a010 	ldr	sl, [r2, #16]
    e388:	e158000a 	cmp	r8, sl
    e38c:	e24dd014 	sub	sp, sp, #20
    e390:	e1a06001 	mov	r6, r1
    e394:	e1a04002 	mov	r4, r2
    e398:	aa000004 	bge	e3b0 <__multiply+0x34>
    e39c:	e1a02008 	mov	r2, r8
    e3a0:	e1a06004 	mov	r6, r4
    e3a4:	e1a0800a 	mov	r8, sl
    e3a8:	e1a04001 	mov	r4, r1
    e3ac:	e1a0a002 	mov	sl, r2
    e3b0:	e5963008 	ldr	r3, [r6, #8]
    e3b4:	e088500a 	add	r5, r8, sl
    e3b8:	e5961004 	ldr	r1, [r6, #4]
    e3bc:	e1550003 	cmp	r5, r3
    e3c0:	c2811001 	addgt	r1, r1, #1
    e3c4:	ebffff0d 	bl	e000 <_Balloc>
    e3c8:	e0807105 	add	r7, r0, r5, lsl #2
    e3cc:	e2803014 	add	r3, r0, #20
    e3d0:	e2877014 	add	r7, r7, #20
    e3d4:	e1530007 	cmp	r3, r7
    e3d8:	e58d0004 	str	r0, [sp, #4]
    e3dc:	33a00000 	movcc	r0, #0
    e3e0:	2a000002 	bcs	e3f0 <__multiply+0x74>
    e3e4:	e4830004 	str	r0, [r3], #4
    e3e8:	e1570003 	cmp	r7, r3
    e3ec:	8afffffc 	bhi	e3e4 <__multiply+0x68>
    e3f0:	e084a10a 	add	sl, r4, sl, lsl #2
    e3f4:	e28aa014 	add	sl, sl, #20
    e3f8:	e2844014 	add	r4, r4, #20
    e3fc:	e086c108 	add	ip, r6, r8, lsl #2
    e400:	e154000a 	cmp	r4, sl
    e404:	e2863014 	add	r3, r6, #20
    e408:	e28cc014 	add	ip, ip, #20
    e40c:	2a000041 	bcs	e518 <__multiply+0x19c>
    e410:	e59d2004 	ldr	r2, [sp, #4]
    e414:	e58d7008 	str	r7, [sp, #8]
    e418:	e58d500c 	str	r5, [sp, #12]
    e41c:	e2826018 	add	r6, r2, #24
    e420:	e1a0700a 	mov	r7, sl
    e424:	e1a05003 	mov	r5, r3
    e428:	e4948004 	ldr	r8, [r4], #4
    e42c:	e1a0a808 	lsl	sl, r8, #16
    e430:	e1b0a82a 	lsrs	sl, sl, #16
    e434:	0a000017 	beq	e498 <__multiply+0x11c>
    e438:	e1a02005 	mov	r2, r5
    e43c:	e2463004 	sub	r3, r6, #4
    e440:	e3a08000 	mov	r8, #0
    e444:	e4920004 	ldr	r0, [r2], #4
    e448:	e5931000 	ldr	r1, [r3]
    e44c:	e1a0b800 	lsl	fp, r0, #16
    e450:	e1a09801 	lsl	r9, r1, #16
    e454:	e1a0b82b 	lsr	fp, fp, #16
    e458:	e1a09829 	lsr	r9, r9, #16
    e45c:	e0299b9a 	mla	r9, sl, fp, r9
    e460:	e1a01821 	lsr	r1, r1, #16
    e464:	e1a00820 	lsr	r0, r0, #16
    e468:	e020109a 	mla	r0, sl, r0, r1
    e46c:	e0898008 	add	r8, r9, r8
    e470:	e1a01808 	lsl	r1, r8, #16
    e474:	e1a01821 	lsr	r1, r1, #16
    e478:	e0808828 	add	r8, r0, r8, lsr #16
    e47c:	e1811808 	orr	r1, r1, r8, lsl #16
    e480:	e15c0002 	cmp	ip, r2
    e484:	e1a08828 	lsr	r8, r8, #16
    e488:	e4831004 	str	r1, [r3], #4
    e48c:	8affffec 	bhi	e444 <__multiply+0xc8>
    e490:	e5838000 	str	r8, [r3]
    e494:	e5148004 	ldr	r8, [r4, #-4]
    e498:	e1b08828 	lsrs	r8, r8, #16
    e49c:	0a000018 	beq	e504 <__multiply+0x188>
    e4a0:	e516a004 	ldr	sl, [r6, #-4]
    e4a4:	e1a02005 	mov	r2, r5
    e4a8:	e1a03006 	mov	r3, r6
    e4ac:	e1a0100a 	mov	r1, sl
    e4b0:	e3a09000 	mov	r9, #0
    e4b4:	e1d200b0 	ldrh	r0, [r2]
    e4b8:	e1a01821 	lsr	r1, r1, #16
    e4bc:	e0211098 	mla	r1, r8, r0, r1
    e4c0:	e1a0a80a 	lsl	sl, sl, #16
    e4c4:	e0819009 	add	r9, r1, r9
    e4c8:	e1a0a82a 	lsr	sl, sl, #16
    e4cc:	e18aa809 	orr	sl, sl, r9, lsl #16
    e4d0:	e503a004 	str	sl, [r3, #-4]
    e4d4:	e1a0b003 	mov	fp, r3
    e4d8:	e4931004 	ldr	r1, [r3], #4
    e4dc:	e492a004 	ldr	sl, [r2], #4
    e4e0:	e1a00801 	lsl	r0, r1, #16
    e4e4:	e1a0a82a 	lsr	sl, sl, #16
    e4e8:	e1a00820 	lsr	r0, r0, #16
    e4ec:	e02a0a98 	mla	sl, r8, sl, r0
    e4f0:	e15c0002 	cmp	ip, r2
    e4f4:	e08aa829 	add	sl, sl, r9, lsr #16
    e4f8:	e1a0982a 	lsr	r9, sl, #16
    e4fc:	8affffec 	bhi	e4b4 <__multiply+0x138>
    e500:	e58ba000 	str	sl, [fp]
    e504:	e1570004 	cmp	r7, r4
    e508:	e2866004 	add	r6, r6, #4
    e50c:	8affffc5 	bhi	e428 <__multiply+0xac>
    e510:	e59d7008 	ldr	r7, [sp, #8]
    e514:	e59d500c 	ldr	r5, [sp, #12]
    e518:	e3550000 	cmp	r5, #0
    e51c:	da000009 	ble	e548 <__multiply+0x1cc>
    e520:	e5173004 	ldr	r3, [r7, #-4]
    e524:	e3530000 	cmp	r3, #0
    e528:	e2477004 	sub	r7, r7, #4
    e52c:	0a000003 	beq	e540 <__multiply+0x1c4>
    e530:	ea000004 	b	e548 <__multiply+0x1cc>
    e534:	e5373004 	ldr	r3, [r7, #-4]!
    e538:	e3530000 	cmp	r3, #0
    e53c:	1a000001 	bne	e548 <__multiply+0x1cc>
    e540:	e2555001 	subs	r5, r5, #1
    e544:	1afffffa 	bne	e534 <__multiply+0x1b8>
    e548:	e59d3004 	ldr	r3, [sp, #4]
    e54c:	e5835010 	str	r5, [r3, #16]
    e550:	e1a00003 	mov	r0, r3
    e554:	e28dd014 	add	sp, sp, #20
    e558:	e8bd4ff0 	pop	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
    e55c:	e12fff1e 	bx	lr

0000e560 <__pow5mult>:
    e560:	e2123003 	ands	r3, r2, #3
    e564:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
    e568:	e1a04002 	mov	r4, r2
    e56c:	e1a08000 	mov	r8, r0
    e570:	e1a07001 	mov	r7, r1
    e574:	1a000025 	bne	e610 <__pow5mult+0xb0>
    e578:	e1b04144 	asrs	r4, r4, #2
    e57c:	0a000018 	beq	e5e4 <__pow5mult+0x84>
    e580:	e5986048 	ldr	r6, [r8, #72]	; 0x48
    e584:	e3560000 	cmp	r6, #0
    e588:	1a000006 	bne	e5a8 <__pow5mult+0x48>
    e58c:	ea000026 	b	e62c <__pow5mult+0xcc>
    e590:	e1b040c4 	asrs	r4, r4, #1
    e594:	0a000012 	beq	e5e4 <__pow5mult+0x84>
    e598:	e5965000 	ldr	r5, [r6]
    e59c:	e3550000 	cmp	r5, #0
    e5a0:	11a06005 	movne	r6, r5
    e5a4:	0a000011 	beq	e5f0 <__pow5mult+0x90>
    e5a8:	e3140001 	tst	r4, #1
    e5ac:	0afffff7 	beq	e590 <__pow5mult+0x30>
    e5b0:	e1a01007 	mov	r1, r7
    e5b4:	e1a02006 	mov	r2, r6
    e5b8:	e1a00008 	mov	r0, r8
    e5bc:	ebffff6e 	bl	e37c <__multiply>
    e5c0:	e3570000 	cmp	r7, #0
    e5c4:	15972004 	ldrne	r2, [r7, #4]
    e5c8:	1598304c 	ldrne	r3, [r8, #76]	; 0x4c
    e5cc:	17931102 	ldrne	r1, [r3, r2, lsl #2]
    e5d0:	15871000 	strne	r1, [r7]
    e5d4:	17837102 	strne	r7, [r3, r2, lsl #2]
    e5d8:	e1b040c4 	asrs	r4, r4, #1
    e5dc:	e1a07000 	mov	r7, r0
    e5e0:	1affffec 	bne	e598 <__pow5mult+0x38>
    e5e4:	e1a00007 	mov	r0, r7
    e5e8:	e8bd41f0 	pop	{r4, r5, r6, r7, r8, lr}
    e5ec:	e12fff1e 	bx	lr
    e5f0:	e1a01006 	mov	r1, r6
    e5f4:	e1a02006 	mov	r2, r6
    e5f8:	e1a00008 	mov	r0, r8
    e5fc:	ebffff5e 	bl	e37c <__multiply>
    e600:	e5860000 	str	r0, [r6]
    e604:	e5805000 	str	r5, [r0]
    e608:	e1a06000 	mov	r6, r0
    e60c:	eaffffe5 	b	e5a8 <__pow5mult+0x48>
    e610:	e59f2034 	ldr	r2, [pc, #52]	; e64c <__pow5mult+0xec>
    e614:	e2433001 	sub	r3, r3, #1
    e618:	e7922103 	ldr	r2, [r2, r3, lsl #2]
    e61c:	e3a03000 	mov	r3, #0
    e620:	ebfffea0 	bl	e0a8 <__multadd>
    e624:	e1a07000 	mov	r7, r0
    e628:	eaffffd2 	b	e578 <__pow5mult+0x18>
    e62c:	e1a00008 	mov	r0, r8
    e630:	e59f1018 	ldr	r1, [pc, #24]	; e650 <__pow5mult+0xf0>
    e634:	ebffff47 	bl	e358 <__i2b>
    e638:	e3a03000 	mov	r3, #0
    e63c:	e5880048 	str	r0, [r8, #72]	; 0x48
    e640:	e1a06000 	mov	r6, r0
    e644:	e5803000 	str	r3, [r0]
    e648:	eaffffd6 	b	e5a8 <__pow5mult+0x48>
    e64c:	00012388 	.word	0x00012388
    e650:	00000271 	.word	0x00000271

0000e654 <__lshift>:
    e654:	e92d47f0 	push	{r4, r5, r6, r7, r8, r9, sl, lr}
    e658:	e591a010 	ldr	sl, [r1, #16]
    e65c:	e1a042c2 	asr	r4, r2, #5
    e660:	e5913008 	ldr	r3, [r1, #8]
    e664:	e084a00a 	add	sl, r4, sl
    e668:	e28a5001 	add	r5, sl, #1
    e66c:	e1550003 	cmp	r5, r3
    e670:	e1a07001 	mov	r7, r1
    e674:	e1a09002 	mov	r9, r2
    e678:	e1a08000 	mov	r8, r0
    e67c:	e5911004 	ldr	r1, [r1, #4]
    e680:	da000003 	ble	e694 <__lshift+0x40>
    e684:	e1a03083 	lsl	r3, r3, #1
    e688:	e1550003 	cmp	r5, r3
    e68c:	e2811001 	add	r1, r1, #1
    e690:	cafffffb 	bgt	e684 <__lshift+0x30>
    e694:	e1a00008 	mov	r0, r8
    e698:	ebfffe58 	bl	e000 <_Balloc>
    e69c:	e3540000 	cmp	r4, #0
    e6a0:	e280c014 	add	ip, r0, #20
    e6a4:	da000007 	ble	e6c8 <__lshift+0x74>
    e6a8:	e3a03000 	mov	r3, #0
    e6ac:	e1a06003 	mov	r6, r3
    e6b0:	e2833001 	add	r3, r3, #1
    e6b4:	e1530004 	cmp	r3, r4
    e6b8:	e48c6004 	str	r6, [ip], #4
    e6bc:	1afffffb 	bne	e6b0 <__lshift+0x5c>
    e6c0:	e0803103 	add	r3, r0, r3, lsl #2
    e6c4:	e283c014 	add	ip, r3, #20
    e6c8:	e5972010 	ldr	r2, [r7, #16]
    e6cc:	e219901f 	ands	r9, r9, #31
    e6d0:	e0872102 	add	r2, r7, r2, lsl #2
    e6d4:	e2874014 	add	r4, r7, #20
    e6d8:	e2822014 	add	r2, r2, #20
    e6dc:	0a000014 	beq	e734 <__lshift+0xe0>
    e6e0:	e2693020 	rsb	r3, r9, #32
    e6e4:	e3a01000 	mov	r1, #0
    e6e8:	e5946000 	ldr	r6, [r4]
    e6ec:	e1811916 	orr	r1, r1, r6, lsl r9
    e6f0:	e48c1004 	str	r1, [ip], #4
    e6f4:	e4941004 	ldr	r1, [r4], #4
    e6f8:	e1520004 	cmp	r2, r4
    e6fc:	e1a01331 	lsr	r1, r1, r3
    e700:	8afffff8 	bhi	e6e8 <__lshift+0x94>
    e704:	e3510000 	cmp	r1, #0
    e708:	e58c1000 	str	r1, [ip]
    e70c:	128a5002 	addne	r5, sl, #2
    e710:	e598304c 	ldr	r3, [r8, #76]	; 0x4c
    e714:	e5972004 	ldr	r2, [r7, #4]
    e718:	e7931102 	ldr	r1, [r3, r2, lsl #2]
    e71c:	e2455001 	sub	r5, r5, #1
    e720:	e5805010 	str	r5, [r0, #16]
    e724:	e5871000 	str	r1, [r7]
    e728:	e7837102 	str	r7, [r3, r2, lsl #2]
    e72c:	e8bd47f0 	pop	{r4, r5, r6, r7, r8, r9, sl, lr}
    e730:	e12fff1e 	bx	lr
    e734:	e4943004 	ldr	r3, [r4], #4
    e738:	e1520004 	cmp	r2, r4
    e73c:	e48c3004 	str	r3, [ip], #4
    e740:	9afffff2 	bls	e710 <__lshift+0xbc>
    e744:	e4943004 	ldr	r3, [r4], #4
    e748:	e1520004 	cmp	r2, r4
    e74c:	e48c3004 	str	r3, [ip], #4
    e750:	8afffff7 	bhi	e734 <__lshift+0xe0>
    e754:	eaffffed 	b	e710 <__lshift+0xbc>

0000e758 <__mcmp>:
    e758:	e52d4004 	push	{r4}		; (str r4, [sp, #-4]!)
    e75c:	e5913010 	ldr	r3, [r1, #16]
    e760:	e1a04000 	mov	r4, r0
    e764:	e5900010 	ldr	r0, [r0, #16]
    e768:	e0500003 	subs	r0, r0, r3
    e76c:	1a00000c 	bne	e7a4 <__mcmp+0x4c>
    e770:	e2833004 	add	r3, r3, #4
    e774:	e1a02103 	lsl	r2, r3, #2
    e778:	e0843002 	add	r3, r4, r2
    e77c:	e0811002 	add	r1, r1, r2
    e780:	e2844014 	add	r4, r4, #20
    e784:	e2833004 	add	r3, r3, #4
    e788:	e2811004 	add	r1, r1, #4
    e78c:	e5332004 	ldr	r2, [r3, #-4]!
    e790:	e531c004 	ldr	ip, [r1, #-4]!
    e794:	e152000c 	cmp	r2, ip
    e798:	1a000003 	bne	e7ac <__mcmp+0x54>
    e79c:	e1540003 	cmp	r4, r3
    e7a0:	3afffff9 	bcc	e78c <__mcmp+0x34>
    e7a4:	e8bd0010 	pop	{r4}
    e7a8:	e12fff1e 	bx	lr
    e7ac:	e15c0002 	cmp	ip, r2
    e7b0:	93a00001 	movls	r0, #1
    e7b4:	83e00000 	mvnhi	r0, #0
    e7b8:	eafffff9 	b	e7a4 <__mcmp+0x4c>

0000e7bc <__mdiff>:
    e7bc:	e92d47f0 	push	{r4, r5, r6, r7, r8, r9, sl, lr}
    e7c0:	e1a06001 	mov	r6, r1
    e7c4:	e1a05000 	mov	r5, r0
    e7c8:	e1a01002 	mov	r1, r2
    e7cc:	e1a00006 	mov	r0, r6
    e7d0:	e1a04002 	mov	r4, r2
    e7d4:	ebffffdf 	bl	e758 <__mcmp>
    e7d8:	e2507000 	subs	r7, r0, #0
    e7dc:	0a000042 	beq	e8ec <__mdiff+0x130>
    e7e0:	b1a03006 	movlt	r3, r6
    e7e4:	b1a06004 	movlt	r6, r4
    e7e8:	e5961004 	ldr	r1, [r6, #4]
    e7ec:	e1a00005 	mov	r0, r5
    e7f0:	b1a04003 	movlt	r4, r3
    e7f4:	a3a09000 	movge	r9, #0
    e7f8:	b3a09001 	movlt	r9, #1
    e7fc:	ebfffdff 	bl	e000 <_Balloc>
    e800:	e5967010 	ldr	r7, [r6, #16]
    e804:	e594a010 	ldr	sl, [r4, #16]
    e808:	e0868107 	add	r8, r6, r7, lsl #2
    e80c:	e084a10a 	add	sl, r4, sl, lsl #2
    e810:	e580900c 	str	r9, [r0, #12]
    e814:	e2888014 	add	r8, r8, #20
    e818:	e28aa014 	add	sl, sl, #20
    e81c:	e2866014 	add	r6, r6, #20
    e820:	e2842014 	add	r2, r4, #20
    e824:	e2801014 	add	r1, r0, #20
    e828:	e3a05000 	mov	r5, #0
    e82c:	e496c004 	ldr	ip, [r6], #4
    e830:	e4924004 	ldr	r4, [r2], #4
    e834:	e1a0380c 	lsl	r3, ip, #16
    e838:	e0855823 	add	r5, r5, r3, lsr #16
    e83c:	e1a09804 	lsl	r9, r4, #16
    e840:	e0453829 	sub	r3, r5, r9, lsr #16
    e844:	e1a05824 	lsr	r5, r4, #16
    e848:	e065582c 	rsb	r5, r5, ip, lsr #16
    e84c:	e1a04803 	lsl	r4, r3, #16
    e850:	e0855843 	add	r5, r5, r3, asr #16
    e854:	e1a04824 	lsr	r4, r4, #16
    e858:	e1a03001 	mov	r3, r1
    e85c:	e1844805 	orr	r4, r4, r5, lsl #16
    e860:	e4834004 	str	r4, [r3], #4
    e864:	e15a0002 	cmp	sl, r2
    e868:	e1a0c006 	mov	ip, r6
    e86c:	e1a05845 	asr	r5, r5, #16
    e870:	e1a01003 	mov	r1, r3
    e874:	8affffec 	bhi	e82c <__mdiff+0x70>
    e878:	e1580006 	cmp	r8, r6
    e87c:	9a000010 	bls	e8c4 <__mdiff+0x108>
    e880:	e49c2004 	ldr	r2, [ip], #4
    e884:	e1a04802 	lsl	r4, r2, #16
    e888:	e0855824 	add	r5, r5, r4, lsr #16
    e88c:	e1a04805 	lsl	r4, r5, #16
    e890:	e1a02822 	lsr	r2, r2, #16
    e894:	e0825845 	add	r5, r2, r5, asr #16
    e898:	e1a04824 	lsr	r4, r4, #16
    e89c:	e1844805 	orr	r4, r4, r5, lsl #16
    e8a0:	e158000c 	cmp	r8, ip
    e8a4:	e1a05845 	asr	r5, r5, #16
    e8a8:	e4834004 	str	r4, [r3], #4
    e8ac:	8afffff3 	bhi	e880 <__mdiff+0xc4>
    e8b0:	e1e03006 	mvn	r3, r6
    e8b4:	e0833008 	add	r3, r3, r8
    e8b8:	e3c33003 	bic	r3, r3, #3
    e8bc:	e2833004 	add	r3, r3, #4
    e8c0:	e0813003 	add	r3, r1, r3
    e8c4:	e3540000 	cmp	r4, #0
    e8c8:	e2433004 	sub	r3, r3, #4
    e8cc:	1a000003 	bne	e8e0 <__mdiff+0x124>
    e8d0:	e5332004 	ldr	r2, [r3, #-4]!
    e8d4:	e3520000 	cmp	r2, #0
    e8d8:	e2477001 	sub	r7, r7, #1
    e8dc:	0afffffb 	beq	e8d0 <__mdiff+0x114>
    e8e0:	e5807010 	str	r7, [r0, #16]
    e8e4:	e8bd47f0 	pop	{r4, r5, r6, r7, r8, r9, sl, lr}
    e8e8:	e12fff1e 	bx	lr
    e8ec:	e1a00005 	mov	r0, r5
    e8f0:	e1a01007 	mov	r1, r7
    e8f4:	ebfffdc1 	bl	e000 <_Balloc>
    e8f8:	e3a03001 	mov	r3, #1
    e8fc:	e5803010 	str	r3, [r0, #16]
    e900:	e5807014 	str	r7, [r0, #20]
    e904:	eafffff6 	b	e8e4 <__mdiff+0x128>

0000e908 <__ulp>:
    e908:	e59f3058 	ldr	r3, [pc, #88]	; e968 <__ulp+0x60>
    e90c:	e0013003 	and	r3, r1, r3
    e910:	e243350d 	sub	r3, r3, #54525952	; 0x3400000
    e914:	e3530000 	cmp	r3, #0
    e918:	da000002 	ble	e928 <__ulp+0x20>
    e91c:	e1a01003 	mov	r1, r3
    e920:	e3a00000 	mov	r0, #0
    e924:	e12fff1e 	bx	lr
    e928:	e2633000 	rsb	r3, r3, #0
    e92c:	e1a03a43 	asr	r3, r3, #20
    e930:	e3530013 	cmp	r3, #19
    e934:	da000007 	ble	e958 <__ulp+0x50>
    e938:	e3530032 	cmp	r3, #50	; 0x32
    e93c:	d2633033 	rsble	r3, r3, #51	; 0x33
    e940:	d3a02001 	movle	r2, #1
    e944:	d1a03312 	lslle	r3, r2, r3
    e948:	c3a03001 	movgt	r3, #1
    e94c:	e3a01000 	mov	r1, #0
    e950:	e1a00003 	mov	r0, r3
    e954:	e12fff1e 	bx	lr
    e958:	e3a02702 	mov	r2, #524288	; 0x80000
    e95c:	e1a01352 	asr	r1, r2, r3
    e960:	e3a00000 	mov	r0, #0
    e964:	e12fff1e 	bx	lr
    e968:	7ff00000 	.word	0x7ff00000

0000e96c <__b2d>:
    e96c:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
    e970:	e5906010 	ldr	r6, [r0, #16]
    e974:	e2866004 	add	r6, r6, #4
    e978:	e0806106 	add	r6, r0, r6, lsl #2
    e97c:	e1a04006 	mov	r4, r6
    e980:	e4945004 	ldr	r5, [r4], #4
    e984:	e1a03000 	mov	r3, r0
    e988:	e1a00005 	mov	r0, r5
    e98c:	e1a08001 	mov	r8, r1
    e990:	e2837014 	add	r7, r3, #20
    e994:	ebfffe36 	bl	e274 <__hi0bits>
    e998:	e2603020 	rsb	r3, r0, #32
    e99c:	e350000a 	cmp	r0, #10
    e9a0:	e1a0c006 	mov	ip, r6
    e9a4:	e5883000 	str	r3, [r8]
    e9a8:	ca00000d 	bgt	e9e4 <__b2d+0x78>
    e9ac:	e260100b 	rsb	r1, r0, #11
    e9b0:	e1a0c135 	lsr	ip, r5, r1
    e9b4:	e1570006 	cmp	r7, r6
    e9b8:	e38cc5ff 	orr	ip, ip, #1069547520	; 0x3fc00000
    e9bc:	e38c3603 	orr	r3, ip, #3145728	; 0x300000
    e9c0:	3514c008 	ldrcc	ip, [r4, #-8]
    e9c4:	31a0113c 	lsrcc	r1, ip, r1
    e9c8:	23a01000 	movcs	r1, #0
    e9cc:	e2800015 	add	r0, r0, #21
    e9d0:	e1812015 	orr	r2, r1, r5, lsl r0
    e9d4:	e1a00002 	mov	r0, r2
    e9d8:	e1a01003 	mov	r1, r3
    e9dc:	e8bd41f0 	pop	{r4, r5, r6, r7, r8, lr}
    e9e0:	e12fff1e 	bx	lr
    e9e4:	e1570006 	cmp	r7, r6
    e9e8:	3246c004 	subcc	ip, r6, #4
    e9ec:	35141008 	ldrcc	r1, [r4, #-8]
    e9f0:	23a01000 	movcs	r1, #0
    e9f4:	e250400b 	subs	r4, r0, #11
    e9f8:	0a00000a 	beq	ea28 <__b2d+0xbc>
    e9fc:	e1a05415 	lsl	r5, r5, r4
    ea00:	e15c0007 	cmp	ip, r7
    ea04:	e38555ff 	orr	r5, r5, #1069547520	; 0x3fc00000
    ea08:	851cc004 	ldrhi	ip, [ip, #-4]
    ea0c:	e260002b 	rsb	r0, r0, #43	; 0x2b
    ea10:	e3855603 	orr	r5, r5, #3145728	; 0x300000
    ea14:	e1853031 	orr	r3, r5, r1, lsr r0
    ea18:	81a0003c 	lsrhi	r0, ip, r0
    ea1c:	93a00000 	movls	r0, #0
    ea20:	e1802411 	orr	r2, r0, r1, lsl r4
    ea24:	eaffffea 	b	e9d4 <__b2d+0x68>
    ea28:	e38555ff 	orr	r5, r5, #1069547520	; 0x3fc00000
    ea2c:	e3853603 	orr	r3, r5, #3145728	; 0x300000
    ea30:	e1a02001 	mov	r2, r1
    ea34:	eaffffe6 	b	e9d4 <__b2d+0x68>

0000ea38 <__d2b>:
    ea38:	e92d45f0 	push	{r4, r5, r6, r7, r8, sl, lr}
    ea3c:	e3a01001 	mov	r1, #1
    ea40:	e24dd00c 	sub	sp, sp, #12
    ea44:	e1a04002 	mov	r4, r2
    ea48:	e1a05003 	mov	r5, r3
    ea4c:	e59d8028 	ldr	r8, [sp, #40]	; 0x28
    ea50:	e59d702c 	ldr	r7, [sp, #44]	; 0x2c
    ea54:	ebfffd69 	bl	e000 <_Balloc>
    ea58:	e3c53102 	bic	r3, r5, #-2147483648	; 0x80000000
    ea5c:	e3c524ff 	bic	r2, r5, #-16777216	; 0xff000000
    ea60:	e1b0aa23 	lsrs	sl, r3, #20
    ea64:	e3c2260f 	bic	r2, r2, #15728640	; 0xf00000
    ea68:	13822601 	orrne	r2, r2, #1048576	; 0x100000
    ea6c:	e3540000 	cmp	r4, #0
    ea70:	e1a06000 	mov	r6, r0
    ea74:	e58d2004 	str	r2, [sp, #4]
    ea78:	0a000020 	beq	eb00 <__d2b+0xc8>
    ea7c:	e28d0008 	add	r0, sp, #8
    ea80:	e5204008 	str	r4, [r0, #-8]!
    ea84:	e1a0000d 	mov	r0, sp
    ea88:	ebfffe0d 	bl	e2c4 <__lo0bits>
    ea8c:	e3500000 	cmp	r0, #0
    ea90:	e59d3004 	ldr	r3, [sp, #4]
    ea94:	159d2000 	ldrne	r2, [sp]
    ea98:	12601020 	rsbne	r1, r0, #32
    ea9c:	11822113 	orrne	r2, r2, r3, lsl r1
    eaa0:	11a03033 	lsrne	r3, r3, r0
    eaa4:	059d2000 	ldreq	r2, [sp]
    eaa8:	15862014 	strne	r2, [r6, #20]
    eaac:	158d3004 	strne	r3, [sp, #4]
    eab0:	05862014 	streq	r2, [r6, #20]
    eab4:	e3530000 	cmp	r3, #0
    eab8:	03a04001 	moveq	r4, #1
    eabc:	13a04002 	movne	r4, #2
    eac0:	e35a0000 	cmp	sl, #0
    eac4:	e5863018 	str	r3, [r6, #24]
    eac8:	e5864010 	str	r4, [r6, #16]
    eacc:	1a000014 	bne	eb24 <__d2b+0xec>
    ead0:	e2400e43 	sub	r0, r0, #1072	; 0x430
    ead4:	e2403002 	sub	r3, r0, #2
    ead8:	e0862104 	add	r2, r6, r4, lsl #2
    eadc:	e5920010 	ldr	r0, [r2, #16]
    eae0:	e5883000 	str	r3, [r8]
    eae4:	ebfffde2 	bl	e274 <__hi0bits>
    eae8:	e0600284 	rsb	r0, r0, r4, lsl #5
    eaec:	e5870000 	str	r0, [r7]
    eaf0:	e1a00006 	mov	r0, r6
    eaf4:	e28dd00c 	add	sp, sp, #12
    eaf8:	e8bd45f0 	pop	{r4, r5, r6, r7, r8, sl, lr}
    eafc:	e12fff1e 	bx	lr
    eb00:	e28d0004 	add	r0, sp, #4
    eb04:	ebfffdee 	bl	e2c4 <__lo0bits>
    eb08:	e59d3004 	ldr	r3, [sp, #4]
    eb0c:	e3a04001 	mov	r4, #1
    eb10:	e35a0000 	cmp	sl, #0
    eb14:	e5863014 	str	r3, [r6, #20]
    eb18:	e5864010 	str	r4, [r6, #16]
    eb1c:	e2800020 	add	r0, r0, #32
    eb20:	0affffea 	beq	ead0 <__d2b+0x98>
    eb24:	e24aae43 	sub	sl, sl, #1072	; 0x430
    eb28:	e24aa003 	sub	sl, sl, #3
    eb2c:	e08aa000 	add	sl, sl, r0
    eb30:	e2600035 	rsb	r0, r0, #53	; 0x35
    eb34:	e588a000 	str	sl, [r8]
    eb38:	e5870000 	str	r0, [r7]
    eb3c:	eaffffeb 	b	eaf0 <__d2b+0xb8>

0000eb40 <__ratio>:
    eb40:	e92d40f0 	push	{r4, r5, r6, r7, lr}
    eb44:	e24dd00c 	sub	sp, sp, #12
    eb48:	e1a06001 	mov	r6, r1
    eb4c:	e1a0100d 	mov	r1, sp
    eb50:	e1a07000 	mov	r7, r0
    eb54:	ebffff84 	bl	e96c <__b2d>
    eb58:	e1a05001 	mov	r5, r1
    eb5c:	e1a04000 	mov	r4, r0
    eb60:	e28d1004 	add	r1, sp, #4
    eb64:	e1a00006 	mov	r0, r6
    eb68:	ebffff7f 	bl	e96c <__b2d>
    eb6c:	e597c010 	ldr	ip, [r7, #16]
    eb70:	e1a03001 	mov	r3, r1
    eb74:	e1a02000 	mov	r2, r0
    eb78:	e59d1004 	ldr	r1, [sp, #4]
    eb7c:	e5960010 	ldr	r0, [r6, #16]
    eb80:	e59de000 	ldr	lr, [sp]
    eb84:	e060c00c 	rsb	ip, r0, ip
    eb88:	e061100e 	rsb	r1, r1, lr
    eb8c:	e081c28c 	add	ip, r1, ip, lsl #5
    eb90:	e35c0000 	cmp	ip, #0
    eb94:	e1a01005 	mov	r1, r5
    eb98:	c0851a0c 	addgt	r1, r5, ip, lsl #20
    eb9c:	e1a07003 	mov	r7, r3
    eba0:	c1a04004 	movgt	r4, r4
    eba4:	c1a05001 	movgt	r5, r1
    eba8:	d0437a0c 	suble	r7, r3, ip, lsl #20
    ebac:	e1a06002 	mov	r6, r2
    ebb0:	d1a03007 	movle	r3, r7
    ebb4:	d1a02002 	movle	r2, r2
    ebb8:	e1a00004 	mov	r0, r4
    ebbc:	e1a01005 	mov	r1, r5
    ebc0:	eb000cd1 	bl	11f0c <__aeabi_ddiv>
    ebc4:	e28dd00c 	add	sp, sp, #12
    ebc8:	e8bd40f0 	pop	{r4, r5, r6, r7, lr}
    ebcc:	e12fff1e 	bx	lr

0000ebd0 <_mprec_log10>:
    ebd0:	e3500017 	cmp	r0, #23
    ebd4:	e92d4010 	push	{r4, lr}
    ebd8:	e1a04000 	mov	r4, r0
    ebdc:	da000008 	ble	ec04 <_mprec_log10+0x34>
    ebe0:	e59f1030 	ldr	r1, [pc, #48]	; ec18 <_mprec_log10+0x48>
    ebe4:	e3a00000 	mov	r0, #0
    ebe8:	e3a02000 	mov	r2, #0
    ebec:	e59f3028 	ldr	r3, [pc, #40]	; ec1c <_mprec_log10+0x4c>
    ebf0:	eb000c21 	bl	11c7c <__aeabi_dmul>
    ebf4:	e2544001 	subs	r4, r4, #1
    ebf8:	1afffffa 	bne	ebe8 <_mprec_log10+0x18>
    ebfc:	e8bd4010 	pop	{r4, lr}
    ec00:	e12fff1e 	bx	lr
    ec04:	e59f3014 	ldr	r3, [pc, #20]	; ec20 <_mprec_log10+0x50>
    ec08:	e0834180 	add	r4, r3, r0, lsl #3
    ec0c:	e2841010 	add	r1, r4, #16
    ec10:	e8910003 	ldm	r1, {r0, r1}
    ec14:	eafffff8 	b	ebfc <_mprec_log10+0x2c>
    ec18:	3ff00000 	.word	0x3ff00000
    ec1c:	40240000 	.word	0x40240000
    ec20:	00012388 	.word	0x00012388

0000ec24 <__copybits>:
    ec24:	e92d0030 	push	{r4, r5}
    ec28:	e5924010 	ldr	r4, [r2, #16]
    ec2c:	e2411001 	sub	r1, r1, #1
    ec30:	e0824104 	add	r4, r2, r4, lsl #2
    ec34:	e1a052c1 	asr	r5, r1, #5
    ec38:	e2823014 	add	r3, r2, #20
    ec3c:	e2844014 	add	r4, r4, #20
    ec40:	e2855001 	add	r5, r5, #1
    ec44:	e1530004 	cmp	r3, r4
    ec48:	e0805105 	add	r5, r0, r5, lsl #2
    ec4c:	2a000009 	bcs	ec78 <__copybits+0x54>
    ec50:	e1a01000 	mov	r1, r0
    ec54:	e493c004 	ldr	ip, [r3], #4
    ec58:	e1540003 	cmp	r4, r3
    ec5c:	e481c004 	str	ip, [r1], #4
    ec60:	8afffffb 	bhi	ec54 <__copybits+0x30>
    ec64:	e0623004 	rsb	r3, r2, r4
    ec68:	e2433015 	sub	r3, r3, #21
    ec6c:	e3c33003 	bic	r3, r3, #3
    ec70:	e2833004 	add	r3, r3, #4
    ec74:	e0800003 	add	r0, r0, r3
    ec78:	e1550000 	cmp	r5, r0
    ec7c:	9a000003 	bls	ec90 <__copybits+0x6c>
    ec80:	e3a03000 	mov	r3, #0
    ec84:	e4803004 	str	r3, [r0], #4
    ec88:	e1550000 	cmp	r5, r0
    ec8c:	8afffffc 	bhi	ec84 <__copybits+0x60>
    ec90:	e8bd0030 	pop	{r4, r5}
    ec94:	e12fff1e 	bx	lr

0000ec98 <__any_on>:
    ec98:	e590c010 	ldr	ip, [r0, #16]
    ec9c:	e1a032c1 	asr	r3, r1, #5
    eca0:	e15c0003 	cmp	ip, r3
    eca4:	e52d4004 	push	{r4}		; (str r4, [sp, #-4]!)
    eca8:	e2802014 	add	r2, r0, #20
    ecac:	b1a0300c 	movlt	r3, ip
    ecb0:	ba000000 	blt	ecb8 <__any_on+0x20>
    ecb4:	ca00000f 	bgt	ecf8 <__any_on+0x60>
    ecb8:	e2833004 	add	r3, r3, #4
    ecbc:	e0800103 	add	r0, r0, r3, lsl #2
    ecc0:	e2803004 	add	r3, r0, #4
    ecc4:	e1520003 	cmp	r2, r3
    ecc8:	2a000012 	bcs	ed18 <__any_on+0x80>
    eccc:	e5133004 	ldr	r3, [r3, #-4]
    ecd0:	e3530000 	cmp	r3, #0
    ecd4:	1a000004 	bne	ecec <__any_on+0x54>
    ecd8:	e1520000 	cmp	r2, r0
    ecdc:	2a00000d 	bcs	ed18 <__any_on+0x80>
    ece0:	e5303004 	ldr	r3, [r0, #-4]!
    ece4:	e3530000 	cmp	r3, #0
    ece8:	0afffffa 	beq	ecd8 <__any_on+0x40>
    ecec:	e3a00001 	mov	r0, #1
    ecf0:	e8bd0010 	pop	{r4}
    ecf4:	e12fff1e 	bx	lr
    ecf8:	e211101f 	ands	r1, r1, #31
    ecfc:	0affffed 	beq	ecb8 <__any_on+0x20>
    ed00:	e080c103 	add	ip, r0, r3, lsl #2
    ed04:	e59cc014 	ldr	ip, [ip, #20]
    ed08:	e1a0413c 	lsr	r4, ip, r1
    ed0c:	e15c0114 	cmp	ip, r4, lsl r1
    ed10:	1afffff5 	bne	ecec <__any_on+0x54>
    ed14:	eaffffe7 	b	ecb8 <__any_on+0x20>
    ed18:	e3a00000 	mov	r0, #0
    ed1c:	eafffff3 	b	ecf0 <__any_on+0x58>

0000ed20 <_sbrk_r>:
    ed20:	e92d4038 	push	{r3, r4, r5, lr}
    ed24:	e59f4030 	ldr	r4, [pc, #48]	; ed5c <_sbrk_r+0x3c>
    ed28:	e3a03000 	mov	r3, #0
    ed2c:	e1a05000 	mov	r5, r0
    ed30:	e1a00001 	mov	r0, r1
    ed34:	e5843000 	str	r3, [r4]
    ed38:	ebffe7b3 	bl	8c0c <_sbrk>
    ed3c:	e3700001 	cmn	r0, #1
    ed40:	0a000001 	beq	ed4c <_sbrk_r+0x2c>
    ed44:	e8bd4038 	pop	{r3, r4, r5, lr}
    ed48:	e12fff1e 	bx	lr
    ed4c:	e5943000 	ldr	r3, [r4]
    ed50:	e3530000 	cmp	r3, #0
    ed54:	15853000 	strne	r3, [r5]
    ed58:	eafffff9 	b	ed44 <_sbrk_r+0x24>
    ed5c:	0001afcc 	.word	0x0001afcc

0000ed60 <__fpclassifyd>:
    ed60:	e1902001 	orrs	r2, r0, r1
    ed64:	1a000001 	bne	ed70 <__fpclassifyd+0x10>
    ed68:	e3a00002 	mov	r0, #2
    ed6c:	e12fff1e 	bx	lr
    ed70:	e2702001 	rsbs	r2, r0, #1
    ed74:	33a02000 	movcc	r2, #0
    ed78:	e3510102 	cmp	r1, #-2147483648	; 0x80000000
    ed7c:	03500000 	cmpeq	r0, #0
    ed80:	0afffff8 	beq	ed68 <__fpclassifyd+0x8>
    ed84:	e3c13102 	bic	r3, r1, #-2147483648	; 0x80000000
    ed88:	e59f1034 	ldr	r1, [pc, #52]	; edc4 <__fpclassifyd+0x64>
    ed8c:	e2430601 	sub	r0, r3, #1048576	; 0x100000
    ed90:	e1500001 	cmp	r0, r1
    ed94:	8a000001 	bhi	eda0 <__fpclassifyd+0x40>
    ed98:	e3a00004 	mov	r0, #4
    ed9c:	e12fff1e 	bx	lr
    eda0:	e3530601 	cmp	r3, #1048576	; 0x100000
    eda4:	2a000001 	bcs	edb0 <__fpclassifyd+0x50>
    eda8:	e3a00003 	mov	r0, #3
    edac:	e12fff1e 	bx	lr
    edb0:	e59f0010 	ldr	r0, [pc, #16]	; edc8 <__fpclassifyd+0x68>
    edb4:	e1530000 	cmp	r3, r0
    edb8:	13a00000 	movne	r0, #0
    edbc:	02020001 	andeq	r0, r2, #1
    edc0:	e12fff1e 	bx	lr
    edc4:	7fdfffff 	.word	0x7fdfffff
    edc8:	7ff00000 	.word	0x7ff00000

0000edcc <__sread>:
    edcc:	e92d4010 	push	{r4, lr}
    edd0:	e1a04001 	mov	r4, r1
    edd4:	e1d110fe 	ldrsh	r1, [r1, #14]
    edd8:	eb0007cc 	bl	10d10 <_read_r>
    eddc:	e3500000 	cmp	r0, #0
    ede0:	a5943050 	ldrge	r3, [r4, #80]	; 0x50
    ede4:	b1d430bc 	ldrhlt	r3, [r4, #12]
    ede8:	a0833000 	addge	r3, r3, r0
    edec:	b3c33a01 	biclt	r3, r3, #4096	; 0x1000
    edf0:	a5843050 	strge	r3, [r4, #80]	; 0x50
    edf4:	b1c430bc 	strhlt	r3, [r4, #12]
    edf8:	e8bd4010 	pop	{r4, lr}
    edfc:	e12fff1e 	bx	lr

0000ee00 <__seofread>:
    ee00:	e3a00000 	mov	r0, #0
    ee04:	e12fff1e 	bx	lr

0000ee08 <__swrite>:
    ee08:	e92d40f8 	push	{r3, r4, r5, r6, r7, lr}
    ee0c:	e1a04001 	mov	r4, r1
    ee10:	e1d110bc 	ldrh	r1, [r1, #12]
    ee14:	e3110c01 	tst	r1, #256	; 0x100
    ee18:	e1a07000 	mov	r7, r0
    ee1c:	e1a06002 	mov	r6, r2
    ee20:	e1a05003 	mov	r5, r3
    ee24:	0a000004 	beq	ee3c <__swrite+0x34>
    ee28:	e1d410fe 	ldrsh	r1, [r4, #14]
    ee2c:	e3a02000 	mov	r2, #0
    ee30:	e3a03002 	mov	r3, #2
    ee34:	eb000757 	bl	10b98 <_lseek_r>
    ee38:	e1d410bc 	ldrh	r1, [r4, #12]
    ee3c:	e3c11a01 	bic	r1, r1, #4096	; 0x1000
    ee40:	e1c410bc 	strh	r1, [r4, #12]
    ee44:	e1a00007 	mov	r0, r7
    ee48:	e1d410fe 	ldrsh	r1, [r4, #14]
    ee4c:	e1a02006 	mov	r2, r6
    ee50:	e1a03005 	mov	r3, r5
    ee54:	eb000514 	bl	102ac <_write_r>
    ee58:	e8bd40f8 	pop	{r3, r4, r5, r6, r7, lr}
    ee5c:	e12fff1e 	bx	lr

0000ee60 <__sseek>:
    ee60:	e92d4010 	push	{r4, lr}
    ee64:	e1a04001 	mov	r4, r1
    ee68:	e1d110fe 	ldrsh	r1, [r1, #14]
    ee6c:	eb000749 	bl	10b98 <_lseek_r>
    ee70:	e1d430bc 	ldrh	r3, [r4, #12]
    ee74:	e3700001 	cmn	r0, #1
    ee78:	03c33a01 	biceq	r3, r3, #4096	; 0x1000
    ee7c:	13833a01 	orrne	r3, r3, #4096	; 0x1000
    ee80:	01c430bc 	strheq	r3, [r4, #12]
    ee84:	11c430bc 	strhne	r3, [r4, #12]
    ee88:	15840050 	strne	r0, [r4, #80]	; 0x50
    ee8c:	e8bd4010 	pop	{r4, lr}
    ee90:	e12fff1e 	bx	lr

0000ee94 <__sclose>:
    ee94:	e92d4008 	push	{r3, lr}
    ee98:	e1d110fe 	ldrsh	r1, [r1, #14]
    ee9c:	eb000538 	bl	10384 <_close_r>
    eea0:	e8bd4008 	pop	{r3, lr}
    eea4:	e12fff1e 	bx	lr

0000eea8 <strcmp>:
    eea8:	e0202001 	eor	r2, r0, r1
    eeac:	e3120003 	tst	r2, #3
    eeb0:	1a000021 	bne	ef3c <strcmp_unaligned>
    eeb4:	e2102003 	ands	r2, r0, #3
    eeb8:	e3c00003 	bic	r0, r0, #3
    eebc:	e3c11003 	bic	r1, r1, #3
    eec0:	e490c004 	ldr	ip, [r0], #4
    eec4:	04913004 	ldreq	r3, [r1], #4
    eec8:	0a000006 	beq	eee8 <strcmp+0x40>
    eecc:	e2222003 	eor	r2, r2, #3
    eed0:	e1a02182 	lsl	r2, r2, #3
    eed4:	e3e034ff 	mvn	r3, #-16777216	; 0xff000000
    eed8:	e1a02233 	lsr	r2, r3, r2
    eedc:	e4913004 	ldr	r3, [r1], #4
    eee0:	e18cc002 	orr	ip, ip, r2
    eee4:	e1833002 	orr	r3, r3, r2
    eee8:	e52d4004 	push	{r4}		; (str r4, [sp, #-4]!)
    eeec:	e3a04001 	mov	r4, #1
    eef0:	e1844404 	orr	r4, r4, r4, lsl #8
    eef4:	e1844804 	orr	r4, r4, r4, lsl #16
    eef8:	e04c2004 	sub	r2, ip, r4
    eefc:	e15c0003 	cmp	ip, r3
    ef00:	01c2200c 	biceq	r2, r2, ip
    ef04:	01120384 	tsteq	r2, r4, lsl #7
    ef08:	0490c004 	ldreq	ip, [r0], #4
    ef0c:	04913004 	ldreq	r3, [r1], #4
    ef10:	0afffff8 	beq	eef8 <strcmp+0x50>
    ef14:	e1a00c0c 	lsl	r0, ip, #24
    ef18:	e1a0c42c 	lsr	ip, ip, #8
    ef1c:	e3500001 	cmp	r0, #1
    ef20:	21500c03 	cmpcs	r0, r3, lsl #24
    ef24:	01a03423 	lsreq	r3, r3, #8
    ef28:	0afffff9 	beq	ef14 <strcmp+0x6c>
    ef2c:	e20330ff 	and	r3, r3, #255	; 0xff
    ef30:	e0630c20 	rsb	r0, r3, r0, lsr #24
    ef34:	e49d4004 	pop	{r4}		; (ldr r4, [sp], #4)
    ef38:	e12fff1e 	bx	lr

0000ef3c <strcmp_unaligned>:
    ef3c:	e3100003 	tst	r0, #3
    ef40:	0a000006 	beq	ef60 <strcmp_unaligned+0x24>
    ef44:	e4d02001 	ldrb	r2, [r0], #1
    ef48:	e4d13001 	ldrb	r3, [r1], #1
    ef4c:	e3520001 	cmp	r2, #1
    ef50:	21520003 	cmpcs	r2, r3
    ef54:	0afffff8 	beq	ef3c <strcmp_unaligned>
    ef58:	e0420003 	sub	r0, r2, r3
    ef5c:	e12fff1e 	bx	lr
    ef60:	e52d5004 	push	{r5}		; (str r5, [sp, #-4]!)
    ef64:	e52d4004 	push	{r4}		; (str r4, [sp, #-4]!)
    ef68:	e3a02001 	mov	r2, #1
    ef6c:	e1822402 	orr	r2, r2, r2, lsl #8
    ef70:	e1822802 	orr	r2, r2, r2, lsl #16
    ef74:	e201c003 	and	ip, r1, #3
    ef78:	e3c11003 	bic	r1, r1, #3
    ef7c:	e4904004 	ldr	r4, [r0], #4
    ef80:	e4915004 	ldr	r5, [r1], #4
    ef84:	e35c0002 	cmp	ip, #2
    ef88:	0a000017 	beq	efec <strcmp_unaligned+0xb0>
    ef8c:	8a00002d 	bhi	f048 <strcmp_unaligned+0x10c>
    ef90:	e3c4c4ff 	bic	ip, r4, #-16777216	; 0xff000000
    ef94:	e15c0425 	cmp	ip, r5, lsr #8
    ef98:	e0443002 	sub	r3, r4, r2
    ef9c:	e1c33004 	bic	r3, r3, r4
    efa0:	1a000007 	bne	efc4 <strcmp_unaligned+0x88>
    efa4:	e0133382 	ands	r3, r3, r2, lsl #7
    efa8:	04915004 	ldreq	r5, [r1], #4
    efac:	1a000006 	bne	efcc <strcmp_unaligned+0x90>
    efb0:	e02cc004 	eor	ip, ip, r4
    efb4:	e15c0c05 	cmp	ip, r5, lsl #24
    efb8:	1a000008 	bne	efe0 <strcmp_unaligned+0xa4>
    efbc:	e4904004 	ldr	r4, [r0], #4
    efc0:	eafffff2 	b	ef90 <strcmp_unaligned+0x54>
    efc4:	e1a05425 	lsr	r5, r5, #8
    efc8:	ea000037 	b	f0ac <strcmp_unaligned+0x170>
    efcc:	e3d334ff 	bics	r3, r3, #-16777216	; 0xff000000
    efd0:	1a000031 	bne	f09c <strcmp_unaligned+0x160>
    efd4:	e5d15000 	ldrb	r5, [r1]
    efd8:	e1a0cc24 	lsr	ip, r4, #24
    efdc:	ea000032 	b	f0ac <strcmp_unaligned+0x170>
    efe0:	e1a0cc24 	lsr	ip, r4, #24
    efe4:	e20550ff 	and	r5, r5, #255	; 0xff
    efe8:	ea00002f 	b	f0ac <strcmp_unaligned+0x170>
    efec:	e1a0c804 	lsl	ip, r4, #16
    eff0:	e0443002 	sub	r3, r4, r2
    eff4:	e1a0c82c 	lsr	ip, ip, #16
    eff8:	e1c33004 	bic	r3, r3, r4
    effc:	e15c0825 	cmp	ip, r5, lsr #16
    f000:	1a00000e 	bne	f040 <strcmp_unaligned+0x104>
    f004:	e0133382 	ands	r3, r3, r2, lsl #7
    f008:	04915004 	ldreq	r5, [r1], #4
    f00c:	1a000004 	bne	f024 <strcmp_unaligned+0xe8>
    f010:	e02cc004 	eor	ip, ip, r4
    f014:	e15c0805 	cmp	ip, r5, lsl #16
    f018:	1a000006 	bne	f038 <strcmp_unaligned+0xfc>
    f01c:	e4904004 	ldr	r4, [r0], #4
    f020:	eafffff1 	b	efec <strcmp_unaligned+0xb0>
    f024:	e1b03803 	lsls	r3, r3, #16
    f028:	1a00001b 	bne	f09c <strcmp_unaligned+0x160>
    f02c:	e1d150b0 	ldrh	r5, [r1]
    f030:	e1a0c824 	lsr	ip, r4, #16
    f034:	ea00001c 	b	f0ac <strcmp_unaligned+0x170>
    f038:	e1a05805 	lsl	r5, r5, #16
    f03c:	e1a0c824 	lsr	ip, r4, #16
    f040:	e1a05825 	lsr	r5, r5, #16
    f044:	ea000018 	b	f0ac <strcmp_unaligned+0x170>
    f048:	e204c0ff 	and	ip, r4, #255	; 0xff
    f04c:	e15c0c25 	cmp	ip, r5, lsr #24
    f050:	e0443002 	sub	r3, r4, r2
    f054:	e1c33004 	bic	r3, r3, r4
    f058:	1a000007 	bne	f07c <strcmp_unaligned+0x140>
    f05c:	e0133382 	ands	r3, r3, r2, lsl #7
    f060:	04915004 	ldreq	r5, [r1], #4
    f064:	1a000006 	bne	f084 <strcmp_unaligned+0x148>
    f068:	e02cc004 	eor	ip, ip, r4
    f06c:	e15c0405 	cmp	ip, r5, lsl #8
    f070:	1a000006 	bne	f090 <strcmp_unaligned+0x154>
    f074:	e4904004 	ldr	r4, [r0], #4
    f078:	eafffff2 	b	f048 <strcmp_unaligned+0x10c>
    f07c:	e1a05c25 	lsr	r5, r5, #24
    f080:	ea000009 	b	f0ac <strcmp_unaligned+0x170>
    f084:	e31400ff 	tst	r4, #255	; 0xff
    f088:	0a000003 	beq	f09c <strcmp_unaligned+0x160>
    f08c:	e4915004 	ldr	r5, [r1], #4
    f090:	e1a0c424 	lsr	ip, r4, #8
    f094:	e3c554ff 	bic	r5, r5, #-16777216	; 0xff000000
    f098:	ea000003 	b	f0ac <strcmp_unaligned+0x170>
    f09c:	e3a00000 	mov	r0, #0
    f0a0:	e49d4004 	pop	{r4}		; (ldr r4, [sp], #4)
    f0a4:	e49d5004 	pop	{r5}		; (ldr r5, [sp], #4)
    f0a8:	e12fff1e 	bx	lr
    f0ac:	e20c20ff 	and	r2, ip, #255	; 0xff
    f0b0:	e20500ff 	and	r0, r5, #255	; 0xff
    f0b4:	e3500001 	cmp	r0, #1
    f0b8:	21500002 	cmpcs	r0, r2
    f0bc:	01a0c42c 	lsreq	ip, ip, #8
    f0c0:	01a05425 	lsreq	r5, r5, #8
    f0c4:	0afffff8 	beq	f0ac <strcmp_unaligned+0x170>
    f0c8:	e0420000 	sub	r0, r2, r0
    f0cc:	e49d4004 	pop	{r4}		; (ldr r4, [sp], #4)
    f0d0:	e49d5004 	pop	{r5}		; (ldr r5, [sp], #4)
    f0d4:	e12fff1e 	bx	lr

0000f0d8 <strlen>:
    f0d8:	e3c01003 	bic	r1, r0, #3
    f0dc:	e2100003 	ands	r0, r0, #3
    f0e0:	e2600000 	rsb	r0, r0, #0
    f0e4:	e4913004 	ldr	r3, [r1], #4
    f0e8:	e280c004 	add	ip, r0, #4
    f0ec:	e1a0c18c 	lsl	ip, ip, #3
    f0f0:	e3e02000 	mvn	r2, #0
    f0f4:	11833c32 	orrne	r3, r3, r2, lsr ip
    f0f8:	e3a0c001 	mov	ip, #1
    f0fc:	e18cc40c 	orr	ip, ip, ip, lsl #8
    f100:	e18cc80c 	orr	ip, ip, ip, lsl #16
    f104:	e043200c 	sub	r2, r3, ip
    f108:	e1c22003 	bic	r2, r2, r3
    f10c:	e012238c 	ands	r2, r2, ip, lsl #7
    f110:	04913004 	ldreq	r3, [r1], #4
    f114:	02800004 	addeq	r0, r0, #4
    f118:	0afffff9 	beq	f104 <strlen+0x2c>
    f11c:	e31300ff 	tst	r3, #255	; 0xff
    f120:	12800001 	addne	r0, r0, #1
    f124:	13130cff 	tstne	r3, #65280	; 0xff00
    f128:	12800001 	addne	r0, r0, #1
    f12c:	131308ff 	tstne	r3, #16711680	; 0xff0000
    f130:	12800001 	addne	r0, r0, #1
    f134:	e12fff1e 	bx	lr

0000f138 <__sprint_r>:
    f138:	e5923008 	ldr	r3, [r2, #8]
    f13c:	e3530000 	cmp	r3, #0
    f140:	e92d47f0 	push	{r4, r5, r6, r7, r8, r9, sl, lr}
    f144:	e1a07000 	mov	r7, r0
    f148:	e1a0a002 	mov	sl, r2
    f14c:	e1a06001 	mov	r6, r1
    f150:	05823004 	streq	r3, [r2, #4]
    f154:	01a00003 	moveq	r0, r3
    f158:	0a000017 	beq	f1bc <__sprint_r+0x84>
    f15c:	e591c064 	ldr	ip, [r1, #100]	; 0x64
    f160:	e31c0a02 	tst	ip, #8192	; 0x2000
    f164:	0a00001e 	beq	f1e4 <__sprint_r+0xac>
    f168:	e5929000 	ldr	r9, [r2]
    f16c:	e2899008 	add	r9, r9, #8
    f170:	e5198004 	ldr	r8, [r9, #-4]
    f174:	e5195008 	ldr	r5, [r9, #-8]
    f178:	e1b08128 	lsrs	r8, r8, #2
    f17c:	12455004 	subne	r5, r5, #4
    f180:	13a04000 	movne	r4, #0
    f184:	1a000002 	bne	f194 <__sprint_r+0x5c>
    f188:	ea00000e 	b	f1c8 <__sprint_r+0x90>
    f18c:	e1580004 	cmp	r8, r4
    f190:	0a00000b 	beq	f1c4 <__sprint_r+0x8c>
    f194:	e1a00007 	mov	r0, r7
    f198:	e5b51004 	ldr	r1, [r5, #4]!
    f19c:	e1a02006 	mov	r2, r6
    f1a0:	eb0004c6 	bl	104c0 <_fputwc_r>
    f1a4:	e3700001 	cmn	r0, #1
    f1a8:	e2844001 	add	r4, r4, #1
    f1ac:	1afffff6 	bne	f18c <__sprint_r+0x54>
    f1b0:	e3a03000 	mov	r3, #0
    f1b4:	e58a3008 	str	r3, [sl, #8]
    f1b8:	e58a3004 	str	r3, [sl, #4]
    f1bc:	e8bd47f0 	pop	{r4, r5, r6, r7, r8, r9, sl, lr}
    f1c0:	e12fff1e 	bx	lr
    f1c4:	e59a3008 	ldr	r3, [sl, #8]
    f1c8:	e0433108 	sub	r3, r3, r8, lsl #2
    f1cc:	e3530000 	cmp	r3, #0
    f1d0:	e58a3008 	str	r3, [sl, #8]
    f1d4:	e2899008 	add	r9, r9, #8
    f1d8:	1affffe4 	bne	f170 <__sprint_r+0x38>
    f1dc:	e1a00003 	mov	r0, r3
    f1e0:	eafffff2 	b	f1b0 <__sprint_r+0x78>
    f1e4:	eb00052c 	bl	1069c <__sfvwrite_r>
    f1e8:	e3a03000 	mov	r3, #0
    f1ec:	e58a3008 	str	r3, [sl, #8]
    f1f0:	e58a3004 	str	r3, [sl, #4]
    f1f4:	eafffff0 	b	f1bc <__sprint_r+0x84>

0000f1f8 <_vfiprintf_r>:
    f1f8:	e92d4ff0 	push	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
    f1fc:	e24dde4e 	sub	sp, sp, #1248	; 0x4e0
    f200:	e24dd00c 	sub	sp, sp, #12
    f204:	e250a000 	subs	sl, r0, #0
    f208:	e1a07001 	mov	r7, r1
    f20c:	e1a04002 	mov	r4, r2
    f210:	e58d3018 	str	r3, [sp, #24]
    f214:	0a000002 	beq	f224 <_vfiprintf_r+0x2c>
    f218:	e59a3038 	ldr	r3, [sl, #56]	; 0x38
    f21c:	e3530000 	cmp	r3, #0
    f220:	0a00036c 	beq	ffd8 <_vfiprintf_r+0xde0>
    f224:	e1d780bc 	ldrh	r8, [r7, #12]
    f228:	e3180a02 	tst	r8, #8192	; 0x2000
    f22c:	05973064 	ldreq	r3, [r7, #100]	; 0x64
    f230:	03888a02 	orreq	r8, r8, #8192	; 0x2000
    f234:	03c33a02 	biceq	r3, r3, #8192	; 0x2000
    f238:	01c780bc 	strheq	r8, [r7, #12]
    f23c:	05873064 	streq	r3, [r7, #100]	; 0x64
    f240:	e3180008 	tst	r8, #8
    f244:	0a00030c 	beq	fe7c <_vfiprintf_r+0xc84>
    f248:	e5973010 	ldr	r3, [r7, #16]
    f24c:	e3530000 	cmp	r3, #0
    f250:	0a000309 	beq	fe7c <_vfiprintf_r+0xc84>
    f254:	e208301a 	and	r3, r8, #26
    f258:	e353000a 	cmp	r3, #10
    f25c:	0a00030f 	beq	fea0 <_vfiprintf_r+0xca8>
    f260:	e3a03000 	mov	r3, #0
    f264:	e28d6080 	add	r6, sp, #128	; 0x80
    f268:	e28dc07f 	add	ip, sp, #127	; 0x7f
    f26c:	e28d00e8 	add	r0, sp, #232	; 0xe8
    f270:	e58d3034 	str	r3, [sp, #52]	; 0x34
    f274:	e58d3054 	str	r3, [sp, #84]	; 0x54
    f278:	e58d3050 	str	r3, [sp, #80]	; 0x50
    f27c:	e58d3010 	str	r3, [sp, #16]
    f280:	e06c3006 	rsb	r3, ip, r6
    f284:	e58d602c 	str	r6, [sp, #44]	; 0x2c
    f288:	e58dc030 	str	ip, [sp, #48]	; 0x30
    f28c:	e58d004c 	str	r0, [sp, #76]	; 0x4c
    f290:	e58d303c 	str	r3, [sp, #60]	; 0x3c
    f294:	e58da020 	str	sl, [sp, #32]
    f298:	e1a0b004 	mov	fp, r4
    f29c:	e1a08000 	mov	r8, r0
    f2a0:	e5db3000 	ldrb	r3, [fp]
    f2a4:	e3530025 	cmp	r3, #37	; 0x25
    f2a8:	13530000 	cmpne	r3, #0
    f2ac:	0a00027e 	beq	fcac <_vfiprintf_r+0xab4>
    f2b0:	e28b3001 	add	r3, fp, #1
    f2b4:	e1a04003 	mov	r4, r3
    f2b8:	e5d33000 	ldrb	r3, [r3]
    f2bc:	e3530025 	cmp	r3, #37	; 0x25
    f2c0:	13530000 	cmpne	r3, #0
    f2c4:	e2843001 	add	r3, r4, #1
    f2c8:	1afffff9 	bne	f2b4 <_vfiprintf_r+0xbc>
    f2cc:	e054500b 	subs	r5, r4, fp
    f2d0:	0a00000d 	beq	f30c <_vfiprintf_r+0x114>
    f2d4:	e59d2050 	ldr	r2, [sp, #80]	; 0x50
    f2d8:	e59d1054 	ldr	r1, [sp, #84]	; 0x54
    f2dc:	e2822001 	add	r2, r2, #1
    f2e0:	e0811005 	add	r1, r1, r5
    f2e4:	e3520007 	cmp	r2, #7
    f2e8:	e588b000 	str	fp, [r8]
    f2ec:	e5885004 	str	r5, [r8, #4]
    f2f0:	e58d1054 	str	r1, [sp, #84]	; 0x54
    f2f4:	e58d2050 	str	r2, [sp, #80]	; 0x50
    f2f8:	d2888008 	addle	r8, r8, #8
    f2fc:	ca000317 	bgt	ff60 <_vfiprintf_r+0xd68>
    f300:	e59dc010 	ldr	ip, [sp, #16]
    f304:	e08cc005 	add	ip, ip, r5
    f308:	e58dc010 	str	ip, [sp, #16]
    f30c:	e5d43000 	ldrb	r3, [r4]
    f310:	e3530000 	cmp	r3, #0
    f314:	0a000271 	beq	fce0 <_vfiprintf_r+0xae8>
    f318:	e3a03000 	mov	r3, #0
    f31c:	e5cd3047 	strb	r3, [sp, #71]	; 0x47
    f320:	e58d301c 	str	r3, [sp, #28]
    f324:	e58d3004 	str	r3, [sp, #4]
    f328:	e284b001 	add	fp, r4, #1
    f32c:	e1a01003 	mov	r1, r3
    f330:	e3e04000 	mvn	r4, #0
    f334:	e4db2001 	ldrb	r2, [fp], #1
    f338:	e2423020 	sub	r3, r2, #32
    f33c:	e3530058 	cmp	r3, #88	; 0x58
    f340:	979ff103 	ldrls	pc, [pc, r3, lsl #2]
    f344:	ea00012f 	b	f808 <_vfiprintf_r+0x610>
    f348:	0000f958 	.word	0x0000f958
    f34c:	0000f808 	.word	0x0000f808
    f350:	0000f808 	.word	0x0000f808
    f354:	0000f948 	.word	0x0000f948
    f358:	0000f808 	.word	0x0000f808
    f35c:	0000f808 	.word	0x0000f808
    f360:	0000f808 	.word	0x0000f808
    f364:	0000f808 	.word	0x0000f808
    f368:	0000f808 	.word	0x0000f808
    f36c:	0000f808 	.word	0x0000f808
    f370:	0000f5b4 	.word	0x0000f5b4
    f374:	0000f940 	.word	0x0000f940
    f378:	0000f808 	.word	0x0000f808
    f37c:	0000f5dc 	.word	0x0000f5dc
    f380:	0000f9a4 	.word	0x0000f9a4
    f384:	0000f808 	.word	0x0000f808
    f388:	0000f994 	.word	0x0000f994
    f38c:	0000f964 	.word	0x0000f964
    f390:	0000f964 	.word	0x0000f964
    f394:	0000f964 	.word	0x0000f964
    f398:	0000f964 	.word	0x0000f964
    f39c:	0000f964 	.word	0x0000f964
    f3a0:	0000f964 	.word	0x0000f964
    f3a4:	0000f964 	.word	0x0000f964
    f3a8:	0000f964 	.word	0x0000f964
    f3ac:	0000f964 	.word	0x0000f964
    f3b0:	0000f808 	.word	0x0000f808
    f3b4:	0000f808 	.word	0x0000f808
    f3b8:	0000f808 	.word	0x0000f808
    f3bc:	0000f808 	.word	0x0000f808
    f3c0:	0000f808 	.word	0x0000f808
    f3c4:	0000f808 	.word	0x0000f808
    f3c8:	0000f808 	.word	0x0000f808
    f3cc:	0000f808 	.word	0x0000f808
    f3d0:	0000f808 	.word	0x0000f808
    f3d4:	0000f808 	.word	0x0000f808
    f3d8:	0000f92c 	.word	0x0000f92c
    f3dc:	0000f808 	.word	0x0000f808
    f3e0:	0000f808 	.word	0x0000f808
    f3e4:	0000f808 	.word	0x0000f808
    f3e8:	0000f808 	.word	0x0000f808
    f3ec:	0000f808 	.word	0x0000f808
    f3f0:	0000f808 	.word	0x0000f808
    f3f4:	0000f808 	.word	0x0000f808
    f3f8:	0000f808 	.word	0x0000f808
    f3fc:	0000f808 	.word	0x0000f808
    f400:	0000f808 	.word	0x0000f808
    f404:	0000f57c 	.word	0x0000f57c
    f408:	0000f808 	.word	0x0000f808
    f40c:	0000f808 	.word	0x0000f808
    f410:	0000f808 	.word	0x0000f808
    f414:	0000f808 	.word	0x0000f808
    f418:	0000f808 	.word	0x0000f808
    f41c:	0000f4ac 	.word	0x0000f4ac
    f420:	0000f808 	.word	0x0000f808
    f424:	0000f808 	.word	0x0000f808
    f428:	0000f7b8 	.word	0x0000f7b8
    f42c:	0000f808 	.word	0x0000f808
    f430:	0000f808 	.word	0x0000f808
    f434:	0000f808 	.word	0x0000f808
    f438:	0000f808 	.word	0x0000f808
    f43c:	0000f808 	.word	0x0000f808
    f440:	0000f808 	.word	0x0000f808
    f444:	0000f808 	.word	0x0000f808
    f448:	0000f808 	.word	0x0000f808
    f44c:	0000f808 	.word	0x0000f808
    f450:	0000f808 	.word	0x0000f808
    f454:	0000f654 	.word	0x0000f654
    f458:	0000f60c 	.word	0x0000f60c
    f45c:	0000f808 	.word	0x0000f808
    f460:	0000f808 	.word	0x0000f808
    f464:	0000f808 	.word	0x0000f808
    f468:	0000f5fc 	.word	0x0000f5fc
    f46c:	0000f60c 	.word	0x0000f60c
    f470:	0000f808 	.word	0x0000f808
    f474:	0000f808 	.word	0x0000f808
    f478:	0000f5ec 	.word	0x0000f5ec
    f47c:	0000f808 	.word	0x0000f808
    f480:	0000f8f4 	.word	0x0000f8f4
    f484:	0000f588 	.word	0x0000f588
    f488:	0000f8ac 	.word	0x0000f8ac
    f48c:	0000f89c 	.word	0x0000f89c
    f490:	0000f808 	.word	0x0000f808
    f494:	0000f834 	.word	0x0000f834
    f498:	0000f808 	.word	0x0000f808
    f49c:	0000f4b8 	.word	0x0000f4b8
    f4a0:	0000f808 	.word	0x0000f808
    f4a4:	0000f808 	.word	0x0000f808
    f4a8:	0000f9ec 	.word	0x0000f9ec
    f4ac:	e59d5004 	ldr	r5, [sp, #4]
    f4b0:	e3855010 	orr	r5, r5, #16
    f4b4:	e58d5004 	str	r5, [sp, #4]
    f4b8:	e59d6004 	ldr	r6, [sp, #4]
    f4bc:	e3160010 	tst	r6, #16
    f4c0:	0a0002e4 	beq	10058 <_vfiprintf_r+0xe60>
    f4c4:	e59dc018 	ldr	ip, [sp, #24]
    f4c8:	e59c3000 	ldr	r3, [ip]
    f4cc:	e28cc004 	add	ip, ip, #4
    f4d0:	e2932000 	adds	r2, r3, #0
    f4d4:	e58dc018 	str	ip, [sp, #24]
    f4d8:	e3a00001 	mov	r0, #1
    f4dc:	13a02001 	movne	r2, #1
    f4e0:	e3a01000 	mov	r1, #0
    f4e4:	e5cd1047 	strb	r1, [sp, #71]	; 0x47
    f4e8:	e3540000 	cmp	r4, #0
    f4ec:	a59d5004 	ldrge	r5, [sp, #4]
    f4f0:	a3c55080 	bicge	r5, r5, #128	; 0x80
    f4f4:	a58d5004 	strge	r5, [sp, #4]
    f4f8:	e3540000 	cmp	r4, #0
    f4fc:	13822001 	orrne	r2, r2, #1
    f500:	e21220ff 	ands	r2, r2, #255	; 0xff
    f504:	0a0001ea 	beq	fcb4 <_vfiprintf_r+0xabc>
    f508:	e3500001 	cmp	r0, #1
    f50c:	0a0002b3 	beq	ffe0 <_vfiprintf_r+0xde8>
    f510:	e3500002 	cmp	r0, #2
    f514:	e59d2030 	ldr	r2, [sp, #48]	; 0x30
    f518:	0a0002c2 	beq	10028 <_vfiprintf_r+0xe30>
    f51c:	e2030007 	and	r0, r3, #7
    f520:	e2800030 	add	r0, r0, #48	; 0x30
    f524:	e1b031a3 	lsrs	r3, r3, #3
    f528:	e1a05002 	mov	r5, r2
    f52c:	e5c20000 	strb	r0, [r2]
    f530:	e2422001 	sub	r2, r2, #1
    f534:	1afffff8 	bne	f51c <_vfiprintf_r+0x324>
    f538:	e59d6004 	ldr	r6, [sp, #4]
    f53c:	e3160001 	tst	r6, #1
    f540:	e1a03005 	mov	r3, r5
    f544:	1a0002f4 	bne	1011c <_vfiprintf_r+0xf24>
    f548:	e59dc02c 	ldr	ip, [sp, #44]	; 0x2c
    f54c:	e065c00c 	rsb	ip, r5, ip
    f550:	e58dc00c 	str	ip, [sp, #12]
    f554:	e58d4014 	str	r4, [sp, #20]
    f558:	e59d300c 	ldr	r3, [sp, #12]
    f55c:	e59d6014 	ldr	r6, [sp, #20]
    f560:	e1530006 	cmp	r3, r6
    f564:	b1a03006 	movlt	r3, r6
    f568:	e3510000 	cmp	r1, #0
    f56c:	e58d3008 	str	r3, [sp, #8]
    f570:	12833001 	addne	r3, r3, #1
    f574:	158d3008 	strne	r3, [sp, #8]
    f578:	ea000043 	b	f68c <_vfiprintf_r+0x494>
    f57c:	e59d6004 	ldr	r6, [sp, #4]
    f580:	e3866010 	orr	r6, r6, #16
    f584:	e58d6004 	str	r6, [sp, #4]
    f588:	e59dc004 	ldr	ip, [sp, #4]
    f58c:	e21c2010 	ands	r2, ip, #16
    f590:	0a0002c6 	beq	100b0 <_vfiprintf_r+0xeb8>
    f594:	e59d5018 	ldr	r5, [sp, #24]
    f598:	e5953000 	ldr	r3, [r5]
    f59c:	e3a00000 	mov	r0, #0
    f5a0:	e0532000 	subs	r2, r3, r0
    f5a4:	e2855004 	add	r5, r5, #4
    f5a8:	13a02001 	movne	r2, #1
    f5ac:	e58d5018 	str	r5, [sp, #24]
    f5b0:	eaffffca 	b	f4e0 <_vfiprintf_r+0x2e8>
    f5b4:	e59d6018 	ldr	r6, [sp, #24]
    f5b8:	e5966000 	ldr	r6, [r6]
    f5bc:	e59dc018 	ldr	ip, [sp, #24]
    f5c0:	e3560000 	cmp	r6, #0
    f5c4:	e58d601c 	str	r6, [sp, #28]
    f5c8:	e28c3004 	add	r3, ip, #4
    f5cc:	aa0002c2 	bge	100dc <_vfiprintf_r+0xee4>
    f5d0:	e2666000 	rsb	r6, r6, #0
    f5d4:	e58d601c 	str	r6, [sp, #28]
    f5d8:	e58d3018 	str	r3, [sp, #24]
    f5dc:	e59d3004 	ldr	r3, [sp, #4]
    f5e0:	e3833004 	orr	r3, r3, #4
    f5e4:	e58d3004 	str	r3, [sp, #4]
    f5e8:	eaffff51 	b	f334 <_vfiprintf_r+0x13c>
    f5ec:	e59d3004 	ldr	r3, [sp, #4]
    f5f0:	e3833010 	orr	r3, r3, #16
    f5f4:	e58d3004 	str	r3, [sp, #4]
    f5f8:	eaffff4d 	b	f334 <_vfiprintf_r+0x13c>
    f5fc:	e59dc004 	ldr	ip, [sp, #4]
    f600:	e38cc040 	orr	ip, ip, #64	; 0x40
    f604:	e58dc004 	str	ip, [sp, #4]
    f608:	eaffff49 	b	f334 <_vfiprintf_r+0x13c>
    f60c:	e5cd1047 	strb	r1, [sp, #71]	; 0x47
    f610:	e59d5004 	ldr	r5, [sp, #4]
    f614:	e3150010 	tst	r5, #16
    f618:	1a000002 	bne	f628 <_vfiprintf_r+0x430>
    f61c:	e59dc004 	ldr	ip, [sp, #4]
    f620:	e31c0040 	tst	ip, #64	; 0x40
    f624:	1a0002d8 	bne	1018c <_vfiprintf_r+0xf94>
    f628:	e59d6018 	ldr	r6, [sp, #24]
    f62c:	e5963000 	ldr	r3, [r6]
    f630:	e2866004 	add	r6, r6, #4
    f634:	e3530000 	cmp	r3, #0
    f638:	e58d6018 	str	r6, [sp, #24]
    f63c:	ba0002d8 	blt	101a4 <_vfiprintf_r+0xfac>
    f640:	e5dd1047 	ldrb	r1, [sp, #71]	; 0x47
    f644:	03a02000 	moveq	r2, #0
    f648:	13a02001 	movne	r2, #1
    f64c:	e3a00001 	mov	r0, #1
    f650:	eaffffa4 	b	f4e8 <_vfiprintf_r+0x2f0>
    f654:	e59d3018 	ldr	r3, [sp, #24]
    f658:	e59dc018 	ldr	ip, [sp, #24]
    f65c:	e5933000 	ldr	r3, [r3]
    f660:	e3a06001 	mov	r6, #1
    f664:	e28cc004 	add	ip, ip, #4
    f668:	e5cd3058 	strb	r3, [sp, #88]	; 0x58
    f66c:	e3a03000 	mov	r3, #0
    f670:	e58d6008 	str	r6, [sp, #8]
    f674:	e58dc018 	str	ip, [sp, #24]
    f678:	e5cd3047 	strb	r3, [sp, #71]	; 0x47
    f67c:	e58d600c 	str	r6, [sp, #12]
    f680:	e28d5058 	add	r5, sp, #88	; 0x58
    f684:	e3a06000 	mov	r6, #0
    f688:	e58d6014 	str	r6, [sp, #20]
    f68c:	e59dc004 	ldr	ip, [sp, #4]
    f690:	e21cc002 	ands	ip, ip, #2
    f694:	159d3008 	ldrne	r3, [sp, #8]
    f698:	e59d6004 	ldr	r6, [sp, #4]
    f69c:	12833002 	addne	r3, r3, #2
    f6a0:	158d3008 	strne	r3, [sp, #8]
    f6a4:	e2166084 	ands	r6, r6, #132	; 0x84
    f6a8:	e58dc024 	str	ip, [sp, #36]	; 0x24
    f6ac:	e58d6028 	str	r6, [sp, #40]	; 0x28
    f6b0:	1a0000d1 	bne	f9fc <_vfiprintf_r+0x804>
    f6b4:	e59dc01c 	ldr	ip, [sp, #28]
    f6b8:	e59d3008 	ldr	r3, [sp, #8]
    f6bc:	e063400c 	rsb	r4, r3, ip
    f6c0:	e3540000 	cmp	r4, #0
    f6c4:	da0000cc 	ble	f9fc <_vfiprintf_r+0x804>
    f6c8:	e3540010 	cmp	r4, #16
    f6cc:	e59d1054 	ldr	r1, [sp, #84]	; 0x54
    f6d0:	e59d2050 	ldr	r2, [sp, #80]	; 0x50
    f6d4:	e59fab88 	ldr	sl, [pc, #2952]	; 10264 <_vfiprintf_r+0x106c>
    f6d8:	da000023 	ble	f76c <_vfiprintf_r+0x574>
    f6dc:	e28d604c 	add	r6, sp, #76	; 0x4c
    f6e0:	e58d5038 	str	r5, [sp, #56]	; 0x38
    f6e4:	e3a09010 	mov	r9, #16
    f6e8:	e1a0500a 	mov	r5, sl
    f6ec:	e1a03002 	mov	r3, r2
    f6f0:	e1a0a004 	mov	sl, r4
    f6f4:	e1a04006 	mov	r4, r6
    f6f8:	e59d6020 	ldr	r6, [sp, #32]
    f6fc:	ea000002 	b	f70c <_vfiprintf_r+0x514>
    f700:	e24aa010 	sub	sl, sl, #16
    f704:	e35a0010 	cmp	sl, #16
    f708:	da000013 	ble	f75c <_vfiprintf_r+0x564>
    f70c:	e2833001 	add	r3, r3, #1
    f710:	e2811010 	add	r1, r1, #16
    f714:	e3530007 	cmp	r3, #7
    f718:	e8880220 	stm	r8, {r5, r9}
    f71c:	e58d1054 	str	r1, [sp, #84]	; 0x54
    f720:	e58d3050 	str	r3, [sp, #80]	; 0x50
    f724:	d2888008 	addle	r8, r8, #8
    f728:	dafffff4 	ble	f700 <_vfiprintf_r+0x508>
    f72c:	e1a00006 	mov	r0, r6
    f730:	e1a01007 	mov	r1, r7
    f734:	e1a02004 	mov	r2, r4
    f738:	ebfffe7e 	bl	f138 <__sprint_r>
    f73c:	e3500000 	cmp	r0, #0
    f740:	1a00016f 	bne	fd04 <_vfiprintf_r+0xb0c>
    f744:	e24aa010 	sub	sl, sl, #16
    f748:	e35a0010 	cmp	sl, #16
    f74c:	e59d1054 	ldr	r1, [sp, #84]	; 0x54
    f750:	e59d3050 	ldr	r3, [sp, #80]	; 0x50
    f754:	e28d80e8 	add	r8, sp, #232	; 0xe8
    f758:	caffffeb 	bgt	f70c <_vfiprintf_r+0x514>
    f75c:	e1a0400a 	mov	r4, sl
    f760:	e1a0a005 	mov	sl, r5
    f764:	e59d5038 	ldr	r5, [sp, #56]	; 0x38
    f768:	e1a02003 	mov	r2, r3
    f76c:	e2822001 	add	r2, r2, #1
    f770:	e0811004 	add	r1, r1, r4
    f774:	e3520007 	cmp	r2, #7
    f778:	e588a000 	str	sl, [r8]
    f77c:	e5884004 	str	r4, [r8, #4]
    f780:	e58d1054 	str	r1, [sp, #84]	; 0x54
    f784:	e58d2050 	str	r2, [sp, #80]	; 0x50
    f788:	d2888008 	addle	r8, r8, #8
    f78c:	da00009c 	ble	fa04 <_vfiprintf_r+0x80c>
    f790:	e59d0020 	ldr	r0, [sp, #32]
    f794:	e1a01007 	mov	r1, r7
    f798:	e28d204c 	add	r2, sp, #76	; 0x4c
    f79c:	ebfffe65 	bl	f138 <__sprint_r>
    f7a0:	e3500000 	cmp	r0, #0
    f7a4:	1a000156 	bne	fd04 <_vfiprintf_r+0xb0c>
    f7a8:	e59d1054 	ldr	r1, [sp, #84]	; 0x54
    f7ac:	e59d2050 	ldr	r2, [sp, #80]	; 0x50
    f7b0:	e28d80e8 	add	r8, sp, #232	; 0xe8
    f7b4:	ea000092 	b	fa04 <_vfiprintf_r+0x80c>
    f7b8:	e59f3aa8 	ldr	r3, [pc, #2728]	; 10268 <_vfiprintf_r+0x1070>
    f7bc:	e5cd1047 	strb	r1, [sp, #71]	; 0x47
    f7c0:	e58d3034 	str	r3, [sp, #52]	; 0x34
    f7c4:	e59d3004 	ldr	r3, [sp, #4]
    f7c8:	e3130010 	tst	r3, #16
    f7cc:	1a000002 	bne	f7dc <_vfiprintf_r+0x5e4>
    f7d0:	e59d6004 	ldr	r6, [sp, #4]
    f7d4:	e3160040 	tst	r6, #64	; 0x40
    f7d8:	1a000266 	bne	10178 <_vfiprintf_r+0xf80>
    f7dc:	e59d5018 	ldr	r5, [sp, #24]
    f7e0:	e5953000 	ldr	r3, [r5]
    f7e4:	e2855004 	add	r5, r5, #4
    f7e8:	e58d5018 	str	r5, [sp, #24]
    f7ec:	e59d6004 	ldr	r6, [sp, #4]
    f7f0:	e3160001 	tst	r6, #1
    f7f4:	1a000222 	bne	10084 <_vfiprintf_r+0xe8c>
    f7f8:	e2932000 	adds	r2, r3, #0
    f7fc:	13a02001 	movne	r2, #1
    f800:	e3a00002 	mov	r0, #2
    f804:	eaffff35 	b	f4e0 <_vfiprintf_r+0x2e8>
    f808:	e3520000 	cmp	r2, #0
    f80c:	e5cd1047 	strb	r1, [sp, #71]	; 0x47
    f810:	0a000132 	beq	fce0 <_vfiprintf_r+0xae8>
    f814:	e3a0c001 	mov	ip, #1
    f818:	e3a03000 	mov	r3, #0
    f81c:	e58dc008 	str	ip, [sp, #8]
    f820:	e5cd2058 	strb	r2, [sp, #88]	; 0x58
    f824:	e5cd3047 	strb	r3, [sp, #71]	; 0x47
    f828:	e58dc00c 	str	ip, [sp, #12]
    f82c:	e28d5058 	add	r5, sp, #88	; 0x58
    f830:	eaffff93 	b	f684 <_vfiprintf_r+0x48c>
    f834:	e59d6018 	ldr	r6, [sp, #24]
    f838:	e5965000 	ldr	r5, [r6]
    f83c:	e59dc018 	ldr	ip, [sp, #24]
    f840:	e3a06000 	mov	r6, #0
    f844:	e1550006 	cmp	r5, r6
    f848:	e5cd6047 	strb	r6, [sp, #71]	; 0x47
    f84c:	e28ca004 	add	sl, ip, #4
    f850:	0a00026e 	beq	10210 <_vfiprintf_r+0x1018>
    f854:	e3540000 	cmp	r4, #0
    f858:	e1a00005 	mov	r0, r5
    f85c:	ba000266 	blt	101fc <_vfiprintf_r+0x1004>
    f860:	e1a01006 	mov	r1, r6
    f864:	e1a02004 	mov	r2, r4
    f868:	ebfff95e 	bl	dde8 <memchr>
    f86c:	e3500000 	cmp	r0, #0
    f870:	0a00026e 	beq	10230 <_vfiprintf_r+0x1038>
    f874:	e0650000 	rsb	r0, r5, r0
    f878:	e1500004 	cmp	r0, r4
    f87c:	e58d000c 	str	r0, [sp, #12]
    f880:	e5dd1047 	ldrb	r1, [sp, #71]	; 0x47
    f884:	e58da018 	str	sl, [sp, #24]
    f888:	c58d400c 	strgt	r4, [sp, #12]
    f88c:	c58d6014 	strgt	r6, [sp, #20]
    f890:	caffff30 	bgt	f558 <_vfiprintf_r+0x360>
    f894:	e58d6014 	str	r6, [sp, #20]
    f898:	eaffff2e 	b	f558 <_vfiprintf_r+0x360>
    f89c:	e59d5004 	ldr	r5, [sp, #4]
    f8a0:	e3855010 	orr	r5, r5, #16
    f8a4:	e58d5004 	str	r5, [sp, #4]
    f8a8:	eafffea1 	b	f334 <_vfiprintf_r+0x13c>
    f8ac:	e59d3018 	ldr	r3, [sp, #24]
    f8b0:	e59d6018 	ldr	r6, [sp, #24]
    f8b4:	e59dc004 	ldr	ip, [sp, #4]
    f8b8:	e3a02030 	mov	r2, #48	; 0x30
    f8bc:	e5933000 	ldr	r3, [r3]
    f8c0:	e5cd2048 	strb	r2, [sp, #72]	; 0x48
    f8c4:	e59f59a0 	ldr	r5, [pc, #2464]	; 1026c <_vfiprintf_r+0x1074>
    f8c8:	e3a02078 	mov	r2, #120	; 0x78
    f8cc:	e2866004 	add	r6, r6, #4
    f8d0:	e38cc002 	orr	ip, ip, #2
    f8d4:	e5cd2049 	strb	r2, [sp, #73]	; 0x49
    f8d8:	e2932000 	adds	r2, r3, #0
    f8dc:	e58d6018 	str	r6, [sp, #24]
    f8e0:	e58dc004 	str	ip, [sp, #4]
    f8e4:	13a02001 	movne	r2, #1
    f8e8:	e58d5034 	str	r5, [sp, #52]	; 0x34
    f8ec:	e3a00002 	mov	r0, #2
    f8f0:	eafffefa 	b	f4e0 <_vfiprintf_r+0x2e8>
    f8f4:	e59dc004 	ldr	ip, [sp, #4]
    f8f8:	e31c0010 	tst	ip, #16
    f8fc:	e5cd1047 	strb	r1, [sp, #71]	; 0x47
    f900:	1a000215 	bne	1015c <_vfiprintf_r+0xf64>
    f904:	e59dc004 	ldr	ip, [sp, #4]
    f908:	e31c0040 	tst	ip, #64	; 0x40
    f90c:	0a000233 	beq	101e0 <_vfiprintf_r+0xfe8>
    f910:	e59d5018 	ldr	r5, [sp, #24]
    f914:	e59d6010 	ldr	r6, [sp, #16]
    f918:	e5953000 	ldr	r3, [r5]
    f91c:	e2855004 	add	r5, r5, #4
    f920:	e58d5018 	str	r5, [sp, #24]
    f924:	e1c360b0 	strh	r6, [r3]
    f928:	eafffe5c 	b	f2a0 <_vfiprintf_r+0xa8>
    f92c:	e59d3004 	ldr	r3, [sp, #4]
    f930:	e3833010 	orr	r3, r3, #16
    f934:	e5cd1047 	strb	r1, [sp, #71]	; 0x47
    f938:	e58d3004 	str	r3, [sp, #4]
    f93c:	eaffff33 	b	f610 <_vfiprintf_r+0x418>
    f940:	e3a0102b 	mov	r1, #43	; 0x2b
    f944:	eafffe7a 	b	f334 <_vfiprintf_r+0x13c>
    f948:	e59d5004 	ldr	r5, [sp, #4]
    f94c:	e3855001 	orr	r5, r5, #1
    f950:	e58d5004 	str	r5, [sp, #4]
    f954:	eafffe76 	b	f334 <_vfiprintf_r+0x13c>
    f958:	e3510000 	cmp	r1, #0
    f95c:	03a01020 	moveq	r1, #32
    f960:	eafffe73 	b	f334 <_vfiprintf_r+0x13c>
    f964:	e1a0000b 	mov	r0, fp
    f968:	e3a03000 	mov	r3, #0
    f96c:	e242c030 	sub	ip, r2, #48	; 0x30
    f970:	e4d02001 	ldrb	r2, [r0], #1
    f974:	e2425030 	sub	r5, r2, #48	; 0x30
    f978:	e0833103 	add	r3, r3, r3, lsl #2
    f97c:	e3550009 	cmp	r5, #9
    f980:	e08c3083 	add	r3, ip, r3, lsl #1
    f984:	e1a0b000 	mov	fp, r0
    f988:	9afffff7 	bls	f96c <_vfiprintf_r+0x774>
    f98c:	e58d301c 	str	r3, [sp, #28]
    f990:	eafffe68 	b	f338 <_vfiprintf_r+0x140>
    f994:	e59d6004 	ldr	r6, [sp, #4]
    f998:	e3866080 	orr	r6, r6, #128	; 0x80
    f99c:	e58d6004 	str	r6, [sp, #4]
    f9a0:	eafffe63 	b	f334 <_vfiprintf_r+0x13c>
    f9a4:	e4db2001 	ldrb	r2, [fp], #1
    f9a8:	e352002a 	cmp	r2, #42	; 0x2a
    f9ac:	0a000224 	beq	10244 <_vfiprintf_r+0x104c>
    f9b0:	e2423030 	sub	r3, r2, #48	; 0x30
    f9b4:	e3530009 	cmp	r3, #9
    f9b8:	83a04000 	movhi	r4, #0
    f9bc:	8afffe5d 	bhi	f338 <_vfiprintf_r+0x140>
    f9c0:	e1a0000b 	mov	r0, fp
    f9c4:	e3a04000 	mov	r4, #0
    f9c8:	e4d02001 	ldrb	r2, [r0], #1
    f9cc:	e0844104 	add	r4, r4, r4, lsl #2
    f9d0:	e0834084 	add	r4, r3, r4, lsl #1
    f9d4:	e2423030 	sub	r3, r2, #48	; 0x30
    f9d8:	e3530009 	cmp	r3, #9
    f9dc:	e1a0b000 	mov	fp, r0
    f9e0:	9afffff8 	bls	f9c8 <_vfiprintf_r+0x7d0>
    f9e4:	e1844fc4 	orr	r4, r4, r4, asr #31
    f9e8:	eafffe52 	b	f338 <_vfiprintf_r+0x140>
    f9ec:	e59fc878 	ldr	ip, [pc, #2168]	; 1026c <_vfiprintf_r+0x1074>
    f9f0:	e5cd1047 	strb	r1, [sp, #71]	; 0x47
    f9f4:	e58dc034 	str	ip, [sp, #52]	; 0x34
    f9f8:	eaffff71 	b	f7c4 <_vfiprintf_r+0x5cc>
    f9fc:	e59d1054 	ldr	r1, [sp, #84]	; 0x54
    fa00:	e59d2050 	ldr	r2, [sp, #80]	; 0x50
    fa04:	e5dd3047 	ldrb	r3, [sp, #71]	; 0x47
    fa08:	e3530000 	cmp	r3, #0
    fa0c:	0a00000a 	beq	fa3c <_vfiprintf_r+0x844>
    fa10:	e2822001 	add	r2, r2, #1
    fa14:	e28d3047 	add	r3, sp, #71	; 0x47
    fa18:	e5883000 	str	r3, [r8]
    fa1c:	e2811001 	add	r1, r1, #1
    fa20:	e3520007 	cmp	r2, #7
    fa24:	e3a03001 	mov	r3, #1
    fa28:	e5883004 	str	r3, [r8, #4]
    fa2c:	e58d1054 	str	r1, [sp, #84]	; 0x54
    fa30:	e58d2050 	str	r2, [sp, #80]	; 0x50
    fa34:	d2888008 	addle	r8, r8, #8
    fa38:	ca00015b 	bgt	ffac <_vfiprintf_r+0xdb4>
    fa3c:	e59dc024 	ldr	ip, [sp, #36]	; 0x24
    fa40:	e35c0000 	cmp	ip, #0
    fa44:	0a00000a 	beq	fa74 <_vfiprintf_r+0x87c>
    fa48:	e2822001 	add	r2, r2, #1
    fa4c:	e28d3048 	add	r3, sp, #72	; 0x48
    fa50:	e5883000 	str	r3, [r8]
    fa54:	e2811002 	add	r1, r1, #2
    fa58:	e3520007 	cmp	r2, #7
    fa5c:	e3a03002 	mov	r3, #2
    fa60:	e5883004 	str	r3, [r8, #4]
    fa64:	e58d1054 	str	r1, [sp, #84]	; 0x54
    fa68:	e58d2050 	str	r2, [sp, #80]	; 0x50
    fa6c:	d2888008 	addle	r8, r8, #8
    fa70:	ca000143 	bgt	ff84 <_vfiprintf_r+0xd8c>
    fa74:	e59d6028 	ldr	r6, [sp, #40]	; 0x28
    fa78:	e3560080 	cmp	r6, #128	; 0x80
    fa7c:	0a0000aa 	beq	fd2c <_vfiprintf_r+0xb34>
    fa80:	e59d6014 	ldr	r6, [sp, #20]
    fa84:	e59dc00c 	ldr	ip, [sp, #12]
    fa88:	e06c4006 	rsb	r4, ip, r6
    fa8c:	e3540000 	cmp	r4, #0
    fa90:	da000038 	ble	fb78 <_vfiprintf_r+0x980>
    fa94:	e3540010 	cmp	r4, #16
    fa98:	e59fa7d0 	ldr	sl, [pc, #2000]	; 10270 <_vfiprintf_r+0x1078>
    fa9c:	da000023 	ble	fb30 <_vfiprintf_r+0x938>
    faa0:	e28d604c 	add	r6, sp, #76	; 0x4c
    faa4:	e58d5014 	str	r5, [sp, #20]
    faa8:	e3a09010 	mov	r9, #16
    faac:	e1a0500a 	mov	r5, sl
    fab0:	e1a03002 	mov	r3, r2
    fab4:	e1a0a004 	mov	sl, r4
    fab8:	e1a04006 	mov	r4, r6
    fabc:	e59d6020 	ldr	r6, [sp, #32]
    fac0:	ea000002 	b	fad0 <_vfiprintf_r+0x8d8>
    fac4:	e24aa010 	sub	sl, sl, #16
    fac8:	e35a0010 	cmp	sl, #16
    facc:	da000013 	ble	fb20 <_vfiprintf_r+0x928>
    fad0:	e2833001 	add	r3, r3, #1
    fad4:	e2811010 	add	r1, r1, #16
    fad8:	e3530007 	cmp	r3, #7
    fadc:	e8880220 	stm	r8, {r5, r9}
    fae0:	e58d1054 	str	r1, [sp, #84]	; 0x54
    fae4:	e58d3050 	str	r3, [sp, #80]	; 0x50
    fae8:	d2888008 	addle	r8, r8, #8
    faec:	dafffff4 	ble	fac4 <_vfiprintf_r+0x8cc>
    faf0:	e1a00006 	mov	r0, r6
    faf4:	e1a01007 	mov	r1, r7
    faf8:	e1a02004 	mov	r2, r4
    fafc:	ebfffd8d 	bl	f138 <__sprint_r>
    fb00:	e3500000 	cmp	r0, #0
    fb04:	1a00007e 	bne	fd04 <_vfiprintf_r+0xb0c>
    fb08:	e24aa010 	sub	sl, sl, #16
    fb0c:	e35a0010 	cmp	sl, #16
    fb10:	e59d1054 	ldr	r1, [sp, #84]	; 0x54
    fb14:	e59d3050 	ldr	r3, [sp, #80]	; 0x50
    fb18:	e28d80e8 	add	r8, sp, #232	; 0xe8
    fb1c:	caffffeb 	bgt	fad0 <_vfiprintf_r+0x8d8>
    fb20:	e1a0400a 	mov	r4, sl
    fb24:	e1a0a005 	mov	sl, r5
    fb28:	e59d5014 	ldr	r5, [sp, #20]
    fb2c:	e1a02003 	mov	r2, r3
    fb30:	e2822001 	add	r2, r2, #1
    fb34:	e0811004 	add	r1, r1, r4
    fb38:	e3520007 	cmp	r2, #7
    fb3c:	e588a000 	str	sl, [r8]
    fb40:	e5884004 	str	r4, [r8, #4]
    fb44:	e58d1054 	str	r1, [sp, #84]	; 0x54
    fb48:	e58d2050 	str	r2, [sp, #80]	; 0x50
    fb4c:	d2888008 	addle	r8, r8, #8
    fb50:	da000008 	ble	fb78 <_vfiprintf_r+0x980>
    fb54:	e59d0020 	ldr	r0, [sp, #32]
    fb58:	e1a01007 	mov	r1, r7
    fb5c:	e28d204c 	add	r2, sp, #76	; 0x4c
    fb60:	ebfffd74 	bl	f138 <__sprint_r>
    fb64:	e3500000 	cmp	r0, #0
    fb68:	1a000065 	bne	fd04 <_vfiprintf_r+0xb0c>
    fb6c:	e59d1054 	ldr	r1, [sp, #84]	; 0x54
    fb70:	e59d2050 	ldr	r2, [sp, #80]	; 0x50
    fb74:	e28d80e8 	add	r8, sp, #232	; 0xe8
    fb78:	e59d600c 	ldr	r6, [sp, #12]
    fb7c:	e2822001 	add	r2, r2, #1
    fb80:	e0811006 	add	r1, r1, r6
    fb84:	e3520007 	cmp	r2, #7
    fb88:	e8880060 	stm	r8, {r5, r6}
    fb8c:	e58d1054 	str	r1, [sp, #84]	; 0x54
    fb90:	e58d2050 	str	r2, [sp, #80]	; 0x50
    fb94:	d2888008 	addle	r8, r8, #8
    fb98:	ca0000a4 	bgt	fe30 <_vfiprintf_r+0xc38>
    fb9c:	e59d3004 	ldr	r3, [sp, #4]
    fba0:	e3130004 	tst	r3, #4
    fba4:	0a000030 	beq	fc6c <_vfiprintf_r+0xa74>
    fba8:	e59d601c 	ldr	r6, [sp, #28]
    fbac:	e59dc008 	ldr	ip, [sp, #8]
    fbb0:	e06c5006 	rsb	r5, ip, r6
    fbb4:	e3550000 	cmp	r5, #0
    fbb8:	da00002b 	ble	fc6c <_vfiprintf_r+0xa74>
    fbbc:	e3550010 	cmp	r5, #16
    fbc0:	e59d2050 	ldr	r2, [sp, #80]	; 0x50
    fbc4:	e59fa698 	ldr	sl, [pc, #1688]	; 10264 <_vfiprintf_r+0x106c>
    fbc8:	da00001f 	ble	fc4c <_vfiprintf_r+0xa54>
    fbcc:	e3a06010 	mov	r6, #16
    fbd0:	e28d404c 	add	r4, sp, #76	; 0x4c
    fbd4:	e59d9020 	ldr	r9, [sp, #32]
    fbd8:	e1a03002 	mov	r3, r2
    fbdc:	e1a0c001 	mov	ip, r1
    fbe0:	ea000002 	b	fbf0 <_vfiprintf_r+0x9f8>
    fbe4:	e2455010 	sub	r5, r5, #16
    fbe8:	e3550010 	cmp	r5, #16
    fbec:	da000014 	ble	fc44 <_vfiprintf_r+0xa4c>
    fbf0:	e2833001 	add	r3, r3, #1
    fbf4:	e28cc010 	add	ip, ip, #16
    fbf8:	e3530007 	cmp	r3, #7
    fbfc:	e588a000 	str	sl, [r8]
    fc00:	e5886004 	str	r6, [r8, #4]
    fc04:	e58dc054 	str	ip, [sp, #84]	; 0x54
    fc08:	e58d3050 	str	r3, [sp, #80]	; 0x50
    fc0c:	d2888008 	addle	r8, r8, #8
    fc10:	dafffff3 	ble	fbe4 <_vfiprintf_r+0x9ec>
    fc14:	e1a00009 	mov	r0, r9
    fc18:	e1a01007 	mov	r1, r7
    fc1c:	e1a02004 	mov	r2, r4
    fc20:	ebfffd44 	bl	f138 <__sprint_r>
    fc24:	e3500000 	cmp	r0, #0
    fc28:	1a000035 	bne	fd04 <_vfiprintf_r+0xb0c>
    fc2c:	e2455010 	sub	r5, r5, #16
    fc30:	e28d3050 	add	r3, sp, #80	; 0x50
    fc34:	e3550010 	cmp	r5, #16
    fc38:	e8931008 	ldm	r3, {r3, ip}
    fc3c:	e28d80e8 	add	r8, sp, #232	; 0xe8
    fc40:	caffffea 	bgt	fbf0 <_vfiprintf_r+0x9f8>
    fc44:	e1a02003 	mov	r2, r3
    fc48:	e1a0100c 	mov	r1, ip
    fc4c:	e2822001 	add	r2, r2, #1
    fc50:	e0851001 	add	r1, r5, r1
    fc54:	e3520007 	cmp	r2, #7
    fc58:	e588a000 	str	sl, [r8]
    fc5c:	e5885004 	str	r5, [r8, #4]
    fc60:	e58d1054 	str	r1, [sp, #84]	; 0x54
    fc64:	e58d2050 	str	r2, [sp, #80]	; 0x50
    fc68:	ca00011d 	bgt	100e4 <_vfiprintf_r+0xeec>
    fc6c:	e59d5010 	ldr	r5, [sp, #16]
    fc70:	e59d6008 	ldr	r6, [sp, #8]
    fc74:	e59dc01c 	ldr	ip, [sp, #28]
    fc78:	e156000c 	cmp	r6, ip
    fc7c:	a0855006 	addge	r5, r5, r6
    fc80:	b085500c 	addlt	r5, r5, ip
    fc84:	e3510000 	cmp	r1, #0
    fc88:	e58d5010 	str	r5, [sp, #16]
    fc8c:	1a000070 	bne	fe54 <_vfiprintf_r+0xc5c>
    fc90:	e3a03000 	mov	r3, #0
    fc94:	e58d3050 	str	r3, [sp, #80]	; 0x50
    fc98:	e5db3000 	ldrb	r3, [fp]
    fc9c:	e3530025 	cmp	r3, #37	; 0x25
    fca0:	13530000 	cmpne	r3, #0
    fca4:	e28d80e8 	add	r8, sp, #232	; 0xe8
    fca8:	1afffd80 	bne	f2b0 <_vfiprintf_r+0xb8>
    fcac:	e1a0400b 	mov	r4, fp
    fcb0:	eafffd95 	b	f30c <_vfiprintf_r+0x114>
    fcb4:	e3500000 	cmp	r0, #0
    fcb8:	1a00006c 	bne	fe70 <_vfiprintf_r+0xc78>
    fcbc:	e59d5004 	ldr	r5, [sp, #4]
    fcc0:	e3150001 	tst	r5, #1
    fcc4:	0a0000d4 	beq	1001c <_vfiprintf_r+0xe24>
    fcc8:	e59d603c 	ldr	r6, [sp, #60]	; 0x3c
    fccc:	e3a03030 	mov	r3, #48	; 0x30
    fcd0:	e5cd307f 	strb	r3, [sp, #127]	; 0x7f
    fcd4:	e58d600c 	str	r6, [sp, #12]
    fcd8:	e59d5030 	ldr	r5, [sp, #48]	; 0x30
    fcdc:	eafffe1c 	b	f554 <_vfiprintf_r+0x35c>
    fce0:	e59d3054 	ldr	r3, [sp, #84]	; 0x54
    fce4:	e3530000 	cmp	r3, #0
    fce8:	e59da020 	ldr	sl, [sp, #32]
    fcec:	0a000004 	beq	fd04 <_vfiprintf_r+0xb0c>
    fcf0:	e1a0000a 	mov	r0, sl
    fcf4:	e1a01007 	mov	r1, r7
    fcf8:	e28d204c 	add	r2, sp, #76	; 0x4c
    fcfc:	e28d5058 	add	r5, sp, #88	; 0x58
    fd00:	ebfffd0c 	bl	f138 <__sprint_r>
    fd04:	e1d730bc 	ldrh	r3, [r7, #12]
    fd08:	e3130040 	tst	r3, #64	; 0x40
    fd0c:	0a000001 	beq	fd18 <_vfiprintf_r+0xb20>
    fd10:	e3e03000 	mvn	r3, #0
    fd14:	e58d3010 	str	r3, [sp, #16]
    fd18:	e59d0010 	ldr	r0, [sp, #16]
    fd1c:	e28dd0ec 	add	sp, sp, #236	; 0xec
    fd20:	e28ddb01 	add	sp, sp, #1024	; 0x400
    fd24:	e8bd4ff0 	pop	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
    fd28:	e12fff1e 	bx	lr
    fd2c:	e59dc01c 	ldr	ip, [sp, #28]
    fd30:	e59d3008 	ldr	r3, [sp, #8]
    fd34:	e063600c 	rsb	r6, r3, ip
    fd38:	e3560000 	cmp	r6, #0
    fd3c:	daffff4f 	ble	fa80 <_vfiprintf_r+0x888>
    fd40:	e3560010 	cmp	r6, #16
    fd44:	e59fa524 	ldr	sl, [pc, #1316]	; 10270 <_vfiprintf_r+0x1078>
    fd48:	da000025 	ble	fde4 <_vfiprintf_r+0xbec>
    fd4c:	e28d404c 	add	r4, sp, #76	; 0x4c
    fd50:	e1a0000a 	mov	r0, sl
    fd54:	e58d5024 	str	r5, [sp, #36]	; 0x24
    fd58:	e1a0a006 	mov	sl, r6
    fd5c:	e1a05004 	mov	r5, r4
    fd60:	e3a09010 	mov	r9, #16
    fd64:	e1a03002 	mov	r3, r2
    fd68:	e59d4020 	ldr	r4, [sp, #32]
    fd6c:	e1a06000 	mov	r6, r0
    fd70:	ea000002 	b	fd80 <_vfiprintf_r+0xb88>
    fd74:	e24aa010 	sub	sl, sl, #16
    fd78:	e35a0010 	cmp	sl, #16
    fd7c:	da000013 	ble	fdd0 <_vfiprintf_r+0xbd8>
    fd80:	e2833001 	add	r3, r3, #1
    fd84:	e2811010 	add	r1, r1, #16
    fd88:	e3530007 	cmp	r3, #7
    fd8c:	e8880240 	stm	r8, {r6, r9}
    fd90:	e58d1054 	str	r1, [sp, #84]	; 0x54
    fd94:	e58d3050 	str	r3, [sp, #80]	; 0x50
    fd98:	d2888008 	addle	r8, r8, #8
    fd9c:	dafffff4 	ble	fd74 <_vfiprintf_r+0xb7c>
    fda0:	e1a00004 	mov	r0, r4
    fda4:	e1a01007 	mov	r1, r7
    fda8:	e1a02005 	mov	r2, r5
    fdac:	ebfffce1 	bl	f138 <__sprint_r>
    fdb0:	e3500000 	cmp	r0, #0
    fdb4:	1affffd2 	bne	fd04 <_vfiprintf_r+0xb0c>
    fdb8:	e24aa010 	sub	sl, sl, #16
    fdbc:	e35a0010 	cmp	sl, #16
    fdc0:	e59d1054 	ldr	r1, [sp, #84]	; 0x54
    fdc4:	e59d3050 	ldr	r3, [sp, #80]	; 0x50
    fdc8:	e28d80e8 	add	r8, sp, #232	; 0xe8
    fdcc:	caffffeb 	bgt	fd80 <_vfiprintf_r+0xb88>
    fdd0:	e1a00006 	mov	r0, r6
    fdd4:	e59d5024 	ldr	r5, [sp, #36]	; 0x24
    fdd8:	e1a0600a 	mov	r6, sl
    fddc:	e1a02003 	mov	r2, r3
    fde0:	e1a0a000 	mov	sl, r0
    fde4:	e2822001 	add	r2, r2, #1
    fde8:	e0811006 	add	r1, r1, r6
    fdec:	e3520007 	cmp	r2, #7
    fdf0:	e588a000 	str	sl, [r8]
    fdf4:	e5886004 	str	r6, [r8, #4]
    fdf8:	e58d1054 	str	r1, [sp, #84]	; 0x54
    fdfc:	e58d2050 	str	r2, [sp, #80]	; 0x50
    fe00:	d2888008 	addle	r8, r8, #8
    fe04:	daffff1d 	ble	fa80 <_vfiprintf_r+0x888>
    fe08:	e59d0020 	ldr	r0, [sp, #32]
    fe0c:	e1a01007 	mov	r1, r7
    fe10:	e28d204c 	add	r2, sp, #76	; 0x4c
    fe14:	ebfffcc7 	bl	f138 <__sprint_r>
    fe18:	e3500000 	cmp	r0, #0
    fe1c:	1affffb8 	bne	fd04 <_vfiprintf_r+0xb0c>
    fe20:	e59d1054 	ldr	r1, [sp, #84]	; 0x54
    fe24:	e59d2050 	ldr	r2, [sp, #80]	; 0x50
    fe28:	e28d80e8 	add	r8, sp, #232	; 0xe8
    fe2c:	eaffff13 	b	fa80 <_vfiprintf_r+0x888>
    fe30:	e59d0020 	ldr	r0, [sp, #32]
    fe34:	e1a01007 	mov	r1, r7
    fe38:	e28d204c 	add	r2, sp, #76	; 0x4c
    fe3c:	ebfffcbd 	bl	f138 <__sprint_r>
    fe40:	e3500000 	cmp	r0, #0
    fe44:	1affffae 	bne	fd04 <_vfiprintf_r+0xb0c>
    fe48:	e59d1054 	ldr	r1, [sp, #84]	; 0x54
    fe4c:	e28d80e8 	add	r8, sp, #232	; 0xe8
    fe50:	eaffff51 	b	fb9c <_vfiprintf_r+0x9a4>
    fe54:	e59d0020 	ldr	r0, [sp, #32]
    fe58:	e1a01007 	mov	r1, r7
    fe5c:	e28d204c 	add	r2, sp, #76	; 0x4c
    fe60:	ebfffcb4 	bl	f138 <__sprint_r>
    fe64:	e3500000 	cmp	r0, #0
    fe68:	0affff88 	beq	fc90 <_vfiprintf_r+0xa98>
    fe6c:	eaffffa4 	b	fd04 <_vfiprintf_r+0xb0c>
    fe70:	e58d200c 	str	r2, [sp, #12]
    fe74:	e59d502c 	ldr	r5, [sp, #44]	; 0x2c
    fe78:	eafffdb5 	b	f554 <_vfiprintf_r+0x35c>
    fe7c:	e1a0000a 	mov	r0, sl
    fe80:	e1a01007 	mov	r1, r7
    fe84:	ebffebef 	bl	ae48 <__swsetup_r>
    fe88:	e3500000 	cmp	r0, #0
    fe8c:	01d780bc 	ldrheq	r8, [r7, #12]
    fe90:	1affff9e 	bne	fd10 <_vfiprintf_r+0xb18>
    fe94:	e208301a 	and	r3, r8, #26
    fe98:	e353000a 	cmp	r3, #10
    fe9c:	1afffcef 	bne	f260 <_vfiprintf_r+0x68>
    fea0:	e1d7c0be 	ldrh	ip, [r7, #14]
    fea4:	e31c0902 	tst	ip, #32768	; 0x8000
    fea8:	1afffcec 	bne	f260 <_vfiprintf_r+0x68>
    feac:	e1a02004 	mov	r2, r4
    feb0:	e28d6e4e 	add	r6, sp, #1248	; 0x4e0
    feb4:	e59f43b8 	ldr	r4, [pc, #952]	; 10274 <_vfiprintf_r+0x107c>
    feb8:	e2866008 	add	r6, r6, #8
    febc:	e3c88002 	bic	r8, r8, #2
    fec0:	e18680b4 	strh	r8, [r6, r4]
    fec4:	e2844002 	add	r4, r4, #2
    fec8:	e186c0b4 	strh	ip, [r6, r4]
    fecc:	e597c01c 	ldr	ip, [r7, #28]
    fed0:	e597b064 	ldr	fp, [r7, #100]	; 0x64
    fed4:	e5979024 	ldr	r9, [r7, #36]	; 0x24
    fed8:	e3a0eb01 	mov	lr, #1024	; 0x400
    fedc:	e28d50e8 	add	r5, sp, #232	; 0xe8
    fee0:	e58dc09c 	str	ip, [sp, #156]	; 0x9c
    fee4:	e1a0000a 	mov	r0, sl
    fee8:	e3a0c000 	mov	ip, #0
    feec:	e28d1080 	add	r1, sp, #128	; 0x80
    fef0:	e59d3018 	ldr	r3, [sp, #24]
    fef4:	e58db0e4 	str	fp, [sp, #228]	; 0xe4
    fef8:	e58d90a4 	str	r9, [sp, #164]	; 0xa4
    fefc:	e58d5080 	str	r5, [sp, #128]	; 0x80
    ff00:	e58d5090 	str	r5, [sp, #144]	; 0x90
    ff04:	e58de088 	str	lr, [sp, #136]	; 0x88
    ff08:	e58de094 	str	lr, [sp, #148]	; 0x94
    ff0c:	e58dc098 	str	ip, [sp, #152]	; 0x98
    ff10:	ebfffcb8 	bl	f1f8 <_vfiprintf_r>
    ff14:	e3500000 	cmp	r0, #0
    ff18:	e58d0010 	str	r0, [sp, #16]
    ff1c:	ba000006 	blt	ff3c <_vfiprintf_r+0xd44>
    ff20:	e1a0000a 	mov	r0, sl
    ff24:	e28d1080 	add	r1, sp, #128	; 0x80
    ff28:	ebfff368 	bl	ccd0 <_fflush_r>
    ff2c:	e59d3010 	ldr	r3, [sp, #16]
    ff30:	e3500000 	cmp	r0, #0
    ff34:	13e03000 	mvnne	r3, #0
    ff38:	e58d3010 	str	r3, [sp, #16]
    ff3c:	e59f3330 	ldr	r3, [pc, #816]	; 10274 <_vfiprintf_r+0x107c>
    ff40:	e28d5e4e 	add	r5, sp, #1248	; 0x4e0
    ff44:	e2855008 	add	r5, r5, #8
    ff48:	e19530b3 	ldrh	r3, [r5, r3]
    ff4c:	e3130040 	tst	r3, #64	; 0x40
    ff50:	11d730bc 	ldrhne	r3, [r7, #12]
    ff54:	13833040 	orrne	r3, r3, #64	; 0x40
    ff58:	11c730bc 	strhne	r3, [r7, #12]
    ff5c:	eaffff6d 	b	fd18 <_vfiprintf_r+0xb20>
    ff60:	e59d0020 	ldr	r0, [sp, #32]
    ff64:	e1a01007 	mov	r1, r7
    ff68:	e28d204c 	add	r2, sp, #76	; 0x4c
    ff6c:	ebfffc71 	bl	f138 <__sprint_r>
    ff70:	e3500000 	cmp	r0, #0
    ff74:	e28d6058 	add	r6, sp, #88	; 0x58
    ff78:	1affff61 	bne	fd04 <_vfiprintf_r+0xb0c>
    ff7c:	e28d80e8 	add	r8, sp, #232	; 0xe8
    ff80:	eafffcde 	b	f300 <_vfiprintf_r+0x108>
    ff84:	e59d0020 	ldr	r0, [sp, #32]
    ff88:	e1a01007 	mov	r1, r7
    ff8c:	e28d204c 	add	r2, sp, #76	; 0x4c
    ff90:	ebfffc68 	bl	f138 <__sprint_r>
    ff94:	e3500000 	cmp	r0, #0
    ff98:	1affff59 	bne	fd04 <_vfiprintf_r+0xb0c>
    ff9c:	e59d1054 	ldr	r1, [sp, #84]	; 0x54
    ffa0:	e59d2050 	ldr	r2, [sp, #80]	; 0x50
    ffa4:	e28d80e8 	add	r8, sp, #232	; 0xe8
    ffa8:	eafffeb1 	b	fa74 <_vfiprintf_r+0x87c>
    ffac:	e59d0020 	ldr	r0, [sp, #32]
    ffb0:	e1a01007 	mov	r1, r7
    ffb4:	e28d204c 	add	r2, sp, #76	; 0x4c
    ffb8:	ebfffc5e 	bl	f138 <__sprint_r>
    ffbc:	e3500000 	cmp	r0, #0
    ffc0:	e28d6058 	add	r6, sp, #88	; 0x58
    ffc4:	1affff4e 	bne	fd04 <_vfiprintf_r+0xb0c>
    ffc8:	e59d1054 	ldr	r1, [sp, #84]	; 0x54
    ffcc:	e59d2050 	ldr	r2, [sp, #80]	; 0x50
    ffd0:	e28d80e8 	add	r8, sp, #232	; 0xe8
    ffd4:	eafffe98 	b	fa3c <_vfiprintf_r+0x844>
    ffd8:	ebfff3a0 	bl	ce60 <__sinit>
    ffdc:	eafffc90 	b	f224 <_vfiprintf_r+0x2c>
    ffe0:	e3530009 	cmp	r3, #9
    ffe4:	9a000046 	bls	10104 <_vfiprintf_r+0xf0c>
    ffe8:	e59d0030 	ldr	r0, [sp, #48]	; 0x30
    ffec:	e59f5284 	ldr	r5, [pc, #644]	; 10278 <_vfiprintf_r+0x1080>
    fff0:	e0826395 	umull	r6, r2, r5, r3
    fff4:	e1a021a2 	lsr	r2, r2, #3
    fff8:	e082c102 	add	ip, r2, r2, lsl #2
    fffc:	e043308c 	sub	r3, r3, ip, lsl #1
   10000:	e1a05000 	mov	r5, r0
   10004:	e2830030 	add	r0, r3, #48	; 0x30
   10008:	e2523000 	subs	r3, r2, #0
   1000c:	e5c50000 	strb	r0, [r5]
   10010:	e2450001 	sub	r0, r5, #1
   10014:	1afffff4 	bne	ffec <_vfiprintf_r+0xdf4>
   10018:	eafffd4a 	b	f548 <_vfiprintf_r+0x350>
   1001c:	e58d000c 	str	r0, [sp, #12]
   10020:	e59d502c 	ldr	r5, [sp, #44]	; 0x2c
   10024:	eafffd4a 	b	f554 <_vfiprintf_r+0x35c>
   10028:	e59dc034 	ldr	ip, [sp, #52]	; 0x34
   1002c:	e203000f 	and	r0, r3, #15
   10030:	e7dc0000 	ldrb	r0, [ip, r0]
   10034:	e1b03223 	lsrs	r3, r3, #4
   10038:	e1a05002 	mov	r5, r2
   1003c:	e5c20000 	strb	r0, [r2]
   10040:	e2422001 	sub	r2, r2, #1
   10044:	1afffff8 	bne	1002c <_vfiprintf_r+0xe34>
   10048:	e59d302c 	ldr	r3, [sp, #44]	; 0x2c
   1004c:	e0653003 	rsb	r3, r5, r3
   10050:	e58d300c 	str	r3, [sp, #12]
   10054:	eafffd3e 	b	f554 <_vfiprintf_r+0x35c>
   10058:	e59d3004 	ldr	r3, [sp, #4]
   1005c:	e3130040 	tst	r3, #64	; 0x40
   10060:	0a000056 	beq	101c0 <_vfiprintf_r+0xfc8>
   10064:	e59d5018 	ldr	r5, [sp, #24]
   10068:	e1d530b0 	ldrh	r3, [r5]
   1006c:	e2855004 	add	r5, r5, #4
   10070:	e2932000 	adds	r2, r3, #0
   10074:	e3a00001 	mov	r0, #1
   10078:	13a02001 	movne	r2, #1
   1007c:	e58d5018 	str	r5, [sp, #24]
   10080:	eafffd16 	b	f4e0 <_vfiprintf_r+0x2e8>
   10084:	e3530000 	cmp	r3, #0
   10088:	159dc004 	ldrne	ip, [sp, #4]
   1008c:	13a01030 	movne	r1, #48	; 0x30
   10090:	138cc002 	orrne	ip, ip, #2
   10094:	15cd2049 	strbne	r2, [sp, #73]	; 0x49
   10098:	15cd1048 	strbne	r1, [sp, #72]	; 0x48
   1009c:	158dc004 	strne	ip, [sp, #4]
   100a0:	13a02001 	movne	r2, #1
   100a4:	01a02003 	moveq	r2, r3
   100a8:	e3a00002 	mov	r0, #2
   100ac:	eafffd0b 	b	f4e0 <_vfiprintf_r+0x2e8>
   100b0:	e59d6004 	ldr	r6, [sp, #4]
   100b4:	e2160040 	ands	r0, r6, #64	; 0x40
   100b8:	0a000020 	beq	10140 <_vfiprintf_r+0xf48>
   100bc:	e59dc018 	ldr	ip, [sp, #24]
   100c0:	e1dc30b0 	ldrh	r3, [ip]
   100c4:	e1a00002 	mov	r0, r2
   100c8:	e28cc004 	add	ip, ip, #4
   100cc:	e2932000 	adds	r2, r3, #0
   100d0:	13a02001 	movne	r2, #1
   100d4:	e58dc018 	str	ip, [sp, #24]
   100d8:	eafffd00 	b	f4e0 <_vfiprintf_r+0x2e8>
   100dc:	e58d3018 	str	r3, [sp, #24]
   100e0:	eafffc93 	b	f334 <_vfiprintf_r+0x13c>
   100e4:	e59d0020 	ldr	r0, [sp, #32]
   100e8:	e1a01007 	mov	r1, r7
   100ec:	e28d204c 	add	r2, sp, #76	; 0x4c
   100f0:	ebfffc10 	bl	f138 <__sprint_r>
   100f4:	e3500000 	cmp	r0, #0
   100f8:	1affff01 	bne	fd04 <_vfiprintf_r+0xb0c>
   100fc:	e59d1054 	ldr	r1, [sp, #84]	; 0x54
   10100:	eafffed9 	b	fc6c <_vfiprintf_r+0xa74>
   10104:	e59dc03c 	ldr	ip, [sp, #60]	; 0x3c
   10108:	e2833030 	add	r3, r3, #48	; 0x30
   1010c:	e5cd307f 	strb	r3, [sp, #127]	; 0x7f
   10110:	e58dc00c 	str	ip, [sp, #12]
   10114:	e59d5030 	ldr	r5, [sp, #48]	; 0x30
   10118:	eafffd0d 	b	f554 <_vfiprintf_r+0x35c>
   1011c:	e3500030 	cmp	r0, #48	; 0x30
   10120:	0affffc8 	beq	10048 <_vfiprintf_r+0xe50>
   10124:	e59d602c 	ldr	r6, [sp, #44]	; 0x2c
   10128:	e1a05002 	mov	r5, r2
   1012c:	e0656006 	rsb	r6, r5, r6
   10130:	e3a02030 	mov	r2, #48	; 0x30
   10134:	e5432001 	strb	r2, [r3, #-1]
   10138:	e58d600c 	str	r6, [sp, #12]
   1013c:	eafffd04 	b	f554 <_vfiprintf_r+0x35c>
   10140:	e59d5018 	ldr	r5, [sp, #24]
   10144:	e5953000 	ldr	r3, [r5]
   10148:	e2855004 	add	r5, r5, #4
   1014c:	e2932000 	adds	r2, r3, #0
   10150:	13a02001 	movne	r2, #1
   10154:	e58d5018 	str	r5, [sp, #24]
   10158:	eafffce0 	b	f4e0 <_vfiprintf_r+0x2e8>
   1015c:	e59d5018 	ldr	r5, [sp, #24]
   10160:	e59d6010 	ldr	r6, [sp, #16]
   10164:	e5953000 	ldr	r3, [r5]
   10168:	e2855004 	add	r5, r5, #4
   1016c:	e58d5018 	str	r5, [sp, #24]
   10170:	e5836000 	str	r6, [r3]
   10174:	eafffc49 	b	f2a0 <_vfiprintf_r+0xa8>
   10178:	e59dc018 	ldr	ip, [sp, #24]
   1017c:	e1dc30b0 	ldrh	r3, [ip]
   10180:	e28cc004 	add	ip, ip, #4
   10184:	e58dc018 	str	ip, [sp, #24]
   10188:	eafffd97 	b	f7ec <_vfiprintf_r+0x5f4>
   1018c:	e59d5018 	ldr	r5, [sp, #24]
   10190:	e1d530f0 	ldrsh	r3, [r5]
   10194:	e2855004 	add	r5, r5, #4
   10198:	e3530000 	cmp	r3, #0
   1019c:	e58d5018 	str	r5, [sp, #24]
   101a0:	aafffd26 	bge	f640 <_vfiprintf_r+0x448>
   101a4:	e2633000 	rsb	r3, r3, #0
   101a8:	e3a0102d 	mov	r1, #45	; 0x2d
   101ac:	e2932000 	adds	r2, r3, #0
   101b0:	e5cd1047 	strb	r1, [sp, #71]	; 0x47
   101b4:	13a02001 	movne	r2, #1
   101b8:	e3a00001 	mov	r0, #1
   101bc:	eafffcc9 	b	f4e8 <_vfiprintf_r+0x2f0>
   101c0:	e59d6018 	ldr	r6, [sp, #24]
   101c4:	e5963000 	ldr	r3, [r6]
   101c8:	e2866004 	add	r6, r6, #4
   101cc:	e2932000 	adds	r2, r3, #0
   101d0:	e3a00001 	mov	r0, #1
   101d4:	13a02001 	movne	r2, #1
   101d8:	e58d6018 	str	r6, [sp, #24]
   101dc:	eafffcbf 	b	f4e0 <_vfiprintf_r+0x2e8>
   101e0:	e59dc018 	ldr	ip, [sp, #24]
   101e4:	e59d5010 	ldr	r5, [sp, #16]
   101e8:	e59c3000 	ldr	r3, [ip]
   101ec:	e28cc004 	add	ip, ip, #4
   101f0:	e58dc018 	str	ip, [sp, #24]
   101f4:	e5835000 	str	r5, [r3]
   101f8:	eafffc28 	b	f2a0 <_vfiprintf_r+0xa8>
   101fc:	ebfffbb5 	bl	f0d8 <strlen>
   10200:	e5dd1047 	ldrb	r1, [sp, #71]	; 0x47
   10204:	e58d000c 	str	r0, [sp, #12]
   10208:	e58da018 	str	sl, [sp, #24]
   1020c:	eafffda0 	b	f894 <_vfiprintf_r+0x69c>
   10210:	e3540006 	cmp	r4, #6
   10214:	23a04006 	movcs	r4, #6
   10218:	e1c43fc4 	bic	r3, r4, r4, asr #31
   1021c:	e58d400c 	str	r4, [sp, #12]
   10220:	e58d3008 	str	r3, [sp, #8]
   10224:	e58da018 	str	sl, [sp, #24]
   10228:	e59f504c 	ldr	r5, [pc, #76]	; 1027c <_vfiprintf_r+0x1084>
   1022c:	eafffd14 	b	f684 <_vfiprintf_r+0x48c>
   10230:	e5dd1047 	ldrb	r1, [sp, #71]	; 0x47
   10234:	e58da018 	str	sl, [sp, #24]
   10238:	e58d400c 	str	r4, [sp, #12]
   1023c:	e58d0014 	str	r0, [sp, #20]
   10240:	eafffcc4 	b	f558 <_vfiprintf_r+0x360>
   10244:	e59d5018 	ldr	r5, [sp, #24]
   10248:	e5954000 	ldr	r4, [r5]
   1024c:	e3540000 	cmp	r4, #0
   10250:	e2853004 	add	r3, r5, #4
   10254:	aaffffa0 	bge	100dc <_vfiprintf_r+0xee4>
   10258:	e58d3018 	str	r3, [sp, #24]
   1025c:	e3e04000 	mvn	r4, #0
   10260:	eafffc33 	b	f334 <_vfiprintf_r+0x13c>
   10264:	000124b0 	.word	0x000124b0
   10268:	00012334 	.word	0x00012334
   1026c:	00012348 	.word	0x00012348
   10270:	000124c0 	.word	0x000124c0
   10274:	fffffba4 	.word	0xfffffba4
   10278:	cccccccd 	.word	0xcccccccd
   1027c:	0001235c 	.word	0x0001235c

00010280 <vfiprintf>:
   10280:	e52d4004 	push	{r4}		; (str r4, [sp, #-4]!)
   10284:	e1a03002 	mov	r3, r2
   10288:	e59f2018 	ldr	r2, [pc, #24]	; 102a8 <vfiprintf+0x28>
   1028c:	e1a04000 	mov	r4, r0
   10290:	e1a0c001 	mov	ip, r1
   10294:	e5920000 	ldr	r0, [r2]
   10298:	e1a01004 	mov	r1, r4
   1029c:	e1a0200c 	mov	r2, ip
   102a0:	e8bd0010 	pop	{r4}
   102a4:	eafffbd3 	b	f1f8 <_vfiprintf_r>
   102a8:	0001a600 	.word	0x0001a600

000102ac <_write_r>:
   102ac:	e92d4038 	push	{r3, r4, r5, lr}
   102b0:	e59f4034 	ldr	r4, [pc, #52]	; 102ec <_write_r+0x40>
   102b4:	e1a05000 	mov	r5, r0
   102b8:	e1a00001 	mov	r0, r1
   102bc:	e1a01002 	mov	r1, r2
   102c0:	e1a02003 	mov	r2, r3
   102c4:	e3a03000 	mov	r3, #0
   102c8:	e5843000 	str	r3, [r4]
   102cc:	ebffe1a4 	bl	8964 <_write>
   102d0:	e3700001 	cmn	r0, #1
   102d4:	1a000002 	bne	102e4 <_write_r+0x38>
   102d8:	e5943000 	ldr	r3, [r4]
   102dc:	e3530000 	cmp	r3, #0
   102e0:	15853000 	strne	r3, [r5]
   102e4:	e8bd4038 	pop	{r3, r4, r5, lr}
   102e8:	e12fff1e 	bx	lr
   102ec:	0001afcc 	.word	0x0001afcc

000102f0 <_calloc_r>:
   102f0:	e92d4010 	push	{r4, lr}
   102f4:	e0010192 	mul	r1, r2, r1
   102f8:	ebfff4f3 	bl	d6cc <_malloc_r>
   102fc:	e2504000 	subs	r4, r0, #0
   10300:	0a00000b 	beq	10334 <_calloc_r+0x44>
   10304:	e5142004 	ldr	r2, [r4, #-4]
   10308:	e3c22003 	bic	r2, r2, #3
   1030c:	e2422004 	sub	r2, r2, #4
   10310:	e3520024 	cmp	r2, #36	; 0x24
   10314:	8a000017 	bhi	10378 <_calloc_r+0x88>
   10318:	e3520013 	cmp	r2, #19
   1031c:	91a03004 	movls	r3, r4
   10320:	8a000006 	bhi	10340 <_calloc_r+0x50>
   10324:	e3a02000 	mov	r2, #0
   10328:	e5832000 	str	r2, [r3]
   1032c:	e5832004 	str	r2, [r3, #4]
   10330:	e5832008 	str	r2, [r3, #8]
   10334:	e1a00004 	mov	r0, r4
   10338:	e8bd4010 	pop	{r4, lr}
   1033c:	e12fff1e 	bx	lr
   10340:	e3a03000 	mov	r3, #0
   10344:	e352001b 	cmp	r2, #27
   10348:	e5843000 	str	r3, [r4]
   1034c:	e5843004 	str	r3, [r4, #4]
   10350:	92843008 	addls	r3, r4, #8
   10354:	9afffff2 	bls	10324 <_calloc_r+0x34>
   10358:	e3520024 	cmp	r2, #36	; 0x24
   1035c:	e5843008 	str	r3, [r4, #8]
   10360:	e584300c 	str	r3, [r4, #12]
   10364:	05843010 	streq	r3, [r4, #16]
   10368:	05843014 	streq	r3, [r4, #20]
   1036c:	12843010 	addne	r3, r4, #16
   10370:	02843018 	addeq	r3, r4, #24
   10374:	eaffffea 	b	10324 <_calloc_r+0x34>
   10378:	e3a01000 	mov	r1, #0
   1037c:	ebffe031 	bl	8448 <memset>
   10380:	eaffffeb 	b	10334 <_calloc_r+0x44>

00010384 <_close_r>:
   10384:	e92d4038 	push	{r3, r4, r5, lr}
   10388:	e59f402c 	ldr	r4, [pc, #44]	; 103bc <_close_r+0x38>
   1038c:	e3a03000 	mov	r3, #0
   10390:	e1a05000 	mov	r5, r0
   10394:	e1a00001 	mov	r0, r1
   10398:	e5843000 	str	r3, [r4]
   1039c:	ebffe1f8 	bl	8b84 <_close>
   103a0:	e3700001 	cmn	r0, #1
   103a4:	1a000002 	bne	103b4 <_close_r+0x30>
   103a8:	e5943000 	ldr	r3, [r4]
   103ac:	e3530000 	cmp	r3, #0
   103b0:	15853000 	strne	r3, [r5]
   103b4:	e8bd4038 	pop	{r3, r4, r5, lr}
   103b8:	e12fff1e 	bx	lr
   103bc:	0001afcc 	.word	0x0001afcc

000103c0 <_fclose_r>:
   103c0:	e92d4070 	push	{r4, r5, r6, lr}
   103c4:	e2514000 	subs	r4, r1, #0
   103c8:	e1a05000 	mov	r5, r0
   103cc:	0a000007 	beq	103f0 <_fclose_r+0x30>
   103d0:	e3500000 	cmp	r0, #0
   103d4:	0a000002 	beq	103e4 <_fclose_r+0x24>
   103d8:	e5903038 	ldr	r3, [r0, #56]	; 0x38
   103dc:	e3530000 	cmp	r3, #0
   103e0:	0a00002b 	beq	10494 <_fclose_r+0xd4>
   103e4:	e1d430fc 	ldrsh	r3, [r4, #12]
   103e8:	e3530000 	cmp	r3, #0
   103ec:	1a000003 	bne	10400 <_fclose_r+0x40>
   103f0:	e3a06000 	mov	r6, #0
   103f4:	e1a00006 	mov	r0, r6
   103f8:	e8bd4070 	pop	{r4, r5, r6, lr}
   103fc:	e12fff1e 	bx	lr
   10400:	e1a00005 	mov	r0, r5
   10404:	e1a01004 	mov	r1, r4
   10408:	ebfff230 	bl	ccd0 <_fflush_r>
   1040c:	e594302c 	ldr	r3, [r4, #44]	; 0x2c
   10410:	e3530000 	cmp	r3, #0
   10414:	e1a06000 	mov	r6, r0
   10418:	0a000005 	beq	10434 <_fclose_r+0x74>
   1041c:	e1a00005 	mov	r0, r5
   10420:	e594101c 	ldr	r1, [r4, #28]
   10424:	e1a0e00f 	mov	lr, pc
   10428:	e12fff13 	bx	r3
   1042c:	e3500000 	cmp	r0, #0
   10430:	b3e06000 	mvnlt	r6, #0
   10434:	e1d430bc 	ldrh	r3, [r4, #12]
   10438:	e3130080 	tst	r3, #128	; 0x80
   1043c:	1a000016 	bne	1049c <_fclose_r+0xdc>
   10440:	e5941030 	ldr	r1, [r4, #48]	; 0x30
   10444:	e3510000 	cmp	r1, #0
   10448:	0a000005 	beq	10464 <_fclose_r+0xa4>
   1044c:	e2843040 	add	r3, r4, #64	; 0x40
   10450:	e1510003 	cmp	r1, r3
   10454:	11a00005 	movne	r0, r5
   10458:	1bfff331 	blne	d124 <_free_r>
   1045c:	e3a03000 	mov	r3, #0
   10460:	e5843030 	str	r3, [r4, #48]	; 0x30
   10464:	e5941044 	ldr	r1, [r4, #68]	; 0x44
   10468:	e3510000 	cmp	r1, #0
   1046c:	0a000003 	beq	10480 <_fclose_r+0xc0>
   10470:	e1a00005 	mov	r0, r5
   10474:	ebfff32a 	bl	d124 <_free_r>
   10478:	e3a03000 	mov	r3, #0
   1047c:	e5843044 	str	r3, [r4, #68]	; 0x44
   10480:	ebfff2d2 	bl	cfd0 <__sfp_lock_acquire>
   10484:	e3a03000 	mov	r3, #0
   10488:	e1c430bc 	strh	r3, [r4, #12]
   1048c:	ebfff2d0 	bl	cfd4 <__sfp_lock_release>
   10490:	eaffffd7 	b	103f4 <_fclose_r+0x34>
   10494:	ebfff271 	bl	ce60 <__sinit>
   10498:	eaffffd1 	b	103e4 <_fclose_r+0x24>
   1049c:	e1a00005 	mov	r0, r5
   104a0:	e5941010 	ldr	r1, [r4, #16]
   104a4:	ebfff31e 	bl	d124 <_free_r>
   104a8:	eaffffe4 	b	10440 <_fclose_r+0x80>

000104ac <fclose>:
   104ac:	e59f3008 	ldr	r3, [pc, #8]	; 104bc <fclose+0x10>
   104b0:	e1a01000 	mov	r1, r0
   104b4:	e5930000 	ldr	r0, [r3]
   104b8:	eaffffc0 	b	103c0 <_fclose_r>
   104bc:	0001a600 	.word	0x0001a600

000104c0 <_fputwc_r>:
   104c0:	e1d230bc 	ldrh	r3, [r2, #12]
   104c4:	e92d45f0 	push	{r4, r5, r6, r7, r8, sl, lr}
   104c8:	e3130a02 	tst	r3, #8192	; 0x2000
   104cc:	e1a04002 	mov	r4, r2
   104d0:	05922064 	ldreq	r2, [r2, #100]	; 0x64
   104d4:	03833a02 	orreq	r3, r3, #8192	; 0x2000
   104d8:	03822a02 	orreq	r2, r2, #8192	; 0x2000
   104dc:	01c430bc 	strheq	r3, [r4, #12]
   104e0:	05842064 	streq	r2, [r4, #100]	; 0x64
   104e4:	e24dd00c 	sub	sp, sp, #12
   104e8:	e1a07000 	mov	r7, r0
   104ec:	e1a06001 	mov	r6, r1
   104f0:	ebfff408 	bl	d518 <__locale_mb_cur_max>
   104f4:	e3500001 	cmp	r0, #1
   104f8:	0a000039 	beq	105e4 <_fputwc_r+0x124>
   104fc:	e28d8004 	add	r8, sp, #4
   10500:	e1a00007 	mov	r0, r7
   10504:	e1a01008 	mov	r1, r8
   10508:	e1a02006 	mov	r2, r6
   1050c:	e284305c 	add	r3, r4, #92	; 0x5c
   10510:	eb000420 	bl	11598 <_wcrtomb_r>
   10514:	e3700001 	cmn	r0, #1
   10518:	e1a0a000 	mov	sl, r0
   1051c:	0a000022 	beq	105ac <_fputwc_r+0xec>
   10520:	e3500000 	cmp	r0, #0
   10524:	0a000035 	beq	10600 <_fputwc_r+0x140>
   10528:	e3a05000 	mov	r5, #0
   1052c:	ea000008 	b	10554 <_fputwc_r+0x94>
   10530:	e5943000 	ldr	r3, [r4]
   10534:	e7d82005 	ldrb	r2, [r8, r5]
   10538:	e5c32000 	strb	r2, [r3]
   1053c:	e5943000 	ldr	r3, [r4]
   10540:	e2833001 	add	r3, r3, #1
   10544:	e5843000 	str	r3, [r4]
   10548:	e2855001 	add	r5, r5, #1
   1054c:	e155000a 	cmp	r5, sl
   10550:	2a00002a 	bcs	10600 <_fputwc_r+0x140>
   10554:	e5943008 	ldr	r3, [r4, #8]
   10558:	e2433001 	sub	r3, r3, #1
   1055c:	e3530000 	cmp	r3, #0
   10560:	e5843008 	str	r3, [r4, #8]
   10564:	aafffff1 	bge	10530 <_fputwc_r+0x70>
   10568:	e5942018 	ldr	r2, [r4, #24]
   1056c:	e1530002 	cmp	r3, r2
   10570:	ba000013 	blt	105c4 <_fputwc_r+0x104>
   10574:	e5943000 	ldr	r3, [r4]
   10578:	e7d82005 	ldrb	r2, [r8, r5]
   1057c:	e5c32000 	strb	r2, [r3]
   10580:	e5943000 	ldr	r3, [r4]
   10584:	e5d31000 	ldrb	r1, [r3]
   10588:	e351000a 	cmp	r1, #10
   1058c:	0a00001d 	beq	10608 <_fputwc_r+0x148>
   10590:	e2833001 	add	r3, r3, #1
   10594:	e5843000 	str	r3, [r4]
   10598:	e3a00000 	mov	r0, #0
   1059c:	e3500000 	cmp	r0, #0
   105a0:	0affffe8 	beq	10548 <_fputwc_r+0x88>
   105a4:	e3e00000 	mvn	r0, #0
   105a8:	ea000002 	b	105b8 <_fputwc_r+0xf8>
   105ac:	e1d430bc 	ldrh	r3, [r4, #12]
   105b0:	e3833040 	orr	r3, r3, #64	; 0x40
   105b4:	e1c430bc 	strh	r3, [r4, #12]
   105b8:	e28dd00c 	add	sp, sp, #12
   105bc:	e8bd45f0 	pop	{r4, r5, r6, r7, r8, sl, lr}
   105c0:	e12fff1e 	bx	lr
   105c4:	e7d81005 	ldrb	r1, [r8, r5]
   105c8:	e1a00007 	mov	r0, r7
   105cc:	e1a02004 	mov	r2, r4
   105d0:	eb0003a3 	bl	11464 <__swbuf_r>
   105d4:	e3700001 	cmn	r0, #1
   105d8:	13a00000 	movne	r0, #0
   105dc:	03a00001 	moveq	r0, #1
   105e0:	eaffffed 	b	1059c <_fputwc_r+0xdc>
   105e4:	e2463001 	sub	r3, r6, #1
   105e8:	e35300fe 	cmp	r3, #254	; 0xfe
   105ec:	8affffc2 	bhi	104fc <_fputwc_r+0x3c>
   105f0:	e28d8008 	add	r8, sp, #8
   105f4:	e5686004 	strb	r6, [r8, #-4]!
   105f8:	e1a0a000 	mov	sl, r0
   105fc:	eaffffc9 	b	10528 <_fputwc_r+0x68>
   10600:	e1a00006 	mov	r0, r6
   10604:	eaffffeb 	b	105b8 <_fputwc_r+0xf8>
   10608:	e1a00007 	mov	r0, r7
   1060c:	eaffffee 	b	105cc <_fputwc_r+0x10c>

00010610 <fputwc>:
   10610:	e92d4070 	push	{r4, r5, r6, lr}
   10614:	e59f403c 	ldr	r4, [pc, #60]	; 10658 <fputwc+0x48>
   10618:	e5943000 	ldr	r3, [r4]
   1061c:	e3530000 	cmp	r3, #0
   10620:	e1a06000 	mov	r6, r0
   10624:	e1a05001 	mov	r5, r1
   10628:	0a000005 	beq	10644 <fputwc+0x34>
   1062c:	e5932038 	ldr	r2, [r3, #56]	; 0x38
   10630:	e3520000 	cmp	r2, #0
   10634:	1a000002 	bne	10644 <fputwc+0x34>
   10638:	e1a00003 	mov	r0, r3
   1063c:	ebfff207 	bl	ce60 <__sinit>
   10640:	e5943000 	ldr	r3, [r4]
   10644:	e1a00003 	mov	r0, r3
   10648:	e1a01006 	mov	r1, r6
   1064c:	e1a02005 	mov	r2, r5
   10650:	e8bd4070 	pop	{r4, r5, r6, lr}
   10654:	eaffff99 	b	104c0 <_fputwc_r>
   10658:	0001a600 	.word	0x0001a600

0001065c <_fstat_r>:
   1065c:	e92d4038 	push	{r3, r4, r5, lr}
   10660:	e59f4030 	ldr	r4, [pc, #48]	; 10698 <_fstat_r+0x3c>
   10664:	e3a03000 	mov	r3, #0
   10668:	e1a05000 	mov	r5, r0
   1066c:	e1a00001 	mov	r0, r1
   10670:	e1a01002 	mov	r1, r2
   10674:	e5843000 	str	r3, [r4]
   10678:	ebffe178 	bl	8c60 <_fstat>
   1067c:	e3700001 	cmn	r0, #1
   10680:	1a000002 	bne	10690 <_fstat_r+0x34>
   10684:	e5943000 	ldr	r3, [r4]
   10688:	e3530000 	cmp	r3, #0
   1068c:	15853000 	strne	r3, [r5]
   10690:	e8bd4038 	pop	{r3, r4, r5, lr}
   10694:	e12fff1e 	bx	lr
   10698:	0001afcc 	.word	0x0001afcc

0001069c <__sfvwrite_r>:
   1069c:	e92d4ff0 	push	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
   106a0:	e5923008 	ldr	r3, [r2, #8]
   106a4:	e3530000 	cmp	r3, #0
   106a8:	e24dd00c 	sub	sp, sp, #12
   106ac:	e1a07002 	mov	r7, r2
   106b0:	e1a0a000 	mov	sl, r0
   106b4:	e1a04001 	mov	r4, r1
   106b8:	0a00001e 	beq	10738 <__sfvwrite_r+0x9c>
   106bc:	e1d130bc 	ldrh	r3, [r1, #12]
   106c0:	e3130008 	tst	r3, #8
   106c4:	0a00001f 	beq	10748 <__sfvwrite_r+0xac>
   106c8:	e5912010 	ldr	r2, [r1, #16]
   106cc:	e3520000 	cmp	r2, #0
   106d0:	0a00001c 	beq	10748 <__sfvwrite_r+0xac>
   106d4:	e2132002 	ands	r2, r3, #2
   106d8:	e5975000 	ldr	r5, [r7]
   106dc:	0a000022 	beq	1076c <__sfvwrite_r+0xd0>
   106e0:	e3a08000 	mov	r8, #0
   106e4:	e1a06008 	mov	r6, r8
   106e8:	e3560000 	cmp	r6, #0
   106ec:	e1a02008 	mov	r2, r8
   106f0:	e1a0000a 	mov	r0, sl
   106f4:	0a00004a 	beq	10824 <__sfvwrite_r+0x188>
   106f8:	e3560b01 	cmp	r6, #1024	; 0x400
   106fc:	31a03006 	movcc	r3, r6
   10700:	23a03b01 	movcs	r3, #1024	; 0x400
   10704:	e594101c 	ldr	r1, [r4, #28]
   10708:	e594c024 	ldr	ip, [r4, #36]	; 0x24
   1070c:	e1a0e00f 	mov	lr, pc
   10710:	e12fff1c 	bx	ip
   10714:	e3500000 	cmp	r0, #0
   10718:	da00004d 	ble	10854 <__sfvwrite_r+0x1b8>
   1071c:	e5973008 	ldr	r3, [r7, #8]
   10720:	e0603003 	rsb	r3, r0, r3
   10724:	e3530000 	cmp	r3, #0
   10728:	e0888000 	add	r8, r8, r0
   1072c:	e0606006 	rsb	r6, r0, r6
   10730:	e5873008 	str	r3, [r7, #8]
   10734:	1affffeb 	bne	106e8 <__sfvwrite_r+0x4c>
   10738:	e3a00000 	mov	r0, #0
   1073c:	e28dd00c 	add	sp, sp, #12
   10740:	e8bd4ff0 	pop	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
   10744:	e12fff1e 	bx	lr
   10748:	e1a0000a 	mov	r0, sl
   1074c:	e1a01004 	mov	r1, r4
   10750:	ebffe9bc 	bl	ae48 <__swsetup_r>
   10754:	e3500000 	cmp	r0, #0
   10758:	1a0000fd 	bne	10b54 <__sfvwrite_r+0x4b8>
   1075c:	e1d430bc 	ldrh	r3, [r4, #12]
   10760:	e2132002 	ands	r2, r3, #2
   10764:	e5975000 	ldr	r5, [r7]
   10768:	1affffdc 	bne	106e0 <__sfvwrite_r+0x44>
   1076c:	e2139001 	ands	r9, r3, #1
   10770:	1a00003c 	bne	10868 <__sfvwrite_r+0x1cc>
   10774:	e1a06009 	mov	r6, r9
   10778:	e3560000 	cmp	r6, #0
   1077c:	0a000024 	beq	10814 <__sfvwrite_r+0x178>
   10780:	e3130c02 	tst	r3, #512	; 0x200
   10784:	e5948008 	ldr	r8, [r4, #8]
   10788:	e1a02003 	mov	r2, r3
   1078c:	0a000066 	beq	1092c <__sfvwrite_r+0x290>
   10790:	e1560008 	cmp	r6, r8
   10794:	e1a0c008 	mov	ip, r8
   10798:	3a00008d 	bcc	109d4 <__sfvwrite_r+0x338>
   1079c:	e2022d12 	and	r2, r2, #1152	; 0x480
   107a0:	e3520000 	cmp	r2, #0
   107a4:	1a0000a6 	bne	10a44 <__sfvwrite_r+0x3a8>
   107a8:	e5940000 	ldr	r0, [r4]
   107ac:	e1a0b008 	mov	fp, r8
   107b0:	e1a03006 	mov	r3, r6
   107b4:	e1a08006 	mov	r8, r6
   107b8:	e1a0200c 	mov	r2, ip
   107bc:	e1a01009 	mov	r1, r9
   107c0:	e58d3004 	str	r3, [sp, #4]
   107c4:	e58dc000 	str	ip, [sp]
   107c8:	eb000103 	bl	10bdc <memmove>
   107cc:	e59dc000 	ldr	ip, [sp]
   107d0:	e5941008 	ldr	r1, [r4, #8]
   107d4:	e5942000 	ldr	r2, [r4]
   107d8:	e06bb001 	rsb	fp, fp, r1
   107dc:	e082c00c 	add	ip, r2, ip
   107e0:	e584b008 	str	fp, [r4, #8]
   107e4:	e584c000 	str	ip, [r4]
   107e8:	e59d3004 	ldr	r3, [sp, #4]
   107ec:	e5972008 	ldr	r2, [r7, #8]
   107f0:	e0688002 	rsb	r8, r8, r2
   107f4:	e3580000 	cmp	r8, #0
   107f8:	e0899003 	add	r9, r9, r3
   107fc:	e0636006 	rsb	r6, r3, r6
   10800:	e5878008 	str	r8, [r7, #8]
   10804:	0affffcb 	beq	10738 <__sfvwrite_r+0x9c>
   10808:	e3560000 	cmp	r6, #0
   1080c:	e1d430bc 	ldrh	r3, [r4, #12]
   10810:	1affffda 	bne	10780 <__sfvwrite_r+0xe4>
   10814:	e5959000 	ldr	r9, [r5]
   10818:	e5956004 	ldr	r6, [r5, #4]
   1081c:	e2855008 	add	r5, r5, #8
   10820:	eaffffd4 	b	10778 <__sfvwrite_r+0xdc>
   10824:	e5958000 	ldr	r8, [r5]
   10828:	e5956004 	ldr	r6, [r5, #4]
   1082c:	e2855008 	add	r5, r5, #8
   10830:	eaffffac 	b	106e8 <__sfvwrite_r+0x4c>
   10834:	eb000146 	bl	10d54 <_realloc_r>
   10838:	e2503000 	subs	r3, r0, #0
   1083c:	1a00009d 	bne	10ab8 <__sfvwrite_r+0x41c>
   10840:	e1a0000a 	mov	r0, sl
   10844:	e5941010 	ldr	r1, [r4, #16]
   10848:	ebfff235 	bl	d124 <_free_r>
   1084c:	e3a0300c 	mov	r3, #12
   10850:	e58a3000 	str	r3, [sl]
   10854:	e1d430bc 	ldrh	r3, [r4, #12]
   10858:	e3833040 	orr	r3, r3, #64	; 0x40
   1085c:	e1c430bc 	strh	r3, [r4, #12]
   10860:	e3e00000 	mvn	r0, #0
   10864:	eaffffb4 	b	1073c <__sfvwrite_r+0xa0>
   10868:	e1a09002 	mov	r9, r2
   1086c:	e1a0c002 	mov	ip, r2
   10870:	e1a0b002 	mov	fp, r2
   10874:	e1a08002 	mov	r8, r2
   10878:	e3580000 	cmp	r8, #0
   1087c:	0a000025 	beq	10918 <__sfvwrite_r+0x27c>
   10880:	e35c0000 	cmp	ip, #0
   10884:	0a000064 	beq	10a1c <__sfvwrite_r+0x380>
   10888:	e5940000 	ldr	r0, [r4]
   1088c:	e5942010 	ldr	r2, [r4, #16]
   10890:	e1590008 	cmp	r9, r8
   10894:	31a03009 	movcc	r3, r9
   10898:	21a03008 	movcs	r3, r8
   1089c:	e1500002 	cmp	r0, r2
   108a0:	e5946008 	ldr	r6, [r4, #8]
   108a4:	e594e014 	ldr	lr, [r4, #20]
   108a8:	9a000002 	bls	108b8 <__sfvwrite_r+0x21c>
   108ac:	e086600e 	add	r6, r6, lr
   108b0:	e1530006 	cmp	r3, r6
   108b4:	ca00008a 	bgt	10ae4 <__sfvwrite_r+0x448>
   108b8:	e153000e 	cmp	r3, lr
   108bc:	ba00002d 	blt	10978 <__sfvwrite_r+0x2dc>
   108c0:	e594101c 	ldr	r1, [r4, #28]
   108c4:	e1a0000a 	mov	r0, sl
   108c8:	e58dc000 	str	ip, [sp]
   108cc:	e1a0200b 	mov	r2, fp
   108d0:	e1a0300e 	mov	r3, lr
   108d4:	e594c024 	ldr	ip, [r4, #36]	; 0x24
   108d8:	e1a0e00f 	mov	lr, pc
   108dc:	e12fff1c 	bx	ip
   108e0:	e2506000 	subs	r6, r0, #0
   108e4:	e59dc000 	ldr	ip, [sp]
   108e8:	daffffd9 	ble	10854 <__sfvwrite_r+0x1b8>
   108ec:	e0599006 	subs	r9, r9, r6
   108f0:	0a000030 	beq	109b8 <__sfvwrite_r+0x31c>
   108f4:	e5973008 	ldr	r3, [r7, #8]
   108f8:	e0663003 	rsb	r3, r6, r3
   108fc:	e3530000 	cmp	r3, #0
   10900:	e08bb006 	add	fp, fp, r6
   10904:	e0668008 	rsb	r8, r6, r8
   10908:	e5873008 	str	r3, [r7, #8]
   1090c:	0affff89 	beq	10738 <__sfvwrite_r+0x9c>
   10910:	e3580000 	cmp	r8, #0
   10914:	1affffd9 	bne	10880 <__sfvwrite_r+0x1e4>
   10918:	e595b000 	ldr	fp, [r5]
   1091c:	e5958004 	ldr	r8, [r5, #4]
   10920:	e3a0c000 	mov	ip, #0
   10924:	e2855008 	add	r5, r5, #8
   10928:	eaffffd2 	b	10878 <__sfvwrite_r+0x1dc>
   1092c:	e5940000 	ldr	r0, [r4]
   10930:	e5943010 	ldr	r3, [r4, #16]
   10934:	e1500003 	cmp	r0, r3
   10938:	9a000001 	bls	10944 <__sfvwrite_r+0x2a8>
   1093c:	e1560008 	cmp	r6, r8
   10940:	8a000075 	bhi	10b1c <__sfvwrite_r+0x480>
   10944:	e5943014 	ldr	r3, [r4, #20]
   10948:	e1560003 	cmp	r6, r3
   1094c:	3a000026 	bcc	109ec <__sfvwrite_r+0x350>
   10950:	e1a0000a 	mov	r0, sl
   10954:	e594101c 	ldr	r1, [r4, #28]
   10958:	e1a02009 	mov	r2, r9
   1095c:	e594c024 	ldr	ip, [r4, #36]	; 0x24
   10960:	e1a0e00f 	mov	lr, pc
   10964:	e12fff1c 	bx	ip
   10968:	e2508000 	subs	r8, r0, #0
   1096c:	daffffb8 	ble	10854 <__sfvwrite_r+0x1b8>
   10970:	e1a03008 	mov	r3, r8
   10974:	eaffff9c 	b	107ec <__sfvwrite_r+0x150>
   10978:	e1a02003 	mov	r2, r3
   1097c:	e1a0100b 	mov	r1, fp
   10980:	e58d3004 	str	r3, [sp, #4]
   10984:	e58dc000 	str	ip, [sp]
   10988:	eb000093 	bl	10bdc <memmove>
   1098c:	e59d3004 	ldr	r3, [sp, #4]
   10990:	e5941008 	ldr	r1, [r4, #8]
   10994:	e5942000 	ldr	r2, [r4]
   10998:	e1a06003 	mov	r6, r3
   1099c:	e0631001 	rsb	r1, r3, r1
   109a0:	e0822003 	add	r2, r2, r3
   109a4:	e0599006 	subs	r9, r9, r6
   109a8:	e5841008 	str	r1, [r4, #8]
   109ac:	e5842000 	str	r2, [r4]
   109b0:	e59dc000 	ldr	ip, [sp]
   109b4:	1affffce 	bne	108f4 <__sfvwrite_r+0x258>
   109b8:	e1a0000a 	mov	r0, sl
   109bc:	e1a01004 	mov	r1, r4
   109c0:	ebfff0c2 	bl	ccd0 <_fflush_r>
   109c4:	e3500000 	cmp	r0, #0
   109c8:	1affffa1 	bne	10854 <__sfvwrite_r+0x1b8>
   109cc:	e1a0c009 	mov	ip, r9
   109d0:	eaffffc7 	b	108f4 <__sfvwrite_r+0x258>
   109d4:	e5940000 	ldr	r0, [r4]
   109d8:	e1a0b006 	mov	fp, r6
   109dc:	e1a03006 	mov	r3, r6
   109e0:	e1a08006 	mov	r8, r6
   109e4:	e1a0c006 	mov	ip, r6
   109e8:	eaffff72 	b	107b8 <__sfvwrite_r+0x11c>
   109ec:	e1a02006 	mov	r2, r6
   109f0:	e1a01009 	mov	r1, r9
   109f4:	eb000078 	bl	10bdc <memmove>
   109f8:	e5942008 	ldr	r2, [r4, #8]
   109fc:	e5943000 	ldr	r3, [r4]
   10a00:	e0662002 	rsb	r2, r6, r2
   10a04:	e0833006 	add	r3, r3, r6
   10a08:	e5843000 	str	r3, [r4]
   10a0c:	e5842008 	str	r2, [r4, #8]
   10a10:	e1a08006 	mov	r8, r6
   10a14:	e1a03006 	mov	r3, r6
   10a18:	eaffff73 	b	107ec <__sfvwrite_r+0x150>
   10a1c:	e1a0000b 	mov	r0, fp
   10a20:	e3a0100a 	mov	r1, #10
   10a24:	e1a02008 	mov	r2, r8
   10a28:	ebfff4ee 	bl	dde8 <memchr>
   10a2c:	e3500000 	cmp	r0, #0
   10a30:	12809001 	addne	r9, r0, #1
   10a34:	106b9009 	rsbne	r9, fp, r9
   10a38:	02889001 	addeq	r9, r8, #1
   10a3c:	e3a0c001 	mov	ip, #1
   10a40:	eaffff90 	b	10888 <__sfvwrite_r+0x1ec>
   10a44:	e5941010 	ldr	r1, [r4, #16]
   10a48:	e594b000 	ldr	fp, [r4]
   10a4c:	e5948014 	ldr	r8, [r4, #20]
   10a50:	e061b00b 	rsb	fp, r1, fp
   10a54:	e0888088 	add	r8, r8, r8, lsl #1
   10a58:	e0888fa8 	add	r8, r8, r8, lsr #31
   10a5c:	e28b0001 	add	r0, fp, #1
   10a60:	e1a080c8 	asr	r8, r8, #1
   10a64:	e0800006 	add	r0, r0, r6
   10a68:	e1580000 	cmp	r8, r0
   10a6c:	e1a02008 	mov	r2, r8
   10a70:	31a08000 	movcc	r8, r0
   10a74:	31a02008 	movcc	r2, r8
   10a78:	e3130b01 	tst	r3, #1024	; 0x400
   10a7c:	e1a0000a 	mov	r0, sl
   10a80:	0affff6b 	beq	10834 <__sfvwrite_r+0x198>
   10a84:	e1a01002 	mov	r1, r2
   10a88:	ebfff30f 	bl	d6cc <_malloc_r>
   10a8c:	e2503000 	subs	r3, r0, #0
   10a90:	0affff6d 	beq	1084c <__sfvwrite_r+0x1b0>
   10a94:	e5941010 	ldr	r1, [r4, #16]
   10a98:	e1a0200b 	mov	r2, fp
   10a9c:	e58d3004 	str	r3, [sp, #4]
   10aa0:	ebfff517 	bl	df04 <memcpy>
   10aa4:	e1d420bc 	ldrh	r2, [r4, #12]
   10aa8:	e3c22d12 	bic	r2, r2, #1152	; 0x480
   10aac:	e3822080 	orr	r2, r2, #128	; 0x80
   10ab0:	e1c420bc 	strh	r2, [r4, #12]
   10ab4:	e59d3004 	ldr	r3, [sp, #4]
   10ab8:	e083000b 	add	r0, r3, fp
   10abc:	e06b2008 	rsb	r2, fp, r8
   10ac0:	e5843010 	str	r3, [r4, #16]
   10ac4:	e5848014 	str	r8, [r4, #20]
   10ac8:	e5840000 	str	r0, [r4]
   10acc:	e1a0b006 	mov	fp, r6
   10ad0:	e5842008 	str	r2, [r4, #8]
   10ad4:	e1a03006 	mov	r3, r6
   10ad8:	e1a08006 	mov	r8, r6
   10adc:	e1a0c006 	mov	ip, r6
   10ae0:	eaffff34 	b	107b8 <__sfvwrite_r+0x11c>
   10ae4:	e1a0100b 	mov	r1, fp
   10ae8:	e1a02006 	mov	r2, r6
   10aec:	e58dc000 	str	ip, [sp]
   10af0:	eb000039 	bl	10bdc <memmove>
   10af4:	e5943000 	ldr	r3, [r4]
   10af8:	e0833006 	add	r3, r3, r6
   10afc:	e5843000 	str	r3, [r4]
   10b00:	e1a0000a 	mov	r0, sl
   10b04:	e1a01004 	mov	r1, r4
   10b08:	ebfff070 	bl	ccd0 <_fflush_r>
   10b0c:	e3500000 	cmp	r0, #0
   10b10:	e59dc000 	ldr	ip, [sp]
   10b14:	0affff74 	beq	108ec <__sfvwrite_r+0x250>
   10b18:	eaffff4d 	b	10854 <__sfvwrite_r+0x1b8>
   10b1c:	e1a01009 	mov	r1, r9
   10b20:	e1a02008 	mov	r2, r8
   10b24:	e58d8004 	str	r8, [sp, #4]
   10b28:	eb00002b 	bl	10bdc <memmove>
   10b2c:	e5942000 	ldr	r2, [r4]
   10b30:	e0822008 	add	r2, r2, r8
   10b34:	e5842000 	str	r2, [r4]
   10b38:	e1a0000a 	mov	r0, sl
   10b3c:	e1a01004 	mov	r1, r4
   10b40:	ebfff062 	bl	ccd0 <_fflush_r>
   10b44:	e3500000 	cmp	r0, #0
   10b48:	e59d3004 	ldr	r3, [sp, #4]
   10b4c:	0affff26 	beq	107ec <__sfvwrite_r+0x150>
   10b50:	eaffff3f 	b	10854 <__sfvwrite_r+0x1b8>
   10b54:	e3e00000 	mvn	r0, #0
   10b58:	eafffef7 	b	1073c <__sfvwrite_r+0xa0>

00010b5c <_isatty_r>:
   10b5c:	e92d4038 	push	{r3, r4, r5, lr}
   10b60:	e59f402c 	ldr	r4, [pc, #44]	; 10b94 <_isatty_r+0x38>
   10b64:	e3a03000 	mov	r3, #0
   10b68:	e1a05000 	mov	r5, r0
   10b6c:	e1a00001 	mov	r0, r1
   10b70:	e5843000 	str	r3, [r4]
   10b74:	ebffe097 	bl	8dd8 <_isatty>
   10b78:	e3700001 	cmn	r0, #1
   10b7c:	1a000002 	bne	10b8c <_isatty_r+0x30>
   10b80:	e5943000 	ldr	r3, [r4]
   10b84:	e3530000 	cmp	r3, #0
   10b88:	15853000 	strne	r3, [r5]
   10b8c:	e8bd4038 	pop	{r3, r4, r5, lr}
   10b90:	e12fff1e 	bx	lr
   10b94:	0001afcc 	.word	0x0001afcc

00010b98 <_lseek_r>:
   10b98:	e92d4038 	push	{r3, r4, r5, lr}
   10b9c:	e59f4034 	ldr	r4, [pc, #52]	; 10bd8 <_lseek_r+0x40>
   10ba0:	e1a05000 	mov	r5, r0
   10ba4:	e1a00001 	mov	r0, r1
   10ba8:	e1a01002 	mov	r1, r2
   10bac:	e1a02003 	mov	r2, r3
   10bb0:	e3a03000 	mov	r3, #0
   10bb4:	e5843000 	str	r3, [r4]
   10bb8:	ebffdf51 	bl	8904 <_lseek>
   10bbc:	e3700001 	cmn	r0, #1
   10bc0:	1a000002 	bne	10bd0 <_lseek_r+0x38>
   10bc4:	e5943000 	ldr	r3, [r4]
   10bc8:	e3530000 	cmp	r3, #0
   10bcc:	15853000 	strne	r3, [r5]
   10bd0:	e8bd4038 	pop	{r3, r4, r5, lr}
   10bd4:	e12fff1e 	bx	lr
   10bd8:	0001afcc 	.word	0x0001afcc

00010bdc <memmove>:
   10bdc:	e1500001 	cmp	r0, r1
   10be0:	e92d00f0 	push	{r4, r5, r6, r7}
   10be4:	9a00000e 	bls	10c24 <memmove+0x48>
   10be8:	e081c002 	add	ip, r1, r2
   10bec:	e150000c 	cmp	r0, ip
   10bf0:	2a00000b 	bcs	10c24 <memmove+0x48>
   10bf4:	e3520000 	cmp	r2, #0
   10bf8:	e0803002 	add	r3, r0, r2
   10bfc:	e2422001 	sub	r2, r2, #1
   10c00:	0a000005 	beq	10c1c <memmove+0x40>
   10c04:	e1a0100c 	mov	r1, ip
   10c08:	e571c001 	ldrb	ip, [r1, #-1]!
   10c0c:	e2422001 	sub	r2, r2, #1
   10c10:	e3720001 	cmn	r2, #1
   10c14:	e563c001 	strb	ip, [r3, #-1]!
   10c18:	1afffffa 	bne	10c08 <memmove+0x2c>
   10c1c:	e8bd00f0 	pop	{r4, r5, r6, r7}
   10c20:	e12fff1e 	bx	lr
   10c24:	e352000f 	cmp	r2, #15
   10c28:	8a000009 	bhi	10c54 <memmove+0x78>
   10c2c:	e1a03000 	mov	r3, r0
   10c30:	e3520000 	cmp	r2, #0
   10c34:	0afffff8 	beq	10c1c <memmove+0x40>
   10c38:	e2411001 	sub	r1, r1, #1
   10c3c:	e0832002 	add	r2, r3, r2
   10c40:	e5f1c001 	ldrb	ip, [r1, #1]!
   10c44:	e4c3c001 	strb	ip, [r3], #1
   10c48:	e1530002 	cmp	r3, r2
   10c4c:	1afffffb 	bne	10c40 <memmove+0x64>
   10c50:	eafffff1 	b	10c1c <memmove+0x40>
   10c54:	e1813000 	orr	r3, r1, r0
   10c58:	e3130003 	tst	r3, #3
   10c5c:	1a000027 	bne	10d00 <memmove+0x124>
   10c60:	e1a0c001 	mov	ip, r1
   10c64:	e1a03000 	mov	r3, r0
   10c68:	e1a04002 	mov	r4, r2
   10c6c:	e59c5000 	ldr	r5, [ip]
   10c70:	e5835000 	str	r5, [r3]
   10c74:	e59c5004 	ldr	r5, [ip, #4]
   10c78:	e5835004 	str	r5, [r3, #4]
   10c7c:	e59c5008 	ldr	r5, [ip, #8]
   10c80:	e5835008 	str	r5, [r3, #8]
   10c84:	e2444010 	sub	r4, r4, #16
   10c88:	e59c500c 	ldr	r5, [ip, #12]
   10c8c:	e354000f 	cmp	r4, #15
   10c90:	e583500c 	str	r5, [r3, #12]
   10c94:	e28cc010 	add	ip, ip, #16
   10c98:	e2833010 	add	r3, r3, #16
   10c9c:	8afffff2 	bhi	10c6c <memmove+0x90>
   10ca0:	e2424010 	sub	r4, r2, #16
   10ca4:	e3c4400f 	bic	r4, r4, #15
   10ca8:	e202c00f 	and	ip, r2, #15
   10cac:	e2844010 	add	r4, r4, #16
   10cb0:	e35c0003 	cmp	ip, #3
   10cb4:	e0803004 	add	r3, r0, r4
   10cb8:	e0811004 	add	r1, r1, r4
   10cbc:	9a000011 	bls	10d08 <memmove+0x12c>
   10cc0:	e24cc004 	sub	ip, ip, #4
   10cc4:	e1a0712c 	lsr	r7, ip, #2
   10cc8:	e0816107 	add	r6, r1, r7, lsl #2
   10ccc:	e1a0c001 	mov	ip, r1
   10cd0:	e2866004 	add	r6, r6, #4
   10cd4:	e1a04003 	mov	r4, r3
   10cd8:	e49c5004 	ldr	r5, [ip], #4
   10cdc:	e15c0006 	cmp	ip, r6
   10ce0:	e4845004 	str	r5, [r4], #4
   10ce4:	1afffffb 	bne	10cd8 <memmove+0xfc>
   10ce8:	e287c001 	add	ip, r7, #1
   10cec:	e1a0c10c 	lsl	ip, ip, #2
   10cf0:	e2022003 	and	r2, r2, #3
   10cf4:	e081100c 	add	r1, r1, ip
   10cf8:	e083300c 	add	r3, r3, ip
   10cfc:	eaffffcb 	b	10c30 <memmove+0x54>
   10d00:	e1a03000 	mov	r3, r0
   10d04:	eaffffcb 	b	10c38 <memmove+0x5c>
   10d08:	e1a0200c 	mov	r2, ip
   10d0c:	eaffffc7 	b	10c30 <memmove+0x54>

00010d10 <_read_r>:
   10d10:	e92d4038 	push	{r3, r4, r5, lr}
   10d14:	e59f4034 	ldr	r4, [pc, #52]	; 10d50 <_read_r+0x40>
   10d18:	e1a05000 	mov	r5, r0
   10d1c:	e1a00001 	mov	r0, r1
   10d20:	e1a01002 	mov	r1, r2
   10d24:	e1a02003 	mov	r2, r3
   10d28:	e3a03000 	mov	r3, #0
   10d2c:	e5843000 	str	r3, [r4]
   10d30:	ebffde8a 	bl	8760 <_read>
   10d34:	e3700001 	cmn	r0, #1
   10d38:	1a000002 	bne	10d48 <_read_r+0x38>
   10d3c:	e5943000 	ldr	r3, [r4]
   10d40:	e3530000 	cmp	r3, #0
   10d44:	15853000 	strne	r3, [r5]
   10d48:	e8bd4038 	pop	{r3, r4, r5, lr}
   10d4c:	e12fff1e 	bx	lr
   10d50:	0001afcc 	.word	0x0001afcc

00010d54 <_realloc_r>:
   10d54:	e92d4ff0 	push	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
   10d58:	e2514000 	subs	r4, r1, #0
   10d5c:	e24dd00c 	sub	sp, sp, #12
   10d60:	e1a08000 	mov	r8, r0
   10d64:	e1a0b002 	mov	fp, r2
   10d68:	0a0000d1 	beq	110b4 <_realloc_r+0x360>
   10d6c:	ebfff4a1 	bl	dff8 <__malloc_lock>
   10d70:	e28b500b 	add	r5, fp, #11
   10d74:	e3550016 	cmp	r5, #22
   10d78:	83c55007 	bichi	r5, r5, #7
   10d7c:	93a01010 	movls	r1, #16
   10d80:	81a06fa5 	lsrhi	r6, r5, #31
   10d84:	93a06000 	movls	r6, #0
   10d88:	91a05001 	movls	r5, r1
   10d8c:	e5143004 	ldr	r3, [r4, #-4]
   10d90:	81a01005 	movhi	r1, r5
   10d94:	e155000b 	cmp	r5, fp
   10d98:	33866001 	orrcc	r6, r6, #1
   10d9c:	e3560000 	cmp	r6, #0
   10da0:	e3c37003 	bic	r7, r3, #3
   10da4:	13a0300c 	movne	r3, #12
   10da8:	e244a008 	sub	sl, r4, #8
   10dac:	15883000 	strne	r3, [r8]
   10db0:	13a06000 	movne	r6, #0
   10db4:	1a00000f 	bne	10df8 <_realloc_r+0xa4>
   10db8:	e1570001 	cmp	r7, r1
   10dbc:	ba000011 	blt	10e08 <_realloc_r+0xb4>
   10dc0:	e1a06007 	mov	r6, r7
   10dc4:	e0652006 	rsb	r2, r5, r6
   10dc8:	e352000f 	cmp	r2, #15
   10dcc:	e2033001 	and	r3, r3, #1
   10dd0:	8a00005e 	bhi	10f50 <_realloc_r+0x1fc>
   10dd4:	e1833006 	orr	r3, r3, r6
   10dd8:	e58a3004 	str	r3, [sl, #4]
   10ddc:	e08aa006 	add	sl, sl, r6
   10de0:	e59a3004 	ldr	r3, [sl, #4]
   10de4:	e3833001 	orr	r3, r3, #1
   10de8:	e58a3004 	str	r3, [sl, #4]
   10dec:	e1a00008 	mov	r0, r8
   10df0:	ebfff481 	bl	dffc <__malloc_unlock>
   10df4:	e1a06004 	mov	r6, r4
   10df8:	e1a00006 	mov	r0, r6
   10dfc:	e28dd00c 	add	sp, sp, #12
   10e00:	e8bd4ff0 	pop	{r4, r5, r6, r7, r8, r9, sl, fp, lr}
   10e04:	e12fff1e 	bx	lr
   10e08:	e59f24b4 	ldr	r2, [pc, #1204]	; 112c4 <_realloc_r+0x570>
   10e0c:	e592c008 	ldr	ip, [r2, #8]
   10e10:	e08a0007 	add	r0, sl, r7
   10e14:	e15c0000 	cmp	ip, r0
   10e18:	0a0000a9 	beq	110c4 <_realloc_r+0x370>
   10e1c:	e590e004 	ldr	lr, [r0, #4]
   10e20:	e3ce9001 	bic	r9, lr, #1
   10e24:	e0809009 	add	r9, r0, r9
   10e28:	e5999004 	ldr	r9, [r9, #4]
   10e2c:	e3190001 	tst	r9, #1
   10e30:	11a0e006 	movne	lr, r6
   10e34:	11a0000e 	movne	r0, lr
   10e38:	0a00003b 	beq	10f2c <_realloc_r+0x1d8>
   10e3c:	e3130001 	tst	r3, #1
   10e40:	1a00004f 	bne	10f84 <_realloc_r+0x230>
   10e44:	e5149008 	ldr	r9, [r4, #-8]
   10e48:	e069900a 	rsb	r9, r9, sl
   10e4c:	e5993004 	ldr	r3, [r9, #4]
   10e50:	e3500000 	cmp	r0, #0
   10e54:	e3c33003 	bic	r3, r3, #3
   10e58:	0a000067 	beq	10ffc <_realloc_r+0x2a8>
   10e5c:	e150000c 	cmp	r0, ip
   10e60:	e083c007 	add	ip, r3, r7
   10e64:	0a0000ce 	beq	111a4 <_realloc_r+0x450>
   10e68:	e08e600c 	add	r6, lr, ip
   10e6c:	e1560001 	cmp	r6, r1
   10e70:	ba000062 	blt	11000 <_realloc_r+0x2ac>
   10e74:	e2802008 	add	r2, r0, #8
   10e78:	e892000c 	ldm	r2, {r2, r3}
   10e7c:	e5832008 	str	r2, [r3, #8]
   10e80:	e582300c 	str	r3, [r2, #12]
   10e84:	e1a0a009 	mov	sl, r9
   10e88:	e599300c 	ldr	r3, [r9, #12]
   10e8c:	e5ba1008 	ldr	r1, [sl, #8]!
   10e90:	e2472004 	sub	r2, r7, #4
   10e94:	e3520024 	cmp	r2, #36	; 0x24
   10e98:	e581300c 	str	r3, [r1, #12]
   10e9c:	e5831008 	str	r1, [r3, #8]
   10ea0:	8a0000fa 	bhi	11290 <_realloc_r+0x53c>
   10ea4:	e3520013 	cmp	r2, #19
   10ea8:	91a0300a 	movls	r3, sl
   10eac:	9a000014 	bls	10f04 <_realloc_r+0x1b0>
   10eb0:	e5943000 	ldr	r3, [r4]
   10eb4:	e5893008 	str	r3, [r9, #8]
   10eb8:	e5943004 	ldr	r3, [r4, #4]
   10ebc:	e352001b 	cmp	r2, #27
   10ec0:	e589300c 	str	r3, [r9, #12]
   10ec4:	92844008 	addls	r4, r4, #8
   10ec8:	92893010 	addls	r3, r9, #16
   10ecc:	9a00000c 	bls	10f04 <_realloc_r+0x1b0>
   10ed0:	e5943008 	ldr	r3, [r4, #8]
   10ed4:	e5893010 	str	r3, [r9, #16]
   10ed8:	e594300c 	ldr	r3, [r4, #12]
   10edc:	e5893014 	str	r3, [r9, #20]
   10ee0:	e3520024 	cmp	r2, #36	; 0x24
   10ee4:	05943010 	ldreq	r3, [r4, #16]
   10ee8:	05893018 	streq	r3, [r9, #24]
   10eec:	05942014 	ldreq	r2, [r4, #20]
   10ef0:	0589201c 	streq	r2, [r9, #28]
   10ef4:	12893018 	addne	r3, r9, #24
   10ef8:	12844010 	addne	r4, r4, #16
   10efc:	02893020 	addeq	r3, r9, #32
   10f00:	02844018 	addeq	r4, r4, #24
   10f04:	e5942000 	ldr	r2, [r4]
   10f08:	e5832000 	str	r2, [r3]
   10f0c:	e5942004 	ldr	r2, [r4, #4]
   10f10:	e5832004 	str	r2, [r3, #4]
   10f14:	e5942008 	ldr	r2, [r4, #8]
   10f18:	e5832008 	str	r2, [r3, #8]
   10f1c:	e1a0400a 	mov	r4, sl
   10f20:	e5993004 	ldr	r3, [r9, #4]
   10f24:	e1a0a009 	mov	sl, r9
   10f28:	eaffffa5 	b	10dc4 <_realloc_r+0x70>
   10f2c:	e3cee003 	bic	lr, lr, #3
   10f30:	e08e6007 	add	r6, lr, r7
   10f34:	e1560001 	cmp	r6, r1
   10f38:	baffffbf 	blt	10e3c <_realloc_r+0xe8>
   10f3c:	e2801008 	add	r1, r0, #8
   10f40:	e8910006 	ldm	r1, {r1, r2}
   10f44:	e581200c 	str	r2, [r1, #12]
   10f48:	e5821008 	str	r1, [r2, #8]
   10f4c:	eaffff9c 	b	10dc4 <_realloc_r+0x70>
   10f50:	e08a1005 	add	r1, sl, r5
   10f54:	e3820001 	orr	r0, r2, #1
   10f58:	e1835005 	orr	r5, r3, r5
   10f5c:	e58a5004 	str	r5, [sl, #4]
   10f60:	e5810004 	str	r0, [r1, #4]
   10f64:	e0812002 	add	r2, r1, r2
   10f68:	e5923004 	ldr	r3, [r2, #4]
   10f6c:	e3833001 	orr	r3, r3, #1
   10f70:	e5823004 	str	r3, [r2, #4]
   10f74:	e1a00008 	mov	r0, r8
   10f78:	e2811008 	add	r1, r1, #8
   10f7c:	ebfff068 	bl	d124 <_free_r>
   10f80:	eaffff99 	b	10dec <_realloc_r+0x98>
   10f84:	e1a00008 	mov	r0, r8
   10f88:	e1a0100b 	mov	r1, fp
   10f8c:	ebfff1ce 	bl	d6cc <_malloc_r>
   10f90:	e2506000 	subs	r6, r0, #0
   10f94:	0a000015 	beq	10ff0 <_realloc_r+0x29c>
   10f98:	e5143004 	ldr	r3, [r4, #-4]
   10f9c:	e3c32001 	bic	r2, r3, #1
   10fa0:	e2461008 	sub	r1, r6, #8
   10fa4:	e08a2002 	add	r2, sl, r2
   10fa8:	e1510002 	cmp	r1, r2
   10fac:	0a0000b3 	beq	11280 <_realloc_r+0x52c>
   10fb0:	e2472004 	sub	r2, r7, #4
   10fb4:	e3520024 	cmp	r2, #36	; 0x24
   10fb8:	8a000076 	bhi	11198 <_realloc_r+0x444>
   10fbc:	e3520013 	cmp	r2, #19
   10fc0:	91a03006 	movls	r3, r6
   10fc4:	91a02004 	movls	r2, r4
   10fc8:	8a000052 	bhi	11118 <_realloc_r+0x3c4>
   10fcc:	e5921000 	ldr	r1, [r2]
   10fd0:	e5831000 	str	r1, [r3]
   10fd4:	e5921004 	ldr	r1, [r2, #4]
   10fd8:	e5831004 	str	r1, [r3, #4]
   10fdc:	e5922008 	ldr	r2, [r2, #8]
   10fe0:	e5832008 	str	r2, [r3, #8]
   10fe4:	e1a00008 	mov	r0, r8
   10fe8:	e1a01004 	mov	r1, r4
   10fec:	ebfff04c 	bl	d124 <_free_r>
   10ff0:	e1a00008 	mov	r0, r8
   10ff4:	ebfff400 	bl	dffc <__malloc_unlock>
   10ff8:	eaffff7e 	b	10df8 <_realloc_r+0xa4>
   10ffc:	e083c007 	add	ip, r3, r7
   11000:	e15c0001 	cmp	ip, r1
   11004:	baffffde 	blt	10f84 <_realloc_r+0x230>
   11008:	e1a06009 	mov	r6, r9
   1100c:	e599300c 	ldr	r3, [r9, #12]
   11010:	e5b61008 	ldr	r1, [r6, #8]!
   11014:	e2472004 	sub	r2, r7, #4
   11018:	e3520024 	cmp	r2, #36	; 0x24
   1101c:	e581300c 	str	r3, [r1, #12]
   11020:	e5831008 	str	r1, [r3, #8]
   11024:	8a000051 	bhi	11170 <_realloc_r+0x41c>
   11028:	e3520013 	cmp	r2, #19
   1102c:	91a03006 	movls	r3, r6
   11030:	9a000014 	bls	11088 <_realloc_r+0x334>
   11034:	e5943000 	ldr	r3, [r4]
   11038:	e5893008 	str	r3, [r9, #8]
   1103c:	e5943004 	ldr	r3, [r4, #4]
   11040:	e352001b 	cmp	r2, #27
   11044:	e589300c 	str	r3, [r9, #12]
   11048:	92844008 	addls	r4, r4, #8
   1104c:	92893010 	addls	r3, r9, #16
   11050:	9a00000c 	bls	11088 <_realloc_r+0x334>
   11054:	e5943008 	ldr	r3, [r4, #8]
   11058:	e5893010 	str	r3, [r9, #16]
   1105c:	e594300c 	ldr	r3, [r4, #12]
   11060:	e5893014 	str	r3, [r9, #20]
   11064:	e3520024 	cmp	r2, #36	; 0x24
   11068:	05943010 	ldreq	r3, [r4, #16]
   1106c:	05893018 	streq	r3, [r9, #24]
   11070:	05942014 	ldreq	r2, [r4, #20]
   11074:	0589201c 	streq	r2, [r9, #28]
   11078:	12893018 	addne	r3, r9, #24
   1107c:	12844010 	addne	r4, r4, #16
   11080:	02893020 	addeq	r3, r9, #32
   11084:	02844018 	addeq	r4, r4, #24
   11088:	e5942000 	ldr	r2, [r4]
   1108c:	e5832000 	str	r2, [r3]
   11090:	e5942004 	ldr	r2, [r4, #4]
   11094:	e5832004 	str	r2, [r3, #4]
   11098:	e5942008 	ldr	r2, [r4, #8]
   1109c:	e5832008 	str	r2, [r3, #8]
   110a0:	e1a04006 	mov	r4, r6
   110a4:	e5993004 	ldr	r3, [r9, #4]
   110a8:	e1a0600c 	mov	r6, ip
   110ac:	e1a0a009 	mov	sl, r9
   110b0:	eaffff43 	b	10dc4 <_realloc_r+0x70>
   110b4:	e1a01002 	mov	r1, r2
   110b8:	ebfff183 	bl	d6cc <_malloc_r>
   110bc:	e1a06000 	mov	r6, r0
   110c0:	eaffff4c 	b	10df8 <_realloc_r+0xa4>
   110c4:	e59ce004 	ldr	lr, [ip, #4]
   110c8:	e3cee003 	bic	lr, lr, #3
   110cc:	e08e0007 	add	r0, lr, r7
   110d0:	e2856010 	add	r6, r5, #16
   110d4:	e1500006 	cmp	r0, r6
   110d8:	b1a0000c 	movlt	r0, ip
   110dc:	baffff56 	blt	10e3c <_realloc_r+0xe8>
   110e0:	e0653000 	rsb	r3, r5, r0
   110e4:	e08aa005 	add	sl, sl, r5
   110e8:	e3833001 	orr	r3, r3, #1
   110ec:	e58a3004 	str	r3, [sl, #4]
   110f0:	e5143004 	ldr	r3, [r4, #-4]
   110f4:	e2033001 	and	r3, r3, #1
   110f8:	e1855003 	orr	r5, r5, r3
   110fc:	e59f31c0 	ldr	r3, [pc, #448]	; 112c4 <_realloc_r+0x570>
   11100:	e5045004 	str	r5, [r4, #-4]
   11104:	e1a00008 	mov	r0, r8
   11108:	e583a008 	str	sl, [r3, #8]
   1110c:	e1a06004 	mov	r6, r4
   11110:	ebfff3b9 	bl	dffc <__malloc_unlock>
   11114:	eaffff37 	b	10df8 <_realloc_r+0xa4>
   11118:	e5943000 	ldr	r3, [r4]
   1111c:	e5863000 	str	r3, [r6]
   11120:	e5943004 	ldr	r3, [r4, #4]
   11124:	e352001b 	cmp	r2, #27
   11128:	e5863004 	str	r3, [r6, #4]
   1112c:	92842008 	addls	r2, r4, #8
   11130:	92863008 	addls	r3, r6, #8
   11134:	9affffa4 	bls	10fcc <_realloc_r+0x278>
   11138:	e5943008 	ldr	r3, [r4, #8]
   1113c:	e5863008 	str	r3, [r6, #8]
   11140:	e594300c 	ldr	r3, [r4, #12]
   11144:	e586300c 	str	r3, [r6, #12]
   11148:	e3520024 	cmp	r2, #36	; 0x24
   1114c:	05943010 	ldreq	r3, [r4, #16]
   11150:	05863010 	streq	r3, [r6, #16]
   11154:	05942014 	ldreq	r2, [r4, #20]
   11158:	12863010 	addne	r3, r6, #16
   1115c:	05862014 	streq	r2, [r6, #20]
   11160:	12842010 	addne	r2, r4, #16
   11164:	02863018 	addeq	r3, r6, #24
   11168:	02842018 	addeq	r2, r4, #24
   1116c:	eaffff96 	b	10fcc <_realloc_r+0x278>
   11170:	e1a01004 	mov	r1, r4
   11174:	e1a00006 	mov	r0, r6
   11178:	e58dc004 	str	ip, [sp, #4]
   1117c:	ebfffe96 	bl	10bdc <memmove>
   11180:	e59dc004 	ldr	ip, [sp, #4]
   11184:	e1a04006 	mov	r4, r6
   11188:	e5993004 	ldr	r3, [r9, #4]
   1118c:	e1a0600c 	mov	r6, ip
   11190:	e1a0a009 	mov	sl, r9
   11194:	eaffff0a 	b	10dc4 <_realloc_r+0x70>
   11198:	e1a01004 	mov	r1, r4
   1119c:	ebfffe8e 	bl	10bdc <memmove>
   111a0:	eaffff8f 	b	10fe4 <_realloc_r+0x290>
   111a4:	e08e300c 	add	r3, lr, ip
   111a8:	e2850010 	add	r0, r5, #16
   111ac:	e1530000 	cmp	r3, r0
   111b0:	baffff92 	blt	11000 <_realloc_r+0x2ac>
   111b4:	e1a06009 	mov	r6, r9
   111b8:	e599100c 	ldr	r1, [r9, #12]
   111bc:	e5b60008 	ldr	r0, [r6, #8]!
   111c0:	e2472004 	sub	r2, r7, #4
   111c4:	e3520024 	cmp	r2, #36	; 0x24
   111c8:	e580100c 	str	r1, [r0, #12]
   111cc:	e5810008 	str	r0, [r1, #8]
   111d0:	8a000035 	bhi	112ac <_realloc_r+0x558>
   111d4:	e3520013 	cmp	r2, #19
   111d8:	91a02006 	movls	r2, r6
   111dc:	9a000014 	bls	11234 <_realloc_r+0x4e0>
   111e0:	e5941000 	ldr	r1, [r4]
   111e4:	e5891008 	str	r1, [r9, #8]
   111e8:	e5941004 	ldr	r1, [r4, #4]
   111ec:	e352001b 	cmp	r2, #27
   111f0:	e589100c 	str	r1, [r9, #12]
   111f4:	92892010 	addls	r2, r9, #16
   111f8:	92844008 	addls	r4, r4, #8
   111fc:	9a00000c 	bls	11234 <_realloc_r+0x4e0>
   11200:	e5941008 	ldr	r1, [r4, #8]
   11204:	e5891010 	str	r1, [r9, #16]
   11208:	e594100c 	ldr	r1, [r4, #12]
   1120c:	e5891014 	str	r1, [r9, #20]
   11210:	e3520024 	cmp	r2, #36	; 0x24
   11214:	05942010 	ldreq	r2, [r4, #16]
   11218:	05892018 	streq	r2, [r9, #24]
   1121c:	05941014 	ldreq	r1, [r4, #20]
   11220:	0589101c 	streq	r1, [r9, #28]
   11224:	12892018 	addne	r2, r9, #24
   11228:	12844010 	addne	r4, r4, #16
   1122c:	02892020 	addeq	r2, r9, #32
   11230:	02844018 	addeq	r4, r4, #24
   11234:	e5941000 	ldr	r1, [r4]
   11238:	e5821000 	str	r1, [r2]
   1123c:	e5941004 	ldr	r1, [r4, #4]
   11240:	e5821004 	str	r1, [r2, #4]
   11244:	e5941008 	ldr	r1, [r4, #8]
   11248:	e5821008 	str	r1, [r2, #8]
   1124c:	e0653003 	rsb	r3, r5, r3
   11250:	e0892005 	add	r2, r9, r5
   11254:	e3833001 	orr	r3, r3, #1
   11258:	e5823004 	str	r3, [r2, #4]
   1125c:	e5993004 	ldr	r3, [r9, #4]
   11260:	e2033001 	and	r3, r3, #1
   11264:	e1855003 	orr	r5, r5, r3
   11268:	e59f3054 	ldr	r3, [pc, #84]	; 112c4 <_realloc_r+0x570>
   1126c:	e5895004 	str	r5, [r9, #4]
   11270:	e1a00008 	mov	r0, r8
   11274:	e5832008 	str	r2, [r3, #8]
   11278:	ebfff35f 	bl	dffc <__malloc_unlock>
   1127c:	eafffedd 	b	10df8 <_realloc_r+0xa4>
   11280:	e5166004 	ldr	r6, [r6, #-4]
   11284:	e3c66003 	bic	r6, r6, #3
   11288:	e0866007 	add	r6, r6, r7
   1128c:	eafffecc 	b	10dc4 <_realloc_r+0x70>
   11290:	e1a01004 	mov	r1, r4
   11294:	e1a0000a 	mov	r0, sl
   11298:	ebfffe4f 	bl	10bdc <memmove>
   1129c:	e1a0400a 	mov	r4, sl
   112a0:	e5993004 	ldr	r3, [r9, #4]
   112a4:	e1a0a009 	mov	sl, r9
   112a8:	eafffec5 	b	10dc4 <_realloc_r+0x70>
   112ac:	e1a00006 	mov	r0, r6
   112b0:	e1a01004 	mov	r1, r4
   112b4:	e58d3004 	str	r3, [sp, #4]
   112b8:	ebfffe47 	bl	10bdc <memmove>
   112bc:	e59d3004 	ldr	r3, [sp, #4]
   112c0:	eaffffe1 	b	1124c <_realloc_r+0x4f8>
   112c4:	0001aaac 	.word	0x0001aaac

000112c8 <cleanup_glue>:
   112c8:	e92d4038 	push	{r3, r4, r5, lr}
   112cc:	e1a04001 	mov	r4, r1
   112d0:	e5911000 	ldr	r1, [r1]
   112d4:	e3510000 	cmp	r1, #0
   112d8:	e1a05000 	mov	r5, r0
   112dc:	1bfffff9 	blne	112c8 <cleanup_glue>
   112e0:	e1a00005 	mov	r0, r5
   112e4:	e1a01004 	mov	r1, r4
   112e8:	ebffef8d 	bl	d124 <_free_r>
   112ec:	e8bd4038 	pop	{r3, r4, r5, lr}
   112f0:	e12fff1e 	bx	lr

000112f4 <_reclaim_reent>:
   112f4:	e59f30f0 	ldr	r3, [pc, #240]	; 113ec <_reclaim_reent+0xf8>
   112f8:	e5933000 	ldr	r3, [r3]
   112fc:	e1500003 	cmp	r0, r3
   11300:	e92d4070 	push	{r4, r5, r6, lr}
   11304:	e1a05000 	mov	r5, r0
   11308:	0a00002b 	beq	113bc <_reclaim_reent+0xc8>
   1130c:	e590204c 	ldr	r2, [r0, #76]	; 0x4c
   11310:	e3520000 	cmp	r2, #0
   11314:	0a000011 	beq	11360 <_reclaim_reent+0x6c>
   11318:	e3a03000 	mov	r3, #0
   1131c:	e1a06003 	mov	r6, r3
   11320:	e7921103 	ldr	r1, [r2, r3, lsl #2]
   11324:	e3510000 	cmp	r1, #0
   11328:	0a000005 	beq	11344 <_reclaim_reent+0x50>
   1132c:	e5914000 	ldr	r4, [r1]
   11330:	e1a00005 	mov	r0, r5
   11334:	ebffef7a 	bl	d124 <_free_r>
   11338:	e2541000 	subs	r1, r4, #0
   1133c:	1afffffa 	bne	1132c <_reclaim_reent+0x38>
   11340:	e595204c 	ldr	r2, [r5, #76]	; 0x4c
   11344:	e2866001 	add	r6, r6, #1
   11348:	e3560020 	cmp	r6, #32
   1134c:	e1a03006 	mov	r3, r6
   11350:	1afffff2 	bne	11320 <_reclaim_reent+0x2c>
   11354:	e1a00005 	mov	r0, r5
   11358:	e1a01002 	mov	r1, r2
   1135c:	ebffef70 	bl	d124 <_free_r>
   11360:	e5951040 	ldr	r1, [r5, #64]	; 0x40
   11364:	e3510000 	cmp	r1, #0
   11368:	11a00005 	movne	r0, r5
   1136c:	1bffef6c 	blne	d124 <_free_r>
   11370:	e5951148 	ldr	r1, [r5, #328]	; 0x148
   11374:	e3510000 	cmp	r1, #0
   11378:	0a000008 	beq	113a0 <_reclaim_reent+0xac>
   1137c:	e2856f53 	add	r6, r5, #332	; 0x14c
   11380:	e1510006 	cmp	r1, r6
   11384:	0a000005 	beq	113a0 <_reclaim_reent+0xac>
   11388:	e5914000 	ldr	r4, [r1]
   1138c:	e1a00005 	mov	r0, r5
   11390:	ebffef63 	bl	d124 <_free_r>
   11394:	e1560004 	cmp	r6, r4
   11398:	e1a01004 	mov	r1, r4
   1139c:	1afffff9 	bne	11388 <_reclaim_reent+0x94>
   113a0:	e5951054 	ldr	r1, [r5, #84]	; 0x54
   113a4:	e3510000 	cmp	r1, #0
   113a8:	11a00005 	movne	r0, r5
   113ac:	1bffef5c 	blne	d124 <_free_r>
   113b0:	e5953038 	ldr	r3, [r5, #56]	; 0x38
   113b4:	e3530000 	cmp	r3, #0
   113b8:	1a000001 	bne	113c4 <_reclaim_reent+0xd0>
   113bc:	e8bd4070 	pop	{r4, r5, r6, lr}
   113c0:	e12fff1e 	bx	lr
   113c4:	e1a00005 	mov	r0, r5
   113c8:	e595c03c 	ldr	ip, [r5, #60]	; 0x3c
   113cc:	e1a0e00f 	mov	lr, pc
   113d0:	e12fff1c 	bx	ip
   113d4:	e59512e0 	ldr	r1, [r5, #736]	; 0x2e0
   113d8:	e3510000 	cmp	r1, #0
   113dc:	0afffff6 	beq	113bc <_reclaim_reent+0xc8>
   113e0:	e1a00005 	mov	r0, r5
   113e4:	e8bd4070 	pop	{r4, r5, r6, lr}
   113e8:	eaffffb6 	b	112c8 <cleanup_glue>
   113ec:	0001a600 	.word	0x0001a600

000113f0 <_wrapup_reent>:
   113f0:	e92d40f8 	push	{r3, r4, r5, r6, r7, lr}
   113f4:	e2507000 	subs	r7, r0, #0
   113f8:	059f3060 	ldreq	r3, [pc, #96]	; 11460 <_wrapup_reent+0x70>
   113fc:	05937000 	ldreq	r7, [r3]
   11400:	e5976148 	ldr	r6, [r7, #328]	; 0x148
   11404:	e3560000 	cmp	r6, #0
   11408:	0a00000d 	beq	11444 <_wrapup_reent+0x54>
   1140c:	e5965004 	ldr	r5, [r6, #4]
   11410:	e2554001 	subs	r4, r5, #1
   11414:	52855002 	addpl	r5, r5, #2
   11418:	50865105 	addpl	r5, r6, r5, lsl #2
   1141c:	4a000005 	bmi	11438 <_wrapup_reent+0x48>
   11420:	e5353004 	ldr	r3, [r5, #-4]!
   11424:	e2444001 	sub	r4, r4, #1
   11428:	e1a0e00f 	mov	lr, pc
   1142c:	e12fff13 	bx	r3
   11430:	e3740001 	cmn	r4, #1
   11434:	1afffff9 	bne	11420 <_wrapup_reent+0x30>
   11438:	e5966000 	ldr	r6, [r6]
   1143c:	e3560000 	cmp	r6, #0
   11440:	1afffff1 	bne	1140c <_wrapup_reent+0x1c>
   11444:	e597303c 	ldr	r3, [r7, #60]	; 0x3c
   11448:	e3530000 	cmp	r3, #0
   1144c:	11a00007 	movne	r0, r7
   11450:	11a0e00f 	movne	lr, pc
   11454:	112fff13 	bxne	r3
   11458:	e8bd40f8 	pop	{r3, r4, r5, r6, r7, lr}
   1145c:	e12fff1e 	bx	lr
   11460:	0001a600 	.word	0x0001a600

00011464 <__swbuf_r>:
   11464:	e92d4070 	push	{r4, r5, r6, lr}
   11468:	e2506000 	subs	r6, r0, #0
   1146c:	e1a05001 	mov	r5, r1
   11470:	e1a04002 	mov	r4, r2
   11474:	0a000002 	beq	11484 <__swbuf_r+0x20>
   11478:	e5963038 	ldr	r3, [r6, #56]	; 0x38
   1147c:	e3530000 	cmp	r3, #0
   11480:	0a00003b 	beq	11574 <__swbuf_r+0x110>
   11484:	e1d430bc 	ldrh	r3, [r4, #12]
   11488:	e5942018 	ldr	r2, [r4, #24]
   1148c:	e3130008 	tst	r3, #8
   11490:	e5842008 	str	r2, [r4, #8]
   11494:	0a000026 	beq	11534 <__swbuf_r+0xd0>
   11498:	e5942010 	ldr	r2, [r4, #16]
   1149c:	e3520000 	cmp	r2, #0
   114a0:	0a000023 	beq	11534 <__swbuf_r+0xd0>
   114a4:	e3130a02 	tst	r3, #8192	; 0x2000
   114a8:	05941064 	ldreq	r1, [r4, #100]	; 0x64
   114ac:	03833a02 	orreq	r3, r3, #8192	; 0x2000
   114b0:	03c11a02 	biceq	r1, r1, #8192	; 0x2000
   114b4:	01c430bc 	strheq	r3, [r4, #12]
   114b8:	e5943000 	ldr	r3, [r4]
   114bc:	05841064 	streq	r1, [r4, #100]	; 0x64
   114c0:	e5941014 	ldr	r1, [r4, #20]
   114c4:	e0622003 	rsb	r2, r2, r3
   114c8:	e1520001 	cmp	r2, r1
   114cc:	e20550ff 	and	r5, r5, #255	; 0xff
   114d0:	b2822001 	addlt	r2, r2, #1
   114d4:	aa00001e 	bge	11554 <__swbuf_r+0xf0>
   114d8:	e5941008 	ldr	r1, [r4, #8]
   114dc:	e2411001 	sub	r1, r1, #1
   114e0:	e5841008 	str	r1, [r4, #8]
   114e4:	e4c35001 	strb	r5, [r3], #1
   114e8:	e5941014 	ldr	r1, [r4, #20]
   114ec:	e1510002 	cmp	r1, r2
   114f0:	e5843000 	str	r3, [r4]
   114f4:	0a000007 	beq	11518 <__swbuf_r+0xb4>
   114f8:	e1d430bc 	ldrh	r3, [r4, #12]
   114fc:	e3130001 	tst	r3, #1
   11500:	0a000001 	beq	1150c <__swbuf_r+0xa8>
   11504:	e355000a 	cmp	r5, #10
   11508:	0a000002 	beq	11518 <__swbuf_r+0xb4>
   1150c:	e1a00005 	mov	r0, r5
   11510:	e8bd4070 	pop	{r4, r5, r6, lr}
   11514:	e12fff1e 	bx	lr
   11518:	e1a00006 	mov	r0, r6
   1151c:	e1a01004 	mov	r1, r4
   11520:	ebffedea 	bl	ccd0 <_fflush_r>
   11524:	e3500000 	cmp	r0, #0
   11528:	0afffff7 	beq	1150c <__swbuf_r+0xa8>
   1152c:	e3e05000 	mvn	r5, #0
   11530:	eafffff5 	b	1150c <__swbuf_r+0xa8>
   11534:	e1a00006 	mov	r0, r6
   11538:	e1a01004 	mov	r1, r4
   1153c:	ebffe641 	bl	ae48 <__swsetup_r>
   11540:	e3500000 	cmp	r0, #0
   11544:	1afffff8 	bne	1152c <__swbuf_r+0xc8>
   11548:	e1d430bc 	ldrh	r3, [r4, #12]
   1154c:	e5942010 	ldr	r2, [r4, #16]
   11550:	eaffffd3 	b	114a4 <__swbuf_r+0x40>
   11554:	e1a00006 	mov	r0, r6
   11558:	e1a01004 	mov	r1, r4
   1155c:	ebffeddb 	bl	ccd0 <_fflush_r>
   11560:	e3500000 	cmp	r0, #0
   11564:	1afffff0 	bne	1152c <__swbuf_r+0xc8>
   11568:	e5943000 	ldr	r3, [r4]
   1156c:	e3a02001 	mov	r2, #1
   11570:	eaffffd8 	b	114d8 <__swbuf_r+0x74>
   11574:	ebffee39 	bl	ce60 <__sinit>
   11578:	eaffffc1 	b	11484 <__swbuf_r+0x20>

0001157c <__swbuf>:
   1157c:	e1a02001 	mov	r2, r1
   11580:	e59f100c 	ldr	r1, [pc, #12]	; 11594 <__swbuf+0x18>
   11584:	e1a03000 	mov	r3, r0
   11588:	e5910000 	ldr	r0, [r1]
   1158c:	e1a01003 	mov	r1, r3
   11590:	eaffffb3 	b	11464 <__swbuf_r>
   11594:	0001a600 	.word	0x0001a600

00011598 <_wcrtomb_r>:
   11598:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
   1159c:	e2516000 	subs	r6, r1, #0
   115a0:	e1a05003 	mov	r5, r3
   115a4:	e24dd018 	sub	sp, sp, #24
   115a8:	e1a04000 	mov	r4, r0
   115ac:	e1a07002 	mov	r7, r2
   115b0:	e59f306c 	ldr	r3, [pc, #108]	; 11624 <_wcrtomb_r+0x8c>
   115b4:	0a000010 	beq	115fc <_wcrtomb_r+0x64>
   115b8:	e5938000 	ldr	r8, [r3]
   115bc:	ebffefd2 	bl	d50c <__locale_charset>
   115c0:	e58d5000 	str	r5, [sp]
   115c4:	e1a03000 	mov	r3, r0
   115c8:	e1a01006 	mov	r1, r6
   115cc:	e1a00004 	mov	r0, r4
   115d0:	e1a02007 	mov	r2, r7
   115d4:	e1a0e00f 	mov	lr, pc
   115d8:	e12fff18 	bx	r8
   115dc:	e3700001 	cmn	r0, #1
   115e0:	03a03000 	moveq	r3, #0
   115e4:	05853000 	streq	r3, [r5]
   115e8:	03a0308a 	moveq	r3, #138	; 0x8a
   115ec:	05843000 	streq	r3, [r4]
   115f0:	e28dd018 	add	sp, sp, #24
   115f4:	e8bd41f0 	pop	{r4, r5, r6, r7, r8, lr}
   115f8:	e12fff1e 	bx	lr
   115fc:	e5937000 	ldr	r7, [r3]
   11600:	ebffefc1 	bl	d50c <__locale_charset>
   11604:	e58d5000 	str	r5, [sp]
   11608:	e1a03000 	mov	r3, r0
   1160c:	e28d100c 	add	r1, sp, #12
   11610:	e1a00004 	mov	r0, r4
   11614:	e1a02006 	mov	r2, r6
   11618:	e1a0e00f 	mov	lr, pc
   1161c:	e12fff17 	bx	r7
   11620:	eaffffed 	b	115dc <_wcrtomb_r+0x44>
   11624:	0001aebc 	.word	0x0001aebc

00011628 <wcrtomb>:
   11628:	e92d45f0 	push	{r4, r5, r6, r7, r8, sl, lr}
   1162c:	e2506000 	subs	r6, r0, #0
   11630:	e24dd01c 	sub	sp, sp, #28
   11634:	e59f5088 	ldr	r5, [pc, #136]	; 116c4 <wcrtomb+0x9c>
   11638:	e1a07001 	mov	r7, r1
   1163c:	e1a04002 	mov	r4, r2
   11640:	0a000013 	beq	11694 <wcrtomb+0x6c>
   11644:	e59f307c 	ldr	r3, [pc, #124]	; 116c8 <wcrtomb+0xa0>
   11648:	e595a000 	ldr	sl, [r5]
   1164c:	e5938000 	ldr	r8, [r3]
   11650:	ebffefad 	bl	d50c <__locale_charset>
   11654:	e58d4000 	str	r4, [sp]
   11658:	e1a03000 	mov	r3, r0
   1165c:	e1a01006 	mov	r1, r6
   11660:	e1a0000a 	mov	r0, sl
   11664:	e1a02007 	mov	r2, r7
   11668:	e1a0e00f 	mov	lr, pc
   1166c:	e12fff18 	bx	r8
   11670:	e3700001 	cmn	r0, #1
   11674:	03a02000 	moveq	r2, #0
   11678:	05953000 	ldreq	r3, [r5]
   1167c:	05842000 	streq	r2, [r4]
   11680:	03a0208a 	moveq	r2, #138	; 0x8a
   11684:	05832000 	streq	r2, [r3]
   11688:	e28dd01c 	add	sp, sp, #28
   1168c:	e8bd45f0 	pop	{r4, r5, r6, r7, r8, sl, lr}
   11690:	e12fff1e 	bx	lr
   11694:	e59f302c 	ldr	r3, [pc, #44]	; 116c8 <wcrtomb+0xa0>
   11698:	e5958000 	ldr	r8, [r5]
   1169c:	e5937000 	ldr	r7, [r3]
   116a0:	ebffef99 	bl	d50c <__locale_charset>
   116a4:	e58d4000 	str	r4, [sp]
   116a8:	e1a03000 	mov	r3, r0
   116ac:	e28d100c 	add	r1, sp, #12
   116b0:	e1a00008 	mov	r0, r8
   116b4:	e1a02006 	mov	r2, r6
   116b8:	e1a0e00f 	mov	lr, pc
   116bc:	e12fff17 	bx	r7
   116c0:	eaffffea 	b	11670 <wcrtomb+0x48>
   116c4:	0001a600 	.word	0x0001a600
   116c8:	0001aebc 	.word	0x0001aebc

000116cc <__ascii_wctomb>:
   116cc:	e3510000 	cmp	r1, #0
   116d0:	0a000006 	beq	116f0 <__ascii_wctomb+0x24>
   116d4:	e35200ff 	cmp	r2, #255	; 0xff
   116d8:	83a0308a 	movhi	r3, #138	; 0x8a
   116dc:	85803000 	strhi	r3, [r0]
   116e0:	95c12000 	strbls	r2, [r1]
   116e4:	83e00000 	mvnhi	r0, #0
   116e8:	93a00001 	movls	r0, #1
   116ec:	e12fff1e 	bx	lr
   116f0:	e1a00001 	mov	r0, r1
   116f4:	e12fff1e 	bx	lr

000116f8 <_wctomb_r>:
   116f8:	e92d41f0 	push	{r4, r5, r6, r7, r8, lr}
   116fc:	e1a06003 	mov	r6, r3
   11700:	e59f303c 	ldr	r3, [pc, #60]	; 11744 <_wctomb_r+0x4c>
   11704:	e24dd008 	sub	sp, sp, #8
   11708:	e1a05000 	mov	r5, r0
   1170c:	e1a08001 	mov	r8, r1
   11710:	e1a07002 	mov	r7, r2
   11714:	e5934000 	ldr	r4, [r3]
   11718:	ebffef7b 	bl	d50c <__locale_charset>
   1171c:	e58d6000 	str	r6, [sp]
   11720:	e1a03000 	mov	r3, r0
   11724:	e1a01008 	mov	r1, r8
   11728:	e1a00005 	mov	r0, r5
   1172c:	e1a02007 	mov	r2, r7
   11730:	e1a0e00f 	mov	lr, pc
   11734:	e12fff14 	bx	r4
   11738:	e28dd008 	add	sp, sp, #8
   1173c:	e8bd41f0 	pop	{r4, r5, r6, r7, r8, lr}
   11740:	e12fff1e 	bx	lr
   11744:	0001aebc 	.word	0x0001aebc

00011748 <__aeabi_uidiv>:
   11748:	e2512001 	subs	r2, r1, #1
   1174c:	012fff1e 	bxeq	lr
   11750:	3a000036 	bcc	11830 <__aeabi_uidiv+0xe8>
   11754:	e1500001 	cmp	r0, r1
   11758:	9a000022 	bls	117e8 <__aeabi_uidiv+0xa0>
   1175c:	e1110002 	tst	r1, r2
   11760:	0a000023 	beq	117f4 <__aeabi_uidiv+0xac>
   11764:	e311020e 	tst	r1, #-536870912	; 0xe0000000
   11768:	01a01181 	lsleq	r1, r1, #3
   1176c:	03a03008 	moveq	r3, #8
   11770:	13a03001 	movne	r3, #1
   11774:	e3510201 	cmp	r1, #268435456	; 0x10000000
   11778:	31510000 	cmpcc	r1, r0
   1177c:	31a01201 	lslcc	r1, r1, #4
   11780:	31a03203 	lslcc	r3, r3, #4
   11784:	3afffffa 	bcc	11774 <__aeabi_uidiv+0x2c>
   11788:	e3510102 	cmp	r1, #-2147483648	; 0x80000000
   1178c:	31510000 	cmpcc	r1, r0
   11790:	31a01081 	lslcc	r1, r1, #1
   11794:	31a03083 	lslcc	r3, r3, #1
   11798:	3afffffa 	bcc	11788 <__aeabi_uidiv+0x40>
   1179c:	e3a02000 	mov	r2, #0
   117a0:	e1500001 	cmp	r0, r1
   117a4:	20400001 	subcs	r0, r0, r1
   117a8:	21822003 	orrcs	r2, r2, r3
   117ac:	e15000a1 	cmp	r0, r1, lsr #1
   117b0:	204000a1 	subcs	r0, r0, r1, lsr #1
   117b4:	218220a3 	orrcs	r2, r2, r3, lsr #1
   117b8:	e1500121 	cmp	r0, r1, lsr #2
   117bc:	20400121 	subcs	r0, r0, r1, lsr #2
   117c0:	21822123 	orrcs	r2, r2, r3, lsr #2
   117c4:	e15001a1 	cmp	r0, r1, lsr #3
   117c8:	204001a1 	subcs	r0, r0, r1, lsr #3
   117cc:	218221a3 	orrcs	r2, r2, r3, lsr #3
   117d0:	e3500000 	cmp	r0, #0
   117d4:	11b03223 	lsrsne	r3, r3, #4
   117d8:	11a01221 	lsrne	r1, r1, #4
   117dc:	1affffef 	bne	117a0 <__aeabi_uidiv+0x58>
   117e0:	e1a00002 	mov	r0, r2
   117e4:	e12fff1e 	bx	lr
   117e8:	03a00001 	moveq	r0, #1
   117ec:	13a00000 	movne	r0, #0
   117f0:	e12fff1e 	bx	lr
   117f4:	e3510801 	cmp	r1, #65536	; 0x10000
   117f8:	21a01821 	lsrcs	r1, r1, #16
   117fc:	23a02010 	movcs	r2, #16
   11800:	33a02000 	movcc	r2, #0
   11804:	e3510c01 	cmp	r1, #256	; 0x100
   11808:	21a01421 	lsrcs	r1, r1, #8
   1180c:	22822008 	addcs	r2, r2, #8
   11810:	e3510010 	cmp	r1, #16
   11814:	21a01221 	lsrcs	r1, r1, #4
   11818:	22822004 	addcs	r2, r2, #4
   1181c:	e3510004 	cmp	r1, #4
   11820:	82822003 	addhi	r2, r2, #3
   11824:	908220a1 	addls	r2, r2, r1, lsr #1
   11828:	e1a00230 	lsr	r0, r0, r2
   1182c:	e12fff1e 	bx	lr
   11830:	e3500000 	cmp	r0, #0
   11834:	13e00000 	mvnne	r0, #0
   11838:	ea000007 	b	1185c <__aeabi_idiv0>

0001183c <__aeabi_uidivmod>:
   1183c:	e3510000 	cmp	r1, #0
   11840:	0afffffa 	beq	11830 <__aeabi_uidiv+0xe8>
   11844:	e92d4003 	push	{r0, r1, lr}
   11848:	ebffffbe 	bl	11748 <__aeabi_uidiv>
   1184c:	e8bd4006 	pop	{r1, r2, lr}
   11850:	e0030092 	mul	r3, r2, r0
   11854:	e0411003 	sub	r1, r1, r3
   11858:	e12fff1e 	bx	lr

0001185c <__aeabi_idiv0>:
   1185c:	e12fff1e 	bx	lr

00011860 <__aeabi_drsub>:
   11860:	e2211102 	eor	r1, r1, #-2147483648	; 0x80000000
   11864:	ea000000 	b	1186c <__adddf3>

00011868 <__aeabi_dsub>:
   11868:	e2233102 	eor	r3, r3, #-2147483648	; 0x80000000

0001186c <__adddf3>:
   1186c:	e92d4030 	push	{r4, r5, lr}
   11870:	e1a04081 	lsl	r4, r1, #1
   11874:	e1a05083 	lsl	r5, r3, #1
   11878:	e1340005 	teq	r4, r5
   1187c:	01300002 	teqeq	r0, r2
   11880:	1194c000 	orrsne	ip, r4, r0
   11884:	1195c002 	orrsne	ip, r5, r2
   11888:	11f0cac4 	mvnsne	ip, r4, asr #21
   1188c:	11f0cac5 	mvnsne	ip, r5, asr #21
   11890:	0a00008c 	beq	11ac8 <__adddf3+0x25c>
   11894:	e1a04aa4 	lsr	r4, r4, #21
   11898:	e0745aa5 	rsbs	r5, r4, r5, lsr #21
   1189c:	b2655000 	rsblt	r5, r5, #0
   118a0:	da000006 	ble	118c0 <__adddf3+0x54>
   118a4:	e0844005 	add	r4, r4, r5
   118a8:	e0202002 	eor	r2, r0, r2
   118ac:	e0213003 	eor	r3, r1, r3
   118b0:	e0220000 	eor	r0, r2, r0
   118b4:	e0231001 	eor	r1, r3, r1
   118b8:	e0202002 	eor	r2, r0, r2
   118bc:	e0213003 	eor	r3, r1, r3
   118c0:	e3550036 	cmp	r5, #54	; 0x36
   118c4:	88bd4030 	pophi	{r4, r5, lr}
   118c8:	812fff1e 	bxhi	lr
   118cc:	e3110102 	tst	r1, #-2147483648	; 0x80000000
   118d0:	e1a01601 	lsl	r1, r1, #12
   118d4:	e3a0c601 	mov	ip, #1048576	; 0x100000
   118d8:	e18c1621 	orr	r1, ip, r1, lsr #12
   118dc:	0a000001 	beq	118e8 <__adddf3+0x7c>
   118e0:	e2700000 	rsbs	r0, r0, #0
   118e4:	e2e11000 	rsc	r1, r1, #0
   118e8:	e3130102 	tst	r3, #-2147483648	; 0x80000000
   118ec:	e1a03603 	lsl	r3, r3, #12
   118f0:	e18c3623 	orr	r3, ip, r3, lsr #12
   118f4:	0a000001 	beq	11900 <__adddf3+0x94>
   118f8:	e2722000 	rsbs	r2, r2, #0
   118fc:	e2e33000 	rsc	r3, r3, #0
   11900:	e1340005 	teq	r4, r5
   11904:	0a000069 	beq	11ab0 <__adddf3+0x244>
   11908:	e2444001 	sub	r4, r4, #1
   1190c:	e275e020 	rsbs	lr, r5, #32
   11910:	ba000005 	blt	1192c <__adddf3+0xc0>
   11914:	e1a0ce12 	lsl	ip, r2, lr
   11918:	e0900532 	adds	r0, r0, r2, lsr r5
   1191c:	e2a11000 	adc	r1, r1, #0
   11920:	e0900e13 	adds	r0, r0, r3, lsl lr
   11924:	e0b11553 	adcs	r1, r1, r3, asr r5
   11928:	ea000006 	b	11948 <__adddf3+0xdc>
   1192c:	e2455020 	sub	r5, r5, #32
   11930:	e28ee020 	add	lr, lr, #32
   11934:	e3520001 	cmp	r2, #1
   11938:	e1a0ce13 	lsl	ip, r3, lr
   1193c:	238cc002 	orrcs	ip, ip, #2
   11940:	e0900553 	adds	r0, r0, r3, asr r5
   11944:	e0b11fc3 	adcs	r1, r1, r3, asr #31
   11948:	e2015102 	and	r5, r1, #-2147483648	; 0x80000000
   1194c:	5a000002 	bpl	1195c <__adddf3+0xf0>
   11950:	e27cc000 	rsbs	ip, ip, #0
   11954:	e2f00000 	rscs	r0, r0, #0
   11958:	e2e11000 	rsc	r1, r1, #0
   1195c:	e3510601 	cmp	r1, #1048576	; 0x100000
   11960:	3a00000f 	bcc	119a4 <__adddf3+0x138>
   11964:	e3510602 	cmp	r1, #2097152	; 0x200000
   11968:	3a000006 	bcc	11988 <__adddf3+0x11c>
   1196c:	e1b010a1 	lsrs	r1, r1, #1
   11970:	e1b00060 	rrxs	r0, r0
   11974:	e1a0c06c 	rrx	ip, ip
   11978:	e2844001 	add	r4, r4, #1
   1197c:	e1a02a84 	lsl	r2, r4, #21
   11980:	e3720501 	cmn	r2, #4194304	; 0x400000
   11984:	2a00006b 	bcs	11b38 <__adddf3+0x2cc>
   11988:	e35c0102 	cmp	ip, #-2147483648	; 0x80000000
   1198c:	01b0c0a0 	lsrseq	ip, r0, #1
   11990:	e2b00000 	adcs	r0, r0, #0
   11994:	e0a11a04 	adc	r1, r1, r4, lsl #20
   11998:	e1811005 	orr	r1, r1, r5
   1199c:	e8bd4030 	pop	{r4, r5, lr}
   119a0:	e12fff1e 	bx	lr
   119a4:	e1b0c08c 	lsls	ip, ip, #1
   119a8:	e0b00000 	adcs	r0, r0, r0
   119ac:	e0a11001 	adc	r1, r1, r1
   119b0:	e3110601 	tst	r1, #1048576	; 0x100000
   119b4:	e2444001 	sub	r4, r4, #1
   119b8:	1afffff2 	bne	11988 <__adddf3+0x11c>
   119bc:	e3310000 	teq	r1, #0
   119c0:	13a03014 	movne	r3, #20
   119c4:	03a03034 	moveq	r3, #52	; 0x34
   119c8:	01a01000 	moveq	r1, r0
   119cc:	03a00000 	moveq	r0, #0
   119d0:	e1a02001 	mov	r2, r1
   119d4:	e3520801 	cmp	r2, #65536	; 0x10000
   119d8:	21a02822 	lsrcs	r2, r2, #16
   119dc:	22433010 	subcs	r3, r3, #16
   119e0:	e3520c01 	cmp	r2, #256	; 0x100
   119e4:	21a02422 	lsrcs	r2, r2, #8
   119e8:	22433008 	subcs	r3, r3, #8
   119ec:	e3520010 	cmp	r2, #16
   119f0:	21a02222 	lsrcs	r2, r2, #4
   119f4:	22433004 	subcs	r3, r3, #4
   119f8:	e3520004 	cmp	r2, #4
   119fc:	22433002 	subcs	r3, r3, #2
   11a00:	304330a2 	subcc	r3, r3, r2, lsr #1
   11a04:	e04331a2 	sub	r3, r3, r2, lsr #3
   11a08:	e2532020 	subs	r2, r3, #32
   11a0c:	aa000007 	bge	11a30 <__adddf3+0x1c4>
   11a10:	e292200c 	adds	r2, r2, #12
   11a14:	da000004 	ble	11a2c <__adddf3+0x1c0>
   11a18:	e282c014 	add	ip, r2, #20
   11a1c:	e262200c 	rsb	r2, r2, #12
   11a20:	e1a00c11 	lsl	r0, r1, ip
   11a24:	e1a01231 	lsr	r1, r1, r2
   11a28:	ea000004 	b	11a40 <__adddf3+0x1d4>
   11a2c:	e2822014 	add	r2, r2, #20
   11a30:	d262c020 	rsble	ip, r2, #32
   11a34:	e1a01211 	lsl	r1, r1, r2
   11a38:	d1811c30 	orrle	r1, r1, r0, lsr ip
   11a3c:	d1a00210 	lslle	r0, r0, r2
   11a40:	e0544003 	subs	r4, r4, r3
   11a44:	a0811a04 	addge	r1, r1, r4, lsl #20
   11a48:	a1811005 	orrge	r1, r1, r5
   11a4c:	a8bd4030 	popge	{r4, r5, lr}
   11a50:	a12fff1e 	bxge	lr
   11a54:	e1e04004 	mvn	r4, r4
   11a58:	e254401f 	subs	r4, r4, #31
   11a5c:	aa00000f 	bge	11aa0 <__adddf3+0x234>
   11a60:	e294400c 	adds	r4, r4, #12
   11a64:	ca000006 	bgt	11a84 <__adddf3+0x218>
   11a68:	e2844014 	add	r4, r4, #20
   11a6c:	e2642020 	rsb	r2, r4, #32
   11a70:	e1a00430 	lsr	r0, r0, r4
   11a74:	e1800211 	orr	r0, r0, r1, lsl r2
   11a78:	e1851431 	orr	r1, r5, r1, lsr r4
   11a7c:	e8bd4030 	pop	{r4, r5, lr}
   11a80:	e12fff1e 	bx	lr
   11a84:	e264400c 	rsb	r4, r4, #12
   11a88:	e2642020 	rsb	r2, r4, #32
   11a8c:	e1a00230 	lsr	r0, r0, r2
   11a90:	e1800411 	orr	r0, r0, r1, lsl r4
   11a94:	e1a01005 	mov	r1, r5
   11a98:	e8bd4030 	pop	{r4, r5, lr}
   11a9c:	e12fff1e 	bx	lr
   11aa0:	e1a00431 	lsr	r0, r1, r4
   11aa4:	e1a01005 	mov	r1, r5
   11aa8:	e8bd4030 	pop	{r4, r5, lr}
   11aac:	e12fff1e 	bx	lr
   11ab0:	e3340000 	teq	r4, #0
   11ab4:	e2233601 	eor	r3, r3, #1048576	; 0x100000
   11ab8:	02211601 	eoreq	r1, r1, #1048576	; 0x100000
   11abc:	02844001 	addeq	r4, r4, #1
   11ac0:	12455001 	subne	r5, r5, #1
   11ac4:	eaffff8f 	b	11908 <__adddf3+0x9c>
   11ac8:	e1f0cac4 	mvns	ip, r4, asr #21
   11acc:	11f0cac5 	mvnsne	ip, r5, asr #21
   11ad0:	0a00001d 	beq	11b4c <__adddf3+0x2e0>
   11ad4:	e1340005 	teq	r4, r5
   11ad8:	01300002 	teqeq	r0, r2
   11adc:	0a000004 	beq	11af4 <__adddf3+0x288>
   11ae0:	e194c000 	orrs	ip, r4, r0
   11ae4:	01a01003 	moveq	r1, r3
   11ae8:	01a00002 	moveq	r0, r2
   11aec:	e8bd4030 	pop	{r4, r5, lr}
   11af0:	e12fff1e 	bx	lr
   11af4:	e1310003 	teq	r1, r3
   11af8:	13a01000 	movne	r1, #0
   11afc:	13a00000 	movne	r0, #0
   11b00:	18bd4030 	popne	{r4, r5, lr}
   11b04:	112fff1e 	bxne	lr
   11b08:	e1b0caa4 	lsrs	ip, r4, #21
   11b0c:	1a000004 	bne	11b24 <__adddf3+0x2b8>
   11b10:	e1b00080 	lsls	r0, r0, #1
   11b14:	e0b11001 	adcs	r1, r1, r1
   11b18:	23811102 	orrcs	r1, r1, #-2147483648	; 0x80000000
   11b1c:	e8bd4030 	pop	{r4, r5, lr}
   11b20:	e12fff1e 	bx	lr
   11b24:	e2944501 	adds	r4, r4, #4194304	; 0x400000
   11b28:	32811601 	addcc	r1, r1, #1048576	; 0x100000
   11b2c:	38bd4030 	popcc	{r4, r5, lr}
   11b30:	312fff1e 	bxcc	lr
   11b34:	e2015102 	and	r5, r1, #-2147483648	; 0x80000000
   11b38:	e385147f 	orr	r1, r5, #2130706432	; 0x7f000000
   11b3c:	e381160f 	orr	r1, r1, #15728640	; 0xf00000
   11b40:	e3a00000 	mov	r0, #0
   11b44:	e8bd4030 	pop	{r4, r5, lr}
   11b48:	e12fff1e 	bx	lr
   11b4c:	e1f0cac4 	mvns	ip, r4, asr #21
   11b50:	11a01003 	movne	r1, r3
   11b54:	11a00002 	movne	r0, r2
   11b58:	01f0cac5 	mvnseq	ip, r5, asr #21
   11b5c:	11a03001 	movne	r3, r1
   11b60:	11a02000 	movne	r2, r0
   11b64:	e1904601 	orrs	r4, r0, r1, lsl #12
   11b68:	01925603 	orrseq	r5, r2, r3, lsl #12
   11b6c:	01310003 	teqeq	r1, r3
   11b70:	13811702 	orrne	r1, r1, #524288	; 0x80000
   11b74:	e8bd4030 	pop	{r4, r5, lr}
   11b78:	e12fff1e 	bx	lr

00011b7c <__aeabi_ui2d>:
   11b7c:	e3300000 	teq	r0, #0
   11b80:	03a01000 	moveq	r1, #0
   11b84:	012fff1e 	bxeq	lr
   11b88:	e92d4030 	push	{r4, r5, lr}
   11b8c:	e3a04b01 	mov	r4, #1024	; 0x400
   11b90:	e2844032 	add	r4, r4, #50	; 0x32
   11b94:	e3a05000 	mov	r5, #0
   11b98:	e3a01000 	mov	r1, #0
   11b9c:	eaffff86 	b	119bc <__adddf3+0x150>

00011ba0 <__aeabi_i2d>:
   11ba0:	e3300000 	teq	r0, #0
   11ba4:	03a01000 	moveq	r1, #0
   11ba8:	012fff1e 	bxeq	lr
   11bac:	e92d4030 	push	{r4, r5, lr}
   11bb0:	e3a04b01 	mov	r4, #1024	; 0x400
   11bb4:	e2844032 	add	r4, r4, #50	; 0x32
   11bb8:	e2105102 	ands	r5, r0, #-2147483648	; 0x80000000
   11bbc:	42600000 	rsbmi	r0, r0, #0
   11bc0:	e3a01000 	mov	r1, #0
   11bc4:	eaffff7c 	b	119bc <__adddf3+0x150>

00011bc8 <__aeabi_f2d>:
   11bc8:	e1b02080 	lsls	r2, r0, #1
   11bcc:	e1a011c2 	asr	r1, r2, #3
   11bd0:	e1a01061 	rrx	r1, r1
   11bd4:	e1a00e02 	lsl	r0, r2, #28
   11bd8:	121234ff 	andsne	r3, r2, #-16777216	; 0xff000000
   11bdc:	133304ff 	teqne	r3, #-16777216	; 0xff000000
   11be0:	1221130e 	eorne	r1, r1, #939524096	; 0x38000000
   11be4:	112fff1e 	bxne	lr
   11be8:	e3320000 	teq	r2, #0
   11bec:	133304ff 	teqne	r3, #-16777216	; 0xff000000
   11bf0:	012fff1e 	bxeq	lr
   11bf4:	e92d4030 	push	{r4, r5, lr}
   11bf8:	e3a04d0e 	mov	r4, #896	; 0x380
   11bfc:	e2015102 	and	r5, r1, #-2147483648	; 0x80000000
   11c00:	e3c11102 	bic	r1, r1, #-2147483648	; 0x80000000
   11c04:	eaffff6c 	b	119bc <__adddf3+0x150>

00011c08 <__aeabi_ul2d>:
   11c08:	e1902001 	orrs	r2, r0, r1
   11c0c:	012fff1e 	bxeq	lr
   11c10:	e92d4030 	push	{r4, r5, lr}
   11c14:	e3a05000 	mov	r5, #0
   11c18:	ea000006 	b	11c38 <__aeabi_l2d+0x1c>

00011c1c <__aeabi_l2d>:
   11c1c:	e1902001 	orrs	r2, r0, r1
   11c20:	012fff1e 	bxeq	lr
   11c24:	e92d4030 	push	{r4, r5, lr}
   11c28:	e2115102 	ands	r5, r1, #-2147483648	; 0x80000000
   11c2c:	5a000001 	bpl	11c38 <__aeabi_l2d+0x1c>
   11c30:	e2700000 	rsbs	r0, r0, #0
   11c34:	e2e11000 	rsc	r1, r1, #0
   11c38:	e3a04b01 	mov	r4, #1024	; 0x400
   11c3c:	e2844032 	add	r4, r4, #50	; 0x32
   11c40:	e1b0cb21 	lsrs	ip, r1, #22
   11c44:	0affff44 	beq	1195c <__adddf3+0xf0>
   11c48:	e3a02003 	mov	r2, #3
   11c4c:	e1b0c1ac 	lsrs	ip, ip, #3
   11c50:	12822003 	addne	r2, r2, #3
   11c54:	e1b0c1ac 	lsrs	ip, ip, #3
   11c58:	12822003 	addne	r2, r2, #3
   11c5c:	e08221ac 	add	r2, r2, ip, lsr #3
   11c60:	e2623020 	rsb	r3, r2, #32
   11c64:	e1a0c310 	lsl	ip, r0, r3
   11c68:	e1a00230 	lsr	r0, r0, r2
   11c6c:	e1800311 	orr	r0, r0, r1, lsl r3
   11c70:	e1a01231 	lsr	r1, r1, r2
   11c74:	e0844002 	add	r4, r4, r2
   11c78:	eaffff37 	b	1195c <__adddf3+0xf0>

00011c7c <__aeabi_dmul>:
   11c7c:	e92d4070 	push	{r4, r5, r6, lr}
   11c80:	e3a0c0ff 	mov	ip, #255	; 0xff
   11c84:	e38ccc07 	orr	ip, ip, #1792	; 0x700
   11c88:	e01c4a21 	ands	r4, ip, r1, lsr #20
   11c8c:	101c5a23 	andsne	r5, ip, r3, lsr #20
   11c90:	1134000c 	teqne	r4, ip
   11c94:	1135000c 	teqne	r5, ip
   11c98:	0b000075 	bleq	11e74 <__aeabi_dmul+0x1f8>
   11c9c:	e0844005 	add	r4, r4, r5
   11ca0:	e0216003 	eor	r6, r1, r3
   11ca4:	e1c11a8c 	bic	r1, r1, ip, lsl #21
   11ca8:	e1c33a8c 	bic	r3, r3, ip, lsl #21
   11cac:	e1905601 	orrs	r5, r0, r1, lsl #12
   11cb0:	11925603 	orrsne	r5, r2, r3, lsl #12
   11cb4:	e3811601 	orr	r1, r1, #1048576	; 0x100000
   11cb8:	e3833601 	orr	r3, r3, #1048576	; 0x100000
   11cbc:	0a00001d 	beq	11d38 <__aeabi_dmul+0xbc>
   11cc0:	e08ec290 	umull	ip, lr, r0, r2
   11cc4:	e3a05000 	mov	r5, #0
   11cc8:	e0a5e291 	umlal	lr, r5, r1, r2
   11ccc:	e2062102 	and	r2, r6, #-2147483648	; 0x80000000
   11cd0:	e0a5e390 	umlal	lr, r5, r0, r3
   11cd4:	e3a06000 	mov	r6, #0
   11cd8:	e0a65391 	umlal	r5, r6, r1, r3
   11cdc:	e33c0000 	teq	ip, #0
   11ce0:	138ee001 	orrne	lr, lr, #1
   11ce4:	e24440ff 	sub	r4, r4, #255	; 0xff
   11ce8:	e3560c02 	cmp	r6, #512	; 0x200
   11cec:	e2c44c03 	sbc	r4, r4, #768	; 0x300
   11cf0:	2a000002 	bcs	11d00 <__aeabi_dmul+0x84>
   11cf4:	e1b0e08e 	lsls	lr, lr, #1
   11cf8:	e0b55005 	adcs	r5, r5, r5
   11cfc:	e0a66006 	adc	r6, r6, r6
   11d00:	e1821586 	orr	r1, r2, r6, lsl #11
   11d04:	e1811aa5 	orr	r1, r1, r5, lsr #21
   11d08:	e1a00585 	lsl	r0, r5, #11
   11d0c:	e1800aae 	orr	r0, r0, lr, lsr #21
   11d10:	e1a0e58e 	lsl	lr, lr, #11
   11d14:	e254c0fd 	subs	ip, r4, #253	; 0xfd
   11d18:	835c0c07 	cmphi	ip, #1792	; 0x700
   11d1c:	8a000011 	bhi	11d68 <__aeabi_dmul+0xec>
   11d20:	e35e0102 	cmp	lr, #-2147483648	; 0x80000000
   11d24:	01b0e0a0 	lsrseq	lr, r0, #1
   11d28:	e2b00000 	adcs	r0, r0, #0
   11d2c:	e0a11a04 	adc	r1, r1, r4, lsl #20
   11d30:	e8bd4070 	pop	{r4, r5, r6, lr}
   11d34:	e12fff1e 	bx	lr
   11d38:	e2066102 	and	r6, r6, #-2147483648	; 0x80000000
   11d3c:	e1861001 	orr	r1, r6, r1
   11d40:	e1800002 	orr	r0, r0, r2
   11d44:	e0211003 	eor	r1, r1, r3
   11d48:	e05440ac 	subs	r4, r4, ip, lsr #1
   11d4c:	c074500c 	rsbsgt	r5, r4, ip
   11d50:	c1811a04 	orrgt	r1, r1, r4, lsl #20
   11d54:	c8bd4070 	popgt	{r4, r5, r6, lr}
   11d58:	c12fff1e 	bxgt	lr
   11d5c:	e3811601 	orr	r1, r1, #1048576	; 0x100000
   11d60:	e3a0e000 	mov	lr, #0
   11d64:	e2544001 	subs	r4, r4, #1
   11d68:	ca00005d 	bgt	11ee4 <__aeabi_dmul+0x268>
   11d6c:	e3740036 	cmn	r4, #54	; 0x36
   11d70:	d3a00000 	movle	r0, #0
   11d74:	d2011102 	andle	r1, r1, #-2147483648	; 0x80000000
   11d78:	d8bd4070 	pople	{r4, r5, r6, lr}
   11d7c:	d12fff1e 	bxle	lr
   11d80:	e2644000 	rsb	r4, r4, #0
   11d84:	e2544020 	subs	r4, r4, #32
   11d88:	aa00001a 	bge	11df8 <__aeabi_dmul+0x17c>
   11d8c:	e294400c 	adds	r4, r4, #12
   11d90:	ca00000c 	bgt	11dc8 <__aeabi_dmul+0x14c>
   11d94:	e2844014 	add	r4, r4, #20
   11d98:	e2645020 	rsb	r5, r4, #32
   11d9c:	e1a03510 	lsl	r3, r0, r5
   11da0:	e1a00430 	lsr	r0, r0, r4
   11da4:	e1800511 	orr	r0, r0, r1, lsl r5
   11da8:	e2012102 	and	r2, r1, #-2147483648	; 0x80000000
   11dac:	e3c11102 	bic	r1, r1, #-2147483648	; 0x80000000
   11db0:	e0900fa3 	adds	r0, r0, r3, lsr #31
   11db4:	e0a21431 	adc	r1, r2, r1, lsr r4
   11db8:	e19ee083 	orrs	lr, lr, r3, lsl #1
   11dbc:	01c00fa3 	biceq	r0, r0, r3, lsr #31
   11dc0:	e8bd4070 	pop	{r4, r5, r6, lr}
   11dc4:	e12fff1e 	bx	lr
   11dc8:	e264400c 	rsb	r4, r4, #12
   11dcc:	e2645020 	rsb	r5, r4, #32
   11dd0:	e1a03410 	lsl	r3, r0, r4
   11dd4:	e1a00530 	lsr	r0, r0, r5
   11dd8:	e1800411 	orr	r0, r0, r1, lsl r4
   11ddc:	e2011102 	and	r1, r1, #-2147483648	; 0x80000000
   11de0:	e0900fa3 	adds	r0, r0, r3, lsr #31
   11de4:	e2a11000 	adc	r1, r1, #0
   11de8:	e19ee083 	orrs	lr, lr, r3, lsl #1
   11dec:	01c00fa3 	biceq	r0, r0, r3, lsr #31
   11df0:	e8bd4070 	pop	{r4, r5, r6, lr}
   11df4:	e12fff1e 	bx	lr
   11df8:	e2645020 	rsb	r5, r4, #32
   11dfc:	e18ee510 	orr	lr, lr, r0, lsl r5
   11e00:	e1a03430 	lsr	r3, r0, r4
   11e04:	e1833511 	orr	r3, r3, r1, lsl r5
   11e08:	e1a00431 	lsr	r0, r1, r4
   11e0c:	e2011102 	and	r1, r1, #-2147483648	; 0x80000000
   11e10:	e1c00431 	bic	r0, r0, r1, lsr r4
   11e14:	e0800fa3 	add	r0, r0, r3, lsr #31
   11e18:	e19ee083 	orrs	lr, lr, r3, lsl #1
   11e1c:	01c00fa3 	biceq	r0, r0, r3, lsr #31
   11e20:	e8bd4070 	pop	{r4, r5, r6, lr}
   11e24:	e12fff1e 	bx	lr
   11e28:	e3340000 	teq	r4, #0
   11e2c:	1a000008 	bne	11e54 <__aeabi_dmul+0x1d8>
   11e30:	e2016102 	and	r6, r1, #-2147483648	; 0x80000000
   11e34:	e1b00080 	lsls	r0, r0, #1
   11e38:	e0a11001 	adc	r1, r1, r1
   11e3c:	e3110601 	tst	r1, #1048576	; 0x100000
   11e40:	02444001 	subeq	r4, r4, #1
   11e44:	0afffffa 	beq	11e34 <__aeabi_dmul+0x1b8>
   11e48:	e1811006 	orr	r1, r1, r6
   11e4c:	e3350000 	teq	r5, #0
   11e50:	112fff1e 	bxne	lr
   11e54:	e2036102 	and	r6, r3, #-2147483648	; 0x80000000
   11e58:	e1b02082 	lsls	r2, r2, #1
   11e5c:	e0a33003 	adc	r3, r3, r3
   11e60:	e3130601 	tst	r3, #1048576	; 0x100000
   11e64:	02455001 	subeq	r5, r5, #1
   11e68:	0afffffa 	beq	11e58 <__aeabi_dmul+0x1dc>
   11e6c:	e1833006 	orr	r3, r3, r6
   11e70:	e12fff1e 	bx	lr
   11e74:	e134000c 	teq	r4, ip
   11e78:	e00c5a23 	and	r5, ip, r3, lsr #20
   11e7c:	1135000c 	teqne	r5, ip
   11e80:	0a000007 	beq	11ea4 <__aeabi_dmul+0x228>
   11e84:	e1906081 	orrs	r6, r0, r1, lsl #1
   11e88:	11926083 	orrsne	r6, r2, r3, lsl #1
   11e8c:	1affffe5 	bne	11e28 <__aeabi_dmul+0x1ac>
   11e90:	e0211003 	eor	r1, r1, r3
   11e94:	e2011102 	and	r1, r1, #-2147483648	; 0x80000000
   11e98:	e3a00000 	mov	r0, #0
   11e9c:	e8bd4070 	pop	{r4, r5, r6, lr}
   11ea0:	e12fff1e 	bx	lr
   11ea4:	e1906081 	orrs	r6, r0, r1, lsl #1
   11ea8:	01a00002 	moveq	r0, r2
   11eac:	01a01003 	moveq	r1, r3
   11eb0:	11926083 	orrsne	r6, r2, r3, lsl #1
   11eb4:	0a000010 	beq	11efc <__aeabi_dmul+0x280>
   11eb8:	e134000c 	teq	r4, ip
   11ebc:	1a000001 	bne	11ec8 <__aeabi_dmul+0x24c>
   11ec0:	e1906601 	orrs	r6, r0, r1, lsl #12
   11ec4:	1a00000c 	bne	11efc <__aeabi_dmul+0x280>
   11ec8:	e135000c 	teq	r5, ip
   11ecc:	1a000003 	bne	11ee0 <__aeabi_dmul+0x264>
   11ed0:	e1926603 	orrs	r6, r2, r3, lsl #12
   11ed4:	11a00002 	movne	r0, r2
   11ed8:	11a01003 	movne	r1, r3
   11edc:	1a000006 	bne	11efc <__aeabi_dmul+0x280>
   11ee0:	e0211003 	eor	r1, r1, r3
   11ee4:	e2011102 	and	r1, r1, #-2147483648	; 0x80000000
   11ee8:	e381147f 	orr	r1, r1, #2130706432	; 0x7f000000
   11eec:	e381160f 	orr	r1, r1, #15728640	; 0xf00000
   11ef0:	e3a00000 	mov	r0, #0
   11ef4:	e8bd4070 	pop	{r4, r5, r6, lr}
   11ef8:	e12fff1e 	bx	lr
   11efc:	e381147f 	orr	r1, r1, #2130706432	; 0x7f000000
   11f00:	e381173e 	orr	r1, r1, #16252928	; 0xf80000
   11f04:	e8bd4070 	pop	{r4, r5, r6, lr}
   11f08:	e12fff1e 	bx	lr

00011f0c <__aeabi_ddiv>:
   11f0c:	e92d4070 	push	{r4, r5, r6, lr}
   11f10:	e3a0c0ff 	mov	ip, #255	; 0xff
   11f14:	e38ccc07 	orr	ip, ip, #1792	; 0x700
   11f18:	e01c4a21 	ands	r4, ip, r1, lsr #20
   11f1c:	101c5a23 	andsne	r5, ip, r3, lsr #20
   11f20:	1134000c 	teqne	r4, ip
   11f24:	1135000c 	teqne	r5, ip
   11f28:	0b00005e 	bleq	120a8 <__aeabi_ddiv+0x19c>
   11f2c:	e0444005 	sub	r4, r4, r5
   11f30:	e021e003 	eor	lr, r1, r3
   11f34:	e1925603 	orrs	r5, r2, r3, lsl #12
   11f38:	e1a01601 	lsl	r1, r1, #12
   11f3c:	0a00004c 	beq	12074 <__aeabi_ddiv+0x168>
   11f40:	e1a03603 	lsl	r3, r3, #12
   11f44:	e3a05201 	mov	r5, #268435456	; 0x10000000
   11f48:	e1853223 	orr	r3, r5, r3, lsr #4
   11f4c:	e1833c22 	orr	r3, r3, r2, lsr #24
   11f50:	e1a02402 	lsl	r2, r2, #8
   11f54:	e1855221 	orr	r5, r5, r1, lsr #4
   11f58:	e1855c20 	orr	r5, r5, r0, lsr #24
   11f5c:	e1a06400 	lsl	r6, r0, #8
   11f60:	e20e1102 	and	r1, lr, #-2147483648	; 0x80000000
   11f64:	e1550003 	cmp	r5, r3
   11f68:	01560002 	cmpeq	r6, r2
   11f6c:	e2a440fd 	adc	r4, r4, #253	; 0xfd
   11f70:	e2844c03 	add	r4, r4, #768	; 0x300
   11f74:	2a000001 	bcs	11f80 <__aeabi_ddiv+0x74>
   11f78:	e1b030a3 	lsrs	r3, r3, #1
   11f7c:	e1a02062 	rrx	r2, r2
   11f80:	e0566002 	subs	r6, r6, r2
   11f84:	e0c55003 	sbc	r5, r5, r3
   11f88:	e1b030a3 	lsrs	r3, r3, #1
   11f8c:	e1a02062 	rrx	r2, r2
   11f90:	e3a00601 	mov	r0, #1048576	; 0x100000
   11f94:	e3a0c702 	mov	ip, #524288	; 0x80000
   11f98:	e056e002 	subs	lr, r6, r2
   11f9c:	e0d5e003 	sbcs	lr, r5, r3
   11fa0:	20466002 	subcs	r6, r6, r2
   11fa4:	21a0500e 	movcs	r5, lr
   11fa8:	2180000c 	orrcs	r0, r0, ip
   11fac:	e1b030a3 	lsrs	r3, r3, #1
   11fb0:	e1a02062 	rrx	r2, r2
   11fb4:	e056e002 	subs	lr, r6, r2
   11fb8:	e0d5e003 	sbcs	lr, r5, r3
   11fbc:	20466002 	subcs	r6, r6, r2
   11fc0:	21a0500e 	movcs	r5, lr
   11fc4:	218000ac 	orrcs	r0, r0, ip, lsr #1
   11fc8:	e1b030a3 	lsrs	r3, r3, #1
   11fcc:	e1a02062 	rrx	r2, r2
   11fd0:	e056e002 	subs	lr, r6, r2
   11fd4:	e0d5e003 	sbcs	lr, r5, r3
   11fd8:	20466002 	subcs	r6, r6, r2
   11fdc:	21a0500e 	movcs	r5, lr
   11fe0:	2180012c 	orrcs	r0, r0, ip, lsr #2
   11fe4:	e1b030a3 	lsrs	r3, r3, #1
   11fe8:	e1a02062 	rrx	r2, r2
   11fec:	e056e002 	subs	lr, r6, r2
   11ff0:	e0d5e003 	sbcs	lr, r5, r3
   11ff4:	20466002 	subcs	r6, r6, r2
   11ff8:	21a0500e 	movcs	r5, lr
   11ffc:	218001ac 	orrcs	r0, r0, ip, lsr #3
   12000:	e195e006 	orrs	lr, r5, r6
   12004:	0a00000d 	beq	12040 <__aeabi_ddiv+0x134>
   12008:	e1a05205 	lsl	r5, r5, #4
   1200c:	e1855e26 	orr	r5, r5, r6, lsr #28
   12010:	e1a06206 	lsl	r6, r6, #4
   12014:	e1a03183 	lsl	r3, r3, #3
   12018:	e1833ea2 	orr	r3, r3, r2, lsr #29
   1201c:	e1a02182 	lsl	r2, r2, #3
   12020:	e1b0c22c 	lsrs	ip, ip, #4
   12024:	1affffdb 	bne	11f98 <__aeabi_ddiv+0x8c>
   12028:	e3110601 	tst	r1, #1048576	; 0x100000
   1202c:	1a000006 	bne	1204c <__aeabi_ddiv+0x140>
   12030:	e1811000 	orr	r1, r1, r0
   12034:	e3a00000 	mov	r0, #0
   12038:	e3a0c102 	mov	ip, #-2147483648	; 0x80000000
   1203c:	eaffffd5 	b	11f98 <__aeabi_ddiv+0x8c>
   12040:	e3110601 	tst	r1, #1048576	; 0x100000
   12044:	01811000 	orreq	r1, r1, r0
   12048:	03a00000 	moveq	r0, #0
   1204c:	e254c0fd 	subs	ip, r4, #253	; 0xfd
   12050:	835c0c07 	cmphi	ip, #1792	; 0x700
   12054:	8affff43 	bhi	11d68 <__aeabi_dmul+0xec>
   12058:	e055c003 	subs	ip, r5, r3
   1205c:	0056c002 	subseq	ip, r6, r2
   12060:	01b0c0a0 	lsrseq	ip, r0, #1
   12064:	e2b00000 	adcs	r0, r0, #0
   12068:	e0a11a04 	adc	r1, r1, r4, lsl #20
   1206c:	e8bd4070 	pop	{r4, r5, r6, lr}
   12070:	e12fff1e 	bx	lr
   12074:	e20ee102 	and	lr, lr, #-2147483648	; 0x80000000
   12078:	e18e1621 	orr	r1, lr, r1, lsr #12
   1207c:	e09440ac 	adds	r4, r4, ip, lsr #1
   12080:	c074500c 	rsbsgt	r5, r4, ip
   12084:	c1811a04 	orrgt	r1, r1, r4, lsl #20
   12088:	c8bd4070 	popgt	{r4, r5, r6, lr}
   1208c:	c12fff1e 	bxgt	lr
   12090:	e3811601 	orr	r1, r1, #1048576	; 0x100000
   12094:	e3a0e000 	mov	lr, #0
   12098:	e2544001 	subs	r4, r4, #1
   1209c:	eaffff31 	b	11d68 <__aeabi_dmul+0xec>
   120a0:	e185e006 	orr	lr, r5, r6
   120a4:	eaffff2f 	b	11d68 <__aeabi_dmul+0xec>
   120a8:	e00c5a23 	and	r5, ip, r3, lsr #20
   120ac:	e134000c 	teq	r4, ip
   120b0:	0135000c 	teqeq	r5, ip
   120b4:	0affff90 	beq	11efc <__aeabi_dmul+0x280>
   120b8:	e134000c 	teq	r4, ip
   120bc:	1a000006 	bne	120dc <__aeabi_ddiv+0x1d0>
   120c0:	e1904601 	orrs	r4, r0, r1, lsl #12
   120c4:	1affff8c 	bne	11efc <__aeabi_dmul+0x280>
   120c8:	e135000c 	teq	r5, ip
   120cc:	1affff83 	bne	11ee0 <__aeabi_dmul+0x264>
   120d0:	e1a00002 	mov	r0, r2
   120d4:	e1a01003 	mov	r1, r3
   120d8:	eaffff87 	b	11efc <__aeabi_dmul+0x280>
   120dc:	e135000c 	teq	r5, ip
   120e0:	1a000004 	bne	120f8 <__aeabi_ddiv+0x1ec>
   120e4:	e1925603 	orrs	r5, r2, r3, lsl #12
   120e8:	0affff68 	beq	11e90 <__aeabi_dmul+0x214>
   120ec:	e1a00002 	mov	r0, r2
   120f0:	e1a01003 	mov	r1, r3
   120f4:	eaffff80 	b	11efc <__aeabi_dmul+0x280>
   120f8:	e1906081 	orrs	r6, r0, r1, lsl #1
   120fc:	11926083 	orrsne	r6, r2, r3, lsl #1
   12100:	1affff48 	bne	11e28 <__aeabi_dmul+0x1ac>
   12104:	e1904081 	orrs	r4, r0, r1, lsl #1
   12108:	1affff74 	bne	11ee0 <__aeabi_dmul+0x264>
   1210c:	e1925083 	orrs	r5, r2, r3, lsl #1
   12110:	1affff5e 	bne	11e90 <__aeabi_dmul+0x214>
   12114:	eaffff78 	b	11efc <__aeabi_dmul+0x280>

00012118 <__gedf2>:
   12118:	e3e0c000 	mvn	ip, #0
   1211c:	ea000002 	b	1212c <__cmpdf2+0x4>

00012120 <__ledf2>:
   12120:	e3a0c001 	mov	ip, #1
   12124:	ea000000 	b	1212c <__cmpdf2+0x4>

00012128 <__cmpdf2>:
   12128:	e3a0c001 	mov	ip, #1
   1212c:	e52dc004 	push	{ip}		; (str ip, [sp, #-4]!)
   12130:	e1a0c081 	lsl	ip, r1, #1
   12134:	e1f0cacc 	mvns	ip, ip, asr #21
   12138:	e1a0c083 	lsl	ip, r3, #1
   1213c:	11f0cacc 	mvnsne	ip, ip, asr #21
   12140:	0a00000e 	beq	12180 <__cmpdf2+0x58>
   12144:	e28dd004 	add	sp, sp, #4
   12148:	e190c081 	orrs	ip, r0, r1, lsl #1
   1214c:	0192c083 	orrseq	ip, r2, r3, lsl #1
   12150:	11310003 	teqne	r1, r3
   12154:	01300002 	teqeq	r0, r2
   12158:	03a00000 	moveq	r0, #0
   1215c:	012fff1e 	bxeq	lr
   12160:	e3700000 	cmn	r0, #0
   12164:	e1310003 	teq	r1, r3
   12168:	51510003 	cmppl	r1, r3
   1216c:	01500002 	cmpeq	r0, r2
   12170:	21a00fc3 	asrcs	r0, r3, #31
   12174:	31e00fc3 	mvncc	r0, r3, asr #31
   12178:	e3800001 	orr	r0, r0, #1
   1217c:	e12fff1e 	bx	lr
   12180:	e1a0c081 	lsl	ip, r1, #1
   12184:	e1f0cacc 	mvns	ip, ip, asr #21
   12188:	1a000001 	bne	12194 <__cmpdf2+0x6c>
   1218c:	e190c601 	orrs	ip, r0, r1, lsl #12
   12190:	1a000004 	bne	121a8 <__cmpdf2+0x80>
   12194:	e1a0c083 	lsl	ip, r3, #1
   12198:	e1f0cacc 	mvns	ip, ip, asr #21
   1219c:	1affffe8 	bne	12144 <__cmpdf2+0x1c>
   121a0:	e192c603 	orrs	ip, r2, r3, lsl #12
   121a4:	0affffe6 	beq	12144 <__cmpdf2+0x1c>
   121a8:	e49d0004 	pop	{r0}		; (ldr r0, [sp], #4)
   121ac:	e12fff1e 	bx	lr

000121b0 <__aeabi_cdrcmple>:
   121b0:	e1a0c000 	mov	ip, r0
   121b4:	e1a00002 	mov	r0, r2
   121b8:	e1a0200c 	mov	r2, ip
   121bc:	e1a0c001 	mov	ip, r1
   121c0:	e1a01003 	mov	r1, r3
   121c4:	e1a0300c 	mov	r3, ip
   121c8:	eaffffff 	b	121cc <__aeabi_cdcmpeq>

000121cc <__aeabi_cdcmpeq>:
   121cc:	e92d4001 	push	{r0, lr}
   121d0:	ebffffd4 	bl	12128 <__cmpdf2>
   121d4:	e3500000 	cmp	r0, #0
   121d8:	43700000 	cmnmi	r0, #0
   121dc:	e8bd4001 	pop	{r0, lr}
   121e0:	e12fff1e 	bx	lr

000121e4 <__aeabi_dcmpeq>:
   121e4:	e52de008 	str	lr, [sp, #-8]!
   121e8:	ebfffff7 	bl	121cc <__aeabi_cdcmpeq>
   121ec:	03a00001 	moveq	r0, #1
   121f0:	13a00000 	movne	r0, #0
   121f4:	e49de008 	ldr	lr, [sp], #8
   121f8:	e12fff1e 	bx	lr

000121fc <__aeabi_dcmplt>:
   121fc:	e52de008 	str	lr, [sp, #-8]!
   12200:	ebfffff1 	bl	121cc <__aeabi_cdcmpeq>
   12204:	33a00001 	movcc	r0, #1
   12208:	23a00000 	movcs	r0, #0
   1220c:	e49de008 	ldr	lr, [sp], #8
   12210:	e12fff1e 	bx	lr

00012214 <__aeabi_dcmple>:
   12214:	e52de008 	str	lr, [sp, #-8]!
   12218:	ebffffeb 	bl	121cc <__aeabi_cdcmpeq>
   1221c:	93a00001 	movls	r0, #1
   12220:	83a00000 	movhi	r0, #0
   12224:	e49de008 	ldr	lr, [sp], #8
   12228:	e12fff1e 	bx	lr

0001222c <__aeabi_dcmpge>:
   1222c:	e52de008 	str	lr, [sp, #-8]!
   12230:	ebffffde 	bl	121b0 <__aeabi_cdrcmple>
   12234:	93a00001 	movls	r0, #1
   12238:	83a00000 	movhi	r0, #0
   1223c:	e49de008 	ldr	lr, [sp], #8
   12240:	e12fff1e 	bx	lr

00012244 <__aeabi_dcmpgt>:
   12244:	e52de008 	str	lr, [sp, #-8]!
   12248:	ebffffd8 	bl	121b0 <__aeabi_cdrcmple>
   1224c:	33a00001 	movcc	r0, #1
   12250:	23a00000 	movcs	r0, #0
   12254:	e49de008 	ldr	lr, [sp], #8
   12258:	e12fff1e 	bx	lr

0001225c <__aeabi_d2iz>:
   1225c:	e1a02081 	lsl	r2, r1, #1
   12260:	e2922602 	adds	r2, r2, #2097152	; 0x200000
   12264:	2a00000c 	bcs	1229c <__aeabi_d2iz+0x40>
   12268:	5a000009 	bpl	12294 <__aeabi_d2iz+0x38>
   1226c:	e3e03e3e 	mvn	r3, #992	; 0x3e0
   12270:	e0532ac2 	subs	r2, r3, r2, asr #21
   12274:	9a00000a 	bls	122a4 <__aeabi_d2iz+0x48>
   12278:	e1a03581 	lsl	r3, r1, #11
   1227c:	e3833102 	orr	r3, r3, #-2147483648	; 0x80000000
   12280:	e1833aa0 	orr	r3, r3, r0, lsr #21
   12284:	e3110102 	tst	r1, #-2147483648	; 0x80000000
   12288:	e1a00233 	lsr	r0, r3, r2
   1228c:	12600000 	rsbne	r0, r0, #0
   12290:	e12fff1e 	bx	lr
   12294:	e3a00000 	mov	r0, #0
   12298:	e12fff1e 	bx	lr
   1229c:	e1900601 	orrs	r0, r0, r1, lsl #12
   122a0:	1a000002 	bne	122b0 <__aeabi_d2iz+0x54>
   122a4:	e2110102 	ands	r0, r1, #-2147483648	; 0x80000000
   122a8:	03e00102 	mvneq	r0, #-2147483648	; 0x80000000
   122ac:	e12fff1e 	bx	lr
   122b0:	e3a00000 	mov	r0, #0
   122b4:	e12fff1e 	bx	lr

Disassembly of section .fini:

000122b8 <_fini>:
   122b8:	e1a0c00d 	mov	ip, sp
   122bc:	e92ddff8 	push	{r3, r4, r5, r6, r7, r8, r9, sl, fp, ip, lr, pc}
   122c0:	e24cb004 	sub	fp, ip, #4
   122c4:	e24bd028 	sub	sp, fp, #40	; 0x28
   122c8:	e89d6ff0 	ldm	sp, {r4, r5, r6, r7, r8, r9, sl, fp, sp, lr}
   122cc:	e12fff1e 	bx	lr
