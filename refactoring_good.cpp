/**
 * Исходная программа
Пример программы очень прост. Она рассчитывает и выводит отчет об оплате клиентом услуг в магазине
видеопроката. Программе сообщается, какие фильмы брал в прокате клиент и на какой срок. После этого она
рассчитывает сумму платежа исходя из продолжительности проката и типа фильма. Фильмы бывают трех
типов: обычные, детские и новинки. Помимо расчета суммы оплаты начисляются бонусы в зависимости от
того, является ли фильм новым.

Movie 			<- Rental 			<- Customer
int priceCode		int DaysRented		statement()

*/

#include <iostream>
#include <vector>
#include <gtest/gtest.h>

//Movie - класс, который представляет данные о фильме.
class Movie
{
public:
	static const int REGULAR;
	static const int NEW_RELEASE;
	static const int CHLDREN;
	Movie() {};
	Movie(const std::string & _title, int _priceCode) : 
		title(_title), priceCode(_priceCode) 
	{}
	void operator=(const Movie& value){
		title = value.title;
		priceCode = value.priceCode;
	}
	void setPriceCode(int value) { priceCode = value; }
	int getPriceCode() const { return priceCode; }
	std::string getTitle() const { return title; }
private:
	std::string title{};
	int priceCode{0};
};

const int Movie::REGULAR = 0;
const int Movie::NEW_RELEASE = 1;
const int Movie::CHLDREN = 2;

//Rental - класс, представляющий данные о прокате фильма.
class Rental
{
public:
	Rental(const Movie & _movie, int _daysRented) :
		movie(_movie), daysRented(_daysRented)
	{}
	Movie getMovie() const { return movie; }

	int getDaysRented() const { return daysRented; }

	double getCharge() {
		double result = 0;
		// Определить сумму для каждой строки.
		switch (getMovie().getPriceCode()) {
		case Movie::REGULAR:
			result += 2;
			if (getDaysRented() > 2)
				result += (getDaysRented() - 2) * 1.5;
			break;
		case Movie::NEW_RELEASE:
			result += getDaysRented() * 3;
			break;
		case Movie::CHLDREN:
			result += 1.5;
			if (getDaysRented() > 3)
				result += (getDaysRented() - 3) * 1.5;
				break;
		}
		return result;
	}
	int getFrequentRenterPoints() {
		// Бонус за аренду новинки на два дня
		if ((getMovie().getPriceCode() == Movie::NEW_RELEASE) && getDaysRented() > 1)
			return 2;
		else
			return 1;
	}
private:
	Movie movie;
	int daysRented {0};
};

//Customer - класс, представляющий клиента магазина. Как и предыдущие классы, он содержит данные и
//методы для доступа к ним:
class Customer
{
private:
	std::string name{};
	std::vector<Rental> rentals;
public:
	explicit Customer(std::string _name) : name(_name)
	{}
	void addRentals(const Rental & rent)
	{
		rentals.push_back(std::move(rent));
	}
	std::string getName () const
	{
		return name;
	}
	std::string Statement()
	{
		// Сначала мы объявляем локальные переменные.
		std::string result = "Учет аренды для : " + name;
		// Затем для каждого клиента мы рассчитываем задолженность...
		for (auto& rent : rentals) {
			// Показать результаты для этой аренды
			result += "\t" + rent.getMovie().getTitle() + "\t" + std::to_string(rent.getCharge()) + "\n";
		}

		// Добавить нижний колонтитул
		result += "Сумма задолженности составляет " + std::to_string(getTotalCharge()) + "\n";
		result += "Вы заработали " + std::to_string(getTotalFrequentRenterPoints()) + " очков ";
		return result;
	}
private:
	double getTotalCharge() {
		double result = 0;
		for (auto& rent : rentals) {
			result += rent.getCharge();
		}
		return result;
	}
	int getTotalFrequentRenterPoints(){
		int result = 0;
		for (auto& rent : rentals) {
			// Добавить очки для активного арендатора.
			result += rent.getFrequentRenterPoints();
		}
		return result;
	}
};


const std::string_view test1 = "Учет аренды для : Ivan\tfilm1\t2.000000\nСумма задолженности составляет 2.000000\nВы заработали 1 очков "; 
const std::string_view test2 = "Учет аренды для : Alex\tfilm2\t9.000000\nСумма задолженности составляет 9.000000\nВы заработали 2 очков "; 
const std::string_view test3 = "Учет аренды для : Anna\tfilm3\t3.000000\nСумма задолженности составляет 3.000000\nВы заработали 1 очков "; 


class StatementTest: public ::testing::Test
{
protected:

    void Setup ( const std::string & name )
    {
        person = new Customer(name);

    }

    void TearDown() override
    {
        delete person;
    }

    Customer* person{nullptr};
};

TEST_F(StatementTest, Test_for_Ivan_Regular_2_days)
{
	Setup("Ivan");
	person->addRentals(Rental(Movie("film1", Movie::REGULAR), 2));
	std::cout << "\n------------------------------------\n";
	std::cout << person->Statement() << "\n";
	std::cout << "------------------------------------\n\n";
	EXPECT_EQ(true, person->Statement() == test1);
}

TEST_F(StatementTest, Test_for_Alex_Release_3_days)
{
	Setup("Alex");
	person->addRentals(Rental(Movie("film2", Movie::NEW_RELEASE), 3));
	std::cout << "\n------------------------------------\n";
	std::cout << person->Statement() << "\n";
	std::cout << "------------------------------------\n\n";
	EXPECT_EQ(true, person->Statement() == test2);
}

TEST_F(StatementTest, Test_for_Anna_Children_4_days)
{
	Setup("Anna");
	person->addRentals(Rental(Movie("film3", Movie::CHLDREN), 4));
	std::cout << "\n------------------------------------\n";
	std::cout << person->Statement() << "\n";
	std::cout << "------------------------------------\n\n";
	EXPECT_EQ(true, person->Statement() == test3);
}