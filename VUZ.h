#pragma once
#include <vector>
#include <list>
#include <cstring>
using namespace std;

#define STRLENG 256
#define NUMSPECS 4
#define NUMINDS 6
#define DEFAULT_INTERVAL 3
#define LOWER_BOUND 40
#define SCALE 100
#define NUMBASES 4
#define STUD_LIMIT 20

const int DEFAULT_LEN = (SCALE - LOWER_BOUND) / DEFAULT_INTERVAL + 1;

const char SPECNAMES[NUMSPECS][STRLENG] = {"Экономика", "Менеджмент",
	"Государственное_и_муниципальное_управление", "Бизнес-информатика"};

const char BASES[NUMBASES][STRLENG] = {"по_конкурсу", "вне_конкурса",
	"целевой_прием", "без_экзаменов"};

const char INDEXNAMES[NUMINDS][STRLENG] = {"Индекс Ягера", "Центроидный индекс",
	"Расстояние по Керри до минимума", "Расстояние по Керри до максимума", 
	"Индекс Бааса", "Средний балл"};

struct Histogram {
	int numstudents;
	float avergrade;
	float* hist;
	float* normhist;
	int leng;
	int* interval;
	float indexes[NUMINDS];
	//0 - Jager, 1 - Centroid, 2 - minKerre, 3 - maxKerre, 4 - Baas, 5 - avGrade
};
struct VUZ {
	char name[STRLENG];
	Histogram h[NUMSPECS + 1];
};

VUZ& Init(VUZ& vuz);
VUZ& Delete(VUZ& vuz);
VUZ& AddStudent(VUZ& vuz, float grade, const char* speciality);
VUZ& Setname(VUZ& vuz, const char* newname);
const char* Getname(const VUZ& vuz);
VUZ& SetIndex(VUZ& vuz, int histnumber, int indexnumber, float value);
const float GetIndex(const VUZ& vuz, int histnumber, int indexnumber);
VUZ& Setavgr(VUZ& vuz, int histnumber, float value);
const float Getavgr(const VUZ& vuz, int histnumber);

typedef list<VUZ>::iterator Iter;
typedef list<VUZ>::const_iterator cIter;