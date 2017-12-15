#include <stdlib.h>
#include <stdio.h>
#include <cgnslib.h>

#define CHKERRQ(err) if((err)) cg_error_exit()

#define PHYSICAL_DIMENSION 3
#define CELL_DIMENSION 3

#define NX 21
#define NY 17
#define NZ 9

int main(int argc, char *argv[])
{
	int err;
	int i, j, k, entry, elementNumber;

	int cellDimension = CELL_DIMENSION;
	int physicalDimension = PHYSICAL_DIMENSION;

	int file;
	char fileName[] = "/home/guesser/cgns_examples/output/UnstructuredGrid3D.cgns";

	int base;
	char baseName[] = "Simple structured grid";

	int zone;
	char zoneName[] = "Grid";
	cgsize_t size[3];

	double *x=NULL, *y=NULL, *z=NULL;
	int coorX, coorY, coorZ;
	char coorXName[] = "CoordinateX";
	char coorYName[] = "CoordinateY";
	char coorZName[] = "CoordinateZ";

	int section;
	char sectionName[] = "Hexaedron conectivity";

	int numberOfElements;
	cgsize_t *hexaedronConnectivity;
	cgsize_t firstElementNumber = 0;
	cgsize_t lastElementNumber;

	/* Open file */
	err = cg_open(fileName, CG_MODE_WRITE, &file); CHKERRQ(err);

	/* Open base */
	err = cg_base_write(file, baseName, cellDimension, physicalDimension, &base); CHKERRQ(err);

	/* Define zone size */
	size[0] = NX*NY*NZ;
	size[1] = (NX-1)*(NY-1)*(NZ-1);
	size[2] = 0;

	/* Open zone */
	err = cg_zone_write(file, base, zoneName, size, Unstructured, &zone);

	/* Alloc coordinates */
	x = (double *) malloc(NX*NY*NZ*sizeof(double**));
	y = (double *) malloc(NX*NY*NZ*sizeof(double**));
	z = (double *) malloc(NX*NY*NZ*sizeof(double**));
	/* Generate coordinates values */
	for(i=0 ; i<NX ; ++i)
	{
		for(j=0 ; j<NY ; ++j)
		{
			for(k=0 ; k<NZ ; ++k)
			{
				entry = i + j*NX + k*NX*NY;
				x[entry] = i;
				y[entry] = j;
				z[entry] = k;
			}
		}
	}
	/* Write coordinates */
	err = cg_coord_write(file, base, zone, CGNS_ENUMV(RealDouble), coorXName, x, &coorX); CHKERRQ(err);
	err = cg_coord_write(file, base, zone, CGNS_ENUMV(RealDouble), coorYName, y, &coorY); CHKERRQ(err);
	err = cg_coord_write(file, base, zone, CGNS_ENUMV(RealDouble), coorZName, z, &coorZ); CHKERRQ(err);

	/* Elements conectivity */
		/* 0 <= elementNumber <= (NX-1)*(NY-1)*(NZ-1) */
	numberOfElements = (NX-1)*(NY-1)*(NZ-1);
	hexaedronConnectivity = (cgsize_t *) malloc(8*numberOfElements*sizeof(cgsize_t));
	for(i=0 ; i<(NX-1) ; ++i)
	{
		for(j=0 ; j<(NY-1) ; ++j)
		{
			for(k=0 ; k<(NZ-1) ; ++k)
			{
				elementNumber = i + (NX-1)*j + (NX-1)*(NY-1)*k;
				entry = i + j*NX + k*NX*NY;
				printf("elementNumber = %d\n", elementNumber);
				hexaedronConnectivity[8*elementNumber+0] = 1 + entry;
				hexaedronConnectivity[8*elementNumber+1] = 1 + entry+1;
				hexaedronConnectivity[8*elementNumber+2] = 1 + entry+NX+1;
				hexaedronConnectivity[8*elementNumber+3] = 1 + entry+NX;
				hexaedronConnectivity[8*elementNumber+4] = 1 + entry+NX*NY;
				hexaedronConnectivity[8*elementNumber+5] = 1 + entry+NX*NY+1;
				hexaedronConnectivity[8*elementNumber+6] = 1 + entry+NX*(NY+1)+1;
				hexaedronConnectivity[8*elementNumber+7] = 1 + entry+NX*(NY+1);
			}
		}
	}
	lastElementNumber = numberOfElements;
	cg_section_write(file, base, zone, sectionName, CGNS_ENUMV(HEXA_8), firstElementNumber+1, lastElementNumber, 0, hexaedronConnectivity, &section);



	cg_close(file);

	free(hexaedronConnectivity);
	free(x);
	free(y);
	free(z);
	printf("\nSuccessfully wrote grid to file '%s'\n\n", fileName);

	exit(EXIT_SUCCESS);
}
