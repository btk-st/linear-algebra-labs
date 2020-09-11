//only A task
#include <stdlib.h>
#include <fstream>
#include <assert.h>  
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
class Fraction
{
private:
	int m_numerator;
	int m_denominator;

public:
	Fraction (int numerator = 0, int denominator = 1) : m_numerator(numerator), m_denominator(denominator) {
		if (m_denominator < 0) {
			m_numerator = -m_numerator;
			m_denominator = -m_denominator;
		}
		assert(denominator != 0);
		reduce();
	}
	Fraction(const std::string &string) {
		assert(string != "");

		int pos = string.find("/");

		if (pos == std::string::npos) {
			m_numerator = stoi(string);
			m_denominator = 1;
		}
		else {
			m_numerator = std::stoi(string.substr(0, pos));
			m_denominator = std::stoi(string.substr(pos+1, string.length()));
			assert(m_denominator != 0);
		}
	}
	static int gcd(int a, int b) {
		return (b == 0) ? (a > 0 ? a : -a) : gcd(b, a % b);
	}
	static int lcm(int a, int b) {
		return (a*b) / Fraction::gcd(abs(a), abs(b));
	}
	void reduce() {
		int gcd = Fraction::gcd(abs(m_numerator), abs(m_denominator));
		m_numerator /= gcd;
		m_denominator /= gcd;
	}
	bool isPositive() {
		if ((m_numerator >= 0 && m_denominator > 0) || (m_numerator <= 0 && m_denominator < 0))
			return true;
		else
			return false;
	}
	bool isZero() {
		if (m_numerator == 0)
			return true;
		else
			return false;
	}
	int getNumerator() {
		return m_numerator;
	}
	int getDenominator() {
		return m_denominator;
	}
	std::string toString() const {
		std::string fraction = "";
		if (m_numerator == 0) {
			fraction.append("0");
			return fraction;
		}
		fraction.append(std::to_string(m_numerator));
		if (m_denominator != 1) {
			fraction.append("/");
			fraction.append(std::to_string(m_denominator));
		}
		return fraction;
	}

	friend Fraction operator+(const Fraction &f1, const Fraction &f2);

	friend Fraction operator-(const Fraction &f1, const Fraction &f2);

	friend Fraction operator*(const Fraction &f1, const Fraction &f2);

	friend Fraction operator/(const Fraction &f1, const Fraction &f2);
	friend std::ostream& operator<<(std::ostream &out, const Fraction &f);


};
Fraction operator+(const Fraction &f1, const Fraction &f2) {
	int denominator = Fraction::lcm(abs(f1.m_denominator), abs(f2.m_denominator));
	int numerator = f1.m_numerator * (denominator / f1.m_denominator) + f2.m_numerator * (denominator / f2.m_denominator);
	return Fraction(numerator, denominator);
}
Fraction operator-(const Fraction &f1, const Fraction &f2) {
	return f1 + Fraction(-f2.m_numerator, f2.m_denominator);
}
Fraction operator*(const Fraction &f1, const Fraction &f2) {
	return Fraction(f1.m_numerator*f2.m_numerator, f1.m_denominator*f2.m_denominator);
}
Fraction operator/(const Fraction &f1, const Fraction &f2)
{
	return Fraction(f1.m_numerator * f2.m_denominator, f1.m_denominator * f2.m_numerator);
}
std::ostream& operator<<(std::ostream &out, const Fraction &f) {
	out << f.toString();
	return out;
};

