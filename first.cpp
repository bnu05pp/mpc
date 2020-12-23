
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <stdlib.h>
#include <vector>
#include <sstream>

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
    long count = 0, cur_id = 0;
    string str; 
	string RDF_data = argv[1]; 
    string entity_file_name = RDF_data + "_entity.txt";
	string partition_file_name = RDF_data + "_intInternalPoints.txt";
	string out_file_name = RDF_data + "InternalPoints.txt";
	string property_file_name = RDF_data + "_property.txt";
        string crossing_file_name = RDF_data + "_intcrossingEdges.txt";
        string out_p_file_name = RDF_data + "crossingEdges.txt";
	string sign=(string(argv[2])=="1")?" ":"\t";
	
    map<int, string> IDEntityMap;
	count = 0;
    ifstream fin12(entity_file_name.c_str()); 
    while( getline(fin12, str) )
    {   
        vector<string> TermArr = split(str, sign);

	cur_id = atoi(TermArr[1].c_str());
	IDEntityMap.insert(make_pair(cur_id, TermArr[0]));

	if(count++ % 100000 == 3){
            cout << count << "---" << endl;
        }
    }

	map<int, string> IDPropertyMap;
        count = 0;
    ifstream fin1(property_file_name.c_str());
    while( getline(fin1, str) )
    {
        vector<string> TermArr = split(str, sign);

        cur_id = atoi(TermArr[1].c_str());
        IDPropertyMap.insert(make_pair(cur_id, TermArr[0]));

        if(count++ % 100000 == 3){
            cout << count << "###" << endl;
        }
    }

	count = 0;
	ifstream fin234(crossing_file_name.c_str()); 
	ofstream outFile134(out_p_file_name.c_str());
	while( getline(fin234, str) )
	{
		vector<string> TermArr = split(str, sign);

		cur_id = atoi(TermArr[0].c_str());
		outFile134 << IDPropertyMap[cur_id] << "\t" << TermArr[1] << "\t" << TermArr[2] << endl;

		if(count++ % 100000 == 3){
            		cout << count << "..." << TermArr.size() << endl;
        	}
	}

	outFile134.close();

	count = 0;
	ifstream fin(partition_file_name.c_str());
        ofstream outFile1(out_file_name.c_str());
    while( getline(fin, str) )
    {
        vector<string> TermArr = split(str, sign);

                cur_id = atoi(TermArr[0].c_str());
                outFile1 << IDEntityMap[cur_id] << "\t" << TermArr[1] << endl;

                if(count++ % 100000 == 3){
            cout << count << "+++" << endl;
        }
    }

        outFile1.close();
	
	cout << "count: " << count << endl;
	return 0;
}

