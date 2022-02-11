int	beep2[] = {
		15000,1,
		12000,1,
		 9000,1,
		 7000,1,
		 3000,1,
		 1000,1,
		  300,1,
		  100,1,
		  0,0
	};

void	main() {

	take_timer();
	beep_seq(beep2);
	getch();
	give_timer();
}
