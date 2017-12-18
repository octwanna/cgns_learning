#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "CGNSFile/structure.h"

void getUserInput(int argc, char *argv[], cgns_unstructured_file *data);

void generateCGNSFile(cgns_unstructured_file *data);
void generateBase(cgns_unstructured_file *data);

void generateZone(cgns_unstructured_file *data);
void generateCoordinates(cgns_unstructured_file *data);
void generateElementsConnectivity(cgns_unstructured_file *data);

void generateZone2D(cgns_unstructured_file *data);
void generateCoordinates2D(cgns_unstructured_file *data);
void generateElementsConnectivity2D(cgns_unstructured_file *data);

/* Private */
void verifyUserInput(int argc);
void getNumberOfVerticesFromUserInput(int argc, char *argv[], cgns_unstructured_file *data);
void getDistancesFromUserInput(int argc, char *argv[], cgns_unstructured_file *data);

#endif
