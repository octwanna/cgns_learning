#ifndef READ_H
#define READ_H

#include "CGNSFile/structure.h"

void verify(int toCheck, int correct, char *errorMessage);
int simpleReadFile2(cgns_unstructured_file *data, Dimension dimension);
int simpleReadFile(cgns_unstructured_file *data) __attribute__ ((deprecated));
void readFile2(cgns_unstructured_file *data, Dimension dimension);
void readFile(cgns_unstructured_file *data) __attribute__ ((deprecated));
void readBase2(cgns_unstructured_file *data, Dimension dimension);
void readBase(cgns_unstructured_file *data) __attribute__ ((deprecated));
void readZone(cgns_unstructured_file *data);
void readGrid(cgns_unstructured_file *data);
void readGridCoordinates2(cgns_unstructured_file *data, Dimension dimension);
void readGridCoordinates(cgns_unstructured_file *data) __attribute__ ((deprecated));

#endif
