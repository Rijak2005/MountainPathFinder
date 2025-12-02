// This file handles the commandline input and parses arguments
#include "../header/cmdline.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <argp.h>


const char *argp_program_version = "v1.0.0";
char doc[] = "A simple app that calculates the shortest path through the terrain based on the given topographic data";
char args_doc[] = "ROWS COLUMNS";

struct argp_option options[] = {
    {"file", 'f', "input file", 0, "The file to read the topographic data from"},
    {"draw-only-best-path", 'b', 0, 0, "Draw only the best GREEDY path found"},
    {"interactive", 'i', 0, 0, "Switch to interactive mode where the user can click to set a start point for pathfinding"},
    {"dynamic-programming", 'd', 0, 0, "Use dynamic programming algorithm to find the best path"},
    {"dijkstra", 'z', 0, 0, "Use dijkstra algorithm to find best path"},
    {"colored-map", 'c', 0, 0, "Use a colored map to visualize the terrain"},
    {"no Greedy", 'o', 0 , 0, "Dont draw GREEDY"},
    {0}};

error_t parse_options(int key, char *arg, struct argp_state *state)
{
  switch (key)
  {
    case 'f':
      fileToBeRead = strdup(arg);
      break;
    case 'b':
      drawOnlyBestPath = 1;
      break;
    case 'i':
      interactiveMode = 1;
      break;
    case 'd':
      useDPAlgo = 1;
      break;
    case 'z':
      useDijkstra = 1;
      break;
    case 'c':
      coloredMap = 1;
      break;
    case 'o':
      noGreedy = 1;
      break;
    case ARGP_KEY_ARG:
      if (state->arg_num >= 2)
        /* Too many arguments. */
        argp_usage (state);
      if (state->arg_num == 0){
        GRIDROWS = atoi(arg);
      } else if (state-> arg_num == 1){
        GRIDCOLS = atoi(arg);
      }
      break;

    case ARGP_KEY_END:
      if (state->arg_num < 2)
        /* Not enough arguments. */
        argp_usage (state);
      break;

    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}

int parse_cmdline(int argc, char **argv) {
	struct argp parser = { options, parse_options, args_doc, doc };
	int ret = argp_parse(&parser, argc, argv, 0, 0, 0);
}

char* fileToBeRead = "../../data/testMountains.dat";
int drawOnlyBestPath = 0; // Flag to indicate if only the best path should be drawn
int interactiveMode = 0; // Flag to indicate if the program is in interactive mode
int useDPAlgo = 0; // Flag to indicate if the dynamic programming algorithm should be used
int GRIDROWS = 100;
int GRIDCOLS = 100;
int useDijkstra = 0; // Flag to indicate if the dijkstra algorithm should be used
int coloredMap = 0; // Flag to indicate if a colored map should be drawn
int noGreedy = 0;