using namespace std;
void printMatrix(vector<vector<Fraction>>);
void printPolynominal(Fraction *, int);
Fraction determinant(vector<vector<Fraction>>);
Fraction* charPolynominal(vector<vector<Fraction>>);
vector<Fraction> rationalRoots(Fraction *, int);
void findDivisors(int, vector<int> &);
bool isRoot(Fraction*, Fraction, int);
bool notInRoots(vector<Fraction>, Fraction);
int main(int argc, char *argv[]) {
	int n;
	string s;
	ifstream fin;
	fin.open(argv[1], ios::in);
	getline(fin, s);
	n = stoi(s.substr(s.rfind(" ") + 1));
	vector<vector<Fraction>> matrix(n, vector<Fraction>(n));
	for (int i =0; i < n; i++)
		for (int j = 0; j < n; j++) {
			string tmp;
			fin >> s;
			matrix[i][j] = Fraction(s);
		}
	cout << "Matrix:" << endl;
	printMatrix(matrix);
	Fraction *polynominal = charPolynominal(matrix);
	cout << endl << "Characteristic polynomial:" << endl ;
	printPolynominal(polynominal, n);

	vector<Fraction> roots = rationalRoots(polynominal, n);
	if (roots.size() > 0) {
		cout << endl  << "Rational eigenvalues: " << endl;
		for (int i = 0; i < roots.size(); i++) {
			cout << "x_" << i << " = " << roots[i] << endl;
		}
	}
	else
		cout << endl << "No rational eigenvalues" << endl;
	system("pause");
}
Fraction determinant(vector<vector<Fraction>> matrix) {
	Fraction det(0,1);
	int n = matrix.size();
	vector<vector<Fraction>> submatrix (n-1, vector<Fraction>(n-1));
	if (n == 1)
		return (matrix[0][0]);
	if (n == 2)
		return ((matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1]));
	else {
		for (int x = 0; x < n; x++) {
			int subi = 0;
			for (int i = 1; i < n; i++) {
				int subj = 0;
				for (int j = 0; j < n; j++) {
					if (j == x)
						continue;
					submatrix[subi][subj] = matrix[i][j];
					subj++;
				}
				subi++;
			}
			det = det + ((x % 2 == 0 ? Fraction(1,1) : Fraction(-1,1)) * matrix[0][x] * determinant(submatrix));
		}
	}
	return det;
}
void printMatrix(vector<vector<Fraction>> matrix) {
	for (int i = 0; i < matrix.size(); i++) {
		for (int j = 0; j < matrix[i].size(); j++) {
			//cout.width(5);
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}
void printPolynominal(Fraction *polynominal, int n) {
	for (int i = 0; i < n + 1; i++) {
		if (polynominal[i].isZero())
			continue;
		if (i == 0) {
			if (polynominal[i].isPositive())
				cout << "x^" << n;
			else
				cout << "-x^" << n;
		}
		else
			if (i < n)
				if (polynominal[i].isPositive())
					cout << "+" << polynominal[i] << "*x^" << n-i;
				else
					cout << polynominal[i] << "*x^" << n - i;
			else
				if (polynominal[i].isPositive())
					cout << "+" << polynominal[i];
				else
					cout << polynominal[i];
	}
	cout << endl;
}
Fraction* charPolynominal(vector<vector<Fraction>> matrix)
{
	int n = matrix.size();
	Fraction *polynominal = new Fraction[n+1]{};
	polynominal[0] = n % 2 == 0 ? Fraction(1, 1) : Fraction(-1, 1);
	for (int order = 1; order < n + 1; order++) {
		Fraction curKoef(0,1);
		std::string bitmask(order, 1);
		bitmask.resize(n, 0);
		do {
			vector<int> minors;
			for (int i = 0; i < n; ++i) {
				if (bitmask[i])
					minors.push_back(i);
			}
			/*cout << "Minors: ";
			for (int i = 0; i < order; i++)
				cout << minors[i] << " ";
			cout << endl;*/
			//create submatrix
			vector<vector<Fraction>> submatrix(order, vector<Fraction>(order));
			for (int i = 0; i < order; i++)
				for (int j = 0; j < order; j++)
					submatrix[i][j] = matrix[minors[i]][minors[j]];
			/*cout << "Sub matr: " << endl;
			printMatrix(submatrix);*/
			//add to koef
			curKoef = curKoef + determinant(submatrix);
		} while (std::prev_permutation(bitmask.begin(), bitmask.end()));
		
	polynominal[order] = curKoef * ((n-order) % 2 == 0 ? Fraction(1,1) : Fraction(-1,1));
	}
	return polynominal;
}
void findDivisors(int x, vector<int> &dividers) {
	dividers.push_back(1);
	for (int i = 2; i <= x; i++) {
		if (x % i == 0) {
			dividers.push_back(i);
		}
	}
}
bool isRoot(Fraction* polynominal, Fraction root, int n) {
	Fraction ans(0, 1);
	for (int i = n; i >= 0; i--) {
		Fraction x(1, 1);
		for (int j = 0; j < n - i; j++, x = x * root);
		ans = ans + polynominal[i] * x;
	}
	if (ans.getNumerator() == 0)
		return true;
	else
		return false;
}
bool notInRoots(vector<Fraction> roots, Fraction root) {
	for (int i = 0; i < roots.size(); i++) {
		if (roots[i].getNumerator() == root.getNumerator() && roots[i].getDenominator() == root.getDenominator())
			return false;
	}
	return true;
}
vector<Fraction> rationalRoots(Fraction *polynominal, int n) {
	vector<Fraction> roots;
	vector<int> anDivisors;
	vector <int> a0Divisors;
	int an, a0;
	//if 0 is root
	Fraction root(0, 1);
	if (isRoot(polynominal, root, n))
		roots.push_back(root);
	//if 0 koef in the end
	for (; n >= 0; n--) {
		if (polynominal[n].getNumerator() != 0)
			break;
	}
	//lcm for polynominal denominators
	int lcm = polynominal[0].getDenominator();
	for (int i = 1; i < n + 1; i++) {
		lcm = Fraction::lcm(lcm, polynominal[i].getDenominator());
	}
	//an*x^n + ... + a0 = 0
	an = polynominal[0].getNumerator() * lcm / polynominal[0].getDenominator();
	a0 = polynominal[n].getNumerator() * lcm / polynominal[n].getDenominator();
	findDivisors(abs(an), anDivisors);
	findDivisors(abs(a0), a0Divisors);
	//checking all combinations p and q
	for (int p = 0; p < a0Divisors.size(); p++)
		for (int q = 0; q < anDivisors.size(); q++) {
			Fraction root(a0Divisors[p], anDivisors[q]);
			if (isRoot(polynominal, root, n))
				if (notInRoots(roots, root))
					roots.push_back(root);
			root = Fraction(-(a0Divisors[p]), anDivisors[q]);
			if (isRoot(polynominal, root, n))
				if (notInRoots(roots, root))
					roots.push_back(root);
		}
	return roots;
}