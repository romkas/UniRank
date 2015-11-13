#include "Interface.h"
using namespace std;

int MyRound(float x)
{
	float fracpart = x - int(x);
	int whole;
	if(fracpart >= 0.5)
		whole = int(x + 1);
	else
		whole = int(x);
	return whole;
}
VUZ& AddGrade(VUZ& vuz, int grade, int histnumber)
{
	if(!CheckBounds(histnumber))
	{
		cout<<"Place_toAdd(): argument is out of bounds\n";
		cin.get();
		exit(1);
	}
	int index;
	if(grade < LOWER_BOUND)
		index = 0;
	else
		if(grade == SCALE)
			index = vuz.h[histnumber].leng - 1;
		else
			index = (grade - LOWER_BOUND) / DEFAULT_INTERVAL + 1;
	vuz.h[histnumber].hist[index] ++;
	return vuz;
}
bool CheckBounds(int histnumber, int indexnumber)
{
	bool flag = true;
	if(histnumber < 0 || histnumber > NUMSPECS)
	{
		flag = false;
	}
	if(indexnumber < 0 || indexnumber > NUMINDS - 1)
	{
		flag = false;
	}
	return flag;
}
int FindMax(VUZ& vuz, int histnumber)
{
	//histnumber is in the correct range
	int imax = 0;
	for(int i = 1;i < vuz.h[histnumber].leng;i ++)
	{
		if(vuz.h[histnumber].hist[i] >= vuz.h[histnumber].hist[imax])
			imax = i;
	}
	return imax;
}
int FindMax_norm(VUZ& vuz, int histnumber)
{
	int imax = 0;
	for(int i = 1;i < vuz.h[histnumber].leng;i ++)
	{
		if(vuz.h[histnumber].normhist[i] >= vuz.h[histnumber].normhist[imax])
			imax = i;
	}
	return imax;
}
bool CheckUnimodal(VUZ& vuz, int histnumber, int& defect)
{
	bool flag = true, peak = false;
	float cur, prev;
	if(!CheckBounds(histnumber))
	{
		cout<<"CheckUnimodal(): argument is out of bounds\n";
		cin.get();
		exit(1);
	}
	defect = -1;
	for(int i = 1;i < vuz.h[histnumber].leng;i ++)
	{
		cur = vuz.h[histnumber].hist[i];
		prev = vuz.h[histnumber].hist[i - 1];
		if(cur < prev)
			if(!peak)
				peak = true;
		if(peak)
			if(cur > prev)
			{
				flag = false;
				defect = i;
				break;
			}
	}
	return flag;
}
VUZ& Alpha_section(VUZ& vuz, int histnumber, int& l, int& r, float alpha)
{
	//assuming that histnumber is in the correct interval after checking in CountJager(),
	//alpha is also in the proper range
	bool left_isfound = false, right_isfound = false;
	for(int i = 0;i < vuz.h[histnumber].leng;i ++)
	{
		if(!left_isfound)
		{
			if(vuz.h[histnumber].normhist[i] >= alpha)
			{
				left_isfound = true;
				l = i;
			}
		}
		else
		{
			if(!right_isfound)
			{
				if(vuz.h[histnumber].normhist[i] < alpha)
				{	
					right_isfound = true;
					r = i - 1;
				}
			}
		}
		if(i == vuz.h[histnumber].leng - 1)
		{
			if(!right_isfound)
			{
				right_isfound = true;
				r = i;
			}
		}
		if(left_isfound && right_isfound)
			break;
	}
	return vuz;
}
float WeightFunction(float alpha)
{
	//any non-decremental function which has an integral equal 1
	//on the segment [0,1]
	return 2 * alpha;
}
bool HistIsEmpty(VUZ& vuz, int histnumber)
{
	if(!CheckBounds(histnumber))
	{
		cout<<"HistIsEmpty(): argument is out of bounds\n";
		cin.get();
		exit(1);
	}
	bool flag = true;
	for(int i = 0;i < vuz.h[histnumber].leng;i ++)
		if(vuz.h[histnumber].hist[i])
		{
			flag = false;
			break;
		}
	return flag;
}
int IntervalValue(VUZ& vuz, int index, int histnumber)
{
	int val = vuz.h[histnumber].interval[0];

	if(index < 0 || index > vuz.h[histnumber].leng - 1 || !CheckBounds(histnumber))
	{
		cout<<"IntervalValue(): argument is out of bounds "<<index<<'\n'
			<<vuz.name<<'\n';
		cin.get();
		exit(1);
	}
	for(int i = 1;i <= index;i ++)
		val += vuz.h[histnumber].interval[i];
	return val - (vuz.h[histnumber].interval[index] - 1) / 2;
	//return LOWER_BOUND-1 + index*vuz.h[histnumber].interval[index] - (vuz.h[histnumber].interval[index]-1)/2;
}
int IntervalValue(const VUZ& vuz, int index, int histnumber)
{
	int val = vuz.h[histnumber].interval[0];

	if(index < 0 || index > vuz.h[histnumber].leng - 1 || !CheckBounds(histnumber))
	{
		cout<<"IntervalValue(): argument is out of bounds "<<index<<'\n'
			<<vuz.name<<'\n';
		cin.get();
		exit(1);
	}
	for(int i = 1;i <= index;i ++)
		val += vuz.h[histnumber].interval[i];
	return val - (vuz.h[histnumber].interval[index] - 1) / 2;
	//return LOWER_BOUND-1 + index*vuz.h[histnumber].interval[index] - (vuz.h[histnumber].interval[index]-1)/2;
}
int IntervalValue(int index)
{
	int len = DEFAULT_LEN;
	if(index < 0 || index > len - 1)
	{
		cout<<"IntervalValue(): argument is out of bounds "<<index<<'\n';
		cin.get();
		exit(1);
	}
	if(index == 0)
		return LOWER_BOUND / 2;
	else
//		return LOWER_BOUND + index-1;
		if(index == len - 1)
			return SCALE - (DEFAULT_INTERVAL) / 2;
		else
			return LOWER_BOUND-1 + index*DEFAULT_INTERVAL - (DEFAULT_INTERVAL-1)/2;
}
//-----test-----
//int IntervalValue(VUZ& vuz, int index, int exc_code, int histnumber=0)
//{
//	/*if(index < 0 || index > vuz.h[histnumber].leng - 1 || !CheckBounds(histnumber))
//	{
//		cout<<"IntervalValue(): argument is out of bounds "<<index<<'\n'
//			<<vuz.name<<'\n';
//		cin.get();
//		exit(1);
//	}*/
//	//int val;
//	//try{
//	//	val = LOWER_BOUND-1 + index*vuz.h[histnumber].interval[index] - (vuz.h[histnumber].interval[index]-1)/2;
//	//}
//	//catch(...){
//	//	cout<<exc_code<<'\n';
//	//	cin.get();
//	//	exit(1488);
//	//}
//	return LOWER_BOUND-1 + index*vuz.h[histnumber].interval[index] - (vuz.h[histnumber].interval[index]-1)/2;
//}
//--------------
float StrToNum(char* strnumber)
{
	if(!IsNumber(strnumber))
	{
		cout<<"StrToNum(): the string cannot be converted into a number\n";
		cin.get();
		exit(1);
	}
	int pointpos,& p = pointpos;
	float number = 0.;
	const float base = 10;
	int len = strlen(strnumber);
	int d;
	if(NumberIsWhole(strnumber, p))
	{
		d = len - 1;
		for(int i = 0;i < len;i ++)
			number += (strnumber[i] - '0') * pow(base, d - i);  
	}
	else
	{
		d = pointpos - 1;
		for(int i = 0;i < pointpos;i ++)
			number += (strnumber[i] - '0') * pow(base, d - i);
		d = -1;
		for(int i = pointpos + 1;i < len;i ++)
			number += (strnumber[i] - '0') * pow(base, d --);
	}
	return number;
}
bool NumberIsWhole(char* strnumber, int& pointpos)
{
	pointpos = -1;
	bool flag;
	char* point = strchr(strnumber, '.');
	if(point)
	{
		pointpos = point - strnumber;
		flag = false;
	}
	else
		flag = true;
	return flag;
}
bool IsNumber(char* strnumber)
{
	char* symbols = {"0123456789."};
	bool flag = true;
	for(int i = 0;i < strlen(strnumber);i ++)
	{
		char* c = strchr(symbols, strnumber[i]);
		if(!c)
		{
			flag = false;
			break;
		}
	}
	return flag;
}
list<VUZ>& SortJager(list<VUZ>& vList, int histnumber)
{
	if(histnumber == 0)
		vList.sort(cmp_jager_gen);
	else if(histnumber == 1)
		vList.sort(cmp_jager_eco);
	else if(histnumber == 2)
		vList.sort(cmp_jager_man);
	else if(histnumber == 3)
		vList.sort(cmp_jager_con);
	else
		vList.sort(cmp_jager_inf);
	return vList;
}
list<VUZ>& SortCentr(list<VUZ>& vList, int histnumber)
{
	if(histnumber == 0)
		vList.sort(cmp_centr_gen);
	else if(histnumber == 1)
		vList.sort(cmp_centr_eco);
	else if(histnumber == 2)
		vList.sort(cmp_centr_man);
	else if(histnumber == 3)
		vList.sort(cmp_centr_con);
	else
		vList.sort(cmp_centr_inf);
	return vList;
}
list<VUZ>& SortmaxKerre(list<VUZ>& vList, int histnumber)
{
	if(histnumber == 0)
		vList.sort(cmp_kerre_gen);
	else if(histnumber == 1)
		vList.sort(cmp_kerre_eco);
	else if(histnumber == 2)
		vList.sort(cmp_kerre_man);
	else if(histnumber == 3)
		vList.sort(cmp_kerre_con);
	else
		vList.sort(cmp_kerre_inf);
	return vList;
}
list<VUZ>& SortBaas(list<VUZ>& vList, int histnumber)
{
	if(histnumber == 0)
		vList.sort(cmp_baas_gen);
	else if(histnumber == 1)
		vList.sort(cmp_baas_eco);
	else if(histnumber == 2)
		vList.sort(cmp_baas_man);
	else if(histnumber == 3)
		vList.sort(cmp_baas_con);
	else
		vList.sort(cmp_baas_inf);
	return vList;
}
list<VUZ>& SortAvGrade(list<VUZ>& vList, int histnumber)
{
	if(histnumber == 0)
		vList.sort(cmp_avgr_gen);
	else if(histnumber == 1)
		vList.sort(cmp_avgr_eco);
	else if(histnumber == 2)
		vList.sort(cmp_avgr_man);
	else if(histnumber == 3)
		vList.sort(cmp_avgr_con);
	else
		vList.sort(cmp_avgr_inf);
	return vList;
}
list<VUZ>& SortNumStud(list<VUZ>& vList, int histnumber)
{
	if(histnumber == 0)
		vList.sort(cmp_nstd_gen);
	else if(histnumber == 1)
		vList.sort(cmp_nstd_eco);
	else if(histnumber == 2)
		vList.sort(cmp_nstd_man);
	else if(histnumber == 3)
		vList.sort(cmp_nstd_con);
	else
		vList.sort(cmp_nstd_inf);
	return vList;
}

