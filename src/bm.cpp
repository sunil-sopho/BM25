#include <bm.h>

#define pb push_back

bm25::bm25(vector<string> v){
	corpusSize = v.size();
	avgdl = 0;
}


string bm25::cleaner(string txt){
	return txt;
}

bm25::bm25(){
	totaleDocLength = 0;
	avgdl = 0;
	corpusSize = 0;
}

void bm25::setCorpusSize(int crSize){
	corpusSize = crSize;
}

int bm25::getCorpusSize(){
	return corpusSize;
}

void bm25::addDoc(string& docNo,string& text,bool clean=true){

	if(clean){
		text = cleaner(text);
	}

	// add to doc length
	docLen.pb(text.length());
	totaleDocLength += (long)text.length();
	unordered_map<string,int> wordCount;


	// code ref : https://www.tutorialspoint.com/The-most-elegant-way-to-iterate-the-words-of-a-C-Cplusplus-string
	stringstream str_strm(text);
    string tmp;
    char delim = ' '; // Ddefine the delimiter to split by
	while (std::getline(str_strm, tmp, delim)) {
		if(wordCount.find(tmp) != wordCount.end() ){
			wordCount[tmp] += 1;
			continue;
		}
		wordCount[tmp] = 1;
    }

    docFreq.pb(wordCount);

    for(auto x: wordCount){
    	if(wordDoc.find(x.first) != wordDoc.end() ){
    		wordDoc[x.first] += 1;
    		continue;
    	}
    	wordDoc[x.first] = 1;
    }

    // increment corpus Size 
    corpusSize++;

}