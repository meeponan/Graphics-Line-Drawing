#if !defined(SSD_UTIL_H_H)
#define SSD_UTIL_H_H
#include <stdlib.h>
#include <string.h>

#define SCREEN_KEY   0
#define COLOR_KEY    1
#define LINE_KEY     2
#define VERTEX_KEY   3
#define POLYLINE_KEY 4
#define CIRCLE_KEY   5
#define ARC_KEY      6
#define SAVE_KEY     7
#define TRIANGLE_KEY 8
 
struct ssd_keyword {
  /* Keyword table entry to be used for reading SSD */
  int key_id;
  char name[32];
  int npara;
};

typedef struct {
  double xyzw[4];
} VERTEX;

typedef struct {
  float rgba[4];
} RGB_COLOR;

typedef struct {
  double xyzw[4];
  float  rgba[4];
} COLOR_VERTEX;

typedef struct {
  double width;
  COLOR_VERTEX vertices[2];
} LINE;

typedef struct {
  double width;
  int    nvertices;
  COLOR_VERTEX *vertices;
} POLYLINE;

typedef struct {
  COLOR_VERTEX vertices[3];
} TRIANGLE;

typedef struct {
  int screen_w, screen_h;
  RGB_COLOR bcolor; /* The background color for the window */
  int  nlines; /* Number of lines */
  LINE *lines;
  int  npolylines; /* Number of the polylines */
  POLYLINE *polylines;
  int  ntriangles; 
  TRIANGLE *triangles;
} SCENE;

typedef struct {
  VERTEX position;
} CAMERA;

#if defined(SSD_UTIL_SOURCE_CODE)
#define EXTERN_FLAG
#else
#define EXTERN_FLAG extern
extern struct ssd_keyword keyword_table[];
#endif

EXTERN_FLAG 
int match_Keyword(char *keyword, int *npara);

EXTERN_FLAG 
int readAndParse(FILE *inFilePtr, char *keyword, char *arg0,
		 char *arg1, char *arg2, char *arg3, char *arg4);
EXTERN_FLAG
int Read_SSD_Scene(char *fname, SCENE *ascene, char *saved_fname);
#undef  EXTERN_FLAG

#endif
