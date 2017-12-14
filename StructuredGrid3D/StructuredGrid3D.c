#include <stdlib.h>
#include <stdio.h>
#include <cgnslib.h>

#define CHKERRQ(ret) if((ret)) cg_error_exit()

#define PHYSICAL_DIMENSION 3
#define CELL_DIMENSION 3

int main(int argc, char *argv[])
{
	int err;
	int i, j, k, entry;

	int cellDimension = CELL_DIMENSION;
	int physicalDimension = PHYSICAL_DIMENSION;

	int NX, NY, NZ;
	double LX, LY, LZ;
	double dx, dy, dz;
	double xPosition, yPosition, zPosition;
	double *x, *y, *z;

	int file;
	char fileName[] = "/home/guesser/cgns_examples/output/StructuredGrid3D.cgns";

	int base;
	char baseName[] = "Simple structured grid";

	int zone;
	char zoneName[] = "Grid";
	ZoneType_t structured = CGNS_ENUMV(Structured);
	cgsize_t size[3*PHYSICAL_DIMENSION];

	int coorX, coorY, coorZ;
	char coorXName[] = "CoordinateX";
	char coorYName[] = "CoordinateY";
	char coorZName[] = "CoordinateZ";
	long double c;

	/* get user input */
	if(argc!=4 && argc!=7)
	{
		fprintf(stdout, "Usage:\n");
		fprintf(stdout, "\tprogram NX NY NZ\n");
		fprintf(stdout, "or\n");
		fprintf(stdout, "\tprogram NX NY NZ LX LY LZ\n");
		exit(EXIT_FAILURE);
	}
	NX = atoi(argv[1]);
	NY = atoi(argv[2]);
	NZ = atoi(argv[3]);
	if(argc==4)
	{
		LX = NX;
		LY = NY;
		LZ = NZ;
	}
	if(argc==7){
		LX = atof(argv[4]);
		LY = atof(argv[5]);
		LZ = atof(argv[6]);
	}
	dx = LX/(NX-1);
	dy = LY/(NY-1);
	dz = LZ/(NZ-1);

	/* Open file */
	err = cg_open(fileName, CG_MODE_WRITE, &file); CHKERRQ(err);

	/* Open base */
	err = cg_base_write(file, baseName, cellDimension, physicalDimension, &base); CHKERRQ(err);

	/* Open zone */
	/* Define zone size */
		/* vertices */
		size[0] = NZ;
		size[1] = NY;
		size[2] = NX;
		/* Cells */
		size[3] = size[0] - 1;
		size[4] = size[1] - 1;
		size[5] = size[2] - 1;
		/* Boundary vertex */
		size[6] = 0;
		size[7] = 0;
		size[8] = 0;
	err = cg_zone_write(file, base, zoneName, size, structured, &zone); CHKERRQ(err);

	/* Alloc coordinates */
	x = (double *) malloc(NX*NY*NZ*sizeof(double**));
	y = (double *) malloc(NX*NY*NZ*sizeof(double**));
	z = (double *) malloc(NX*NY*NZ*sizeof(double**));
	/* Generate coordinates values */
	for(i=0, xPosition=0 ; i<NX ; ++i, xPosition+=dx)
	{
		for(j=0, yPosition=0 ; j<NY ; ++j, yPosition+=dy)
		{
			for(k=0, zPosition=0 ; k<NZ ; ++k, zPosition+=dz)
			{
				entry = k+ j*NZ + i*NZ*NY ;
				x[entry] = xPosition;
				y[entry] = yPosition;
				z[entry] = zPosition;
			}
		}
	}
	/* Write coordinates */
	err = cg_coord_write(file, base, zone, CGNS_ENUMV(RealDouble), coorXName, x, &coorX); CHKERRQ(err);
	err = cg_coord_write(file, base, zone, CGNS_ENUMV(RealDouble), coorYName, y, &coorY); CHKERRQ(err);
	err = cg_coord_write(file, base, zone, CGNS_ENUMV(RealDouble), coorZName, z, &coorZ); CHKERRQ(err);

	/* Close file */
	cg_close(file);

	free(x);
	free(y);
	free(z);
	printf("\nSuccessfully wrote grid to file '%s'\n\n", fileName);
	exit(EXIT_SUCCESS);
}
