#include"graphDisk.cpp"
#include<ctime>
clock_t st,ed;
double endtime;
int
main(int argc, char* argv[])
{	
	string RDF_name = argv[1];
	string txt_name= RDF_name + "_int_triples.nt";
	string name= RDF_name + "_int";
	//string sign=(string(argv[2])=="1")?" ":"\t";
	string sign = "\t";
	int part=atoi(argv[2]);
	//int eNum = 1843219;
	
	string meta_name = RDF_name + "_meta.txt";
	string line;
	ifstream meta_in(meta_name.data());
	getline(meta_in,line);
	getline(meta_in,line);
	int eNum = atoi(line.c_str());
	
	st=clock();
		graph* test=new graph();
		test->init(eNum);
		test->RDF=name;
		test->part=part;
		
		test->loadGraph(txt_name,sign);
		int predNum = test->getPreNum();
		if(predNum < 20)
			// if the number of properties is smaller than 20, we find the optimal partitioning results.
			test->unionEdgeForEnum();
		else if(predNum >= 20 && predNum < 120)
			test->unionEdgeForGreed();
		else
			test->greed2();

	ed=clock();
	endtime=(double)(ed-st)/CLOCKS_PER_SEC;
	cout<<"partition : "<<endtime<<" s"<<endl;

	delete test;

	return 0;
}
