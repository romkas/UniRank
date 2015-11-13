#include "Interface.h"
using namespace std;

VUZ& Init(VUZ& vuz)
{
	strcpy(vuz.name, "Unnamed");
	for(int i = 0;i < NUMSPECS + 1;i ++)
	{
		vuz.h[i].numstudents = 0;
		vuz.h[i].avergrade = 0;
		vuz.h[i].leng = DEFAULT_LEN;
		int len = vuz.h[i].leng;
		try {
			vuz.h[i].hist = new float[len];
			vuz.h[i].normhist = new float[len];
			vuz.h[i].interval = new int[len];
		}
		catch(bad_alloc& ex) {
			cout<<"Init(): "<<ex.what()<<'\n';
			cin.get();
			exit(1);
		}
		for(int j = 0;j < len;j ++)
		{
			if(j == 0)
				vuz.h[i].interval[j] = LOWER_BOUND - 1;
			else
				if(j == len - 1)
					vuz.h[i].interval[j] = DEFAULT_INTERVAL + 1;
				else
					vuz.h[i].interval[j] = DEFAULT_INTERVAL;
			vuz.h[i].normhist[j] = 0;
			vuz.h[i].hist[j] = 0;
		}
		for(int j = 0;j < NUMINDS;j ++)
			vuz.h[i].indexes[j] = -1;
	}
	return vuz;
}
VUZ& Delete(VUZ& vuz)
{
	strcpy(vuz.name, "");
	for(int i = 0;i < NUMSPECS + 1;i ++)
	{
		delete [] vuz.h[i].hist;
		delete [] vuz.h[i].normhist;
		delete [] vuz.h[i].interval;
		vuz.h[i].leng = -1;
		vuz.h[i].numstudents = 0;
		vuz.h[i].avergrade = 0;
		for(int j = 0;j < NUMINDS;j ++)
			vuz.h[i].indexes[j] = -1;
	}
	return vuz;
}
VUZ& AddStudent(VUZ& vuz, float grade, const char* spec)
{
	int gr = MyRound(grade);
	//int gt = round(grade);
	int i = -1;
	for(int j = 0;j < NUMSPECS;j ++)
	{
		if(strcmp(spec, SPECNAMES[j]) == 0)
		{
			i = j + 1;
			//i = j;
			break;
		}
	}
	if(i == -1)
	{
		cout<<"AddStudent(): unknown speciality "<<spec<<'\n';
		cout<<vuz.name<<'\n';
		cin.get();
		exit(1);
	}
	AddGrade(vuz, gr);
	vuz.h[0].numstudents ++;
	AddGrade(vuz, gr, i);
	vuz.h[i].numstudents ++;
	return vuz;
}
VUZ& Setname(VUZ& vuz, const char* newname)
{
	strcpy(vuz.name, newname);
	return vuz;
}
const char* Getname(const VUZ& vuz)
{
	return vuz.name;
}
VUZ& SetIndex(VUZ& vuz, int histnumber, int indexnumber, float val)
{
	if(!CheckBounds(histnumber, indexnumber))
	{
		cout<<"SetIndex(): argument is out of bounds\n";
		cin.get();
		exit(1);
	}
	//if(val < 0)
	//{
	//	cout<<"SetIndex(): value must be non-negative\n";
	//	cin.get();
	//	exit(1);
	//}
	vuz.h[histnumber].indexes[indexnumber] = val;
	return vuz;
}
const float GetIndex(const VUZ& vuz, int histnumber, int indexnumber)
{
	if(!CheckBounds(histnumber, indexnumber))
	{
		cout<<"GetIndex(): argument is out of bounds\n";
		cin.get();
		exit(1);
	}
	return vuz.h[histnumber].indexes[indexnumber];
}
VUZ& Setavgr(VUZ& vuz, int histnumber, float val)
{
	vuz.h[histnumber].avergrade = val;
	return vuz;
}
const float Getavgr(const VUZ& vuz, int histnumber)
{
	return vuz.h[histnumber].avergrade;
}
VUZ& Normalize(VUZ& vuz, int histnumber)
{
	if(!CheckBounds(histnumber))
	{
		cout<<"Normalize(): argument is out of bounds\n";
		cin.get();
		exit(1);
	}
	if(HistIsEmpty(vuz, histnumber))
		return vuz;
	float max = vuz.h[histnumber].hist[FindMax(vuz, histnumber)];
	for(int i = 0;i < vuz.h[histnumber].leng;i ++)
		vuz.h[histnumber].normhist[i] = vuz.h[histnumber].hist[i] / max;
	return vuz;
}
VUZ& MakeUnimodal(VUZ& vuz, int histnumber)
{
	int k,& refk = k;
	while(!CheckUnimodal(vuz, histnumber, refk))
	{
		//ResizeHist(vuz, histnumber);
		UniteIntervals(vuz, histnumber, k);
	}
	//	vuz.h[histnumber].hist[k] = vuz.h[histnumber].hist[k - 1] =
	//	(vuz.h[histnumber].hist[k] + vuz.h[histnumber].hist[k - 1]) / 2;
	return vuz;
}
//VUZ& ResizeHist(VUZ& vuz, int histnumber)
//{
//
//	return vuz;
//}
VUZ& UniteIntervals(VUZ& vuz, int histnumber, int pos)
{
	float* values;
	int* intervals;
	try{
		values = new float[vuz.h[histnumber].leng];
		intervals = new int[vuz.h[histnumber].leng];
	}
	catch(bad_alloc& ex){
		cout<<"UniteIntervals(): "<<ex.what()<<'\n';
		cin.get();
		exit(1);
	}
	for(int k = 0;k < vuz.h[histnumber].leng;k ++)
	{
		values[k] = vuz.h[histnumber].hist[k];
		intervals[k] = vuz.h[histnumber].interval[k];
	}
	//-----test printing-----
    /*
	for(int k = 0;k < vuz.h[histnumber].leng;k ++)
		cout<<"temp1["<<setw(2)<<k<<"] = "<<setw(3)<<values[k]
			<<", temp2["<<setw(2)<<k<<"] = "<<setw(3)<<intervals[k]<<'\n';
    */
	//---------------
	delete [] vuz.h[histnumber].hist;
	delete [] vuz.h[histnumber].normhist;
	delete [] vuz.h[histnumber].interval;
	vuz.h[histnumber].leng --;
	try{
		vuz.h[histnumber].hist = new float[vuz.h[histnumber].leng];
		vuz.h[histnumber].normhist = new float[vuz.h[histnumber].leng];
		vuz.h[histnumber].interval = new int[vuz.h[histnumber].leng];
	}
	catch(bad_alloc& ex){
		cout<<"UniteIntervals(): "<<ex.what()<<'\n';
		cin.get();
		exit(1);
	}
	int i = 0, j = 0;
	while(i < vuz.h[histnumber].leng && j < vuz.h[histnumber].leng + 1)
	{
		if(j != pos - 1)
		{
			vuz.h[histnumber].hist[i] = values[j];
			vuz.h[histnumber].normhist[i] = 0.;
			vuz.h[histnumber].interval[i] = intervals[j];
			i ++; j ++;
		}
		else
		{
			vuz.h[histnumber].hist[i] = values[j] + values[j + 1];
			vuz.h[histnumber].normhist[i] = 0;
			vuz.h[histnumber].interval[i] = intervals[j] + intervals[j + 1];
			i ++; j += 2;
		}
	}
	//-----test printing-----
    /*
	for(int k = 0;k < vuz.h[histnumber].leng;k ++)
		cout<<"hist["<<setw(2)<<k<<"] = "<<setw(3)<<vuz.h[histnumber].hist[k]
			<<", nhist["<<setw(2)<<k<<"] = "<<setw(3)<<vuz.h[histnumber].normhist[k]
			<<", i-val["<<setw(2)<<k<<"] = "<<setw(3)<<vuz.h[histnumber].interval[k]<<'\n';
	cout<<'\n';
    */
	//---------------
	delete [] values;
	delete [] intervals;
	return vuz;
}
float CountJager(VUZ& vuz, int histnumber, float weight)
{
	if(weight > 1 || weight < 0 || !CheckBounds(histnumber))
	{
		cout<<"CountJager(): argument is out of bounds\n";
		cin.get();
		exit(1);
	}
	if(HistIsEmpty(vuz, histnumber))
		return -1.;
	float index = 0.;
	const float step = 0.01;
	int left, right,& l = left,& r = right;
	for(float alpha = 1.;alpha > 0.;alpha -= step)
	{
		Alpha_section(vuz, histnumber, l, r, alpha);
		//-----------------------------------------
		//cout<<"a = "<<fixed<<setprecision(2)<<alpha<<", l = "<<l<<", r = "<<r<<'\n';
		//-----------------------------------------
		/*int var1, var2;
		try{
			var1 = IntervalValue(vuz, left, histnumber);
		}
		catch(...){
			cout<<"left\n";
			cin.get();
			exit(1);
		}
		try{
			var2 = IntervalValue(vuz, right, histnumber);
		}
		catch(...){
			cout<<"right\n";
			cin.get();
			exit(1);
		}*/
		if(l != r)
			index += WeightFunction(alpha) *
				((1 - weight)*IntervalValue(vuz, left, histnumber) + weight*IntervalValue(vuz, right, histnumber));
		else
			index += WeightFunction(alpha) * IntervalValue(vuz, right, histnumber);
	}
	return index / (1./step);
}
float CountCentroid(VUZ& vuz, int histnumber)
{
	if(!CheckBounds(histnumber))
	{
		cout<<"CountCentroid(): argument is out of bounds\n";
		cin.get();
		exit(1);
	}
	if(HistIsEmpty(vuz, histnumber))
		return -1.;
	float S = 0., S2 = 0.;
	float temp;
	for(int i = 0;i < vuz.h[histnumber].leng;i ++)
	{
		temp = vuz.h[histnumber].normhist[i] * vuz.h[histnumber].interval[i];
		S += temp;
		S2 += temp *
			IntervalValue(vuz, i, histnumber);
	}
	return S2 / S;
}
float CountminKerre(VUZ& vuz, int histnumber, float* min)
{
	if(!CheckBounds(histnumber))
	{
		cout<<"CountminKerre(): argument is out of bounds\n";
		cin.get();
		exit(1);
	}
	if(HistIsEmpty(vuz, histnumber))
		return -1.;
	float index = 0.;
	//'min' length = SCALE - LOWER_BOUND + 2
	int len = SCALE - LOWER_BOUND + 2;
	float* split = 0,*& sp = split;
	Split(vuz, histnumber, sp);
	//assume that min hist has been already split
	for(int i = 0;i < len;i ++)
		if(i == 0)
			index += fabs(min[i] - split[i]) * vuz.h[histnumber].interval[i];
		else
			index += fabs(min[i] - split[i]);
	return index;
}
float CountmaxKerre(VUZ& vuz, int histnumber, float* max)
{
	if(!CheckBounds(histnumber))
	{
		cout<<"CountmaxKerre(): argument is out of bounds\n";
		cin.get();
		exit(1);
	}
	if(HistIsEmpty(vuz, histnumber))
		return -1.;
	float index = 0.;
	//'max' length = SCALE - LOWER_BOUND + 2
	int len = SCALE - LOWER_BOUND + 2;
	float* split = 0,*& sp = split;
	Split(vuz, histnumber, sp);
	//assume that max hist has been already split
	for(int i = 0;i < len;i ++)
		if(i == 0)
			index += fabs(max[i] - split[i]) * vuz.h[histnumber].interval[i];
		else
			index += fabs(max[i] - split[i]);
	return index;
}
float CountBaas(VUZ& vuz1, VUZ& vuz2, int histnumber)
{
    float* split1 = 0,*& sp1 = split1,* split2 = 0,*& sp2 = split2;
    const int len = SCALE - LOWER_BOUND + 2;

    Split(vuz1, histnumber, sp1);
    Split(vuz2, histnumber, sp2);
	//int len = vuz1.h[histnumber].leng;
    float h;
    float supremum = 0, min, maxh;
	//float* pt1 = vuz1.h[histnumber].normhist,* pt2 = vuz2.h[histnumber].normhist;
    for(int k = 0;k < len;k ++)
    {
		h = sp2[k];
        maxh = 0;
        for(int t = k;t < len;t ++)
        {
            if(sp1[t] < h)
                min = sp1[t];
            else
                min = h;
            if(min > maxh)
                maxh = min;
        }
        if(maxh > supremum)
            supremum = maxh;
        if(supremum == 1)
            break;
        //if(!maxh)
        //	break;
    }
    return supremum;
}
//float CountBaas(VUZ& vuz1, VUZ& vuz2, int histnumber1, int histnumber2)
//{
//    float* split1 = 0,*& sp1 = split1,* split2 = 0,*& sp2 = split2;
//    const int len = SCALE - LOWER_BOUND + 2;
//
//    Split(vuz1, histnumber1, sp1);
//    Split(vuz2, histnumber2, sp2);
//
//    float h;
//    float supremum = 0, min, maxh;
//    for(int k = 0;k < len;k ++)
//    {
//        h = sp2[k];
//        maxh = 0;
//        for(int t = k;t < len;t ++)
//        {
//            if(sp1[t] < h)
//                min = sp1[t];
//            else
//                min = h;
//            if(min > maxh)
//                maxh = min;
//        }
//        if(maxh > supremum)
//            supremum = maxh;
//        if(supremum == 1)
//            break;
//        //if(!maxh)
//        //	break;
//    }
//    return supremum;
//}
list<VUZ>& RangingBaas(list<VUZ>& vList, int histnumber, float**& matrix)
{
	float r, maxr = 0.;
	int counter, maxcounter = 0, i = 0, imax = 0;
	int indexnum = 4;
	for(Iter it = vList.begin();it != vList.end();it ++)
	{
		r = 0.;
		//counter = 0;
		//if(matrix[i][0] < 0) // for i-th VUZ Baas index is not calculated
		//	continue;
		for(int j = 0;j < vList.size();j ++)
		{
			if(j != i) // 
				if(matrix[i][j] >= 0) // if for some VUZ the index is calculated
				{
					r += matrix[i][j];
					//counter ++;
				}
		}
		//if(counter > maxcounter)
		//{
		//	maxcounter = counter;
		//	imax = i;
		//}
		if(r > maxr)
			maxr = r;
		if(r)
			it->h[histnumber].indexes[indexnum] = r;
		i ++;
	}
	for(Iter it = vList.begin();it != vList.end();it ++)
		it->h[histnumber].indexes[indexnum] /= maxr; // to get a relative Baas index
	return vList;
}
float CountAvGrade(VUZ& vuz, int histnumber)
{
	if(!CheckBounds(histnumber))
	{
		cout<<"CountCentroid(): argument is out of bounds\n";
		cin.get();
		exit(1);
	}
	if(HistIsEmpty(vuz, histnumber))
		return -1.;
	float gr = 0;
	for(int i = 0;i < vuz.h[histnumber].leng;i ++) {
		gr += IntervalValue(vuz, i, histnumber) * vuz.h[histnumber].hist[i];
	}
	return gr / vuz.h[histnumber].numstudents;
}