bool cmp_jager_gen(const VUZ& first, const VUZ& second) { return first.h[0].indexes[0] > second.h[0].indexes[0]; }
bool cmp_jager_eco(const VUZ& first, const VUZ& second) { return first.h[1].indexes[0] > second.h[1].indexes[0]; }
bool cmp_jager_man(const VUZ& first, const VUZ& second) { return first.h[2].indexes[0] > second.h[2].indexes[0]; }
bool cmp_jager_con(const VUZ& first, const VUZ& second) { return first.h[3].indexes[0] > second.h[3].indexes[0]; }
bool cmp_jager_inf(const VUZ& first, const VUZ& second) { return first.h[4].indexes[0] > second.h[4].indexes[0]; }

bool cmp_centr_gen(const VUZ& first, const VUZ& second) { return first.h[0].indexes[1] > second.h[0].indexes[1]; }
bool cmp_centr_eco(const VUZ& first, const VUZ& second) { return first.h[1].indexes[1] > second.h[1].indexes[1]; }
bool cmp_centr_man(const VUZ& first, const VUZ& second) { return first.h[2].indexes[1] > second.h[2].indexes[1]; }
bool cmp_centr_con(const VUZ& first, const VUZ& second) { return first.h[3].indexes[1] > second.h[3].indexes[1]; }
bool cmp_centr_inf(const VUZ& first, const VUZ& second) { return first.h[4].indexes[1] > second.h[4].indexes[1]; }

