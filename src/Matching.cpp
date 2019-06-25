//============================================================================
// Name        : Matching.cpp
// Author      : Niloufar Seyed Majidi
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "argraph.h"
#include "argloader.h"
#include "vf2_sub_state.h"
#include "match.h"
#include "allocpool.h"
#include <time.h>
#include<algorithm>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <dirent.h>

using namespace std;

#define DEBUG_MODE 0

#define TEST 1

#define MAXNODES 500

#define EdgeDiff 1

class EdgeLabel {
public:
	float score;
};
istream& operator>>(istream& in, EdgeLabel &l) {
	in >> l.score;
	return in;
}
ostream& operator>>(ostream& out, EdgeLabel &l) {
	out << l.score;
	return out;
}

class EdgeComparator: public AttrComparator {

private:
	double threshold;
public:
	EdgeComparator(double thr) {
		threshold = thr;

	}
	virtual bool compatible(void *la, void*lb) {
		//SUBGRAPH
		EdgeLabel *a = (EdgeLabel *) la;
		//GRAPH
		EdgeLabel *b = (EdgeLabel *) lb;

		if ( DEBUG_MODE == 1) {
			cout << " a: " << a->score << " ";
			cout << " b: " << b->score << endl;
			cout << endl;
		}
		//if (b->score == 6.12215)

		float diff = a->score - b->score;
		//cout << "diff: " << diff << endl;
		if (diff > threshold || diff < -threshold)
			return false;
		//if (b->score > 6.121 && b->score < 6.14)
		//cout << "yeaaaaahhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh";
		//cout << "a score: " << a->score;
		//cout << "b score: " << b->score << endl;
		return true;
	}
};

class Label {

public:
	string name;
	int id;
	/*	Label(string name, int id) {
	 this->name = name;
	 this->id = id;
	 }*/
};
istream& operator>>(istream& in, Label &l) {
	in >> l.name >> l.id;
	return in;
}
ostream& operator>>(ostream& out, Label &l) {
	out << l.name << ' ' << l.id;
	return out;
}

class LabelDestroyer: public AttrDestroyer {
public:
	virtual void destroy(void *p) {
		delete p;

	}
};

string targetName = "1bmc.grf";
bool isPositionSpecific = false;
vector<vector<string>> position;
string positive[] = { "ARG", "HIS", "LYS", "" };
string negative[] = { "ASP", "GLU", "" };
string B[] = { "SER", "THR", "ASN", "GLN", "" };
string C[] = { "CYS", "GLY", "PRO", "" };
string D[] = { "ALA", "ILE", "LEU", "MET", "PHE", "TYR", "TRP", "VAL", "" };

vector<pair<string, float>> matches;

class LabelComparator: public AttrComparator {

private:
	double threshold;
public:
	LabelComparator(double thr) {
		threshold = thr;

	}
	virtual bool compatible(void *la, void*lb) {
		//SUBGRAPH
		Label *a = (Label *) la;
		//GRAPH
		Label *b = (Label *) lb;

		if ( DEBUG_MODE == 1) {
			cout << " a: " << a->name << " ";
			cout << " b: " << b->name << endl;
			cout << endl;
		}

		if (isPositionSpecific) {
			string temp[20];
			for (int i = 0; i < position.at(a->id).size(); i++)
				temp[i] = position.at(a->id).at(i);
			if (findor(temp, 20, b->name)) {
				//cout << "it is a match :" << a->id << " b: " << b->name << endl;
				return true;
			} else
				return false;
			//cout << "not a match";
		} else {
//			if (a->name == "MET") {
//				cout << "a is MET "
//						<< findor(negative,
//								(sizeof(negative) / sizeof(negative[0])),
//								a->name) << endl;
//
//			}

			if (findor(positive, (sizeof(positive) / sizeof(positive[0])),
					a->name)) {
				if (findor(positive, (sizeof(positive) / sizeof(positive[0])),
						b->name)) {
					if ( DEBUG_MODE == 1)
						cout << "matchpositive: " << "a: " << a->name << " "
								<< a->id << " b: " << b->name << " " << b->id;
					return true;
				} else
					return false;
			}
			if (findor(negative, (sizeof(negative) / sizeof(negative[0])),
					a->name)) {
				//cout << "matchnegativefirst: " << "a: " << a->name << " "
				//	<< a->id << endl;
				if (findor(negative, (sizeof(negative) / sizeof(negative[0])),
						b->name)) {
					if ( DEBUG_MODE == 1)
						cout << "matchnegative: " << "a: " << a->name << " "
								<< a->id << " b: " << b->name << " " << b->id
								<< endl;
					return true;
				} else
					return false;
			}
			if (findor(B, (sizeof(B) / sizeof(B[0])), a->name)) {
				if (findor(B, (sizeof(B) / sizeof(B[0])), b->name)) {
					if ( DEBUG_MODE == 1)
						cout << "match" << "a: " << a->name << " " << a->id
								<< "b: " << b->name << " " << b->id;
					return true;
				} else
					return false;
			}
			if (findor(C, (sizeof(C) / sizeof(C[0])), a->name)) {
				if (findor(C, (sizeof(C) / sizeof(C[0])), b->name)) {
					if ( DEBUG_MODE == 1)
						cout << "match" << "a: " << a->name << " " << a->id
								<< "b: " << b->name << " " << b->id;
					return true;
				} else
					return false;
			}
			if (findor(D, (sizeof(D) / sizeof(D[0])), a->name)) {
				if (findor(D, (sizeof(D) / sizeof(D[0])), b->name)) {
					if ( DEBUG_MODE == 1)
						cout << "match D: " << "a: " << a->name << " " << a->id
								<< " b: " << b->name << " " << b->id;
					return true;
				} else
					return false;
			}
			//cout << "not a match : " << "a: " << a->name << " " << a->id
			//		<< " b: " << b->name << " " << b->id << endl;
			return false;
		}
	}
	bool findor(string array[], int size, string item) {
		for (int i = 0; i < size; i++) {
			if (array[i] == item) {
				if ( DEBUG_MODE == 1) {
					//cout << "the value" << array[i] << "is equal to: " << item;
				}
				return true;
			}
		}
		return false;
	}
};

