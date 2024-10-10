#include <Not-Red/Inc/NotRed.h>

using namespace NotRed;
using namespace NotRed::Core;

static int sUniqueId = 0;

class Student
{
public:
	Student(const std::string& name)
		: mName(name)
		, mId(++sUniqueId)
	{

	}
	Student(const std::string& name, int id)
		: mName(name)
		, mId(id)
	{

	}
private:
	int mId = 0;
	std::string mName;
};

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 200);

	std::vector<Student*> students;
	for (uint32_t i = 0; i < 100; ++i)
	{
		Student* newStudent = studentPool.New("Student_" + std::to_string(i), i);
		students.push_back(newStudent);
	}

	for (uint32_t i = 0; i < 20; ++i)
	{
		auto& student = students.back();
		studentPool.Delete(student);
		students.pop_back();
	}

	for (uint32_t i = 0; i < 100; ++i)
	{
		Student* student = studentPool.New("Students_New" + std::to_string(i), i);
		students.push_back(student);
	}

	for (auto& student : students)
	{
		studentPool.Delete(student);
	}

	students.clear();

	return 0;
}