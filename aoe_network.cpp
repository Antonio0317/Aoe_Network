#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <sstream>

using namespace std;

class Aoe_Network {
	public:

		class Vertex;
		class Edge {
			public:
				int weight;
				Edge* next;
				Vertex* from;
				Vertex* target;

				Edge(int);
				~Edge();
		};

		class Vertex {
			public:
				int data;
				int visit;
				Vertex* next;
				Edge* out_list;
				Edge* in_list;

				Vertex(int);
				~Vertex();
		};

		Aoe_Network(int); //Create Graph
		~Aoe_Network();
		void DestroyVertex(Vertex*);
		void DestroyEdge(Edge*);
		void AddVertex(Vertex*);
		void AddEdge(int, int, Edge*);
		void R_AddEdge(int, int, Edge*);
		void PrintGraph();
		void DFS(Vertex*, int);
		void R_DFS(Vertex*, int);
		void Error_check();

	private:

		Vertex* vertices;
		int vertex_cnt;
		bool cycle;
		int* out_adjacency;
		int* in_adjacency;
		int* ee;
		int* le;
		int critical;
};

class StrTok {
	public:
		string str;
		string* tmp;
		void StringSplit(string);
		StrTok();
		~StrTok();
};

Aoe_Network::Aoe_Network(int len) {
	vertices = NULL;
	vertex_cnt = 0;
	cycle = false;
	out_adjacency = new int[len];
	in_adjacency = new int[len];
	ee = new int[len];
	le = new int[len];
	for (int i = 0; i< len; i++) {
		out_adjacency[i] = 0;
		in_adjacency[i] = 0;
		ee[i] = 0;
	}
}

Aoe_Network::~Aoe_Network() {
	while (vertices != NULL) {
		Vertex* nextNode = vertices->next;
		DestroyVertex(vertices);
		vertices = nextNode;
	}

	delete vertices;
}

Aoe_Network::Vertex::Vertex(int d) {
	data = d;
	next = NULL;
	out_list = NULL;
	in_list = NULL;
	visit = 0;
}

Aoe_Network::Vertex::~Vertex() {
}

void Aoe_Network::DestroyVertex(Vertex* v) {
	while (v->out_list != NULL) {
		Edge* nextnode = v->out_list->next;
		DestroyEdge(v->out_list);
		v->out_list = nextnode;
	}
	while (v->in_list != NULL) {
		Edge* nextnode = v->in_list->next;
		DestroyEdge(v->in_list);
		v->in_list = nextnode;
	}
	delete v;
}

Aoe_Network::Edge::Edge(int w) {
	from = NULL;
	target = NULL;
	weight = w;
	next = NULL;
}

Aoe_Network::Edge::~Edge() {
}

void Aoe_Network::DestroyEdge(Edge* e) {
	delete e;
}

void Aoe_Network::AddVertex(Vertex* v) {
	Vertex* current = vertices;
	if (current == NULL) {
		vertices = v;
	}
	else {
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = v;
	}
	vertex_cnt++;
}

void Aoe_Network::AddEdge(int f, int t, Edge* e) {
	out_adjacency[t]++;
	Vertex* v1 = vertices;
	Vertex* v2 = vertices;
	Edge* current;
	while (v1->data != f) {
		v1 = v1->next;
	}
	while (v2->data != t) {
		v2 = v2->next;
	}
	e->from = v1;
	e->target = v2;
	if (v1->out_list == NULL) {
		v1->out_list = e;
	}
	else {
		current = v1->out_list;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = e;
	}
}

void Aoe_Network::R_AddEdge(int f, int t, Edge* e) {
	in_adjacency[t]++;
	Vertex* v1 = vertices;
	Vertex* v2 = vertices;
	Edge* current;
	while (v1->data != f) {
		v1 = v1->next;
	}
	while (v2->data != t) {
		v2 = v2->next;
	}
	e->from = v1;
	e->target = v2;
	if (v1->in_list == NULL) {
		v1->in_list = e;
	}
	else {
		current = v1->in_list;
		while (current->next != NULL) {
			current = current->next;
		}
		current->next = e;
	}
}

