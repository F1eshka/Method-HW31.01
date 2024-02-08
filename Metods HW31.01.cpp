#include <iostream>
#include <ctime>
using namespace std;

class String
{
	char* text = nullptr;
	unsigned int length = 0;
	unsigned int capacity = 80;

public:
	String() : String("", 80) {}

	String(const char* text) : String(text, 80) {}

	String(unsigned int capacity) : String("", capacity) {}

	String(const String& original) : String(original.text, original.capacity) {}

	String(const char* text, unsigned int capacity)
	{
		SetString(text, capacity);
	}

private:

	void SetString(const char* text, unsigned int capacity = 80)
	{
		length = strlen(text);

		if (length == 0)
		{
			this->capacity = 80;
		}
		else 
		{
			if (capacity > length * 10)
				capacity = length * 10;

			if (capacity <= length)
				capacity = length + 1;

			this->capacity = capacity;
		}

		/*cout << "\n\n=================\n\n";
		cout << "capacity param: " << this->capacity << "\n";
		cout << "length param: " << this->length << "\n";
		cout << "\n\n=================\n\n";*/

		if (this->text != nullptr)
			delete[] this->text;

		this->text = new char[this->capacity];
		strcpy_s(this->text, length + 1, text);
	}

public:

	void Print()
	{
		cout << text << "\n";
	}

	~String()
	{
		if (text != nullptr)
		{
			delete[] text;
			text = nullptr;
		}
	}

	// возвращать из класса указатель на массив чаров нельзя - это нарушит инкапсуляцию и позволит клиенту СЛОМАТЬ содержимое (состояние) объекта
	// если создавать копию массива чаров, то под неё придётся выделять память, а почистить её клиент скорее всего ЗАБУДЕТ 
	// поэтому здесь выдаётся КОПИЯ состояния в виде объекта текущего класса
	
	String GetString() const
	{
		//char* copy = new char[length + 1];
		//strcpy_s(copy, length + 1, text);
		//return copy;

		String copy = text;
		return copy;
	}

	int GetLength() const
	{
		return length;
	}

	// метода SetLength не должно быть!

	int GetCapacity() const
	{
		return capacity;
	}

	// в целях повшения производительности, не тратится время на:
	// 1) очистку памяти делитом
	// 2) запись в каждый символ пробелов (не запускается цикл фор)
	// 3) не меняется capacity

	// достаточно в начало строки записать \0, и длину выставить в 0
	// многие методы класса ориентируются именно на длину, и если она равна 0, то клиент будет уверен, что строка пустая
	
	void Clear()
	{
		text[0] = '\0';
		length = 0;
	}

	// если резерв памяти стал значительно больше, чем размер строки, то можно вызвать принудительное перевыделение памяти, суть которого в 
	void ShrinkToFit() // том, чтобы сделать резерв такой же длины, как и length
	{
		if (length + 1 <= capacity)
		{
			return;
		}

		capacity = length + 1;
		char* temp = new char[capacity];
		strcpy_s(temp, capacity, text);
		delete[] text;
		text = temp;
	}

	void Print() const
	{
		cout << "Text: " << text << "\n";
		cout << "Length: " << length << "\n";
		cout << "Capacity: " << capacity << "\n";
	}
	void FromKeybord()
	{
		cout << "Enter text: ";
		char txt[100];
		cin.getline(txt, sizeof(txt));
		SetString(txt);
	}
	bool CompareTo(String& str) //Сравнение строки с лругой
	{
		for (int i = 0; i < strlen(text); i++)
		{
			if (text[i] == str.text[i])
			{
				cout << "true" << "\n";
				return true;
			}
			else
			{
				cout << "false" << "\n";
				return false;
			}

		}
	}

	bool Contains(String& txt)
	{
		return strstr(text, txt.text) != nullptr;
	}

	void Concat(String& txt)
	{
		int end = length;
		int start = 0;
		length += txt.length;
		ShrinkToFit();
		for (int i = end; i < length; i++)
		{
			text[i] = txt.text[start];
			start++;
		}
		text[length] = '\0';
	}

	bool EndWith(String& txt) //Заканчивается ли строка нужным текстом
	{
		if (txt.length > length)
			return false;

		return strcmp(text + length - txt.length, txt.text) == 0;
	}

	bool Startwith(String& txt) //Начинается ли с нужного текста
	{
		if (txt.length > length)
			return false;
		return strncmp(text, txt.text, txt.length) == 0;
	}

