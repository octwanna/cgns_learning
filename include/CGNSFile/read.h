#ifndef READ_H
#define READ_H

void verify(int toCheck, int correct, char *errorMessage);
int simpleReadFile(cgns_unstructured_file *data);
void readFile(cgns_unstructured_file *data);
void readBase(cgns_unstructured_file *data);
void readZone(cgns_unstructured_file *data);
void readGrid(cgns_unstructured_file *data);
void readGridCoordinates(cgns_unstructured_file *data);

#endif
