#include <bm.h>
// #include <boost/algorithm/string/replace.hpp>
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
	std::replace_if(txt.begin(), txt.end(),
    [](char c) { return (c=='\''||c=='-'); } ,
    ' ');

	txt.erase(std::remove_if(txt.begin(), txt.end(),
    [](char c) { return (isdigit(c) ||ispunct(c) )&&(c!='<'&&c!='>');}), // c=="." || c=='(' || c==')'||c==','); } ),
    txt.end());

	// ::isdigit(ch) || ::ispunct(ch)

    	// std::replace_if(txt.begin(), txt.end(),
    // [](char c) { return ( c==','); } ,
    // '');

	// cerr << txt <<endl;

	return txt;
}
// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

void bm25::setStop(string path){
	string ln,FILENAME,ln1;
	// char * FILENAME = new char [path.length()+1];
	// strcpy (FILENAME, path.c_str());
	FILENAME = path;
	ifstream file(FILENAME);
	// freopen(FILENAME,'r',stdin);
	if (file.is_open()) {
		while (getline(file, ln)) {
			// cout <<ln.size()<<" \n";
			// ln1;
			trim(ln);
			// cerr << ln <<" : "<<ln.size()<<endl;
			// cerr << ln.substr(0,ln.size()-1)<<endl;
			stopWords.insert(ln);
		}
	}
		file.close();
	// cout << stopWords.size() <<endl;
}

