#include "stdafx.h"

#include "Basic.h"
#include "Error.h"
#include "Class.h"

//
// Byte-code disassembler. Disassembles java class file into "Oolong" assembly language listing.
//
// This program is based on the Java Virtual Machine spec., as described in Lindholm, Yellin, et al.
//

int _tmain(int argc, _TCHAR* argv[])
{
    Class cls;

    if(argc != 2) {
        printf("Usage: OolongDisasm <classfilename>\r\n");
        exit(1);
    }
    cout << "Oolong Disassembler\r\n";

    ifstream source(argv[1], ios::binary);
    source.seekg(ios::beg);

    wcout << "Starting " << argv[1] << " ..." << endl << endl;
        
    if(!cls.Read(source))
        return DisplayError("Error reading class");
        
    cls.Dump(cout);
        
    cout << "\r\n ...Done.";

    return 0;
}

