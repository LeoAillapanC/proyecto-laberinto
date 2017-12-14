#include <cstdio>
#include "bitmap_image.hpp"
#include <fstream>
#include <string>


int main()
{
   bitmap_image image("Tres.bmp");
   int i=0,c=0, j=0, X=0, Y=0;
   std::string st, add,s;
   if (!image)
   {
      printf("Error - Failed to open: input.bmp\n");
      return 1;
   }
    std::fstream fs;

   unsigned int total_number_of_pixels = 0;
unsigned int cordenadaX=0;
unsigned int cordenadaY=0;
   const unsigned int height = image.height();
   const unsigned int width  = image.width();
   fs.open ("lab.txt", std::fstream::in | std::fstream::out | std::fstream::app);
   j=0;
for (std::size_t y = 0; y < height; ++y ){
   /* code */
        for (std::size_t x = 0; x < width; ++x)
      {

         rgb_t colour;
         cordenadaX= x;
         cordenadaY= y;

         image.get_pixel(x, y, colour);
//   printf(" X: %d ", cordenadaX);
  //    printf(" Y : %d\n", cordenadaY);

        if (colour.blue >= 111){// lee color blanco ? rojo?
          fs << "1";// acepta que hay color blanco porque contiene todos los colores. negro ausencia de color.
          /* code */
      }else{
        fs << " ";// acepta que hay color blanco porque contiene todos los colores. negro ausencia de color.
        /* code */
      }
      i++;
          //  total_number_of_pixels++;
      }
      fs<<"\n";

      j++;
  }
   fs.close();

   printf("Done! >= 111: %d\n",total_number_of_pixels);
   printf("%d\n", i );
   printf("%d\n", j );

   return 0;
}
