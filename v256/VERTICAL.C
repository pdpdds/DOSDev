void vertical_retrace(void)
{
 asm mov dx,3dah;

VR_Address_OK:asm {
		   in al,dx
		   test al,08h
		   jz Wait_For_On
		   }
Wait_For_Off: asm {
		   in al,dx
		   test al,08h
		   jnz Wait_For_Off
		   }
Wait_For_On:  asm {
		  in al,dx
		  test al,08h
		  jz Wait_For_On
		  }
}
