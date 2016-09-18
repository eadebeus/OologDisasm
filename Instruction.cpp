#include "stdafx.h"
#include "Instruction.h"


Instruction::Instruction(int offset, int size)
{
    offset_ = offset;
    size_ = size;
    offsetNext_ = offset_ + size_;
}

Instruction::~Instruction(void)
{
}

Instruction *Instruction::Read(istream &source, int offset)
{
    Byte opcode, byteArg, byteIndex;
    Word wordArg, wordIndex;
    Dword dwordIndex;
    int padding;
    string descriptor;
    
    if(!opcode.Read(source))
        return (Instruction *) SetError();
    switch((int) opcode) {
        case NOP:
        case ACONSTNUL: case ICONSTM1: case ICONST0: case ICONST1:
        case ICONST2: case ICONST3: case ICONST4: case ICONST5:
        case LCONST0: case LCONST1: case FCONST0: case FCONST1:
        case FCONST2: case DCONST0: case DCONST1:
        case ILOAD0: case ILOAD1: case ILOAD2: case ILOAD3:
        case LLOAD0: case LLOAD1: case LLOAD2: case LLOAD3:
        case FLOAD0: case FLOAD1: case FLOAD2: case FLOAD3:
        case DLOAD0: case DLOAD1: case DLOAD2: case DLOAD3:
        case ALOAD0: case ALOAD1: case ALOAD2: case ALOAD3:
        case IALOAD: case LALOAD: case FALOAD: case DALOAD:
        case AALOAD: case BALOAD: case CALOAD: case SALOAD:
        case ISTORE0: case ISTORE1:    case ISTORE2: case ISTORE3: 
        case LSTORE0: case LSTORE1: case LSTORE2: case LSTORE3: 
        case FSTORE0: case FSTORE1: case FSTORE2: case FSTORE3: 
        case DSTORE0: case DSTORE1: case DSTORE2: case DSTORE3: 
        case ASTORE0: case ASTORE1: case ASTORE2: case ASTORE3: 
        case IASTORE: case LASTORE: case FASTORE: case DASTORE: 
        case AASTORE: case BASTORE: case CASTORE: case SASTORE: 
        case POP: case POP2: case DUP: case DUPX1: 
        case DUPX2: case DUP2: case DUP2X1: case DUP2X2: 
        case SWAP: 
        case IADD: case LADD: case FADD: case DADD: 
        case ISUB: case LSUB: case FSUB: case DSUB: 
        case IMUL: case LMUL: case FMUL: case DMUL: 
        case IDIV: case LDIV: case FDIV: case DDIV: 
        case IREM: case LREM: case FREM: case DREM: 
        case INEG: case LNEG: case FNEG: case DNEG: 
        case ISHL: case LSHL: case ISHR: case LSHR: 
        case IUSHR: case LUSHR: case IAND: case LAND: 
        case IOR: case LOR: case IXOR: case LXOR:
        case I2L: case I2F: case I2D: case L2I:
        case L2F: case L2D: case F2I: case F2L:
        case F2D: case D2I: case D2L: case D2F:
        case I2B: case I2C: case I2S: case LCMP:
        case FCMPL: case FCMPG: case DCMPL: case DCMPG:
        case IRETURN: case LRETURN: case FRETURN: case DRETURN:
        case ARETURN: case RETURN: case MONENTER: case MONEXIT:
        case ARRAYLENGTH: case ATHROW: case 186:
            return new Instruction(offset, 1);
        case BIPUSH:
            if(!byteArg.Read(source))
                return (Instruction *) SetError();
            return new Instruction(offset, 2);
        case SIPUSH: case IFNULL: case IFNONNULL:
            if(!wordArg.Read(source))
                return (Instruction *)SetError();
            return new Instruction(offset, 3);
        case GETSTATIC:    case PUTSTATIC:    case GETFIELD:    case PUTFIELD:
        case VIRTUAL: case SPECIAL:    case STATIC: case LDCW:
        case LDCW2: case NEW: case ANEWARRAY: case CHECKCAST: 
        case INSTANCEOF:
            if (!wordIndex.Read(source))
                return (Instruction *)SetError();
            return new Instruction(offset, 3);
        case ILOAD:    case LLOAD:    case FLOAD:    case DLOAD:    
        case ALOAD:    case ISTORE: case LSTORE: case FSTORE: 
        case DSTORE: case ASTORE: case LDC: case RET:
            if(!byteIndex.Read(source))
                return (Instruction *) SetError();
            return new Instruction(offset, 2);
        case IINC:
            if(!byteIndex.Read(source))
                return (Instruction *) SetError();
            if(!byteArg.Read(source))
                return (Instruction *) SetError();
            return new Instruction(offset, 3);
        case IFEQ: case IFNE: case IFLT: case IFGE:
        case IFGT: case IFLE: case IFICMPEQ: case IFICMPNE: 
        case IFICMPLT: case IFICMPGE: case IFICMPGT: case IFICMPLE: 
        case IFACMPEQ: case IFACMPNE: case GOTO: case JSR:
            if(!wordArg.Read(source))
                return (Instruction *) SetError();
            return new Instruction(offset, 3);
        case TBLSWITCH:
            padding = (-offset - 1) & 0x03;
            source.ignore(padding);
            return new TableSwitchInstruction(offset, padding + 1, source);
        case LKUPSWITCH:
            padding = (-offset - 1) & 0x03;
            source.ignore(padding);
            return new LookupSwitchInstruction(offset, padding + 1, source);
        case INTERFACE:
            if(!wordIndex.Read(source))
                return (Instruction *) SetError();
            // Read and ignore 'nargs' byte
            if(!byteArg.Read(source))
                return (Instruction *) SetError();
            // Read and ignore 0 padding byte
            if(!byteArg.Read(source))
                return (Instruction *) SetError();
            return new Instruction(offset, 5);
        case NEWARRAY:
            if(!byteArg.Read(source))
                return (Instruction *) SetError();    
            switch((int) byteArg) {
                case 4: descriptor = "[Z"; break;
                case 5: descriptor = "[C"; break;
                case 6: descriptor = "[F"; break;
                case 7: descriptor = "[D"; break;
                case 8: descriptor = "[B"; break;
                case 9: descriptor = "[S"; break;
                case 10: descriptor = "[I"; break;
                case 11: descriptor = "[J"; break;
                default:
                    return (Instruction *) SetError();
            }
            return new Instruction(offset, 2);
        case WIDE:
            if(!byteArg.Read(source))
                return (Instruction *) SetError();
            if(!wordIndex.Read(source))
                return (Instruction *) SetError();
            switch((int) byteArg) {
                case ILOAD: case LLOAD: case FLOAD: case DLOAD: 
                case ALOAD: case ISTORE: case LSTORE: case FSTORE: 
                case DSTORE: case ASTORE: case RET:
                    return new Instruction(offset, 4);
                case IINC:
                    if(!wordArg.Read(source))
                        return (Instruction *) SetError();
                    return new Instruction(offset, 6);
                default:
                    return (Instruction *) SetError();
            }            
            break;
        case MNEWARRAY:
            if(!wordIndex.Read(source))
                return (Instruction *) SetError();
            if(!byteArg.Read(source))
                return (Instruction *) SetError();
            return new Instruction(offset, 4);
        case GOTOW:
        case JSRW:
            if(!dwordIndex.Read(source))
                return (Instruction *) SetError();
            return new Instruction(offset, 5);
        case BREAK:
        case IMPDEP1:
        case IMPDEP2:
            return new Instruction(offset, 1);
        default:
            return (Instruction *) SetError();
    }
}

