/* Hovertank 3-D Source Code
 * Copyright (C) 1993-2014 Flat Rock Software
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

//
// From Sound Editor v1.31 by Lane Roath
//

#define FIRESND 1
#define BUMPWALLSND 2
#define AFTERBURNSND 3
#define SHOOTWALLSND 4
#define SHOOTTHINGSND 5
#define SAVEHOSTAGESND 6
#define HOSTAGEDEADSND 7
#define LOWTIMESND 8
#define TAKEDAMAGESND 9
#define NUKESND 10
#define WARPGATESND 11
#define PLAYERDEADSND 12
#define GUNREADYSND 13
#define MAXPOWERSND 14
#define LASTHOSTAGESND 15
#define ARMORUPSND 16
#define TIMESCORESND 17
#define GUYSCORESND 18
#define STARTGAMESND 19
#define HIGHSCORESND 20

// as: Support for extra sound effects

// Mutant attack and death sounds use existing sounds: TAKEDAMAGE and SHOOTTHING
// Tank missile hitting wall uses existing sound SHOOTWALLSND

/// <summary>Mutant attacks player.</summary>
#define SNDEX_MUTEDAMAGE (TAKEDAMAGESND)

/// <summary>Mutant dies.</summary>
#define SNDEX_MUTEDIE (SHOOTTHINGSND)

/// <summary>Drone attacks player (explosion).</summary>
#define SNDEX_DRONEDAMAGE (21)

/// <summary>Drone dies.</summary>
#define SNDEX_DRONEDIE (22)

/// <summary>Tank shoots a missile.</summary>
#define SNDEX_TANKFIRE (23)

/// <summary>Tank missile hits player.</summary>
#define SNDEX_TANKDAMAGE (24)

/// <summary>Tank dies.</summary>
#define SNDEX_TANKDIE (25)

/// <summary>Last male refugee killed.</summary>
#define SNDEX_LASTDEAD1 (26)

/// <summary>Last female refugee killed.</summary>
#define SNDEX_LASTDEAD2 (27)

/// <summary>Last male refugee killed by player.</summary>
#define SNDEX_LASTDEAD3 (28)

/// <summary>Last female refugee killed by player.</summary>
#define SNDEX_LASTDEAD4 (29)

/// <summary>Female refugee saved.</summary>
#define SNDEX_SAVHOSTAGE2 (30)

/// <summary>Last refugee saved (female).</summary>
#define SNDEX_LSTHOSTAGE2 (31)

/// <summary>Female refugee killed.</summary>
#define SNDEX_HSTAGEDEAD2 (32)

/// <summary>Player killed male refugee.</summary>
#define SNDEX_HSTAGEDEAD3 (33)

/// <summary>Player killed female refugee.</summary>
#define SNDEX_HSTAGEDEAD4 (34)

/// <summary>Player fires fully charged weapon.</summary>
#define SNDEX_FIRE2 (35)

/// <summary>Tank missile hits wall.</summary>
#define SNDEX_TSHOTWALL (SHOOTWALLSND)

/// <summary>Player missile hits wall.</summary>
#define SNDEX_PSHOTWALL (36)

/// <summary>Player fully charged missile hits wall.</summary>
#define SNDEX_PSHOTWALL2 (37)

/// <summary>Player picks up a shield object.</summary>
#define SNDEX_SHIELDUP (38)

/// <summary>The total number of sounds.</summary>
#define SNDEX_NUMSOUNDS (1 + 20 + 18)
