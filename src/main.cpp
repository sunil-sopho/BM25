#include<bits/stdc++.h>
#include <experimental/filesystem>

#include <bm.h>
#include <omp.h>

using namespace std;
namespace fs = std::experimental::filesystem::v1;

int main(){
	std::string path = "./data/part_1/";
	int transCount = 0;

    bool txt = false;
    bool doc = false;
    
    string docno,text,line,FILENAME;

    bm25 algo;

    algo.setStop("./extra/stopwords_en.txt");

    #pragma parallel omp for num_threads(3)
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
				if(txt){
					text = line;
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
				    algo.addDoc(docno,text,transCount-1);
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

	FILENAME = "./data/topics.51-100";
	// FILENAME = "./data/queryL";

	ifstream file(FILENAME);
	vector<pair<float,string> > vec;
	int num =51 ;

	// mode 0 -> out, 1-> in top, 2-> top, 3-> des ,4-> sum, 5-> nar
	int queryB = 0;
	string topic,des,nar,cons;
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
				continue;
			}
			if(line == "</top>"){
				// cout << num;
				// if(num==66)
				// cerr << nar << endl;
				nar += cons;
				algo.getScore(nar,num);
				num++;
			}
			if(line.substr(0,4)=="<tit"){
				topic = line.substr(15,line.size()-16);
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
			if(queryB == 3){
				des += line+" ";
			 // break;
			}
			if(queryB == 5){
				nar += line+" ";
			}
			if( queryB == 6){
				cons += line+" ";
			}

			// algo.getScore(line,num);
			// num++;
			// sort(vec.begin(),vec.end());
			// for(int i=0;i<5;i++)
				// cout << vec[i].second<<endl;
			// cout <<endl<<"====================="<<endl<<endl;
		}
	}

	return 0;
}