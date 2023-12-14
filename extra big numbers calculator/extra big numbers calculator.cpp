#include<iostream>
#include<cstring>
#include<cstdlib>
#include<strstream>
#include<iomanip>
#include<cmath>
using namespace std;
const int SZ = 50;

class extr
{
private:
	char mas[SZ];//array for number
	int vlen;//length of number
	char si = ' ';//sign
	extr mult(const int a);//mult numb
	extr mult10(const extr d);//mult 10

public:
	void ltoa(int, char*);//int to char
	void strrev(char*, int);//revers
	void outd();//out data
	void ind();
	void upr(char*, int n);//delete 000 before number to avoid 003, 004, 06

	extr() { mas[0] = '\0'; vlen = 0; si = ' '; }//constr 0

	extr(int a)//constr int
	{
		si = ' ';
		if (a < 0) { si = '-'; a = abs(a); }
		ltoa(a, mas);
		vlen = strlen(mas); mas[vlen] = '\0';
		strrev(mas, vlen - 1);
		upr(mas, vlen - 1);
	}

	extr(char* hoba)//constr char
	{
		strcpy_s(mas, hoba);
		vlen = strlen(mas);
		si = ' ';
	}

	extr(char* hoba, char zn)//numb+sign(constr char +char)
	{
		strcpy_s(mas, hoba);
		vlen = strlen(mas);
		si = zn;
	}

	extr(const extr& a)//copy constr
	{
		strcpy_s(mas, a.mas);
		vlen = a.vlen;
		si = a.si;
	}

	extr operator+(const extr);//operators
	extr operator*(const extr);
	extr operator-(const extr);
	extr operator/(const extr);
	void operator =(const extr);
};
void extr::ind()//input data
{
	cout << "введите число, если минус, поставьте" << endl;
	char temp[SZ];
	cin >> temp;
	int tvl = strlen(temp);
	if (temp[0] == '-')
	{
		si = '-';
		for (int t = 1; t < tvl; t++)
		{
			mas[t - 1] = temp[t];
		}
	}
	else strcpy_s(mas, temp);
	vlen = strlen(mas);
	mas[vlen] = '\0';
	strrev(mas, vlen - 1);
	upr(mas, vlen - 1);
}

void extr::outd() //output data
{
	char time[SZ];
	strcpy_s(time, mas);
	strrev(time, vlen - 1); cout << setw(45) << setiosflags(ios::right) << si << time << endl;
}

void extr::ltoa(int n, char* mas)
{
	int j = 0;
	while (n>0)
	{
		mas[j++] = (n % 10)+'0';
		n /= 10;
	}
	mas[j] = '\0';
	strrev(mas, j-1);
	vlen = j - 1;
}

void extr::strrev(char* mas, int n)
{
	for (int j = 0, vlen = n; j <= vlen / 2; j++, n--)
		swap(mas[j], mas[n]);
}

void extr::upr(char* mas, int n)
{
	int j = n;
	while (mas[j] == '0' && j > 0)
	{
		mas[j--] = '\0';
	}
}

void extr::operator=(const extr a)
{
	strcpy_s(mas, a.mas);
	vlen = a.vlen;
	si = a.si;
}

extr extr::operator+(const extr v)
{
	extr a = v;
	//action of signs
	if (si != '-' && v.si == '-') { a.si = ' '; return (*this - a); }
	if (si == '-' && v.si != '-') { si = ' '; return(a - *this); }
	char temp[SZ];
	int j = 0;
	int maxlen = (vlen > v.vlen) ? vlen : v.vlen;
	int sum;
	int carry = 0;
	for (j; j < maxlen; j++)
	{
		int d1 = (j < vlen) ? mas[j] - '0' : 0;//take one num
		int d2 = (j < v.vlen) ? v.mas[j] - '0' : 0;
		sum = d1 + d2 + carry;
		if (sum >= 10)
		{
			sum -= 10; carry = 1;
		}
		else
		{
			carry = 0;
		}
		temp[j] = sum + '0';
	}
	if (carry == 1)temp[j++] = carry + '0';
	temp[j] = '\0';
	int k = strlen(temp);
	upr(temp, k - 1);
	return extr(temp, si);
}

