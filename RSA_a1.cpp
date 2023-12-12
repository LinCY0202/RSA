/*
=========================================================================
    2023/12/7   林承億
    環境：Linux version 6.2.0-37-generic (buildd@bos03-amd64-055) (x86_64-linux-gnu-gcc-11 (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0
    RSA加解密功能，使用外部GMP函式庫。https://gmplib.org

	RSA加密原理、過程：
		1.找出質數p、q
		2.n = p x q
		3.phi(n) = (p-1)(q-1)
		4.公鑰e 滿足1 < e < phi(n) 的整數，並且 e 和 phi(n) 必須互質。
		5.私鑰d 滿足 e x d % phi(n) 餘1
		加密：M^e % n = C	(密文)
		解密：C^d % n = M	(原文)

    輸入公鑰e、質數p、q及待加密內容Text。
    輸出
        公鑰e、質數p、q
        待加密內容Text、n、d
        (轉碼)M、（加密）C
        After Decode、Decode Text
    編譯方式：g++ RSA.cpp -o RSA -lgmpxx -lgmp
    開啟方法：
        自行輸入測資：./RSA
        使用預設測資：./RSA < RSA_Test_Data/3.in  > RSA_Test_Data/3.out
=========================================================================
*/


#include<iostream>
#include<string>
#include<gmpxx.h>
#include<map>
#include<vector>
using namespace std;
map<char,int> letter_to_int = {
        {'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, 
		{'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15}, 
		{'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23}, 
		{'Y', 24}, {'Z', 25}, {' ', 26}
    };
map<int, string> int_to_letter;

int main(){
	for(auto &pair : letter_to_int) {
		int_to_letter[pair.second] = pair.first;
	}
	mpz_class e,p,q,n,phi,d;


    while (true) {
        cout << "輸入（p）：";
        cin >> p;
        if (mpz_probab_prime_p(p.get_mpz_t(), 25) != 0) {
            std::cout << "p是質數。\n";
            break;
        } else {
            cout << "p不是質數，請重新輸入。\n";
        }
    }

    while (true) {
        cout << "輸入（q）：";
        cin >> q;
        if (mpz_probab_prime_p(q.get_mpz_t(), 25) != 0) {
            cout << "q是質數。\n";
            break;
        } else {
            cout << "q不是質數，請重新輸入。\n";
        }
	}

	// 計算 n = p*q
	n = p * q;
	p--,q--;
    phi = p * q;

	while(true) {
		cout << "輸入公鑰e：";
		cin >> e;
		mpz_class gcd;
		mpz_gcd(gcd.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t());

		if (gcd == 1) {
			cout << "e和phi是互質的。\n";
			break;
		} else {
			cout << "qe和phi不是互質的，請重新輸入。\n";
		}
	}


	cout << "Plain Text：";
	cin.ignore();
	string s;	getline(cin,s);

    cout << "\n\n=========================================================\n\n";
    cout << "公鑰e：" << e << "\n";	
    cout << "（p）：" << p << "\n";
	cout << "（q）：" << q << "\n";
	cout << "Plain Text：" << s << "\n";
	cout << "n = " << n << "\n";


	vector<mpz_class> M;
	for(int i=0;i<s.size();i+=2){
		M.push_back(letter_to_int[s[i]]*100+letter_to_int[s[i+1]]);
	}if(s.size()%2!=0){
		M.back()=M.back()+26;
	}
		

	vector<mpz_class> C; 
	
    // 計算 d = e^(-1) mod phi
	if (mpz_invert(d.get_mpz_t(), e.get_mpz_t(), phi.get_mpz_t())) {  // 如果模反元素存在
		cout << "d = " << d << "\n";
	} else {
		cout << "Inverse doesn't exist\n" << endl;
	}

	cout << "M = ";
	for(auto &v:M){					//m^e%n  -> C
		cout << v << "  ";
		mpz_class tame;
		mpz_powm(tame.get_mpz_t(), v.get_mpz_t(), e.get_mpz_t(), n.get_mpz_t());
		C.push_back(tame);
	}cout << "\n";
	
	cout << "C = ";
	for(auto &v:C){
		cout << v << "  ";
	}cout << "\n";


	//decoding
	vector<int> decode;
	for(auto &v:C){
		mpz_class tame;
		mpz_powm(tame.get_mpz_t(), v.get_mpz_t(), d.get_mpz_t(), n.get_mpz_t());

		int tame_int = mpz_get_si(tame.get_mpz_t());// 將 mpz_t 轉換為 int
		decode.push_back(tame_int);
	}

	cout << "After Decode = ";
	for(auto &v:decode){
		cout << v << " ";
	}cout << "\n";
	string text="";

	for(auto &v:decode){
		if(v==0){
			text+="AA";
		}
		else if(v<10){
			text+="A"+int_to_letter[v];
		}
		else if(v<100){
			text+="A"+int_to_letter[v];
		}
		else if(v<1000){
			text+=int_to_letter[v/100]+int_to_letter[v%100];
		}
		else{
			text+=int_to_letter[v/100]+int_to_letter[v%100];
		}
	}
    cout << "Decode Text：";
	cout << text << "\n";

}
