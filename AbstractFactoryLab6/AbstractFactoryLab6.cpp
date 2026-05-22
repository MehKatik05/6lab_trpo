//Разработайте иерархию классов Суп →(Борщ, Тыквенный суп),
//ВтороеБлюдо →(ЖаренаяКартошка, Котлета).При помощи паттерна AbstractFactory
//разработайте классы Диета →(МяснаяДиета, ВегетарианскаяДиета).Продемонстрируйте
//работу паттерна.
#include <iostream>

using namespace std;

//Абстрактный суп
class Soup {
public:
    virtual ~Soup() {}
    virtual void serve() const = 0;
    virtual int getCalories() const = 0;
};

//Борщ
class Borscht : public Soup {
public:
    void serve() const override {
        cout << "Подаётся борщ." << std::endl;
    }
    int getCalories() const override { return 150; }
};

//Тыквенный суп
class PumpkinSoup : public Soup {
public:
    void serve() const override {
        cout << "Подаётся тыквенный суп." << std::endl;
    }
    int getCalories() const override { return 120; }
};

//Вторые блюда
class SecondCourse {
public:
    virtual ~SecondCourse() {}
    virtual void serve() const = 0;
    virtual int getCalories() const = 0;
};

//Жареная картошка
class FriedPotatoes : public SecondCourse {
public:
    void serve() const override {
        cout << "Подаётся жареная картошка." << endl;
    }
    int getCalories() const override { return 300; }
};

//Котлета
class Cutlet : public SecondCourse {
public:
    void serve() const override {
        cout << "Подаётся котлета." << endl;
    }
    int getCalories() const override { return 250; }
};

class Diet {
public:
    virtual ~Diet() {}
    virtual Soup* createSoup() const = 0;
    virtual SecondCourse* createSecondCourse() const = 0;

    void printMenu() const {
        Soup* soup = createSoup();
        SecondCourse* sec_course = createSecondCourse();

        cout << "Калории: " << soup->getCalories() + sec_course->getCalories() << endl;
        cout << "Меню: ";
        soup->serve();
        cout << ", ";
        sec_course->serve();
        cout << endl;

        delete soup;
        delete sec_course;
    }
};

//Мясная диета
class MeatDiet : public Diet {
public:
    Soup* createSoup() const override {
        return new Borscht(); //борщ
    }
    SecondCourse* createSecondCourse() const override {
        return new Cutlet(); //котлета
    }
};

//Вегетарианская диета
class VegetarianDiet : public Diet {
public:
    Soup* createSoup() const override {
        return new PumpkinSoup(); //тыквенный суп
    }
    SecondCourse* createSecondCourse() const override {
        return new FriedPotatoes(); //жареная картошка
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    MeatDiet meat;
    VegetarianDiet veg;

    cout << "Мясная диета:" << endl;
    meat.printMenu();

    cout << "\nВегетарианская диета:" << endl;
    veg.printMenu();

    return 0;
}