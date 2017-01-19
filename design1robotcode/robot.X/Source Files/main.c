#include <htc.h>
#include <PICinit.h>
#include <PICinit.c>
main (void)
{
configurePIC(); //Initialize the PIC board configuration
while (1)
{
}
}
void interrupt isr (void)
{
return;
}