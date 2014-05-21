#ifndef TIFFPARSE_H
#define TIFFPARSE_H

#include <tiffio.h>
#include <iostream>

namespace KS{namespace Tiff{

class TIFFParse
{
public:
    TIFFParse();

    ~TIFFParse();
    void open(std::string fileName);

private:
    TIFF* m_tiff;
};

}}

#endif // TIFFPARSE_H