int scoring_Matrix[24][24];
string labels[24];
vector<string> pat_aminos;
vector<int> pat_ids;
vector<string> tar_aminos;
vector<int> tar_ids;
vector<string> tar_nums;
vector<char> tar_chains;
int maxScore = -1000;
vector<float> finalEdgeScore;

int scoring(int ni1, int ni2) {
	string first = pat_aminos.at(ni1);
	int firstID = 0;
	int secondID = 0;
	string second = tar_aminos.at(ni2);
	for (int i = 0; i < 24; i++) {
		if (first == labels[i])
			firstID = i;
		if (second == labels[i])
			secondID = i;
	}
	if (DEBUG_MODE == 1) {
		cout << "score of " << first << " and " << second << " is: "
				<< scoring_Matrix[firstID][secondID] << endl;
	}
	return scoring_Matrix[firstID][secondID];
}

vector<vector<int>> gLabels;
int mcount = 0;
bool my_visitor(int n, node_id ni1[], node_id ni2[], void *usr_data) {
	FILE *f = (FILE *) usr_data;
	vector<int> glabel;
	//print match
	int i;
	int score = 0;
	fprintf(f, targetName.c_str());
	string match = targetName + " ";
	//cout << "a match has been found " << endl;
	for (i = 0; i < n; i++) {
		match += to_string(ni1[i]) + " " + tar_nums.at(ni2[i]) + " , ";
		fprintf(f, " (%hd,%hd)", ni1[i], tar_nums.at(ni2[i]));
		glabel.push_back(ni2[i]);
		score = score + scoring(ni1[i], ni2[i]);
	}
	if (maxScore < score) {
		maxScore = score;
	}
	gLabels.push_back(glabel);

	fprintf(f, " %hd \n", score);
	matches.push_back(make_pair(match, score));
	mcount++;
	return false;
}

bool comparator(const pair<string, int>& a, const pair<string, int>& b) {
	return a.second > b.second;
}