bm25::bm25(){
	totaleDocLength = 0;
	avgdl = 0;
	corpusSize = 0;
	vocabSize=0;
	// hyper params 
	k  = 1.5;
	b = 0.69;
	eps = 3.1;
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
	// cerr << "here -"<<endl;

	if(clean){
		text = cleaner(text);
	}
	// add to doc length
	docLen.pb(text.length());
	docName.pb(docNo);
	docIdMapper[docNo] = corpusSize;
	totaleDocLength += (long)text.length();
	// cerr << "last 1\n";

	unordered_map<int,int> *wordCount = new unordered_map<int, int>();;

	// code ref : https://www.tutorialspoint.com/The-most-elegant-way-to-iterate-the-words-of-a-C-Cplusplus-string
	stringstream str_strm(text);
    string tmp;
    char delim = ' '; // Ddefine the delimiter to split by

    int index=0;
		// cerr << "here "<<endl;

	while (std::getline(str_strm, tmp, delim)) {
		if(stopWords.find(tmp) != stopWords.end()){
			continue;
		}
		// if(tmp == "<person>")
		// 	tmp = "person";
		// else if(tmp == "<organization>"){
		// 	if(wordCount->find(tmp) != wordCount->end() ){
		// 		wordCount->at("organization") += 1;
		// 	}
		// 	else
		// 		wordCount->insert({"organization",1});
		// 	tmp = "company";
		// }
		// else if(tmp=="<location>")
		// 	tmp = "location";
		// cerr << "here 1"<<endl;
		tmp = stemmer(tmp);
		if(vocab.find(tmp) != vocab.end()){
			index = vocab[tmp];
		}else{
			vocab[tmp] = vocabSize;
			index = vocabSize;
			vocabSize++;
			// cerr << vocabSize<<"  "<<tmp <<endl;
			vocabVector.push_back(tmp);
		}
		// cerr << (wordCount->find(tmp) ==  wordCount->end() ) <<endl;
		// cerr << "last 1 "<<tmp<< "\n";

		if(wordCount->find(index) != wordCount->end() ){
			wordCount->at(index) += 1;
			continue;
		}
		wordCount->insert({index,1});
    }
		// cerr << "here 1 "<< cnt<<endl;

    // cerr << "cnt : "<<cnt <<"  "<<wordCount->size()<<endl;
    docFreq.pb((shared_ptr<unordered_map<int,int> >)wordCount);
    // docFreq.pb(wordCount);

	// return;

	// cerr << "last\n";
    for(auto x: *wordCount){
    	// tmp = vocabVector[x.first];
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

void bm25::printReport(){
	cerr <<" vocab -- "<< wordDoc.size()<<endl;
}
void bm25::loadData(string path){
	ifstream file(path);
	if(file.is_open()){
		file >> vocabSize ;
		string s;
		int a,b,d;
		float c;
		for(int i=0;i<vocabSize;i++){
			file >> s >> a >> c;
			vocab[s] = a;
			idf[a] = c;
		}
		cerr << "loading vocab and idf completed "<<endl;
		for(int i=0;i<vocabSize;i++){
			file >> a >> b;
			for(int j=0;j<b;j++){
				file >> d;
				// cerr <<"inserting in "<<a <<" :: "<<d<<endl;
				wordDocNames[a].insert(d);
			}
		}
		cerr << "loading wordDocNames completed "<<endl;

		file >> corpusSize;
		for(int i=0;i<corpusSize;i++){
			file >>s >> a;
			docName.push_back(s);
			docLen.push_back(a);
		}
		cerr << "loading docName completed "<<endl;

		file >> avgdl;
		for(int i=0;i<corpusSize;i++){
			unordered_map<int,int> *wordCount = new unordered_map<int, int>();;
			file >> d;
			for(int j=0;j<d;j++){
				file >> a >> b;
				wordCount->insert({a,b});
			}
			docFreq.pb((shared_ptr<unordered_map<int,int> >)wordCount);
		}
		cerr << "loading docFreq completed "<<endl;

		file.close();
	}

}
void bm25::saveData(string path){
	// all data need to be saved
	// corpusSize- :: vocabVector- :: vocabSize- ::vocab- :: wordDoc :: avgdl
	// wordDocNames :: docFreq
	// docLen- :: docName-

	// lets save vocab first
	string FILENAME;
	FILENAME = path;
	ofstream file(FILENAME);
	// freopen(FILENAME,'r',stdin);
	if (file.is_open()) {
		file << vocabSize << endl;
		for(auto x: vocab){
			file << x.first <<" "<<x.second<< " " << idf[x.second]<<endl;
		}

		for(auto x:wordDocNames){
			file << x.first<<" "<<x.second.size()<<" ";
			for(auto y : x.second){
				file << y<<" ";
			}
			file << endl;
		}
		file << corpusSize << endl;
		for(int i=0;i<corpusSize;i++)
			file << docName[i] <<" "<<docLen[i]<<endl;
		file << avgdl<< endl;
		for(int i=0;i<corpusSize;i++){
			file << docFreq[i]->size()<<endl;
			for(auto x : *docFreq[i] ){
				file<< x.first<<" "<<x.second<<endl;
			}
		}



	}
	file.close();
}
void bm25::calcIdf(){
	float idfSum = 0,idfCal;
	int word;
	int count;
	int len=0;
	vector<int> negativeIdf;
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

string bm25::stemmer(string& tmp){
	// return tmp;
	if (st.find(tmp) != st.end())
		return st[tmp];

    wchar_t* UnicodeTextBuffer = new wchar_t[tmp.length()+1];
    std::wmemset(UnicodeTextBuffer, 0, tmp.length()+1);
    std::mbstowcs(UnicodeTextBuffer, tmp.c_str(), tmp.length());
    word = UnicodeTextBuffer;
	StemEnglish(word);
	// cout <<" word :: "<<tmp<< "  :: ";
	string tmp1(word.begin(),word.end());
	return tmp1;
}

// bool replacer(string& tmp){

// }

void bm25::updateStemmer(){
	st["authorities"] = "authoriti";
	st["authority"] = "authoriti";

}
string sr1 = "AP891202-0059";
string sr2 = "AP880511-0145";
string sr3 = "AP891012-0020";
string sr4 = "AP891209-0074";
void bm25::updateScore(string& query,vector<pair<float,string> >& v,float reducer,float factor,int *ar,int* br){
	// cerr << query <<endl;
	stringstream str_strm(query);

    string tmp1;
    int tmp;
    float tmpF;
    // int cnt = 0;
    char delim = ' '; // Ddefine the delimiter to split by
    bool negator=false;

	// stringstream str_strm2(query);
 //    unordered_map<string,int> mm;
	// while (std::getline(str_strm2, tmp1, delim)) {
	// 	tmp1 = stemmer(tmp1);
	// 	if(mm.find(tmp1) == mm.end())
	// 		mm[tmp1]=1;
	// 	else
	// 		mm[tmp1] +=1;
 //    }

	while (std::getline(str_strm, tmp1, delim)) {

		trim(tmp1);
		if(tmp1=="")
			continue;
		if(stopWords.find(tmp1) != stopWords.end()){
			if(tmp1 == "not"){
				negator = true;
				// cerr << "negator set \n";
			}
			// if(tmp1 == "fail")
				// cerr << "fail detected"<<endl;
			continue;
		}

		tmp1 = stemmer(tmp1);
		// if(mm[tmp1] == 0 && (!negator))
		// 	continue;


		if(vocab.find(tmp1) == vocab.end())
			continue;

		tmp = vocab[tmp1];
		// cerr << tmp1<<"  ::  "<<tmp<<" : "<<wordDocNames[tmp].size() <<endl;
		for(auto i:wordDocNames[tmp]){

			ar[i] = docFreq[i]->at(tmp);
			br[i] += 1;

			tmpF = 0;
			if(idf.find(tmp) != idf.end())
				tmpF = idf[tmp];
			if(negator){
				// if(docName[i] == sr1 || docName[i] ==sr2|| docName[i] == sr3||docName[i]==sr4)
				// cerr <<docName[i]<<" tmp1 :: "<<tmp1<<"  ::  "<< -1*((float)reducer)*tmpF * ((ar[i]*(k+1))/( ar[i] + k*(1-b + (b*(docLen[i]/avgdl))) ))<<endl;
				v[i].first -= ((float)reducer)*tmpF * ((ar[i]*(k+1))/( ar[i] + k*(1-b + (b*(docLen[i]/avgdl))) ));
			}
			else{
				v[i].first += factor*tmpF * ((ar[i]*(k+1))/( ar[i] + k*(1-b + (b*(docLen[i]/avgdl))) ));
				// if(docName[i] == sr1|| docName[i] ==sr2|| docName[i] == sr3||docName[i]==sr4)
				// cerr <<docName[i]<< " tmp1 :: "<<tmp1<<"  ::  "<< ((float)factor)*tmpF * ((ar[i]*(k+1))/( ar[i] + k*(1-b + (b*(docLen[i]/avgdl))) ))<<endl;
			}
		}
		// mm[tmp1] = 0;

		negator = false;
    }

    // cerr << endl <<endl;

}


string bm25::getScore(string& query,string& cons,string& def,string& topic,int num){
	vector<pair<float,string> > v;
	int ar[corpusSize];
	int br[corpusSize];
	// vector<pair<string,int> >jk;
	for(int i=0;i<corpusSize;i++){
		v.pb({0.0,docName[i]});
		// if(docName[i] == sr1|| docName[i] ==sr2|| docName[i] == sr3||docName[i]==sr4)
			// jk.push_back({docName[i],i});
		ar[i] = 0;
		br[i] = 0;
	}
	
	query = cleaner(query);
	// cerr << num << " :: "<<query<<endl;
	updateScore(query,v,1.0,1.0,ar,br);

    // cerr <<endl;
	cons = cleaner(cons);
	stringstream str_strm1(cons);
    float factor =max((float)0.799,(float)query.size()/(float)cons.size());
    factor = min(factor,(float)2.19);

    updateScore(cons,v,1.15,factor,ar,br);

    topic = cleaner(topic);
    updateScore(topic,v,0,0.32,ar,br);
 
 	// for(int i=0;i<corpusSize;i++){
 	// 	if(br[i]  != 0)
 	// 	v[i].first = (float)v[i].first/br[i];
 	// }
	// for(auto x:jk){
	// 	cerr << x.first <<" :: " << v[x.second].first <<endl;	
	// }

    // cerr <<"sort and out query "<<endl;
	sort(v.rbegin(),v.rend());

	// int selecter = 4,termNum = 0,docnum;
	// string terms = "";
	// float val;
	// vector<pair<float,int> > mapper;
	// while(selecter--){
	// 	docnum = docIdMapper[v[0].second];//<<endl;
	// 	for (auto x:*docFreq[docnum]){
	// 		val = idf[x.first]*(x.second);
	// 		mapper.push_back({val,x.first});
	// 	}
	// }

	// // cerr << num <<" : "<<query<<endl<<endl;;
	// sort(mapper.rbegin(),mapper.rend());
	// int getNum = 0;
	// for(int i=0;;i++){
	// 	// cerr << vocabVector[mapper[i].second]<<" : "<<mapper[i].first<<endl;
	// 	if(vocabVector[mapper[i].second]=="organ")
	// 		continue;
	// 	else if(vocabVector[mapper[i].second]=="person")
	// 		continue;
	// 	else if(vocabVector[mapper[i].second]=="locat")
	// 		continue;
	// 	if(getNum == termNum)
	// 		break;
	// 	terms += vocabVector[mapper[i].second]+" ";
	// 	getNum++;

	// }

	// v.clear();
	// // cerr <<" size of v :: "<<v.size()<<endl;
	// cerr <<" terms added :: "<<terms<<endl;

	// for(int i=0;i<corpusSize;i++){
	// 	v.pb({0.0,docName[i]});
	// 	ar[i] = 0;
	// }
	
	// query += " "+terms;
	// query = cleaner(query);
	// updateScore(query,v,1.0,1.0,ar);

 //    // cerr <<endl;
	// cons = cleaner(cons);
	// // stringstream str_strm1(cons);
 //    factor =max((float)0.799,(float)query.size()/(float)cons.size());
 //    factor = min(factor,(float)2.19);

 //    updateScore(cons,v,1.15,factor,ar);

 //    topic = cleaner(topic);
 //    updateScore(topic,v,0,0.32,ar);
 
	// sort(v.rbegin(),v.rend());

	// cerr << "here"<<endl;
	string outputQuery="";
	for(int i=0;i<50;i++){
		// cerr << to_string(num)<<" "<<v[i].second<<endl;
		outputQuery += to_string(num)+" 0 "+v[i].second+" "+to_string(i+1)+" "+to_string(v[i].first)+" o\n";
		// cerr << outputQuery;
		// fileOut <<num <<" 0 " <<v[i].second<<"  "<<i+1<<" "<<v[i].first << " o "<<endl;
		// if(v[i].second == "AP880906-0198")
			// break;
	}
	// cerr <<endl<<"====================="<<endl<<endl;
    return outputQuery;
}