#ifndef BM_H
#define BM_H
#include <bits/stdc++.h>

#include "stemming/english_stem.h"
using namespace std;

class bm25{
	private:
		int corpusSize;
		// string -> number of docs
		// space reductions
		unordered_map<string,int> vocab;
		vector<string> vocabVector;
		unordered_map<string,int> docIdMapper;
		int vocabSize;
		
		unordered_map<int,int> wordDoc;
		unordered_map<int,unordered_set<int> > wordDocNames;
		float avgdl;
		vector<shared_ptr<unordered_map<int,int>> > docFreq;
		// vector<unordered_map<string,int> > docFreq;

		unordered_map<int,float> idf;
		vector<int> docLen;
		vector<string> docName;
		long totaleDocLength;

		unordered_set<string> stopWords;
		// hyper params
		float k,b,eps;
		stemming::english_stem<> StemEnglish;
		wstring word;
		string stemmer(string& tmp);
		unordered_map<string,string> st;
	public:

		void loadData(string path);
		void saveData(string path);
		void updateStemmer();
		void printReport();
		bm25(vector<string> v);
		bm25();
		void setStop(string path);

		// int itemName;
		// int count;
		string cleaner(string& txt);
		void calcIdf();
		void calcIdf2();
		void calcIdf3();

		void setCorpusSize(int crSize);
		int getCorpusSize();
		void addDoc(string& docNo,string& text,int cnt,bool clean=true);

		string getScore(string& query,string& cons,string& def,string& topic,int num);
		void setAvgdl();

		void updateScore(string& query,vector<pair<float,string> >& v,float reducer,float factor,int* ar,int* br);

};


#endif