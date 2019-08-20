#ifndef BM_H
#define BM_H
#include <bits/stdc++.h>

using namespace std;

class bm25{
	private:
		int corpusSize;
		unordered_map<string,int> wordDoc;
		float avgdl;
		vector<unordered_map<string,int> > docFreq;
		unordered_map<string,float> idf;
		vector<int> docLen;

		long totaleDocLength;

	public:
		bm25(vector<string> v);
		bm25();

		// int itemName;
		// int count;
		string cleaner(string txt);
		void calcIdf();
		void setCorpusSize(int crSize);
		int getCorpusSize();
		void addDoc(string& docNo,string& text,bool clean=true);
		// void setter();
		// node(int itemName,int count);
		// node(int itemName);
		// shared_ptr<node> nodeLink;
		// shared_ptr<node> parent;
		// vector<shared_ptr<node> > child;
};


#endif