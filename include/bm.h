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
		int vocabSize;
		
		unordered_map<string,int> wordDoc;
		unordered_map<string,unordered_set<int> > wordDocNames;
		float avgdl;
		vector<shared_ptr<unordered_map<int,int>> > docFreq;

		unordered_map<string,float> idf;
		vector<int> docLen;
		vector<string> docName;
		long totaleDocLength;

		unordered_set<string> stopWords;
		// hyper params
		float k,b,eps;
		stemming::english_stem<> StemEnglish;
		wstring word;
		string stemmer(string tmp);

	public:

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

		void getScore(string& query,string& cons,int num);
		void setAvgdl();



};


#endif