int Instruction::Size(void)
{
    return size_;
}


TableSwitchInstruction::TableSwitchInstruction(int offset, int size, istream &source)
    : Instruction(offset, size)
{
    Dword def, low, high, addr;
     
    table_ = (int *) 0;

    if(!def.Read(source))
        RetError();
    default_ = offset + (int) def;

    if(!low.Read(source))
        RetError();
    low_ = (int) low;

    if(!high.Read(source))
        RetError();
    high_ = (int) high;

    tableSize_ = high_ - low_ + 1;
    if(!(table_ = new int[tableSize_]))
        RetError();
    for(int i = 0; i < tableSize_; i++) {
        if(!addr.Read(source))
            RetError();
        table_[i] = offset + (int) addr;
    }
}

TableSwitchInstruction::~TableSwitchInstruction(void)
{
    delete [] table_;
}

int TableSwitchInstruction::Size(void)
{
    return (size_ + 4 * (tableSize_ + 3));
}
 

LookupSwitchInstruction::LookupSwitchInstruction(int offset, int size, istream &source)
    : Instruction(offset, size)
{
    Dword def, count, key, addr;
     
    keyTable_ = (int *) 0;
    addrTable_ = (int *) 0;

    if(!def.Read(source))
        RetError();
    default_ = offset + (int) def;

    if(!count.Read(source))
        RetError();
    tableSize_ = (int) count;

    if(!(keyTable_ = new int[tableSize_]))
        RetError();
    if(!(addrTable_ = new int[tableSize_]))
        RetError();
    for(int i = 0; i < tableSize_; i++) {
        if(!key.Read(source))
            RetError();
        keyTable_[i] = (int) key;
        if(!addr.Read(source))
            RetError();
        addrTable_[i] = offset + (int) addr;
    }
}

LookupSwitchInstruction::~LookupSwitchInstruction(void)
{
    delete [] keyTable_;
    delete [] addrTable_;
}

int LookupSwitchInstruction::Size(void)
{
    return (size_ + 8 * (tableSize_ + 1));
}