bool cmp_kerre_gen(const VUZ& first, const VUZ& second) { return first.h[0].indexes[3] < second.h[0].indexes[3]; }
bool cmp_kerre_eco(const VUZ& first, const VUZ& second) { return first.h[1].indexes[3] < second.h[1].indexes[3]; }
bool cmp_kerre_man(const VUZ& first, const VUZ& second) { return first.h[2].indexes[3] < second.h[2].indexes[3]; }
bool cmp_kerre_con(const VUZ& first, const VUZ& second) { return first.h[3].indexes[3] < second.h[3].indexes[3]; }
bool cmp_kerre_inf(const VUZ& first, const VUZ& second) { return first.h[4].indexes[3] < second.h[4].indexes[3]; }

bool cmp_baas_gen(const VUZ& first, const VUZ& second) { return first.h[0].indexes[4] > second.h[0].indexes[4]; }
bool cmp_baas_eco(const VUZ& first, const VUZ& second) { return first.h[1].indexes[4] > second.h[1].indexes[4]; }
bool cmp_baas_man(const VUZ& first, const VUZ& second) { return first.h[2].indexes[4] > second.h[2].indexes[4]; }
bool cmp_baas_con(const VUZ& first, const VUZ& second) { return first.h[3].indexes[4] > second.h[3].indexes[4]; }
bool cmp_baas_inf(const VUZ& first, const VUZ& second) { return first.h[4].indexes[4] > second.h[4].indexes[4]; }

