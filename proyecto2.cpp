#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <cstring>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////
// CLASS BMPimage
//////////////////////////////////////////////////////////////////////////////

// La clase BMPimage sólo reconoce archivos BMP blanco y negro de 1 bit

#define Bits_in_a_BYTE 8
#define DWORD 4                   // Cantidad de BYTES que ocupa un DWORD
#define Reading_Pack Bits_in_a_BYTE*DWORD

#define BYTE_base 0x100L          // == 256: Base numérica que se obtiene al 
                                  // representar números con una base igual al
                                  // total de números representables con 1 BYTE
// Potencias 0, 1, 2, 3, de BYTE_base
#define B0 1L
#define B1 B0*BYTE_base
#define B2 B1*BYTE_base
#define B3 B2*BYTE_base

#define MAX_PARAMS 0x001A
#define BMP_Identifier   0x0000L
#define BMP_FileSize     0x0002L
#define BMP_ResLaterUse  0x0006L
#define BMP_DataOffset   0x000aL
#define BMP_HeaderSize   0x000eL
#define BMP_PixelsWidth  0x0012L
#define BMP_PixelsHeight 0x0016L

#define MASK (0x1UL << (Reading_Pack - 1))

// Lo importante en la siguiente definición es que INTEGER sea un 
// entero de 4 BYTES. Si en su sistema el tipo INT no tiene 4 BYTES, 
// utilice otro, como LONG INT:

#define INTEGER unsigned int

class BMPimage {
   public:
     BMPimage(char *filename);
     ~BMPimage();
     
     void PrintBitMap(const char On, const char Off) const;
   protected:
     char *fname;
     ifstream f;
     int width, height;
     vector<bool> bitmap;     
};

BMPimage::BMPimage(char *filename)
{
  char parameters[MAX_PARAMS];
  union {
    char buffer[DWORD];
    unsigned char byte[DWORD];
    INTEGER bits;    

    void make_swap(void) { 
       unsigned char x; 
       x = byte[0]; byte[0] = byte[3]; byte[3] = x;
       x = byte[1]; byte[1] = byte[2]; byte[2] = x;
    }
    
    unsigned long int count(void)  {
       return B3*byte[3]+B2*byte[2]+B1*byte[1]+B0*byte[0];
    }

  } u;
  
  unsigned long begdata, enddata, width_inside_file;
  
  fname = filename;
      
  f.open(fname, ios::in | ios::binary);
                       
  f.read(parameters, sizeof(parameters));
  
  strncpy(u.buffer,parameters+BMP_PixelsWidth,DWORD);  
  width = u.count();
  
  strncpy(u.buffer,parameters+BMP_PixelsHeight,DWORD);
  height = u.count();
  
  strncpy(u.buffer,parameters+BMP_DataOffset,DWORD);
  begdata = u.count();
  
  strncpy(u.buffer,parameters+BMP_FileSize,DWORD);
  enddata = u.count();
  
  width_inside_file = (enddata - begdata) / height;
  
  for(int i = 1; i <= height; i++)
    {
      // Las líneas en el archivo BMP están guardadas de abajo hacia arriba,
      // luego conviene hacer una lectura hacia atrás, desde el fin del archivo
      f.seekg(enddata - width_inside_file*i);
      
      // Leer una línea      
      for(long int k = width; k>0; k -= Reading_Pack)
        {
          f.read(u.buffer, DWORD);
          
          // Los 4 Bytes leídos deben reordenarse 
          // porque el formato BMP originalmente los coloca invertidos           
          u.make_swap();               

          for (int j=0; j<((k>=Reading_Pack)? Reading_Pack: k); j++)
             bitmap.push_back((u.bits << j) & MASK);
        }
    }
    
  f.close();
}

BMPimage::~BMPimage()
{ 
   bitmap.clear();
   fname = NULL;
   width = 0;
   height = 0;
}

void BMPimage::PrintBitMap(const char On, const char Off) const
{
  unsigned long offset = 0;
  for (int i = 0; i < height; cout << endl, i++)
    for (int j = 0; j < width; j++, offset++)
       cout << (bitmap[offset]? On: Off);          
}

//////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
  cout << "MASK == " << hex << MASK << endl;
  
  BMPimage dibujillo("./dibujo.bmp");
  
  dibujillo.PrintBitMap(' ','#');  

  system("PAUSE");      
  
  return 0;
}
