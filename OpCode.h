#ifndef _OPCODE_H
#define _OPCODE_H
/*
 * Byte codes
 * 
 * Notes:
 *  JVM state registers:
 *   PC points to next byte code to execute
 *   VARS points to method's local variables
 *   OPTOP points to operand stack
 *   FRAME points to execution environment
 */

#include <map>
using namespace std;

enum OpCode {
    NOP = 0,            /* nop */
    ACONSTNUL = 1,      /* push null object onto operand stack */
    ICONSTM1 = 2,       /* push integer -1 */
    ICONST0 = 3,        /* push integer 0 */
    ICONST1 = 4,        /* push integer 1 */
    ICONST2 = 5,        /* push integer 2 */
    ICONST3 = 6,        /* push integer 3 */
    ICONST4 = 7,        /* push integer 4 */
    ICONST5 = 8,        /* push integer 5 */
    LCONST0 = 9,        /* push long 0 */
    LCONST1 = 10,       /* push long 1 */
    FCONST0 = 11,       /* push float 0 */
    FCONST1 = 12,       /* push float 1 */
    FCONST2 = 13,       /* push float 2 */
    DCONST0 = 14,       /* push double 0 */
    DCONST1 = 15,       /* push double 1 */
    BIPUSH = 16,        /* push following byte */
    SIPUSH = 17,        /* push following two bytes */
    LDC = 18,           /* push item from CP indexed by following byte */
    LDCW = 19,          /* push item from CP indexed by following 2 bytes */
    LDCW2 = 20,         /* push long or double from CP indexed by following 2 bytes */
    ILOAD = 21,         /* push integer from VARS[following byte] */
    LLOAD = 22,         /* push long from VARS[following byte] and VARS[following byte + 1] */
    FLOAD = 23,         /* push float from VARS[following byte] */
    DLOAD = 24,         /* push double from VARS[following byte] and VARS[following byte + 1] */
    ALOAD = 25,         /* push object from VARS[following byte */
    ILOAD0 = 26,        /* push integer from VARS[0] */
    ILOAD1 = 27,        /* push integer from VARS[1] */
    ILOAD2 = 28,        /* push integer from VARS[2] */
    ILOAD3 = 29,        /* push integer from VARS[3] */
    LLOAD0 = 30,        /* push long from VARS[0] and VARS[1] */
    LLOAD1 = 31,        /* push long VARS[1] and VARS[2] */
    LLOAD2 = 32,        /* push long from VARS[2] and VARS[3] */
    LLOAD3 = 33,        /* push long from VARS[3] and VARS[4] */
    FLOAD0 = 34,        /* push float from VARS[0] */
    FLOAD1 = 35,        /* push float from VARS[1] */
    FLOAD2 = 36,        /* push float from VARS[2] */
    FLOAD3 = 37,        /* push float from VARS[3] */
    DLOAD0 = 38,        /* push double from VARS[0] and VARS[1] */
    DLOAD1 = 39,        /* push double from VARS[1] and VARS[2] */
    DLOAD2 = 40,        /* push double from VARS[2] and VARS[3] */
    DLOAD3 = 41,        /* push double from VARS[3] and VARS[4] */
    ALOAD0 = 42,        /* push object from VARS[0] */
    ALOAD1 = 43,        /* push object from VARS[1] */
    ALOAD2 = 44,        /* push object from VARS[2] */
    ALOAD3 = 45,        /* push object from VARS[3] */
    IALOAD = 46,        /* pop index and array, push array[index] int */
    LALOAD = 47,        /* pop index and array, push array[index] long */
    FALOAD = 48,        /* pop index and array, push array[index] float */
    DALOAD = 49,        /* pop index and array, push array[index] double */
    AALOAD = 50,        /* pop index and array, push array[index] object */
    BALOAD = 51,        /* pop index and array, push array[index] byte */
    CALOAD = 52,        /* pop index and array, push array[index] char */
    SALOAD = 53,        /* pop index and array, push array[index] short */
    ISTORE = 54,        /* pop integer into VARS[following byte] */
    LSTORE = 55,        /* pop long into VARS[following byte] and VARS[following byte + 1] */
    FSTORE = 56,        /* pop float into VARS[following byte] */
    DSTORE = 57,        /* pop double into VARS[following byte] and VARS[following byte + 1] */
    ASTORE = 58,        /* pop object into VARS[following byte] */
    ISTORE0 = 59,       /* pop integer into VARS[0] */
    ISTORE1 = 60,       /* pop integer into VARS[1] */
    ISTORE2 = 61,       /* pop integer into VARS[2] */
    ISTORE3 = 62,       /* pop integer into VARS[3] */
    LSTORE0 = 63,       /* pop long into VARS[0] and VARS[1] */
    LSTORE1 = 64,       /* pop long VARS[1] and VARS[2] */
    LSTORE2 = 65,       /* pop long into VARS[2] and VARS[3] */
    LSTORE3 = 66,       /* pop long into VARS[3] and VARS[4] */
    FSTORE0 = 67,       /* pop float into VARS[0] */
    FSTORE1 = 68,       /* pop float into VARS[1] */
    FSTORE2 = 69,       /* pop float into VARS[2] */
    FSTORE3 = 70,       /* pop float into VARS[3] */
    DSTORE0 = 71,       /* pop double into VARS[0] and VARS[1] */
    DSTORE1 = 72,       /* pop double into VARS[1] and VARS[2] */
    DSTORE2 = 73,       /* pop double into VARS[2] and VARS[3] */
    DSTORE3 = 74,       /* pop double into VARS[3] and VARS[4] */
    ASTORE0 = 75,       /* pop object into VARS[0] */
    ASTORE1 = 76,       /* pop object into VARS[1] */
    ASTORE2 = 77,       /* pop object into VARS[2] */
    ASTORE3 = 78,       /* pop object into VARS[3] */
    IASTORE = 79,       /* pop integer, index, and array, store integer into array[index] */
    LASTORE = 80,       /* pop long, index, and array, store long into array[index] */
    FASTORE = 81,       /* pop float, index, and array, store float into array[index] */
    DASTORE = 82,       /* pop double, index, and array, store double into array[index] */
    AASTORE = 83,       /* pop object, index, and array, store object into array[index] */
    BASTORE = 84,       /* pop byte, index, and array, store byte into array[index] */
    CASTORE = 85,       /* pop char, index, and array, store char into array[index] */
    SASTORE = 86,       /* pop short, index, and array, store short into array[index] */
    POP = 87,           /* pop top item off operand stack */
    POP2 = 88,          /* pop top 2 items off operand stack */
    DUP = 89,           /* dup top item on operand stack: a1 --> a1 a1 */
    DUPX1 = 90,         /* stack: a1 a2 --> a2 a1 a2 */
    DUPX2 = 91,         /* stack: a1 a2 a3 --> a3 a1 a2 a3 */
    DUP2 = 92,          /* stack: a1 a2 --> a1 a2 a1 a2 */
    DUP2X1 = 93,        /* stack: a1 a2 a3 --> a2 a3 a1 a2 a3 */
    DUP2X2 = 94,        /* stack: a1 a2 a3 a4 --> a3 a4 a1 a2 a3 a4 */
    SWAP = 95,          /* stack: a1 a2 --> a2 a1 */
    IADD = 96,          /* pop 2 integers, add, and push result */
    LADD = 97,          /* pop 2 longs, add, and push result */
    FADD = 98,          /* pop 2 floats, add, and push result */
    DADD = 99,          /* pop 2 doubles, add, and push result */
    ISUB = 100,         /* pop 2 integers, subtract, and push result */
    LSUB = 101,         /* pop 2 longs, subtract, and push result */
    FSUB = 102,         /* pop 2 floats, subtract, and push result */
    DSUB = 103,         /* pop 2 doubles, subtract, and push result */
    IMUL = 104,         /* pop 2 integers, multiply, and push result */
    LMUL = 105,         /* pop 2 longs, multiply, and push result */
    FMUL = 106,         /* pop 2 floats, multiply, and push result */
    DMUL = 107,         /* pop 2 doubles, multiply, and push result */
    IDIV = 108,         /* pop 2 integers, divide, and push result */
    LDIV = 109,         /* pop 2 longs, divide, and push result */
    FDIV = 110,         /* pop 2 floats, divide, and push result */
    DDIV = 111,         /* pop 2 doubles, divide, and push result */
    IREM = 112,         /* pop 2 integers, modulo, and push result */
    LREM = 113,         /* pop 2 longs, modulo, and push result */
    FREM = 114,         /* pop 2 floats, modulo, and push result */
    DREM = 115,         /* pop 2 doubles, modulo, and push result */
    INEG = 116,         /* pop 2 integers, negate, and push result */
    LNEG = 117,         /* pop 2 longs, negate, and push result */
    FNEG = 118,         /* pop 2 floats, negate, and push result */
    DNEG = 119,         /* pop 2 doubles, negate, and push result */
    ISHL = 120,         /* pop 2 ints, push one left-shift the other */
    LSHL = 121,         /* pop 2 longs, push one left-shift the other */
    ISHR = 122,         /* pop 2 ints, push one right-shift the other */
    LSHR = 123,         /* pop 2 longs, push one right-shift the other */
    IUSHR = 124,        /* pop 2 ints, push one logical right-shift the other */
    LUSHR = 125,        /* pop 2 longs, push one unsigned right-shift the other */
    IAND = 126,         /* pop 2 ints, push one AND the other */
    LAND = 127,         /* pop 2 longs, push one AND the other */
    IOR = 128,          /* pop 2 ints, push one OR the other */
    LOR = 129,          /* pop 2 longs, push one OR the other */
    IXOR = 130,         /* pop 2 ints, push one XOR the other */
    LXOR = 131,         /* pop 2 longs, push one XOR the other */
    IINC = 132,         /* increment VARS[following byte] by constant following the following byte */
    I2L =  133,         /* pop integer, convert to long, push */
    I2F = 134,          /* pop integer, convert to float, push */
    I2D = 135,          /* pop integer, convert to double, push */
    L2I = 136,          /* pop long, convert to integer, push */
    L2F = 137,          /* pop long, convert to float, push */
    L2D = 138,          /* pop long, convert to double, push */
    F2I = 139,          /* pop float, convert to integer, push */
    F2L = 140,          /* pop float, convert to long, push */
    F2D = 141,          /* pop float, convert to double, push */
    D2I = 142,          /* pop double, convert to integer, push */
    D2L = 143,          /* pop double, convert to long, push */
    D2F = 144,          /* pop double, convert to float, push */
    I2B = 145,          /* pop integer, convert to byte, push */
    I2C = 146,          /* pop integer, convert to char, push */
    I2S = 147,          /* pop integer, convert to short, push */
    LCMP = 148,         /* pop 2 longs, push comparison (-1, 0, or 1) */
    FCMPL = 149,        /* pop 2 floats, push comparison (-1, 0, or 1) */
    FCMPG = 150,        /* pop 2 floats, push comparison (-1, 0, or 1) */
    DCMPL = 151,        /* pop 2 doubles, push comparison (-1, 0, or 1) */
    DCMPG = 152,        /* pop 2 doubles, push comparison (-1, 0, or 1) */
    IFEQ = 153,         /* pop int, if 0, branch to 16-bit offset from PC given by 2 following bytes */
    IFNE = 154,         /* pop int, if not 0, branch to following offset */
    IFLT = 155,         /* pop int, if < 0, branch to following offset */
    IFGE = 156,         /* pop int, if >= 0, branch to following offset */
    IFGT = 157,         /* pop int, if > 0, branch to following offset */
    IFLE = 158,         /* pop int, if <= 0, branch to following offset */
    IFICMPEQ =159,      /* pop 2 ints, compare if equal... */
    IFICMPNE =160,      /* pop 2 ints, compare if not equal... */
    IFICMPLT =161,      /* pop 2 ints, compare if one < the other */
    IFICMPGE =162,      /* pop 2 ints, compare if one >= the other */
    IFICMPGT =163,      /* pop 2 ints, compare if one > the other */
    IFICMPLE =164,      /* pop 2 ints, compare if one <= the other */
    IFACMPEQ =165,      /* pop 2 objects, compare if equal... */
    IFACMPNE =166,      /* pop 2 objects, compare if not equal... */
    GOTO = 167,         /* unconditional branch to 16-bit following offset */
    JSR = 168,          /* branch to subroutine with 16-bit following offset, push return address */
    RET = 169,          /* return from subroutine, using return address VARS[following byte] */
    TBLSWITCH = 170,    /* tableswitch: access jump table by index and jump.
                           opcode is followed by 0-3 bytes of padding to get
                           rest of args aligned, then 4-byte default addr, then
                           4-byte low, then 4-byte high, then high-low+1 4-byte
                           offset addrs. index is popped of stack and if >= low
                           and <= high, opcode plus offset at index-low is
                           branched to, else opcode plus default offset. */
    LKUPSWITCH = 171,   /* lookupswitch: access jump table by key and jump.
                           opcode is followed by 0-3 bytes of padding to get
                           rest of args aligned, then 4-byte default addr, 4-byte
                           count, then count sets of 8-byte pairs of 4-byte key
                           and 4-byte offset addr. key is popped of stack and matched
                           against all keys, then if match branch to opcode plus
                           offset addr, else to opcode plus default. */
    IRETURN = 172,      /* pop int, and return from subroutine, pushing int onto caller's operand stack */
    LRETURN = 173,      /* pop long, and return from subroutine, pushing long onto caller's operand stack */
    FRETURN = 174,      /* pop float, and return from subroutine, pushing float onto caller's operand stack */
    DRETURN = 175,      /* pop double, and return from subroutine, pushing double onto caller's operand stack */
    ARETURN = 176,      /* pop object, and return from subroutine, pushing object onto caller's operand stack */
    RETURN = 177,       /* return from subroutine */
    GETSTATIC = 178,    /* push class field given by following 2 bytes of CP index */
    PUTSTATIC = 179,    /* pop value and put into class field referenced by following 2 bytes of CP index */
    GETFIELD = 180,     /* pop object, push object's field given by following 2 bytes of CP index */
    PUTFIELD = 181,     /* pop value, objectref, and put value into field of object referenced by following 2 bytes of CP index */
    VIRTUAL = 182,      /* invokevirtual: object and method args 
                           are on stack, use 2 following index bytes to look 
                           up method in object, pop object and args and
                           store into method's local variables, branch */
    SPECIAL = 183,      /* invokespecial: used for super and private methods */
    STATIC = 184,       /* invokestatic: used for static methods */
    INTERFACE = 185,    /* invokeinterface: object and args are on stack, use
                           2 following index bytes to look up interface method,
                           number of args byte follows the 2 following index bytes;
                           and is followed by a 0 byte. */
                        /*** 186 is unused ***/
    NEW = 187,          /* push object from following 2 bytes of index */
    NEWARRAY = 188,     /* pop size, push array[size] of following type byte,
                           which must be one of: 4=boolean, 5=char, 6=float,
                           7=double, 8=byte, 9=short, 10=int, 11=long */
    ANEWARRAY = 189,    /* pop size, push array[size] of objects given by 2 following bytes of constant pool index */
    ARRAYLENGTH = 190,  /* pop array, push its length */        
    ATHROW = 191,       /* pop object and throw exception */
    CHECKCAST = 192,    /* pop object, throw exception if can't be cast to object given by 2 following bytes of constant pool index */
    INSTANCEOF = 193,   /* pop object, push 1 or 0 if is or is not an instance of object from following 2 index bytes */
    MONENTER = 194,     /* monitorenter: enter monitor for object. pop
                           object, thread waits for object's monitor then
                           takes possession of it, or increments reference
                           count if alread has possession. */
    MONEXIT = 195,      /* monitorexit: exit monitor for object. thread
                           decrements reference count, and releases monitor
                           it's zero. */
    WIDE = 196,         /* modifies the following opcode so that it takes 2
                           following bytes of CP index instead of 1. can be used
                           with ILOAD, FLOAD, ALOAD, LLOAD, DLOAD, ISTORE,
                           FSTORE, ASTORE, LSTORE, DSTORE, or RET. alternate
                           form, used only with IINC, double the size of both
                           the index and the constant. */
    MNEWARRAY = 197,    /* multinewarray: pop n sizes, push multi-dimensional
                           array[][] of objects given by 2 following bytes of
                           class index; 3rd following byte is n. */
    IFNULL = 198,       /* pop object, if null, branch to following offset */
    IFNONNULL = 199,    /* pop object, if not null, branch to following offset */
    GOTOW = 200,        /* unconditional branch to 32-bit following offset, although
                           method =size is limited to 65535 bytes of code...*/
    JSRW = 201,         /* branch to subroutine with 32-bit following offset, push return address */
    BREAK = 202,        /* reserved for calling breakpoint handler */
                        /*** 203-228 re 'quick' opcodes ***/
                        /*** 229-253 are unused ***/
    IMPDEP1 = 254,      /* reserved for implementation-dependent functionality */
    IMPDEP2 = 255       /* reserved for implementation-dependent functionality */
};

extern std::map<OpCode, string> OpCodeMap;

#endif