bool cmp_avgr_gen(const VUZ& first, const VUZ& second) { return first.h[0].indexes[5] > second.h[0].indexes[5]; }
bool cmp_avgr_eco(const VUZ& first, const VUZ& second) { return first.h[1].indexes[5] > second.h[1].indexes[5]; }
bool cmp_avgr_man(const VUZ& first, const VUZ& second) { return first.h[2].indexes[5] > second.h[2].indexes[5]; }
bool cmp_avgr_con(const VUZ& first, const VUZ& second) { return first.h[3].indexes[5] > second.h[3].indexes[5]; }
bool cmp_avgr_inf(const VUZ& first, const VUZ& second) { return first.h[4].indexes[5] > second.h[4].indexes[5]; }

bool cmp_nstd_gen(const VUZ& first, const VUZ& second) { return first.h[0].numstudents > second.h[0].numstudents; }
bool cmp_nstd_eco(const VUZ& first, const VUZ& second) { return first.h[1].numstudents > second.h[1].numstudents; }
bool cmp_nstd_man(const VUZ& first, const VUZ& second) { return first.h[2].numstudents > second.h[2].numstudents; }
bool cmp_nstd_con(const VUZ& first, const VUZ& second) { return first.h[3].numstudents > second.h[3].numstudents; }
bool cmp_nstd_inf(const VUZ& first, const VUZ& second) { return first.h[4].numstudents > second.h[4].numstudents; }

