#include"graph.cpp"
#include<ctime>
clock_t st,ed;
double endtime;
int
main(int argc, char* argv[])
{	
	string txt_name=argv[1];
	string name=argv[2];
	string sign=(string(argv[3])=="1")?" ":"\t";
	int part=atoi(argv[4]);
	
	st=clock();
		graph* test=new graph();
		test->init();
		test->RDF=name;
		test->part=part;
		
		test->loadGraph(txt_name,sign);
		int predNum = test->getPreNum();
		if(predNum < 20)
			// if the number of properties is smaller than 20, we find the optimal partitioning results.
			test->unionEdgeForEnum();
		else if(predNum >= 20 && predNum < 100)
			test->unionEdgeForGreed();
		else
			test->greed2();

	ed=clock();
	endtime=(double)(ed-st)/CLOCKS_PER_SEC;
	cout<<"partition : "<<endtime<<" s"<<endl;

	delete test;

	return 0;
}