void Aoe_Network::PrintGraph() {
	Vertex* v = NULL;
	Edge* e = NULL;
	int cnt = 0;
	for( int i = 0; i< vertex_cnt; i++) {
		if(out_adjacency[i] == 0) {
			cnt++;
		}
	}
	if(cnt != 1) {
		cout << "AOE Network Error" <<endl;
		return;
	}
	//start가 한개인지 확인!
	v=vertices;
	while(v->in_list != NULL) {
		v = v->next;
	}	//start포인트를 찾음
	DFS(v, 0);
	//위상정렬수행.
	cnt = 0;
	critical = ee[vertex_cnt-1];
	for(int i = 0; i<vertex_cnt; i++) {
		le[i] = critical;
	}	//가장 큰 값으로 초기화
	v = vertices;
	while( v->out_list != NULL ) {
		v = v->next;
	}

	if(cycle) {
		cout << "AOE Network Error" << endl;
		cycle = false;
		return;
	}	//사이클이 있으면 에러 출력
	R_DFS(v, critical);

	if ((v = vertices) == NULL)
		return;

	while (v != NULL) {
		if ((e = v->out_list) == NULL) {
			v = v->next;
			cout << endl;
			continue;
		}

		while (e != NULL) {
			if(cnt++) {
				cout << endl;
			}
			cout << v->data << ",";
			cout << e->target->data;
			cout << " " << ee[e->from->data] << " ";
			cout << le[e->target->data]-e->weight << " ";
			cout << le[e->target->data]-e->weight-ee[e->from->data] << " ";
			if(le[e->target->data]-e->weight-ee[e->from->data] == 0) {
				cout << "Yes";
			}
			else {
				cout << "No";
			}
			e = e->next;
		}
		v = v->next;
	}
}

void Aoe_Network::DFS (Vertex *v, int w) {
	if(ee[v->data] < w || ee[v->data] == 0) 
		ee[v->data] = w;
	else
		return;
	v->visit = 1;
	for(Edge* e = v->out_list; e != NULL; e = e->next) {
		in_adjacency[e->target->data]--;
		if(e->target->visit == 0) {
			DFS(e->target, e->weight+ee[e->from->data]);
		}
		else {
			cycle = true;
		}
	}
	v->visit = 0;
}

void Aoe_Network::R_DFS (Vertex *v, int w) {
	if(le[v->data] > w || le[v->data] == critical) 
		le[v->data] = w;
	else
		return;

	v->visit = 1;
	for(Edge* e = v->in_list; e != NULL; e = e->next) {
		in_adjacency[e->target->data]--;
		if(e->target->visit == 0) {
			R_DFS(e->target, le[e->from->data]-e->weight);
		}
	}
	v->visit = 0;
}

void StrTok::StringSplit(string strTok) {
	int cutAt;
	int index = 0;
	string* strResult = new string[1002];
	string strOrigin = str;

	while ((cutAt = strOrigin.find_first_of(strTok)) != strOrigin.npos) {
		if (cutAt>0) {
			strResult[index++] = strOrigin.substr(0, cutAt);
		}
		strOrigin = strOrigin.substr(cutAt + 1);
	}

	if (strOrigin.length() > 0) {
		strResult[index++] = strOrigin.substr(0, cutAt);
	}
	tmp = strResult;
}

StrTok::StrTok() {
	str = "";
	tmp = NULL;
}

StrTok::~StrTok() {
	delete[] tmp;
}

void Aoe_Network::Error_check() {
	int cnt = 0;
	if(cycle) {
		cout << "AOE Network Error\n" << endl;
		cycle = false;
	}
	for( int i = 0; i< vertex_cnt; i++) {
		if(out_adjacency[i] == 0) {
			cnt++;
		}
	}
	if(cnt > 1) {
		cout << "AOE Network Error\n" <<endl;
	}
}

int main() {
	int len = 0;
	int vertex;
	string s, input;
	StrTok buffer1, buffer2;

	while(getline(cin, s)) {
		input+=s;
		input+="\n";
		len++;
	}
	Aoe_Network test(len);	//인접리스트 형성
	input = input.substr(0, input.length()-1);
	istringstream iss1(input);
	while(getline(iss1, buffer1.str)) {
		vertex = 0;
		buffer1.StringSplit(" ");
		for(int i = 0; i< buffer1.tmp[0].length(); i++) {
			vertex *= 10;
			vertex += buffer1.tmp[0][i] - '0';
		}
		test.AddVertex(new Aoe_Network::Vertex(vertex)); //정점추가
	}
	istringstream iss2(input);
	while(getline(iss2, buffer2.str)) {
		vertex = 0;
		buffer2.StringSplit(" ");
		for(int i = 0; buffer2.tmp[i].length(); i++) {
			if (i == 0) {
				for(int j = 0; j< buffer2.tmp[0].length(); j++) {
					if(buffer2.tmp[0][j] >= '0' && buffer2.tmp[0][j] <= '9') {
						vertex *= 10;
						vertex += buffer2.tmp[0][j] - '0';
					}
				}
			}
			else {
				istringstream tmp_t(buffer2.tmp[i]);
				string token = "";
				int e[2] = {0,0};
				int index = 0;
				while(getline(tmp_t, token, ',')) {
					for(int j = 0; j<token.length(); j++) {
						if(token[j] >= '0' && token[j] <= '9') {
							e[index] *= 10;
							e[index] += token[j] -'0';
						}
					}
					index++;
				}
				test.AddEdge(vertex, e[0], new Aoe_Network::Edge(e[1])); //간선추가
				test.R_AddEdge(e[0], vertex, new Aoe_Network::Edge(e[1]));
			}
		}
	}
	test.PrintGraph();
	test.~Aoe_Network();
}

