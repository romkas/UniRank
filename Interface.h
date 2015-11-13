#pragma once
#include "VUZ.h"
#include <iostream>
#include <fstream>
#include <io.h>
#include <cmath>
#include <iomanip>

// функции преобразования гистограмм, вычисления индексов 
VUZ& Normalize(VUZ& vuz, int histnumber); // нормировка гистограмм по высоте
VUZ& MakeUnimodal(VUZ& vuz, int histnumber); // приведение к унимодальному виду
float CountJager(VUZ& vuz, int histnumber, float weight_rightbound); // индекс Ягера с lambda=weight_rightbound
float CountCentroid(VUZ& vuz, int histnumber); // Центроидный индекс
float CountminKerre(VUZ& vuz, int histnumber, float* min); // Керри до минимума
float CountmaxKerre(VUZ& vuz, int histnumber, float* max); // Керри до максимума
float CountBaas(VUZ&, VUZ&, int histnumber); // индекс Бааса
list<VUZ>& RangingBaas(list<VUZ>& vList, int histnumber, float**& mtx); // ранжирование по Баасу
float CountAvGrade(VUZ& vuz, int histnumber); // средний балл по гистограмме
//float CountBaas(VUZ&, VUZ&, int histnumber1, int histnumber2);

//возвращаемое значение типа int - номер специальности
int OnCountingJager(list<VUZ>& vList);
int OnCountingCentroid(list<VUZ>& vList);
int OnCountingKerre(list<VUZ>& vList, int& mode); // mode - kerreMin или kerreMax
int OnCountingBaas(list<VUZ>& vList, float**& matrix); // matrix - матрица индексов парных сравнений
int OnCountingAvGrade(list<VUZ>& vList);
int OnCountingNumStud(list<VUZ>& vList);
//float** CreateBaasMatrix(float** pt, int V_size);

// функции общего назначения
int MyRound(float x); // only for positive numbers
VUZ& AddGrade(VUZ& vuz, int grade, int histnumber=0); // добавление среднего балла студента для гистограммы histnumber
bool CheckBounds(int histnumber, int indexnumber=0); // проверка histnumber и indexnumber на корректность
bool CheckUnimodal(VUZ& vuz, int histnumber, int& defectindex); // проверка на унимодальность
int FindMax(VUZ& vuz, int histnumber); // поиск столбца максимальной высоты гистограммы вуза
int FindMax_norm(VUZ& vuz, int histnumber); // поиск столбца максимальной высоты нормированной гистограммы вуза
VUZ& Alpha_section(VUZ& vuz, int histnumber, int& left, int& right, float alpha); // определение left и right границ а-среза
float WeightFunction(float alpha); // значение весовой функции для индекса Ягера
bool HistIsEmpty(VUZ& vuz, int histnumber); // проверка на "пустоту"
int IntervalValue(VUZ& vuz, int index, int histnumber=0); // значение середины интервала
int IntervalValue(const VUZ& vuz, int index, int histnumber=0);
int IntervalValue(int index); // значение середины интервала по умолчанию
//-----test-----
//int IntervalValue(VUZ& vuz, int index, int code, int histnumber);
//--------------
float StrToNum(char* number);
bool NumberIsWhole(char* strnumber, int& pointpos);
bool IsNumber(char* strnumber);
//list<VUZ>& Sort(list<VUZ>& vList, int histnumber, int i, int left_bound, int right_bound, bool incremental=false);
//VUZ& Copy(VUZ& destination, VUZ& source);
void Prepare(list<VUZ>& vList);
float*& SetZeros(float*& ptr, int len);
//VUZ& ResizeHist(VUZ& vuz, int histnumber);
VUZ& UniteIntervals(VUZ& vuz, int histnumber, int position);
float*& AllocateMem(float*& ptr, int len);
float*& Split(VUZ& vuz, int histnumber, float*& ptr_split);
float*& Split(float* const source, const int interval, float*& ptr_split);
float**& CreateBaasMatrix(float**& pt, int V_size);
void DeleteBaasMatrix(float**& mtx, int mtx_sz);
float*& Fuzzymin(list<VUZ>& vList, int histnumber, float*& min); // максимизация нечётких чисел
float*& Fuzzymax(list<VUZ>& vList, int histnumber, float*& max); // минимизация нечётких чисел