//VUZ& Copy(VUZ& dest, VUZ& src)
//{
//	//"dest" and "src" must be initialized	
//	int len;
//	
//	strcpy(dest.name, src.name);
//	for(int i = 0;i < NUMSPECS + 1;i ++)
//	{
//		dest.h[i].leng = len = src.h[i].leng;//modify the length
//		dest.h[i].numstudents = src.h[i].numstudents;//the number of students
//
//		//allocate a block of memory of a new size 
//		delete [] dest.h[i].hist;
//		delete [] dest.h[i].normhist;
//		delete [] dest.h[i].interval;
//		try
//		{
//			dest.h[i].hist = new int[len];
//			dest.h[i].normhist = new float[len];
//			dest.h[i].interval = new int[len];
//		}
//		catch(bad_alloc& ex)
//		{
//			cout<<"Copy(): "<<ex.what()<<'\n';
//			cin.get();
//			exit(1);
//		}
//
//		//copy data of histograms and indices
//		for(int j = 0;j < len;j ++)
//		{
//			dest.h[i].hist[j] = src.h[i].hist[j];
//			dest.h[i].normhist[j] = src.h[i].normhist[j];
//			dest.h[i].interval[j] = src.h[i].interval[j];
//		}
//		for(int j = 0;j < NUMINDS;j ++)
//			dest.h[i].indexes[j] = src.h[i].indexes[j];
//	}
//	return dest;
//}
void Prepare(list<VUZ>& vList)
{
    //int testval1 = 176, testval2 = 177, testspec = 4;
    for(Iter it = vList.begin();it != vList.end();it ++) {
        for(int j = 0;j < NUMSPECS + 1;j ++) {
		    //test
            //cout<<"i = "<<setw(2)<<i<<", j = "<<setw(2)<<j<<'\n';
			Setavgr(*it, j, CountAvGrade(*it, j));
			Normalize(MakeUnimodal(*it, j), j);
		}
	}
	return;
}
float*& SetZeros(float*& ptr, int len)
{
	if(ptr != 0)
		for(int i = 0;i < len;i ++)
			ptr[i] = 0.;
	return ptr;
}			
float*& AllocateMem(float*& ptr, int len)
{
	try{
		ptr = new float[len];
	}
	catch(bad_alloc& ex){
		cout<<"AllocateMem(): "<<ex.what()<<'\n';
		cin.get();
		exit(1);
	}
	return ptr;
}
float*& Split(VUZ& vuz, int histnumber, float*& split)
{
	//argument 'split' must be a histogram of the maximal possible length,
	//it depends on global parameters 'SCALE' and 'LOWER_BOUND' in the following way:
	//length('split') = SCALE - LOWERBOUND + 2
	if(!CheckBounds(histnumber))
	{
		cout<<"Split(): argument is out of bounds\n";
		cin.get();
		exit(1);
	}
	int len = SCALE - LOWER_BOUND + 2;
	try{
		split = new float[len];
	}
	catch(bad_alloc& ex){
		cout<<"Split(): "<<ex.what()<<'\n';
		cin.get();
		exit(1);
	}
	split[0] = vuz.h[histnumber].normhist[0];
	int i = 1, j = 1, k = 0;
	while(i < len)
	{
		split[i ++] = vuz.h[histnumber].normhist[j];
		k ++;
		if(k == vuz.h[histnumber].interval[j])
		{
			j ++;
			k = 0;
		}
	}
	return split;
}
float*& Split(float* const source, const int interval, float*& split)
{
	int len = SCALE - LOWER_BOUND + 2;
	try{
		split = new float[len];
	}
	catch(bad_alloc& ex){
		cout<<"Split(): "<<ex.what()<<'\n';
		cin.get();
		exit(1);
	}
	split[0] = source[0];
	int i = 1, j = 1, k = 0;
	while(i < len)
	{
		split[i ++] = source[j];
		k ++;
		if(k == interval && i < len-1)
		{
			j ++;
			k = 0;
		}
	}
	return split;
}
float**& CreateBaasMatrix(float**& pt, int V_sz)
{
    try{
        pt = new float*[V_sz];
    }
    catch(bad_alloc& ex){
        cout<<"CreateBaasMatrix(): "<<ex.what()<<'\n';
        cin.get();
        exit(1);
    }
    for(int i = 0;i < V_sz;i ++)
    {
        try{
            pt[i] = new float[V_sz];
        }
        catch(bad_alloc& ex){
            cout<<"CreateBaasMatrix(): "<<ex.what()<<'\n';
            cin.get();
            exit(1);
        }
        for(int j = 0;j < V_sz;j ++)
        {
            pt[i][j] = -1;//default value is set
        }
    }
    return pt;
}
void DeleteBaasMatrix(float**& mtx, int mtx_sz)
{
	for(int i = 0;i < mtx_sz;i ++)
		delete [] mtx[i];
	delete [] mtx;
	return;
}