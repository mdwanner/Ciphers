#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include "nihilist.h"
#include <sstream>

using namespace std;

string uppercase(string s) {
	for (unsigned int i = 0; i < s.length(); i++) {
		if (s[i] < 123 && s[i] > 96) {
			s[i] = s[i] - 32;
		}
	}
	return s;
}

string lowercase(string s) {
	for (unsigned int i = 0; i < s.length(); i++) {
		if (s[i] < 91 && s[i] > 64) {
			s[i] = s[i] + 32;
		}
	}
	return s;
}

// removes all characters that aren't letters from string
string removeNonLetters(string s) {
	for (unsigned int i = 0; i < s.length(); i++) {
		if (!((s[i] < 91 && s[i] > 64) ^ (s[i] < 123 && s[i] > 96))) {
			s.erase(i--, 1);
		}
		else if (s[i] == 74) {
			s[i] = s[i] - 1;
		}
		else if (s[i] == 106) {
			s[i] = s[i] - 1;
		}
	}
	return s;
}

string removeNonNumbers(string s) {
	for (unsigned int i = 0; i < s.length(); i++) {
		if (!(s[i] < 58 && s[i] > 47)) {
			s.erase(i--, 1);
		}
	}
	return s;
}

string nihilist(string k1, string k2, string s, bool encrypt) {
	char polybius[5][5];
	vector<char> alphabet;
	string result = "";

	k1 = removeNonLetters(k1);
	k2 = removeNonLetters(k2);

	if (encrypt) {
		s = removeNonLetters(s);

		k1 = uppercase(k1);
		k2 = uppercase(k2);
		s = uppercase(s);

		// populate alphabet
		for (unsigned int i = 0; i < 26; i++) {
			if ('A' + i == 'J')
				continue;
			alphabet.push_back('A' + i);
		}

		// remove duplicate characters from polybius key
		for (unsigned int i = 0; i < k1.length() - 1; i++) {
			unsigned int index = k1.find(k1[i], i + 1);
			if (index != string::npos) {
				k1.erase(index, 1);
				i--;
			}
		}

		// populate modified alphabet
		unsigned int temp1 = 0;
		unsigned int temp2 = 0;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (temp1 < k1.length()) {
					polybius[i][j] = k1[temp1];
					if (k1[temp1] > 74) {
						alphabet[(int)(k1[temp1] - 66)] = '0';
					}
					else {
						alphabet[(int)(k1[temp1] - 65)] = '0';
					}
					temp1++;
				}
				else {
					if (temp2 < 26) {
						if (alphabet[temp2] != '0') {
							polybius[i][j] = alphabet[temp2];
						}
						else {
							while (alphabet[temp2] == '0') {
								temp2++;
							}
							polybius[i][j] = alphabet[temp2];
						}
						temp2++;
					}
				}
			}
		}

		// encode plaintext intermediate
		unsigned int i = 0;
		vector<int> plainNums;
		while (i < s.length()) {
			for (int j = 0; j < 5; j++) {
				for (int k = 0; k < 5; k++) {
					if (s[i] == polybius[j][k]) {
						plainNums.push_back((j+1) * 10 + (k + 1));
						goto OUT1;
					}
				}
			}
		OUT1:
			i++;
		}

		// encode vigenere key intermediate
		i = 0;
		unsigned int n = 0;
		unsigned int temp;
		vector<int> keyNums;
		while (i < k2.length()) {
			for (int j = 0; j < 5; j++) {
				for (int k = 0; k < 5; k++) {
					if (k2[i] == polybius[j][k]) {
						keyNums.push_back((j + 1) * 10 + (k + 1));
						goto OUT2;
					}
				}
			}
		OUT2:
			i++;
		}
		
		// repeat the intermediate key so it matches the plaintext length
		temp = plainNums.size() - keyNums.size();
		for (i = 0; i < temp; i++) {
			keyNums.push_back(keyNums[n++]);
			if (n == keyNums.size()) {
				n = 0;
			}
		}

		stringstream ss;
		string num;

		for (i = 0; i < plainNums.size(); i++) {
			ss << (plainNums[i] + keyNums[i]);
			ss >> num;

			ss.str(string());
			ss.clear();

			result += num + " ";
		}
	}
	else { // decrypt nihilist cipher
		s = removeNonNumbers(s);

		k1 = lowercase(k1);
		k2 = lowercase(k2);

		// populate alphabet
		for (unsigned int i = 0; i < 26; i++) {
			if ('a' + i == 'j')
				continue;
			alphabet.push_back('a' + i);
		}

		// remove duplicate characters from polybius key
		for (unsigned int i = 0; i < k1.length() - 1; i++) {
			unsigned int index = k1.find(k1[i], i + 1);
			if (index != string::npos) {
				k1.erase(index, 1);
				i--;
			}
		}

		// populate modified alphabet
		unsigned int temp1 = 0;
		unsigned int temp2 = 0;
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 5; j++) {
				if (temp1 < k1.length()) {
					polybius[i][j] = k1[temp1];
					if (k1[temp1] > 106) {
						alphabet[(int)(k1[temp1] - 98)] = '0';
					}
					else {
						alphabet[(int)(k1[temp1] - 97)] = '0';
					}
					temp1++;
				}
				else {
					if (temp2 < 26) {
						if (alphabet[temp2] != '0') {
							polybius[i][j] = alphabet[temp2];
						}
						else {
							while (alphabet[temp2] == '0') {
								temp2++;
							}
							polybius[i][j] = alphabet[temp2];
						}
						temp2++;
					}
				}
			}
		}

		stringstream ss;
		int num;
		unsigned int i = 0;
		vector<int> cryptoNums;

		for (i = 0; i < s.length() - 1; i+=2) {
			ss << s.substr(i,2);
			ss >> num;

			cryptoNums.push_back(num);

			ss.str(string());
			ss.clear();
		}

		// encode vigenere key intermediate
		vector<int> keyNums;
		i = 0;
		while (i < k2.length()) {
			for (int j = 0; j < 5; j++) {
				for (int k = 0; k < 5; k++) {
					if (k2[i] == polybius[j][k]) {
						keyNums.push_back((j + 1) * 10 + (k + 1));
						goto OUT3;
					}
				}
			}
		OUT3:
			i++;
		}

		// repeat the intermediate key so it matches the ciphertext length
		unsigned int n = 0;
		unsigned int temp;

		temp = cryptoNums.size() - keyNums.size();
		for (i = 0; i < temp; i++) {
			keyNums.push_back(keyNums[n++]);
			if (n == keyNums.size()) {
				n = 0;
			}
		}

		// subtract key from ciphertext to obtain plaintext intermediate
		for (i = 0; i < cryptoNums.size(); i++) {
			cryptoNums[i] = cryptoNums[i] - keyNums[i];
		}

		// decode the numbered intermediate back into English characters
		for (i = 0; i < cryptoNums.size(); i++) {
			result += polybius[cryptoNums[i] / 10 - 1][cryptoNums[i] % 10 - 1];
		}
	}

	return result;
}

int main() {
	cout << nihilist("Playfair", "Paris", "24 58 67 34 68 45 26 67 46 57 64 54", false) << endl;
	return 0;
>>>>>>> 01ba30fe3b3a2a63af83b53a3a44f710dafc5ff3
}