// функции чтения и записи
void Readfile(const char* filename, list<VUZ>& vList); // чтение исходного файла
void PrintIndex(const char* filename, list<VUZ>& vList, int histnumber, int i); // печать списка вузов и i-го индекса
void PrintNumStud(const char* filename, list<VUZ>& vList, int histnumber); // печать списка вузов с числом поступивших
void PrintMatrix(const char* filename, float**& matrix, const int m_sz, int histnumber); // печать матрицы индексов Бааса
void PrintHistogram(const char* filename, VUZ& vuz, int histnumber); // печать гистограммы в файл
void PrintMenu();

// функции сравнения двух вузов по индексу; используются в std::list<VUZ>::sort
// по Ягеру, отношение '>'
bool cmp_jager_gen(const VUZ& first, const VUZ& second);
bool cmp_jager_eco(const VUZ& first, const VUZ& second);
bool cmp_jager_man(const VUZ& first, const VUZ& second);
bool cmp_jager_con(const VUZ& first, const VUZ& second);
bool cmp_jager_inf(const VUZ& first, const VUZ& second);
// Центроидный, '>'
bool cmp_centr_gen(const VUZ& first, const VUZ& second);
bool cmp_centr_eco(const VUZ& first, const VUZ& second);
bool cmp_centr_man(const VUZ& first, const VUZ& second);
bool cmp_centr_con(const VUZ& first, const VUZ& second);
bool cmp_centr_inf(const VUZ& first, const VUZ& second);
// по Керри, '<'
bool cmp_kerre_gen(const VUZ& first, const VUZ& second);
bool cmp_kerre_eco(const VUZ& first, const VUZ& second);
bool cmp_kerre_man(const VUZ& first, const VUZ& second);
bool cmp_kerre_con(const VUZ& first, const VUZ& second);
bool cmp_kerre_inf(const VUZ& first, const VUZ& second);
// по Баасу, '>'
bool cmp_baas_gen(const VUZ& first, const VUZ& second);
bool cmp_baas_eco(const VUZ& first, const VUZ& second);
bool cmp_baas_man(const VUZ& first, const VUZ& second);
bool cmp_baas_con(const VUZ& first, const VUZ& second);
bool cmp_baas_inf(const VUZ& first, const VUZ& second);
// по среднему баллу, '>'
bool cmp_avgr_gen(const VUZ& first, const VUZ& second);
bool cmp_avgr_eco(const VUZ& first, const VUZ& second);
bool cmp_avgr_man(const VUZ& first, const VUZ& second);
bool cmp_avgr_con(const VUZ& first, const VUZ& second);
bool cmp_avgr_inf(const VUZ& first, const VUZ& second);
// по числу поступивших, '>'
bool cmp_nstd_gen(const VUZ& first, const VUZ& second);
bool cmp_nstd_eco(const VUZ& first, const VUZ& second);
bool cmp_nstd_man(const VUZ& first, const VUZ& second);
bool cmp_nstd_con(const VUZ& first, const VUZ& second);
bool cmp_nstd_inf(const VUZ& first, const VUZ& second);

list<VUZ>& SortJager(list<VUZ>& vList, int histnumber);
list<VUZ>& SortCentr(list<VUZ>& vList, int histnumber);
list<VUZ>& SortmaxKerre(list<VUZ>& vList, int histnumber);
list<VUZ>& SortBaas(list<VUZ>& vList, int histnumber);
list<VUZ>& SortAvGrade(list<VUZ>& vList, int histnumber);
list<VUZ>& SortNumStud(list<VUZ>& vList, int histnumber);
