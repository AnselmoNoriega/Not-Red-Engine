#include <Not-Red/Inc/NotRed.h>

using namespace NotRed;
using namespace NotRed::Core;

static int sUniqueId = 0;

class Student
{
public:
	Student(const std::string& name)
		: mName(name)
		, mId(++sUniqueId) { }

	Student(const std::string& name, int id)
		: mName(name)
		, mId(id) { }

private:
	int mId = 0;
	std::string mName;
};

class Dog
{
public:
	Dog(const std::string& breed)
		: mBreed(breed)
		, mId(++sUniqueId) { }

	Dog(const std::string& breed, int id)
		: mBreed(breed)
		, mId(id) { }

private:
	int mId = 0;
	std::string mBreed;
};

class Cat
{
public:
	Cat(const std::string& toyName)
		: mToyName(toyName)
		, mId(++sUniqueId) { }

	Cat(const std::string& toyName, int id)
		: mToyName(toyName)
		, mId(id) { }

private:
	int mId = 0;
	std::string mToyName;
};

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 200);
	TypedAllocator dogPool = TypedAllocator<Dog>("DogPool", 50);
	TypedAllocator catPool = TypedAllocator<Cat>("CatPool", 70);

	std::vector<Student*> students;
	std::vector<Dog*> dogs;
	std::vector<Cat*> cats;

	{
		for (uint32_t i = 0; i < 140; ++i)
		{
			Student* newStudent = studentPool.New("Student_" + std::to_string(i), i);
			students.push_back(newStudent);
		}

		for (uint32_t i = 0; i < 34; ++i)
		{
			Dog* newDog = dogPool.New("GermanShepherd_" + std::to_string(i), i);
			dogs.push_back(newDog);
		}

		for (uint32_t i = 0; i < 67; ++i)
		{
			Cat* newCats = catPool.New("CatToy_" + std::to_string(i), i);
			cats.push_back(newCats);
		}
	}

	{
		for (uint32_t i = 0; i < 101; ++i)
		{
			auto& student = students.back();
			studentPool.Delete(student);
			students.pop_back();
		}

		for (uint32_t i = 0; i < 21; ++i)
		{
			auto& cat = cats.back();
			catPool.Delete(cat);
			cats.pop_back();
		}

		for (uint32_t i = 0; i < 17; ++i)
		{
			auto& dog = dogs.back();
			dogPool.Delete(dog);
			dogs.pop_back();
		}
	}

	{
		for (auto& student : students)
		{
			studentPool.Delete(student);
		}

		for (auto& cat : cats)
		{
			catPool.Delete(cat);
		}

		for (auto& dog : dogs)
		{
			dogPool.Delete(dog);
		}
	}

	students.clear();
	dogs.clear();
	cats.clear();

	return 0;
}