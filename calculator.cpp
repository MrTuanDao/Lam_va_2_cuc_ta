#include <iostream>
#include <stack>
#include <math.h>
#include <map>
using namespace std;
// this program calcute the expression with non-negative intergers and +,-,/,*,^

int findNum(string s,int i){
	int j;
	for (j=i;j<s.size();j++){
		if (s[j]<'0' || s[j]>'9')
			break;
	}
	return j-1;
}

int cal(int a,int b, char c){
	switch (c){
		case '+': return a+b;
		case '-': return a-b;
		case '*': return a*b;
		case '/': return a/b;
		case '^': return pow(a,b);
	}
}

int toNum(string s){
	int res=0;
	int n=s.size();
	for (int i=0;i<n;i++){
		res+=((s[i]-48)*pow(10,n-i-1));
	}
	return res;
}

int order(char c){
	if (c=='+'||c=='-')
		return 1;
	if (c=='*'||c=='/')
		return 2;
	if (c=='^')
		return 3;
	return 0;
}

int cal(string s){
	int i=0,k;
	stack<char> st;
	stack<int> st1;
	string res="";
	while (i<s.size()){
		if (s[i]>='0' && s[i]<='9'){
			k=findNum(s,i);
			res+=(s.substr(i,k-i+1)+" ");
			i=k+1;
		}
		else{
			if(s[i]!='('&& s[i]!=')'){
				if (st.empty()){
					st.push(s[i]);
				}
				else{
					while (!st.empty()&&((order(s[i])<order(st.top()))||(order(s[i])==order(st.top())&&s[i]!='^'))){
						res+=st.top();
						st.pop();
					}
					st.push(s[i]);
				}
			}
			else{
				if (s[i]=='(')
					st.push(s[i]);
			
				else{
					while(st.top()!='('){
						res+=st.top();
						st.pop();
					}
					st.pop();
				}
			}
			i++;
		}
	}
	while(!st.empty()){
		res+=st.top();
		st.pop();
	}
	//cout<<res<<"\n"; // convert to postfix notation
	i=0;
	while(i<res.size()){
		if (res[i]>='0' && res[i]<='9'){
			k=findNum(res,i);
			st1.push(toNum(res.substr(i,k-i+1)));
			i=k+2;
		}
		else{
			int b=st1.top();
			st1.pop();
			int a=st1.top();
			st1.pop();
			st1.push(cal(a,b,res[i]));
			i++;
		}
	}
	return st1.top();
} 

int main(){	
	int i=0,k;
	stack<char> st;
	stack<int> st1;
	string s,res;
	res="";
	cout<<"CALCULATOR CONSOLE PROGRAM"<<endl;
	while(1){
		cout<<"Insert the expression: ";
		getline(cin,s);
		cout<<cal(s)<<endl;
	}
	return 0;
}
