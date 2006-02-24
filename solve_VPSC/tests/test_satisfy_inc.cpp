#include <variable.h>
#include <constraint.h>
#include <solve_VPSC.h>
#include <algorithm>
#include <iostream>
#include <time.h>
#include <math.h>
using namespace std;

inline int getRand(const int range) {
	return (double)range*rand()/(RAND_MAX+1.0);
}
inline bool approxEquals(const double a, const double b) {
	return fabs((double)a-b)<0.000001;
}
typedef vector<Constraint*> CS;

// most basic example
void test1() {
	cout << "Test 1..." << endl;
	Variable *a[] = {
		new Variable(0,6,1),
		new Variable(0,5,1)};
	Constraint *c[] = {
		new Constraint(a[0],a[1],2)};
	VPSC vpsc(a,2,c,1);
	vpsc.satisfy_inc();
	assert(approxEquals(a[0]->position(),4.5));
	assert(approxEquals(a[1]->position(),6.5));
	cout << "Test 1... done." << endl;
}

// no splits required
void test2() {
	cout << "Test 2..." << endl;
	Variable *a[] = {
		new Variable(0,4,1),
		new Variable(1,6,1),
		new Variable(2,9,1),
		new Variable(3,2,1),
		new Variable(4,5,1)};
	Constraint *c[] = {
		new Constraint(a[0],a[2],3),
		new Constraint(a[0],a[3],3),
		new Constraint(a[1],a[4],3),
		new Constraint(a[2],a[4],3),
		new Constraint(a[2],a[3],3),
		new Constraint(a[3],a[4],3)};
	double expected[]={0.5,6,3.5,6.5,9.5};
	unsigned int n = sizeof(a)/sizeof(Variable*);
	unsigned int m = sizeof(c)/sizeof(Constraint*);
	VPSC vpsc(a,n,c,m);
	vpsc.satisfy_inc();
	for(int i=0;i<n;i++) {
		assert(approxEquals(a[i]->position(),expected[i]));
	}
	cout << "Test 2... done." << endl;
}

// split required - cycles without change in direction check
void test3() {
	cout << "Test 3..." << endl;
	Variable *a[] = {
		new Variable(0,5,1),
		new Variable(1,6,1),
		new Variable(2,7,1),
		new Variable(3,4,1),
		new Variable(4,3,1)};
	Constraint *c[] = {
		new Constraint(a[0],a[4],3),
		new Constraint(a[1],a[2],3),
		new Constraint(a[2],a[3],3),
		new Constraint(a[2],a[4],3),
		new Constraint(a[3],a[4],3)};
	double expected[]={5,0.5,3.5,6.5,9.5};
	unsigned int n = sizeof(a)/sizeof(Variable*);
	unsigned int m = sizeof(c)/sizeof(Constraint*);
	VPSC vpsc(a,n,c,m);
	vpsc.satisfy_inc();
	for(int i=0;i<n;i++) {
		assert(approxEquals(a[i]->position(),expected[i]));
	}
	cout << "Test 3... done." << endl;
}
// split required - cycles!
void test4() {
	cout << "Test 4..." << endl;
	Variable *a[] = {
		new Variable(0,7,1),
		new Variable(1,1,1),
		new Variable(2,6,1),
		new Variable(3,0,1),
		new Variable(4,2,1)};
	Constraint *c[] = {
		new Constraint(a[0],a[3],3),
		new Constraint(a[0],a[1],3),
		new Constraint(a[1],a[4],3),
		new Constraint(a[2],a[4],3),
		new Constraint(a[2],a[3],3),
		new Constraint(a[3],a[4],3)};
	double expected[]={5,0.5,3.5,6.5,9.5};
	unsigned int n = sizeof(a)/sizeof(Variable*);
	unsigned int m = sizeof(c)/sizeof(Constraint*);
	VPSC vpsc(a,n,c,m);
	vpsc.satisfy_inc();
	/*
	for(int i=0;i<n;i++) {
		assert(approxEquals(a[i]->position(),expected[i]));
	}
	*/
	cout << "Test 4... done." << endl;
}
// n=number vars
// m=max constraints per var
void rand_test(int n, int m) {
	Variable *a[n];
	CS cs;
	for(int i=0;i<n;i++) {
		a[i]=new Variable(i,getRand(10),1);
	}
	for(int i=0;i<n-1;i++) {
		for(int j=0;j<getRand(m)+1;j++) {
			int e = i + getRand(n-1-i)+1;
			cs.push_back(new Constraint(a[i],a[e],3));
		}
	}
	Constraint *acs[cs.size()];
	for(int i=0;i<cs.size();i++) {
		acs[i]=cs[i];
	}
	/*
	cout<<"digraph g {"<<endl;
	for(CS::iterator i(cs.begin());i!=cs.end();i++) {
		Constraint *c=*i;
		cout << c->left->id << "->" << c->right->id << ";" << endl;
	}
	cout<<"}"<<endl;
	*/
	VPSC vpsc(a,n,acs,cs.size());
	try {
		vpsc.satisfy_inc();
	} catch (char const *msg) {
		cout << msg << endl;
		for(int i=0;i<n;i++) {
			cout << "new Variable("<<i<<","<<a[i]->position()<<
				",1)," << endl;
			cout << "a[i]->Pos="<<a[i]->position() << endl;
		}
		for(CS::iterator i(cs.begin());i!=cs.end();i++) {
			Constraint *c=*i;
			cout << c->left->id << "->" << c->right->id << ";" << endl;
			cout << "cs.push_back(new Constraint(a[" <<  c->left->id << "],a[" <<  c->left->id << "],3));" << endl;
		}
		throw "test failed!";
	}
}
int main() {
	srand(time(NULL));
	test1();
	test2();
	test3();
	test4();
	/*
	for(int i=0;i<100000;i++) {
		if(i%10==0) cout << "i=" << i << endl;
		rand_test(100,3);
	}
	*/
	return 0;
}