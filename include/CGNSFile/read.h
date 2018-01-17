#ifndef READ_H
#define READ_H

#include "CGNSFile/structure.h"

void verify(int toCheck, int correct, char *errorMessage);
void simpleReadFile(cgns_unstructured_file *data, Dimension dimension);
void readFile(cgns_unstructured_file *data, Dimension dimension);
void readBase(cgns_unstructured_file *data, Dimension dimension);
void readZone(cgns_unstructured_file *data);
void readGrid(cgns_unstructured_file *data);
void readGridCoordinates(cgns_unstructured_file *data, Dimension dimension);

#endif
