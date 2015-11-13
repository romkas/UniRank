#include "Interface.h"

void Readfile(const char* filename, list<VUZ>& vList)
{
	//VUZ Temp;
	char basis[STRLENG], name[STRLENG], speciality[STRLENG];
	//char temp[STRLENG];
	float grade;
	int i = 0;
	fstream f(filename);
	if(!f)
	{
		cout<<"Readfile(): unable to open the file to read\n";
		cin.get();
		exit(1);
	}
	//---the way function should read a text file---
	char str[STRLENG];
	int option;
	cout<<"To consider:\n"
		"<0> all graduates\n<1> competitive base\n<2> out-of-competitive base\n<3> aimed base\n";
	while(true)
	{
		cin.getline(str, STRLENG);
		option = int(StrToNum(str));
		if(option >= 0 && option <= 3)
			break;
	}
	cout<<option<<" accepted\n";
	//----------------------------------------------
	bool flag = true;
	while(!f.eof())
	{
		f>>basis>>name>>speciality>>grade;	
		f.ignore(100, '\n');
		if(option != 0)
			if(strcmp(basis, BASES[option - 1]) != 0)
				continue;
		if(flag)
		{
			vList.push_back(*(new VUZ));
			AddStudent(Setname(Init(vList.back()), name), grade, speciality);
			flag = false;
			continue;
		}
		if(strcmp(name, Getname(vList.back())) == 0)
		{
			AddStudent(vList.back(), grade, speciality);
		}
		else
		{
			vList.push_back(*(new VUZ));
			AddStudent(Setname(Init(vList.back()), name), grade, speciality);
		}
	}
	f.close();
	return;
}
void PrintIndex(const char* filename, list<VUZ>& vList, int histnumber, int i)
{
	fstream f;
	int j = 1;

	if(!CheckBounds(histnumber, i))
	{
		cout<<"Print(): argument is out of bounds\n";
		cin.get();
		exit(1);
	}
	f.open(filename, fstream::app);
	if(!f)
	{
		cout<<"Print(): unable to open the file to write\n";
		cin.get();
		exit(1);
	}
	if(histnumber != 0)
		f<<"## speciality - "<<SPECNAMES[histnumber - 1]<<". "<<INDEXNAMES[i]<<" ##\n";
	else
		f<<"## Все специальности. "<<INDEXNAMES[i]<<" ##\n";
	for(cIter it = vList.begin();it != vList.end();it ++)
	{
		float key = GetIndex(*it, histnumber, i);
		if(key >= 0)
		{
			f<<setw(3)<<j ++<<") "<<setw(154)<<left<<Getname(*it)
				<<right<<fixed<<setprecision(3)<<key<<'\n';
		}
	}
	f<<'\n';
	f.close();
	return;
}
void PrintNumStud(const char* filename, list<VUZ>& vList, int histnumber)
{
	fstream f;
	int j = 1;

	if(!CheckBounds(histnumber))
	{
		cout<<"Print(): argument is out of bounds\n";
		cin.get();
		exit(1);
	}
	f.open(filename, fstream::app);
	if(!f)
	{
		cout<<"Print(): unable to open the file to write\n";
		cin.get();
		exit(1);
	}
	if(histnumber != 0)
		f<<"## speciality - "<<SPECNAMES[histnumber - 1]<<". Число поступивших ##\n";
	else
		f<<"## Все специальности. Число поступивших ##\n";
	for(cIter it = vList.begin();it != vList.end();it ++)
	{
		f<<setw(3)<<j ++<<") "<<setw(154)<<left<<Getname(*it)
			<<right<<setw(4)<<it->h[histnumber].numstudents<<'\n';
	}
	f<<'\n';
	f.close();
	return;
}
void PrintMatrix(const char* filename, float**& matrix, const int m_sz, int histnumber)
{
    fstream f;
	if(!CheckBounds(histnumber))
    {
        cout<<"Print(): argument is out of bounds\n";
        cin.get();
        exit(1);
    }
    f.open(filename, fstream::app);
    if(!f)
    {
        cout<<"Print(): unable to open the file to write\n";
        cin.get();
        exit(1);
    }
    if(histnumber != 0)
        f<<"## speciality - "<<SPECNAMES[histnumber - 1]<<". Индекс Бааса ##\n";
    else
        f<<"## Все специальности. Индекс Бааса ##\n";
    for(int k = 0;k < m_sz;k ++)
    {
        for(int t = 0;t < m_sz;t ++)
        {
            if(k != t)
            {
                if(t != m_sz - 1)
                    f<<fixed<<setprecision(2)<<matrix[k][t]<<'\t';
                else
                    f<<fixed<<setprecision(2)<<matrix[k][t];
            }
            else
                f<<"\t\t";
        }
        f<<'\n';
    }
    f<<'\n';
    f.close();
    return;
}
void PrintHistogram(const char* filename, VUZ& vuz, int histnumber)
{
	if(!CheckBounds(histnumber))
	{
		cout<<"PrintHistogram(): argument is out of bounds\n";
		cin.get();
		exit(1);
	}
	fstream f;
	f.open(filename, fstream::app);
	if(!f)
	{
		cout<<"PrintHistogram(): unable to open the file to write\n";
		cin.get();
		exit(1);
	}
	f<<setw(154)<<left<<vuz.name<<'\n'<<right;
	for(int i = 0;i < vuz.h[histnumber].leng;i ++)
		f<<"h["<<setw(2)<<i<<"]: "<<setw(3)<<vuz.h[histnumber].hist[i]<<setw(5)<<"nh["
		<<setw(2)<<i<<"]: "<<fixed<<setprecision(2)<<vuz.h[histnumber].normhist[i]
		<<"  intleng = "<<vuz.h[histnumber].interval[i]<<'\n';
	f<<'\n';
	f.close();
	return;
}
void PrintMenu()
{
	cout<<"Do you want to calculate:\n<1> Yager\n"
		"<2> Centroid\n"
		"<3> Kerre\n"
		"<4> Baas\n"
		"<5> average grade\n"
		"<6> number of students\n"
		"type 'q' to stop the program\n";
	return;
}


