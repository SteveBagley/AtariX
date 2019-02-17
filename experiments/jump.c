void* sBaseAddr;
#define uint   unsigned int

typedef struct
{
	uint cpu_type;     /* CPU Type: 68000, 68008, 68010, 68EC020, or 68020 */
	uint dar[16];      /* Data and Address Registers */
	uint ppc;		   /* Previous program counter */
} m68ki_cpu_core;



m68ki_cpu_core m68ki_cpu = {0};

// MagicMacX specific
void m68k_op_call_emu_proc(void)
{
	unsigned a0, a1;
	unsigned char *p;
	typedef unsigned tfHostCall(unsigned a1, unsigned char *emubase);
	tfHostCall *proc;

	a0 = m68ki_cpu.dar[8];	// hopefully in host's endianess mode
	a1 = m68ki_cpu.dar[9];
	p = sBaseAddr + a0;		// address in host's address range
	// geht nicht:
	proc = *((tfHostCall *)(p));
	a0 = *((unsigned *) (p + 0));
	proc = (tfHostCall *) a0;
	// call host function. Put return value into d0 (all in host endian-mode)
	m68ki_cpu.dar[0] = proc(a1, sBaseAddr);
}

