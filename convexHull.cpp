//author Volkov Andrey IVT12BO
//all tasks are realised
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <cstdlib>
#include <gdiplus.h>
#include <stdio.h>
#include <math.h> 
typedef struct point
{
	int x, y, z;
} Point;
typedef struct vector
{
	int x, y, z;
} Vector;
typedef struct plane
{
	int a, b, c, d;
} Plane;
int** createMatr(int n, int m)
{
	int **matr;
	matr = (int**)malloc(n * sizeof(int*));
	for (int i = 0; i < n; i++)
	{
		matr[i] = (int*)malloc(m * sizeof(int));
		for (int j = 0; j < m; matr[i][j] = 0, j++);
	}
	return matr;

}
void printMatr(int **a, int n, int m)
{
	for (int i = 0; i < n; printf("\n"), i++)
		for (int j = 0; j < m; j++)
			printf("%2d ", a[i][j]);
}
void clearMatr(int **a, int v)
{
	for (int i = 0; i < v; i++)
		free(a[i]);
	free(a);
}
char sign(int x)
{
	return (x > 0) ? '+' : '-';
}
int abs(int x)
{
	return (x > 0) ? x : -x;
}
int det3x3(int a, int b, int c, int d, int e, int f, int g, int h, int i)
{
	return a * e*i + b * f*g + c * d*h - b * d*i - a * f*h - c * e*g;
}
void printPlane(Plane plane)
{
	int f = 0;
	if (plane.a)
	{
		if (plane.a == 1)
			if (f)
				printf("+x");
			else
				printf("x");
		else
			if (plane.a == -1)
				printf("-x");
			else
				if (plane.a > 0)
					if (f)
						printf("+%dx", abs(plane.a));
					else
						printf("%dx", abs(plane.a));
				else
					printf("-%dx", abs(plane.a));
		f = 1;
	}
	if (plane.b)
	{
		if (plane.b == 1)
			if (f)
				printf("+y");
			else
				printf("y");
		else
			if (plane.b == -1)
				printf("-y");
			else
				if (plane.b > 0)
					if (f)
						printf("+%dy", abs(plane.b));
					else
						printf("%dy", abs(plane.b));
				else
					printf("-%dy", abs(plane.b));
		f = 1;
	}
	if (plane.c)
	{
		if (plane.c == 1)
			if (f)
				printf("+z");
			else
				printf("z");
		else
			if (plane.c == -1)
				printf("-z");
			else
				if (plane.c > 0)
					if (f)
						printf("+%dz", abs(plane.c));
					else
						printf("%dz", abs(plane.c));
				else
					printf("-%dz", abs(plane.c));
		f = 1;
	}
	if (plane.d)
	{
		if (plane.d > 0)
			if (f)
				printf("+%d", abs(plane.d));
			else
				printf("%d", abs(plane.d));
		else
			printf("%d", plane.d);
	}
	printf("<=0\n");
}
void printVertices(Point* vertices, int n)
{
	for (int i = 0; i < n; i++)
		printf("%d %d %d\n", vertices[i].x, vertices[i].y, vertices[i].z);
}
Vector pointsToVector(Point p1, Point p2)
{
	Vector a;
	a.x = p2.x - p1.x;
	a.y = p2.y - p1.y;
	a.z = p2.z - p1.z;
	return a;
}
Vector crossProduct(Vector a, Vector b)
{
	Vector c;
	c.x = a.y*b.z - a.z*b.y;
	c.y = a.z*b.x - a.x*b.z;
	c.z = a.x*b.y - a.y*b.x;
	return c;
}
Plane planeEquation(Vector normalVector, Point point)
{
	Plane plane;
	plane.a = normalVector.x;
	plane.b = normalVector.y;
	plane.c = normalVector.z;
	plane.d = -(point.x * normalVector.x + point.y * normalVector.y + point.z * normalVector.z);
	return plane;
}
int isPointsOnLine(Point p1, Point p2, Point p3)
{
	Vector a, b, c;
	a = pointsToVector(p1, p2);
	b = pointsToVector(p2, p3);
	c = crossProduct(a, b);
	if (c.x || c.y || c.z)
		return 0;
	else
		return 1;
}
int isLinearIndependent(Plane p, Plane* planes, int nPlanes)
{
	for (int i = 0; i < nPlanes; i++)
	{
		Plane p1 = planes[i];
		if (p.a*p1.b == p1.a*p.b &&
			p.a*p1.c == p1.a*p.c &&
			p.a*p1.d == p1.a*p.d &&
			p.b*p1.c == p1.b*p.c &&
			p.b*p1.d == p1.b*p.d &&
			p.c*p1.d == p1.c*p.d)
			return 0;
	}
	return 1;
}
int isSupportingPlane(Plane plane, Point* vertices, int n)
{
	int k = 0;
	for (int i = 0; i < n; i++)
	{
		Point vertex = vertices[i];
		k += ((plane.a*vertex.x +
			plane.b*vertex.y +
			plane.c*vertex.z +
			plane.d) <= 0);
	}
	if (k == n)
		return 1; // if <=0
	else
		if (k == 0)
			return 2; // if >=0
		else
			return 0; //not sup plane
}
void appendToPlanes(Plane plane, Plane** planes, int* nPlanes)
{
	(*nPlanes)++;
	*planes = (Plane*)realloc(*planes, (*nPlanes) * sizeof(Plane));
	(*planes)[*nPlanes - 1] = plane;
	return;
}
Point threePlanesIntersection(Plane p1, Plane p2, Plane p3)
{
	Point p;
	int dx, dx1, dx2, dx3;
	dx = det3x3(
		p1.a, p1.b, p1.c,
		p2.a, p2.b, p2.c,
		p3.a, p3.b, p3.c);
	dx1 = det3x3(
		-p1.d, p1.b, p1.c,
		-p2.d, p2.b, p2.c,
		-p3.d, p3.b, p3.c);
	dx2 = det3x3(
		p1.a, -p1.d, p1.c,
		p2.a, -p2.d, p2.c,
		p3.a, -p3.d, p3.c);
	dx3 = det3x3(
		p1.a, p1.b, -p1.d,
		p2.a, p2.b, -p2.d,
		p3.a, p3.b, -p3.d);

	p.x = dx1 / dx;
	p.y = dx2 / dx;
	p.z = dx3 / dx;
	return p;
}
int isPointInVertices(Point p, Point* vertices, int nVertices)
{
	for (int i = 0; i < nVertices; i++)
		if (p.x == vertices[i].x && p.y == vertices[i].y && p.z == vertices[i].z)
			return 1;
	return 0;
}
void appendToVertices(Point p, Point **vertices, int *nVertices)
{
	(*nVertices)++;
	*vertices = (Point*)realloc(*vertices, (*nVertices) * sizeof(Point));
	(*vertices)[*nVertices - 1] = p;
	return;
}
int isPointSuitsInSystem(Point p, Plane *planes, int nPlanes)
{
	for (int i = 0; i < nPlanes; i++)
		if (planes[i].a*p.x + planes[i].b*p.y + planes[i].c*p.z + planes[i].d > 0)
			return 0;
	return 1;
}
Plane* convexHull(Point *vertices, int nVertices, int *nPlanes)
{
	Plane* planes = NULL;

	for (int i = 0; i < nVertices; i++)
		for (int j = 0; j < nVertices; j++)
			for (int k = 0; k < nVertices; k++)
				if ((i != j) && (i != k) && (j != k))
					if (!isPointsOnLine(vertices[i], vertices[j], vertices[k]))
					{
						Plane plane;
						plane = planeEquation(
							crossProduct(
								pointsToVector(vertices[i], vertices[j]),
								pointsToVector(vertices[j], vertices[k])),
							vertices[i]);
						if (isLinearIndependent(plane, planes, *nPlanes))
						{
							int supState = isSupportingPlane(plane, vertices, nVertices);
							if (supState)
							{
								if (supState == 2)
								{
									plane.a = -plane.a;
									plane.b = -plane.b;
									plane.c = -plane.c;
									plane.d = -plane.d;
								}
								appendToPlanes(plane, &planes, nPlanes);
							}
						}
					}
	return planes;
}
Point* vertexEnumeration(Plane *planes, int nPlanes, int *nVertices)
{
	Point* vertices = NULL;

	for (int i = 0; i < nPlanes; i++)
		for (int j = 0; j < nPlanes; j++)
			for (int k = 0; k < nPlanes; k++)
				if ((i != j) && (i != k) && (j != k))
				{
					if (det3x3(
						planes[i].a, planes[i].b, planes[i].c,
						planes[j].a, planes[j].b, planes[j].c,
						planes[k].a, planes[k].b, planes[k].c)
						)
					{
						Point p;
						p = threePlanesIntersection(planes[i], planes[j], planes[k]);
						if (!isPointInVertices(p, vertices, *nVertices))
							if (isPointSuitsInSystem(p, planes, nPlanes))
								appendToVertices(p, &vertices, nVertices);
					}
				}

	return vertices;
}
int **oneSkeleton(Point* vertices, Plane* planes, int nVertices, int nPlanes)
{
	//optimised cycle
	int **adjacancy = createMatr(nVertices, nVertices);
	for (int plane1 = 0; plane1 < nPlanes - 1; plane1++)
		for (int plane2 = plane1 + 1; plane2 < nPlanes; plane2++)
			for (int p1 = 0; p1 < nVertices - 1; p1++)
			{
				if (!((planes[plane1].a*vertices[p1].x + planes[plane1].b*vertices[p1].y + //1st vertex 1st plane
					planes[plane1].c*vertices[p1].z + planes[plane1].d == 0) &&
					(planes[plane2].a*vertices[p1].x + planes[plane2].b*vertices[p1].y + //1st vertex 2nd plane
						planes[plane2].c*vertices[p1].z + planes[plane2].d == 0)))
					continue; //if 1st point not on line of intersection plane1 & plane2
				for (int p2 = p1 + 1; p2 < nVertices; p2++)
					if ((planes[plane1].a*vertices[p2].x + planes[plane1].b*vertices[p2].y + //2nd vertex 1st plane 
						planes[plane1].c*vertices[p2].z + planes[plane1].d == 0) &&
						(planes[plane2].a*vertices[p2].x + planes[plane2].b*vertices[p2].y + //2nd vertex 2nd plane 
							planes[plane2].c*vertices[p2].z + planes[plane2].d == 0))
					{
						adjacancy[p1][p2] = adjacancy[p2][p1] = 1; //p1 and p2 on line of intersection plane1 & plane 2
					}
			}

	//2nd output format
	int *vertexSuitInPlane;
	vertexSuitInPlane = (int*)malloc(nVertices * sizeof(int));
	for (int plane = 0; plane < nPlanes; plane++)
	{
		for (int v = 0; v < nVertices; vertexSuitInPlane[v] = 0, v++);

		printf("Face: ");
		printPlane(planes[plane]);

		printf("Vertices: ");
		for (int vertex = 0; vertex < nVertices; vertex++)
			if (planes[plane].a*vertices[vertex].x + planes[plane].b*vertices[vertex].y +
				planes[plane].c*vertices[vertex].z + planes[plane].d == 0)
			{
				vertexSuitInPlane[vertex] = 1;
				printf("%c ", vertex + 65);
			}
		printf("\n");

		printf("Edges: ");
		for (int vertex = 0; vertex < nVertices - 1; vertex++)
			if (vertexSuitInPlane[vertex])
				for (int p1 = vertex + 1; p1 < nVertices; p1++)
					if (vertexSuitInPlane[p1] && adjacancy[vertex][p1])
						printf("%c%c ", vertex + 65, p1 + 65);
		printf("\n");

	}
	free(vertexSuitInPlane);
	return adjacancy;
}
int collisionDetection(Point* vertices1, int nVertices1, Point* vertices2, int nVertices2) //need to test
{
	Point *newVertices;
	Plane* planes;
	int nPlanes = 0, k = 0;
	newVertices = (Point*)malloc(nVertices1*nVertices2 * sizeof(Point));
	for (int v1 = 0; v1 < nVertices1; v1++)
		for (int v2 = 0; v2 < nVertices2; v2++)
		{
			Point p;
			p.x = vertices1[v1].x - vertices2[v2].x;
			p.y = vertices1[v1].y - vertices2[v2].y;
			p.z = vertices1[v1].z - vertices2[v2].z;
			newVertices[k++] = p;
		}
	planes = convexHull(newVertices, nVertices1*nVertices2, &nPlanes);
	for (int plane = 0; plane < nPlanes; plane++)
		if (planes[plane].d > 0)
			return 0;
	return 1;
}
int nVertices1 = 0, nVertices2 = 0, nPlanes1 = 0, nPlanes2 = 0, *nVertices = NULL, *nPlanes = NULL;
int **adjacency1 = NULL, **adjacency2 = NULL, ***adjacency = NULL;
Point **vertices = NULL, *vertices1 = NULL, *vertices2 = NULL;
Plane **planes = NULL, *planes1 = NULL, *planes2 = NULL;
int main(int argc, char* argv[])
{
	int nFiles = 0;
	char format;
	FILE *input = stdin;
	//if input by hands
	if (argc == 1)
	{
		printf("Number of polyhedrals to input: ");
		scanf("%d", &nFiles);
		input = stdin;
	}
	else
		nFiles = argc - 1;
	//after cycle we have 1st and 2nd vertices and planes (A, B.1, B.2 here)
	for (int polyhedral = 1; polyhedral < nFiles + 1; polyhedral++)
	{
		if (polyhedral == 1)
		{
			vertices = &vertices1;
			planes = &planes1;
			nVertices = &nVertices1;
			nPlanes = &nPlanes1;
			adjacency1 = createMatr(nVertices1, nVertices1);
			adjacency = &adjacency1;
		}
		if (polyhedral == 2)
		{
			vertices = &vertices2;
			planes = &planes2;
			nVertices = &nVertices2;
			nPlanes = &nPlanes2;
			adjacency2 = createMatr(nVertices2, nVertices2);
			adjacency = &adjacency2;
		}
		if (argc != 1)
		{
			input = fopen(argv[polyhedral], "r");
			printf("Working with '%s'\n", argv[polyhedral]);
		}
		else
			printf("Enter %d polyhedral:\n", polyhedral);
		fscanf(input, " %c", &format);
		if (argc != 1)
			printf("Mode: %c\n", format);
		//A, B.1
		if (format == 'V')
		{
			//A
		//read input
			int n;
			fscanf(input, "%d", &n);
			*vertices = (Point*)malloc(n * sizeof(Point));
			for (int i = 0; i < n; i++)
				fscanf(input, "%d %d %d", &((*vertices)[i].x), &((*vertices)[i].y), &((*vertices)[i].z));
			if (argc != 1)
				fclose(input);
			//print input
			printf("Input:\nNumber of vertices: %d\n", n);
			printVertices(*vertices, n);
			//convexHull
			*planes = convexHull(*vertices, n, nPlanes);
			//print result
			printf("----------\nconvexHull\n----------\n");
			printf("Number of faces: %d\n", *nPlanes);
			for (int i = 0; i < *nPlanes; i++)
				printPlane((*planes)[i]);

			//B.1 with A planes
			*vertices = vertexEnumeration(*planes, *nPlanes, nVertices);
			//print result
			printf("-----------------\nvertexEnumeration\n-----------------\n");
			printf("(with faces from 'convexHull')\n");
			printf("Number of vertices: %d\n", *nVertices);
			for (int i = 0; i < *nVertices; i++)
				if (i / 26 == 0)
					printf("%c: %d %d %d\n", i + 65, (*vertices)[i].x, (*vertices)[i].y, (*vertices)[i].z);
				else
					printf("%c%d: %d %d %d\n", i + 65, i / 26, (*vertices)[i].x, (*vertices)[i].y, (*vertices)[i].z);
		}
		if (format == 'H')
		{
			//B.1
		//read input
			fscanf(input, "%d", nPlanes);
			*planes = (Plane*)malloc(*nPlanes * sizeof(Plane));
			for (int i = 0; i < *nPlanes; i++)
			{
				fscanf(input, "%d %d %d %d", &((*planes)[i].a), &((*planes)[i].b), &((*planes)[i].c), &((*planes)[i].d));
				(*planes)[i].d = -((*planes)[i].d);
			}
			if (argc != 1)
				fclose(input);
			//print input
			printf("Input:\nNumber of faces: %d\n", *nPlanes);
			for (int i = 0; i < *nPlanes; i++)
				printPlane((*planes)[i]);
			//vertexEnumeration
			*vertices = vertexEnumeration(*planes, *nPlanes, nVertices);
			//print result
			printf("-----------------\nvertexEnumeration\n-----------------\n");
			printf("Number of vertices: %d\n", *nVertices);
			for (int i = 0; i < *nVertices; i++)
				if (i / 26 == 0)
					printf("%c: %d %d %d\n", i + 65, (*vertices)[i].x, (*vertices)[i].y, (*vertices)[i].z);
				else
					printf("%c%d: %d %d %d\n", i + 65, i / 26, (*vertices)[i].x, (*vertices)[i].y, (*vertices)[i].z);
		}

		//B.2
		printf("----------\n1-skeleton\n----------\n");
		*adjacency = oneSkeleton(*vertices, *planes, *nVertices, *nPlanes);
		//1st output format
		printf("\nAdjacency matrix:\n  ");
		for (int i = 0; i < *nVertices; printf("%c ", i + 65), i++);
		printf("\n");
		for (int i = 0; i < *nVertices; i++)
		{
			printf("%c ", i + 65);
			for (int j = 0; j < *nVertices; j++)
				printf("%d ", (*adjacency)[i][j]);
			printf("\n");
		}

	}
	//B.3
	if (nFiles == 2)
	{
		if (argc != 1)
			printf("------------------\ncolissionDetection of '%s' and '%s': ", argv[1], argv[2]);
		else
			printf("------------------\ncolissionDetection: ");
		if (collisionDetection(vertices1, nVertices1, vertices2, nVertices2))
			printf("YES\n------------------\n");
		else
			printf("NO\n------------------\n");

	}

		
	//DRAW
	//ujasniy kod no ono rabotaet
	LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam);
	HINSTANCE currentInstance = GetModuleHandle(argv[0]);
	// Initialize GDI+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

	// Register the window class
	const char *CLASS_NAME = "myWin32WindowClass";
	WNDCLASS wc{};
	wc.hInstance = currentInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProcessMessages;
	RegisterClass(&wc);

	// Create the window
	CreateWindow(CLASS_NAME, "3D",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZE,            // Window style
		CW_USEDEFAULT, CW_USEDEFAULT,                // Window initial position
		GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),                                    // Window size
		nullptr, nullptr, nullptr, nullptr);

	// Window loop
	MSG msg{};
	while (GetMessage(&msg, nullptr, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Gdiplus::GdiplusShutdown(gdiplusToken);
	//return 0;


	/*if (nFiles == 1)
		Main(instance, vertices1, nVertices1, adjacency1, NULL, 0, NULL);
	else
		Main(instance, vertices1, nVertices1, adjacency1, vertices2, nVertices2, adjacency2);*/

	free(vertices1);
	free(vertices2);
	free(planes1);
	free(planes2);
	clearMatr(adjacency1, nVertices1);
	clearMatr(adjacency2, nVertices2);
	//system("pause");
}
VOID drawText(HDC, WCHAR[], int, Gdiplus::PointF, int);


void toString(int b, WCHAR* s, int *len)
{
	if (b == 0)
	{
		*len = 1;
		s[0] = '0';
		return;
	}
	int i = 0;
	int a = b;
	a = abs(a);
	while (a != 0)
	{
		s[i++] = '0' + a % 10;
		a /= 10;
	}
	if (b < 0)
	{
		s[i++] = '-';
	}
	*len = i;
	for (int j = 0; j < *len / 2; j++)
	{
		WCHAR tmp;
		tmp = s[j];
		s[j] = s[*len-1 - j];
		s[*len-1 - j] = tmp;
	}

}
void printPoint(HDC hdc, Gdiplus::PointF start,  int name, Point p, int boldness)
{
	WCHAR string[50];
	WCHAR tmp[10];
	int i = 0;
	if (name <= 25)
		string[i++] = 'A' + name;
	else
	{
		string[i++] = 'A' + name % 25;
		string[i++] = '0' + name / 25;
	}
	string[i++] = '(';

	int iTmp = 0;
	toString(p.x, tmp, &iTmp);
	for (int k = 0; k < iTmp; k++)
		string[i++] = tmp[k];
	string[i++] = ',';
	iTmp = 0;
	toString(p.y, tmp, &iTmp);
	for (int k = 0; k < iTmp; k++)
		string[i++] = tmp[k];
	string[i++] = ',';
	iTmp = 0;
	toString(p.z, tmp, &iTmp);
	for (int k = 0; k < iTmp; k++)
		string[i++] = tmp[k];
	string[i++] = ')';
	//printf("%ls", string);
	drawText(hdc, string, i, start, boldness);
}
void draw(HDC hdc);
LRESULT CALLBACK WindowProcessMessages(HWND hwnd, UINT msg, WPARAM param, LPARAM lparam) {
	HDC hdc;
	PAINTSTRUCT ps;

	switch (msg) {
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		draw(hdc);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hwnd, msg, param, lparam);
	}
}
VOID drawText(HDC hdc, WCHAR string[], int size, Gdiplus::PointF start, int boldness)
{
	Gdiplus::Graphics gf(hdc);

	// Create a string.
	//WCHAR string[] = L"Sample Text";

	// Initialize arguments.
	Gdiplus::Font myFont(L"Arial", 13, boldness);
	Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0));

	// Draw string.
	gf.DrawString(
		string,
		size,
		&myFont,
		start,
		&blackBrush);
}
void draw(HDC hdc) {

	double mnoj = float(1) / sqrt(5);
	Gdiplus::Graphics gf(hdc);
	Gdiplus::Pen planePositive(Gdiplus::Color(255, 0, 0, 255), 3);
	Gdiplus::Pen planeNegative(Gdiplus::Color(100, 0, 0, 255), 3);
	Gdiplus::Pen polyhedralEdges1(Gdiplus::Color(200, 255, 0, 0), 3);
	Gdiplus::Pen polyhedralEdges2(Gdiplus::Color(200, 0, 255, 0), 3);
	int width = GetSystemMetrics(SM_CXSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	Point origin;
	
	origin.x = width / 2;
	origin.y = height / 2;
	//coordinate plane
	int lenStr = 0;
	WCHAR axis[10] = L"x";
	Gdiplus::PointF tmpPoint;
	//x
	axis[0] = 'x';
	gf.DrawLine(&planeNegative, width / 10, height / 2, origin.x, origin.y);
	gf.DrawLine(&planePositive, origin.x, origin.y, width - width / 10, height / 2);
	gf.DrawLine(&planePositive, (width - width / 10) - 13, (height / 2) - 7, width - width / 10, height / 2);
	gf.DrawLine(&planePositive, (width - width / 10) - 13, (height / 2) + 7, width - width / 10, height / 2);
	tmpPoint.X = Gdiplus::REAL(width - width / 10);
	tmpPoint.Y = Gdiplus::REAL(height / 2);
	drawText(hdc, axis, 1, tmpPoint, 0);
	//y
	axis[0] = 'y';
	gf.DrawLine(&planePositive, origin.x - (origin.y - height / 10), height - height / 10, origin.x, origin.y);
	gf.DrawLine(&planeNegative, origin.x + origin.y - height / 10, height / 10, origin.x, origin.y);
	gf.DrawLine(&planePositive, origin.x - (origin.y - height / 10), height - height / 10, (origin.x - (origin.y - height / 10)) + 3, (height - height / 10) - 15);
	gf.DrawLine(&planePositive, origin.x - (origin.y - height / 10), height - height / 10, (origin.x - (origin.y - height / 10)) + 15, (height - height / 10) - 3);
	tmpPoint.X = Gdiplus::REAL(origin.x - (origin.y - height / 10));
	tmpPoint.Y = Gdiplus::REAL(height - height / 10);
	drawText(hdc, axis, 1, tmpPoint, 0);
	//z
	axis[0] = 'z';
	gf.DrawLine(&planeNegative, width / 2, height - height / 10, origin.x, origin.y);
	gf.DrawLine(&planePositive, origin.x, origin.y, width / 2, height / 10);
	gf.DrawLine(&planePositive, width / 2 - 7, height / 10 + 13, width / 2, height / 10);
	gf.DrawLine(&planePositive, width / 2 + 7, height / 10 + 13, width / 2, height / 10);
	tmpPoint.X = Gdiplus::REAL(width / 2);
	tmpPoint.Y = Gdiplus::REAL(height / 10 - 25);
	drawText(hdc, axis, 1, tmpPoint, 0);
	//print edges 1
	for (int i = 0; i < nVertices1 - 1; i++)
		for (int j = i + 1; j < nVertices1; j++)
			if (adjacency1[i][j])
			{
				int xi = origin.x + (vertices1[i].x * 100 - int(float(vertices1[i].y * 100)*mnoj));
				int yi = origin.y - vertices1[i].z * 100 + int(float(vertices1[i].y * 100) *mnoj);
				int xj = origin.x + (vertices1[j].x * 100 - int(float(vertices1[j].y * 100) *mnoj));
				int yj = origin.y - vertices1[j].z * 100 + int(float(vertices1[j].y * 100)  *mnoj);
				gf.DrawLine(&polyhedralEdges1, xi, yi, xj, yj);
				
			}
	//print verteces 1
	for (int i = 0; i < nVertices1; i++)
	{
		int xi = origin.x + (vertices1[i].x * 100 - int(float(vertices1[i].y * 100)*mnoj));
		int yi = origin.y - vertices1[i].z * 100 + int(float(vertices1[i].y * 100) *mnoj);
		tmpPoint.X = Gdiplus::REAL(xi);
		tmpPoint.Y = Gdiplus::REAL(yi);
		printPoint(hdc, tmpPoint, i, vertices1[i], 0);
	}
	//print edges 2
	for (int i = 0; i < nVertices2 - 1; i++)
		for (int j = i + 1; j < nVertices2; j++)
			if (adjacency2[i][j])
				gf.DrawLine(&polyhedralEdges2, origin.x + (vertices2[i].x * 100 - int(float(vertices2[i].y * 100)*mnoj)),
					origin.y - vertices2[i].z * 100 + int(float(vertices2[i].y * 100) *mnoj),
					origin.x + (vertices2[j].x * 100 - int(float(vertices2[j].y * 100) *mnoj)),
					origin.y - vertices2[j].z * 100 + int(float(vertices2[j].y * 100)  *mnoj));
	//print vertices 2
	for (int i = 0; i < nVertices2; i++)
	{
		int xi = origin.x + (vertices2[i].x * 100 - int(float(vertices2[i].y * 100)*mnoj));
		int yi = origin.y - vertices2[i].z * 100 + int(float(vertices2[i].y * 100) *mnoj);
		tmpPoint.X = Gdiplus::REAL(xi);
		tmpPoint.Y = Gdiplus::REAL(yi);
		printPoint(hdc, tmpPoint, i, vertices2[i], 1);
	}


}
