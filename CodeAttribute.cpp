#include "stdafx.h"
#include "Basic.h"
#include "CodeAttribute.h"


CodeAttribute::CodeAttribute(istream &source, Class *cls)
{
    /*
     * CodeAttribute structure:
     *
     *  word attributeName;
     *  dword attributeLength;
     *  word maxStack;
     *  word maxLocals;
     *  dword codeLength;
     *  byte code[codeLength];
     *  word exceptionTableLength;
     *  ExceptionItem exceptions[exceptionTableLength];
     *  word attributeCount;
     *  AttributeInfo attributes[attributeCount];
     */

    codeBytes_ = NULL;
    exceptions_ = NULL;

    // Read maxStack
    Word tmp;
    if(!tmp.Read(source))
        RetError();
    maxStack_ = (int) tmp;

    // Read maxLocals
    if(!tmp.Read(source))
        RetError();
    maxLocals_ = (int) tmp;

    // Read code
    Dword codeLength;
    if(!codeLength.Read(source))
        RetError();
    codeLength_ = codeLength;

    if(!(codeBytes_ = new byte[codeLength_]))
        RetError();
    if(!source.read((char *) codeBytes_, codeLength_) || source.gcount() != (int) codeLength_)
        RetError();
    source.seekg(- ((int) codeLength_), ios::cur);
    if(!(code_ = new Code(codeLength)))
        RetError();
    Instruction *ins;
    for(int offset = 0; offset < (int) codeLength_; offset += ins->Size()) {
        ins = Instruction::Read(source, offset);
        if(!ins || ErrorSet()) {
            DumpError();
            return;
        }
        code_->SetInstruction(ins);
    }

    // Read exceptions
    Word count;
    if(!count.Read(source))
        RetError();
    exceptionCount_ = count;
    if(exceptionCount_ > 0) {
        if(!(exceptions_ = new ExceptionItem[exceptionCount_]))
            RetError();
        for(int i = 0; i < exceptionCount_; i++) {
            if(!exceptions_[i].Read(source, cls))
                RetError();
        }
    }    
    code_->SetExceptions(exceptions_);
    
    // Read attributes
    if(!count.Read(source))
        RetError();
    attributeCount_ = count;
    /*
     * AttributeInfo structure:
     * 
     *  word attributeName;
     *  dword attributeLength;
     *  byte info[attributeLength];
     */
    for(int i = 0; i < attributeCount_; i++) {
        Word index;
        if(!index.Read(source))
            RetError();
        Dword attributeLength;
        if(!attributeLength.Read(source))
            RetError();
        char *attrName = (char *) *(cls->Constant(index));
        if(attrName && !strcmp(attrName, "LocalVariableTable"))
            source.ignore((int)attributeLength);
        else if(attrName && !strcmp(attrName, "LineNumberTable"))
            source.ignore((int) attributeLength);
        else 
            // Ignore any other attributes.
            source.ignore((int) attributeLength);
        delete attrName;
    }
}

CodeAttribute::~CodeAttribute(void)
{
    delete [] codeBytes_;
    delete [] exceptions_;
}

