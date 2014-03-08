#include <iostream>
#include <string>
#include <algorithm>
#include <locale>
#include <clocale>
#include <cwctype>
#include <map>
#include <regex>
using namespace std;

//Для русского Метафона
const wstring alf = L"ОЕАИУЭЮЯПСТРКЛМНБВГДЖЗЙФХЦЧШЩЫЁ";//алфавит кроме исключаемых букв
const wstring zvonkie = L"БЗДВГ";//звонкие
const wstring gluhie = L"ПСТФК";//глухие
const wstring soglasnie = L"ПСТКБВГДЖЗФХЦЧШЩ";//согласные, перед которыми звонкие оглушаются
const wstring glasnie = L"ОЮЕЭЯЁЫ";//образец гласных
const wstring ct = L"АУИИАИИ";// замена гласных
const map<wstring, wstring> suffixMap = 
{
	{ L"ОВСКИЙ", L"@" },
	{ L"ЕВСКИЙ", L"#" },
	{ L"ОВСКАЯ", L"$" },
	{ L"ЕВСКАЯ", L"%" },
	{ L"ИЕВА", L"9" },
	{ L"ЕЕВА", L"9" },
	{ L"ОВА", L"9" },
	{ L"ЕВА", L"9" },
	{ L"ИНА", L"1" },
	{ L"ИЕВ", L"4" },
	{ L"ЕЕВ", L"4" },
	{ L"НКО", L"3" },
	{ L"ОВ", L"4" },
	{ L"ОФ", L"4" },
	{ L"ЕВ", L"4" },
	{ L"АЯ", L"6" },
	{ L"ИЙ", L"7" },
	{ L"ЫЙ", L"7" },
	{ L"ЫХ", L"5" },
	{ L"ИХ", L"5" },
	{ L"ИН", L"8" },
	{ L"ИК", L"2" },
	{ L"ЕК", L"2" },
	{ L"УК", L"0" },
	{ L"ЮК", L"0" }
};

bool EndsWith(wstring check, wstring find)
{
	bool include = true;
	int j = check.size();
	for (int i = find.size() - 1; i >= 0; i--)
	{
		if (check[--j] != find[i])
		{
			include = false;
			break;
		}
	}
	return include;
}
wstring MetaphoneRu(const wstring ww)
{
	wstring w = ww;
	//в верхний регистр
	for (int i = 0; i < w.size(); i++)
		w[i] = std::towupper(w[i]);
	wstring sb = L" ";
	//оставили только символы из alf
	for (int i = 0; i < w.size(); i++)
	{
		if (alf.find(w[i]) != wstring::npos) sb += w[i];
	}
	wstring s = sb;
	//компрессия окончаний
	for (auto item = suffixMap.begin(); item != suffixMap.end(); item++)
	{
		if (!EndsWith(s, item->first)) continue;
		s = std::tr1::regex_replace(s, std::tr1::wregex(item->first + L"$"), item->second);
	}
	//Оглушаем последний символ, если он - звонкий согласный
	int idx = zvonkie.find(s[s.size() - 1]);
	if (idx != wstring::npos) s[s.size() - 1] = gluhie[idx];
	wchar_t old_c = L' ';
	wstring V = L"";
	for (int i = 0; i < s.size(); i++)
	{
		wchar_t c = s[i];
		int B = glasnie.find(c);
		if (B != wstring::npos)
		{//гласная
			if (old_c == L'Й' || old_c == L'И')
			{
				if (c == L'О' || c == L'Е')//'Буквосочетания с гласной
				{
					old_c = L'И';
					V[V.size() - 1] = old_c;
				}
				else//Если не буквосочетания с гласной, а просто гласная
				{
					if (c != old_c) V = V + ct[B];
				}
			}
			else//Если не буквосочетания с гласной, а просто гласная
			{
				if (c != old_c) V = V + ct[B];
			}
		}
		else
		{//согласная
			if (c != old_c)//для «Аввакумов»
			{
				if (soglasnie.find(c) != wstring::npos)
				{ // 'Оглушение согласных
					B = zvonkie.find(old_c);
					if (B != wstring::npos)
					{
						old_c = gluhie[B];
						V[V.size() - 1] = old_c;
					}
				}
				if (c != old_c) V = V + c;//для «Шмидт»
			}
		}
		old_c = c;
	}
	return V;
}

