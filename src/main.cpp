#include<bits/stdc++.h>
#include <experimental/filesystem>

#include <bm.h>

using namespace std;
namespace fs = std::experimental::filesystem::v1;

int main(){
	std::string path = "./data/part_1/";
	int transCount = 0;

    bool txt = false;
    bool doc = false;
    string text;
    string docno;

    bm25 algo;

    for (const auto & entry : fs::directory_iterator(path)){
        // std::cout << entry.path() << std::endl;
        // read file here ::
   		string FILENAME = entry.path();
    	ifstream file(FILENAME);
		if (file.is_open()) {
		    string line;
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
				    algo.addDoc(docno,text);
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
		    // cout << transCount <<" : "<< algo.getCorpusSize() <<endl;
		    file.close();
		}
		break;
    }
    // assert(algo.getCorpusSize() ==transCount);
    
	cout << "totale Docs :: "<<transCount<<endl;

	return 0;
}