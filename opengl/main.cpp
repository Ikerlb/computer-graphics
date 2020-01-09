#include "Window.h"
#include <vector>

using namespace std;

//TODO: Flag parsing!
int main(int argc,char** argv){
	time_t t;
	srand((unsigned) time(&t));
	string title;
	vector<string> paths;
	vector<string> shaders;
	if(argc<3){
		cout<<"expected 3 arguments: title model and shader"<<endl;
		return 1;
	}
	//for now only title, path to one model, one vshader and one fshader
	title=string(argv[1]);
	//for now 2 models, two pairs of shaders
	paths.push_back(string(argv[2]));
	shaders.push_back(string(argv[3]));
	shaders.push_back(string(argv[4]));

	Window win;
	cout<<"starting window..."<<endl;
	win.startup(800,600,title,paths,shaders);
	cout<<"running window..."<<endl;
	win.run();
	return 0;
}
