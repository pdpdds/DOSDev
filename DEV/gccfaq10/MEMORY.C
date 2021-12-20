#include <dpmi.h>

main()
{
 printf("Physical memory availible %d\n", _go32_dpmi_remaining_physical_memory);
 printf("Virtual memory availible %d\n", _go32_dpmi_remaining_virtual_memory);
}
