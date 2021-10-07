/******************************************************************************/
/*!
\file  TypeErasedObj.cpp

\author ONG, Zhi Hao Jaryl
\par    email: o.zhihaojaryl\@digipen.edu
\par    DigiPen login: o.zhihaojaryl

\brief
This file contains function and structs definition for TypeErasedObj class

All content © 2018 DigiPen (SINGAPORE) Corporation, all rights
reserved.
*/
/******************************************************************************/
#include <utility>
#include <any>

class TypeErasedObj
{
private:
	struct Model
	{
		virtual Model* clone() = 0;
		virtual std::any GetValue() = 0;
		~Model() {}
	};

	template<typename T>
	struct Instance : Model
	{
		Instance(const T& t) :obj(t) {}
		Instance<T>* clone()
		{
			return new Instance(obj);
		}

		std::any GetValue()
		{
			return std::any{ &obj };
		}

		T obj;
	};

public:

	TypeErasedObj()
		:ptr{}
	{
		
	}

	std::any GetValue()
	{
		return ptr->GetValue();
	}

	template<typename T>
	TypeErasedObj(const T& t) :
		ptr(new Instance<T>(t))
	{
	}

	template<typename T, typename U = typename std::enable_if<
										!std::is_same<typename std::decay<T>::type, TypeErasedObj>::value
															>::type>
		TypeErasedObj(T&& t)
		: ptr(new Instance<T>(std::forward<T>(t)))
	{
	}

	TypeErasedObj(const TypeErasedObj& rhs)
		: ptr(rhs.ptr->clone())
	{

	}
	TypeErasedObj& operator=(const TypeErasedObj& rhs)
	{
		Model *tmp(rhs.ptr->clone());
		delete ptr;
		ptr = tmp;
		return *this;
	}

	~TypeErasedObj() { delete ptr; }
	Model *ptr;
};


