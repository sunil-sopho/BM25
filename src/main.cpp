#include<bits/stdc++.h>
#include <experimental/filesystem>

#include <bm.h>
// #include <omp.h>

using namespace std;
namespace fs = std::experimental::filesystem::v1;

int main(int argc, char *argv[]){
	int mode;
	cerr << argc <<endl;
	for(int i=0;i<argc;i++){
		cerr <<argv[i]<<" ";
	}
	cerr <<endl;
	string path,outfile,topicfile;
	if(argc == 5){
		path = argv[2];
		outfile = argv[4];
		mode = 0;
	}else{
		path = argv[2];
		outfile = argv[6];
		topicfile =argv[4] ;
		mode = 1;
	}

	int transCount = 0;
    bool txt = false;
    bool doc = false;
    

    string docno,text,line,FILENAME;
    // return;

    bm25 algo;
    cerr << "Started stopwords\n";
    algo.setStop("./extra/stopwords_en.txt");
    algo.updateStemmer();
    if(mode==0){
	    // #pragma parallel omp for num_threads(3)
	    for (const auto & entry : fs::directory_iterator(path)){
	        // std::cout << entry.path() << std::endl;
	        // read file here ::
	   		FILENAME = entry.path();
	    	ifstream file(FILENAME);
			if (file.is_open()) {
			    
			    txt = false;
			    doc = false;
			    while (getline(file, line)) {
					// split(line,frequency_by_item);
					if(line == "</TEXT>"){
						txt = false;
						continue;
					}
					if(txt){
						text += line;
						// cout << docno<<endl;
						txt = false;
						continue;
					}
					if(line == "<DOC>"){
						doc = true;
						transCount++;
						continue;
					}
					else if(line == "</DOC>"){
						doc = false;
						// cerr << docno << endl;
					    algo.addDoc(docno,text,transCount-1);
					    text = "";
						continue;
					}
					if(line == "<TEXT>"){
						txt = true;
						continue;
					}

					if(line.length() > 8 && line.substr(0,7) == "<DOCNO>"){
						docno = line.substr(8,line.length()-17);
					}
					

			    }
				// int id = omp_get_thread_num();
			 //    int data = transCount;
			 //    int total = omp_get_num_threads();
			 //    printf("Greetings from process %d out of %d with Data %d\n", id, total, data);
			    // cout << transCount <<" : "<< algo.getCorpusSize() <<endl;
			    file.close();
			}
			// break;
	    }
	    assert(algo.getCorpusSize() ==transCount);

		// cout << "totale Docs :: "<<transCount<<endl;

		algo.setAvgdl();
		algo.calcIdf();

		algo.saveData(outfile);
		return;
	}
	
	algo.loadData(path);
	FILENAME = topicfile;
	// freopen(argv[6],'w',stdout);
	// FILENAME = path;
	ofstream fileOut(outfile);

	ifstream file(FILENAME);
	vector<pair<float,string> > vec;
	int num =51 ;
	cerr << "starting queries"<<endl;

	// mode 0 -> out, 1-> in top, 2-> top, 3-> des ,4-> sum, 5-> nar
	int queryB = 0;
	string topic,des,nar,cons,fac,def;
	if (file.is_open()) {
		while (getline(file, line)) {
			// cout << line <<endl;
			if(line == "\n" || line == ""){
				continue;
			}
			if(line == "<top>"){
				queryB = 1;
				nar = "";
				des = "";
				cons = "";
				fac = "";
				def = "";
				continue;
			}
			if(line == "</top>"){
				// cout << num;
				// cerr << def << endl;
				// nar += cons;
				// nar += cons;
				// nar += cons;
				// nar += def;
				cons = topic+" "+cons;//+" "+def;
				// if(num==87)
				fileOut <<	algo.getScore(nar,cons,def,topic,num);
				num++;
			}
			if(line.substr(0,4)=="<tit"){
				topic = line.substr(15,line.size()-16);
				queryB = 2;
				continue;
			}
			if(line.substr(0,4) == "<des"){
				queryB = 3;
				continue;
			}

			if(line.substr(0,4) == "<smr"){
				queryB = 4;
				continue;
			}

			if(line.substr(0,4)=="<nar"){
				queryB = 5;
				continue;
			}
			if(line.substr(0,4)=="<con"){
				queryB = 6;
				continue;
			}
			if(line.substr(0,4)=="<fac"){
				queryB = 7;
				continue;
			}
			if(line.substr(0,4)=="<def"){
				queryB = 8;
				continue;
			}
			if(queryB==2){
				// topic +=" "+line;
			}
			if(queryB == 3){
				des += line+" ";
			 // break;
			}
			else if(queryB == 5){
				nar += line+" ";
			}
			else if( queryB == 6){
				cons += line+" ";
			}
			else if(queryB == 7){
				fac += line;
			}
			else if(queryB == 8){
				def += line;
			}

		}
	}
	fileOut.close();
	file.close();

	return 0;
}