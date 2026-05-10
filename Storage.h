#pragma once
#include<iostream>
using namespace std;

template<class T>
class Storage
{
private:
	int count;
	T data[100];
public:
	Storage()
	{
		count = 0;
	}
	void Add(T obj)
	{
		if (count < 100)
		{
			data[count] = obj;
			count++;
		}
		else
			cout << "Invalid!Can't add more objects,Storage is full.";
	}
	void RemoveByID(int id)
	{
		for (int i = 0; i < count; i++)
		{
			if (id == data[i].getID())
			{
				for (int j = i; j < count - 1; j++)
				{
					data[j] = data[j + 1];
				}
				count--;
				break;
			}
		}
	}
	T* findByID(int id)
	{
		for (int i = 0; i < count; i++)
		{
			if (id == data[i].getID())
			{
				return &data[i];
			}
		}
			return nullptr;
	}

	T* getAll()
	{
		return data;
	}
	int getSize()
	{
		return count;
	}

	T& operator[](int index) {
		return data[index];
	}
};