//Deletes the first word from a string
wstring delfirstword(wstring s)
{
	wstring sout;
	int i = 0, i2 = 0;

	if (s == L"") return s;
	while (isspace(s[i])) i++;
	while ((!isspace(s[i])) && (s[i] != (char)NULL)) i++;
	if (s[i] == L'\0') return L"";
	while ((isspace(s[i])) && (s[i] != L'\0')) i++;
	while (s[i] != L'\0')
	{
		sout += s[i];
		i++;
	}

	return sout;
}
//Returns the soundex equivalent to In 
wstring  Soundex(wstring In)
{
	int Iin(1);
	wchar_t C, PrevDig;
	wstring Out;

	Out += towlower(In[0]);
	PrevDig = Out[0];

	while ((In[Iin] != L'\0') && (Out.size() <= 4))
	{
		In[Iin] = towlower(In[Iin]);
		switch (In[Iin])
		{
		case L'b': C = L'1'; break;
		case L'p': C = L'1'; break;
		case L'f': C = L'1'; break;
		case L'v': C = L'1'; break;
		case L'c': C = L'2'; break;
		case L's': C = L'2'; break;
		case L'k': C = L'2'; break;
		case L'g': C = L'2'; break;
		case L'j': C = L'2'; break;
		case L'q': C = L'2'; break;
		case L'x': C = L'2'; break;
		case L'z': C = L'2'; break;
		case L'd': C = L'3'; break;
		case L't': C = L'3'; break;
		case L'l': C = L'4'; break;
		case L'm': C = L'5'; break;
		case L'n': C = L'5'; break;
		case L'r': C = L'6'; break;
		default: C = L'*';
		}
		if ((C != PrevDig) && (C != L'*'))
		{
			Out += C;
			PrevDig = C;
		}
		Iin++;
	}

	while (Out.size() < 4)
		Out += L'0';

	return Out;
}

//Levenshteyn
double LevenComparison(const wstring st1, const wstring st2)
{
	wstring st1_ = L' ' + st1;
	wstring st2_ = L' ' + st2;
	const int s1_size = st1_.size();
	const int s2_size = st2_.size();
	const int max_size = max(s1_size, s2_size);

	int **a = new int *[s1_size];
	for (int i = 0; i < s1_size; i++)
		a[i] = new int[s2_size];

	double penalty = 0; // штраф непохожести... меньше обычного штрафа
	int i, j;
	a[0][0] = 0;
	for (i = 1; i < s1_size; i++)
		a[i][0] = i;
	for (i = 1; i < s2_size; i++)
		a[0][i] = i;

	for (i = 1; i < s1_size; i++)
	for (j = 1; j < s2_size; j++)
	{
		if (st1_[i] == st2_[j]) a[i][j] = a[i - 1][j - 1];
		else
		{
			if ((towlower(st1_[i])) == towlower(st2_[j]))
			{
				a[i][j] = a[i - 1][j - 1];
				penalty += 1. / (max_size - 1);
			}
			else
				a[i][j] = min(a[i - 1][j - 1], min(a[i - 1][j], a[i][j - 1])) + 1;
			if (i > 1 && j > 1 && st1_[i] == st2_[j - 1] && st1_[i - 1] == st2_[j])
			{
				a[i][j] = min(a[i][j], a[i - 2][j - 2] + 1);
			}
		}

	}
	int diff = a[s1_size - 1][s2_size - 1];
	for (i = 0; i < s1_size; i++)
		delete[] a[i];
	delete[] a;
	return double((max_size - 1) - penalty - diff) / (max_size - 1); // "-1' because ' ' 
}

double GetComparison(const wstring st1_, const wstring st2_)
{
	bool RUString1 = true, RUString2 = true;
	for(int i = 0; i < st1_.size(); i++)
	{
		if (st1_[i] > 64 && st1_[i] < 123)
		{
			RUString1 = false;
			break;
		}
			
	}
	for (int i = 0; i < st2_.size(); i++)
	{
		if (st2_[i] > 64 && st2_[i] < 123)
		{
			RUString2 = false;
			break;
		}
	}
		
	if (RUString1 && RUString2)
	{
		if (MetaphoneRu(st1_) == MetaphoneRu(st2_))
		{
			const double CoefMetaphone = 0.8;
			return 1 - ((1 - LevenComparison(st1_, st2_)) * CoefMetaphone);
		}
	}
	if (!RUString1 && !RUString2)
	{
		if (Soundex(st1_) == Soundex(st2_))
		{
			const double Coefsoundex = 0.8;
			return 1 - ((1 - LevenComparison(st1_, st2_)) * Coefsoundex);
		}
	}
	return LevenComparison(st1_, st2_);
}
int main()
{
	setlocale(LC_ALL, "");
	char k;
	do 
	{
		wstring ws1, ws2;

		wcin.imbue(locale("rus_rus.866"));
		cout << "Enter 1 string: ";
		getline(std::wcin, ws1);
		cout << "Enter 2 string: ";
		getline(std::wcin, ws2);

		wcout << ws1 << L" and " << ws2 << endl;
		cout << "1 and 2: " "Comparison: " << GetComparison(ws1, ws2) << endl << endl;

		cout << "enter 1 to repeat: ";
		k = cin.get();
		cin.clear();
		cin.sync();
		cout << endl;
	} while (k == '1');
}




