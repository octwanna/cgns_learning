#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "CGNSFile/structure.h"

void generate_zone(cgns_unstructured_file *data);
void generateCoordinates(cgns_unstructured_file *data);
void generateElementsConnectivity(cgns_unstructured_file *data);

#endif