int OnCountingJager(list<VUZ>& vList)
{
	char str[STRLENG];
	float lambda;
	int spec_num;
	int serialnum = 0;
	cout<<"Lambda value ( 0; 1 ]: ";
	while(true)
	{
		cin.getline(str, STRLENG);
		lambda = StrToNum(str);
		if(lambda > 0 && lambda <= 1)
		{
			cout<<lambda<<" accepted\n";
			break;
		}
		cout<<"Incorrect. Try once again: ";
	}
	cout<<"Choose the speciality\n<0> all specialities\n<1> economics\n<2> management\n"
		"<3> state and municipal control\n<4> business informatics\n";
	while(true)
	{
		cin.getline(str, STRLENG);
		spec_num = int(StrToNum(str));
		if(spec_num >= 0 && spec_num <= 4)
		{
			cout<<spec_num<<" accepted\n";
			break;
		}
		cout<<"Incorrect. Try once again: ";
	}
	for(Iter it = vList.begin();it != vList.end();it ++)
    {
        //try
        //{
		if(it->h[spec_num].numstudents > STUD_LIMIT)
			SetIndex(*it, spec_num, serialnum, CountJager(*it, spec_num, lambda));
        //}
        //catch(int ex)
        //{
        //    cout<<"EXCEPTION in OnCountingJager(): "<<ex<<'\n';
        //    cin.get();
        //    cin.ignore(100, '\n');
        //}

    }
    cout<<"Completed!\n";
	return spec_num;
}
int OnCountingCentroid(list<VUZ>& vList)
{
	char str[STRLENG];
	int spec_num;
	int serialnum = 1;
	cout<<"Choose the speciality\n<0> all specialities\n"
		"<1> economics\n<2> management\n<3> state and municipal controls\n<4> business informatics\n";
	while(true)
	{
		cin.getline(str, STRLENG);
		spec_num = int(StrToNum(str));
		if(spec_num >= 0 && spec_num <= 4)
		{
			cout<<spec_num<<" accepted\n";
			break;
		}
		cout<<"Incorrect. Try once again: ";
	}
	for(Iter it = vList.begin();it != vList.end();it ++)
		if(it->h[spec_num].numstudents > STUD_LIMIT)
			SetIndex(*it, spec_num, serialnum, CountCentroid(*it, spec_num));
	cout<<"Completed!\n";
	return spec_num;
}
int OnCountingKerre(list<VUZ>& vList, int& mode)
{
	char str[STRLENG];
	int spec_num;
	float(*f1)(VUZ&, int, float*);
	float*&(*f2)(list<VUZ>&, int, float*&);
	cout<<"\"max\" - distance to the maximum\n"
		"\"min\" - distance to the minumum\nType one of the options: ";
	while(true)
	{
		cin.getline(str, STRLENG);
		if(strcmp(str, "min") == 0)
		{
			f1 = CountminKerre;
			f2 = Fuzzymin;
			mode = 2;
			break;
		}
		if(strcmp(str, "max") == 0)
		{
			f1 = CountmaxKerre;
			f2 = Fuzzymax;
			mode = 3;
			break;
		}
		cout<<"Incorrect. Try once again: ";
	}
	cout<<str<<" accepted\n";
	cout<<"Choose the speciality\n<0> all specialities\n"
		"<1> economics\n<2> management\n<3> state and municipal control\n<4> business informatics\n";
	while(true)
	{
		cin.getline(str, STRLENG);
		spec_num = int(StrToNum(str));
		if(spec_num >= 0 && spec_num <= 4)
		{
			cout<<spec_num<<" accepted\n";
			break;
		}
		cout<<"Incorrect. Try once again: ";
	}
	float* ptr = 0,*& p = ptr;
	float* split = 0,*& sp = split;
	
	//AllocateMem(p, DEFAULT_LEN);
	//SetZeros(p, DEFAULT_LEN);
	//f2(vList, spec_num, p);
	//Split(p, DEFAULT_INTERVAL, sp);
	
	f2(vList, spec_num, SetZeros(AllocateMem(p, DEFAULT_LEN), DEFAULT_LEN));
	
	cout<<"Print max/min histogram? (y/n): ";
	while(true)
	{
		cin.getline(str, STRLENG);
		if(str[0] == 'y') {
			cout<<"Printing\n";
			fstream fs;
			const char* fn = "C:\\Users\\ROM\\Desktop\\practise-2\\07-02_unite3\\hist_max_07-02.txt";
			fs.open(fn, ios::app);
			if(fs) {
				for(int t = 0;t < DEFAULT_LEN;t ++)
					fs<<"h["<<setw(2)<<t<<"]: "<<setw(3)<<ptr[t]<<'\n';
			}
			fs.close();
			cout<<"Printed\n";
			break;
		}
		else if(str[0] == 'n')
			break;
		cout<<"Incorrect. Try once again: ";
	}

	Split(p, DEFAULT_INTERVAL, sp);
	for(Iter it = vList.begin();it != vList.end();it ++)
		if(it->h[spec_num].numstudents > STUD_LIMIT)
			SetIndex(*it, spec_num, mode, f1(*it, spec_num, split));
	cout<<"Completed!\n";
	return spec_num;
}
int OnCountingBaas(list<VUZ>& vList, float**& matrix)
{
    char str[STRLENG];
    int spec_num, i, j;
    cout<<"Choose the speciality\n<0> all specialities\n"
        "<1> economics\n<2> management\n<3> state and municipal controls\n<4> business informatics\n";
    while(true)
    {
        cin.getline(str, STRLENG);
        spec_num = int(StrToNum(str));
        if(spec_num >= 0 && spec_num <= 4)
        {
            cout<<spec_num<<" accepted\n";
            break;
        }
        cout<<"Incorrect. Try once again: ";
    }

    CreateBaasMatrix(matrix, vList.size());

	i = -1;
    for(Iter it1 = vList.begin();it1 != vList.end();it1 ++)
    {
        j = -1;
		i ++;
		if(it1->h[spec_num].numstudents <= STUD_LIMIT)
			continue;
		for(Iter it2 = vList.begin();it2 != vList.end();it2 ++)
        {
			j ++;
			if(it2->h[spec_num].numstudents <= STUD_LIMIT)
				continue;
			if(i != j)
            {
                matrix[i][j] = CountBaas(*it1, *it2, spec_num);
            }
        }
        //SetIndex(V[i], spec_num, serialnum, CountCentroid(V[i], spec_num));
    }
    cout<<"Completed!\n";
    return spec_num;
}
int OnCountingAvGrade(list<VUZ>& vList)
{
	char str[STRLENG];
	int spec_num, serialnum = 5;
	cout<<"Choose the speciality\n<0> all specialities\n"
		"<1> economics\n<2> management\n<3> state and municipal control\n<4> business informatics\n";
	while(true)
	{
		cin.getline(str, STRLENG);
		spec_num = int(StrToNum(str));
		if(spec_num >= 0 && spec_num <= 4)
		{
			cout<<spec_num<<" accepted\n";
			break;
		}
		cout<<"Incorrect. Try once again: ";
	}
	for(Iter it = vList.begin();it != vList.end();it ++)
		if(it->h[spec_num].numstudents > STUD_LIMIT)
			SetIndex(*it, spec_num, serialnum, Getavgr(*it, spec_num));
	cout<<"Completed!\n";
	return spec_num;
}
int OnCountingNumStud(list<VUZ>& vList)
{
	char str[STRLENG];
	int spec_num, serialnum = 5;
	cout<<"Choose the speciality\n<0> all specialities\n"
		"<1> economics\n<2> management\n<3> state and municipal control\n<4> business informatics\n";
	while(true)
	{
		cin.getline(str, STRLENG);
		spec_num = int(StrToNum(str));
		if(spec_num >= 0 && spec_num <= 4)
		{
			cout<<spec_num<<" accepted\n";
			break;
		}
		cout<<"Incorrect. Try once again: ";
	}
	cout<<"Completed!\n";
	return spec_num;
}
