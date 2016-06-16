def div(r0, scst):
	r3 = scst;
	t1 = r0*r3;
	print("\tr0 = " + str(r0));
	print("\tr3 = " + str(hex(r3)));
	print("\tr0*r3 = " + str(hex(t1)));
	print("smull r2, r3, r0, r3");
	r3 = t1 >> 32;
	print("\tr3 = " + str(r3) + " (" + str(hex(r3)) + ")");

	print("\nadd r3, r3, r0");
	r3 = r3 + r0;
	print("\tr3 = " + str(r3));

	print("\nmov r0, r0, ASR#31");
	print("rsb r0, r0, r3, ASR#2");
	if(r0 < 0):
		r0 = -1;
	else:
		r0 = 0;
	r0 = r3/4 - r0;
	print("\tr0 = " + str(r0));
	

def div7(r0):
	print("Computing " + str(r0) + "/7");
	div(r0, -0x6DB6DB6D); # -(3/7) << 32
	return;

#for i in range(0,33):
#	print("r0*r3 >> " + str(i) + str(bin(r0*r3>>i)))

