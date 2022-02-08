#include "graph.h"

void main(void)
{
   char *(adapter_name[]) =
   {
      "MDA", "HGC", "CGA", "EGA", "VGA", "MCGA",
      "Tseng Labs. ET 3000", "Tseng Labs. ET 4000",
      "ATI VGA Wonder Rev. 1", "ATI VGA Wonder Rev. 2"
   };
   int adapter, svga_chip;

   adapter = detect_adapter();
   if(adapter == 0)
      printf("There is no video adapter.\n");
   else
      printf("Current video adapter is %s.\n", adapter_name[adapter-1]);
   if(adapter == VGA)
   {
      if((adapter = detect_svga()) != VGA)
      {
     printf("SVGA detected. Chip name is %s.\n", adapter_name[adapter-1]);
      }
   }
}
