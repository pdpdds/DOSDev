#include <stdio.h>
#include <string.h>

#include "sok.h"
#include "vga320.h"

int readscreen(MAPINFO *map)
{
    FILE *fp;
    char buf[20];
    int x=0, y=0, destno = 0, c;

    map->manx = map->many = -1;
    map->maxx = map->maxy = map->blockno = map->mandir = 0;
    memset( map->map, 0, MAX_MAP_X * MAX_MAP_Y );
    sprintf( buf, "screen.%d", map->level);

    if( ( fp = fopen( buf, "r") ) == NULL )
	return -1;

    while( ( (c = getc( fp) ) != EOF) ) {
	switch( c ) {
	    case '#' :
		map->map[ x++ ][ y ] = WALL;
		break;
	    case '*' :
		map->map[ x++ ][ y ] = BLOCK+DEST;
		break;
	    case ' ' :
		map->map[ x++ ][ y ] = EMPTY;
		break;
	    case '\n' :
		map->maxy++;
		y++;
		if( x > map->maxx )
		    map->maxx = x;
		x = 0;
		break;
	    case '$' :
		map->blockno++;
		map->map[ x++ ][ y ] = BLOCK;
		break;
	    case '.' :
		destno++;
		map->map[ x++ ][ y ] = DEST;
		break;
	    case '@' :
		map->map[ x ][ y ] = MAN;
		map->manx = x++;
		map->many = y;
	    default : // ignore. \t, \r  ignored.
		;
	}
	if( y >= MAX_MAP_X || x >= MAX_MAP_Y ) {
	    fclose( fp );
	    return -1;
	}
    }

    fclose( fp );

    if( map->blockno != destno  || map->manx == -1 || map->many == -1
	|| map->blockno == 0 )
	return -2;
    map->mapoffx = MAX_MAP_X/2 - map->maxx/2;
    map->mapoffy = MAX_MAP_Y/2 - map->maxy/2;
    return 0;
}
