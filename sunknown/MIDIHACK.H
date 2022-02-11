/* MIDIHACK.H
***
** Ronny Hanssen
***
** Inneholder 3 rutiner for midi-trafikk
** - smidistart()       // Initialiserer gus-kortet for midi-fil.
** - smidistopp()       // Resetter midi-trafikk.
** - smidigo()          // Kj›rer Šn l›kke med sjekk for midi-events.
*/

void smidistart(void);
void smidireset(void);
void smidigo(void);
