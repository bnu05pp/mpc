
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <ctime>

using namespace std;

vector<string> 
split(string textline, string tag){
	vector<string> res;
	std::size_t pre_pos = 0;
	std::size_t pos = textline.find(tag);
	while (pos != std::string::npos){
		string curStr = textline.substr(pre_pos, pos - pre_pos);
		curStr.erase(0, curStr.find_first_not_of("\r\t\n "));
		curStr.erase(curStr.find_last_not_of("\r\t\n ") + 1);
		if(curStr.compare("") != 0)
			res.push_back(curStr);
		pre_pos = pos + tag.size();
		pos = textline.find(tag, pre_pos);
	}

	string curStr = textline.substr(pre_pos, pos - pre_pos);
	curStr.erase(0, curStr.find_first_not_of("\r\t\n "));
	curStr.erase(curStr.find_last_not_of("\r\t\n ") + 1);
	if(curStr.compare("") != 0)
		res.push_back(curStr);

	return res;
}

int
main(int argc, char * argv[])
{
	clock_t st,ed;
	double endtime;
	st = clock();
	
    long count = 0, crossing_count = 0, crossing_p_count = 0;
    string str;  
    string file_name = (argv[1]);
	string data_name = argv[2];
	string sign=(string(argv[3])=="1")?" ":"\t";

	string out_triple_int_path = data_name + "_int_triples.nt";
	ofstream outFile(out_triple_int_path.c_str());
    map<string, int> eMap;
	map<string, int> pMap;
	count = 0;
	int p_count = 0, e_count = 1;
    ifstream fin(file_name.c_str()); 
    while( getline(fin, str) )
    {   
	string textline = str;
	str.resize(str.length()-2); 
//	cout << str << endl;
        vector<string> TermArr = split(str, sign);
       
	//eSet.insert(TermArr[0]);
	if(eMap.count(TermArr[0]) == 0){
                eMap.insert(make_pair(TermArr[0], e_count));
                e_count++;
        }

	if(pMap.count(TermArr[1]) == 0){
                pMap.insert(make_pair(TermArr[1], p_count));
                p_count++;
        }

	if (TermArr[2].at(0) == ('<') || TermArr[2].at(0) == ('_')) {
		//eSet.insert(TermArr[2]);
		if(eMap.count(TermArr[2]) == 0){
        	        eMap.insert(make_pair(TermArr[2], e_count));
                	e_count++;
        	}
		outFile << eMap[TermArr[0]] << "\t" << pMap[TermArr[1]] << "\t" << eMap[TermArr[2]] << " ." << endl;
	}
/*
	if(pMap.count(TermArr[1]) == 0){
		pMap.insert(make_pair(TermArr[1], p_count));
		p_count++;
	}
*/	//pMap[TermArr[1]]++; 
//	outFile << eMap[TermArr[0]] << "\t" << pMap[TermArr[1]] << "\t" << eMap[TermArr[2]] << " ." << endl;

	if(count++ % 100000 == 3){
            cout << count << "---" << endl;
        }
    }
	ed = clock();
	cout << "mapping entities to ids : " << (double)(ed - st)/CLOCKS_PER_SEC << "s." << endl;

	string p_path_str = data_name + "_property.txt";
	ofstream outFile2(p_path_str.c_str());
	for(map<string, int>::iterator iter1 = pMap.begin(); iter1 != pMap.end(); iter1++){
		outFile2 << iter1->first << "\t" << iter1->second << endl;
	}
	outFile2.close();
	outFile.close();
	
	string path_str = data_name + "_entity.txt";
        ofstream outFile1(path_str.c_str());
        for(map<string, int>::iterator iter = eMap.begin(); iter != eMap.end(); iter++){
                outFile1 << iter->first << "\t" << iter->second << endl;
        }
        outFile1.close();

	string meta_path_str = data_name + "_meta.txt";
	ofstream outFile3(meta_path_str.c_str());
        outFile3 << count << endl;
	outFile3 << eMap.size() << endl;
	outFile3 << pMap.size() << endl;
        outFile3.close();

	cout << "edges' number: " << count << endl;
	cout << "entites' number: " << eMap.size() << endl;
	cout << "properties' number: " << pMap.size() << endl;
	return 0;
}