float*& Fuzzymin(list<VUZ>& vList, int histnumber, float*& min)
{
	int len = DEFAULT_LEN;
	//int len = vList.front().h[histnumber].leng;
	int ileft, iright,& l = ileft,& r = iright, valueleft, valueright;
	int minleft, minright;
	int value;
	float alpha;
	const float step = 0.01;
	for(int i = 0;i < len;i ++)
	{
		value = IntervalValue(i);
		alpha = 1.;
		//for(float alpha = 1;alpha > 0;alpha -= step)
		while(alpha > 0)
		{
			minright = minleft = 100;
			for(Iter it = vList.begin();it != vList.end();it ++)
			{
				if(it->h[histnumber].numstudents <= STUD_LIMIT)
					continue;
				if(HistIsEmpty(*it, histnumber))
					continue;
				Alpha_section(*it, histnumber, l, r, alpha);
				if((valueright = IntervalValue(*it, iright, histnumber)) < minright)
					minright = valueright;
				if((valueleft = IntervalValue(*it, ileft, histnumber)) < minleft)
					minleft = valueleft;
			}
			if(value >= minleft && value <= minright)
			{
				min[i] = alpha;
				break;
			}
			alpha -= step;
		}
	}	
	return min;
}
float*& Fuzzymax(list<VUZ>& vList, int histnumber, float*& max)
{
	int len = DEFAULT_LEN;
	int ileft, iright,& l = ileft,& r = iright, valueleft, valueright;
	int maxleft, maxright;
	int value;
	float alpha;
	const float step = 0.001;
	for(int i = 0;i < len;i ++)
	{
		value = IntervalValue(i);
		alpha = 1.;
		//for(float alpha = 1;alpha > 0;alpha -= step)
		//// ????????????????????????
		while(alpha > 0)
		{
			maxleft = maxright = 0;
			for(Iter it = vList.begin();it != vList.end();it ++)
			{
				if(it->h[histnumber].numstudents <= STUD_LIMIT)
					continue;
				if(HistIsEmpty(*it, histnumber))
					continue;
				Alpha_section(*it, histnumber, l, r, alpha);
				if((valueright = IntervalValue(*it, iright, histnumber)) > maxright)
					maxright = valueright;
				if((valueleft = IntervalValue(*it, ileft, histnumber)) > maxleft)
					maxleft = valueleft;
			}	
			if(value >= maxleft && value <= maxright)
			{
				max[i] = alpha;
				break;
			}
			alpha -= step;
		}
		//alpha -= step;
	}
	return max;
}
