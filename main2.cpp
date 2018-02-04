#include "head.h"

int main()
{
	const char* res = "C:\\Users\\ROM\\Desktop\\practise-2\\07-07-unite\\correlation-unite.txt";
	const char* sources[NUM_IND] = { "C:\\Users\\ROM\\Desktop\\practise-2\\07-07-unite\\jager025.txt",
		"C:\\Users\\ROM\\Desktop\\practise-2\\07-07-unite\\jager05.txt",
		"C:\\Users\\ROM\\Desktop\\practise-2\\07-07-unite\\jager075.txt",
		"C:\\Users\\ROM\\Desktop\\practise-2\\07-07-unite\\centr.txt",
		"C:\\Users\\ROM\\Desktop\\practise-2\\07-07-unite\\kerremax.txt",
		"C:\\Users\\ROM\\Desktop\\practise-2\\07-07-unite\\baas.txt",
		"C:\\Users\\ROM\\Desktop\\practise-2\\07-07-unite\\avgrade_noround.txt" };
	fstream f;
	vector<VUZ> V1, V2;
	vector<int> LINK_LENGTHS[NUM_IND];
	vector<int> POSITIONS[NUM_IND];
	double cor_mtx[NUM_IND][NUM_IND];
	for(int i = 0;i < NUM_IND;i ++) {
		cor_mtx[i][i] = 1;
		readfile(sources[i], V1, LINK_LENGTHS[i], POSITIONS[i]);
		correct_ranks(V1, LINK_LENGTHS[i], POSITIONS[i]);
		for(int j = i+1;j < NUM_IND;j ++) {
			readfile(sources[j], V2);
			correct_ranks(V2, LINK_LENGTHS[j], POSITIONS[j]);
			cor_mtx[i][j] = correlation(V1, V2,
				LINK_LENGTHS[i], POSITIONS[i],
				LINK_LENGTHS[j], POSITIONS[j] );
			V2.clear();
			LINK_LENGTHS[j].clear();
			POSITIONS[j].clear();
		}
		V1.clear();
		LINK_LENGTHS[i].clear();
		POSITIONS[i].clear();
	}
	for(int i = 0;i < NUM_IND;i ++) {
		for(int j = i+1;j < NUM_IND;j ++) {
			cor_mtx[j][i] = cor_mtx[i][j];
		}
	}
	f.open(res, ios::app);
	if(!f) {
		cout<<"Unable to open file\n";
		cin.get();
		exit(1);
	}
	for(int i = 0;i < NUM_IND;i ++) {
		for(int j = 0;j < NUM_IND;j ++) {
			f<<fixed<<setprecision(3)<<cor_mtx[i][j];
			if(j < NUM_IND-1)
				f<<'\t';
		}
		f<<'\n';
	}
	f<<'\n';
	f.close();
	return 0;
}