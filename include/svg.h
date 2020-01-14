//File to convert a ladder(s) to an html doc which will contain the ladder as an svg image
//the doc will be loaded to the html page so that it can be rendered on the site combos.org
//Author: Patrick Di Salvo
//Date: Jan. 11 2020


#ifndef __SVG__
#define __SVG__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ladder.h"
#include "utilities.h"


char* ladderToSvg(Ladder* l, int* perm);

char* htmlHeader( void );

char* htmlLadder(Ladder* l, int yHeight);

char* htmlBody(List* list);


#endif