vector<string> splitstr(const string& str, const string& delim) {
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do {
		pos = str.find(delim, prev);
		if (pos == string::npos)
			pos = str.length();
		string token = str.substr(prev, pos - prev);
		if (!token.empty())
			tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

int main(int argc, const char * argv[]) {
	clock_t tStart = clock();
	int numberOfResults = stoi(argv[2]);

	//subgraph
	string paternName = argv[1];

	//Allocate attributes to nodes and edges
	NewAllocator<Label> node_allocator;
	NewAllocator<EdgeLabel> edge_allocator;

	//open the graph
	ifstream graphInPat(paternName);

	StreamARGLoader<Label, EdgeLabel> pattloader(&node_allocator,
			&edge_allocator, graphInPat);
	graphInPat.close();

	//Read files of graphs to store more data
	ifstream graphInPat2(paternName);

	ARGraph<Label, EdgeLabel> p(&pattloader);

	p.SetNodeDestroyer(new LabelDestroyer());
	p.SetEdgeDestroyer(new LabelDestroyer());

	double my_threshold = 0.1;
	p.SetNodeComparator(new LabelComparator(my_threshold));
	p.SetEdgeComparator(new EdgeComparator(EdgeDiff));

	vector<float> pdist;
	for (int i = 0; i < p.NodeCount(); i++)
		for (int j = 0; j < p.NodeCount(); j++) {
			cout << "i: " << i << " j: " << j << endl;
			if (i != j) {
				if (p.GetEdgeAttr(i, j) != NULL) {
					cout << "pdist:" << abs(p.GetEdgeAttr(i, j)->score) << endl;
					pdist.push_back(abs(p.GetEdgeAttr(i, j)->score));
				}
			}
			cout << "here " << p.NodeCount() << endl;
		}

	string data;
	graphInPat2 >> data;
	int nOfresidues = stoi(data);
	for (int i = 0; i < nOfresidues; i++) {
		graphInPat2 >> data;
		pat_ids.push_back(stoi(data));
		graphInPat2 >> data;
		pat_aminos.push_back(data);
		graphInPat2 >> data;
	}

	graphInPat2.close();

	//graph

	//Read Scoring Matrix
	ifstream infile;
	infile.open("blosum62.txt");
	for (int i = 0; i < 24 && infile >> data; i++) {
		labels[i] = data;
	}
	for (int i = 0; i < 24; i++) {
		infile >> data;
		for (int j = 0; j < 24; j++) {
			infile >> data;
			scoring_Matrix[i][j] = stoi(data);
		}
	}
	infile.close();

	//Read position specific file
	ifstream pfile;
	pfile.open("position.txt");
	//if there exist a file with position specific classification of Labels
	if (pfile.peek() == std::ifstream::traits_type::eof() == false) {
		isPositionSpecific = true;
		while (getline(pfile, data)) {
			string sample;
			vector<string> line = splitstr(data, " ");
			//line.push_back(data);
			if (DEBUG_MODE == 1)
				for (int i = 0; i < line.size(); i++)
					cout << i << " : " << line.at(i) << " ";
			position.push_back(line);
		}
		//cout << "position lines: " << position.size() << endl;

	}
	pfile.close();

	struct dirent *entry = nullptr;
	DIR *dp = nullptr;
	dp = opendir("graphs/");
	if (dp != nullptr) {
		while ((entry = readdir(dp))) {
			string filename = entry->d_name;
			cout << "file: " << filename << endl;
			if (filename != "." && filename != "..") {
				maxScore = -1000;
				tar_aminos.clear();
				tar_ids.clear();
				tar_nums.clear();
				gLabels.clear();
				targetName = filename;

				//open the graph
				ifstream graphInTarg("graphs/" + targetName, ios::in);
//				char buffer[256];
//				if (!graphInTarg.is_open()) {
//					cout << "Error opening file";
//					exit(1);
//				}

//				while (!graphInTarg.eof()) {
//					graphInTarg.getline(buffer, 100);
//					cout << buffer << endl;
//				}
				//Read graphs
				StreamARGLoader<Label, EdgeLabel> targloader(&node_allocator,
						&edge_allocator, graphInTarg);
				graphInTarg.close();
				//Read files of graphs to store more data
				ifstream graphInTarg2("graphs/" + targetName);

				graphInTarg2 >> data;
				nOfresidues = stoi(data);
				for (int i = 0; i < nOfresidues; i++) {
					graphInTarg2 >> data;
					tar_ids.push_back(stoi(data));
					graphInTarg2 >> data;
					tar_aminos.push_back(data);
					graphInTarg2 >> data;
					tar_nums.push_back(data);
				}
				graphInTarg2.close();
				/*
				 //example of creating a graph manually
				 ARGEdit ed;
				 ARGEdit pat;
				 int i, j;
				 ed.InsertNode(new Label("a"));
				 ed.InsertNode(new Label("a"));
				 ed.InsertNode(new Label("b"));
				 ed.InsertNode(new Label("c"));
				 ed.InsertNode(new Label("d"));
				 ed.InsertNode(new Label("e"));
				 ed.InsertNode(new Label("f"));
				 ed.InsertNode(new Label("a"));
				 ed.InsertNode(new Label("b"));
				 ed.InsertNode(new Label("c"));
				 ed.InsertNode(new Label("d"));
				 ed.InsertNode(new Label("e"));
				 ed.InsertNode(new Label("f"));

				 //for (i = 0; i < 8; i++)
				 for (j = 0; j < 8; j++)
				 if (i != j)
				 ed.InsertEdge(i, j, NULL);
				 ed.InsertEdge(1, 2, NULL);
				 ed.InsertEdge(1, 5, NULL);
				 ed.InsertEdge(2, 3, NULL);
				 ed.InsertEdge(2, 4, NULL);
				 ed.InsertEdge(3, 6, NULL);
				 ed.InsertEdge(4, 5, NULL);
				 ed.InsertEdge(7, 8, NULL);
				 ed.InsertEdge(7, 11, NULL);
				 ed.InsertEdge(8, 9, NULL);
				 ed.InsertEdge(8, 10, NULL);
				 ed.InsertEdge(9, 12, NULL);
				 ed.InsertEdge(10, 11, NULL);
				 pat.InsertNode(new Label("a"));
				 pat.InsertNode(new Label("a"));
				 pat.InsertNode(new Label("b"));
				 pat.InsertNode(new Label("d"));
				 pat.InsertNode(new Label("e"));

				 pat.InsertEdge(1, 2, NULL);
				 pat.InsertEdge(2, 3, NULL);
				 pat.InsertEdge(3, 4, NULL);
				 pat.InsertEdge(1, 4, NULL);
				 */

				ARGraph<Label, EdgeLabel> g(&targloader);

				g.SetNodeDestroyer(new LabelDestroyer());
				g.SetEdgeDestroyer(new LabelDestroyer());
				//the starting state
				VF2SubState s0(&p, &g);

				//output file

				stringstream stream;
				stream << "Results/output-" << targetName.erase(4, 4) << ".txt";
				string str = stream.str();
				const char *output = str.c_str();
				if (DEBUG_MODE == 1)
					cout << "output is: " << output << endl;
				FILE * f = fopen(output, "w");
				//running the match algorithm
				int nOfmatches = match(&s0, my_visitor, f);
				cout << "numberOfmatches " << nOfmatches << endl;

				//compute score for edges
				for (int k = 0; k < nOfmatches; k++) {
					int c = 0;
					float edgeScore = 0;
					for (int l1 = 0; l1 < gLabels.at(k).size(); l1++)
						for (int l2 = 0; l2 < gLabels.at(k).size(); l2++)
							if (l1 != l2) {
								edgeScore += abs(
										g.GetEdgeAttr(gLabels.at(k).at(l1),
												gLabels.at(k).at(l2))->score
												- pdist.at(c));
								cout << "l1: " << l1 << "l2: " << l2
										<< " edge score: " << edgeScore << endl;
								c++;

							}
					finalEdgeScore.push_back(edgeScore);
				}

				/*node_id id = 0;
				 cout << "hey: " << g.NodeCount();
				 for (int i = 0; i < g.NodeCount(); i++) {
				 cout << "before";
				 Label* hi = g.GetNodeAttr(i);
				 cout << "atttr: " << hi->name;
				 }
				 */
				fclose(f);
				cout << "maxScore: " << maxScore << endl;
				/*
				 int n;
				 node_id ni1[MAXNODES], ni2[MAXNODES];
				 if (!match(&s0, &n, ni1, ni2)) {
				 printf("NO MATCH!\n");
				 return 1;
				 }

				 printf("Found a match %d nodes :\n", n);
				 for (i = 0; i < n; i++)
				 printf("\tNode %hd of graph 1 is paired wiht node %hd of graph 2\n",

				 ni1, ni2);*/

			} else
				cout << "directory skipped \n";
		}
	}

	closedir(dp);

	for (int i = 0; i < matches.size(); i++) {
		float f = finalEdgeScore.at(i);
		if (f != 0)
			matches[i].second *= (1 / f);
		if (f == 0)
			matches[i].second += 1000;
	}

	sort(matches.begin(), matches.end(), comparator);

	for (int i = 0; i < numberOfResults && i < matches.size(); i++) {
		string m = matches[i].first;
		cout << m << " " << matches[i].second << endl;
//		m = m.substr(m.find(" ") + 1);
//		do {
//			int token1 = stoi(m.substr(0, m.find(" ")));
//			m = m.substr(m.find(" ") + 1);
//			int token2 = stoi(m.substr(0, m.find(" ")));
//			m = m.substr(m.find(" ") + 3);
//			glabels.push_back(token2);
//			cout << "token1: " << token1 << " token2: " << token2 << endl;
//
//		} while (m != "");
//		for (int key : glabels) {
//			vector<int>::iterator itr = find(tar_nums.begin(), tar_nums.end(),
//					key);
//
//			if (itr != tar_nums.cend()) {
//				cout << "tarnums: " << tar_nums.at(119);
//				cout << "Element present at index " << key << " "
//						<< distance(tar_nums.begin(), itr) << endl;
//			} else {
//				std::cout << "Element not found";
//			}
//		}

	}

	cout << "this is the end" << endl;
	printf("Time taken: %.2fs\n", (double) (clock() - tStart) / CLOCKS_PER_SEC);
	cout << "hold your breath and count to 10" << endl;

	return 0;
}