extr extr::operator-(const extr a)
{
	extr r = *this;
	extr t = a;
	char temp[SZ];//for return
	int razn = 0;//raznost
	int carry = 0;//perenos
	char zn;//sign
	int j;//counter
	bool por=1;//order of integ in operation
	if (r.si == '-' && t.si == '-')//returns by entered signes
	{
		t.si = ' '; return(r + t);
	}
	if (r.si == '-' && t.si == ' ')
	{
		t.si = '-'; return (r + t);
	}
	if (r.si == ' ' && t.si == '-')
	{
		t.si = ' '; return(r + t);
	}

	if (vlen > a.vlen)
	{
		por = 1;
	}
	else if (a.vlen > vlen)
	{
		por = 0;
	}
	else
	{
		for (j = vlen - 1; j >= 0; j--)//if vlens are equal
		{
			if (mas[j] - '0' != a.mas[j] - '0')
			{
				por = (mas[j] - '0' > a.mas[j] - '0') ? 1 : 0;
				break;
			}
		}
	}
	zn = (por) ? ' ' : '-';//sign

	int maxlen = (vlen > a.vlen) ? vlen : a.vlen;
	for (j = 0; j < maxlen; j++)//operation '-'
	{
		int d1 = (j < vlen) ? mas[j] - '0' : 0;
		int d2 = (j < a.vlen) ? a.mas[j] - '0' : 0;
		razn = (por) ? d1 - d2 - carry : d2 - d1 - carry;
		if (razn < 0) { razn += 10; carry = 1; }
		else carry = 0;
		temp[j] = razn + '0';
	}
	temp[j] = '\0';
	upr(temp, maxlen - 1);//delete '0' before num
	return extr(temp, zn);
}

extr extr::mult10(const extr d)
{
	char time[SZ];
	for (int j = d.vlen; j > 0; j--)
	{
		time[j] = d.mas[j - 1];
	}
	time[0] = '0';
	time[d.vlen + 1] = '\0';
	int v = strlen(time);
	return extr(time);
}

extr extr::mult(const int a)
{
	char tumn[SZ];
	int umn;
	int curry = 0;
	int perem;
	int j = 0;
	for (j; j < vlen; j++)
	{
		perem = mas[j] - '0';
		umn = perem * a + curry;
		if (umn >= 10)
		{
			curry = umn / 10;
			umn = umn % 10;
		}
		else { curry = 0; }
		tumn[j] = umn + '0';
	}
	if (curry != 0) { tumn[j++] = curry + '0'; }
	tumn[j] = '\0';
	return extr(tumn);
}

extr extr::operator*(const extr ap)
{
	extr umntec;
	extr tecsum;
	for (int j = 0; j < ap.vlen; j++)
	{
		int mnozh = ap.mas[j] - '0';
		umntec = mult(mnozh);
		for (int k = 0; k < j; k++)
		{
			umntec = mult10(umntec);
		}//shift per bit after some mult
		tecsum = tecsum + umntec;
	}
	tecsum.si = ((this->si == '-' && ap.si != '-') ||
		(this->si != '-' && ap.si == '-')) ? '-' : ' ';//set sign
	return tecsum;
}

extr extr::operator/(const extr ap)
{
	extr t = ap;//copy of ap
	t.si = ' ';//sign
	extr promezh = *this;//промеж сумма если остаток может быть
	promezh.si = ' ';

	int raz = 0;//сумма минусов(1,2,3деление)
	extr temp = promezh;//сумма без остатка
	char tem[SZ];//возвр.в конструкт результ.

	bool indik = 0;//выход из цикла
	if (vlen > ap.vlen)indik = 1;
	else if (vlen == ap.vlen)//for equal
	{
		for (int j = vlen - 1; j >= 0; j--)
		{
			if (this->mas[j] - '0' >= ap.mas[j] - '0') { indik = 1; break; }
		}
	}
	else indik = 0;
	char s;//sign
	while (indik)//minuses like a devide
	{
		promezh = promezh - t;

		//promezh.outd();
		if (promezh.si == '-') { indik = 0; }//quit because general number is end.save resting number
		else { temp = promezh; raz++; }//save prev.if it would be minus next time
		// temp.outd();
	}
	if ((si == '-' && ap.si != '-') || (si != '-' && ap.si == '-'))s = '-';
	else s = ' ';//sign numbers
	ltoa(raz, tem);//to text arr
	int v = strlen(tem);
	strrev(tem, v - 1);
	cout << "остаток: "; temp.outd();
	return extr(tem, s);
}

int main()
{
	setlocale(LC_ALL, "Russian");
	char ans = 'y';
	char op;
	while (ans == 'y')
	{
		extr ap;
		ap.ind();
		ap.outd();
		extr ap2;
		ap2.ind();
		ap2.outd();
		cout << "input operation:+, -, *, /" << endl;
		cin >> op;
		extr ap3;
		switch (op)
		{
		case '+': ap3 = ap + ap2; break;
		case '-':  ap3 = ap - ap2; break;
		case '*': ap3 = ap * ap2; break;
		case '/': ap3 = ap / ap2; break;
		default: break;
		}
		ap3.outd();
		cout << "else: y/n" << endl;
		cin >> ans;
	}
}