#include <stdlib.h>
#include <stdio.h>
#include <cgnslib.h>

#define CHKERRQ(ret) if((ret)) cg_error_exit()

#define NX 50
#define NY 25
#define NZ 10

#define PHYSICAL_DIMENSION 3

int main()
{
	int err;
	int i, j, k;

	int cellDimension = 3;
	int physicalDimension = PHYSICAL_DIMENSION;

	double x[NX][NY][NZ];
	double y[NX][NY][NZ];
	double z[NX][NY][NZ];

	int file;
	char fileName[] = "/home/guesser/cgns_examples/output/my_structured_simple.cgns";

	int base;
	char baseName[] = "Simple structured grid";

	int zone;
	char zoneName[] = "Grid";
	ZoneType_t structured = CGNS_ENUMV(Structured);
	cgsize_t size[3][PHYSICAL_DIMENSION];

	int coorX, coorY, coorZ;
	char coorXName[] = "CoordinateX";
	char coorYName[] = "CoordinateY";
	char coorZName[] = "CoordinateZ";

	/* Open file */
	err = cg_open(fileName, CG_MODE_WRITE, &file); CHKERRQ(err);

	/* Open base */
	err = cg_base_write(file, baseName, cellDimension, physicalDimension, &base); CHKERRQ(err);

	/* Open zone */
	/* Define zone size */
		/* vertices */
		#define VERTEX 0
		size[VERTEX][0] = NZ;
		size[VERTEX][1] = NY;
		size[VERTEX][2] = NX;
		/* Cells */
		#define CELL 1
		size[CELL][0] = size[VERTEX][0] - 1;
		size[CELL][1] = size[VERTEX][1] - 1;
		size[CELL][2] = size[VERTEX][2] - 1;
		/* Boundary vertex */
		#define BVERTEX 2
		size[BVERTEX][0] = 0;
		size[BVERTEX][1] = 0;
		size[BVERTEX][2] = 0;
	err = cg_zone_write(file, base, zoneName, *size, structured, &zone); CHKERRQ(err);

	/* Write coordinates */
	for(i=0 ; i<NX ; ++i)
	{
		for(j=0 ; j<NY ; ++j)
		{
			for(k=0 ; k<NZ ; ++k)
			{
				x[i][j][k] = i;
				y[i][j][k] = j;
				z[i][j][k] = k;
			}
		}
	}
	err = cg_coord_write(file, base, zone, RealDouble, coorXName, x, &coorX); CHKERRQ(err);
	err = cg_coord_write(file, base, zone, RealDouble, coorYName, y, &coorY); CHKERRQ(err);
	err = cg_coord_write(file, base, zone, RealDouble, coorZName, z, &coorZ); CHKERRQ(err);

	/* Close file */
	cg_close(file);

   printf("\nSuccessfully wrote grid to file '%s'\n\n", fileName);
	exit(EXIT_SUCCESS);
}
