#pragma once

int parse_cmdline(int argc, char **argv);

extern char* fileToBeRead;
extern int drawOnlyBestPath;
extern int interactiveMode;
extern int useDPAlgo;
extern int GRIDROWS;
extern int GRIDCOLS;
extern int useDijkstra;
extern int coloredMap; 
extern int noGreedy;