	int IndexOf(char target) //Есть ли нужный символ и возвращаем его индекс
	{
		for (int i = 0; i < length; ++i)
		{
			if (text[i] == target)
			{
				return i;
			}
		}
		return -1;
	}
	
	int LastIndexOf(char target) //Возвращаем последний индекс нужного символа
	{
		for (int i = length - 1; i >= 0; --i)
		{
			if (text[i] == target)
			{
				return i;
			}
		}
		return -1;
	}

	void Remove(int start) //Удаление всех символов до конца начинаем с переданного индекса (с прроверкой корректности индекса)
	{
		if (start >= 0 && start < length)
		{
			memmove(text + start, text + start + 1, length - start);
			length--;
			text[length] = '\0';
		}
	}
	 
	void Replce() //Меняет все "О" на "D"
	{
		char oldsymbol = 'o';
		char newsymbol = 'd';
		for (unsigned int i = 0; i < length; ++i)
		{
			if (text[i] == oldsymbol)
			{
				text[i] = newsymbol;
			}
		}
		cout << text << "\n";
	}

	String ToLower() //Привод к верхнему регистру (в том числе и для кириллического текста)
	{
		String lowerString = String(text, capacity);
		for (int i = 0; i < length; i++)
		{
			if (lowerString.text[i] >= 65 and lowerString.text[i] <= 90)
			{
				lowerString.text[i] = char((int)lowerString.text[i] + 32);
			}
			/*else if (lowerString.text[i] >= -112 and lowerString.text[i] <= -80)
			{
				cout << "блабла";
				lowerString.text[i] = char((int)lowerString.text[i] + 32);
			}*/
		}
		return lowerString;
	}
	String ToUpper() //Привод к нижнему регистру (в том числе и для кириллического текста)
	{
		String upperString = String(text, capacity);
		for (int i = 0; i < length; i++)
		{
			if (upperString.text[i] >= 97 and upperString.text[i] <= 122)
			{
				upperString.text[i] = char((int)upperString.text[i] - 32);
			}
		}
		return upperString;
	}
	void Reverse()
	{
		char temp;
		int end = length - 1;
		for (int i = 0; i < length / 2; i++)
		{
			temp = text[end];
			text[end] = text[i];
			text[i] = temp;
			end--;
		}
	}
	void SortAZ() //Сортировка символов от А до Я
	{
		int temp;
		int i = 0;
		int j = 1;
		int countOfCoincidence = 0;
		while (countOfCoincidence < length)
		{
			if (text[i] > text[j] and j < length)
			{
				temp = text[i];
				text[i] = text[j];
				text[j] = temp;
				i++;
				j++;
				countOfCoincidence = 0;
			}
			else if (j == length)
			{
				i = 0;
				j = 1;
			}
			else
			{
				i++;
				j++;
				countOfCoincidence++;
			}
		}
	}
	void SortZA() //Сортировка от Я до А
	{
		int temp;
		int i = 0;
		int j = 1;
		int countOfCoincidence = 0;
		while (countOfCoincidence < length)
		{
			if (text[i] < text[j] and j < length)
			{
				temp = text[i];
				text[i] = text[j];
				text[j] = temp;
				i++;
				j++;
				countOfCoincidence = 0;
			}
			else if (j == length)
			{
				i = 0;
				j = 1;
			}
			else
			{
				i++;
				j++;
				countOfCoincidence++;
			}
		}
	}
	void Shuffle() //Случайное перемещение символов
	{
		for (int i = 0; i < length; i++)
		{
			int x = rand() % length;
			int y = rand() % length;
			int temp;
			temp = text[x];
			text[x] = text[y];
			text[y] = temp;
		}
	}
	void RandomFill() //Заполнение случайными символами (на весть капасити)
	{
		for (int f = 0; f < length; f++)
		{
			text[f] = rand() % length;
			text[f] = 'a' + rand() % 26;
			cout << text[f];
		}

	}
};

int main()
{
	setlocale(LC_ALL, "");
	srand(time(NULL));
	//String a;
	//a.Print();

	//String b = "Alex"; // String b("Alex");
	//b.Print();

	//String c(120); // TO DO: учесть пожелание клиента, если он хочет сделать резерв больше чем 80.
	//c.Print();// как вариант, при передаче числа можно его перевести в строку, "120"
	//
	String d("Sasha", 30);
	String b("Sasha", 20);
	char character = 'A';
	char oldChar = 'a';
	char newChar = 'b';
	int index = 3;
	int start = 3;
	int count = 2;

	d.RandomFill();
	d.Print();
}