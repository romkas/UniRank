#include "Interface.h"
#define DEBUG 0
using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	list<VUZ> vList;
	const char* source = "C:\\Users\\ROM\\Desktop\\practise-2\\Source list.txt",
        * res = "C:\\Users\\ROM\\Desktop\\practise-2\\07-07-unite\\results_07-07-unite.txt",
        * res_baas = "C:\\Users\\ROM\\Desktop\\practise-2\\07-07-unite\\results_baas_07-07-unite.txt";
	char c;

	cout<<"Reading file\n";
	Readfile(source, vList);
	cout<<"Reading completed\n";
	
	cout<<"Preparing\n";
	Prepare(vList);
	cout<<"Prepared\n";
	
#if DEBUG == 1
    const char filename[NUMSPECS + 1][STRLENG] = {"C:\\Users\\ROM\\Desktop\\practise-2\\07-04\\testhist_common_07-04.txt",
        "C:\\Users\\ROM\\Desktop\\practise-2\\07-04\\testhist_econom_07-04.txt", "C:\\Users\\ROM\\Desktop\\practise-2\\07-04\\testhist_manag_07-04.txt",
        "C:\\Users\\ROM\\Desktop\\practise-2\\07-04\\testhist_controls_07-04.txt", "C:\\Users\\ROM\\Desktop\\practise-2\\07-04\\testhist_inform_07-04.txt"};
	cout<<"Recording histograms\n";
	for(int k = 0;k < NUMSPECS + 1;k ++)
		for(Iter it = vList.begin(); it != vList.end(); it ++) 
			if(it->h[k].numstudents > STUD_LIMIT)
				PrintHistogram(filename[k], *it, k);
	cout<<"Recorded\nEnd of debugging\n";
	cin.get();
	return 0;
#else
	int histnum, indexnum;
	int mode,& m = mode;
    float** matrix = 0,**& mtx = matrix;
	
	PrintMenu();
	while(true)
	{
		cout<<"<SELECT> ";
		cin>>c;
		cin.ignore(100, '\n');
		switch( int(c)-int('0') )
		{
		case(1):
			histnum = OnCountingJager(vList);
			indexnum = 0;
			SortJager(vList, histnum);
			PrintIndex(res, vList, histnum, indexnum);
			break;
		case(2):
			histnum = OnCountingCentroid(vList);
			indexnum = 1;
			SortCentr(vList, histnum);
			PrintIndex(res, vList, histnum, indexnum);
			break;
		case(3):
			histnum = OnCountingKerre(vList, m); // m = 2 или m = 3
			SortmaxKerre(vList, histnum);
			PrintIndex(res, vList, histnum, mode);
			break;
		case(4):
			histnum = OnCountingBaas(vList, mtx);
			RangingBaas(vList, histnum, mtx);
			indexnum = 4;
			SortBaas(vList, histnum);
			PrintMatrix(res_baas, mtx, vList.size(), histnum);
			PrintIndex(res, vList, histnum, indexnum);
			DeleteBaasMatrix(mtx, vList.size());
			break;
		case(5):
			histnum = OnCountingAvGrade(vList);
			indexnum = 5;
			SortAvGrade(vList, histnum);
			PrintIndex(res, vList, histnum, indexnum);
			break;
		case(6):
			histnum = OnCountingNumStud(vList);
			SortNumStud(vList, histnum);
			PrintNumStud(res, vList, histnum);
			break;
		case( int('q')-int('0') ):
			return 0;
		default:
			cout<<"Unknown option\n";
		}
	}
#endif
}
