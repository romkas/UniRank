#pragma once
#include "VUZ.h"
#include <iostream>
#include <fstream>
#include <io.h>
#include <cmath>
#include <iomanip>

// ������� �������������� ����������, ���������� �������� 
VUZ& Normalize(VUZ& vuz, int histnumber); // ���������� ���������� �� ������
VUZ& MakeUnimodal(VUZ& vuz, int histnumber); // ���������� � ������������� ����
float CountJager(VUZ& vuz, int histnumber, float weight_rightbound); // ������ ����� � lambda=weight_rightbound
float CountCentroid(VUZ& vuz, int histnumber); // ����������� ������
float CountminKerre(VUZ& vuz, int histnumber, float* min); // ����� �� ��������
float CountmaxKerre(VUZ& vuz, int histnumber, float* max); // ����� �� ���������
float CountBaas(VUZ&, VUZ&, int histnumber); // ������ �����
list<VUZ>& RangingBaas(list<VUZ>& vList, int histnumber, float**& mtx); // ������������ �� �����
float CountAvGrade(VUZ& vuz, int histnumber); // ������� ���� �� �����������
//float CountBaas(VUZ&, VUZ&, int histnumber1, int histnumber2);

//������������ �������� ���� int - ����� �������������
int OnCountingJager(list<VUZ>& vList);
int OnCountingCentroid(list<VUZ>& vList);
int OnCountingKerre(list<VUZ>& vList, int& mode); // mode - kerreMin ��� kerreMax
int OnCountingBaas(list<VUZ>& vList, float**& matrix); // matrix - ������� �������� ������ ���������
int OnCountingAvGrade(list<VUZ>& vList);
int OnCountingNumStud(list<VUZ>& vList);
//float** CreateBaasMatrix(float** pt, int V_size);

// ������� ������ ����������
int MyRound(float x); // only for positive numbers
VUZ& AddGrade(VUZ& vuz, int grade, int histnumber=0); // ���������� �������� ����� �������� ��� ����������� histnumber
bool CheckBounds(int histnumber, int indexnumber=0); // �������� histnumber � indexnumber �� ������������
bool CheckUnimodal(VUZ& vuz, int histnumber, int& defectindex); // �������� �� ��������������
int FindMax(VUZ& vuz, int histnumber); // ����� ������� ������������ ������ ����������� ����
int FindMax_norm(VUZ& vuz, int histnumber); // ����� ������� ������������ ������ ������������� ����������� ����
VUZ& Alpha_section(VUZ& vuz, int histnumber, int& left, int& right, float alpha); // ����������� left � right ������ �-�����
float WeightFunction(float alpha); // �������� ������� ������� ��� ������� �����
bool HistIsEmpty(VUZ& vuz, int histnumber); // �������� �� "�������"
int IntervalValue(VUZ& vuz, int index, int histnumber=0); // �������� �������� ���������
int IntervalValue(const VUZ& vuz, int index, int histnumber=0);
int IntervalValue(int index); // �������� �������� ��������� �� ���������
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
float*& Fuzzymin(list<VUZ>& vList, int histnumber, float*& min); // ������������ �������� �����
float*& Fuzzymax(list<VUZ>& vList, int histnumber, float*& max); // ����������� �������� �����

// ������� ������ � ������
void Readfile(const char* filename, list<VUZ>& vList); // ������ ��������� �����
void PrintIndex(const char* filename, list<VUZ>& vList, int histnumber, int i); // ������ ������ ����� � i-�� �������
void PrintNumStud(const char* filename, list<VUZ>& vList, int histnumber); // ������ ������ ����� � ������ �����������
void PrintMatrix(const char* filename, float**& matrix, const int m_sz, int histnumber); // ������ ������� �������� �����
void PrintHistogram(const char* filename, VUZ& vuz, int histnumber); // ������ ����������� � ����
void PrintMenu();

// ������� ��������� ���� ����� �� �������; ������������ � std::list<VUZ>::sort
// �� �����, ��������� '>'
bool cmp_jager_gen(const VUZ& first, const VUZ& second);
bool cmp_jager_eco(const VUZ& first, const VUZ& second);
bool cmp_jager_man(const VUZ& first, const VUZ& second);
bool cmp_jager_con(const VUZ& first, const VUZ& second);
bool cmp_jager_inf(const VUZ& first, const VUZ& second);
// �����������, '>'
bool cmp_centr_gen(const VUZ& first, const VUZ& second);
bool cmp_centr_eco(const VUZ& first, const VUZ& second);
bool cmp_centr_man(const VUZ& first, const VUZ& second);
bool cmp_centr_con(const VUZ& first, const VUZ& second);
bool cmp_centr_inf(const VUZ& first, const VUZ& second);
// �� �����, '<'
bool cmp_kerre_gen(const VUZ& first, const VUZ& second);
bool cmp_kerre_eco(const VUZ& first, const VUZ& second);
bool cmp_kerre_man(const VUZ& first, const VUZ& second);
bool cmp_kerre_con(const VUZ& first, const VUZ& second);
bool cmp_kerre_inf(const VUZ& first, const VUZ& second);
// �� �����, '>'
bool cmp_baas_gen(const VUZ& first, const VUZ& second);
bool cmp_baas_eco(const VUZ& first, const VUZ& second);
bool cmp_baas_man(const VUZ& first, const VUZ& second);
bool cmp_baas_con(const VUZ& first, const VUZ& second);
bool cmp_baas_inf(const VUZ& first, const VUZ& second);
// �� �������� �����, '>'
bool cmp_avgr_gen(const VUZ& first, const VUZ& second);
bool cmp_avgr_eco(const VUZ& first, const VUZ& second);
bool cmp_avgr_man(const VUZ& first, const VUZ& second);
bool cmp_avgr_con(const VUZ& first, const VUZ& second);
bool cmp_avgr_inf(const VUZ& first, const VUZ& second);
// �� ����� �����������, '>'
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