void CodeAttribute::Dump(ostream &sink)
{
    sink << Indent << "CodeAttribute ";
    sink << "(maxStack " << maxStack_ << " maxLocals " << maxLocals_ << ")\r\n";
    sink << IncreaseIndent;
    sink << Indent << "Code:\r\n";
    sink << IncreaseIndent;
    for(int offset = 0; offset < (int) codeLength_; ) {
        int i, padding, base, def, lo, hi, n, addr, key;
        short branch;
        sink << Indent << offset << ": ";
        switch(codeBytes_[offset]) {
            case NOP: case ACONSTNUL: case ICONSTM1: case ICONST0:
            case ICONST1: case ICONST2: case ICONST3: case ICONST4:
            case ICONST5: case LCONST0: case LCONST1: case FCONST0:
            case FCONST1: case FCONST2: case DCONST0: case DCONST1:
            case ILOAD0: case ILOAD1: case ILOAD2: case ILOAD3:
            case LLOAD0: case LLOAD1: case LLOAD2: case LLOAD3:
            case FLOAD0: case FLOAD1: case FLOAD2: case FLOAD3:
            case DLOAD0: case DLOAD1: case DLOAD2: case DLOAD3:
            case ALOAD0: case ALOAD1: case ALOAD2: case ALOAD3:
            case IALOAD: case LALOAD: case FALOAD: case DALOAD:
            case AALOAD: case BALOAD: case CALOAD: case SALOAD:
            case ISTORE0: case ISTORE1: case ISTORE2: case ISTORE3:
            case LSTORE0: case LSTORE1: case LSTORE2: case LSTORE3:
            case FSTORE0: case FSTORE1: case FSTORE2: case FSTORE3:
            case DSTORE0: case DSTORE1: case DSTORE2: case DSTORE3:
            case ASTORE0: case ASTORE1: case ASTORE2: case ASTORE3:
            case IASTORE: case LASTORE: case FASTORE: case DASTORE:
            case AASTORE: case BASTORE: case CASTORE: case SASTORE:
            case POP: case POP2: case DUP: case DUPX1:
            case DUPX2: case DUP2: case DUP2X1: case DUP2X2:
            case SWAP: case IADD: case LADD: case FADD:
            case DADD: case ISUB: case LSUB: case FSUB:
            case DSUB: case IMUL: case LMUL: case FMUL:
            case DMUL: case IDIV: case LDIV: case FDIV:
            case DDIV: case IREM: case LREM: case FREM:
            case DREM: case INEG: case LNEG: case FNEG:
            case DNEG: case ISHL: case LSHL: case ISHR:
            case LSHR: case IUSHR: case LUSHR: case IAND:
            case LAND: case IOR: case LOR: case IXOR:
            case LXOR: case I2L: case I2F: case I2D:
            case L2I: case L2F: case L2D: case F2I:
            case F2L: case F2D: case D2I: case D2L:
            case D2F: case I2B: case I2C: case I2S:
            case LCMP: case FCMPL: case FCMPG: case DCMPL:
            case DCMPG: case IRETURN: case LRETURN:    case FRETURN:
            case DRETURN: case ARETURN: case RETURN: case ARRAYLENGTH:
            case ATHROW: case MONENTER: case MONEXIT: case BREAK:
            case IMPDEP1: case IMPDEP2:
                sink << OpCodeMap[(OpCode)codeBytes_[offset]] << "\r\n";
                offset++;
                break;
            case BIPUSH: case LDC: case ILOAD: case LLOAD:
            case FLOAD: case DLOAD: case ALOAD: case ISTORE:
            case LSTORE: case FSTORE: case DSTORE: case ASTORE:
            case RET:
                sink << OpCodeMap[(OpCode)codeBytes_[offset]] << " ";
                if(codeBytes_[offset] == LDC)
                    sink << "#";
                offset++;
                sink << (int)codeBytes_[offset] << "\r\n"; offset++;
                break;
            case SIPUSH: case LDCW: case LDCW2: case GETSTATIC:
            case PUTSTATIC: case GETFIELD: case PUTFIELD: case VIRTUAL:
            case SPECIAL: case STATIC: case NEW: case CHECKCAST:
            case INSTANCEOF:
                sink << OpCodeMap[(OpCode)codeBytes_[offset]] << " ";
                if(codeBytes_[offset] != SIPUSH)
                    sink << "#";
                offset++;
                sink << (int) (((int)codeBytes_[offset] << 8) + (int) codeBytes_[offset + 1]) << "\r\n";
                offset += 2;
                break;
            case IFEQ: case IFNE: case IFLT: case IFGE:
            case IFGT: case IFLE: case IFICMPEQ: case IFICMPNE:
            case IFICMPLT: case IFICMPGE: case IFICMPGT: case IFICMPLE:
            case IFACMPEQ: case IFACMPNE: case GOTO: case JSR:
            case IFNULL: case IFNONNULL:
                sink << OpCodeMap[(OpCode)codeBytes_[offset]] << " ";
                offset++;
                branch = ((short)codeBytes_[offset] << 8) + (short) codeBytes_[offset + 1];
                sink << (offset - 1 + branch) << "\r\n";
                offset += 2;
                break;
            case IINC:
                sink << OpCodeMap[(OpCode)codeBytes_[offset]] << " ";
                offset++;
                sink << (int)codeBytes_[offset] << " ";
                offset++;
                sink << (int)codeBytes_[offset] << "\r\n";
                offset++;
                break;
            case GOTOW:
            case JSRW:
                sink << OpCodeMap[(OpCode)codeBytes_[offset]] << " ";
                offset++;
                sink << ReadIntFromByteArray(codeBytes_, offset) << "\r\n";
                offset += 4;
                break;
            case TBLSWITCH:
                base = offset;
                padding = (-offset - 1) & 0x03;
                offset++;
                offset += padding;
                def = ReadIntFromByteArray(codeBytes_, offset);
                def += base;
                offset += 4;
                lo = ReadIntFromByteArray(codeBytes_, offset);
                offset += 4;
                hi = ReadIntFromByteArray(codeBytes_, offset);
                offset += 4;
                sink << "tableswitch " << lo << " to " << hi << ":\r\n";
                sink << IncreaseIndent;
                n = hi - lo + 1;
                for(i = 0; i < n; i++) {
                    addr = ReadIntFromByteArray(codeBytes_, offset);
                    addr += base;
                    offset += 4;
                    sink << Indent << (lo + i) << ": " << addr << "\r\n";
                }
                sink << Indent << "default: " << def << "\r\n";
                sink << DecreaseIndent;
                break;
            case LKUPSWITCH:
                base = offset;
                padding = (-offset - 1) & 0x03;
                offset++;
                offset += padding;
                def = ReadIntFromByteArray(codeBytes_, offset);
                def += base;
                offset += 4;
                n = ReadIntFromByteArray(codeBytes_, offset);
                offset += 4;
                sink << "lookupswitch " << n << ":\r\n";
                sink << IncreaseIndent;
                for(i = 0; i < n; i++) {
                    key = ReadIntFromByteArray(codeBytes_, offset);
                    offset += 4;
                    addr = ReadIntFromByteArray(codeBytes_, offset);
                    addr += base;
                    offset += 4;
                    sink << Indent << key << ": " << addr << "\r\n";
                }
                sink << Indent << "default: " << def << "\r\n";
                sink << DecreaseIndent;
                break;
            case INTERFACE:
                sink << OpCodeMap[(OpCode)codeBytes_[offset]] << " #";
                offset++;
                sink << (int) (((int) codeBytes_[offset] << 8) + (int) codeBytes_[offset + 1]) << " ";
                offset += 2;
                sink << (int) codeBytes_[offset];
                offset += 2;
                break;
            case NEWARRAY:
                sink << OpCodeMap[(OpCode)codeBytes_[offset]] << " ";
                offset++;
                switch((int) codeBytes_[offset]) {
                    case 4: sink << "boolean\r\n"; break;
                    case 5: sink << "char\r\n"; break;
                    case 6: sink << "float\r\n"; break;
                    case 7: sink << "double\r\n"; break;
                    case 8: sink << "byte\r\n"; break;
                    case 9: sink << "short\r\n"; break;
                    case 10: sink << "int\r\n"; break;
                    case 11: sink << "long\r\n"; break;
                    default: break;
                }
                offset++;
                break;
            case ANEWARRAY:
                sink << OpCodeMap[(OpCode)codeBytes_[offset]] << " #"; 
                offset++;
                sink << (int) (((int) codeBytes_[offset] << 8) + (int) codeBytes_[offset + 1]) << "\r\n";
                offset += 2;
                break;
            case MNEWARRAY:
                sink << OpCodeMap[(OpCode)codeBytes_[offset]] << " #"; 
                offset++;
                sink << (int)(((int)codeBytes_[offset] << 8) + (int)codeBytes_[offset + 1]) << " ";
                offset += 2;
                sink << (int)codeBytes_[offset] << "\r\n"; offset++;
                break;
            case WIDE:
                offset++;
                switch((int) codeBytes_[offset]) {
                    case ILOAD: case LLOAD: case FLOAD: case DLOAD:
                    case ALOAD: case ISTORE: case LSTORE: case FSTORE:
                    case DSTORE: case ASTORE: case RET:
                        sink << OpCodeMap[(OpCode)codeBytes_[offset]] << " ";
                         offset++;
                        sink << (int) (((int) codeBytes_[offset] << 8) + (int) codeBytes_[offset + 1]) << "\r\n";
                        offset += 2;
                        break;
                    case IINC:
                        sink << OpCodeMap[(OpCode)codeBytes_[offset]] << " "; 
                        offset++;
                        sink << (int) (((int) codeBytes_[offset] << 8) + (int) codeBytes_[offset + 1]) << " ";
                        offset += 2;
                        sink << (int) (((int) codeBytes_[offset] << 8) + (int) codeBytes_[offset + 1]) << "\r\n";
                        offset += 2;
                    default:
                        break;
                }            
                break;
            default:
                break;
        }
    }
    sink << DecreaseIndent;
    if(exceptionCount_ > 0) {
        sink << IncreaseIndent;
        sink << Indent << "Exceptions:\r\n";
        sink << IncreaseIndent;
        for(int i = 0; i < exceptionCount_; i++)
            exceptions_[i].Dump(sink);
        sink << DecreaseIndent;
        sink << DecreaseIndent;
    }
    sink << DecreaseIndent;
}

int CodeAttribute::ReadIntFromByteArray(byte *codeBytes, int offset)
{
    return (((int)codeBytes[offset] << 24) + ((int)codeBytes[offset + 1] << 16) + ((int)codeBytes[offset + 2] << 8) + ((int)codeBytes[offset + 3]));
}
