#include <bm.h>

#define pb push_back

bm25::bm25(vector<string> v){
	corpusSize = v.size();
	avgdl = 0;
}


string bm25::cleaner(string& txt){
	transform(txt.begin(), txt.end(), txt.begin(), ::tolower);
	// std::replace_if(txt.begin(), txt.end(),
 //    [](char c) { return !(std::isspace(c) || std::isalpha(c) || c=='<' || c=='>' || c=='/'); } ,
 //    ' ');

	// std::replace_if(txt.begin(), txt.end(),
 //    [](char c) { return (c==','||c=='-'||c=='.' || c=='(' || c==')'); } ,
 //    ' ');
    	// std::replace_if(txt.begin(), txt.end(),
    // [](char c) { return ( c==','); } ,
    // '');

	// cerr << txt <<endl;

	return txt;
}

void bm25::setStop(string path){
	string FILENAME,ln;
	FILENAME = path;
	ifstream file(FILENAME);
	if (file.is_open()) {
		while (getline(file, ln,'\n')) {
			// cout <<ln.size()<<" \n";
			stopWords.insert(ln.substr(0,ln.size()-1));
		}
	}
	// cout << stopWords.size() <<endl;
}

bm25::bm25(){
	totaleDocLength = 0;
	avgdl = 0;
	corpusSize = 0;

	// hyper params 
	k  = 1.5;
	b = 0.75;
	eps = 1;
}

void bm25::setAvgdl(){
	avgdl = (float)totaleDocLength/corpusSize;
}
void bm25::setCorpusSize(int crSize){
	corpusSize = crSize;
}

int bm25::getCorpusSize(){
	return corpusSize;
}

void bm25::addDoc(string& docNo,string& text,int cnt,bool clean=true){

	if(clean){
		text = cleaner(text);
	}

	// add to doc length
	docLen.pb(text.length());
	docName.pb(docNo);
	totaleDocLength += (long)text.length();
	unordered_map<string,int> wordCount;


	// code ref : https://www.tutorialspoint.com/The-most-elegant-way-to-iterate-the-words-of-a-C-Cplusplus-string
	stringstream str_strm(text);
    string tmp;
    char delim = ' '; // Ddefine the delimiter to split by


	while (std::getline(str_strm, tmp, delim)) {
		if(stopWords.find(tmp) != stopWords.end()){
			continue;
		}

		tmp = stemmer(tmp);
		if(wordCount.find(tmp) != wordCount.end() ){
			wordCount[tmp] += 1;
			continue;
		}
		wordCount[tmp] = 1;
    }

    docFreq.pb(wordCount);

    for(auto x: wordCount){

    	wordDocNames[x.first].insert(cnt);
    	if(wordDoc.find(x.first) != wordDoc.end() ){
    		wordDoc[x.first] += 1;
    		continue;
    	}
    	wordDoc[x.first] = 1;
    }

    // increment corpus Size 
    corpusSize++;

}

void bm25::calcIdf(){
	float idfSum = 0,idfCal;
	string word;
	int count;
	int len=0;
	vector<string> negativeIdf;
	for(auto x: wordDoc){
		len++;
		word = x.first;
		count = x.second;

		idfCal = log(corpusSize - count + 0.5) - log(count + 0.5);
		idf[word]  = idfCal;
		idfSum += idfCal;
		if(idfCal < 0){
			negativeIdf.pb(word);
		}
	}
	float avgIdf = idfSum/(float)len;
	float adjust  = eps*avgIdf;
	// for negative idf

	for(auto x:negativeIdf){
		idf[x] = adjust;
	}

}

void bm25::calcIdf2(){
	
	for(auto x:wordDoc){
		idf[x.first] = log(corpusSize + 1) - log(x.second+0.5);
	}
}

void bm25::calcIdf3(){
	for(auto x:wordDoc){
		idf[x.first] = log(corpusSize + 1) - log(x.second);
	}
}

string bm25::stemmer(string tmp){
	// return tmp;
    wchar_t* UnicodeTextBuffer = new wchar_t[tmp.length()+1];
    std::wmemset(UnicodeTextBuffer, 0, tmp.length()+1);
    std::mbstowcs(UnicodeTextBuffer, tmp.c_str(), tmp.length());
    word = UnicodeTextBuffer;
	StemEnglish(word);
	// cout <<" word :: "<<tmp<< "  :: ";
	string tmp1(word.begin(),word.end());
	return tmp1;
}

// vector<pair<float,string> >
void bm25::getScore(string& query,int num){
	vector<pair<float,string> > v;
	int ar[corpusSize];
	for(int i=0;i<corpusSize;i++){
		v.pb({0.0,docName[i]});
		ar[i] = 0;
	}
	query = cleaner(query);
	stringstream str_strm(query);
    string tmp;
    float tmpF;
    int cnt = 0;
    char delim = ' '; // Ddefine the delimiter to split by
	while (std::getline(str_strm, tmp, delim)) {
		if(stopWords.find(tmp) != stopWords.end())
			continue;
		if(tmp == "<person>")
			tmp = "person";
		else if(tmp == "<organization>")
			tmp = "company";
		tmp = stemmer(tmp);
		cnt=0;
		for(auto y:wordDocNames[tmp]){
			cnt = y;
			// auto x = docFreq[cnt];
			ar[cnt] = docFreq[cnt][tmp];

		}
		for(int i:wordDocNames[tmp]){
			tmpF = 0;
			if(idf.find(tmp) != idf.end())
				tmpF = idf[tmp];
			// cout << tmpF <<" :-: "<< (ar[i]*(k+1)) <<" "<<(((1-b + (b*(docLen[i]/avgdl))) )) <<endl;
			v[i].first += tmpF * ((ar[i]*(k+1))/( ar[i] + k*(1-b + (b*(docLen[i]/avgdl))) ));
		}

    }
	sort(v.rbegin(),v.rend());
	for(int i=0;i<50;i++)
		cout <<num <<" 0 " <<v[i].second<<"  "<<i+1<<" "<<v[i].first << " o "<<endl;
	// cout <<endl<<"====================="<<endl<<endl;
    return ;
}