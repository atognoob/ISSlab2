#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>

#define Population_Size		2000				// размер популяции
#define Max_Iteration		100						// максимальное число итераций
#define Elitrate		0.10f						// элитарность
#define Mutationrate	0.25f						// мутации
#define GA_MUTATION		RAND_MAX * Mutationrate
#define GA_TARGET		string("WORLD")

using namespace std;

//Определение класса Individual для представления отдельного индивида в популяции.
class Individual			
{
public:
	string str;		// Хранит строку
	unsigned int fitness;	//Хранит значение функции приспособленности

	Individual() : fitness(0) {}	//Конструктор Individual(): Устанавливает значение fitness в 0 при создании индивида.
};

typedef vector<Individual> Gen_vector;
//Определение нового типа Gen_vector как вектор, содержащий объекты класса Individual.
//Теперь Gen_vector можно использовать для создания вектора индивидов в коде.

void init_population(Gen_vector& population1, Gen_vector& population2)	//инициализирует начальную популяцию индивидов.
{

	int tsize = GA_TARGET.size();
	// Проходимся по всей популяции population1
	for (int i = 0; i < Population_Size; i++) {
		Individual citizen;
		citizen.fitness = 0;
		citizen.str.erase();
		// Генерируем случайную строку для индивида citizen
		for (int j = 0; j < tsize; j++)
			citizen.str += (rand() % 90) + 32;
		// Добавляем индивида citizen в популяцию population1
		population1.push_back(citizen);
	}
	// Изменяем размер популяции population2, чтобы он соответствовал размеру population1
	population2.resize(Population_Size);
}

void calc_fitness(Gen_vector& population1)	//вычисляет пригодность (фитнес) для каждого индивида в популяции population1.
{
	string target = GA_TARGET;
	int tsize = target.size();
	unsigned int fitness;
	// Проходимся по всей популяции population1
	for (int i = 0; i < Population_Size; i++) {
		fitness = 0;
		// Вычисляем пригодность (фитнес) для каждого индивида
		for (int j = 0; j < tsize; j++) {
			// вычисляем пригодность индивида, сравнивая каждый символ его строки str 
			// с соответствующим символом целевой строки target и суммируя абсолютные разности.
			fitness += abs(int(population1[i].str[j] - target[j]));
		}
		// Устанавливаем вычисленное значение пригодности (фитнеса) для текущего индивида
		population1[i].fitness = fitness;
	}
}

bool fitness_sort(Individual population1, Individual population2)	//используется для сортировки индивидов по их пригодности (фитнесу).
{
	// Сравниваем пригодности (фитнес) двух индивидов
	return (population1.fitness < population2.fitness);
}

void sort_by_fitness(Gen_vector& population1)	//выполняет сортировку популяции индивидов population1 по их пригодности (фитнесу).
{
	// Сортирует популяцию индивидов по пригодности (фитнесу)
	sort(population1.begin(), population1.end(), fitness_sort);
	//Функция sort_by_fitness выполняет сортировку популяции population1 по возрастанию пригодности, так что индивид с наименьшей пригодностью будет находиться в начале популяции, 
	//а индивид с наибольшей пригодностью - в конце.
}

void elitism(Gen_vector& population1, Gen_vector& population2, int esize)	//выполняет сохранение наиболее приспособленных индивидов из популяции population1 в популяцию population2.
{
	//Параметр esize определяет количество индивидов, которые должны быть сохранены (элитарность).
	// Сохраняет наиболее приспособленные индивиды из population1 в population2
	for (int i = 0; i < esize; i++) {
		// Копируем строку и пригодность индивида из population1 в population2
		population2[i].str = population1[i].str;
		population2[i].fitness = population1[i].fitness;
	}
	//После выполнения функции population2 будет содержать наиболее приспособленные индивиды из population1.
}

void mutate(Individual& population)
{
	// Мутирует индивида, изменяя случайный символ в его строке
	int tsize = GA_TARGET.size();
	int x = rand() % tsize;		// Выбираем случайную позицию символа в строке
	int delta = (rand() % 90);		// Генерируем случайное число для изменения символа

	population.str[x] = ((population.str[x] + delta) % 122);
}

void mate(Gen_vector& population1, Gen_vector& population2)			// создает новое поколение индивидов путем скрещивания и мутации.
{
	// Создает новое поколение индивидов путем скрещивания и мутации
	int esize = Population_Size * Elitrate;			// Размер элитной части популяции
	int tsize = GA_TARGET.size(), x, i1, i2;		// Размер целевой строки

	elitism(population1, population2, esize);		// Копируем элитную часть в новую популяцию

	for (int i = esize; i < Population_Size; i++) {		//В цикле от esize до Population_Size, происходит скрещивание и мутация индивидов.
		i1 = rand() % (Population_Size / 2);		// Выбираем случайного индивида из первой половины популяции
		i2 = rand() % (Population_Size / 2);		// Выбираем случайного индивида из первой половины популяции
		x = rand() % tsize;		// Выбираем случайную позицию для скрещивания
		// Создаем новую строку для индивида путем комбинирования частей строк двух родителей
		population2[i].str = population1[i1].str.substr(0, x) + population1[i2].str.substr(x, esize - x);

		if (rand() < GA_MUTATION) mutate(population2[i]);		// Производим мутацию с некоторой вероятностью
	}
}

void print(Gen_vector& population)
{
	cout << "String: " << population[0].str << " \t Fitness : " << population[0].fitness << endl;
}

void swap(Gen_vector*& population1, Gen_vector*& population2)
{
	Gen_vector* temp = population1;
	population1 = population2;
	population2 = temp;
}

int main()
{
	srand((time(NULL)));

	Gen_vector pop_alpha, pop_beta;		// Создание двух популяций индивидов
	Gen_vector* population1, * population2;		// Указатели на текущую и следующую популяции

	init_population(pop_alpha, pop_beta);		// Инициализация начальной популяции
	population1 = &pop_alpha;		// Установка указателя на текущую популяцию
	population2 = &pop_beta;		// Установка указателя на следующую популяцию

	for (int i = 0; i < Max_Iteration; i++) {
		calc_fitness(*population1);		// Расчет фитнесс-значений для текущей популяции
		sort_by_fitness(*population1);	// Сортировка популяции по фитнесс-значениям
		print(*population1);	// Вывод информации о наилучшем индивиде текущей популяции

		if ((*population1)[0].fitness == 0) break;	// Если достигнуто решение, выходим из цикла

		mate(*population1, *population2);	// Создание новой популяции путем скрещивания и мутации
		swap(population1, population2);	// Обмен указателями на текущую и следующую популяции
	}

	return 0;
}
