#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>

using namespace std;

#define NUM_IND 7

struct VUZ {
	string name;
	double index;
	double rank;
};
VUZ& setname(VUZ& vuz, const string& name) {
	vuz.name = name;
	return vuz;
}
VUZ& setindex(VUZ& vuz, double index) {
	vuz.index = index;
	return vuz;
}
VUZ& setrank(VUZ& vuz, double rank) {
	vuz.rank = rank;
	return vuz;
}
const string& getname(const VUZ& vuz) {
	return vuz.name;
}
double getindex(const VUZ& vuz) {
	return vuz.index;
}
double getrank(const VUZ& vuz) {
	return vuz.rank;
}
void readfile(const char* filename, vector<VUZ>& V, vector<int>& link_lens, vector<int>& link_posns) {
	fstream f;
	string buf, s = "";
	char c;
	int k, rank = 0;
	double r;
	bool flag = true;

	f.open(filename);
	if(!f) {
		cout<<"Unable to open file\n";
		cin.get();
		exit(1);
	}
	while(!f.eof()) {
		getline(f, buf);
		k = 0;
		rank ++;
		while(buf[k] != ')') {
			k ++;
		}
		
		k += 2;
		while(buf[k] != ' ' && buf[k] != '\t') {
			s += buf[k ++];
		}
		V.push_back(*(new VUZ));
		setname(V.back(), s);
		s.clear();
		
		setrank(V.back(), rank);
		
		while(buf[k] == ' ' || buf[k] == '\t') {
			k ++;
		}
		
		while(buf[k] != '\0') {
			s += buf[k ++];
		}
		setindex(V.back(), atof(s.c_str()));
		s.clear();
		if(rank > 1) { // считываем НЕ первую запись из файла
			if(getindex(V.back()) == r) { // индексы совпали
				if(flag) { // если мы выясняем, что есть связка
					link_posns.push_back(rank-2);
					link_lens.push_back(2);
					flag = false;
				}
				else {
					link_lens.back() ++;
				}
			}
			else {
				flag = true;
			}
		}
		//else {
		//	if(getindex(V.back()) != r) {
		//		counter ++;
		//		//link_len = 1;
		//		setlink(V.back());
		//		//setlinklen(V.back());
		//		flag2 = true;
		//	}
		//	else {
		//		if(counter == 1) {
		//			positions.push_back(0);
		//		}
		//		if(flag2) {
		//			links.push_back(2);
		//			flag2 = false;
		//		}
		//		else {
		//			links.back() ++;
		//		}
		//		//link_len ++;
		//		setlink(V.back(), true);
		//		//setlinklen(V.back(), link_len);
		//	}
		//	setrank(V.back(), counter);
		//}
		r = getindex(V.back());
	}
	f.close();
	return;
}
void readfile(const char* filename, vector<VUZ>& V) {
	fstream f;
	string buf, s = "";
	char c;
	int k, rank = 0;
	bool flag = true;

	f.open(filename);
	if(!f) {
		cout<<"Unable to open file\n";
		cin.get();
		exit(1);
	}
	while(!f.eof()) {
		getline(f, buf);
		k = 0;
		rank ++;
		while(buf[k] != ')') {
			k ++;
		}
		
		k += 2;
		while(buf[k] != ' ' && buf[k] != '\t') {
			s += buf[k ++];
		}
		V.push_back(*(new VUZ));
		setname(V.back(), s);
		s.clear();
		
		setrank(V.back(), rank);
		
		while(buf[k] == ' ' || buf[k] == '\t') {
			k ++;
		}
		
		while(buf[k] != '\0') {
			s += buf[k ++];
		}
		setindex(V.back(), atof(s.c_str()));
		s.clear();
	}
	f.close();
	return;
}
int Find(const vector<VUZ>& V, const string& s) {
	for(int i = 0;i < V.size();i ++) {
		if(s == getname(V[i]))
			return getrank(V[i]);
	}
	return -1;
}
int is_linked(const vector<int>& lens, const vector<int>& posns, int i) {
	int N = lens.size();
	for(int k = 0;k < N;k ++) {
		if(i >= posns[k] && i < posns[k] + lens[k]) {
			return k;
		}
	}
	return -1;
}
vector<VUZ>& correct_ranks(vector<VUZ>& V, const vector<int>& lens, const vector<int>& posns) {
	int i = 0, k;
	double r;
	while(i < V.size()) {
		if((k = is_linked(lens, posns, i)) >= 0) {
			r = 0;
			for(int t = posns[k];t < posns[k] + lens[k];t ++) {
				r += getrank(V[t]);
			}
			r /= lens[k];
			for(int j = posns[k];j < posns[k] + lens[k];j ++) {
				setrank(V[j], r);
			}
			i += lens[k];
		}
		else { i ++; }
	}
	return V;
}
double correlation(vector<VUZ>& V1, vector<VUZ>& V2,
				   const vector<int>& lin_len1, const vector<int>& lin_pos1,
				   const vector<int>& lin_len2, const vector<int>& lin_pos2) {
	double D = 0, r1, r2, delta = 0;
	int N = V1.size();
	for(int i = 0;i < N;i ++) {
		r1 = getrank(V1[i]);
		r2 = Find(V2, getname(V1[i]));
		D += (r1 - r2) * (r1 - r2);
	}
	for(int i = 0;i < lin_len1.size();i ++) {
		delta += lin_len1[i] * (lin_len1[i]*lin_len1[i] - 1);
	}
	for(int i = 0;i < lin_len2.size();i ++) {
		delta += lin_len2[i] * (lin_len2[i]*lin_len2[i] - 1);
	}
	delta /= 12;
	return 1 - (6*D + delta) / (N*N*N - N);
}